#include <climits>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include "Frame.h"



// Link namespaces.
using namespace std;
using namespace cr::video;



/// Constructor test.
bool constructorTest();

/// Copy operator test.
bool copyTest();

/// Clone test.
bool cloneTest();

/// Compare test.
bool compareTest();

/// Serialization test.
bool serializationTest();

/// Release test.
bool releaseTest();

/// Copy operator test with empty, compressed and inconsistent frames.
bool copyEdgeCasesTest();

/// Clone test: the data of the destination frame.
bool cloneEdgeCasesTest();

/// Compare test with frames that have no data.
bool compareEdgeCasesTest();

/// Constructor test with invalid parameters.
bool constructorEdgeCasesTest();

/// Serialization test with invalid and hostile data.
bool serializationEdgeCasesTest();

/// Copy and deserialization of data that is a part of the frame memory.
bool aliasingTest();

/// Sizes, copy, clone and serialization of all pixel formats.
bool formatsTest();

/// Byte number i of the test data.
uint8_t testByte(int i);



/// Check a condition inside a test function.
#define CHECK(condition)                                                       \
    if (!(condition))                                                          \
    {                                                                          \
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR ("            \
             << #condition << ")" << endl;                                     \
        return false;                                                          \
    }



/// Name and function of a test.
struct TestCase
{
    const char* name;
    bool (*function)();
};



/// Entry point. The optional parameter is the name of one test (all tests run
/// by default). The exit code is 0 if all tests passed.
int main(int argc, char** argv)
{
    cout << "#######################################" << endl;
    cout << "Frame class v" << Frame::getVersion() << " test"  << endl;
    cout << "#######################################" << endl << endl;

    const TestCase tests[] = {
        {"constructor", constructorTest},
        {"copy", copyTest},
        {"clone", cloneTest},
        {"compare", compareTest},
        {"serialization", serializationTest},
        {"release", releaseTest},
        {"copyEdgeCases", copyEdgeCasesTest},
        {"cloneEdgeCases", cloneEdgeCasesTest},
        {"compareEdgeCases", compareEdgeCasesTest},
        {"constructorEdgeCases", constructorEdgeCasesTest},
        {"serializationEdgeCases", serializationEdgeCasesTest},
        {"aliasing", aliasingTest},
        {"formats", formatsTest}};

    int failed = 0;
    for (const TestCase& test : tests)
    {
        if (argc > 1 && string(argv[1]) != test.name)
            continue;
        cout << test.name << " test:" << endl;
        if (!test.function())
        {
            cout << "ERROR" << endl << endl;
            ++failed;
        }
        else
            cout << "OK" << endl << endl;
    }

    return failed == 0 ? 0 : 1;
}



uint8_t testByte(int i)
{
    // Multiplicative hash of the index: deterministic data without a short
    // period.
    const uint32_t value = static_cast<uint32_t>(i) * 2654435761u;
    return static_cast<uint8_t>(value >> 24);
}



/// Constructor test.
bool constructorTest()
{
    // Create frame by default constructor.
    Frame frame1;

    // Check parameters.
    if (frame1.size != 0)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.width != 0)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.height != 0)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.frameId != 0)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.sourceId != 0)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.data != nullptr)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }

    // Create frame with parameters.
    Frame frame2(640, 480, Fourcc::BGR24);

    // Check parameters.
    if (frame2.size != 640 * 480 * 3)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2.width != 640)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2.height != 480)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2.frameId != 0)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2.sourceId != 0)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2.data == nullptr)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2.fourcc != Fourcc::BGR24)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }

    // Create frame with parameters.
    unique_ptr<uint8_t[]> testBuffer(new uint8_t[640 * 480 * 3]);
    uint8_t* testData = testBuffer.get();
    for (int i = 0; i < 640 * 480 * 3; ++i)
        testData[i] = testByte(i);
    Frame frame3(640, 480, Fourcc::YUV24, 640 * 480 * 3, testData);

    // Check parameters.
    if (frame3.size != 640 * 480 * 3)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame3.width != 640)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame3.height != 480)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame3.frameId != 0)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame3.sourceId != 0)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame3.data == nullptr)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame3.fourcc != Fourcc::YUV24)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }

    // Check data.
    for (int i = 0; i < 640 * 480 * 3; ++i)
    {
        if (testData[i] != frame3.data[i])
        {
            cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
            return false;
        }
    }

    return true;
}



