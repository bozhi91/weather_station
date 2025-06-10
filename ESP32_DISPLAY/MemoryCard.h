 
#ifndef __MEMORY_CARD_MODULE__
#define __MEMORY_CARD_MODULE__

    char initSDCard(void);
    int fread(const char *path, unsigned char* fileBuffer, unsigned long* fSize, bool isBinary);
    int fwrite(const char *path, const char* mode, unsigned char* fileBuffer, unsigned long nBytes);
    char sd_Status(void);
    
#endif
