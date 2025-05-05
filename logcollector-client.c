#include <stdio.h> // for I/O operations
#include <stdlib.h> // for memory management
#include <fcntl.h> // for file access
#include <unistd.h> // for reading files
#include <utmp.h> // for reading utmp files


// Define the structure utmp_data to store the data list and its length
struct utmp_data
{
    struct utmp *data; // Array of utmp structures
    int length; // Number of entries in the array
};

// Function declaration to read the utmp file and return a utmp_data structure
struct utmp_data *read_file(char *path);


int main()
{
    // Read the data from the file and store it in the structure
    struct utmp_data *entry = read_file("/var/log/wtmp");

    // Loop to print the information of each entry
    for(int i = 0; i < entry->length; i++)
    {
        // Print the information of each entry
        printf("Type: %d\n", entry->data[i].ut_type);
        printf("User: %s\n", entry->data[i].ut_user);
        printf("TTY : %s\n", entry->data[i].ut_line);
        printf("Host: %s\n", entry->data[i].ut_host);
        printf("PID : %d\n", entry->data[i].ut_pid);
        printf("Time: %u\n", entry->data[i].ut_tv.tv_sec);
        printf("---------------------------\n");
    }

    // End using the utmp file
    endutent();

    return 0;
}
    

// Function to read the utmp file and return the data
struct utmp_data *read_file(char *path)
{
    // Allocate memory for the utmp_data structure
    struct utmp_data *Returning_Data = malloc(sizeof(struct utmp_data)); // Structure to store data and count
    struct utmp *utmp_entry = NULL; // Pointer to each utmp entry
    struct utmp *entry_list = NULL; // Array to store the entries

    int count = 0; // Number of entries

    // Open the utmp file and prepare to read it
    utmpname(path);  // Open the wtmp file from /var/log as a read-only file
    setutent(); // Start reading the data from the utmp file


    // Read each entry from the file until the end
    while((utmp_entry = getutent()) != NULL)
    {
        // Reallocate memory based on the number of entries
        struct utmp *temp = realloc(entry_list, (count + 1) * sizeof(struct utmp));
        if(temp == NULL)
        {
            // If realloc fails, free the previous memory and print an error
            free(entry_list);
            perror("You don't have enough memory space.");
            endutent();
            return NULL;
        }    

        // Assign the reallocated memory back to entry_list
        entry_list = temp;
        // Copy the data of the current utmp entry to the list
        entry_list[count++] = *utmp_entry;
    }

    // Assign the list of entries and the count to the returning data structure
    Returning_Data->data = entry_list;
    Returning_Data->length = count;

    // Return the structure containing the data and length
    return Returning_Data;
}

