## CONTENTS

- [OVERVIEW](#overview)
- [API DESCRIPTION](#api-description)
  - [CONSTRUCTOR AND DESTRUCTOR](#constructor-and-destructor)
  - [getVersion(..)](#getversion)
  - [cloneTo(...)](#cloneto)
  - [release(..)](#release)
  - [serialize(...)](#serialize)
  - [deserialize(...)](#deserialize)
  - [operator ==](#operator-==)
  - [public members](#public-members)
  

## OVERVIEW

This project includes base video frame class which will be used
in other projects.

## API DESCRIPTION

The main file **Frame.h** contains one class `Frame` which can be used 
to simplify creating and copying video frames in various projects. 

### CONSTRUCTOR AND DESTRUCTOR

```cpp
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
 * @param frameId ID of frame.
 * @param sourceId ID of video source.
 */
Frame(uint32_t width,
      uint32_t height,
      Fourcc fourcc,
      uint32_t size = 0,
      uint8_t* data = nullptr,
      uint32_t frameId = 0,
      uint32_t sourceId = 0);
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
 * @param src Source fram object.
 */
Frame& operator= (const Frame& src);
```

##### Description

*Class constructors and destructor*

### getVersion(...)

```cpp
/**
 * @brief Get string of current class version.
 * @return String of current class version.
 */
static std::string getVersion();
```

##### Description

*Static method to get string of current version of Frame class.*

### cloneTo(...)

```cpp
/**
 * @brief Clone data. Method copies frame and copy just pointer to data.
 * @param dst Output frame.
 */
void cloneTo(Frame& dst);
```

##### Description

*Method copies frame atributes and copy pointer to frame data instead of copy data.*

### release(..)

```cpp
/**
 * @brief Free frame memory.
 */
void release();
```

##### Description

*Method to free frame memory.*

### serialize(...)

```cpp
/**
 * @brief Serialize frame data. The method will encode data with params.
 * @param data Pointer to data buffer.
 * @param size Size of serialized data.
 */
void serialize(uint8_t* data, int& size);
```

##### Description

*Method to serialize frame data into buffer.*

### deserialize(...)

```cpp
/**
 * @brief Deserialize data data to frame object.
 * @param data Pointer to serialized data.
 * @param size Size of serialized data.
 * @return TRUE if the data deserialized or FALSE.
 */
bool deserialize(uint8_t* data, int size);
```

##### Description

*Method to deserialize serialized frame data.*

### operator ==

```cpp
/**
 * @brief Operator "==". Operator to compare two frame objects.
 * @param src Source frame object.
 * @return TRUE if the frames are identical or FALSE.
 */
bool operator== (const Frame& src);
```

##### Description

*Operator to compare two frame objects.*

### Public members

```cpp
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
/// Transformation matrix.
float transformMatrix[3][3]; 
```
