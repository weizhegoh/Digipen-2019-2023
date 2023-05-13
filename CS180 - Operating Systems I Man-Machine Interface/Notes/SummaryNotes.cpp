Lecture 1:
==========

Outline: 
--------
- Execution of a program
- Boot Sequence 
- Roles of an OS
- History of OS

Von Neumann Model:
------------------

Input Device-> Central Processing Unit (CPU) consist of Control Unit & Arithmetic/Logic Unit -> Output Device 
    
Central Processing Unit <-> Memory Unit

1.Execution
2.Fetch
- Registers rax, rbx to store the address of the next instruction through address bus 
- Address bus allows CPU to send the address to the Random Access Memory(RAM)
- Data bus allows the actual data transfer to the CPU. 

3. 


https://www.javatpoint.com/von-neumann-model#:~:text=Von%2DNeumann%20proposed%20his%20computer,%2C%20Registers%20and%20Inputs%2FOutputs. 

https://tdck.weebly.com/uploads/7/7/0/5/77052163/04_-_fetch_decode_execute_cycle.pdf 


--------------------------------------------------------------------------------

Lecture 8: Syncronization
-------------------------

Producer:
---------

while(true)
{
    full.wait();                //block when buffer is full, while(count == BUF_SIZE);
    mutex.wait();
    buffer[in] = new product;
    in = (in + 1) % BUF_SIZE;
    mutex.signal();
    empty.signal();             //increment avaiable buffer count, count ++
}

Consumer:
---------

while(true)
{
    empty.wait();               //block when buffer is empty, while(count == 0);
    mutex.wait();
    consumed = buffer[out++];
    out = (out + 1) % BUF_SIZE;
    mutex.signal();
    full.signal();              //increment available buffer count, count--
}

SW Solution 1: Peterson's algorithm
-----------------------------------

int turn;
int interested[2];

void EnterCS(int proc)
{
    int other;
    other = proc^ 0x1; //toggle
    interested[proc] = true;
    turn = proc;
    
    __asm__ (“mfence”);
    
    while((turn==proc) && interested[other]);
}

void LeaveCS(int proc)
{
    interested[proc]= false ;
}

HW solution 1: Disable interrupts
---------------------------------
while(true)
{
    Disable interrupts;
    //Critical Section
    …
    Enable interrupts;
    //Remainder Section
}

HW Solution 2: Test And Set
---------------------------

bool TestAndSet(bool *tgt)
{
    bool rv = *tgt;
    *tgt = true;
    return rv;
}

do
{
    while(TestAndSet(&lock));   
    // critical section 
    lock = false;
    // remainder section
} while (true);

HW Solution 3: Compare and Swap
--------------------------------

int CompareAndSwap(int*value, int expected, int new_value)
{
    int temp = *value;
    
    if(*value == expected)
        *value = new_value;

    return temp;
}

do {
    while(CompareAndSwap(&lock, 0, 1) !=0);
        // critical section
        lock = 0;
        // remainder section
} while (true)
    

Semaphore:
----------
class Semaphore
{
int s;

public:
    void wait ()
    {
        while(TestAndSet(&lock));
        if(s <= 0)              //critical section 
        {
            //add self-process to queue;
            lock = false;
            block(&lock);
        }
        
        s--;                   //critical section 
        lock = false;
    }

    void signal()   
    { 
        while(TestAndSet(&lock));
        s++;                   //critical section 
        //if queue is not empty 
        //wake up any process waiting in queue 
    }
}

Using Semaphore:
----------------
Semaphore s(1);

while(true)
{
    s.wait();
    //critical section, only 1 in CS each time 
    s.signal();             
    //remainder section;
}


Semaphore s(N);

while(true)
{
s.wait();
//critical section, up to N in CS each time 
s.signal();
//remainder section;
}


//Process 1
while(true)
{
    S1;
    synch.signal();
}

//Process 2
while(true)
{
    synch.wait();
    S2;
}

//shared memory, semophore synch

//Sleeping Barber

Semaphore barbers = 0;
Semaphore mutex = 1;
Semaphore customers = 0;
int nFreeWRSeats = N;

void Barber() 
{
    while (1) 
    {
        wait(customers);
        wait(mutex);
        nFreeWRSeats++;
        signal(barbers);
        signal(mutex);
        haircut();
    }
}

void Customer()
{
    wait(mutex);

    if (nFreeWRSeats > 0) 
    {
        nFreeWRSeats--;
        signal(customers);
        signal(mutex);
        wait(barber);
        get_haircut();
    }
    else 
    {
        signal(mutex);
        //leaveWR();
    }
}

//Dining philosopher's problem

Semaphore chopsticks[N];

while (true)
{
    if ((i + 1) % N > i) 
    {
        wait(chopsticks[i]);
        get_left();
        wait(chopsticks[(i + 1) % N]);
        get_right();
    }
    else 
    {
        wait(chopsticks[(i + 1) % N]);
        get_right();
        wait(chopsticks[i % N]);
        get_left();
    }
    eat();
    signal(chopsticks[i]);
    signal(chopsticks[(i + 1) % N]);
    think();
}

//Deadlock
/* PROCESS 0 */
flag[0] = true;
while (flag[1]) /* do nothing */;
/* CS */
…
flag[0] = false;
/* PROCESS 1 */
flag[1] = true;
while (flag[0]) /* do nothing */;
/* CS */
…
flag[1] = false;





//Livelock
/* PROCESS 0 */
flag[0] = true;
while (flag[1]) {
    flag[0] = false;
    sleep(10);
    flag[0] = true;
}
/*CS*/
…
flag[0] = false;
/* PROCESS 1 */
flag[1] = true;
while (flag[0]) {
    flag[1] = false;
    sleep(10);
    flag[1] = true;
}
/*CS*/
…
flag[1] = false;

////////////////Chapter 9 - Memory Management ///////////////////////////////
code 
data 
stack - LIFO , very fast 
heap - global variable, allocate huge block of memory(malloc), slower,
