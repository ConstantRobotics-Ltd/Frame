#include "Frame.h"
#include "FrameVersion.h"



// Link namespaces.
using namespace std;
using namespace cr::video;



/// Get class version.
string Frame::getVersion()
{
    return FRAME_VERSION;
}



/// Default class constructor.
Frame::Frame()
{
    // Init transformation matrix.
    transformMatrix[0][0] = 0.0f;
    transformMatrix[0][1] = 0.0f;
    transformMatrix[0][2] = 0.0f;
    transformMatrix[1][0] = 0.0f;
    transformMatrix[1][1] = 0.0f;
    transformMatrix[1][2] = 0.0f;
    transformMatrix[2][0] = 0.0f;
    transformMatrix[2][1] = 0.0f;
    transformMatrix[2][2] = 0.0f;
}



/// Class constructor with parameters.
Frame::Frame(uint32_t _width,
             uint32_t _height,
             Fourcc _fourcc,
             uint32_t _size,
             uint8_t* _data,
             uint32_t _frameId,
             uint32_t _sourceId,
             float _matrix[3][3])
{
    // Check frame size.
    if (_width == 0 || _height == 0)
        return;

    // Calculate frame data size according to pixel format.
    switch (_fourcc)
    {
    case Fourcc::RGB24:
        size = _width * _height * 3;
        break;
    case Fourcc::BGR24:
        size = _width * _height * 3;
        break;
    case Fourcc::UYVY:
        size = _width * _height * 2;
        break;
    case Fourcc::YUY2:
        size = _width * _height * 2;
        break;
    case Fourcc::Y800:
        size = _width * _height;
        break;
    case Fourcc::NV12:
        size = _width * (_height + _height / 2);
        break;
    case Fourcc::YUV1:
        size = _width * _height * 3;
        break;
    case Fourcc::JPEG:
        if (_size != 0)
            size = _size;
        else
            size = _width * _height * 4;
        break;
    case Fourcc::JPG2:
        if (_size != 0)
            size = _size;
        else
            size = _width * _height * 4;
        break;
    case Fourcc::H264:
        if (_size != 0)
            size = _size;
        else
            size = _width * _height * 4;
        break;
    case Fourcc::H265:
        if (_size != 0)
            size = _size;
        else
            size = _width * _height * 4;
        break;
    default:
        return;
    }

    // Copy atributes.
    width = _width;
    height = _height;
    fourcc = _fourcc;
    frameId = _frameId;
    sourceId = _sourceId;

    // Copy data.
    if (_size == size && _data != nullptr)
    {
        data = new uint8_t[size];
        memcpy(data, _data, size);
    }
    else
    {
        // Allocate zero memory.
        data = new uint8_t[size];
        memset(data, 0, size);
    }

    // Init transformation matrix.
    if (_matrix != nullptr)
    {
        // Init transformation matrix.
        transformMatrix[0][0] = _matrix[0][0];
        transformMatrix[0][1] = _matrix[0][1];
        transformMatrix[0][2] = _matrix[0][2];
        transformMatrix[1][0] = _matrix[1][0];
        transformMatrix[1][1] = _matrix[1][1];
        transformMatrix[1][2] = _matrix[1][2];
        transformMatrix[2][0] = _matrix[2][0];
        transformMatrix[2][1] = _matrix[2][1];
        transformMatrix[2][2] = _matrix[2][2];
    }
    else
    {
        // Init transformation matrix.
        transformMatrix[0][0] = 0.0f;
        transformMatrix[0][1] = 0.0f;
        transformMatrix[0][2] = 0.0f;
        transformMatrix[1][0] = 0.0f;
        transformMatrix[1][1] = 0.0f;
        transformMatrix[1][2] = 0.0f;
        transformMatrix[2][0] = 0.0f;
        transformMatrix[2][1] = 0.0f;
        transformMatrix[2][2] = 0.0f;
    }
}



/// Copy-constructor.
Frame::Frame(const Frame &src)
{
    // Copy fields.
    width = src.width;
    height = src.height;
    fourcc = src.fourcc;
    size = src.size;
    sourceId = src.sourceId;
    frameId = src.frameId;

    // Free memory.
    delete[] data;
    data = nullptr;

    // Copy frame data.
    if (size > 0)
    {
        data = new uint8_t[size];
        memcpy(data, src.data, size);
    }

    // Copy transformation matrix.
    transformMatrix[0][0] = src.transformMatrix[0][0];
    transformMatrix[0][1] = src.transformMatrix[0][1];
    transformMatrix[0][2] = src.transformMatrix[0][2];
    transformMatrix[1][0] = src.transformMatrix[1][0];
    transformMatrix[1][1] = src.transformMatrix[1][1];
    transformMatrix[1][2] = src.transformMatrix[1][2];
    transformMatrix[2][0] = src.transformMatrix[2][0];
    transformMatrix[2][1] = src.transformMatrix[2][1];
    transformMatrix[2][2] = src.transformMatrix[2][2];
}



