#include "Frame.h"
#include "FrameVersion.h"
#include <algorithm>
#include <climits>
#include <new>



// Link namespaces.
using namespace std;
using namespace cr::video;



namespace
{

/// Size of the header of serialized data (bytes): format version (2), width,
/// height, FOURCC, data size, frame ID and source ID (4 bytes each).
constexpr int SERIALIZED_HEADER_SIZE{26};

/// Version of the serialization format. It is not the version of the library:
/// it changes only if the layout of the serialized data changes. All versions
/// 5.0.x and 5.1.x of the library read and write format 5.0.
constexpr uint8_t SERIALIZED_FORMAT_MAJOR{5};
constexpr uint8_t SERIALIZED_FORMAT_MINOR{0};

// The serialized data stores the integer fields with 4 bytes each.
static_assert(sizeof(int) == 4 && sizeof(uint32_t) == 4,
              "The serialization format needs 4-byte integers");



/**
 * @brief Calculate the size of the frame data (bytes) according to the pixel
 * format.
 * @param fourcc FOURCC code of data format.
 * @param width Frame width (pixels).
 * @param height Frame height (pixels).
 * @return Frame data size or -1 if the pixel format is unknown, the width or
 * the height is negative or the size does not fit in an int.
 */
int64_t calculateDataSize(Fourcc fourcc, int width, int height)
{
    if (width < 0 || height < 0)
        return -1;

    // Every pixel format needs at least one byte per pixel.
    const int64_t w = width;
    const int64_t h = height;
    if (w * h > INT_MAX)
        return -1;

    int64_t dataSize = 0;
    switch (fourcc)
    {
    case Fourcc::BGR24:
    case Fourcc::RGB24:
    case Fourcc::YUV24:
        dataSize = w * h * 3;
        break;
    case Fourcc::NV12:
    case Fourcc::NV21:
    case Fourcc::YU12:
    case Fourcc::YV12:
        dataSize = w * (h + h / 2);
        break;
    case Fourcc::YUYV:
    case Fourcc::UYVY:
        dataSize = w * h * 2;
        break;
    case Fourcc::JPEG:
    case Fourcc::H264:
    case Fourcc::HEVC:
        dataSize = w * h * 4;
        break;
    case Fourcc::GRAY:
        dataSize = w * h;
        break;
    default:
        return -1;
    }

    return (dataSize > INT_MAX) ? -1 : dataSize;
}

}



string Frame::getVersion()
{
    return FRAME_VERSION;
}



Frame::Frame()
{

}



Frame::Frame(int _width,
             int _height,
             Fourcc _fourcc,
             int _size,
             uint8_t* _data)
{
    // Check frame size.
    if (_width == 0 || _height == 0)
    {
        _width = 0;
        _height = 0;
    }

    // Calculate frame data size according to pixel format. A frame with an
    // unknown pixel format, a negative size or a size that does not fit in an
    // int stays empty.
    const int64_t dataSize = calculateDataSize(_fourcc, _width, _height);
    if (dataSize < 0)
        return;
    size = static_cast<int>(dataSize);

    // The data of the user is copied if it fits in the frame, the rest of the
    // memory is filled with zeros.
    const bool copyData = (_data != nullptr && _size >= 0 && _size <= size);
    const int copySize = copyData ? _size : 0;

    // Allocate memory.
    if (size > 0)
    {
        data = new uint8_t[static_cast<size_t>(size)];
        m_isAllocated = true;
        if (copySize > 0)
            memcpy(data, _data, static_cast<size_t>(copySize));
        memset(data + copySize, 0, static_cast<size_t>(size - copySize));
    }

    // The size of the data of the user is the data size of the frame.
    if (copyData)
        size = _size;

    // Copy attributes.
    width = _width;
    height = _height;
    fourcc = _fourcc;
    frameId = 0;
    sourceId = 0;
}



Frame::Frame(Frame &src)
{
    // The new object is empty (default values of the fields): the operator
    // makes the copy.
    *this = src;
}



Frame::~Frame()
{
    // Release memory.
    if (m_isAllocated)
        delete[] data;
}



