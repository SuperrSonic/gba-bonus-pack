# GBA Bonus Pack

This is a GameCube (also Wii) app that when run from a gc-loader or cubeODE SD card will allow editing savefiles dumped by GBI's save dumper, allowing things like event distributions to work directly from the GameCube and the Game Boy Player.


# GPSP

A port of RetroArch with gpsp is almost fully working, saves created from this emulator are compatible with GBA Bonus Pack.

# GUI-based Wii versions

More visually appealing, and split versions exist for Wii that mimic the look and feel of the original programs, using libwiigui and wiixplorer as a base.
These will be shared eventually, after I've reviewed some of the code.

# Preview

[![Video backstory](https://img.youtube.com/vi/GBEMP2kEpPw/maxresdefault.jpg)](https://youtu.be/GBEMP2kEpPw)

# Known Issues

Restoring the EEPROM save that's used by Zelda ALTTP doesn't work on GBI.

# Credits

Python code for ALTTP's checksum:
https://github.com/LemonBoy/savezelda-gba

Tiny Chao Garden's decompiled savefile check:
https://github.com/phonymike

The idea to support FE8 comes from here:
https://feuniverse.us/t/fire-emblem-the-sacred-stones-bonus-content-unlocked/6705

Pok�mon Gen III savefile documentation:
https://bulbapedia.bulbagarden.net/wiki/Save_data_structure_(Generation_III)

And of course, this wouldn't have been possible without gc-loader support:
https://github.com/extremscorner/libogc2
