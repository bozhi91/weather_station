 
#ifndef __MEMORY_CARD_MODULE__
#define __MEMORY_CARD_MODULE__

    char initSDCard(void);
    int fread(const char *path, unsigned char* fileBuffer, unsigned long* fSize, bool isBinary);
    int fwrite(const char *path, const char* mode, unsigned char* fileBuffer, unsigned long nBytes);
    int sd_Status(void);
    int getFileSize(const char *path);

#endif
