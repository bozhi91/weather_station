 
#ifndef __WIFI_MODULE__
#define __WIFI_MODULE__


    void initWifi(void);
    char remoteConnCheck(void);
    int getHttpData(char* url, char* outData);
    int getConnStatus(void);

#endif
