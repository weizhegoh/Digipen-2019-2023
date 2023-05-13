Lecture 1 - Operatring System: Introduction, program, loading, history 
======================================================================

Execution of a program:
-----------------------

Need to have:
- instructions

Von Neumann Model:



CPU to memory
1.Execution:

- rax, rbx is register
- rbp is base pointer 
- rsp is stack pointer - points to the top of the stack

movq $10, (%rbx) //rbx is 0x1000   stack  0x1000 | 10 |

2.Fetch:


Use PC which is register to store the address of the next instructions

- address bus from CPU(PC) to memory

3.Decode
4.Execute

- Upon decoding the instructions, pass over to the ALU for execution.

5.Writeback

- result of execution may be written back to registers or memory 

6.Repeat Cycle


Program execution
-----------------

Stack and Heap
--------------
- It is runtime


Heap:
-----
cs120: malloc, calloc 

Stack: 
------
void foo()
{
    int x = goo(c,1.23);
}

foo
----

high address                

local variable___________   |    //arguments
1.23_____________________   |    //args
c________________________   v
return address 0x100200__
goo stack frame__________
saved reg________________
local variable:x,y,z_____
arguments________________

low address

first in, last out 

push c, b, a 
pop a, b, c 

Load a program: 
---------------
- need a program called loader

Boot up process:
----------------
BIOS - Based input output system 

20 bits - 2^20

1.Power-ON
2.Run BIOS 
3.


Why need BIOS?
--------------
BIOS after bootloading, throw away

What OS Do?
-----------

- Abtraction of hardwares 
- Portability ( can  migrate from here to there)

Phase 1: 1948 - 1970
--------------------



Lecture 2: Hardware support for OS
===================================
How CPU communicate with IO:
- CPU supplies instructions in and out 
- 

CPU ------address-> memory controller ----map to--> I/O Device or main memory 

0x000 Address space 
------------
-----------
----------
------------
memory mapp I/O


2 ways from I/O to CPU
- Polling (ask) 
- Interrupt

CF, SF(Signed Flag), OF(Over Flag), ZF(Zero Flag)

ISR - Internet Service Return??

3 kinds of interrupt:
---------------------
- exception 
- hardware interrupt
- software generated intrerrupt

if interrupt dont mask, higher priority interrupts will interrupt the handling

if u mask interrupt, will ignore the higher prioty interrupts

a[i]

base + i * 8 to get address

real address mode: IVT is to allow access to whichever physical memory 

who sets up the IDTR?  
Who sets up the IDT?

Lecture 3: OS Architecture and System Calls:
============================================

Binary level:

- PC runs faster, 

- System calls is the gate 

stack - local variable, pass in address, return address 

kernel stack 

1980s: DOS OS architecture - 1
------------------------------
ROM - Read only memory 
MS-DOS device drivers - more flexible way to assess device, press certain key to access function

System Utilities - to manage memory, access particular part in memory 

System program - shell, accept program command and load 

application program - game 


whenever an application running, only show ur own application details. Protection from other system?


DOS OS architecture - 11
------------------------
 - cannot touch kernel 
 - Command interpreter - load back the shell 

Monolithic OS architecture 
--------------------------

- Monolithic ( as a whole) 
- 

File system need to know where it is, that why need device driver 


Monolithic OS achiecture:
=========================
Pros: 
- good performance (Speed)/(Shared kenel space) simplicity of design 

Cons: 
- No information hiding
- Potential stability issues / Chaotic
- Hard to understand 

- Can become huge (Linus 4.15 has 20 million lines of code, Windows 10 contains over 40 million lines!)
- Potentially difficult to maintain 

Examples:: 
Traditional Unix kernels (includes BSDs and Solaris) 
linux 
MS-DOS, Windows 9x 
Mac OS versions below 8.6 


Micrcokenel Architecture 
========================
Pros: 
- Modularity: easier management
- Fault isolation and reliability 

Cons:
- Inefficient (boundary crossing); 
- Insufficient protections; 
- Inconvenient to share data between kernal and services 

Exokernel architecture 
----------------------


Comparison 
----------
- Base PC, virtual Memory, Scheduling (all kernel) 


Qn 1 answer: 

- No, not all the time  


Lecture 4 - Processes 
=====================
IR - instruction Register 

when running a program, needs a memory, CPU to execut elogic 

Context of a process
--------------------
FPs - floating point registers, PC, stack pointers 

IRS - part of kernel 




When interrupt happens, its too fast and not enough time to call hardware

before executing ISR, needs hardware 


SW context saving / restoring 
------------------------------
FPR - floating point registers 
GPR -   

3 types of interrupts: 
------------------------
1. cannot mask - exception / fault 
2. hardware interrupt 
3. software generator interrupt 

memory management unit 

PCB - Processor Control Block 

PIC - Program interrupt controller 
CS - Code Segment 

ISE occurred in user mode 
=========================
3 perspective 

1. user
2. kernel
3. ISR  

Week 3 Lecture 1:
-----------------


