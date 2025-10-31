 
#ifndef __API__
#define __API__

    typedef struct {
        char ssid[30];
        char pass[30];
        int refresh_time;
        char lang[2];
        char timezone[20];
        int time_format;
        char temp_units;
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

    typedef struct{
        char timezone;      //currnet timezone shift relavive to the UTC.(+/-)1,2,3
        char timeFormat;    //12/24 time format. 0:24, 1:12
        char dateFormat;    //0: dd/mm/yyy

    }DateTime_Cfg;

    void readWeatherAPI(Current_weather* current);
    void readTimeAPI(DateTime* date);
    
    int loadConfig(void);
    void getConfig(Config* conf);
    void shell(void);
    
#endif
