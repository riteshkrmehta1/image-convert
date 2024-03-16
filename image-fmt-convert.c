/*******************************************************************************************
 *
 *  CONVERTING RAW TO JPEG ,BMP ,PNG Image format ....
 *  
 ********************************************************************************************/

#include "image-fmt-convert.h"
#define NUM_PLANES 3
#pragma pack(pop)
unsigned char *buffer;

HEADER Bmp_header;

void print_help() {
    printf("Usage: image-fmt-convert [OPTIONS]\n");
    printf("Options:\n");
    printf("  -i, --input    Input file name\n");
    printf("  -o, --output   Output file name\n");
    printf("  -w, --width    Width of the image\n");
    printf("  -h, --height   Height of the image\n");
    printf("  -f, --format   Input image format (RGB888, RGB565, BGR888, RGBA8888)\n");
    printf("  --help         Display this help message\n");
}

/* Checking the input and output file format */
int check_file_format(char *filename)
{
	int i,j,k=0;
	char file_type[SIZE];
	for(i=0;filename[i];i++)
	{
		if(filename[i]=='.')
			break;
	}
	for(j=i+1;filename[j];j++)
		file_type[k++]=filename[j];
	file_type[k]='\0';
	if(strcmp(file_type,"raw")==0)
		return RAW ;
	else if(strcmp(file_type,"bmp")==0)
		return BMP ;
	else if(strcmp(file_type,"jpg")==0)
		return JPEG ;
	else if(strcmp(file_type,"png")==0)
		return PNG ;
	else
		return -1 ;
}

/* Checking the input file format */
int input_image_format(char *fmt)
{
	if(strcmp(fmt,"RGB565")==0)
		return RGB565;
	else if(strcmp(fmt,"RGB888")==0)
		return RGB888;
	else if(strcmp(fmt,"BGR888")==0)
        return BGR888;
	else if(strcmp(fmt,"RGBA8888")==0)
        return RGBA8888;
	else
		return -1;

}

