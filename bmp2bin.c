/**
 * Mikael Mello
 * 
 * bmp2bin.c
 * 23/06/2016
 * 
 * Takes a 8-bit colored bitmap file and converts it to a binary file
 * with only the raw bitmap stored. Since bitmaps are stored "upside-down",
 * this is also fixed.
 * 
 * Created to upload images on the MARS Simulator (Assembly MIPS).
 * 
 * Adapted from CS50's problem-set 4 
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[]) {
    // ensure proper usage
    if (argc != 3) {
        printf("Usage: ./copy [bitmap file input] [binary file output]\ne.g.:  ./copy monster.bmp monster.bin\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[1];
    char* outfile = argv[2];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL) {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "wb");
    if (outptr == NULL) {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Checks the file format (24-bit bitmap)
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0) {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format or different bitmap type.\n");
        return 4;
    }
    
    // If width is not divisible by 4, a padding is present, so we must ignore it.
    int realWidth = ((bi.biWidth*3 + 3) / 4)*4;
    
    // Jumps to the raw bitmap
    rewind(inptr);
    fseek(inptr, bf.bfOffBits, SEEK_SET);
    bi.biSizeImage = bi.biWidth * bi.biHeight * 3;
    
    // Reads all the raw bitmap into an array.
    BYTE rawBitmap[bi.biSizeImage];
    fread(rawBitmap, 1, bi.biSizeImage, inptr);
    
    // Iterates from the last line to the first (since it is stored upside-down)
    int index;
    for (int i = bi.biHeight-1; i >= 0; i--) {
        index = realWidth * i;
        for (int j = 0; j < bi.biWidth; j++) {
            
            RGBTRIPLE rgb;
            rgb.rgbtBlue = rawBitmap[index+j*3];
            rgb.rgbtGreen = rawBitmap[index+j*3+1];
            rgb.rgbtRed = rawBitmap[index+j*3+2];

            BYTE newByte = ((rgb.rgbtBlue * 3 / 255) << 6) + ((rgb.rgbtGreen * 7 / 255) << 3) + (rgb.rgbtRed * 7 / 255);    
            
            fwrite(&newByte, 1, sizeof(BYTE), outptr);          
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}