/// Frame destructor.
Frame::~Frame()
{
    // Free memory.
    if (this->data != nullptr)
        delete[] this->data;
    this->data = nullptr;
}



/// Copy operator. 
Frame &Frame::operator=(const Frame &src)
{
    // Check yourself.
    if (this == &src)
        return *this;

    // Copy frame ID and source ID.
    frameId = src.frameId;
    sourceId = src.sourceId;

    // Copy transformation matrix.
    transformMatrix[0][0] = src.transformMatrix[0][0];
    transformMatrix[0][1] = src.transformMatrix[0][1];
    transformMatrix[0][2] = src.transformMatrix[0][2];
    transformMatrix[1][0] = src.transformMatrix[1][0];
    transformMatrix[1][1] = src.transformMatrix[1][1];
    transformMatrix[1][2] = src.transformMatrix[1][2];
    transformMatrix[2][0] = src.transformMatrix[2][0];
    transformMatrix[2][1] = src.transformMatrix[2][1];
    transformMatrix[2][2] = src.transformMatrix[2][2];

    // Copy frame ID and source ID.
    sourceId = src.sourceId;
    frameId = src.frameId;

    // Check size and pixel format.
    if (width == src.width &&
        height == src.height &&
        size == src.size &&
        fourcc == src.fourcc)
    {
        // Copy frame data.
        memcpy(data, src.data, size);
    }
    else
    {
        // Copy frame data.
        if (src.size > 0)
        {
            if (size == src.size)
            {
                memcpy(data, src.data, src.size);
            }
            else
            {
                delete[] data;
                data = new uint8_t[src.size];
                memcpy(data, src.data, src.size);
            }
        }

        // Copy atributes.
        width = src.width;
        height = src.height;
        fourcc = src.fourcc;
        size = src.size;
    }

    return *this;
}



/// Clone frame to another. 
void Frame::cloneTo(Frame& dst)
{
    // Check yourself.
    if (this == &dst)
        return;

    // Copy frame ID and source ID.
    dst.frameId = frameId;
    dst.sourceId = sourceId;

    // Copy transformation matrix.
    dst.transformMatrix[0][0] = transformMatrix[0][0];
    dst.transformMatrix[0][1] = transformMatrix[0][1];
    dst.transformMatrix[0][2] = transformMatrix[0][2];
    dst.transformMatrix[1][0] = transformMatrix[1][0];
    dst.transformMatrix[1][1] = transformMatrix[1][1];
    dst.transformMatrix[1][2] = transformMatrix[1][2];
    dst.transformMatrix[2][0] = transformMatrix[2][0];
    dst.transformMatrix[2][1] = transformMatrix[2][1];
    dst.transformMatrix[2][2] = transformMatrix[2][2];

    // Copy other atributes.
    dst.width = width;
    dst.height = height;
    dst.fourcc = fourcc;
    dst.size = size;

    // Release memory.
    if (dst.data != nullptr)
        delete[] dst.data;

    // Copy pointer to data.
    dst.data = data;
}



/// Compare operator. 
bool Frame::operator==(const Frame &src)
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
        size != src.size ||
        transformMatrix[0][0] != src.transformMatrix[0][0] ||
        transformMatrix[0][1] != src.transformMatrix[0][1] ||
        transformMatrix[0][2] != src.transformMatrix[0][2] ||
        transformMatrix[1][0] != src.transformMatrix[1][0] ||
        transformMatrix[1][1] != src.transformMatrix[1][1] ||
        transformMatrix[1][2] != src.transformMatrix[1][2] ||
        transformMatrix[2][0] != src.transformMatrix[2][0] ||
        transformMatrix[2][1] != src.transformMatrix[2][1] ||
        transformMatrix[2][2] != src.transformMatrix[2][2])
        return false;

    // Compare frame data.
    if (data == src.data)
        return true;

    for (int i = 0; i < size; ++i)
        if (data[i] != src.data[i])
            return false;

    return true;
}



/// Set transformation matrix. 
void Frame::setTransformMatrix(const float matrix[3][3])
{
    // Copy transformation matrix.
    transformMatrix[0][0] = matrix[0][0];
    transformMatrix[0][1] = matrix[0][1];
    transformMatrix[0][2] = matrix[0][2];
    transformMatrix[1][0] = matrix[1][0];
    transformMatrix[1][1] = matrix[1][1];
    transformMatrix[1][2] = matrix[1][2];
    transformMatrix[2][0] = matrix[2][0];
    transformMatrix[2][1] = matrix[2][1];
    transformMatrix[2][2] = matrix[2][2];
}