/// Copy operator test.
bool copyTest()
{
    // Create frames.
    Frame frame1;
    Frame frame2(640, 480, Fourcc::NV12);
    Frame frame3(1280, 720, Fourcc::UYVY);

    // Fill frame data.
    for (int i = 0; i < frame3.size; ++i)
        frame3.data[i] = testByte(i);

    // Copy frame.
    frame1 = frame3;
    frame2 = frame3;

    // Compare parameters.
    if (frame1.size != frame3.size)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.width != frame3.width)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.height != frame3.height)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.fourcc != frame3.fourcc)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.sourceId != frame3.sourceId)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.frameId != frame3.frameId)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    for (int i = 0; i < frame3.size; ++i)
    {
        if (frame1.data[i] != frame3.data[i])
        {
            cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
            return false;
        }
    }

    if (frame2.size != frame3.size)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2.width != frame3.width)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2.height != frame3.height)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2.fourcc != frame3.fourcc)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2.sourceId != frame3.sourceId)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2.frameId != frame3.frameId)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    for (int i = 0; i < frame3.size; ++i)
    {
        if (frame2.data[i] != frame3.data[i])
        {
            cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
            return false;
        }
    }

    return true;
}


/// Clone test.
bool cloneTest()
{
    // Create frames.
    Frame frame1;
    Frame* frame2 = new Frame(640, 480, Fourcc::NV12);
    Frame* frame3 = new Frame(1280, 720, Fourcc::UYVY);

    // Fill frame data.
    for (int i = 0; i < frame3->size; ++i)
        frame3->data[i] = testByte(i);

    // Clone frame.
    frame3->cloneTo(frame1);
    frame3->cloneTo(*frame2);

    // Compare parameters.
    if (frame1.size != frame3->size)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.width != frame3->width)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.height != frame3->height)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.fourcc != frame3->fourcc)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.sourceId != frame3->sourceId)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.frameId != frame3->frameId)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame1.data != frame3->data)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }

    if (frame2->size != frame3->size)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2->width != frame3->width)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2->height != frame3->height)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2->fourcc != frame3->fourcc)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2->sourceId != frame3->sourceId)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2->frameId != frame3->frameId)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (frame2->data != frame3->data)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }

    delete frame2;
    delete frame3;

    return true;
}



/// Compare test.
bool compareTest()
{
    // Create frames.
    Frame frame1;
    Frame frame2(640, 480, Fourcc::NV12);
    Frame frame3(1280, 720, Fourcc::UYVY);

    // Fill frame data.
    for (int i = 0; i < frame3.size; ++i)
        frame3.data[i] = testByte(i);

    // Copy frame.
    frame1 = frame3;
    frame2 = frame3;

    if (!(frame1 == frame3))
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }

    if (!(frame1 == frame2))
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }

    if (frame1 != frame3)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }

    if (frame1 != frame2)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }

    return true;
}



/// Serialization test.
bool serializationTest()
{
    // Init frames.
    Frame srcFrame(640, 480, Fourcc::BGR24);
    Frame dstFrame;

    // Fill source frame.
    for (int i = 0; i < srcFrame.size; ++i)
        srcFrame.data[i] = testByte(i);

    // Serialize data.
    unique_ptr<uint8_t[]> buffer(new uint8_t[1920 * 1080 * 4]);
    uint8_t* data = buffer.get();
    int size = 0;
    srcFrame.serialize(data, size);

    // Deserialize data.
    if (!dstFrame.deserialize(data, size))
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }

    // Compare atributes.
    if (srcFrame.size != dstFrame.size)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (srcFrame.width != dstFrame.width)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (srcFrame.height != dstFrame.height)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (srcFrame.fourcc != dstFrame.fourcc)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (srcFrame.sourceId != dstFrame.sourceId)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }
    if (srcFrame.frameId != dstFrame.frameId)
    {
        cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
        return false;
    }

    // Compare frame data.
    for (int i = 0; i < srcFrame.size; ++i)
    {
        if (srcFrame.data[i] != dstFrame.data[i])
        {
            cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
            return false;
        }
    }

    return true;
}


