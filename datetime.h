#pragma once 

#include <time.h>
#include <string.h>
#include <stdlib.h>

struct session_time
{
    //char login_duration[50];
    char logout_time[50];
    char login_time[50];
};



struct session_time *show_time(time_t login_time , time_t logout_time );
void set_login_time(time_t login_time , char *login , struct session_time *time_data);
void set_logout_time(time_t logout_time , char *logout , struct session_time *time_data);

