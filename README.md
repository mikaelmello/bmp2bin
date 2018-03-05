# bmp2bin

This tool was created to make it easier for students to upload their own images into Assembly MIPS' memory mapped display, since it only recognizes raw binary data. So it strips the raw bitmap from a bitmap file by removing the headers and bytes that are for padding or are out of order.

## Installation

```bash
$ git clone https://github.com/MikaelMello/bmp2bin.git
$ cd bmp2bin
$ make
```

bmp2bin is installed on `~/bin/bmp2bin` and you can use it by executing the command `bmp2bin`.

## How to use bmp2bin

In case you intend to use your own images on MARS, using 1-byte per color in the Bitmap display.

Usage Instructions:
 
- `bmp2bin image.bmp image.bin`
- Now you can load your binary file into MARS.

**Note**: For now, only 24-bits (RGB colors) bitmap files are supported.
**Note**: For now, all bitmap files should be saved via Microsoft Window's Paint because some other softwares have different headers and this may lead `bmp2bin` to have undefined behavior.