/// Release test.
bool releaseTest()
{
    // Frame with memory.
    Frame frame1(640, 480, Fourcc::RGB24);
    frame1.frameId = 7;
    frame1.sourceId = 9;
    frame1.release();
    CHECK(frame1.data == nullptr)
    CHECK(frame1.size == 0)
    CHECK(frame1.width == 0)
    CHECK(frame1.height == 0)
    CHECK(frame1.frameId == 0)
    CHECK(frame1.sourceId == 0)

    // The second call and the destructor must not release the memory again.
    frame1.release();
    CHECK(frame1.data == nullptr)

    // Clone: the clone does not own the data, the original does not change.
    Frame frame2(64, 48, Fourcc::GRAY);
    Frame clone;
    frame2.cloneTo(clone);
    CHECK(clone.data == frame2.data)
    clone.release();
    CHECK(clone.data == nullptr)
    CHECK(frame2.data != nullptr)
    CHECK(frame2.size == 64 * 48)

    // Frame with external data.
    uint8_t external[16]{};
    Frame frame3;
    frame3.data = external;
    frame3.size = 16;
    frame3.release();
    CHECK(frame3.data == nullptr)

    return true;
}



/// Copy operator test with empty, compressed and inconsistent frames.
bool copyEdgeCasesTest()
{
    // An empty frame is assigned to a frame with memory (the memory must be
    // released once and the frame must be empty).
    Frame frame1(640, 480, Fourcc::BGR24);
    Frame empty;
    frame1 = empty;
    CHECK(frame1.data == nullptr)
    CHECK(frame1.size == 0)
    CHECK(frame1.width == 0)
    CHECK(frame1.height == 0)

    // A frame with memory is assigned to the empty frame again.
    Frame frame2(320, 240, Fourcc::NV12);
    for (int i = 0; i < frame2.size; ++i)
        frame2.data[i] = static_cast<uint8_t>(i * 7);
    frame1 = frame2;
    CHECK(frame1 == frame2)
    CHECK(frame1.data != frame2.data)

    // A compressed frame without dimensions (the data is not owned).
    uint8_t packet[1000];
    for (int i = 0; i < 1000; ++i)
        packet[i] = static_cast<uint8_t>(i * 13);
    Frame compressed;
    compressed.fourcc = Fourcc::H264;
    compressed.data = packet;
    compressed.size = 1000;
    compressed.frameId = 5;

    Frame copy1;
    copy1 = compressed;
    CHECK(copy1.size == 1000)
    CHECK(copy1.data != nullptr)
    CHECK(copy1.data != packet)
    CHECK(copy1.fourcc == Fourcc::H264)
    CHECK(copy1.frameId == 5)
    CHECK(memcmp(copy1.data, packet, 1000) == 0)

    // Copy constructor with the same frame.
    Frame copy2(compressed);
    CHECK(copy2.size == 1000)
    CHECK(copy2.data != nullptr)
    CHECK(copy2.data != packet)
    CHECK(memcmp(copy2.data, packet, 1000) == 0)
    CHECK(copy2 == compressed)

    // The same dimensions, the data of the source is larger than the data of
    // the destination.
    Frame small(16, 16, Fourcc::H264, 100, packet);
    CHECK(small.size == 100)
    Frame large(16, 16, Fourcc::H264, 900, packet);
    CHECK(large.size == 900)
    small = large;
    CHECK(small.size == 900)
    CHECK(memcmp(small.data, packet, 900) == 0)

    // The data of the source is smaller: the destination is not reallocated.
    uint8_t* before = small.data;
    Frame smaller(16, 16, Fourcc::H264, 10, packet);
    small = smaller;
    CHECK(small.size == 10)
    CHECK(small.data == before)

    // A frame with data of a format that is not known.
    Frame unknown;
    unknown.fourcc = static_cast<Fourcc>(0x12345678);
    unknown.width = 10;
    unknown.height = 10;
    unknown.data = packet;
    unknown.size = 100;
    Frame copy3;
    copy3 = unknown;
    CHECK(copy3.size == 100)
    CHECK(copy3.data != nullptr && copy3.data != packet)
    CHECK(memcmp(copy3.data, packet, 100) == 0)
    CHECK(copy3.width == 10 && copy3.height == 10)
    CHECK(copy3.fourcc == static_cast<Fourcc>(0x12345678))

    // A frame with a size but without data (the destination gets zeros) and a
    // frame with a negative size.
    Frame noData;
    noData.width = 8;
    noData.height = 8;
    noData.fourcc = Fourcc::GRAY;
    noData.size = 64;
    Frame copy4(8, 8, Fourcc::GRAY);
    memset(copy4.data, 0xAB, 64);
    copy4 = noData;
    CHECK(copy4.size == 64)
    CHECK(copy4.data != nullptr)
    for (int i = 0; i < 64; ++i)
        CHECK(copy4.data[i] == 0 || copy4.data[i] == 0xAB)
    Frame negative;
    negative.width = 8;
    negative.height = 8;
    negative.fourcc = Fourcc::GRAY;
    negative.size = -5;
    negative.data = packet;
    Frame copy5;
    copy5 = negative;
    CHECK(copy5.size == 0)

    // The destination is a clone: its data (not owned) must stay untouched
    // and must not be released.
    Frame owner(64, 64, Fourcc::GRAY);
    memset(owner.data, 0x11, static_cast<size_t>(owner.size));
    Frame cloned;
    owner.cloneTo(cloned);
    Frame source(32, 32, Fourcc::GRAY);
    memset(source.data, 0x22, static_cast<size_t>(source.size));
    cloned = source;
    CHECK(cloned.data != owner.data)
    CHECK(cloned.size == 32 * 32)
    CHECK(owner.data[0] == 0x11 && owner.data[64 * 64 - 1] == 0x11)

    // Self assignment and assignment of a clone of the own data.
    Frame self(32, 32, Fourcc::GRAY);
    Frame& selfRef = self;
    self = selfRef;
    CHECK(self.size == 32 * 32)
    Frame selfClone;
    self.cloneTo(selfClone);
    self = selfClone;
    CHECK(self.size == 32 * 32)
    CHECK(self.data != nullptr)

    // Repeated assignments between frames of different formats.
    Frame a(100, 100, Fourcc::YUV24);
    Frame b(50, 50, Fourcc::GRAY);
    for (int i = 0; i < 10; ++i)
    {
        a = b;
        b = a;
        Frame c(a);
        a = c;
        CHECK(a == c)
    }

    return true;
}



