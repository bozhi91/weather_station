 
#ifndef __SHELL_MODULE__
#define __SHELL_MODULE__


    typedef struct{
        char* command;
        char* description;
        void (*f_ptr)(void);
    }CMD_SHELL;

    void help(void);
    void push(void);        //move to sdcard.cpp
    void displTest(void);   //move to display.cpp
    void sysInfo(void);
    void init_shell(void);

#endif
