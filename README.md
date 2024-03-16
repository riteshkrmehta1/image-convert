# Introduction
The Image Format Converter is a command-line tool written in C that allows users to convert image files from one format to another. It supports various input image formats, including RGB565, BGR888, and RGBA8888, and can output images in BMP, JPEG, or PNG formats.

## Features
* Converts images from RGB888, RGB565, BGR888, and RGBA8888 formats to BMP, JPEG, or PNG formats.
* Provides command-line options for specifying input and output file names, image dimensions, and input image format.
* Supports the --help option to display usage information.

## Prerequisite
* Make sure you have a C compiler installed on your system. This project is developed and tested with gcc compiler.
* This project uses standard C libraries and does not require any external dependencies.
* Install the jpeg and png library which is getting used for generating jpg & png image respectively
```
sudo apt install libpng-dev
sudo apt install libjpeg-dev
```

## Installation
* Clone the repository to your local machine:
```
git clone git@github.com:riteshkrmehta1/image-convert.git
```
* Navigate to the project directory:
```
cd image-convert
```
* Compile the source code:
```
make 
```
* Clean the project build code:
```
make clean
```

## Usage
To convert an image, use the following command-line to provide input RGB formats and get final png/jpeg/bmp image:
* For output BMP image
```
./image-fmt-convert -i <input_file.rgb> -o <output_file.bmp> -w <width> -h <height> -f <input_format>
```
* For output JPEG image
```
./image-fmt-convert -i <input_file.rgb> -o <output_file.jpg> -w <width> -h <height> -f <input_format>
```
* For output PNG image
```
./image-fmt-convert -i <input_file.rgb> -o <output_file.png> -w <width> -h <height> -f <input_format>
```
* Example:
```
./image-fmt-convert -i image_1280_720_rgb565.rgb -o image_1280_720_rgb565.png -w 1280 -h 720 -f RGB565
```

## Options
```
-i, --input: Input file name.
-o, --output: Output file name.
-w, --width: Width of the image.
-h, --height: Height of the image.
-f, --format: Input image format (RGB888, RGB565, BGR888, RGBA8888).
--help: Display usage information.
```
## Contributing
Contributions are welcome! Feel free to open an issue or submit a pull request for any improvements or bug fixes.