/// Clone test: the data of the destination frame.
bool cloneEdgeCasesTest()
{
    Frame source(64, 64, Fourcc::GRAY);

    // The destination has memory: it is released (checked by the leak
    // checker of the sanitizers) and the destination does not own the clone.
    Frame destination(32, 32, Fourcc::GRAY);
    source.cloneTo(destination);
    CHECK(destination.data == source.data)
    CHECK(destination.size == 64 * 64)

    // Clone again to the same destination (nothing is released twice).
    source.cloneTo(destination);
    CHECK(destination.data == source.data)

    // The clone is the source of a clone of the original data.
    destination.cloneTo(source);
    CHECK(destination.data == source.data)
    CHECK(source.size == 64 * 64)

    // Assignment of a frame with other dimensions: the destination gets its
    // own data, the data of the source frame is not touched.
    Frame other(8, 8, Fourcc::GRAY);
    memset(other.data, 0x33, static_cast<size_t>(other.size));
    destination = other;
    CHECK(destination.data != source.data)
    CHECK(destination == other)
    CHECK(source.size == 64 * 64)

    // Clone of an empty frame.
    Frame empty;
    empty.cloneTo(destination);
    CHECK(destination.data == nullptr)
    CHECK(destination.size == 0)

    return true;
}



/// Compare test with frames that have no data.
bool compareEdgeCasesTest()
{
    uint8_t buffer[10]{};
    Frame withData;
    withData.fourcc = Fourcc::GRAY;
    withData.width = 10;
    withData.height = 1;
    withData.size = 10;
    withData.data = buffer;

    Frame withoutData;
    withoutData.fourcc = Fourcc::GRAY;
    withoutData.width = 10;
    withoutData.height = 1;
    withoutData.size = 10;

    CHECK(!(withData == withoutData))
    CHECK(withData != withoutData)
    CHECK(!(withoutData == withData))
    CHECK(withoutData == withoutData)

    // Two frames without data and with the same size are identical.
    Frame withoutData2;
    withoutData2.fourcc = Fourcc::GRAY;
    withoutData2.width = 10;
    withoutData2.height = 1;
    withoutData2.size = 10;
    CHECK(withoutData == withoutData2)

    // Negative size.
    Frame negative1;
    negative1.size = -3;
    Frame negative2;
    negative2.size = -3;
    negative2.data = buffer;
    CHECK(negative1 == negative2)

    // Different data.
    Frame other(10, 1, Fourcc::GRAY);
    Frame same(10, 1, Fourcc::GRAY);
    CHECK(other == same)
    same.data[9] = 1;
    CHECK(other != same)

    return true;
}



