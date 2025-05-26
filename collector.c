#include "collector.h"

// Function to read the utmp file and return a structure containing the data and its length
struct utmp_data *read_file(char *path)
{
    // Allocate memory for the utmp_data structure to store the data and its length
    struct utmp_data *Returning_Data = malloc(sizeof(struct utmp_data));
    struct utmp *utmp_entry = NULL; // Pointer to hold each individual utmp entry
    struct session_data *entry_list = NULL; // Array to store all the entries read from the file
    struct session_data utmp_entry_formatted; // A variable to hold each formatted entry

    // Initialize the count to 0 to keep track of the number of entries read
    int count = 0;
    uint32_t offset;

    // Open the utmp file for reading
    utmpname(path);  // Open the wtmp file from /var/log as a read-only file
                     
    setutent(); // Start reading the data from the utmp file

    // Loop to read each entry from the file until the end
    while((utmp_entry = getutent()) != NULL)
    {
        // Reallocate memory to accommodate the next entry in the list
        entry_list = realloc(entry_list, (count + 1) * sizeof(struct session_data));
        
        // If memory allocation fails, print an error and return NULL
        if(entry_list == NULL)
        {
            free(Returning_Data); // Free the previously allocated memory for Returning_Data
            perror("Memory allocation failed."); // Print the error message
            endutent(); // Close the utmp file
            return NULL;
        }    
        
        // Store the current utmp entry into the list
        strncpy(utmp_entry_formatted.ut_user , utmp_entry->ut_user , 10); // Copy the user name
        strncpy(utmp_entry_formatted.ut_host , utmp_entry->ut_host , 25); // Copy the host name
        strncpy(utmp_entry_formatted.ut_line , utmp_entry->ut_line , 25); // Copy the terminal line
        utmp_entry_formatted.ut_type = utmp_entry->ut_type; // Store the type of the entry
        utmp_entry_formatted.tv_sec = utmp_entry->ut_tv.tv_sec; // Store the time of the entry

        // Add the formatted entry to the list
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

