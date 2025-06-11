# logchester

A modular, configurable log collector and session analyzer for Linux, written in C. `logchester` reads utmp/wtmp files, processes user session data, generates human-readable reports, and can send results to a remote server via SSH/SFTP. Supports both password and SSH key-based authentication.

---

## Features

- **Reads and parses utmp/wtmp**: Extracts and processes user session data (user logins, logouts, session types).
- **Session analysis**: Calculates login/logout times, durations, and session status (still logged in, crash, shutdown, etc).
- **Configurable via file**: All paths, credentials, and remote destinations are set in `logchester.conf`.
- **Remote log transfer**: Sends processed logs to a remote server using SSH (libssh2), supporting both password and SSH key-based authentication.
- **Flexible authentication**: Choose between password and public/private key authentication for secure transfers.
- **File monitoring**: Uses inotify to watch for changes in log files and trigger processing automatically.
- **Modular design**: Each major functionality is separated into its own module for easy maintenance and extension.
- **Memory management**: Dynamically allocates and frees resources as needed.
- **Extensible**: Easy to add new features, output formats, or support for other platforms.

---

## Project Structure

| File/Directory      | Purpose                                                      |
|--------------------|--------------------------------------------------------------|
| collector.c/h      | Read utmp/wtmp and collect session data                      |
| datetime.c/h       | Format and calculate session time/duration                   |
| read_config.c/h    | Parse configuration file (`logchester.conf`)                 |
| send_data.c/h      | Send logs to remote server via SSH/SFTP (password or key)    |
| session_process.c/h| Process sessions and generate reports                        |
| logchester.c       | Main entry point, event loop, and orchestrator               |
| logchester.conf    | Configuration file (paths, credentials, authentication, etc) |

---

## Configuration Example (`logchester.conf`)

```
# logchester.conf file
# Path to the utmp/wtmp file to monitor
utmp_path = /var/log/wtmp

# Remote server IP or hostname
ip = <REMOTE_SERVER_IP>

# Username for authentication
username = <REMOTE_USERNAME>

# Password for authentication (leave blank if using SSH key)
password = <REMOTE_PASSWORD>

# Path to the file on the remote server where logs will be uploaded
remote_file = /path/on/remote/server/logins.txt

# Path to the local file where processed logs will be saved
log_file = /path/to/local/logins.txt

# (Optional) Path to SSH public and private keys for key-based authentication
pub_key = /path/to/ssh/public_key
pri_key = /path/to/ssh/private_key
passphrase = <KEY_PASSPHRASE>
```

- If both password and key are set, key-based authentication will be attempted first.
- If key authentication fails, password authentication will be attempted as a fallback.

---

## Build & Run

1. **Dependencies:**
   - GCC
   - libssh2-dev (for SSH/SFTP support)
2. **Build:**
   ```bash
   gcc -o logchester *.c -lssh2
   ```
3. **Run:**
   ```bash
   ./logchester
   ```
   The program will monitor the utmp/wtmp file and automatically process and send logs on changes.

---

## How it Works

- Watches the utmp/wtmp log file for changes (using inotify).
- When a change is detected, reads all session records and processes them.
- Generates a human-readable report (log file) with session details (login, logout, duration, status).
- Sends the report to the configured remote server via SSH/SFTP, using either password or key-based authentication.

---

## Extending & Contributing

- Modular codebase makes it easy to add new features (e.g., support for BSD, new output formats, CLI tools, or authentication methods).
- To contribute, fork the repo, make your changes, and submit a pull request.
- Please open issues for bugs or feature requests.

---

## License

This project is licensed under the GNU General Public License v3.0 (GPLv3).

Copyright (C) 2025  Alireza Masih

See the LICENSE file for details.

---

## Author

Alireza Masih

For contact: https://github.com/masih83

---

## Acknowledgements

- Inspired by classic Unix tools like `last` and `syslog-ng`.
- Uses [libssh2](https://www.libssh2.org/) for SSH/SFTP transfers.