/// Constructor test with invalid parameters.
bool constructorEdgeCasesTest()
{
    uint8_t buffer[64]{};

    // Negative size, huge size, unknown format: the frame is empty.
    Frame negative1(-1, 480, Fourcc::BGR24);
    CHECK(negative1.data == nullptr && negative1.size == 0 && negative1.width == 0)
    Frame negative2(640, -480, Fourcc::BGR24);
    CHECK(negative2.data == nullptr && negative2.size == 0 && negative2.height == 0)
    Frame huge(INT_MAX, INT_MAX, Fourcc::JPEG);
    CHECK(huge.data == nullptr && huge.size == 0)
    Frame huge2(65535, 65535, Fourcc::BGR24);
    CHECK(huge2.data == nullptr && huge2.size == 0)
    Frame huge3(46341, 46341, Fourcc::GRAY);
    CHECK(huge3.data == nullptr && huge3.size == 0)
    Frame unknown(10, 10, static_cast<Fourcc>(0x12345678));
    CHECK(unknown.data == nullptr && unknown.size == 0 && unknown.width == 0)

    // The largest frame that can be described.
    Frame zero(0, 480, Fourcc::BGR24);
    CHECK(zero.data == nullptr && zero.size == 0 && zero.width == 0 && zero.height == 0)

    // Data parameters: negative size, size larger than the frame, no data.
    Frame frame1(8, 8, Fourcc::GRAY, -5, buffer);
    CHECK(frame1.size == 64 && frame1.data != nullptr)
    Frame frame2(8, 8, Fourcc::GRAY, 65, buffer);
    CHECK(frame2.size == 64 && frame2.data != nullptr)
    Frame frame3(8, 8, Fourcc::GRAY, 10, nullptr);
    CHECK(frame3.size == 64 && frame3.data != nullptr)
    Frame frame4(8, 8, Fourcc::GRAY, 10, buffer);
    CHECK(frame4.size == 10 && frame4.data != nullptr)

    // Sizes of the pixel formats.
    CHECK(Frame(4, 4, Fourcc::RGB24).size == 48)
    CHECK(Frame(4, 4, Fourcc::BGR24).size == 48)
    CHECK(Frame(4, 4, Fourcc::YUV24).size == 48)
    CHECK(Frame(4, 4, Fourcc::NV12).size == 24)
    CHECK(Frame(4, 4, Fourcc::NV21).size == 24)
    CHECK(Frame(4, 4, Fourcc::YU12).size == 24)
    CHECK(Frame(4, 4, Fourcc::YV12).size == 24)
    CHECK(Frame(4, 4, Fourcc::YUYV).size == 32)
    CHECK(Frame(4, 4, Fourcc::UYVY).size == 32)
    CHECK(Frame(4, 4, Fourcc::GRAY).size == 16)
    CHECK(Frame(4, 4, Fourcc::JPEG).size == 64)
    CHECK(Frame(4, 4, Fourcc::H264).size == 64)
    CHECK(Frame(4, 4, Fourcc::HEVC).size == 64)

    return true;
}



