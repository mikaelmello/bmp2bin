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

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./copy [bitmap file input] [binary file output]\ne.g.:  ./copy monster.bmp monster.bin\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[1];
    char* outfile = argv[2];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "wb");
    if (outptr == NULL)
    {
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

    // Checks the file format (8-bit bitmap)
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 1078 || bi.biSize != 40 || 
        bi.biBitCount != 8 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format or different bitmap type.\n");
        return 4;
    }
    
    // If width is not divisible by 4, a padding is present, so we must ignore it.
    int realWidth = (bi.biWidth % 4 ? bi.biSizeImage / bi.biHeight : bi.biWidth);
    
    // Jumps to the raw bitmap
    rewind(inptr);
    fseek(inptr, 1078, SEEK_SET);
    
    // Reads all the raw bitmap into an array.
    BYTE rawBitmap[bi.biSizeImage];
    fread(rawBitmap, 1, bi.biSizeImage, inptr);
    
    // Iterates from the last line to the first (since it is stored upside-down)
    int index;
    for (int i = bi.biHeight-1; i >= 0; i--) {
        index = realWidth * i;
        for (int j = 0; j < bi.biWidth; j++) {
            
            /* For some reason Paint stores true RGB colors as these three
             * handled in the ifs/elses, so I had to manually change them
             * in order to save fidelity to the original colors.
             * Haven't tested what would happen with the real 0xfc, 0xf9 and 0xfa colors.
             */
            if (rawBitmap[index+j] == 0xfc) rawBitmap[index+j] = 0xc0;
            else if (rawBitmap[index+j] == 0xf9) rawBitmap[index+j] = 0x07;
            else if (rawBitmap[index+j] == 0xfa) rawBitmap[index+j] = 0x38;
        }
        fwrite(&rawBitmap[index], 1, bi.biWidth, outptr);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
