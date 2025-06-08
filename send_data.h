#include <libssh2.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <libssh2_sftp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "read_config.h"


void send_file();
