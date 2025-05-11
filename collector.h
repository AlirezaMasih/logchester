#pragma once

#include <stdio.h> // For I/O operations (input/output)
#include <stdlib.h> // For memory management (malloc, realloc, free)
#include <utmp.h> // For reading utmp files
#include <time.h> // For making time field human readable
#include <string.h>
#include <paths.h>
#include <stdint.h>

// Define the structure utmp_data to store the list of entries and its length
struct utmp_data
{
    struct utmp *data; // Array to store the utmp structures
    int length; // Number of entries in the array
};

// Function declaration to read the utmp file and return the data in a utmp_data structure
struct utmp_data *read_file(char *path);
void show_time(uint32_t *input_time , char *time_buffer_ptr , size_t size);


