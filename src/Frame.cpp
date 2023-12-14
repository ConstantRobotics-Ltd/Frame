#include "Frame.h"
#include "FrameVersion.h"



// Link namespaces.
using namespace std;
using namespace cr::video;



string Frame::getVersion()
{
    return FRAME_VERSION;
}



Frame::Frame()
{

}



Frame::Frame(uint32_t _width,
             uint32_t _height,
             Fourcc _fourcc,
             uint32_t _size,
             uint8_t* _data)
{
    // Check frame size.
    if (_width == 0 || _height == 0)
    {
        _width = 0;
        _height = 0;
    }

    // Calculate frame data size according to pixel format.
    switch (_fourcc)
    {
    case Fourcc::BGR24:
    case Fourcc::RGB24:
    case Fourcc::YUV24:
        size = _width * _height * 3;
        break;
    case Fourcc::NV12:
    case Fourcc::NV21:
    case Fourcc::YU12:
    case Fourcc::YV12:
        size = _width * (_height + _height / 2);
        break;
    case Fourcc::YUYV:
    case Fourcc::UYVY:
        size = _width * _height * 2;
        break;
    case Fourcc::JPEG:
    case Fourcc::H264:
    case Fourcc::HEVC:
        size = _width * _height * 4;
        break;
    case Fourcc::GRAY:
        size = _width * _height;
        break;
    default:
        return;
    }

    // Allocate memory.
    if (size > 0)
    {
        data = new uint8_t[size];
        memset(data, 0, size);
        m_isAllocated = true;
    }

    // Copy data.
    if (_size <= size && _data != nullptr)
    {
        memcpy(data, _data, _size);
        size = _size;
    }

    // Copy atributes.
    width = _width;
    height = _height;
    fourcc = _fourcc;
    frameId = 0;
    sourceId = 0;
}



Frame::Frame(Frame &src)
{
    // free memory if allocated before.
    release();

    // Copy fields.
    width = src.width;
    height = src.height;
    fourcc = src.fourcc;
    sourceId = src.sourceId;
    frameId = src.frameId;

    // Calculate frame data size according to pixel format.
    switch (fourcc)
    {
    case Fourcc::BGR24:
    case Fourcc::RGB24:
    case Fourcc::YUV24:
        size = width * height * 3;
        break;
    case Fourcc::NV12:
    case Fourcc::NV21:
    case Fourcc::YU12:
    case Fourcc::YV12:
        size = width * (height + height / 2);
        break;
    case Fourcc::YUYV:
    case Fourcc::UYVY:
        size = width * height * 2;
        break;
    case Fourcc::JPEG:
    case Fourcc::H264:
    case Fourcc::HEVC:
        size = width * height * 4;
        break;
    case Fourcc::GRAY:
        size = width * height;
        break;
    default:
        return;
    }

    // Allocate memory.
    if (size > 0)
    {
        data = new uint8_t[size];
        memset(data, 0, size);
        m_isAllocated = true;
    }

    // Copy data.
    if (src.size <= size && src.data != nullptr)
    {
        memcpy(data, src.data, src.size);
    }

    // Copy size.
    size = src.size;
}



Frame::~Frame()
{
    // Release memory.
    if (m_isAllocated)
    {
        delete[] data;
        m_isAllocated = false;
    }
}



Frame &Frame::operator= (const Frame &src)
{
    // Check yourself.
    if (this == &src)
        return *this;

    // Copy frame ID and source ID.
    frameId = src.frameId;
    sourceId = src.sourceId;

    // Check size and pixel format.
    if (width == src.width &&
        height == src.height &&
        fourcc == src.fourcc)
    {
        // Copy frame data.
        memcpy(data, src.data, src.size);
        size = src.size;
    }
    else
    {
        // Copy atributes.
        width = src.width;
        height = src.height;
        fourcc = src.fourcc;

        // Calculate frame data size according to pixel format.
        switch (fourcc)
        {
        case Fourcc::BGR24:
        case Fourcc::RGB24:
        case Fourcc::YUV24:
            size = width * height * 3;
            break;
        case Fourcc::NV12:
        case Fourcc::NV21:
        case Fourcc::YU12:
        case Fourcc::YV12:
            size = width * (height + height / 2);
            break;
        case Fourcc::YUYV:
        case Fourcc::UYVY:
            size = width * height * 2;
            break;
        case Fourcc::JPEG:
        case Fourcc::H264:
        case Fourcc::HEVC:
            size = width * height * 4;
            break;
        case Fourcc::GRAY:
            size = width * height;
            break;
        default:
            return *this;
        }

        if (m_isAllocated)
            delete[] data;

        // Allocate memory.
        if (size > 0)
        {
            data = new uint8_t[size];
            memset(data, 0, size);
            m_isAllocated = true;
        }

        // Copy data.
        if (src.size <= size && src.data != nullptr && size > 0)
        {
            memcpy(data, src.data, src.size);
        }

        // Copy size.
        size = src.size;
    }

    return *this;
}



