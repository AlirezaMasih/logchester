# logchester

A modular, configurable log collector and session analyzer for Linux, written in C. logchester reads utmp/wtmp files, processes user session data, generates human-readable reports, and can send results to a remote server via SSH/SFTP.

---

## Features

- **Reads and parses utmp/wtmp**: Extracts and processes user session data.
- **Session analysis**: Calculates login/logout times, durations, and session status (still logged in, crash, shutdown, etc).
- **Configurable via file**: All paths, usernames, and remote destinations are set in `logchester.conf`.
- **Remote log transfer**: Sends processed logs to a remote server using SSH (libssh2).
- **File monitoring**: Uses inotify to watch for changes in log files and trigger processing automatically.
- **Modular design**: Each major functionality is separated into its own module for easy maintenance and extension.
- **Memory management**: Dynamically allocates and frees resources as needed.

---

## Project Structure

| File/Directory      | Purpose                                                      |
|--------------------|--------------------------------------------------------------|
| collector.c/h      | Read utmp/wtmp and collect session data                      |
| datetime.c/h       | Format and calculate session time/duration                   |
| read_config.c/h    | Parse configuration file (logchester.conf)                   |
| send_data.c/h      | Send logs to remote server via SSH/SFTP                      |
| session_process.c/h| Process sessions and generate reports                        |
| logchester.c       | Main entry point, event loop, and orchestrator               |
| logchester.conf    | Configuration file (paths, credentials, etc)                 |

---

## Configuration Example (`logchester.conf`)

```
# logchester.conf file
utmp_path = /var/log/wtmp
ip = <REMOTE_SERVER_IP>
username = <REMOTE_USERNAME>
password = <REMOTE_PASSWORD>
remote_file = /path/on/remote/server/logins.txt
log_file = /path/to/local/logins.txt
# For SSH key authentication (future):
# ssh_key_path = /home/user/.ssh/id_rsa
# ssh_key_passphrase = <KEY_PASSPHRASE>
```

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
- Generates a report (log file) with session details.
- Sends the report to the configured remote server via SSH/SFTP.

---

## Extending & Contributing

- Modular codebase makes it easy to add new features (e.g., support for BSD, new output formats, CLI tools).
- For contributing, fork the repo, make changes, and submit a pull request.
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
