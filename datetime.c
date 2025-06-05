#include "datetime.h"

// Function to format login and logout times based on the type of session (still, crash, or down)
struct session_time *show_time(time_t login_time, time_t logout_time , short status)
{
    struct session_time *time_data = malloc(sizeof(struct session_time)); // Allocate memory for the session_time structure
    
    char login[19];  // Buffer to hold the formatted login time
    char logout[30]; // Buffer to hold the formatted logout time


    // If logout_time is 0, the session is still ongoing
    if(status == 0)
    {
        set_login_time(login_time, time_data); // Format the login time
        strcpy(time_data->logout_time, "still"); // Set logout time as "still"
        strcpy(time_data->login_duration , "logged in");
    }
    // If logout_time is 1, the session ended due to a crash
    else if(status == 1)
    {
        set_login_time(login_time, time_data); // Format the login time
        strcpy(time_data->logout_time, "crash"); // Set logout time as "crash"
        login_amount(time_data , login_time , logout_time); 
    }
    // If logout_time is 2, the session ended due to the system shutting down
    else if(status == 2)
    {
        set_login_time(login_time, time_data); // Format the login time
        strcpy(time_data->logout_time, "down"); // Set logout time as "down"
        login_amount(time_data , login_time , logout_time);  
    }
    // Otherwise, format both login and logout times
    else
    {
        set_login_time(login_time, time_data); // Format the login time
        set_logout_time(logout_time, time_data); // Format the logout time
        login_amount(time_data , login_time , logout_time);
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

void login_amount(struct session_time *time_data , time_t login_time , time_t logout_time)
{

    uint8_t day = 0;
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t second = 0;

    time_t duration_time;

    if(logout_time < login_time)
    {
        strcpy(time_data->login_duration , "(error)");

    }
    // Otherwise, format both login and logout times
    else
    {

        duration_time = logout_time - login_time;
        day = (uint8_t)(duration_time / 86400);
        duration_time = duration_time % 86400;
        hour = (uint8_t)(duration_time / 3600);
        duration_time = duration_time % 3600;
        minute = (uint8_t)(duration_time / 60);
        second = (uint8_t)(duration_time % 60);

            
        if(day > 0)
        {
            sprintf(time_data->login_duration , "(%d+%d:%d:%d)" , day , hour , minute , second);
        }
        else if(duration_time < 86400)
        {
            sprintf(time_data->login_duration , "(%d:%d:%d)" , hour , minute , second); 
        }


    }


}
