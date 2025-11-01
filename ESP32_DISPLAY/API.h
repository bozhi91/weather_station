 
#ifndef __API__
#define __API__

    typedef struct {
        char ssid[30];
        char pass[30];
        int refresh_time;
        char lang[2];
        int timezone;  //currnet timezone shift relavive to the UTC. (+/-)1,2,3
        unsigned char time_format;
        unsigned char date_format;
        unsigned char temp_units;
    }Config;

    typedef struct{
        int max_temp;
        int min_temp;
        char condition[10];
        int cond_id;
        char date[6];
    }Current_weather;

    typedef struct {
        char date[12];
        char time[12];
    } DateTime; 

    void readWeatherAPI(Current_weather* current);
    void readTimeAPI(DateTime* date);
    
    int loadDeviceConfig(void);
    void getDeviceConfig(Config* conf);
    void shell(void);
    
#endif
