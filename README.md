<p align="center">
 <img src="https://github.com/DS-Homebrew/10SecondRun-DS/blob/master/resources/DSiWareOnDS-title.png"><br>
</p>

Part of the DSiWare on DS series, this is a work-in-progress homebrew recreation of the DSiWare title, "10 Second Run".

It can be played on all DS family consoles.

# Compiling

## Setting up

Compiling this app requires devkitPro's devkitARM, libnds, grit, and mmutil. These can be installed using [devkitPro pacman](https://devkitpro.org/wiki/devkitPro_pacman) with the following command:
```
sudo dkp-pacman -S nds-dev
```
(Note: Command will vary by OS, sudo may not be needed and it may be just `pacman` instead)

## Building

Once you have devkitPro's toolchains installed you can build the source code by simply running `make` in the root of the repository.

Once it finishes building, the output .nds file will be in the root of the repository.

# Credits
- [chyyran](https://github.com/chyyran): Stremable sound playback code
- [devkitPro](https://github.com/devkitPro): devkitARM, libnds, and libfat