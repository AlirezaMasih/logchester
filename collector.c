#include "collector.h"

// Function to read the utmp file and return a structure containing the data and its length
struct utmp_data *read_file(char *path)
{
    // Allocate memory for the utmp_data structure to store the data and its length
    struct utmp_data *Returning_Data = malloc(sizeof(struct utmp_data));
    struct utmp *utmp_entry = NULL; // Pointer to hold each individual utmp entry
    struct session_data *entry_list = NULL; // Array to store all the entries read from the file
    struct session_data utmp_entry_formatted;

    strcpy(utmp_entry_formatted.logout , "still");
    strcpy(utmp_entry_formatted.login_amount , "login");

    int count = 0; // Keep track of the number of entries

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
        /*if(utmp_entry->ut_type != 8)
        {
            entry_list[count++] = *utmp_entry;
        }*/
        strncpy(utmp_entry_formatted.ut_user , utmp_entry->ut_user , 32); 
        strncpy(utmp_entry_formatted.ut_host , utmp_entry->ut_host , 32); 
        strncpy(utmp_entry_formatted.ut_line , utmp_entry->ut_line , 32); 
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
                
void show_time(uint32_t *input_time , char *time_buffer_ptr , size_t size)
{

    time_t time = (time_t)*input_time;
                
    struct tm *time_info = localtime(&time);
    strftime(time_buffer_ptr , size , "%a %b %e %H:%M" , time_info);

 
}

