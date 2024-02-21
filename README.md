# geekvape-t200MOD
a reverse engineering project of Geekvape T200 (aegis touch)

Firmware files: mainflash.bin (dumped from 0x08000000), optbytes.bin (option bytes), aliasflash.bin (dumped from 0x0 on a halted chip after the bootup, identical to the mainflash)

Extrenal 32mb flash chip (GD25Q256E) dump: outspi.bin

Full documentation on the N32G455 chip is in respective folders. Obtained from ftp://download.nationstech.com/, with anonymous login (anonynous:anonymous). Use the default linux command line ftp, all other ftp browsers I used couldn't connect for some reason.

This is a very raw and work-in-progress repo. Use at your own risk. I may be accessible through issues, but I might not be able to help you.

Used the Sigga and svd ghidra plugins. Find them on github.
