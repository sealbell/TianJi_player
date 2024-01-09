# Tianji Player

#### Introduction
Tianji Player is a music player application that uses C++ and Qt for user interface design. This application allows users to play music files, control playback, and view lyrics.

## Features
- Play/Pause music
- Navigate the playlist with Next and Previous buttons
- Adjust volume
- Display lyrics with the current line highlighted

## System Requirements
- Linux Operating System
- Qt 5.14.2 with GCC 64-bit
- mplayer installation required

## Installation
Before running Tianji Player on a Linux system, you need to install mplayer. This can be done through your distribution's package manager, for example:
- Ubuntu: `sudo apt install mplayer`
- Fedora: `sudo dnf install mplayer`
- Arch Linux: `sudo pacman -S mplayer`
Then, copy the `tianji-player` folder to your home directory and run the `make` command to compile the application.

After installing mplayer, compile the Tianji Player using the built-in compiler of Qt Creator according to your Qt Creator configuration.

## Compilation
Open the project in Qt Creator, configure it using the Desktop Qt 5.14.2 GCC 64bit kit, and build the project in Debug mode.

## Execution
Run the compiled `tianji_player` executable to start the player.

## License
Licensed under the MIT open source license.

## Contact
For support or to report issues, please contact the WeChat ID: zero_GK397. Remember to add a note, or you might be ignored.
