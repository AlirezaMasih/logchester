// Define the structure to hold session time information
#pragma once // Ensures this header is included only once during compilation.

#include <stdio.h>  // Standard input/output library for I/O operations
#include <stdlib.h> // Standard library for memory allocation and general functions
#include <stdint.h> // Library for fixed-width integer types (e.g., uint8_t)
#include <string.h> // Library for string manipulation functions
#include <time.h>   // Library for time-related functions and types

// Structure to store session time information (login, logout, and duration)
struct session_time
{
    // Holds the formatted logout time (e.g., "still", "crash", "down", or actual time)
    char logout_time[28]; 
    
    // Holds the formatted login time (e.g., "Mon Oct  5 15:30")
    char login_time[19];  
    
    // Holds the formatted login duration (e.g., "1+2:30:15" for 1 day, 2 hours, 30 minutes, 15 seconds)
    char login_duration[50];    
};

// Structure to store the duration of a session in days, hours, minutes, and seconds
struct login_duration
{
    uint8_t day;    // Number of days the user was logged in
    uint8_t hour;   // Number of hours the user was logged in
    uint8_t minute; // Number of minutes the user was logged in
    uint8_t second; // Number of seconds the user was logged in
};

// Function declarations:

// Function to format login and logout times and return them in a session_time structure
struct session_time *show_time(time_t login_time, time_t logout_time , short status);

// Function to format and set the login time in a session_time structure
void set_login_time(time_t login_time, struct session_time *time_data);

// Function to format and set the logout time in a session_time structure
void set_logout_time(time_t logout_time, struct session_time *time_data);

// Function to calculate and format the login duration between login and logout times
void login_amount(struct session_time *time_data , time_t login_time , time_t logout_time);