Frame &Frame::operator= (const Frame &src)
{
    // Check yourself.
    if (this == &src)
        return *this;

    // The size of the source data must be positive and the data must exist to
    // copy it.
    const int srcSize = (src.size > 0) ? src.size : 0;
    const int copySize = (src.data != nullptr) ? srcSize : 0;

    // Check size and pixel format.
    if (data != nullptr &&
        width == src.width &&
        height == src.height &&
        fourcc == src.fourcc &&
        srcSize <= size)
    {
        // Copy frame data to the buffer that is already there (also if this
        // frame does not own it, for example a clone). The size field of this
        // frame tells how many bytes of the buffer can be used. The data of
        // the source can be a part of this buffer: memmove.
        if (copySize > 0 && data != src.data)
            memmove(data, src.data, static_cast<size_t>(copySize));
        size = srcSize;
    }
    else
    {
        // Calculate the size of the buffer according to pixel format. The
        // size of the source data is the minimum if the pixel format is
        // unknown or the data is larger than the pixel format needs.
        int64_t capacity = calculateDataSize(src.fourcc, src.width, src.height);
        capacity = max<int64_t>(capacity, srcSize);

        // Allocate and fill the new buffer first: if the allocation fails the
        // frame is not changed.
        uint8_t* newData = nullptr;
        if (capacity > 0)
        {
            newData = new uint8_t[static_cast<size_t>(capacity)];
            if (copySize > 0)
                memcpy(newData, src.data, static_cast<size_t>(copySize));
            memset(newData + copySize, 0,
                   static_cast<size_t>(capacity - copySize));
        }

        // Release the old buffer (the source can use it, so after the copy).
        if (m_isAllocated)
            delete[] data;

        // Copy attributes.
        width = src.width;
        height = src.height;
        fourcc = src.fourcc;
        data = newData;
        m_isAllocated = (newData != nullptr);
        size = srcSize;
    }

    // Copy frame ID and source ID.
    frameId = src.frameId;
    sourceId = src.sourceId;

    return *this;
}



void Frame::cloneTo(Frame& dst)
{
    // Check yourself.
    if (this == &dst)
        return;

    // The destination gives up its own data: nobody else can release it
    // after the pointer is replaced. If the destination already owns the
    // data of this frame it stays the owner.
    if (dst.data != data)
    {
        if (dst.m_isAllocated)
            delete[] dst.data;
        dst.m_isAllocated = false;
    }

    // Copy frame ID and source ID.
    dst.frameId = frameId;
    dst.sourceId = sourceId;

    // Copy other attributes.
    dst.width = width;
    dst.height = height;
    dst.fourcc = fourcc;
    dst.size = size;

    // Copy pointer to data.
    dst.data = data;
}



bool Frame::operator==(Frame &src)
{
    // Check yourself.
    if (this == &src)
        return true;

    // Check frame attributes.
    if (width != src.width ||
        height != src.height ||
        fourcc != src.fourcc ||
        frameId != src.frameId ||
        sourceId != src.sourceId ||
        size != src.size)
        return false;

    // Compare frame data.
    if (data == src.data || size <= 0)
        return true;

    // Frames with data size but without data are not identical to frames with
    // data.
    if (data == nullptr || src.data == nullptr)
        return false;

    return memcmp(data, src.data, static_cast<size_t>(size)) == 0;
}



bool Frame::operator!=(Frame &src)
{
    return !(*this == src);
}



void Frame::release()
{
    if (m_isAllocated)
    {
        delete[] data;
        m_isAllocated = false;
    }

    // Reset fields. The pointer is reset also if the data belongs to someone
    // else (a clone of another frame): the frame is empty after the call.
    data = nullptr;
    width = 0;
    height = 0;
    size = 0;
    frameId = 0;
    sourceId = 0;
}



