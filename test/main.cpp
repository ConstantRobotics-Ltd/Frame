#include <iostream>
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



/// Entry point.
int main(void)
{
    cout << "#######################################" << endl;
    cout << "Frame vlass v" << Frame::getVersion() << " test"  << endl;
    cout << "#######################################" << endl << endl;

    cout << "Constructor test:" << endl;
    if (!constructorTest())
        cout << "ERROR" << endl << endl;
    else
        cout << "OK" << endl << endl;

    cout << "Copy test:" << endl;
    if (!copyTest())
        cout << "ERROR" << endl << endl;
    else
        cout << "OK" << endl << endl;

    cout << "Clone test:" << endl;
    if (!cloneTest())
        cout << "ERROR" << endl << endl;
    else
        cout << "OK" << endl << endl;

    cout << "Compare test:" << endl;
    if (!compareTest())
        cout << "ERROR" << endl << endl;
    else
        cout << "OK" << endl << endl;

    cout << "Serialization test:" << endl;
    if (!serializationTest())
        cout << "ERROR" << endl << endl;
    else
        cout << "OK" << endl << endl;

    return -1;
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
    if (frame1.data() != nullptr)
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
    if (frame2.data() == nullptr)
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
    uint8_t* testData = new uint8_t[640 * 480 * 3];
    for (int i = 0; i < 640 * 480 * 3; ++i)
        testData[i] = (uint8_t)(rand() % 255);
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
    if (frame3.data() == nullptr)
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
        if (testData[i] != frame3.data()[i])
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
    for (uint32_t i = 0; i < frame3.size; ++i)
        frame3.data()[i] = (uint8_t)(rand() % 255);

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
    for (uint32_t i = 0; i < frame3.size; ++i)
    {
        if (frame1.data()[i] != frame3.data()[i])
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
    for (uint32_t i = 0; i < frame3.size; ++i)
    {
        if (frame2.data()[i] != frame3.data()[i])
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
    for (uint32_t i = 0; i < frame3->size; ++i)
        frame3->data()[i] = (uint8_t)(rand() % 255);

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
    if (frame1.data() != frame3->data())
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
    if (frame2->data() != frame3->data())
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
    for (uint32_t i = 0; i < frame3.size; ++i)
        frame3.data()[i] = (uint8_t)(rand() % 255);

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
    Frame dstFrame(1280, 720, Fourcc::YUV24);

    // Fill source frame.
    for (uint32_t i = 0; i < srcFrame.size; ++i)
        srcFrame.data()[i] = (uint8_t)(rand() % 255);

    // Serialize data.
    uint8_t* data = new uint8_t[1920 * 1080 * 4];
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
    for (uint32_t i = 0; i < srcFrame.size; ++i)
    {
        if (srcFrame.data()[i] != dstFrame.data()[i])
        {
            cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << endl;
            return false;
        }
    }

    return true;
}






























