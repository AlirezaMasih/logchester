#pragma once

#include <stdio.h> // For I/O operations (input/output)
#include <stdlib.h> // For memory management (malloc, realloc, free)
#include <utmp.h> // For reading utmp files
#include <time.h> // For making time field human readable
#include <string.h>
#include <paths.h>
#include <stdint.h>


struct session_data
{
    char ut_user[11];
    char ut_host[26];
    char ut_line[26];
    short ut_type;
    uint32_t tv_sec;
};

// Define the structure utmp_data to store the list of entries and its length
struct utmp_data
{
    int length; // Number of entries in the array
    struct session_data *data; // Array to store the utmp structures
};

struct session_time
{
    //char login_duration[50];
    char logout_time[50];
    char login_time[50];
};

// Function declaration to read the utmp file and return the data in a utmp_data structure
struct utmp_data *read_file(char *path);
struct session_time *show_time(time_t login_time , time_t logout_time );
void set_login_time(time_t login_time , char *login , struct session_time *time_data);
void set_logout_time(time_t logout_time , char *logout , struct session_time *time_data);
