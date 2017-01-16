# zndkcl
test application for command line interdace

## Getting Started
```bash
$ make clean
$ make depend
$ make
$ ./zndkcl 
```
## Build and Operation log
```bash
$ make clean
rm -rf zndkcl.o zndkcl *~

$ make depend
rm -rf Makefile.depend
gcc -MM -MG -I. -c -Wall -Werror zndkcl.c > Makefile.depend

$ make
gcc -I. -c -Wall -Werror zndkcl.c
gcc  -o zndkcl zndkcl.o 

$ ./zndkcl 
=== zndkcmdline start ===
type '?' for help
> 
> 
> ?
 Usage: <op> <arg>[ <arg>]

  op:
   f FILE       loads FILE.
   m <msgid> <msgsz>[ <data>[ <data>]]

> 
> 
> m 0x12345 0
 msgid   =    0x12345
 msgsz   =          0
> 
> 
> m 0x12345 4 0xAAAA5555
 msgid   =    0x12345
 msgsz   =          4
 data[0] = 0xAAAA5555
> 
> m 0x12345 8 0xAAAA5555 deadbeef
 msgid   =    0x12345
 msgsz   =          8
 data[0] = 0xAAAA5555
 data[1] = 0xDEADBEEF
> 
> 
> m 0x12345 4 0xAAAA5555 0xdeadbeef
 msgid   =    0x12345
 msgsz   =          4
 data[0] = 0xAAAA5555
> 
> 
> f cmdlist
 ----- loads command file start: cmdlist -----
  L   1(  26[B], len=120): # this is a comment line., 23
  L   2(  13[B], len=120): m 0x00000  0, 6D
 msgid   =    0x00000
 msgsz   =          0
  L   3(  24[B], len=120): m 0x11111  4 0x00001111, 6D
 msgid   =    0x11111
 msgsz   =          4
 data[0] = 0x00001111
  L   4(  35[B], len=120): m 0x22222  8 0x00001111 0x00002222, 6D
 msgid   =    0x22222
 msgsz   =          8
 data[0] = 0x00001111
 data[1] = 0x00002222
  L   5(  46[B], len=120): m 0x33333 12 0x00001111 0x00002222 0x00003333, 6D
 msgid   =    0x33333
 msgsz   =         12
 data[0] = 0x00001111
 data[1] = 0x00002222
 data[2] = 0x00003333
  L   6(   2[B], len=120): ,  0
  L   7(  58[B], len=120): m 0x44444 16 0x00001111 0x00002222 0x00003333 0x00004444, 6D
 msgid   =    0x44444
 msgsz   =         16
 data[0] = 0x00001111
 data[1] = 0x00002222
 data[2] = 0x00003333
 data[3] = 0x00004444
  L   8(   1[B], len=120): ,  0
  L   9(  11[B], len=120): f cmdlist2, 66
 ----- loads command file start: cmdlist2 -----
  L   1(  15[B], len=120): # nesting test, 23
  L   2(   1[B], len=120): ,  0
  L   3(  69[B], len=120): m 0x55555 16 0x00001111 0x00002222 0x00003333 0x00004444 0x00005555, 6D
 msgid   =    0x55555
 msgsz   =         16
 data[0] = 0x00001111
 data[1] = 0x00002222
 data[2] = 0x00003333
 data[3] = 0x00004444
  L   4(   1[B], len=120): ,  0
  L   5(   5[B], len=120): # end, 23
 ----- loads command file end  : cmdlist2 -----
  L  10(   1[B], len=120): ,  0
  L  11(   6[B], len=120): # end, 23
 ----- loads command file end  : cmdlist -----
> 
> 
> 
> q
 quit
=== zndkcmdline end   ===
$ 
```
.
