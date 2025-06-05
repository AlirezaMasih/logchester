#include "send_data.h"


#define username "masih"
#define remote_file "/home/masih/log.txt"
#define file_path "log.txt"
#define password "Alireza.md1383"
#define hostname "127.0.0.1"

void send_file()
{
    LIBSSH2_SESSION *session;
    LIBSSH2_SFTP *sftp;
    LIBSSH2_SFTP_HANDLE *sftp_handle;

    libssh2_init(0);

    int socket_fd = socket(AF_INET , SOCK_STREAM , 0);
    struct sockaddr_in ninfo;

    ninfo.sin_family = AF_INET;
    ninfo.sin_port = htons(22);
    ninfo.sin_addr.s_addr = inet_addr(hostname);

    connect(socket_fd , (struct sockaddr *)&ninfo , sizeof(struct sockaddr_in));

    session = libssh2_session_init();

    libssh2_session_handshake(session , socket_fd);
    libssh2_userauth_password(session , username , password);

    sftp = libssh2_sftp_init(session);
    int local_file_descriptor = open(file_path , O_RDONLY);

    sftp_handle = libssh2_sftp_open(sftp , remote_file , LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT , LIBSSH2_SFTP_S_IRUSR | LIBSSH2_SFTP_S_IWUSR);

    char buffer[2048];
    ssize_t read_file;

    while((read_file = read(local_file_descriptor , buffer , sizeof(buffer))) > 0)
    {
        libssh2_sftp_write(sftp_handle , buffer , read_file);
    }


    libssh2_sftp_close(sftp_handle);
    libssh2_sftp_shutdown(sftp);
    libssh2_session_disconnect(session, "Shutdown");
    libssh2_session_free(session);
    close(local_file_descriptor);
    close(socket_fd);
    libssh2_exit();

}