void Frame::serialize(uint8_t* _data, int& _size)
{
    // The size field is the size of the data that follows. A frame without
    // data is serialized without data.
    const int dataSize = (data != nullptr && size > 0) ? size : 0;

    // Nothing is written if there is no buffer or the serialized frame is too
    // large to describe its size with an int.
    if (_data == nullptr || dataSize > INT_MAX - SERIALIZED_HEADER_SIZE)
    {
        _size = 0;
        return;
    }

    // Copy the version of the serialization format.
    int pos = 0;
    _data[pos] = SERIALIZED_FORMAT_MAJOR; pos += 1;
    _data[pos] = SERIALIZED_FORMAT_MINOR; pos += 1;

    // Copy frame size.
    memcpy(&_data[pos], &width, 4); pos += 4;
    memcpy(&_data[pos], &height, 4); pos += 4;

    // Copy FOURCC.
    uint32_t value = static_cast<uint32_t>(fourcc);
    memcpy(&_data[pos], &value, 4); pos += 4;

    // Copy size.
    memcpy(&_data[pos], &dataSize, 4); pos += 4;

    // Copy frame ID.
    memcpy(&_data[pos], &frameId, 4); pos += 4;

    // Copy source ID.
    memcpy(&_data[pos], &sourceId, 4); pos += 4;

    // Copy data.
    if (dataSize > 0)
        memcpy(&_data[pos], data, static_cast<size_t>(dataSize));
    pos += dataSize;

    _size = pos;
}



bool Frame::deserialize(uint8_t* _data, int _size)
{
    // Check params.
    if (_data == nullptr || _size < SERIALIZED_HEADER_SIZE)
        return false;

    // Check the version of the serialization format.
    if (_data[0] != SERIALIZED_FORMAT_MAJOR ||
        _data[1] != SERIALIZED_FORMAT_MINOR)
        return false;

    // Get frame size.
    int pos = 2;
    int w = 0;
    int h = 0;
    memcpy(&w, &_data[pos], 4); pos += 4;
    memcpy(&h, &_data[pos], 4); pos += 4;

    // Get FOURCC.
    uint32_t f = 0;
    memcpy(&f, &_data[pos], 4); pos += 4;

    // Get size.
    int s = 0;
    memcpy(&s, &_data[pos], 4); pos += 4;

    // Get frame ID.
    int fId = 0;
    memcpy(&fId, &_data[pos], 4); pos += 4;

    // Get source ID.
    int sId = 0;
    memcpy(&sId, &_data[pos], 4); pos += 4;

    // Check size.
    if (s != _size - SERIALIZED_HEADER_SIZE)
        return false;

    // Check the pixel format and the frame size. The frame is not changed if
    // the data is not valid.
    const Fourcc format = static_cast<Fourcc>(f);
    const int64_t pixelSize = calculateDataSize(format, w, h);
    if (pixelSize < 0)
        return false;

    // The serialized data can be a part of the memory of this frame (a frame
    // that carries a serialized frame is deserialized into itself): the data
    // is moved, and a new buffer is filled before the old one is released.
    const uint8_t* payload = &_data[pos];

    // Check size and pixel format.
    if (data != nullptr &&
        width == w &&
        height == h &&
        fourcc == format &&
        s <= size)
    {
        // Copy the data to the buffer that is already there (also if this
        // frame does not own it, for example a clone).
        if (s > 0)
            memmove(data, payload, static_cast<size_t>(s));
    }
    else
    {
        // Allocate memory first: if the allocation fails the frame is not
        // changed. The buffer is large enough for the data of the blob also
        // if it is larger than the pixel format needs.
        const int64_t capacity = max<int64_t>(pixelSize, s);
        uint8_t* newData = nullptr;
        if (capacity > 0)
        {
            newData = new (nothrow) uint8_t[static_cast<size_t>(capacity)];
            if (newData == nullptr)
                return false;
            if (s > 0)
                memcpy(newData, payload, static_cast<size_t>(s));
            memset(newData + s, 0, static_cast<size_t>(capacity - s));
        }

        // Release memory.
        if (m_isAllocated)
            delete[] data;
        data = newData;
        m_isAllocated = (newData != nullptr);
    }

    // Copy attributes.
    width = w;
    height = h;
    fourcc = format;
    size = s;
    frameId = fId;
    sourceId = sId;

    return true;
}
