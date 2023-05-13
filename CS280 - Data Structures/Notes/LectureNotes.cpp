Week 1 Lecture 1: Memory Management
================================================================================
Memory management - custom solution for allocation, deallocation, reallocation 

why new & delete are not enough?

- no control over program  
- no debugging 
- no statistics 

Memory Manager - allocates the memory & divides the memory 
block - small chunk of memory 

freelist - a linked list of empty blocks (unallocated / free )

Allocation - block from freelist is allocated and is in use 
Deallocation - Block is released back and added to freelist 

Attibutes of memory manager:
----------------------------
1. Ease of use for front end users
    - should have garbarge collection  
    
2. Performance
    - locality of reference 
        1. Temporal 
            > same data used multiple times. e.g. same book 
        2. Spatial 
            > Data near / surrounding will be used again, e.g. book in politics
            
    - speed vs consistency 
        > smaller, faster, costlier per byte
        > larger, slower, cheaper per byte 
    - allocation & deallocation 

allocators - stack, pool, fixed sized, variable size 

types of allocators: 
1. linear
    - no free list 
    - Pros: faster & simple, low overhead memory 
    - no single free()
2. stack 
    - LIFO (last in first out)
    - Pros: still faster , has header with pointer, free LIFO()
3. Pool
    - fixed size block 
    - there is pagelist and freelist
    
    pagelist:
    |H| s1 | s2 | s3 | s4 |
    
    |H| s5| freelist |
    
    OA.free(s3)
    OA.free(s4)
    
4. Freelist 
    - no fixed size block, its a variable size block 
    - every block will have a head header
    
    |H|H|  allocated     |H|  allocated    |
    
    - general purpose allocator 
    - alloc/free in any order 

Assignment 1 :
--------------
struct student 
{
    int age; // 4 bytes 
    int year;
    int gpa; 
    int ID;
}

student x s1:

|a|y|a|I| // 16 bytes 

main Idea: 

1. Allocate a big chunk of memory 
2. Allocate() - return 1 block of memory 
3. Free(obj) - put the block back in the free list 
4. Re use the blocks 

OA.allocate(size) -> allocation policy (next, best, first fit)

OA.free(obj) -> merge empty blocks 

Lecture 2 :

Sorting - array / linked list 
searching - searched 'x' array or linked list 


Data structure:
- way to organise / store the data 
- facilitate efficient access / update 

100 Names + sort into diary 

1. Any order 
2. Alphabetic 

Dairy - sort


Algorithm can be sorted by complexity or correctness 

complexity - time & space (focus more on time complexity)

complexity analysis: 
- comparing performance of different algorithm. e.g. sort algo 1 or sort algo 2 is better?


why cannot we do real analysis? 
- CPU, DISK, OS, etc. Ans: too many parameters, therefore we will use more theoretical analysis - time. 

Big OH (covered in cs280)
Big Thera (covered cs330)
Big Omega (covered cs330)

Linear search: 
--------------
- search 'x' throughout the whole array 
- fastest is at first position 
- slowest is at last position 
- the bigger the size of the array, the longer the time to find an element, esp at the last position 

Lecture 2: Algorithm Analysis
================================================================================
How to determine g(n)?
-----------------------
1. Determine T(n) " the running time"
    #g 8n^2 + nlogn + n

2. Drop all but the most significant term. 
    8n^2

3. Drop the constant coeff.
    0(n^2)

Sorting: - algorithms aim to rearrange A to A' where each ai are in order. 
--------

input - a sequence of n numbers // a, .... an 
output - A permutation // a1' a2',....an'

Increasing a1' < a2' < an'
Decreasing a1' > a2' > an'

#google search - page rank 

Attributes of sorting algorithm 
-------------------------------
1. Stable - preserves the relative order of items with equal value 

e.g. 7 5 2 5'   7 5 2 5'
e.g. 7 5 5' 7   2 5' 5 7 
    stable      not stable 

2. Loop - Invariant: property that holds before & after each repetition 

int max (int n, int a[])
{
    int m = a[0];   // m equals max value a[0] 
    
    for (i = 1, i < n; i++)
    {
        if( m < a[i])
        m = a[i]
    }
    return m;   // m stores the max value 
}

3. In-place : algorithm which transform input using a data structure with a small constant amount of extra storage space 

Reversing an array:

void f1(int a[], int n) //out of place 
{
    int* b = new int[n];
    
    for(i = 0; i < n, i++)
    {
        b[n-i-1] = a[i]
        
        for (i = 0; i < n; i++)
        {
            a[i] = b[i]
        }
    }
    delete [] b;
}

void f2(int a[], int n) //in-place
{
    int m = m/2;
    
    for(i=0; i < m; i++)
    {
        swap(a[i], a[n-i-1]);
        temp = a[i];
        a[i] = a[n-i-1]
        a[n-i-1] = temp;
    }
}

4. Adaptive - It takes advntage of existing order in the input so it benefits from the pre-sortedness in input sequence. 

