#include <stdio.h> // for i/o
#include <stdlib.h> // memory management 
#include <fcntl.h> // access files
#include <unistd.h> // read files
#include <utmp.h> // read utmp files


int main()
{

	struct utmp *utmp_entry;

	utmpname("/var/log/wtmp"); // opening wtmp file from /var/log as a readonly file
	setutent();

	do{
		utmp_entry = getutent();


		printf("Type: %d\n", utmp_entry->ut_type);
        	printf("User: %s\n", utmp_entry->ut_user);
        	printf("TTY : %s\n", utmp_entry->ut_line);
        	printf("Host: %s\n", utmp_entry->ut_host);
        	printf("PID : %d\n", utmp_entry->ut_pid);
        	printf("Time: %u\n", utmp_entry->ut_tv.tv_sec);
        	printf("---------------------------\n");

	}while(utmp_entry != NULL);

	endutent();

	return 0;
}
	

	

	

