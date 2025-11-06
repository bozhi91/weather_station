 
#ifndef __WIFI_MODULE__
#define __WIFI_MODULE__

    #define WIFI_CONN_TIMEOUT   5000

    int initWifi(void);
    char remoteConnCheck(void);
    int getHttpData(char* url, char* outData);
    int getConnStatus(void);
    void getNetworkInfo(void);

#endif
