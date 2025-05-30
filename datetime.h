// Define the structure to hold session time information
#pragma once 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

struct session_time
{
    // Store the formatted login time as a string
    char logout_time[28]; // Holds the formatted logout time (e.g., still, crash, down, or actual time)
    char login_time[19];  // Holds the formatted login time
    char login_duration[50];    //Holds the formatted time for how long a user was logged in
};

struct login_duration
{
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

// Function declaration to format and return both login and logout times in a session_time structure
struct session_time *show_time(time_t login_time, time_t logout_time , short status);

// Function to set the formatted login time in a session_time structure
void set_login_time(time_t login_time, struct session_time *time_data);

// Function to set the formatted logout time in a session_time structure
void set_logout_time(time_t logout_time, struct session_time *time_data);

void login_amount(struct session_time *time_data , time_t login_time , time_t logout_time);

