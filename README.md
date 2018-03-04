## How to use bmp2bin.c

In case you intend to use your own images on MARS, using 1-byte per color in the Bitmap display.

Usage Instructions:
 
- Compile the source-code using (preferrably) "clang -O0 -std=c11 bmp2bin.c -o bmp2bin"
- Save your image on Paint as a 24-bit Bitmap file, all RGB colors will be converted to 8-bit ones. This part is prefered to be done on Windows because I am not sure if different softwares use different headers for a 24-bit bitmap file.
- Execute the program doing ./bmp2bin yourimage.bmp yourimage.bin
- Now only the raw bitmap of your image is stored in the binary file, load it with syscalls and store it on the VGA memory as you want.
