#include "collector.h"

// Function to read the utmp file and return a structure containing the data and its length
struct utmp_data *read_file(char *path)
{
    // Allocate memory for the utmp_data structure to store the data and its length
    struct utmp_data *Returning_Data = malloc(sizeof(struct utmp_data));
    struct utmp *utmp_entry = NULL; // Pointer to hold each individual utmp entry
    struct session_data *entry_list = NULL; // Array to store all the entries read from the file
    struct session_data utmp_entry_formatted;

    //strcpy(utmp_entry_formatted.logout , "still");
    //strcpy(utmp_entry_formatted.login_amount , "login");

    int count = 0; // Keep track of the number of entries
    uint32_t offset;

    // Open the utmp file and prepare to read from it
    utmpname(path);  // Open the wtmp file from /var/log as a read-only file
                     
    setutent(); // Start reading the data from the utmp file

    
    // Loop to read each entry from the file until the end
    while((utmp_entry = getutent()) != NULL)
    {
        // Reallocate memory to accommodate the next entry in the list
        entry_list = realloc(entry_list, (count + 1) * sizeof(struct session_data));
        if(entry_list == NULL)
        {
            // If realloc fails, free the previously allocated memory and print an error
            free(entry_list);
            free(Returning_Data);
            perror("Memory allocation failed.");
            endutent();
            return NULL;
        }    
                
        // Store the current utmp entry into the list
      
        //show_time(&utmp_entry->tv_sec , time_buffer , sizeof(time_buffer));
        strncpy(utmp_entry_formatted.ut_user , utmp_entry->ut_user , 10); 
        strncpy(utmp_entry_formatted.ut_host , utmp_entry->ut_host , 25); 
        strncpy(utmp_entry_formatted.ut_line , utmp_entry->ut_line , 25); 
        utmp_entry_formatted.ut_type = utmp_entry->ut_type; 
        utmp_entry_formatted.tv_sec = utmp_entry->ut_tv.tv_sec;

        entry_list[count++] = utmp_entry_formatted;

    }
            
    // Assign the list of entries and the total number of entries to the structure to be returned
    Returning_Data->data = entry_list;
    Returning_Data->length = count;
            
    // End the use of the utmp file
    endutent();
            
    // Return the structure containing the list of data and the count of entries
    return Returning_Data;
}
                
struct session_time *show_time(time_t login_time , time_t logout_time)
{
    struct session_time *time_data = malloc(sizeof(struct session_time));
    
    char *login = malloc(19 * sizeof(char));
    char *logout = malloc(30 * sizeof(char));
    //char *login_amount = malloc(30 * sizeof(char));

    

    if(logout_time == 0)
    {
        set_login_time(login_time , login , time_data);
        strcpy(time_data->logout_time , ""); 
        //strcpy(time_data->login_duration , "logged in)");

    }
    else
    {
        set_login_time(login_time , login , time_data);
        set_logout_time(logout_time  , logout , time_data);
    }
    free(login);
    free(logout);

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