void Frame::cloneTo(Frame& dst)
{
    // Check yourself.
    if (this == &dst)
        return;

    // Copy frame ID and source ID.
    dst.frameId = frameId;
    dst.sourceId = sourceId;

    // Copy other atributes.
    dst.width = width;
    dst.height = height;
    dst.fourcc = fourcc;
    dst.size = size;

    // Copy pointer to data.
    dst.data = data;
    dst.m_isAllocated = false;
}



bool Frame::operator==(Frame &src)
{
    // Check yourself.
    if (this == &src)
        return true;

    // Check frame atributes.
    if (width != src.width ||
        height != src.height ||
        fourcc != src.fourcc ||
        frameId != src.frameId ||
        sourceId != src.sourceId ||
        size != src.size)
        return false;

    // Compare frame data.
    if (data == src.data)
        return true;

    if (size > 0 && src.size > 0)
        for (uint32_t i = 0; i < size; ++i)
            if (data[i] != src.data[i])
                return false;

    return true;
}



bool Frame::operator!=(Frame &src)
{
    // Check yourself.
    if (this == &src)
        return false;

    // Check frame atributes.
    if (width != src.width ||
        height != src.height ||
        fourcc != src.fourcc ||
        frameId != src.frameId ||
        sourceId != src.sourceId ||
        size != src.size)
        return true;

    // Compare frame data.
    if (data == src.data)
        return false;

    if (size > 0 && src.size > 0)
        for (uint32_t i = 0; i < size; ++i)
            if (data[i] != src.data[i])
                return true;

    return false;
}



void Frame::release()
{
    if (m_isAllocated)
    {
        delete[] data;
        m_isAllocated = false;
    }

    // Reset fields.
    width = 0;
    height = 0;
    frameId = 0;
    size = 0;
    frameId = 0;
    sourceId = 0;
}



void Frame::serialize(uint8_t* _data, int& _size)
{
    // Copy Frame class version.
    int pos = 0;
    _data[pos] = FRAME_MAJOR_VERSION; pos += 1;
    _data[pos] = FRAME_MINOR_VERSION; pos += 1;

    // Copy frame size.
    memcpy(&_data[pos], &width, 4); pos += 4;
    memcpy(&_data[pos], &height, 4); pos += 4;

    // Copy FOURCC.
    uint32_t value = (uint32_t)fourcc;
    memcpy(&_data[pos], &value, 4); pos += 4;

    // Copy size.
    memcpy(&_data[pos], &size, 4); pos += 4;

    // Copy frame ID.
    memcpy(&_data[pos], &frameId, 4); pos += 4;

    // Copy source ID.
    memcpy(&_data[pos], &sourceId, 4); pos += 4;

    // Copy data.
    if (size > 0)
        memcpy(&_data[pos], data, size);
    pos += size;

    _size = pos;
}



bool Frame::deserialize(uint8_t* _data, int _size)
{
    // Check params.
    if (_data == nullptr || _size < 26)
        return false;

    // Check frame class version.
    if (_data[0] != FRAME_MAJOR_VERSION || _data[1] != FRAME_MINOR_VERSION)
        return false;

    // Get frame size.
    int pos = 2;
    uint32_t w = 0;
    uint32_t h = 0;
    memcpy(&w, &_data[pos], 4); pos += 4;
    memcpy(&h, &_data[pos], 4); pos += 4;

    // Get FOURCC.
    uint32_t f = 0;
    memcpy(&f, &_data[pos], 4); pos += 4;

    // Get size.
    uint32_t s = 0;
    memcpy(&s, &_data[pos], 4); pos += 4;

    // Get frame ID.
    uint32_t fId = 0;
    memcpy(&fId, &_data[pos], 4); pos += 4;

    // Get source ID.
    uint32_t sId = 0;
    memcpy(&sId, &_data[pos], 4); pos += 4;

    // Check size.
    if (s != _size - 26)
        return false;

    // Check FOURCC.
    if (width != w || height != h || fourcc != (Fourcc)f)
    {
        // Update params.
        width = w;
        height = h;
        fourcc = (Fourcc)f;

        // Release memory.
        if (m_isAllocated)
            delete[] data;

        // Calculate frame data size according to pixel format.
        switch ((Fourcc)f)
        {
        case Fourcc::BGR24:
        case Fourcc::RGB24:
        case Fourcc::YUV24:
            size = width * height * 3;
            break;
        case Fourcc::NV12:
        case Fourcc::NV21:
        case Fourcc::YU12:
        case Fourcc::YV12:
            size = width * (height + height / 2);
            break;
        case Fourcc::YUYV:
        case Fourcc::UYVY:
            size = width * height * 2;
            break;
        case Fourcc::JPEG:
        case Fourcc::H264:
        case Fourcc::HEVC:
            size = width * height * 4;
            break;
        case Fourcc::GRAY:
            size = width * height;
            break;
        default:
            return false;
        }

        // Allocate memory.
        if (size > 0)
        {
            data = new uint8_t[size];
            m_isAllocated = true;
        }
    }

    // Copy atributes.
    width = w;
    height = h;
    fourcc = (Fourcc)f;
    size = s;
    frameId = fId;
    sourceId = sId;

    // Copy data.
    if (size > 0)
        memcpy(data, &_data[pos], size);

    return true;
}