e.g. 2 13 4 5 9 8 7 

Lecture 3: Sorting Part 1
================================================================================
- Stable 
- Loop invariant
- In place
- Adaptive Memory

Bubble sort: Put largest at the end 
------------------------------------
Main idea - keep bubbling the largest element to the end. Switch a pair of neighbouring elements. 

1. get i & i+1
2. swap them in correct order (if a[3] > a[4], swap a[3] & a[4]) 
3. Start from the front of the array again when reached the end

void bubblesort (int a[], int N) //n is number of elements 
{
    for(i = 0; i < N-1; i++)
    {
        for (j = 0; j < N-i-1;j++)
        {
            if(a[j] > a[j+1])
            {
                swap(a[j], a[j+1])
            }
        }
}

first iteration: 
4 23 17 25 8 
4 17 23 25 8
4 17 23 8 25 // end of first iteration, biggest number which is 25 is at the end 

second iteration: 
4 17 23 8 25
4 17 8 23 25 //end of second iteration, senond largest number 23 is in the second last position 

third iteration:
4 8 17 23 25 //end of third iteration, all numbers sorted in ascending order 

which properties does bubblesort follows?

1. stable           //Yes
2. loop invariant   //Yes
3. Inplace          //Yes
4. Adaptive         //No 

Best case scenario: 
- O(N), Given a sorted array 

Worst case senario: if want increasing order but given array is decreasing order 
- 8 6 5 4 3 
- 6 8 5 4 3
- 6 5 8 4 3 
- 6 5 4 8 3 
- 6 5 4 3 8 

- 5 6 4 3 8 
- 5 4 6 3 8 
- 5 4 3 6 8 
- 4 5 3 6 8 
- 4 3 5 6 8


- for improvement bubble sort, add record for whether swapping accured or not 

N elements :

(N-1) + (N-2) + (N-3) + ... + 1 


Selection sort:
---------------
Main idea - Increase the sorted sequence by selecting the smallest element from the unsorted 

1. Select smallest element from unsorted 
2. Append at the end of sorted side 

find smallest, 2nd smallest, 3rd smallest 

selection sort(int a[], int N)
{
    for(i = 0; i < N-1; i++)
    {
        int min = i;
        
        for (j = i+1; j < N; j ++)  //find position of minimum element in unsorted array 
        {
            if (a[j] < a[min])
                min = j;
        }
        
        swap(a[min], a[i]);
    }
}

first iteration     //i = 0 = min , j= i+1 = 1  a[min] = 4  a[j]
4 23 17 25 8

comparing:
4 < 23
4 < 17 
4 < 25 
4 < 8

second iteration: 
4 23 17 25 8        //i = 1 = min   j = 2   a[min] = 23 

comparing: 
23 > 17
m = 2 a[min]= 17
a[j] = 8

end of second iteration: 
4 8 17 25 23

third iteration:    // i = 2 = min  j = 3   a[min] = 17
4 8 17 25 23 

end of third iteration: 
4 8 17 25 23 

fourth iteration:   // i = 3 = min j = 4 a[min] = 23
4 8 17 25 23 



4a 2 3 4b 1 
1 2 3 4b 4a
1 2 3 4b 4a
1 2 3 4b 4a 

Stable - N 
Loop invariant - Y 
In place - Y 
Adaptive - N 

Best scenario:
- O(N^2), sorted array 1 2 3 4

Worse scenario: 
- 8 6 5 4 3     //4 updates 
- 3 6 5 4 8     //3 updates 
- 3 4 5 6 8     // 

N elements: 
(N-1) + (N-2) + ... 1
see prof notes for the rest 

Insetion Sort:
--------------
1 2 3 4 5 

5 | 4 3 2 1 
4 5 | 3 2 1 // 1 shift 
3 4 5 | 2 1 // 2 shift 
2 3 4 5 | 1 // 3 shifts 

1+2+3+...(N-1) 

Test on next tuesday, until this chapter 

Lecture 4: Sorting (Part 2 )
================================================================================
Recursive allgorithms
---------------------

- Something is defined in terms of smaller version of iteself

Factorial
---------

fact(n)
{
    if(n==0)                //Base case 
        return 1;
    else
        return n*fact(n-1)  //recursive case 
}

factorial(n) T(n) = T(n-1) + C1 // C1 = (multiplication n* fact(n-1)

general case: 

T(n) = T(n-k) + kc1 

base case: T(0) +nc1
T(n) = C2 + nC1 
    => 0(N)

-----------------------------------------
fact(5) will call fact(4)
fact(4) will call fact(3)
fact(2) will call fact(1)
fact(1) will call fact(0) = 1

T(n) = 8n + 6n^2 + 5 = O(N^2);

fact(n) = n* fact(n-1)
f(n) = T(n-1) + cm


Recursive Algorithm:
--------------------


Merge sort: 
-----------
Main idea: Divide 2 merge - merge them back in order 

Recurrsively divide until single elements 







