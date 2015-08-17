CPU Info:

1000h (4096 bytes of ram)

Memory Map:

0x0000 - 0x03FF: Ram
0x0400 - 0x040F: Data Stack
0x0410 - 0x0419: Keyboard (WASDOK)
0x041A - 0x041F: Mouse (XYLRD)
0x0420 - 0x042F: GPU Stuff
0x0430 - 0x043F: APU Stuff
0x0440 - 0x044F: Call Stack
0x0500 - 0x0FFF: Prg Data 

[A]ccumulator - Used for math operations (byte)
[Z]Register - Used for storing other numbers (byte)

(PC)Program Counter - What operation the program is executing
(DataSP)Data Stack Pointer - Where in the data stack we are. Used by push, pop operations.
(CallSP)Call Stack Pointer - where in the call stack we are. Used by call, ret operations.