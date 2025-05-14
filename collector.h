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
    char ut_user[33];
    char ut_host[33];
    char ut_line[33];
    char login_amount[10];
    char logout[10];
    short ut_type;
    uint32_t tv_sec;
};

// Define the structure utmp_data to store the list of entries and its length
struct utmp_data
{
    int length; // Number of entries in the array
    struct session_data *data; // Array to store the utmp structures
};

// Function declaration to read the utmp file and return the data in a utmp_data structure
struct utmp_data *read_file(char *path);
void show_time(uint32_t *input_time , char *time_buffer_ptr , size_t size);


