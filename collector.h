#pragma once

#include <stdio.h> // For I/O operations (input/output)
#include <stdlib.h> // For memory management (malloc, realloc, free)
#include <utmp.h> // For reading utmp files (system logs)
#include <time.h> // For handling time and making time fields human-readable
#include <string.h> // For string manipulation functions
#include <paths.h> // For standard system paths
#include <stdint.h> // For using fixed-width integers


// Define the structure 'session_data' to hold the session information
struct session_data
{
    char ut_user[11]; // Store the user name (up to 10 characters)
    char ut_host[26]; // Store the host name (up to 25 characters)
    char ut_line[26]; // Store the terminal line (up to 25 characters)
    short ut_type; // Store the type of the entry (e.g., login, logout, etc.)
    uint32_t tv_sec; // Store the time (in seconds) of the session (UNIX timestamp)
};

// Define the structure 'utmp_data' to store the list of entries and its length
struct utmp_data
{
    int length; // Store the number of entries in the array
    struct session_data *data; // Pointer to the array storing the 'session_data' structures
};

// Function declaration to read the utmp file and return the data in a 'utmp_data' structure
struct utmp_data *read_file(char *path);