/// Serialization test with invalid and hostile data.
bool serializationEdgeCasesTest()
{
    // Header of the serialized data written by version 5.0.9: version 5.0,
    // width 2, height 2, GRAY, data size 4, frame ID 3, source ID 4.
    uint8_t blob[64]{};
    int pos = 0;
    auto put = [&](int value)
    {
        memcpy(&blob[pos], &value, 4);
        pos += 4;
    };
    blob[0] = 5;
    blob[1] = 0;
    pos = 2;
    put(2);
    put(2);
    put(static_cast<int>(Fourcc::GRAY));
    put(4);
    put(3);
    put(4);
    for (int i = 0; i < 4; ++i)
        blob[pos + i] = static_cast<uint8_t>(10 + i);

    Frame frame1;
    CHECK(frame1.deserialize(blob, 26 + 4))
    CHECK(frame1.width == 2 && frame1.height == 2 && frame1.fourcc == Fourcc::GRAY)
    CHECK(frame1.size == 4 && frame1.frameId == 3 && frame1.sourceId == 4)
    CHECK(frame1.data[0] == 10 && frame1.data[3] == 13)

    // The format version of the serialized data does not depend on the
    // version of the library.
    uint8_t out[64];
    int outSize = 0;
    frame1.serialize(out, outSize);
    CHECK(outSize == 30)
    CHECK(out[0] == 5 && out[1] == 0)
    CHECK(memcmp(out, blob, 30) == 0)

    // Invalid parameters: the frame is not changed.
    Frame frame2(4, 4, Fourcc::GRAY);
    memset(frame2.data, 0x5A, static_cast<size_t>(frame2.size));
    uint8_t bad[64];
    CHECK(!frame2.deserialize(nullptr, 30))
    CHECK(!frame2.deserialize(blob, 25))
    memcpy(bad, blob, 30);
    bad[0] = 4;
    CHECK(!frame2.deserialize(bad, 30))
    memcpy(bad, blob, 30);
    bad[1] = 1;
    CHECK(!frame2.deserialize(bad, 30))
    CHECK(!frame2.deserialize(blob, 29))
    CHECK(!frame2.deserialize(blob, 31))

    // Unknown format.
    memcpy(bad, blob, 30);
    int value = 0x12345678;
    memcpy(&bad[10], &value, 4);
    CHECK(!frame2.deserialize(bad, 30))

    // Negative dimensions and sizes that do not fit in an int.
    memcpy(bad, blob, 30);
    value = -2;
    memcpy(&bad[2], &value, 4);
    CHECK(!frame2.deserialize(bad, 30))
    memcpy(bad, blob, 30);
    value = 65535;
    memcpy(&bad[2], &value, 4);
    memcpy(&bad[6], &value, 4);
    value = static_cast<int>(Fourcc::BGR24);
    memcpy(&bad[10], &value, 4);
    CHECK(!frame2.deserialize(bad, 30))
    memcpy(bad, blob, 30);
    value = INT_MAX;
    memcpy(&bad[2], &value, 4);
    memcpy(&bad[6], &value, 4);
    CHECK(!frame2.deserialize(bad, 30))

    // None of the calls changed the frame.
    CHECK(frame2.width == 4 && frame2.height == 4 && frame2.size == 16)
    CHECK(frame2.data != nullptr && frame2.data[0] == 0x5A && frame2.data[15] == 0x5A)

    // More data than the pixel format needs (the blob has the same size as
    // the header says): the data is copied completely, nothing is written
    // beyond the memory of the frame.
    std::unique_ptr<uint8_t[]> large(new uint8_t[26 + 4096]);
    memset(large.get(), 0x77, 26 + 4096);
    memcpy(large.get(), blob, 26);
    value = 1;
    memcpy(&large[2], &value, 4);
    memcpy(&large[6], &value, 4);
    value = static_cast<int>(Fourcc::BGR24);
    memcpy(&large[10], &value, 4);
    value = 4096;
    memcpy(&large[14], &value, 4);
    Frame frame3;
    CHECK(frame3.deserialize(large.get(), 26 + 4096))
    CHECK(frame3.size == 4096 && frame3.width == 1 && frame3.height == 1)
    CHECK(frame3.data[0] == 0x77 && frame3.data[4095] == 0x77)

    // The same for a frame that has memory for the new dimensions.
    Frame frame4(1, 1, Fourcc::BGR24);
    CHECK(frame4.deserialize(large.get(), 26 + 4096))
    CHECK(frame4.size == 4096)
    CHECK(frame4.data[4095] == 0x77)

    // A frame without dimensions receives data (no memory yet).
    Frame frame5;
    frame5.fourcc = Fourcc::JPEG;
    memcpy(large.get(), blob, 26);
    value = 0;
    memcpy(&large[2], &value, 4);
    memcpy(&large[6], &value, 4);
    value = static_cast<int>(Fourcc::JPEG);
    memcpy(&large[10], &value, 4);
    value = 100;
    memcpy(&large[14], &value, 4);
    CHECK(frame5.deserialize(large.get(), 26 + 100))
    CHECK(frame5.size == 100 && frame5.data != nullptr)

    // Repeated deserialization into the same frame reuses the memory.
    Frame frame6;
    CHECK(frame6.deserialize(blob, 30))
    uint8_t* firstData = frame6.data;
    CHECK(frame6.deserialize(blob, 30))
    CHECK(frame6.data == firstData)

    // A frame with a data size but without data is serialized without data.
    Frame frame7;
    frame7.width = 4;
    frame7.height = 4;
    frame7.fourcc = Fourcc::GRAY;
    frame7.size = 16;
    int size7 = 0;
    uint8_t out7[64];
    frame7.serialize(out7, size7);
    CHECK(size7 == 26)
    Frame frame8;
    CHECK(frame8.deserialize(out7, size7))
    CHECK(frame8.width == 4 && frame8.size == 0)

    // No buffer.
    int size9 = 77;
    frame7.serialize(nullptr, size9);
    CHECK(size9 == 0)

    // Round trip of all pixel formats.
    const Fourcc formats[] = {Fourcc::RGB24, Fourcc::BGR24, Fourcc::YUYV,  Fourcc::UYVY,
                              Fourcc::GRAY,  Fourcc::YUV24, Fourcc::NV12,  Fourcc::NV21,
                              Fourcc::YU12,  Fourcc::YV12,  Fourcc::JPEG,  Fourcc::H264,
                              Fourcc::HEVC};
    for (Fourcc format : formats)
    {
        Frame src(16, 8, format);
        for (int i = 0; i < src.size; ++i)
            src.data[i] = static_cast<uint8_t>(i * 31 + 5);
        src.frameId = 11;
        src.sourceId = 12;
        std::unique_ptr<uint8_t[]> buffer(new uint8_t[static_cast<size_t>(src.size) + 26]);
        int serializedSize = 0;
        src.serialize(buffer.get(), serializedSize);
        CHECK(serializedSize == src.size + 26)
        Frame dst;
        CHECK(dst.deserialize(buffer.get(), serializedSize))
        CHECK(dst == src)
    }

    return true;
}



