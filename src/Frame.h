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
    /// RGB 24bit pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-rgb.html#v4l2-pix-fmt-rgb24
    RGB24 = MAKE_FOURCC_CODE('R', 'G', 'B', '3'),
    /// BGR 24bit pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-rgb.html#v4l2-pix-fmt-bgr24
    BGR24 = MAKE_FOURCC_CODE('B', 'G', 'R', '3'),
    /// YUYV 16bits per pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-packed-yuv.html#v4l2-pix-fmt-yuyv
    YUYV  = MAKE_FOURCC_CODE('Y', 'U', 'Y', 'V'),
    /// UYVY 16bits per pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-packed-yuv.html#v4l2-pix-fmt-vyuy
    UYVY  = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y'),
    /// Grayscale 8bit.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-yuv-luma.html#v4l2-pix-fmt-grey
    GRAY  = MAKE_FOURCC_CODE('G', 'R', 'A', 'Y'),
    /// YUV 24bit per pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-packed-yuv.html#v4l2-pix-fmt-yuv24
    YUV24  = MAKE_FOURCC_CODE('Y', 'U', 'V', '3'),
    /// NV12 pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-yuv-planar.html#v4l2-pix-fmt-nv12
    NV12  = MAKE_FOURCC_CODE('N', 'V', '1', '2'),
    /// NV21 pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-yuv-planar.html#v4l2-pix-fmt-nv21
    NV21  = MAKE_FOURCC_CODE('N', 'V', '2', '1'),
    /// YU12 (YUV420) - Planar pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-yuv-planar.html#v4l2-pix-fmt-yuv420
    YU12 = MAKE_FOURCC_CODE('Y', 'U', '1', '2'),
    /// YV12 (YVU420) - Planar pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-yuv-planar.html#v4l2-pix-fmt-yuv420
    YV12 = MAKE_FOURCC_CODE('Y', 'V', '1', '2'),
    /// JPEG compressed format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-compressed.html#v4l2-pix-fmt-jpeg
    JPEG  = MAKE_FOURCC_CODE('J', 'P', 'E', 'G'),
    /// H264 compressed format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-compressed.html#v4l2-pix-fmt-h264
    H264  = MAKE_FOURCC_CODE('H', '2', '6', '4'),
    /// HEVC compressed format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-compressed.html#v4l2-pix-fmt-hevc
    HEVC  = MAKE_FOURCC_CODE('H', 'E', 'V', 'C')
};



/**
 * @brief Video frame class.
 */
class Frame
{
public:

    /**
     * @brief Get string of current class version.
     * @return String of current class version "Major.Minor.Patch"
     */
    static std::string getVersion();

    /**
     * @brief Default class constructor.
     */
    Frame();

    /**
     * @brief Class constructor with parameters. This constructor allocates
     * memory according to frame size and format. A frame with an unknown
     * format, a negative width or height or a data size that does not fit in
     * an int is empty (no memory is allocated).
     * @param width Frame width (pixels).
     * @param height Frame height (pixels).
     * @param fourcc FOURCC code of data format.
     * @param size Frame data size (bytes).
     * @param data Pointer to data buffer. If pointer to data provided the class
     * will copy data to internal buffer.
     */
    Frame(int width, int height, Fourcc fourcc, int size = 0, uint8_t* data = nullptr);

    /**
     * @brief Copy class constructor.
     * @param src Source class object.
     */
    Frame(Frame& src);

    /**
     * @brief Class destructor.
     */
    ~Frame();

    /**
     * @brief Operator "=". Operator makes full copy of data. If the memory
     * can not be allocated the method throws std::bad_alloc and the frame is
     * not changed.
     * @param src Source frame object.
     */
    Frame& operator= (const Frame& src);

    /**
     * @brief Operator "!=". Operator to compare two frame objects.
     * @param src Source frame object.
     * @return TRUE if the frames are not identical or FALSE.
     */
    bool operator!= (Frame& src);

    /**
     * @brief Operator "==". Operator to compare two frame objects.
     * @param src Source frame object.
     * @return TRUE if the frames are identical or FALSE.
     */
    bool operator== (Frame& src);

    /**
     * @brief Clone data. Method copies frame and copy just pointer to data.
     * The data of the output frame that it owned before is released. The
     * output frame does not own the cloned data: it stays valid until this
     * frame releases it.
     * @param dst Output frame.
     */
    void cloneTo(Frame& dst);

    /**
     * @brief Release frame memory. The method resets the fields (the pointer
     * to data too), the frame is empty after the call.
     */
    void release();

    /**
     * @brief Serialize frame data. The method will encode data with params.
     * The format of the serialized data is version 5.0 in all versions 5.0.x
     * and 5.1.x of the class.
     * @param data Pointer to data buffer.
     *             Buffer size must be >= frame data size + 26.
     * @param size Size of serialized data. Zero if nothing was written (no
     *             buffer or data too large).
     */
    void serialize(uint8_t* data, int& size);

    /**
     * @brief Deserialize data to frame object. The frame is not changed if
     * the data is not valid.
     * @param data Pointer to serialized data.
     * @param size Size of serialized data.
     * @return TRUE if the data deserialized or FALSE.
     */
    bool deserialize(uint8_t* data, int size);

    /// Frame width (pixels).
    int width{0};
    /// Frame height (pixels).
    int height{0};
    /// FOURCC code of data format.
    Fourcc fourcc{Fourcc::YUV24};
    /// Frame data size (bytes).
    int size{0};
    /// ID of frame.
    int frameId{0};
    /// ID of video source.
    int sourceId{0};
    /// Pointer to frame data.
    uint8_t* data{nullptr};

private:

    /// Flag data allocation.
    bool m_isAllocated{false};
};
}
}
