============================================================================
Lecture 1: Data Structures
============================================================================
Abstract Data Types(ADTs):

1. Linked List 
2. Array 
3. Stacks 
4. Queues 
5. Trees (BST-Trees, AVL Trees, Splay Trees, Red Black Trees)
6. Graphs 
7. Hash Maps 
8. Skip Lists 
9. Heaps 

1. Queue:
--------
- Has beginning and end called front and back queue. 
- FIFO structure 
- Continuous 
- Bad for sorting

- Good use for storing things needed to be kept in order.
    e.g. Documents waiting to be printed on a network printer. 

2. Binary Tree
--------------
- An upside down tree 
- Top most (first node) is the root node 

- Each node holds 3 things:
    1. Data 
    2. left pointer 
    3. right pointer 
    
- Non continuous
- Very good for sorting ("Left" is smaller, "Right" is larger)
- Has no duplicates 

- Good use for searching sorted data 
- Lesser items to the left, greater items to the right 

3. Linked List 
--------------
- Each node contains 2 things :
    1. Data 
    2. Pointer to the next node. Last node has a pointer to null.
    
- Non contiguous 
- When items is deleted, it fills up the empty spot by making the previous
  node point to the next next node. 

4. Arrays 
---------
- int a[10];
- When deleting items in an array, it leaves a gap in between. 

================================================================================
Lecture 1: Memory Management 
================================================================================

Locality of Reference:
----------------------
1. Temporal locality (Time)
    - memory address which is used is likely to be used again. 

2. Spatial locality (Memory)
    - memory address that are close to a used address is likely to be used again.
    
#define SIZE 10000

int a[SIZE][SIZE];

for(int i=0; i < SIZE; ++i)
{
    for(int j=0; j < SIZE; ++j)
    {
        a[i][j] = 10;
    }
}

//------------------

#define SIZE 10000

int a[SIZE][SIZE];

for(int i = 0; i < SIZE ; ++i) 
{
    for (int j = 0; j < SIZE ; ++j)
    {
        a[j][i] = 10;
    }
}

Allocation Policies
-------------------
1. First Fit (waste space)
    - Search free list from beginning 
    - Uses the first free block that is large enough to satify the user request
    - Split the block and remainder is put on free list.
    
2. Next Fit 
    - Uses a roving pointer on a circuar free block chain 
    - Each allocation begins looking where the previous allocation finish
    - Avoids creating accumulation of small fragments at the head of free block 
      chain 

3. Best Fit (Not neccessarily the best)
    - Always allocates from the smallest suitable free block. 
    - Search for perfect fit 
    - First fit on a size-ordered free block chain 
    - Segregated fits 
    - May exhibit bad fragmentation, but not commonly observed

Buddy System
------------
- Not tested 

- Coalescing: when a block of larger size is requested, it merges back with its 
    buddy. 

- Buddy address = block address + original size / (2* level)

Attributes of Memory Managers:
------------------------------
- Ease of use 
- Performance
- Memory overhead
- Debugging Capabilities

Internal Fragmentation:
----------------------
- Continuous memory divided into 4kb pages. 
- Process A request 5kb block 
- 5kb is used by process A so 2 4kb pages are actually allocated, even though
  process A is only using 1kb of one of them. 
  
External Fragmentation:
-----------------------
- 1 continuous memory block allocated by A | B | A | C | B | C
- B frees its memory :  A | | A | C | | C 
- C request free memory but its contiguous and remained unused. 

Stack Allocators: (LIFO)
-----------------
- Eliminate fragmentation problems 
- The last block allocated is first freed. 

- Records the size of the block allocated in the header, know how much allocated
  per block so can free each block separately. 
  
- Disadvantage: Last in first out (LIFO) order 
- Allocate(size) -> allocate block at the top of the stack 
- Free (obj *) -> free based on LIFO

Pool allocator: (Fixed size block) 
----------------------------------
- Eliminates fragmentation since all blocks have the same size. 
- Not very useful for general purposes 
- Can use multiple sized pools
- Can be a waste of memory if using blocks of greater size than the memory
  needed. 
  
