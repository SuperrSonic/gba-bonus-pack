# GBA Bonus Pack

This is a GameCube (also Wii) app that when started from a gc-loader or cubeODE SD card will allow editing savefiles copied by GBI's save dumper, making things like event distributions work directly from the GameCube and the Game Boy Player.

# Supported Games

* Zelda ALTTP -- Access the hidden fetch quest and Palace of the Four Sword.
* Fire Emblem -- Same exact content from the Mario Kart bonus disc.
* Fire Emblem: The Sacred Stones -- Japan exclusive items, others handpicked.
* Sonic Advance -- Rare Chao
* Sonic Advance 2 -- Rare Chao
* Sonic Pinball Party -- Rare Chao
* Pokémon FireRed & LeafGreen -- Jirachi, event tickets, etc.
* Pokémon Ruby & Sapphire -- Jirachi, event tickets, etc.
* Pokémon Emerald -- Jirachi, Old Sea Map, etc.


Since the focus is the GameCube with a gc-loader or sd2sp2, the naming of the savefiles is the same used by GBI's dumper, which follows the pattern of "sd:/TITLEID|PUBLISHERID|.|SAVETYPE"

You can use the agb_titleids.txt file to find the correct ID for a game, example: EUR version of Sonic Advance is ASOP78.FLA, the game uses flash memory so it has .FLA as the extension, and the publisher code is 78(THQ)

Since emulator saves are compatible as well, there's an alternate naming convention that can be used, this one follows my fork of RetroArch's path:

"sd:/private/other/savefiles/NAME.srm" -- notice that all RetroArch saves use ".srm" as the extension. You can check agb_titleids.txt for the names used to find a specific save.

It is done this way to avoid using arguments, or a file browser that could cause the user to accidentally pick the wrong file, it keeps things simple and the interface clean.


# GPSP

A port of RetroArch with gpsp is almost fully working, the savefiles created from this emulator are compatible with GBA Bonus Pack. Coming soon.

# GUI-based Wii versions

More visually appealing, and split versions exist for Wii that mimic the look and feel of the original programs, using libwiigui and WiiXplorer as a base.
These will be shared eventually, after I've reviewed some of the code.

# Video Preview

[![Video backstory](https://img.youtube.com/vi/GBEMP2kEpPw/maxresdefault.jpg)](https://youtu.be/GBEMP2kEpPw)

# Known Issues

Restoring the EEPROM save that's used by Zelda ALTTP doesn't work on GBI.

Compiling the Wii version has the option of using the original libogc, that's what the regular makefile is set for, the official release here is built using it.

# Credits

Python code for ALTTP's checksum:
https://github.com/LemonBoy/savezelda-gba

Tiny Chao Garden's decompiled savefile check:
https://github.com/phonymike

The idea to support FE8 comes from here:
https://feuniverse.us/t/fire-emblem-the-sacred-stones-bonus-content-unlocked/6705

Pokémon Gen III savefile documentation:
https://bulbapedia.bulbagarden.net/wiki/Save_data_structure_(Generation_III)

And of course, this wouldn't have been possible without gc-loader support:
https://github.com/extremscorner/libogc2
