#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <jpeglib.h>
#include <png.h>
#include <math.h>
#define BITSPERPIXEL 24
#define BYTESPERPIXEL 3
#define SIZE 10
#define RAW 1
#define RGB565 2
#define RGB888 1
#define BGR888 6
#define RGBA8888 7
#define BMP 3
#define JPEG 4
#define PNG 5
#pragma pack(push,1)

/* Bmp header structure */
typedef struct
{                                                                                                                                                               
	unsigned short int  type;						// Magic identifier
    unsigned int        filesize;					// File size in bytes
    unsigned short int  reserved1, reserved2;
    unsigned int        offset;						// Offset to image data, bytes
    unsigned int        headersize;					// Header size in bytes
    int             	width,height;				// Width and height of image
    unsigned short int  planes;						// Number of colour planes
    unsigned short int  bits;						// Bits per pixel
	unsigned int        compression;				// Compression type
    unsigned int        imagesize;  				// Image size in bytes
    int             	xresolution,yresolution;    // Pixels per meter
    unsigned int        no_of_colours;				// Number of colours
    unsigned int        importantcolours;			// Important colours
}HEADER;