- Used in assignment 1, with page and blocks
- One linked list stores the page list, one more stores the free list. 
- The linked list utilizes the first block in the memory for each node as the 
  pointer to the next, for both page and free list. 
  
Alignment:
----------
Assume word = 4 bytes (i.e.g an int)

struct Strudent1
{
    int Age;        //4 bytes 
    char Initial_1  //1 bytes 
    char Initial_2  //1 bytes 
}

Student1* pS = new Student1[2];

pS[0].Age = 1 operation //wrong 
pS[0].I1  = 1 operation //wrong 
pS[1].Age = 2 operation //wrong 

//Add Padd!

struct Strudent1    //6 bytes + 2 pad = 8 bytes 
{
    int Age;        //4 bytes 
    char Initial_1  //1 bytes 
    char Initial_2  //1 bytes 
}

Student1* pS = new Student1[2];

pS[0].Age = 1 operation //correct
pS[0].I1  = 1 operation //correct
pS[1].Age = 1 operation //correct

================================================================================
Lecture 2: Algorithm Analysis 
================================================================================
Algorithm Analysis:
-------------------
1. Correctness analysis 
2. Complexity analysis 
    - Time 
    - Space 

Linear Search: (find out the value's position in the array)
--------------
- Linear time algorithm (time is directly proportional to the size of the array)

- Search one by one 
- Worse case: element at last position of array 
- Best case: 0th position of the array 

int linearSearch( int* array, int size, int value)
{
    for (int i = 0, i < size, ++i)
    {
        if(array[i] == value)
        {
            return i;
        }
    }
    
    return -1;
}

Shuffle:
--------
#include <cstdlib> 

void Shuffle(int* array, int size)
{
    for(int i = 0; i < size; ++i)
    {
        int random = rand() % size;
        int temp = array[i];
        array[i] = array[random];
        array[random] = temp;
    }
}

// Generate an unsorted array of unique integers
for (int i = 0; i < SIZE; ++i)
{
    a[i] = i;
}

// Mix it up
Shuffle(a, SIZE);

Binary Search:
--------------
- logarithmic-time algorithm (proportional to the logarithm of the size of the array)

- Elements are sorted in order 

int binarySearch(int* array, int size, int value)
{
    if(size <= 1)
        return 1;
        
    int count = 0;
    int left = 0; 
    int right = size - 1;
    
    while (right > left)
    {
        ++count;
        
        int middle = (left + right) / 2;
        
        if(value == array[middle])
            return count;
            
        if(value < array[middle])
            right = middle - 1;
        else 
            left = middle + 1;
    }
    
    return count;
}

Big-Oh Notation:
----------------
O() : Worse case asymptotic time complexity

- In O() notation, we only care about the dominant term. 

Dominant term: 
--------------
The term that will account for the biggest portion of the running time. 

1. 2n 
2. n^2 (square)
3. n^3 (cubic term
4. n^n (exponential term) 

Example of Big-O Notation Comparision:
--------------------------------------
O(6) = O(12)
O(N^2 + N) < O(N^3)
O(N+5) = O(N + 100)
O(Nlog2N + N ^3) < O(2^N)
O(log2N) < O(N)
O(N) + O(log2(2^N)) = O(N)
O(N/2) = O(2N)
O(lnN) = O(log10N)

Tight Big-Oh Bound: 
-------------------
f(n) = 8n + 128

- is f(n) in 0(n) or 0(n^2)?    //choose the tighter bound!

- Since n is in O(n^2), O(n) is tigher bound for f(n) than O(n^2)
- "f(n) is in O(n)" is a more accurate analysis 

Writing Big-Oh Expressions:
---------------------------
1. Determine running time // n^2 + (n log2N) + 3n

2. Drop all but the most significant terms 
    - O(n^2 + nlog2n + 3n) -> O(n^2)
    - O(n log2N + 3n) -> O(n log2n)

3. Drop constant coefficients 
    - O(3n) -> O(n)
    - O(10) -> O(1)
    
Common growth rates:
--------------------
O(k)        - Constant 
O(log2N)    - Logarithm 
O(N)        - linear (direct propotional to N)
O(Nlog2n)   - N log N 
O(N^2)      - Quadratic (proportional to square of N)
O(N^3)      - Cubic (proportional to cube of N)
O(N^k)      - Polynomial (proportional to N to the power of K)
O(a^N)(a>1) - Exponential (proportional to 2 to the power of N)


O(1) - will always execute in the same time or space regardless of the size of 
       the input data size. 
        
O(N) - performance will grow linearly and in direct proportion to the size of 
       the input data set. 

O(N^2) - performance will grow linearly and in direct proportion to the size 
         of the input data set. The example below demonstrates how Big O favours
         the worse-case performance scenario. A matching string could be found 
         during any iteration of the fr loop and the function woud return early,
         but Big O notation will always assume the upper limit where the 
         algorithm will perform the maximum number of iteration. 


bool ContainDuplicates(IList<string> elements)
{
    for(var outer = 0; outer < elements.Count; outer++)
    {
        for(var inner = 0; inner < elements.Count; inner ++)
        {
            //Dont compare with self 
            if(outer == inner) continue; 
            if(elements[outer] == elements[inner]) return true; 
        }
    }
    return false;
}

O(2^N) - growth doubles with each addition to the input data set. Growth curve 
         of an O(2^N) is exponential - starting off very shallow, then rising
         meteorically. 
         
        e.g. Recurrive calculation of Fibonacci numbers. 
        
int Fibonacci(int number)
{
    if(number <= 1) return number;
    return Fibonacci(number - 2) + Fibonacci(number - 1);
}

Fastest to slowest:
-------------------
FASTER // less dominant 

O(1)        - Constant 
O(log n)    - Logarithm
O(n)        - linear 
O(n log n)  - n log n 
O(n^2)      - Quadratic
O(n^3)      - Cubic 
O(n^k)      - Polynomial
O(2^n)      - Exponential

SLOWER // more dominant

Chaining together Big-O notation:
---------------------------------
g()                         <- O(N)
for(j=0; j < N; ++j) g(j)   <- O(N^2)

Performance vs Complexity:
--------------------------
Performance:
    - Time, memory, disk
    - Machine, compiler, code 
    
Complexity:
    - Time complexity: big-O 
    

Estimating the growth rate:
---------------------------
1. Constant time elementary operations: 
    - one arithmetic operation. e.g. +, *
    - one assignment 
    - one test e.g. x == 0
    - one read 
    - one write (of a primitive type)
    - ... 
    - T(n) = a != f(n) 
    
2. Sequences 

    e.g. 
    
    statement 1;
    statement 2;
    ....
    statement k;
    
    e.g. 
    total time = T(statement 1) + T(statement 2) + ... + T(statement k)

3. Conditionals 
    
   e.g.
   
   Total time = max(T(sequence 1), T(sequence 2))
   
   if(condition)
   {
       sequence of statement 1 
   }
   else 
   {
       sequence of statement 2
   }
   
4. Loops (this is the big-ticket item) 

    e.g. 
    
    Total time = N x T(statement)
    
    for(i = 0; i < N; ++i)
    {
        sequence of statements 
    }
    
    e.g. Nested loops
    
    Total time = N x N x T(statements) 
    
    for(i = 0; i < N; ++i)
    {
        for(j = 0; j < N; ++j)
        {
            sequence of statements 
        }
    }
    
5. Function calls 

    f(n);       //O(1)
    g(n);       //O(n)
    
    for(j = 0; j < N; ++j)
    g(j);
    
    total time = O(N^2)

================================================================================
Lecture 3: Sorting
================================================================================
Bubble Sort:
------------
Elements "bubble" to the top of the list, by continuous comparison and swaps.
Bubbles on the top are regarded as sorted. 

Selection Sort:
---------------
Two groups, sorted and unsorted. The smallest from the unsorted (Which is still
bigger than the largest in the sorted) is select and put in the sorted at the 
end of the sorted group. 

Insertion Sort:
---------------
Two groups, sorted and unsorted. The first in the unsorted is inserted into the 
sorted group. 

Merge Sort:
-----------
Every element is an individual group at the start. Sorted elements are merged 
then sorted. 

Quick Sort:
-----------
idk how the fuck to memory this. 

Attributes of Sorting Algorithms:
---------------------------------
1. Stable/unstable 
    
    - Preserves the relative order of items with equal values. 
    - 7,5,6,5,2 -> 2,5,5,6,7 
    - which 5 goes in front of which? if it cares, its stable, otherwise, it is 
      not stable. 
    
2. In-place/Out-of-place sorting 
    
    - If there is an array of 5 and the algorithm uses exactly the same amount 
      of memory, it is in-place sorting, otherwise, it is out of place. 
      
    //Example 

    void f1(int a[],int n)
    {
        int* b = new int[n];
        int i;

        for(i=0;i<n;++i)
            b[n-i-1] = a[i];

        for(i=0;i<n;++i)
            a[i]=b[i];

        delete [] b;
    }
    
    void f2(int a[], int n)
    {
        int i;
        int m=n/2;

        for(i=0;i<m;++i)
        Swap(a[i],a[n-i-1]);
    }
      
3. Adaptive 

    - if the array is sorted partially and the algorithm takes advantage of it, 
      It is adaptive. 

4. Loop Invariant (not gone much into in this course, probably not tested)

    - Loop variant is a property that holds before and after each repetition. 
    - e.g. It is true that for every iteration, i+j == 9. 
    
    - A weaker invariant that is also true is that:
        - I >= 0 && i < 10 // because this is the continuation condition 
        - or that j <= 9 && j >= 0

    //Example 

    int max(int n, const int a[n]) 
    {
        int m = a[0];     // m equals the maximum value in a[0...0]
        
        int i = 1;
        
        while (i != n)    // m equals the maximum value in a[0...i-1]
        {
            if (m < a[i])
                m = a[i];   // m equals the maximum value in a[0...i]

            ++i;            // m equals the maximum value in a[0...i-1]
        }
        
        return m;         // m equals the maximum value in a[0...i-1], and i==n
    }

int j = 9;

for(int i = 0; i < 10; ++i)
    --j;

Bubble Sort:
------------
Exchange Sort: switch a pair of neighbouring elements. 
Main idea: keep bubbling the largest element to the end. 

1. Get the current i and i+1 elements 
2. Swap them into the correct order 
3. Start from front again when reach the end. 

Example: 2, 8, 5, 3, 9, 4, 1 

 



Bubble Sort: (Non-Adaptive Implementation)
------------------------------------------

Stable:         Y 
Loop invariant: Y 
In-place:       Y
Adaptive:       N

Best case complexity: O(N)
Worst case complexity: O(N^2)

void BubbleSort(int a[], int N)
{
    for (int i = 0; i < N - 1; ++i)             // number of iteration 
    {
        for (int j = 0; j < N - i - 1; ++j)     //inner looop 
        {
            if (a[j] > a[j + 1])
                Swap(a[j], a[j + 1]);
        }
    }
}

void Swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

Bubble Sort: Adaptive Implementation
------------------------------------
Worse case: O(N^2)
Best case : O(N) 

//Doesnt do the inside loop since it always goes into the else in the if conditional

Stable
In-place 
Adaptive or Non-adaptive depending on implementation - one way to make it
adaptive is to use a boolean to check if a swap has to be done, in which case its adaptive. 


void BubbleSort(int a[], int N)
{
    for (int i = 0; i < N - 1; ++i)
    {
        bool swap = false;
        for (int j = 0; j < N - i - 1; ++j)
        {
            if (a[j] > a[j + 1])
            {
                swap=true;
                Swap(a[j], a[j + 1]);
            }
        }
    }
    
    if(!swap) return;
}

Selection Sort:
---------------
Main idea: 
Increase the sorted sequence by selecting the smallest element from the unsorted.

1. Select the smallest element from the unsorted side. 
2. Append it at the end of the sorted side (or the first position of the unsorted)

Find the smallest, then the second smallest from the rest...

Stable:         N 
Loop invariant: Y
In-place:       Y 
Adaptive:       N (Worst-case and best-case are the same) 

Best case complexity: O(N^2)
Worst case complexity: O(N^2)

void SelectionSort(int a[], int N)
{
    for (int i = 0; i < N-1; ++i)
    {
        int min = i;
        int j;
        
        for (j = i + 1; j < N; ++j)
        {
            if (a[j] < a[min])
                min = j;

            Swap(a[min], a[i]);
        }
    }
}

Improved Selection Sort: 
------------------------
void BiSelectionSort(int a[], int N)
{
    for (int i = 0; i < N/2; ++i)
    {
        int min = i;
        int max = N-i-1;
        int j;

        for (j = i; j < N-i; ++j)
        {
            if (a[j] < a[min])
                min = j;

            if (a[j] > a[max])
                max = j;
        }

        Swap(a[min], a[i]);

        if (i==max)
            Swap(a[min], a[N-i-1]);
        else
            Swap(a[max], a[N-i-1]);
    }
}

Insertion Sort:
---------------
Main idea: keep inserting next element into the sorted sequence. 

1. Get the 1st element from the unsorted side 
2. Find the correct position in the sorted side 
3. Shift elements to make room 

Stable:         Y
Loop invariant: Y 
In-place:       Y
Adaptive:       Y

Best case: O(N)    //if sorted, while loop doesnt execute 
Worse case: O(N^2) //while loop constantly being called with tons of steps  

void InsertionSort(int a[], int N)
{
    for (int i = 1; i < N; ++i)
    {
        int j = i;
        int current = a[i]; // the item to be inserted

        while ((j > 0) && (a[j-1] > current))
        {
            a[j] = a[j-1];
            --j;
        } // find the position for insertion: j

        a[j] = current;
    }
}

Recursive algorithm:
--------------------
Example:
- Finding length of string 
- Binary Search 

- recursive slower than iteration, due to the overhead of maintaining the stack 
- recursive uses more memory than iteration, due to stack memory 

size_t(char* s)
{
    if (*s == 0)                //base cases 
        return 0;   

    return 1 + length(++s);   //recursive calls
}

int binarysearch(int a[], int x, int low, int high)
{
    int mid = (low + high) / 2;     //base case

    if (low > high)                 //base case 
        return -1;
    else if (a[mid] == x)
        return mid;
    else if (a[mid] < x)
        return bsearch(a, x, mid + 1, high);    //recursive calls
    else
        return bsearch(a, x, low, mid - 1);     //recursive calls
}

Merge sort (recursion algorithm)
--------------------------------
Main Idea: Divide and merge 

1. Recursively divide sequence until single element 
2. Merge them bac together in order 

Best case: O(nlog2n)
Worse case: O(nlog2n)

A[9] = { 7, 4, 1, 3, 8, 6, 5, 9, 2 }
A'[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9}

Split:      
------
                          3, 1, 4, 1, 5, 9, 2, 6, 5, 4 (Duplicates)  
                         /                              \   
          3, 1, 4, 1, 5                                9, 2, 6, 5, 4                     
         /            \                               /              \ 
     3, 1           4, 1, 5                        9, 2            6, 5, 4
    /    \         /       \                      /   \           /        \                           
   3      1       4       1, 5                   9     2         6        5, 4
                         /    \                                          /    \
                       1        5                                       5      4
Sort & Merge:
-------------
                         \    /                                           \    /
                          1, 5                                             4, 5
   \    /         \        /                   \    /               \       /
    1, 3           1, 4, 5                      2, 9                 4, 5, 6
       \            /                               \               /
       1, 1, 3, 4, 5                                  2, 4, 5, 6, 9
                     \                              / 
                       1, 1, 2, 3, 4, 4, 5, 5, 6, 9                                

void DoMergeSort int a[], int left, int right)
{
    if(left < right)
    {
        unsigned const middle = (left + right) / 2;
        DoMergeSort(a, left, middle);
        DoMergeSort(a, middle + 1, right);
        Merge(a, left, middle, right);
    }
}

void Merge(int array[], int left, int middle, int right) 
{
    unsigned i = left; // counter for the temp array
    unsigned j = left; // counter for left array
    unsigned k = middle + 1; // counter for right array
    
    int temp = new int[right + 1];
        
    while (j <= middle && k <= right)
    {
        if (array[j] <= array[k])
            temp[i++] = array[j++];
        else
            temp[i++] = array[k++];
                            
        while (j <= middle)
            temp[i++] = array[j++];
                                
        while (k <= right)
            temp[i++] = array[k++];
                                                    
        for (i = left; i <= right; ++i)
            array[i] = temp[i];
                                                        
        delete[] temp;
}

Quick sort (recursion algorithm)
--------------------------------
Main Idea:
Divide a large array into two smaller sub arrays -high and low elements 

Steps: 
1. Pick a pivot p element from array
2. Partition the remaining elements in 2 parts L (lesser) and G(greater)

- reorder the array 
- Elements with values less than pivot comes before the pivot 
- Elements greater than the pivtos comes after the pivot.
- Equal values can go either way 

3. Recursively quicksort the unsorted L & G 

Best case: O(nlog2n)
Worse case: O(N^2)      //Pivot might always be max/min

[P, [SSSSSS], [LLLLL], C, [RRRRRR]]

P = pivot           //green 
S = smaller         //purple 
L = larger          //blue 
C = current         //black
R = remaining       //red

if C >= P, leave it at the same place 
if C < P , swap with the first L element 
At the end swap with the pivot with the last S element 

//pivot: 4, next item (current): 7 > pivot 4, no need swap
//{pivot, current, rest, rest, rest, rest, rest, rest}
A[8] = { 4, 7, 1, 3, 8, 6, 5, 2 }  

//pivot: 4, next item (current): 1 < pivot 4, swap with first large element which is 7
//{pivot, larger, current, red, red, red, red, red}
4, 7, 1, 3, 8, 6, 5, 2 

//pivot: 4, next item (current): 3 < pivot 4, swap with first large element which is 7
//{pivot, smaller, larger, current, red, red, red, red}
4, 1, 7, 3, 8, 6, 5, 2

//pivot: 4, next item (current): 8 > pivot 4, no need swap
//{pivot, smaller, smaller, larger, current, red, red, red}
4, 1, 3, 7, 8, 6, 5, 2 

//pivot: 4, next item (current): 6 > pivot 4, no need swap 
//{pivot, smaller, smaller, larger, larger, current, red, red}
4, 1, 3, 7, 8, 6, 5, 2 

//pivot: 4, next item (current): 5 > pivot 4, no need swap
//{pivot, smaller, smaller, larger, larger, larger, current, red}
4, 1, 3, 7, 8, 6, 5, 2

//pivot: 4, next item (current): 2 < pivot 4, swap with first large element which is 7
//{pivot, smaller, smaller, larger, larger, larger, larger, current}
4, 1, 3, 7, 8, 6, 5, 2

//End of list. Swap pivot with last smalle element which is 2
//{pivot, smaller, smaller, smaller, larger, larger, larger, larger}
4, 1, 3, 2, 8, 6, 5, 7

//{smaller, smaller, smaller, pivot, larger, larger, larger, larger}
2, 1, 3, 4, 8, 6, 5, 7

void QuickSort(int a[], int left, int right)
{
    if (left < right)
    {
        int i = Partition(a, left, right);
        QuickSort(a, left, i - 1);
        QuickSort(a, i + 1, right);
    }
}

unsigned Partition(int a[], int i, int j)
{
    int p = a[i];
    int h = i;                       //the position of the first larger element 

    for (int k = i + 1; k <= j; ++k)
    {
        if (a[k] < p)
        {
            ++h;                    //increment larger element from pivot to first pos
            Swap(a[k], a[h]);       //swap a[k] with first larger element  
        }
    }

    //swap pivot a[i] with first larger element which is now the last smallest element
    Swap(a[h], a[i]);               
    
    return h;
}

//Randomize Quicksort
void RandomQuickSort(int a[], int left, int right)
{
    if (left < right)
    {
        int i = RandomPartion(a, left, right);
        
        RandomQuickSort(a, left, i - 1);
        RandomQuickSort(a, i + 1, right);
    }
}

unsigned RandomPartition(int a[], int i, int j)
{
    int r = rand() % (j - 1) + i + 1; 
    Swap(a[i], a[r]);

    int p = a[i];
    int h = i;

    for (int k = i + 1; k <= j; ++k)
    {
        if (a[k] < p)
        {
            ++h;
            Swap(a[k], a[h]);
        }
    }

    Swap(a[h], a[i]);

    return h;
}
