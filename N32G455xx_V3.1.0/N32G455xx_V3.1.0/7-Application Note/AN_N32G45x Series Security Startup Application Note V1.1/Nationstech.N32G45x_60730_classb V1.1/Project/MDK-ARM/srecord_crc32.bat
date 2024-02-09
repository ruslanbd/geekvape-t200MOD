.\SREC\srec_cat .\Objects\N32G45x_SelfTest.hex -intel -crop 0x08000000 0x0801FF00 -fill 0x00 0x08000000 0x0801FF00 ^
-crc32-l-e 0x0801FF00 -o .\Objects\N32G45x_SelfTest_CRC.hex -intel
