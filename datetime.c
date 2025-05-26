#include "datetime.h"

// Function to format login and logout times based on the type of session (still, crash, or down)
struct session_time *show_time(time_t login_time, time_t logout_time)
{
    struct session_time *time_data = malloc(sizeof(struct session_time)); // Allocate memory for the session_time structure
    
    char login[19];  // Buffer to hold the formatted login time
    char logout[30]; // Buffer to hold the formatted logout time

    // If logout_time is 0, the session is still ongoing
    if(logout_time == 0)
    {
        set_login_time(login_time, login, time_data); // Format the login time
        strcpy(time_data->logout_time, "still"); // Set logout time as "still"
    }
    // If logout_time is 1, the session ended due to a crash
    else if(logout_time == 1)
    {
        set_login_time(login_time, login, time_data); // Format the login time
        strcpy(time_data->logout_time, "crash"); // Set logout time as "crash"
    }
    // If logout_time is 2, the session ended due to the system shutting down
    else if(logout_time == 2)
    {
        set_login_time(login_time, login, time_data); // Format the login time
        strcpy(time_data->logout_time, "down"); // Set logout time as "down"
    }
    // Otherwise, format both login and logout times
    else
    {
        set_login_time(login_time, login, time_data); // Format the login time
        set_logout_time(logout_time, logout, time_data); // Format the logout time
    }

    // Return the populated session_time structure
    return time_data;
}

// Function to set the formatted login time in the session_time structure
void set_login_time(time_t login_time, char *login, struct session_time *time_data)
{
    struct tm *login_tm = localtime(&login_time); // Convert login_time to a struct tm for local time representation
    strftime(login, 18, "%a %b %e %H:%M", login_tm); // Format the login time into the 'login' buffer
    strcpy(time_data->login_time, login); // Store the formatted login time in the session_time structure
}        

// Function to set the formatted logout time in the session_time structure
void set_logout_time(time_t logout_time, char *logout, struct session_time *time_data)
{
    struct tm *logout_tm = localtime(&logout_time); // Convert logout_time to a struct tm for local time representation

    strftime(logout, 28, "%H:%M", logout_tm); // Format the logout time into the 'logout' buffer
    strcpy(time_data->logout_time, logout); // Store the formatted logout time in the session_time structure
}