/// Get transformation matrix.
void Frame::getTransformMatrix(float matrix[3][3])
{
    // Copy transformation matrix.
    matrix[0][0] = transformMatrix[0][0];
    matrix[0][1] = transformMatrix[0][1];
    matrix[0][2] = transformMatrix[0][2];
    matrix[1][0] = transformMatrix[1][0];
    matrix[1][1] = transformMatrix[1][1];
    matrix[1][2] = transformMatrix[1][2];
    matrix[2][0] = transformMatrix[2][0];
    matrix[2][1] = transformMatrix[2][1];
    matrix[2][2] = transformMatrix[2][2];
}



/// Release memory.
void Frame::release()
{
    // Free memory.
    delete[] data;
    data = nullptr;

    // Reset fields.
    width = 0;
    height = 0;
    frameId = 0;
    size = 0;
    // Init transformation matrix.
    transformMatrix[0][0] = 0.0f;
    transformMatrix[0][1] = 0.0f;
    transformMatrix[0][2] = 0.0f;
    transformMatrix[1][0] = 0.0f;
    transformMatrix[1][1] = 0.0f;
    transformMatrix[1][2] = 0.0f;
    transformMatrix[2][0] = 0.0f;
    transformMatrix[2][1] = 0.0f;
    transformMatrix[2][2] = 0.0f;
}



/// Serialize frame data.
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

    // Copy transformation matrix.
    memcpy(&_data[pos], &transformMatrix[0][0], 4); pos += 4;
    memcpy(&_data[pos], &transformMatrix[0][1], 4); pos += 4;
    memcpy(&_data[pos], &transformMatrix[0][2], 4); pos += 4;
    memcpy(&_data[pos], &transformMatrix[1][0], 4); pos += 4;
    memcpy(&_data[pos], &transformMatrix[1][1], 4); pos += 4;
    memcpy(&_data[pos], &transformMatrix[1][2], 4); pos += 4;
    memcpy(&_data[pos], &transformMatrix[2][0], 4); pos += 4;
    memcpy(&_data[pos], &transformMatrix[2][1], 4); pos += 4;
    memcpy(&_data[pos], &transformMatrix[2][2], 4); pos += 4;

    // Copy data.
    if (size > 0)
        memcpy(&_data[pos], data, size);
    pos += size;

    _size = pos;
}



/// Deserialize data data to frame object.
bool Frame::deserialize(uint8_t* _data, int _size)
{
    // Check params.
    if (_data == nullptr || _size < 62)
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

    // Get transformation matrix.
    float tM[3][3];
    memcpy(&tM[0][0], &_data[pos], 4); pos += 4;
    memcpy(&tM[0][1], &_data[pos], 4); pos += 4;
    memcpy(&tM[0][2], &_data[pos], 4); pos += 4;
    memcpy(&tM[1][0], &_data[pos], 4); pos += 4;
    memcpy(&tM[1][1], &_data[pos], 4); pos += 4;
    memcpy(&tM[1][2], &_data[pos], 4); pos += 4;
    memcpy(&tM[2][0], &_data[pos], 4); pos += 4;
    memcpy(&tM[2][1], &_data[pos], 4); pos += 4;
    memcpy(&tM[2][2], &_data[pos], 4); pos += 4;

    // Check size.
    if (s != _size - 62)
        return false;

    // Check FOURCC.
    if (s > 0)
    {
        switch ((Fourcc)f)
        {
        case Fourcc::RGB24:
            if (s != w * h * 3)
                return false;
            break;
        case Fourcc::BGR24:
            if (s != w * h * 3)
                return false;
            break;
        case Fourcc::UYVY:
            if (s != w * h * 2)
                return false;
            break;
        case Fourcc::YUY2:
            if (s != w * h * 2)
                return false;
            break;
        case Fourcc::Y800:
            if (s != w * h)
                return false;
            break;
        case Fourcc::NV12:
            if (s != w * (h + h / 2))
                return false;
            break;
        case Fourcc::YUV1:
            if (s != w * h * 3)
                return false;
            break;
        case Fourcc::JPEG:
            if (s > w * h * 4)
                return false;
            break;
        case Fourcc::JPG2:
            if (s > w * h * 4)
                return false;
            break;
        case Fourcc::H264:
            if (s > w * h * 4)
                return false;
            break;
        case Fourcc::H265:
            if (s > w * h * 4)
                return false;
            break;
        default:
            return false;
        }

        // Release memory.
        if (size != s)
        {
            delete[] data;
            data = new uint8_t[s];
        }

        // Copy data.
        memcpy(data, &_data[pos], );
    }
    
    /// TOTO: Exclude allocate opperations for compressed formats.
    


    return true;
}