/* Writting png file formate in the output file */
void write_png_file(char *filename , int image_width , int image_height )
{

	FILE *Output_file = fopen(filename, "wb");
	if(!Output_file) abort();
	png_byte color_type;
	png_byte bit_depth;
	png_bytep *row_pointers;
	int y,x,i=0;

	color_type = BYTESPERPIXEL;
	bit_depth  = BITSPERPIXEL;

	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) abort();

	png_infop info = png_create_info_struct(png);
	if (!info) abort();
	if (setjmp(png_jmpbuf(png))) abort();


	png_init_io(png, Output_file);

	// Output is 8bit depth, RGBA format.
	png_set_IHDR(
			png,
			info,
			image_width, image_height,
			8,
			PNG_COLOR_TYPE_RGB,
			PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT,
			PNG_FILTER_TYPE_DEFAULT
		    );

	if(bit_depth == 16)
		png_set_strip_16(png);

	if(color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if(png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	if(color_type == PNG_COLOR_TYPE_GRAY ||
			color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);


	png_write_info(png, info);

	row_pointers = ( png_bytep* ) malloc( sizeof(png_bytep) * image_height);

	for( y = 0; y < image_height; y++) {
		row_pointers[y] = ( png_byte* ) malloc( sizeof(png_byte) * image_width * color_type );
	}

	for(y=0;y< image_height ;y++)
	{
		for( x=0 ; x<(image_width*color_type) ; x++)
			row_pointers[y][x]=buffer[i++];
	}
	png_write_image(png, row_pointers);

	png_write_end(png, NULL);

	for(y = 0; y < image_height; y++) {
		free(row_pointers[y]);
	}
	free(row_pointers);

	if (png && info)
		png_destroy_write_struct(&png, &info);

	fclose(Output_file);
}

/* Writting the jpeg image in the output file */
int write_jpeg_file( char *filename, int image_width , int image_height)
{


	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	int bytes_per_pixel;
	int color_space ; //JCS_RGB;//JCS_UNKNOWN;//JCS_RGB; //JCS_RGB;*/   /*JCS_GRAYSCALE; for grayscale images */


	JSAMPROW row_pointer[1];
	bytes_per_pixel =BYTESPERPIXEL;
	color_space =JCS_RGB;


	FILE *outfile = fopen( filename, "wb" );
	if ( !outfile )
	{
		printf("Error opening output jpeg file %s\n!", filename );
		return -1;
	}
	cinfo.err = jpeg_std_error( &jerr );
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);


	cinfo.image_width = image_width;
	cinfo.image_height = image_height;
	cinfo.input_components = bytes_per_pixel;
	cinfo.in_color_space = color_space;
	//      cinfo.data_precision = bits_per_pixel;
	cinfo.input_gamma = 1.00;
	jpeg_set_defaults( &cinfo );

	jpeg_start_compress( &cinfo, TRUE );

	while( cinfo.next_scanline < cinfo.image_height )
	{
		row_pointer[0] = &buffer[ cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
		jpeg_write_scanlines( &cinfo, row_pointer, 1 );
	}

	jpeg_finish_compress( &cinfo );
	jpeg_destroy_compress( &cinfo );
	fclose( outfile );

	return 1;
}


int write_bmp_file(char *filename ,unsigned long int imagesize ,int image_width ,int image_height)
{
	FILE *OUTPUT_file;
	char *bmp_buffer;
	int pixel;
	OUTPUT_file=fopen(filename,"wb");
	if(OUTPUT_file==NULL)
	{
		printf("file is not opened sucessfully\n");

		return -1;
	}

	/* 54 bytes of bmp header information */
	Bmp_header.type	 		= 0x4d42;
	Bmp_header.reserved1 		= 0;
	Bmp_header.reserved2 		= 0;
	Bmp_header.offset	 	= sizeof(HEADER);
	Bmp_header.headersize	 	= 40;
	Bmp_header.planes	 	= 1;
	Bmp_header.compression 		= 0;
	Bmp_header.no_of_colours	= 0;
	Bmp_header.importantcolours	= 0;
	Bmp_header.xresolution		= 0;
	Bmp_header.yresolution		= 0;
	Bmp_header.bits			= BITSPERPIXEL;
	Bmp_header.width		= image_width;
	Bmp_header.height		= image_height;
	Bmp_header.imagesize 		=  (image_width * image_height * BYTESPERPIXEL);
	Bmp_header.filesize		= Bmp_header.imagesize + sizeof(HEADER);

	bmp_buffer=(char *) malloc(imagesize *sizeof(char));
	if( bmp_buffer == NULL)
	{
		printf("sufficient memory not available\n");
		fclose(OUTPUT_file);
		return -1;
	}
	// Copy data while considering padding
    int row_size_bytes = image_width * BYTESPERPIXEL;
    int padded_row_size_bytes = (row_size_bytes + 3) & ~3; // Round up to multiple of 4
    int padding_bytes_per_row = padded_row_size_bytes - row_size_bytes;

    for (pixel = 0; pixel < image_height; ++pixel) {
        int row_start_index_src = pixel * row_size_bytes;
        int row_start_index_dest = pixel * padded_row_size_bytes;

        // Copy pixel data
        for (int i = 0; i < row_size_bytes; i += BYTESPERPIXEL) {
            bmp_buffer[row_start_index_dest + i ] = buffer[row_start_index_src + i + 2];     // Blue component
            bmp_buffer[row_start_index_dest + i + 1] = buffer[row_start_index_src + i + 1]; // Green component
            bmp_buffer[row_start_index_dest + i + 2] = buffer[row_start_index_src + i];     // Red component
        }

        // Add padding bytes
        for (int i = 0; i < padding_bytes_per_row; ++i) {
            bmp_buffer[row_start_index_dest + row_size_bytes + i] = 0;
        }
    }
	fwrite(&Bmp_header,sizeof(HEADER),sizeof(char),OUTPUT_file);
	fwrite(bmp_buffer, image_width * image_height * BYTESPERPIXEL, sizeof(char),OUTPUT_file);

	free(bmp_buffer);
	fclose(OUTPUT_file);
	return 1;
}

unsigned char * rgba8888_to_rgb888(int width ,int height , int size )
{
	int i,j;
	unsigned char *rgb888frame = malloc(height * width * NUM_PLANES * sizeof(unsigned char));

	for (i = 0, j = 0; i < size; i += 4, j += 3) {
		rgb888frame[j]     = buffer[i];
		rgb888frame[j + 1] = buffer[i + 1];
		rgb888frame[j + 2] = buffer[i + 2];
	}
	return rgb888frame;

}

unsigned char * bgr888_to_rgb888(int size )
{
	int i;
	uint8_t temp;

	for (i = 0; i < size; i += 3) {
		temp = buffer[i];
		buffer[i] = buffer[i + 2];
		buffer[i + 2] = temp;
	}
	return buffer;
}
unsigned char * rgb565_to_rgb888(int width ,int height , int size )
{
	unsigned char *rgb888frame = malloc(height * width * NUM_PLANES * sizeof(unsigned char));
	bzero(rgb888frame,height * width * NUM_PLANES);
	int i,c=0;
	uint8_t r5, g6, b5, r8, g8, b8;
	uint16_t rgb565;

	for (i = 0; i < size; i += 2) {
		rgb565 = (buffer[i]) | (buffer[i + 1] << 8);
		r5 = (rgb565 >> 11) & 0x1F;
		g6 = (rgb565 >> 5) & 0x3F;
		b5 = rgb565 & 0x1F;

		r8 = (r5 * 255 + 15) / 31;
		g8 = (g6 * 255 + 31) / 63;
		b8 = (b5 * 255 + 15) / 31;

		rgb888frame[c++] = r8;
		rgb888frame[c++] = g8;
		rgb888frame[c++] = b8;
	}
	return rgb888frame;
}
int main(int argc ,char *argv[] )
{

	unsigned long int width, height, size;
	int input_file_format, output_file_format;
	FILE *INPUT_file;
	char output_file[100];

	if (argc < 2) {
        printf("Error: No options provided. Use --help for usage information.\n");
        return 1;
    }

	// Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        } else if ((strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) && i + 1 < argc) {
            // Handle input file option
            INPUT_file = fopen(argv[++i], "rb");
            if (INPUT_file == NULL) {
                printf("Error: Failed to open input file.\n");
                return 1;
            }
        } else if ((strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0) && i + 1 < argc) {
            width = atoi(argv[++i]);
        } else if ((strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--height") == 0) && i + 1 < argc) {
            height = atoi(argv[++i]);
        } else if ((strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--format") == 0) && i + 1 < argc) {
            // Handle format option
            const char *format = argv[++i];
            if (strcmp(format, "RGB565") == 0) {
                input_file_format = RGB565;
            } else if (strcmp(format, "BGR888") == 0) {
                input_file_format = BGR888;
            } else if (strcmp(format, "RGBA8888") == 0) {
                input_file_format = RGBA8888;
            } else if (strcmp(format, "RGB888") == 0) {
                input_file_format = RGB888;
            } else {
                printf("Error: Invalid input image format.\n");
                return 1;
            }
        } else if ((strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) && i + 1 < argc) {
			strcpy(output_file, argv[++i]);
        } else {
            printf("Error: Unknown option or missing argument: %s\n", argv[i]);
            return 1;
        }
    }


	fseek(INPUT_file,0,SEEK_END);
	size=ftell(INPUT_file);
	rewind(INPUT_file);

	buffer = (unsigned char *)malloc(sizeof(unsigned char) * (size));
	if(buffer == NULL)
	{
		printf("Sufficient memory not available\n");
		fclose(INPUT_file);
		return -1;
	}

	/* Checking output file format like jpg , bmp ,png */
	output_file_format= check_file_format(output_file);
	if(input_file_format < 1)
	{
		printf("Failed !!! Check Input file format\n");
		fclose(INPUT_file);
		free(buffer);
		return -1;
	}

	/* Reading data of input file */
	fread(buffer,size,sizeof(char),INPUT_file);

	/*If input  file is yuv422 format then converted to RGB24 format */
	if(input_file_format == RGB565)
		buffer = rgb565_to_rgb888(width , height, size );
	else if (input_file_format == BGR888)
		buffer = bgr888_to_rgb888(size);
	else if (input_file_format == RGBA8888)
		buffer = rgba8888_to_rgb888(width , height, size );

	/* For converting Input file to the bmp file format */
	if(output_file_format == BMP )
		write_bmp_file( output_file , size , width , height);

	/* For converting Input file to the jpeg file format */
	if(output_file_format == JPEG)
		write_jpeg_file(output_file, width , height);
	/* For converting Input file to the png file format */
	if(output_file_format == PNG)
		write_png_file(output_file ,width , height);

	fclose(INPUT_file);
	free(buffer);

}
