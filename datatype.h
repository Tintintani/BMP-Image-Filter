#include <stdint.h>

typedef uint8_t BYTE;
typedef uint16_t USHORTINT;
typedef uint32_t USNGDINT;
typedef int32_t NORMALINT;

// BITMAP FILE HEADER
typedef struct
{
    USHORTINT BFHSIGN;
    USNGDINT BFHSIZE;
    USHORTINT BFHRESERVED1;
    USHORTINT BFHRESERVED2;
    USNGDINT BFHIMGSTRT;
} __attribute__((__packed__))
BMPFileHeader;

// BITMAP INFO HEADER
typedef struct
{
    USNGDINT BIHSIZE;
    NORMALINT WIDTH;
    NORMALINT HEIGHT;
    USHORTINT PLANES;
    USHORTINT COLOURDEPTH;
    USNGDINT BIHCOMP;
    USNGDINT IMGSIZE;
    NORMALINT X_PXL_PERMETER;
    NORMALINT Y_PXL_PERMETER;
    USNGDINT COLORNO;
    USNGDINT IMPCOLORNO;
} __attribute__((__packed__))
BMPInfoHeader;

// STRUCTURE OF A PIXEL
typedef struct
{
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
} RGB;

typedef struct
{
    float rgbtBlue;
    float rgbtGreen;
    float rgbtRed;
} RGB1;
