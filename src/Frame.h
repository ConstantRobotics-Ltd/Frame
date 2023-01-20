#pragma once
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>



namespace cr
{
namespace video
{

/// Macro to make FOURCC code.
#define MAKE_FOURCC_CODE(a,b,c,d) ((uint32_t)(((d)<<24)|((c)<<16)|((b)<<8)|(a)))

/**
 * @brief FOURCC codes enum.
 */
enum class Fourcc
{
    RGB24 = MAKE_FOURCC_CODE('R', 'G', 'B', 'R'),
    BGR24 = MAKE_FOURCC_CODE('B', 'G', 'R', 'B'),
    UYVY  = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y'),
    Y800  = MAKE_FOURCC_CODE('Y', '8', '0', '0'),
    YUY2  = MAKE_FOURCC_CODE('Y', 'U', 'Y', '2'),
    YUV1  = MAKE_FOURCC_CODE('Y', 'U', 'V', '1'),
    NV12  = MAKE_FOURCC_CODE('N', 'V', '1', '2'),
    JPEG  = MAKE_FOURCC_CODE('J', 'P', 'E', 'G'),
    JPG2  = MAKE_FOURCC_CODE('J', 'P', 'G', '2'),
    H264  = MAKE_FOURCC_CODE('H', '2', '6', '4'),
    H265  = MAKE_FOURCC_CODE('H', '2', '6', '5')
};



/**
 * @brief Video frame class.
 */
class Frame
{
public:

    /**
     * @brief Get string of current class version.
     * @return String of current class version.
     */
    static std::string getVersion();

    /**
     * @brief Default class constructor.
     */
    Frame();

    /**
     * @brief Class constructor with parameters.
     * @param width Frame width (pixels).
     * @param height Frame height (pixels).
     * @param fourcc FOURCC code of data format.
     * @param size Frame data size (bytes).
     * @param data Pointer to data buffer.
     */
    Frame(uint32_t width,
          uint32_t height,
          Fourcc fourcc,
          uint32_t size = 0,
          uint8_t* data = nullptr);

    /**
     * @brief Copy class constructor.
     * @param src Source class object.
     */
    Frame(const Frame& src);

    /**
     * @brief Class destructor.
     */
    ~Frame();

    /**
     * @brief Operator "=". Operator makes full copy of data.
     * @param src Source frame object.
     */
    Frame& operator= (const Frame& src);

    /**
     * @brief Operator "!=". Operator to compare two frame objects.
     * @param src Source frame object.
     * @return TRUE if the frames are not identical or FALSE.
     */
    bool operator!= (const Frame& src);

    /**
     * @brief Operator "==". Operator to compare two frame objects.
     * @param src Source frame object.
     * @return TRUE if the frames are identical or FALSE.
     */
    bool operator== (const Frame& src);

    /**
     * @brief Clone data. Method copies frame and copy just pointer to data.
     * @param dst Output frame.
     */
    void cloneTo(Frame& dst);

    /**
     * @brief Free frame memory.
     */
    void release();

    /**
     * @brief Serialize frame data. The method will encode data with params.
     * @param data Pointer to data buffer.
     *             Buffer size mus be >= frame data size + 26.
     * @param size Size of serialized data.
     */
    void serialize(uint8_t* data, int& size);

    /**
     * @brief Deserialize data data to frame object.
     * @param data Pointer to serialized data.
     * @param size Size of serialized data.
     * @return TRUE if the data deserialized or FALSE.
     */
    bool deserialize(uint8_t* data, int size);

    /// Frame width (pixels).
    uint32_t width{0};
    /// Frame height (pixels).
    uint32_t height{0};
    /// FOURCC code of data format.
    Fourcc fourcc{Fourcc::YUV1};
    /// Frame data size (bytes).
    uint32_t size{0};
    /// Pointer to data buffer.
    uint8_t* data{nullptr};
    /// ID of frame.
    uint32_t frameId{0};
    /// ID of video source.
    uint32_t sourceId{0};
};
}
}