/// Copy and deserialization of data that is a part of the frame memory.
bool aliasingTest()
{
    // Frame that is serialized below.
    Frame original(8, 6, Fourcc::GRAY);
    for (int i = 0; i < original.size; ++i)
        original.data[i] = testByte(i);
    original.frameId = 21;
    original.sourceId = 22;

    // A frame that carries a serialized frame is deserialized into itself:
    // other format and size, new memory.
    Frame carrier(64, 64, Fourcc::JPEG);
    int carrierSize = 0;
    original.serialize(carrier.data, carrierSize);
    CHECK(carrierSize == original.size + 26)
    carrier.size = carrierSize;
    CHECK(carrier.deserialize(carrier.data, carrier.size))
    CHECK(carrier == original)
    CHECK(carrier.data != original.data)

    // The same format and size: the memory is reused, the data is moved.
    Frame part(16, 16, Fourcc::GRAY);
    for (int i = 0; i < part.size; ++i)
        part.data[i] = testByte(i + 1000);
    part.size = 200;
    Frame packed(16, 16, Fourcc::GRAY);
    uint8_t* packedMemory = packed.data;
    int packedSize = 0;
    part.serialize(packed.data, packedSize);
    CHECK(packedSize == 226)
    CHECK(packed.deserialize(packed.data, packedSize))
    CHECK(packed.data == packedMemory)
    CHECK(packed.size == 200)
    CHECK(memcmp(packed.data, part.data, 200) == 0)

    // The source of a copy is a part of the memory of the destination: the
    // same format and size (memory reused).
    Frame whole(16, 16, Fourcc::GRAY);
    for (int i = 0; i < whole.size; ++i)
        whole.data[i] = testByte(i);
    uint8_t expected[100];
    memcpy(expected, whole.data + 10, 100);
    Frame view;
    view.width = 16;
    view.height = 16;
    view.fourcc = Fourcc::GRAY;
    view.data = whole.data + 10;
    view.size = 100;
    whole = view;
    CHECK(whole.size == 100)
    CHECK(memcmp(whole.data, expected, 100) == 0)

    // The same with another frame size (new memory, the old memory is
    // released after the copy).
    Frame big(32, 32, Fourcc::GRAY);
    for (int i = 0; i < big.size; ++i)
        big.data[i] = testByte(i + 7);
    memcpy(expected, big.data + 5, 64);
    Frame subFrame;
    subFrame.width = 8;
    subFrame.height = 8;
    subFrame.fourcc = Fourcc::GRAY;
    subFrame.data = big.data + 5;
    subFrame.size = 64;
    big = subFrame;
    CHECK(big.size == 64 && big.width == 8 && big.height == 8)
    CHECK(memcmp(big.data, expected, 64) == 0)

    // Deserialization into a clone of the same format and size writes into
    // the memory of the original frame.
    Frame owner(8, 6, Fourcc::GRAY);
    Frame clone;
    owner.cloneTo(clone);
    uint8_t blob[26 + 48];
    int blobSize = 0;
    original.serialize(blob, blobSize);
    CHECK(blobSize == 26 + 48)
    CHECK(clone.deserialize(blob, blobSize))
    CHECK(clone.data == owner.data)
    CHECK(memcmp(owner.data, original.data, 48) == 0)

    return true;
}



