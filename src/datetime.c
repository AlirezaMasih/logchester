#include "../include/datetime.h" // Header file for date and time related functions

// Function to format login and logout times based on the type of session (still, crash, or down)
struct session_time *show_time(time_t login_time, time_t logout_time , short status)
{
    struct session_time *time_data = malloc(sizeof(struct session_time)); // Allocate memory for the session_time structure
    
    // Buffers to hold the formatted login and logout times
    char login[19];  // Buffer to hold the formatted login time
    char logout[30]; // Buffer to hold the formatted logout time
    strcpy(time_data->login_duration , "\0");

    // If logout_time is 0, the session is still ongoing
    if(status == 0)
    {
        set_login_time(login_time, time_data); // Format the login time
        strcpy(time_data->logout_time, "still"); // Set logout time as "still" indicating the session is still ongoing
        strcpy(time_data->login_duration , "logged in"); // Indicate that the user is still logged in
    }
    // If logout_time is 1, the session ended due to a crash
    else if(status == 1)
    {
        set_login_time(login_time, time_data); // Format the login time
        strcpy(time_data->logout_time, "crash"); // Set logout time as "crash" indicating session ended due to crash
        login_amount(time_data , login_time , logout_time); // Calculate the duration of the session, even if it was a crash
    }
    // If logout_time is 2, the session ended due to the system shutting down
    else if(status == 2)
    {
        set_login_time(login_time, time_data); // Format the login time
        strcpy(time_data->logout_time, "down"); // Set logout time as "down" indicating session ended due to shutdown
        login_amount(time_data , login_time , logout_time); // Calculate the session duration during shutdown
    }
    // Otherwise, format both login and logout times normally
    else
    {
        set_login_time(login_time, time_data); // Format the login time
        set_logout_time(logout_time, time_data); // Format the logout time
        login_amount(time_data , login_time , logout_time); // Calculate the duration between login and logout
    }

    // Return the populated session_time structure
    return time_data;
}

// Function to set the formatted login time in the session_time structure
void set_login_time(time_t login_time, struct session_time *time_data)
{
    struct tm *login_tm = localtime(&login_time); // Convert login_time to a struct tm for local time representation
    strftime(time_data->login_time, 18, "%a %b %e %H:%M", login_tm); // Format the login time into the 'login' buffer
}        

// Function to set the formatted logout time in the session_time structure
void set_logout_time(time_t logout_time, struct session_time *time_data)
{
    struct tm *logout_tm = localtime(&logout_time); // Convert logout_time to a struct tm for local time representation
    strftime(time_data->logout_time, 28, "%H:%M", logout_tm); // Format the logout time into the 'logout' buffer
}

// Function to calculate and format the login duration between login and logout times
void login_amount(struct session_time *time_data , time_t login_time , time_t logout_time)
{
    uint8_t day = 0;
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t second = 0;

    time_t duration_time;  // Variable to hold the total duration

    // Check for error case where logout time is earlier than login time
    if(logout_time < login_time)
    {
        strcpy(time_data->login_duration , "(error)"); // Indicate an error if logout time is before login time
    }
    else
    {
        // Calculate the total session duration by subtracting login_time from logout_time
        duration_time = logout_time - login_time;
        
        // Convert total duration into days, hours, minutes, and seconds
        day = (uint8_t)(duration_time / 86400);  // Days (86400 seconds in a day)
        duration_time = duration_time % 86400;  // Remaining time after subtracting full days
        hour = (uint8_t)(duration_time / 3600);  // Hours (3600 seconds in an hour)
        duration_time = duration_time % 3600;   // Remaining time after subtracting full hours
        minute = (uint8_t)(duration_time / 60);  // Minutes (60 seconds in a minute)
        second = (uint8_t)(duration_time % 60);  // Remaining seconds

        // Format the login duration into a human-readable format
        if(day > 0)
        {
            // If the session is longer than a day, show the duration as (days+hours:minutes:seconds)
            sprintf(time_data->login_duration , "(%d+%d:%d:%d)" , day , hour , minute , second);
        }
        else if(duration_time < 86400)
        {
            // If the session is less than a day, show the duration as (hours:minutes:seconds)
            sprintf(time_data->login_duration , "(%d:%d:%d)" , hour , minute , second); 
        }
    }
}

