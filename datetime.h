#pragma once

#include <time.h>  // For handling time-related operations
#include <string.h> // For string manipulation (e.g., strcpy, strncpy)
#include <stdlib.h> // For memory allocation functions (e.g., malloc)

// Define the structure to hold session time information
struct session_time
{
    // Store the formatted login time as a string
    char logout_time[50]; // Holds the formatted logout time (e.g., still, crash, down, or actual time)
    char login_time[50];  // Holds the formatted login time
};

// Function declaration to format and return both login and logout times in a session_time structure
struct session_time *show_time(time_t login_time, time_t logout_time);

// Function to set the formatted login time in a session_time structure
void set_login_time(time_t login_time, char *login, struct session_time *time_data);

// Function to set the formatted logout time in a session_time structure
void set_logout_time(time_t logout_time, char *logout, struct session_time *time_data);