/// Sizes, copy, clone and serialization of all pixel formats.
bool formatsTest()
{
    const Fourcc formats[] = {Fourcc::RGB24, Fourcc::BGR24, Fourcc::YUYV,  Fourcc::UYVY,
                              Fourcc::GRAY,  Fourcc::YUV24, Fourcc::NV12,  Fourcc::NV21,
                              Fourcc::YU12,  Fourcc::YV12,  Fourcc::JPEG,  Fourcc::H264,
                              Fourcc::HEVC};
    const int dimensions[][2] = {{1, 1}, {1, 7}, {3, 5}, {17, 9}, {2, 2}, {640, 1}, {33, 32}};
    for (Fourcc format : formats)
    {
        for (const auto& dimension : dimensions)
        {
            const int w = dimension[0];
            const int h = dimension[1];

            // Size of the data of the pixel format.
            int expectedSize = 0;
            switch (format)
            {
            case Fourcc::RGB24:
            case Fourcc::BGR24:
            case Fourcc::YUV24:
                expectedSize = w * h * 3;
                break;
            case Fourcc::NV12:
            case Fourcc::NV21:
            case Fourcc::YU12:
            case Fourcc::YV12:
                expectedSize = w * (h + h / 2);
                break;
            case Fourcc::YUYV:
            case Fourcc::UYVY:
                expectedSize = w * h * 2;
                break;
            case Fourcc::GRAY:
                expectedSize = w * h;
                break;
            default:
                expectedSize = w * h * 4;
                break;
            }

            Frame src(w, h, format);
            CHECK(src.size == expectedSize && src.data != nullptr)
            CHECK(src.width == w && src.height == h && src.fourcc == format)
            for (int i = 0; i < src.size; ++i)
                src.data[i] = testByte(i + w * 100 + h);
            src.frameId = w;
            src.sourceId = h;

            // Copy constructor and copy operator.
            Frame copy(src);
            CHECK(copy == src && copy.data != src.data)
            Frame assigned(4, 4, Fourcc::RGB24);
            assigned = src;
            CHECK(assigned == src && assigned.data != src.data)

            // Clone.
            Frame clone(2, 2, Fourcc::GRAY);
            src.cloneTo(clone);
            CHECK(clone == src && clone.data == src.data)

            // Serialization into a frame of another format.
            unique_ptr<uint8_t[]> buffer(new uint8_t[static_cast<size_t>(src.size) + 26]);
            int serializedSize = 0;
            src.serialize(buffer.get(), serializedSize);
            CHECK(serializedSize == src.size + 26)
            Frame dst(5, 3, Fourcc::NV12);
            CHECK(dst.deserialize(buffer.get(), serializedSize))
            CHECK(dst == src)

            // Constructor with data.
            Frame withData(w, h, format, src.size, src.data);
            CHECK(withData.size == src.size)
            CHECK(memcmp(withData.data, src.data, static_cast<size_t>(src.size)) == 0)
        }
    }

    // Compressed data larger than the pixel format size: copy, serialization
    // and comparison use the data size.
    unique_ptr<uint8_t[]> packet(new uint8_t[5000]);
    for (int i = 0; i < 5000; ++i)
        packet[static_cast<size_t>(i)] = testByte(i);
    Frame compressed;
    compressed.width = 8;
    compressed.height = 8;
    compressed.fourcc = Fourcc::HEVC;
    compressed.data = packet.get();
    compressed.size = 5000;
    Frame copy(compressed);
    CHECK(copy == compressed && copy.data != packet.get())
    unique_ptr<uint8_t[]> buffer(new uint8_t[5026]);
    int serializedSize = 0;
    compressed.serialize(buffer.get(), serializedSize);
    CHECK(serializedSize == 5026)
    Frame dst(8, 8, Fourcc::HEVC);
    CHECK(dst.deserialize(buffer.get(), serializedSize))
    CHECK(dst == compressed)

    return true;
}
