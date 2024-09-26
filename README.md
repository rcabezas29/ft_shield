# ft_shield
Remote Access Trojan (RAT)

A trojan horse is a special kind of malicious software, or malware, often mistaken with
virus or parasites. The trojan horse looks like a legit software, but it contains a malicious
parasite. The goal of the trojan horse is to get this parasite inside the computer and
install it unbeknownst to the user.
The contained program is called "payload". It can be any kind of parasite: virus,
keylogger,spyware... This parasite is the only element that will execute the actions
inside the victim’s computer. The trojan horse is just the vehicle that "puts the fox in
charge of the henhouse". It’s not malicious itself because it can’t execute any action,
other than installing the real parasite.
In common language, the parasite is often called "trojan horse" by metonymy. This
confusion is often fueled by antivirus publishers.They use "trojan" as a generic name to
describe different kinds of malware that are all but trojan horses.

## Overview

ft_shield is a custom binary designed to execute as a background daemon that provides root shell access over a network. It operates silently, only displaying a minimal message on launch while performing critical actions in the background.

> Disclaimer: This project is strictly for educational purposes to demonstrate potential vulnerabilities. Use responsibly, and do not deploy in unauthorized systems. Misuse of this tool could lead to serious legal consequences.

## Features

- Root Access Control: ft_shield only executes with root privileges.
- Silent Execution: On launch, ft_shield will only display the user's login name while executing critical tasks in the background.
- Daemon Creation: ft_shield creates a copy of itself in the system's binary directory and ensures it automatically runs on system startup.
- Single Instance Daemon: Only one instance of the ft_shield daemon can be running at any time to avoid multiple access points.
- Background Operation: The daemon runs silently in the background, listening for incoming client connections.
- Network Access: The daemon listens on port 4242 and supports up to three simultaneous client connections.
- Password Protection: A secure password is required for any client to gain access. Clear text passwords in the code are strictly prohibited.
- Root Shell Access: Once authenticated, clients can gain root shell access remotely. However, direct access through typical shell commands is restricted, and a custom interaction is required.

## Functionality

1. **Initial Launch**: Upon launching, ft_shield displays the current user's login name in the terminal. Meanwhile, in the background, it:
  - Creates a copy of itself in the system's binary directory.
  - Ensures that it will be executed automatically on the machine's next startup using a method of your choice (be creative, e.g., cron job, systemd service, rc.local, etc.).

2. **Daemon Setup**:
        The newly created ft_shield runs as a daemon in the background.
        It listens on port 4242 and can handle up to three concurrent client connections.
        Only one instance of this daemon can run at a time, ensuring limited access to the machine.

3. **Client Connection**:
  - Clients must provide a secure password to connect. Implement robust password handling to ensure no passwords are stored or transmitted in clear text.
  - Upon successful authentication, the client is granted root shell access to the system.

4. **Security Considerations**:
  - All connections require secure password authentication.
  - Direct shell access is not possible through common commands. Only authenticated clients can access the shell with elevated privileges.

## Usage

1. **Build and Launch**:

```bash
make
sudo ./build/ft_shield
```

2. **Connect**:
```bash
nc <Infected IP> 4242
```

3. **Execute commands**:

```
Commands:
     ?                    Show this help message.
     shell                Create a bind shell on port 4243.
     reverse <IP> <PORT>  Connect to the specified IP address and port, and start a reverse shell.
     exit                 Close the current client connection.

Options:
     <IP>                 The target IP address.
     <PORT>               The target port number.
```
