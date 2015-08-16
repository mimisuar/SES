CPU Info:

1000h (65,556 bytes of ram)

Memory Map:

0x0000 - 0x01FF: Ram
0x0200 - 0x020F: Data Stack
0x0210 - 0x0219: Keyboard (WASDOK)
0x021A - 0x021F: Mouse (XYLRD)
0x0220 - 0x022F: GPU Stuff
0x0230 - 0x023F: APU Stuff
0x0240 - 0x024F: Call Stack
0x0250 - 0xFFFF: Prg Data 

[A]ccumulator - Used for math operations (byte)
[Z]Register - Used for storing other numbers (byte)

(PC)Program Counter - What operation the program is executing
(DataSP)Data Stack Pointer - Where in the data stack we are. Used by push, pop operations.
(CallSP)Call Stack Pointer - where in the call stack we are. Used by call, ret operations.