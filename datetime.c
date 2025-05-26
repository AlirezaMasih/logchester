#include "datetime.h"

struct session_time *show_time(time_t login_time , time_t logout_time)
{
    struct session_time *time_data = malloc(sizeof(struct session_time));
    
    char login[19];
    char logout[30];
    //char *login_amount = malloc(30 * sizeof(char));

    

    if(logout_time == 0)
    {
        set_login_time(login_time , login , time_data);
        strcpy(time_data->logout_time , "still"); 
        //strcpy(time_data->login_duration , "logged in)");

    }
    else if(logout_time == 1)
    {
        set_login_time(login_time , login , time_data);
        strcpy(time_data->logout_time , "crash");

    }
    else if(logout_time == 2)
    {
        set_login_time(login_time , login , time_data);
        strcpy(time_data->logout_time , "down");
    }
    else
    {
        set_login_time(login_time , login , time_data);
        set_logout_time(logout_time  , logout , time_data);
    }
    //free(login);
    //free(logout);

    return time_data;
    

 
}

void set_login_time(time_t login_time , char *login , struct session_time *time_data)
{
    
    struct tm *login_tm= localtime(&login_time);
    strftime(login , 18 , "%a %b %e %H:%M" , login_tm);
    strcpy(time_data->login_time , login);


}        

    
//struct tm *time_info = localtime(&time);
    
void set_logout_time(time_t logout_time , char *logout , struct session_time *time_data)
{
    struct tm *logout_tm = localtime(&logout_time);
    //struct tm *login_d_tm = localtime(&login_duration);

    strftime(logout , 28 , "%H:%M" , logout_tm);
    //strftime(login_amount , 30 ,"(%H:%M)" , login_d_tm);


    strcpy(time_data->logout_time , logout);
        
}

