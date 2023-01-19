#include <iostream>
#include <string>

#include "VideoDataStructures.h"
using namespace cr::vsource;

/**
 * @brief Method for comparing frame similarity
 *
 * @param l First frame to compare
 * @param r Second frame to compare
 *
 * @return TRUE if both frames are the same, otherwise FALSE
 */
bool isEqualFrames(const Frame& l, const Frame& r);

int main(void)
{
    std::cout<< "=================================================" << std::endl;
    std::cout<< "VideoDataStructuresDemoApp "                       << std::endl;
    std::cout<< "=================================================" << std::endl;
    std::cout<< "Library versions: "                                << std::endl;
    std::cout<< "Frame:........... " << Frame::getVersion()         << std::endl;
    std::cout<< "-------------------------------------------------" << std::endl;
    std::cout<< std::endl;

    // #1 Frame create test.
    std::cout << "#1 Frame create test\t: ";
    const uint32_t width = 256;
    const uint32_t height = 256;
    const uint32_t fourcc = (uint32_t)ValidFourccCodes::BGR24;
    const uint32_t size = width * height * 3;
    uint8_t data[size]; memset(data, 1, size);
    uint32_t frameId = 1;
    uint32_t srcId = 0;
    float matrix[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};
    Frame testFrame = Frame(width,height,fourcc,size,data,frameId,srcId,matrix);
    if (testFrame.size != 0)
        std::cout << "OK" << std::endl;
    else
        std::cout << "ERROR" << std::endl;

    // #2 Frame copy test.
    std::cout << "#2 Frame copy test\t: ";
    Frame copyFrame = testFrame;
    if (isEqualFrames(copyFrame, testFrame))
        std::cout << "OK" << std::endl;
    else
        std::cout << "ERROR" << std::endl;

    // #3 Frame release test.
    std::cout << "#3 Frame release test\t: ";
    testFrame.release();
    if (testFrame.size == 0)
        std::cout << "OK" << std::endl;
    else
        std::cout << "ERROR" << std::endl;

    return 1;
}

bool isEqualFrames(const Frame& l, const Frame& r)
{
    if (r.width == l.width &&
        r.height == l.height &&
        r.size == l.size &&
        r.fourcc == l.fourcc &&
        r.sourceId == l.sourceId &&
        r.frameId == l.frameId &&
        r.transformMatrix[0][0] == l.transformMatrix[0][0] &&
        r.transformMatrix[0][1] == l.transformMatrix[0][1] &&
        r.transformMatrix[0][2] == l.transformMatrix[0][2] &&
        r.transformMatrix[1][0] == l.transformMatrix[1][0] &&
        r.transformMatrix[1][1] == l.transformMatrix[1][1] &&
        r.transformMatrix[1][2] == l.transformMatrix[1][2] &&
        r.transformMatrix[2][0] == l.transformMatrix[2][0] &&
        r.transformMatrix[2][1] == l.transformMatrix[2][1] &&
        r.transformMatrix[2][2] == l.transformMatrix[2][2] &&
        memcmp(r.data, l.data, l.size) == 0)
        return true;
    else
        return false;
}
