cs120 C89 week 1

#include <stdio.h>      //Preprocessor Directive #

int main(void)
{
  printf("Hello World \n");
  return 0;
}

gcc -Wall -Werror -Wextra -Wconversion -pedantic -ansi -o output.exe helloworld.c
must type: -ansi in c89*/


header guard: 
#ifndef A
#def A

#endif


#include "A.h"  //will include header
#include "A.h"  //wont include header because repeated 

Compile process:
1. Preprocessing - copy paste 
2. Compilation - translate code into machine instruction (object code) (-c gcc)
3. Linking - 
4.

Error: 
1. Compile error 
2. Linker error - unresolved ext symbol. got declaration, no definition.
3. logical error - code makes no sense

Never put #include in header file 

.h should provide interface
.c should contain main 

Computer Architecture:
- CPU (ALU, Control Unit) (Registers)
- Memory (ROM & RAM) Cache
- Input/Output

visual studio -> Assembly. Type in C then go to disassemble under debug tools.

**********************************Lesson 2**************************************
8 bits = 1 bytes
0000 0011

types:

char - 1 bytes (char c = 'a';)
short(int) - 2 bytes
int - 4 bytes
float - 4 bytes (Single precision)
double - 8 bytes (double precision)

Qualifiers:
- Unsigned (unsigned char range: 0-255, char range:-128-127, wont change range)
- long (long int - 4 bytes, long long int - 8 bytes)

Identifiers:
- may contain letters, digits & underscores 
- No spaces
- Numbers never at the front, but can middle and back. 
- Cannot be keyword
- Case sensitive

int main(void)
{
  int a = 2;    //variables to be declared all at the top
  int b = 3;
  int c = 0;
  int d = 0;
  
  c = b + a;
  
  int d = c + a;  //cannot compile
  
  d = c + a;
}

C operators:
- Arithmetic (+,-,*,/,++,--)

  e.g. unary +, - : +a , -a;
  e.g. binary +, -: a = a+b;
  a++ = a = a+1;
  
  int a = 1;
  a ++ will return 1;  //will generate temp variable
  ++a will return 2;   //always use ++a
  
- Relational (==, !=,>,<,>=,<=) - will return 1 or 0

  e.g. if(a==b)
  e.g. a = (a==b) //output: 1 or 0
  
- Logicial (&&, ||, !) output: non-zero = true, 0 = false

  e.g. int a=5, b=5, c=5; if(a==b==c), cannot compile. ((a==b)==c) 1==c
  e.g. if(a==b && a==c)
  
- Assignment (=, +=, -=, *=, /=, %=)
  e.g. a=b=c; equals to a=c;
  e.g. a+=b+=c;
  e.g. a+=b; equals to a=a+b;
  
  
  9/5 = 1 (quotient)
  9%5 = 4 (remainder)

  to check for odd and even: modulus by 2

- Bitwise(&,|,^, ~, <<, >>) - result in bits
  
  int a = 3, b = 5; 
  e.g. a & b output: 001
  e.g. a | b output: 111
  
  << - bit shift left
  >> - bit shift right
  
  a<<=2; 
 
  00000101 will shift 2 slots to the left becoming 00010100, shift by 2 slots
  will double the value because power of 2. 
  
- Miscallaneous (sizeof(), & called address,* called dereference, ? , comma)
  
  int a;
  e.g. sizeof(int) //output:4
  e.g. sizeof(a)   //output:4
  e.g. &a;
  
  int a = 5;
  e.g. int*ptrToA == &a;
       *ptrToa = value 
  
  e.g. A?B:C if A is true, do B else C
  
  comma operator: 
  a = 5, b = 10;  //a evaluate first, then evaluate b. But discards a
  if(a==b, a =5, b = 10) // a==b result = 1, a==b discarded, cant compile 
  
  if(a=5,a+=10,b=0) 
//output:can execute chain of expression but will return only the last one b=0;
  
- precedence & Associativity
  
  a+b*c //evaluate b*c first 
  
  a=b+=c++-d+--e/-f
  
  -- and ++ are always the fastest
  unary +, - are 2nd fast
  divide 
  + or -, read left to right
  
  i=2;
  j = i*i++; //output: j = 2*2
  
*******lab 1:**********
  Section B&C
  
  for TRUE/FALSE answers, it should be like... expr?"TRUE":"FALSE"
  
  Section C : Bitwise Operators 
  A = 011
  B = 101
  e.g A|B = A or B = 011
                     101
                     111
  
  A&B = A and B = 011
                  101
                  001
                  
        A = 0010 1011
        B = 0000 1111
     same = 0000 1011 (A&B)
different = 0010 0100 (A^B) XOR
Common bits of A & B = 0000 1011
different bits of A&B = 0010 0100

Section A last Qn: (Update B&C)
Section B last Qn: (Is A, B, C the value 0?)

1   00000001
<<3 00001000
&   10110001

*******************************Lesson 3*****************************************
Statements & Flow Control
loops

Need control of execution

Selection statements -> if-else, switch 

Iteration (Loops) statements -> while, do, for 
Jump statement  -> break, continue, goto, return

if (expression) statement
*************************

if (a)
{
  a++;
  a--;
  a;
}

if (expression) statement
else statement 

if(a==5)      //use 5==a, if forgot type equal becomes 5=a so will have error
{
}
else (expression 2)
{
}

if(y!=0)
  if(x!=0)
    result =x/y;
else // else will cling to the last unbounded if in line 226
  printf("cannot divide by 0");
  
Switch case
***********

switch (expression)
{
  case constant_expr: statements
  .
  .
  .
  case constant_expr: statements
  default: statements (default is optional)
}

e.g.
char keypressed;
switch (keyPressed) 
{
  case 'w': ......;     //cannot have duplicates
  break;
  case 'A': ......;
  break;
}

switch > if else 
  
if(keyPressed == 'A')
else if(" == 'W')
else if (' == 'D')
.
.
.

****while (expression) statement****


while(health >0)
  enemySpawn++;
  
while(i<10)
{
  ++i;
  do other stuff;
}

while (1)     //while true
{
  if (gameOver)
    break;
}

do statement while (expression);


for(expr1; exp2; exp3) statement 
expr1 = initializer
expr2 = conditional 
expr3 = increment / post update

for (i=0;i<10;++i)
{
  ....
}

exp1;           
while (expr2)     // same as for loop
{
  ....
  expr3;
}

for( ; i<10; ) //same as the above while looop

for ( ; ; ) //infinite loop same as 
while(1){}
  
for ( ; ;)
{
  if (gameEnded)
    break;        //break will jump to the next bracket
  Render();
  UpdateObjects();
}

for (; ; )        //nested for loop 
{
  for (; ; )
  {
      if (gameEnded) break; //break out of inner loop
  }
  break; //break out of outer loop
}


***continue***

int enemyType[100];

for(i=0; i<100;++i)
{
  if(enemyType[i]=='p') 
    Continue;   //skip and increment, then continue the loop 
}

for (i=0; i<100;++i)
updateEnemy (&enemytype[i]);  //no curly brackets

for(i=0;i<100;++i)      //run 10000 times
{
  for(j=0;j<100;++i)
  {
    //statement
  }
}

for(i=0;i<100;++i)   //run 100 times, brackets are important 
for(j=0;j<100;++j)  // run 100 times, brackets are important

*****goto*******

label -> identifier : statement
         
goto identifier    //hard to manage but super powerful, most fundamental of jump

e.g.
goto LOL;   //jump pass initializer, then increment and loop
         
for(i=0; i<100;++i)
{
  LOL: UpdateEnemy();
}

or e.g.

for (i=0;i<100;++i)
{
  LOL: UpdateEnemy();
}
goto LOL;   //infinite loop

for()
{
  for (i=0; i<100;++i)
  {
    if(....)
      goto exitloop;    //will get out of nested loop, goto to be useful
  }
}



exitloop:;statement;
}


All the wrong things:

if(exp);  //semi colon    
statement 

for(....);  //semi colon. for loop true, execute semi colon, execute statement once
{
  statements
}

while (expr); //semi colon
{
  statements;
}

*******************************lesson 4*****************************************
Arrays / Strings of Text 

Scalars - Hold a single data item 
Aggregates - Arrays & structures (Collection of values)

int a = 10;
int a[10];    //10 of them, row of 10 ints or called pointers, only can have static values

Array has to be Contiguous // packed together 

access variable itself -> a[0] to a[9] // start from 0 to 9, size = 10;

Array Subscript access always start with 0, end with N-1 e.g. a[i+j*10]

Array initialisation:

int a[10]={1,2,3,4,5,6,7,8,9,10};
int a[10]={1,2,3} //first 3 initialised, the rest become 0;
int a[10] {0}; // easy default all to 0
int a[] = {1,2,3}; //Size = 3, length = 3?

Designated Initialisers:

int a[100]={0};   //init array[10]=5, array[20]=100, rest to 0
a[10] = 5;
a[20] = 100;

int a[15] = {[2]=29,[4]=10};
int a[] = {[2]=29,[4]=10};  //size = last element which is 5
int c[10] = {5,1,9,[4]=3,7,2,[8]=6}; // 3rd & 7th element = 0, 

sizeof() operator: // can put variable or type

sizeof(int) //4 bytes

int k;
sizeof(k) //4 bytes 

int a[10];
sizeof(a) // 40 bytes
sizeof(a[0]) // 4 bytes 

sizeof(a)/sizeof(a[0]) // give u element size of 40/type(int) = 10 element

void foo(int a[]) //will not be an array in a function

0x0 to 0xFFFFFFFF

Multidimentional Arrays:

int m[3][4]; //3rows, 4 columns, 3x4 = 12 elements

- Row Major order, row will go first 

0,0|0,1|0,2|0,3|1,0|1,1|1,2|1,3|2,1|2,2|2,3|3,0|3,1|3,2|3,3|
     m[0]      |     m[1]      |        m[2]   |     m[3]
     
int m[3][4] = {1,2,3,4,5,6,7,8,9,10};  // last 2 elements = 0
int m[3][4] = {{1,2,3,4},{5,6,7,8},{9,10}};

int m[1][] = {......};

*never ever ever do: (assignment of arrays)

int a[10]; int b[10];

a=b;  //illegal because pointers

Strings:

No such thing as string type in c and c++

String literals - "Example"   //double quote is string literals

""Example"", cannot compile // starting quote need to match with ending quote ""
\ is known as escape character //next character after / will be interpreted 
"\"Example\"" , can compile 


should be, const char* a = "Example";

'E'
'X'
'A'
'M'
'P'
'L'
'E'
/0

NULL-Terminated Strings

sizeof("example") //size: 8

char buffer[256] = "Default";
char buffer[] = "Default";  //size = 8
char buffer[256] = {'a','b','c'};

char abc[4] = {'a','b','c','d', 0};
char abc [] = "abc";
char abc [3] = "abc"; //cannot compile 

"abc"[1], access 2nd element of abc which is b 

String (character pointers) not equal to char   "a" Not equal to 'a'

"a" - location in memory that stores {'a', 0};
'a' - number in ascii

0 not equal to '0'
'0' = character 0 = 43 in ascii
0 = zero 

char buffer[7] = "Default" //Compiler wont complain

char a[] = "Example";
a[4] = 0;
printf{"%s", a);    //output: Exam 

strlen(a);  //4

character array vs character pointers:

char a[] = "abcdef";// can modify, modify string using array[]
char *a = "abcdef"; //same, cannot modify

sizeof(a) // output: 7 (size of char)
sizeof(a) // output: 4 or 8 (size of pointer)

text //string literals, global scope, cannot modify, protected memory 
heap //malloc and free
stack //array

void foo(int a[]) //int*a
{
  ...
}
int a[10];
foo(a);

void foo(int*a, int length)
{
}

lab 2

DiamondPrint 13

1234567890123210987654321

diamondprint 13 into 

argc - argument count, number of argument provided by user, willl count exe
itself. e.g. Diamondprint 13, count = 2 
argv - argument vector, array of arguments/text Diamonprint 13 
1st ways char** argv
2nd way char* argv[]

DiamondPrint.exe 3

49 51
1  3

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char*argv[])
{
  int i = 0;
  
  printf("argc=%i\n", argc); 
  
  for (i=0; i<argc;++i)
  {
    printf"argv[%i] string value = %s, integer value = %i\n", i, argv[i], 
    atoi(argv[i]);
  }
  return 0;
}

int main(int argc, char*argv[])
{
  int inputValue = 0;
  int i,j;
  
  if(argc!=2)
  {
    printf("Error -Invalid usage\n");
    return 1;
  }
  
  if(num <= 0)
  {
    printf("Invalid number");
    return 1;
  }
  
  /*attempt to convert 2nd element of arguement into an integer*/
  
  intputValue =atoi(argv[1]);
  
  if(inputValue <= 0)
  {
    printf("Error -Invalid number given\n");
    return 1;
  }
  
  /*We now have a proper input value to print, proceed...*/
  
  for (i=0;i<inputValue;++i)      //column 
  {
    for (j = inputValue-1;j<inputValue; ++j) //row
    {
      printf("*");
    }
    
    for (j = i;j<inputValue; ++j) //row
    {
      printf("%i", j%10);
    }
    printf("\n");
  }
  
  
  
  return 0;
}


Game loop 

while(isGameOver)
{
  ProcessInput();
  ProcessGamelogic();
  UpdateObjet();
  RenderToScreen();
}

 1
121
 1

cppreference
atoi - array to integer 

*******************************Lesson 5*****************************************
Prgram Organization 

Formatted Input & Output

printf(string, expr1, exp2,....)

string - will also start with string literal, called format string
% - conversion specifier 

expr1, expr2, .... - varidic Argument List, dump any number of expression in the
function 

%[Flags][width][.precision][length]specifier - c = char, dori = int(signed),
e/E = float w/ exponential, f = float, g = (use f or e, choose shorter),
o = octal, s = string, x/x = hexadecimal, u=unsigned int, p = pointer address,
n = nothing 

minus sign(-) - left justify
plus sign(+) - force +/- print 
space - no sign will leave space 
# - precede w/ 0 or decimal points
0 - left pcd w/o 0

h - short
l - long 
L - long double

int i;
i=printf("....") //can do 

number of printed character, just get from returned number

 -ve number if fail, return -ve
 
scanf(string, expr1, expre2...) //

scanf("%d %f", &myint, &myfloat); /*use %s get chunks of text till whitespaces, 
will never get stucked*/

need memory, need program to store allocation 
always pass in address

scanf will return the number of scanf they it scanned. 
Space is seperator 

scanf wait for input response, only when u press enter then will process 

puts(str); - prints string with new line \n //string 
gets(buffer); - read input until new line \n & discards newline. It accepts 
white space /*for string, if want sentence, use gets(), only gets one line, need 
use multiple*/

getchar() - get return value of one input //keypresses 

int read_line(char str[], int n )
{
  int ch, i = 0;
  while ((ch=getchar())!='\n')
  {
    if(i<n)
      str[i++] =ch;
  }
    str[i] = '\0';
    
    return i;
  }
}

char str[] - buffer memory to write in
int n - limit 

char inputBuffer[256];
readline (inputBuffer, 256);

<string.h>

char*

char* strcpy(char*s1, const char* s2);
s1 - destination , s2 - source 

strncpy(dest, src, size limit) //always use this version 
{
  
}

strncpy(str1, str2, sizeof(str1)-1)
{
  str1[sizeof(str1)-1] = '\0';
}

size_t strlen(const char*) //return length of string

size_t - default sizetype for machine, ios

char* strcat (char* str1, const char* str2) //concatenate str2 to str1

char* strncat();

strcmp (const char* str1, const char*str2) // comopare and return lexicmp result

char*str1, char*str2

str1 ==str2; //never ever do this, because comparing memory address, use strcmp

can put in non-const into const, but cannot const to non-const

char[][] vs char ** //different 

| | |
| | | vs |a| | | | a-> |x| //memory a pointing to another memory x 
| | |

Program Organization:

- Directives
- Body Statements
- Declarations (structures & Function Prototypes)
- Functions (stores Procedures/Sub routines)

struct StudentData
{
  .....
};

int Foo(int a)
{
  .....
}

if A uses B, B uses C, C uses A 

how to declare?

A
B
C

put int Foo(int a); //include semi-colon after declaring

// can have duplicate function but cannot have duplicate declarations 

never write #includes in header files 

Scoping :

local variables (automatic storage, blocks scope(c99))
static local (static storage duration, blocks scope)
external / global (static storage duration, file scope)

text  //abc, texts store constant and global
heap
stack //string pointing to abc

char*string = "abc";


static local:

void foo()
{
  static int i = 0; // global variables stuck in function
  i++;
  printf("%d\n", i);
}

//Always refer to the nearest i

int i;    //1, global 

void f(int i)   //2
{
  i=1;//refer to 2
}

void g(void)
{
  int i=2; //3
  if(i>0) //refer to 3
  {
    int i; //4
    i=3; //refer to 4
  }
  i=4; //refer to 3
}

void h(void)
{
  i=5; //refer to 1
}

******************************Lesson 6******************************************
Pointers
Dynamic Data Allocation 

Pointer Variables:

- Memory divided in bytes 
- Addressable via location 
- It is also a variable, takes space in memory
- Content is address (is just a number)

To declare, add * infront of vairable name 
e.g. int *pInt  
e.g. double* pDbl;

int - 4 bytes
double -> 8 bytes 
All poiners -> 4/8 bytes  32bit/64bit address

To assign, use &(Address of) operator, 
e.g. int i=0; pInt=&i;

pInt++ is moving address by 1, not value 

To modify value/access pointee value, use * operator (dereference)

(*pInt)++ = increase value by 1.    

int i=10;
int*p, *q;
p=&i;
q=p; 
q++;

Address   Name  Value 
200       i     10->11->12
1200      p     ?->200
1400      q     ?->200->204 (increase by 4 bytes int)

(*p)++; //10 to 11
(*q)++; //11 to 12

int i=10;
int j=10;
int*p, *q;
p=&i;
q=&j;

Type                          Address   Name  Value 
int                           200       i     10
Pointer to Int                1200      p     ?->200
Pointer to Int                2400      q     ?->2600
int                           2600      j     10
pointer to (pointer to int)   3000      ppInt 2400

if(*p==*q) //compare value pointed
vs
if(p==q) //compare addresses 
vs 
if(&p==&q) //can compile but will never become true 

int** ppInt =&q;

int main(void)
{
  int i=10;

  void Add(int input)  //Pass by value
  {
    input +=5;
  }
  add(i); //i=10, no change 
}

int main(void)
{
  int i=10;

  void Add(int*input)  //Pass by "reference" 
  {
    *input +=5;
  }
  add(&i); //i=15
}

int main(void)
{
  int* Foo()
  {
    int lol;
    return &lol; //never return address of local variable int lol
  }
}

q=Foo();

int Boo()
{
  int omg;
}

Type                          Address   Name  Value 
int                           5000      omg    X
int                           6000      input  5000

Dynamic Data Allocation:

text/data
heap - magic, 3 function, dynamic allocated memory (lol memory), doesnt disappear
unless u do free
stack - Automatic local variables (foo and lol), after function, foo and lol 
will be deleted 

To solve:

/*This allocates memory and please deallocate*/
int* Foo()
{
  int* lol =malloc(sizeof(int)); //variables from within to outside, malloc
  return lol;
}

Given in <stdlib.h>
- malloc (simply allocate a block of memory)
- calloc (allocate and clear)
- realloc (Resize a previously allocated block)

void* //typeless pointer
void*!=NULL (void is a type, NULL is a value which is 0)

NULL is a 0 but it may not be a zero. 

int NULL = 0; //cannot
&NULL         //cannot

malloc -> void* malloc(size_t size) //void pointer can point to any pointers 
malloc -> malloc(sizeof(int)*100)

malloc(sizeof(char)*strlen(inputStr)+1)

calloc -> void* calloc(size_t member, size_t size)
initialise all to 0

realloc -> void *realloc (void* ptr, size_t size)

if fail, return is NULL
if Null, if works like malloc
if input size as 0, it will work as free 

void free(void*ptr)

0x8BADFOOD // crashes phone 

void free (void*ptr), //if got memory leak, wont affect system. 

// will affect system, if keep memory leak non stop 

memory leak != resource leak, memory kept during the process only 

q=malloc(sizeof(int))
q= 0 // cannot reaccess q because assigned to 0 already 

free(q); //dangling pointer 
q=NULL; // after free then u NULL it. 

(*q)++; // fail because cannot access NULL already 

Operators:

New_Node ->value = 10; 
(*New_node).value = 10;


lab 3:

IntToString:

123 -> 12\0

if(s1==NULL)
return 0;

int result = 0;
while(s1[result]!=0)
{
  ++result;
}
return result;





int curr = 0;
if(s1==NULL)
  return;

while(s1[curr]!=0)
{
  if(s1[curr]>='a' && s1[curr]<='z')
    s1[curr]+='A'- 'a';
  ++curr;
}

char*srcCpy =NULL;
int srcLength = 0;



if(dest==NULL||source == NULL || length <=0)
  return;

srcLength = StringLength(source);

srcCpy=malloc(sizeof(char)*(srcLength+1));

if(srcCpy==NULL)
  return;

for(curr=0; curr <srcLength;++curr)
  srcCpy[curr] = source [curr];

srcCpy[curr] = source[curr];
srcCpy[srcLength]=0;

curr =0;
while(srcCpy[curr]!=0)
{
  if(curr>=


*********************************Lesson 7***************************************
Pointers & 
Array 

Pointer Arithmetic - 3 forms:

1. Add int to pointers (depends on types)
2. Subtract int from pointer (depends on types)
3. Subtract pointers from pointers (return integer /type)

int a[10];
int *p, *q;, i;

p = &a[0]; //a[0] is faster than &, get affrs of 1st element in an array.
*p = 5;   //Deference & assign value 
q = p + 3; //"Adds" to pointers 3 steps of int 
q = p -3; // "Subtract" pointers to move 3 steps of int backwards

a[3] -> *(a+3); 
p = &a[5]; q = &a[1];

i = p - q; (2 possibility: 5-1, or 0x20-0x06 = 16 divide by bytes which is 4 = 4)
i = q - p // Ans: -4 

Comparing Pointers 
- Normally use == , !=

e.g. 
p = &a[0];
q = &a[0];
p!=q;

*p = 0;
*q = 0;
p==q; (cannot because comparing address)

-Not normal: In array possble to use >, <, <=, >=

while(q<p)  //never use it. 
{
  *q = 0;
  ++q;
}

correct usage:

given an array a[N];
for(p=&a[o];p<&a[N];++p)
  
Combining * and ++/--

*p++ // ++ first then *, but ++ afterwards, so dereference p but not p +1; 
*p++ // same as *(p++);
(*p)++
*++p
++*p 

while(*p!=0)
*p++ = 0; 

p=&a[0];
while(p<&a[N])
  sum+=*p++;


lab 4:

"Hello World" //buffer is 12, strlen is 11, ignore null terminator

find the smallest square that can fit the word

4^2 = 16, can contain

allocate 4x4 memory and copy it into the memory 

H E L L
O _ W O
R L D D
D D D D 

then transport it, flip it by the diagonal. 

H O R D
E _ L D
L W D D
L O D D

then read it as a string Horde ldlwddlodd

if want to decrypt, reverse it.

H E L L 
O _ W O 
R L D D 
D D D D 

print out Hello World, delete duplicates at the end. 

My Name is Todd, can cancel last d when decrypt. 

lab need 4?

-gcc -o FileDum.exe FileIODemo.c 

FileDump.exe StringNum

fileIO demo:

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char*argv[])
{
  int fileLength = 0;
  FILE* inputFile = NULL;
  
  /*Check for correct arguments*/
  if(argc!=2)
  {
    /*wrong number of input, exit program*/
    return 1;
  }
  
  /*Correct number of arguments, time to process*/
  
  inputFile = fopen(argv[1],"r");
  if(inputFile == NULL)
  {
    /*failed to open*/
    printf("Under to open file : %s\n", arve[1]);
    return 1;
  }
  /* We have the file! Time to read */
  fseek(inputFile, 0, SEEK_END); // go to end of file */
  fileLength = ftell(inputFile); // Give us index at current location
  
  fseek(inputFile, 0, SEEK_SET); //Go to Start of file
  printf("Length of file is %i bytes\n", fileLength);
  
  /*Proceed to dump out the file*/
  do
  {
    /*Print out character by character*/
    printf("%c", fgetc(inputFile));
    
  }while(!feof(inputFile)); // loop while not end of file (eof). 
  
  /*Everything successful,/ 
  
  return 0;
}  
*/

*********************************Lecture 8**************************************
Pointers & Arrays

Array name as Pointer

int a[10]; // array store in stack 
sizeof(a) // 40 
*a = 7; // a[0] = 7 *(a + 0) = 7
foo(a);

int* aPtr = a; // pointers store in heap
sizeof(aPtr) //4

void Foo (int input[]) //input[] is an array, it will become int* input 
{
  sizeof(input) // sizeof(a) = 4. 
  *input = 10;  
}

void Foo (int input)  //pass by value 
{
  sizeof(input)
}

int main (int arc, char** argc[]) //online version 

int main(int argc, char* argv[])  // normal version
{
  
}

int a[4][3] //multi dimensional array [y][x]
int** a // Wrong

will become int*

0|1|2|    // 4 rows 3 columns 
3|4|5|
6|7|8|
9|10|11|

will degrade down to a pointer 

A
_
0
1
2
3
4
5
6
7
8
9
10
11

a[2][2]; = *(aPtr+(2*3+2))

int** aPPtr = malloc (sizeof(int*)*4)
aPPtr[0] = malloc(sizeof(int)*5);
apptr[1] = malloc(sizeof(int)*3);
apptr[2] = malloc(sizeof(int)*4);
apptr[3] = malloc(sizeof(int)*1);

aPPtr[2][2]; 

 | | | | |  //visual representation , x is empty memory. Known as jagged array 
 | | | x x
 | | | | x
 | x x x x 

aPPtr[2][2] = *(*(aPPtr+2)+2);

int b[10];
int* bptr;
bptr = b;
b[0]; 
(*bptr + 0)

Function Pointers/ Pointer to Functions:

Actual Format:

<Return type> (*name)(Inputs..)

double integrate(FP, double a, double b);

FP is double(*f)(double)

double integrate(double (*f)(double), double a, double b);

typedef it to make it simpler

integrate(sin, 1.0, 3.0);

y = (*f)(a);

void (*pf)(int); //Function ptr variable pf 

void PrintNum(int);

pf = printNum;// = equals to assignment of function ptr vaiable

(*pf)(10)

void (*pf[])(int) // can create an array using function pointers, just add []

void DoNothing(){}

function pointers allowed in c++

C++:

Bird -> fly 
Crow -> Bird // crow will get fly function
Chicken ->Bird ->Fly (Do nothing) // chicken is a bird but cannot fly 

Bird* myBird[2];
myBird[0] = new Crow();
myBird[1] = new chicken();

myBird[0]->fly();
myBird[1]->fly();

is called polymophism and it came from C

vTable (cs225) from pointers to function 

0 is bird, 1 is crow, 2 is chicken 
int type; // can implement c style polymophism 

void BirdFly (int type)
{
  switch(type)
  {
    case 0: birdfly(); break;
    case 1: Birdfly(); break;
    case 2: Chickenfly(); break; 
  }
}

Array of function pointers based on type size 
void(*BirdFly[4])(void)

BirdFly |BirdFly |ChickenFly| Penguin |

(*BirdFly[enemy[i]])();

******************************Lecture 9*****************************************
Complex Data Types:
- Structures
- Unions
- Enumurations 

Structure Variables:

int array[10]; 
int/short array[5,3] // cannot mix 

Structures:
- Can contain different types of members
- Can be used with arrays
- Members can have unique names

Struct StudentRecord  //Declaration
{
  char* name;
  int age;
  int admin_num;
};  // Must have semi-colon; 

struct StudentRecord[100]; 

studentRecord.name; //to get name 

have to write the word struct everywhere we go in C. 
C++ dont need. 

Struct StudentRecord[100] //Must write struct 

Struct  // very rare, not recommended to write 
{
  int x;            //4
  char buffer[32];  //32.   if char buffer[30], total size is still 40. Padding
  int y;            //4
}pos1,pos2, *pPos; //Declared 2 variables (pos1, pos2) of "this" struct type (unnamed struct)

C and C++ allows unnamed structs

|4|      32     |4| //total size 40 
4|4|4|4|4|4|4|4|4|4|

- only char no need to padd. 
- order of struct matters

struct
{
  int x;    //0-3
  char buffer[30]; //4-33
  double y;   //40-48    //padd from 33-40
}pos1, pos2, *pPos;

sizeof(pos1); //get the size including the padding;

- biggest size at the top, smallest size at the btm. 
- Odd number for char, confirm will have padd.  

pos1 = {10, "ABC", 20}; // x, buffer, y (Must follow sequence declared!!!)

-Can initialise like an array. 

Operations: Basic access -> pos1.x = 60.

(*pPos).x = 60; // is the same, for pointers 
pPos->x = 60; // is the same , for pointers

Struct POS 
{
  int x;
  char buffer[32];
  int y;
}pos1, pos2;

struct POS* pPos;
pPos = &pos1. 

addressof(&) with member access(.) has lower precedence. 
&pos1.y // . is faster // need put brackets. 

1. Get y from pos1. 
2. Return address of pos y. 

Assignment = pos1 = pos2. // must be same type. //called Shallow Copy. 
//Shallow copy have 2 duplicate copies, but if struct variable has char*buffer,
//address of will be copied from pos1 to pos2, pos2 = pos1.
//if one is changed, another one will be affected.  

pos1 = {10, malloc(sizeof(char)*4),20};
pos1.buf = "ABC";

//Deep copy is 2 duplicate copies. Wont affect one another. 

Struct POS
{
  int x;
  char* buf;
  int y;
};

Struct POS pos1, pos2, *pPos; //tagging 

- Can return struct type in a function. It will be a shallow copy. 
- Pass in pointer to a struct itself. 
- Return a pointer to a malloc struct

e.g. return pos1; 

Nested Structs:

Struct POS
{
  int x;
  char buf[32];
  int y;
  struct StudentRecord record
};

pos[0].record.age



Lab 5:
AddToEnd 
Node* listhead which is NULL 0x100

1. allocate a node 0x200
 allocate to a value 
 Next will be null

pointlisthead to node value but next value is null. because is linked by pointers

2. Allocate another node (0x400)

allocate to value, 2. 
next is NULL

Finding the last node and point to the next node. 

3. Node (0x800)

value = 3;
Next = NULL; 


AdToFront:

1. find the first node. copy first node. 

Node(0x1800)
value = 4; next = null will point to node 1
Node head will point to node 4 and node 4 will point to node 1,2,3

repeat the order. will become 654123

InsertAt:

node 6 is array[0]

insert 7 at 0 is equal adding 7 to the front. 

node 7 (0x2100)

value 7, next point to 6. Node head point to 7

76584123


Reverse: 

Node


freelist:

Node* listhead will become NULL. All gone. 

Test in vs:
open visual studio 2019 or 2017

new project. empty c++ project. call it, cs120_list_lab5. 

right click and start adding files. 

add new item, add list.c 

open folder in explorer. copy paste and drag into folder. compile.

first step: compile. Control shift b or build solution. 

unresoled external symbol means havent define function. Linker error 
code is okay, but book not filled. 

copy paste .h to list.c

#include "list.h"
#include <stdlib.h>
#include <stdio.h>


convert all semi colons, into brackets. make it do nothing. 

count need return 0. 

then build and will succeed. 

if not, go to driver add system("pause"); //to stop the console from closing 


debug code: 

segmentation fault. do a breakpoint. 

debug step into. f11.  check one by one step. 


printlist:

void PrintList(struct Node* list)
{
  //Declare variable to iterate through the list*/
  struct Node* curr = list;
  
  /*Check if the list is empty*/
  if(list == NULL)
  {    
    printf("List is Empty");
    return;
  }
  /*Loop while current is not NULL*/
  while(curr!=NULL)
  {
    /*Print the current value*/
    printf("%i->", curr->value);
    /*Update current to next*/
    curr = curr->next; 
  }
  /*Print NULL to showcase end point*/
  printf("NULL");
}

list is emmpty
List is empty


void AddToEnd(struct Node** ppList, int value)
{
  /*Temp pointer to store new node*/
  struct Node* newNode = NULL;
  
  /*Serves as our runner to find the node*/
  struct Node* curr = NULL;
  /*Assign the values to the new Node*/
  
  /*Check ppList is valid*/
  if(ppList == NULL)
  {
    /*Invalid, Return dont do anything*/
    return; 
  }
  /*Find the location to add*/ /*Special Case: Head is empty*/
  if(*ppList == NULL)
  {
  /*Allocate the Node memory via malloc*/
    newNode = (struct Node*)malloc(sizeof(struct Node));
    
    if(newNode == NULL)
      return; 
    
    (*ppList)->
    
    (*(*ppList)). 
    
    newNode->value = value; 
    newNode -> next = NULL;
    *ppList = newNode;
    
  }
  
}


output: 1->NULL

AddToFront;
dont need runner, dont need loop, no need special case, 

newNode-> = *ppList 
**ppList = newNode 

********************************Lecture 10**************************************
Structs / Unions / Enums

struct TAG{
.
.
.
}var1;

Unions: 
- Difference vs struct: Memory layout!

Union
{
  int i[2];
  double d;
}u;

|4|4|8| memory layout for struct 

|i[0]|i[1]|
|    d    |   choose 1 only. (Unions)

Union always choose the larger variable as the size. 

Union
{
  int i;
  double d;
}u;

|i|blank| 4
|   d   | 8 choose 1 only. 

Initialisation: 

U = {0};
Always use first & need {};

Use Cases:
- Same base but different representation
- Save space in "choice-of-data" situations

Union EntityData
{
  struct PlayerData pData;
  struct EnemyData eData;
}eArray[100];

struct PlayerData //8 bytes
{
  int pHealth;
  int pLvl;
};

struct EnemyData //12 bytes
{
  int eHealth;
  int eAir;
  Int eStats;
};

int clr = 0xABCDEFFF

AB = R
CD = G
EF = B 
FF = A 

1 byte = 8 bits

clr.r = 0x7F;
clr.g = 0x00;
.
.
clr[0] = 0.7F:
clr[1] = 0x00; 

Union Colour
{
  int clr;
  char clr[4];
  struct
  {
    char r;
    char g;
    char b;
    char a;
  }comp;
};

Union memory layout: 
|value                      |
|clr[0]|clr[1]|clr[2]|clr[3]|
|r     |g     |b     |a     |

union Colour k;
k.value = 0;
k.com.r = 0xFF;
k.comp.a = 0xFF:

for (i=0; i<4; ++i)
printf("%i", k.clr[i]); 

union Colour k;
k.value = 0;
k.comp.r = 1;
k.comp.g = 2;
k.comp.b = 3;
k.comp.a = 4;

printf("%i", k.value); 

Union Vec3
{
  float v[3];
  struct
  {
    float i; 
    float j;
    float k;
  }value;
};

structs   //12 bytes 
{
  int type:
  union   // 8 bYtes
  {
    int i;
    double d;
    char c[8];
  }u;
}dataArray[N]; 

Enumerations: 
- Hard coded Numbers in code not a good practice
- Idea! Give labels to numbers

enum BOOL
{
  false,    // equals to 0
  true      // previous + 1 which is equal to 1 
};

#define TRUE 1 //Bad practise
#define FALSE 0 //Bade Practise

if(k == TRUE) // #define will replace true with 1 

BOOL isAlive = TRUE; // cannot initalise with BOOL, BOOL is not a type 

but for enum, BOOL is now a type. 

enum BIRD_TYPES
{
  INVALID,  //DoNothing()
  CROW,     //Fly()
  CHICKEN,  //DontFly()
  PEACOCK,    
  SPARROW,
  BIRD_TYPE_COUNT
};

Array of FunctionPtr[BIRD_TYPE_COUNT]

if add dodo flamingo, the enum will auto count the BIRD_TYPE_COUNT for u. 

enum Dept
{
  ENGLISH = 10,   //can assign values to enums 
  HUMANITIES      // HUMANITIES = 11 
  CS = 20,
  CE              // CE = 21
  CV              //CV = 22
  GAM = 10,
  PRJ             //PRJ = 11
  ART = 50
};

*******************************Lecture 11***************************************
Quiz 2: 

char buf[4]
char* pbuf [4]

for(i=0; i<4;++i)
  pBuf[i] = & buf[i];

char** curr = & pBuf[0];
*curr+=2;
curr ++;
*curr -=1;
curr++;
curr -=1; 

for (i=0; i<4; ++i)
  printf("%c", *pBuf[i]); 


Functions, Stack & Run-time Environment:
Declarations: 

Functions -> Essentially smaller programs to solve a bigger one. 

Function contains Instructions 

Text  | Code |  //main inside 
Heap  |      |
Stack |      |

void Foo(int* p)
{
  (*p)++;
  return; 
}

Foo is a pointer that will point to the first instruction in the function 

return-type name (parameters/input)
{
  declarations
  statements
}

struct studentDatabase 

Foo (int input) //pass by copy or pass by value 
{
  input++;
}

void Foo(int*input) //pass by copy but local variable, input has own address
{
  input++;
  (*input)++;
}

int k = 10; 
foo(k);
printf("%i", k) //printf 10.

foo(&k);
printf("%i", k); 


Declarations:

Header(.h)

void ListInsert(...);
void StringLength(const char*); 
void Chicken(); 

1. Keep definitions in .c, declarations in .h. 
2. Always try to avoid includes in .h 

struct Node* CreateNode (int value)

int* Foo()
{
  int k = 10; 
  return &k; 
}

*********************************Lecture 12*************************************
int Fib(int index)
{
  static int callCounter;
  printf("Fib %i Called! Total = %i",fib(i),callCounter);
  if(index<=2)  //Termination Clause
    return 1;
  
  return Fib(index-1)+Fib(index-2); //Recursion Clause  
}

int main(void)
{
  int i = 0;
  for(i=0;i<10;++i)
  {
    printf("Fib %i = %i\n",i,fib(i));
  }
}

Fib(9); 

local variables are stored on a stack 

Lab 6:

if do wrongly, will get this input. 

when putting in a list, should do duplicate. 

if free buffer will crash, cannot free stack variables. 

duplicate strings for every single node. 

for stringcopy:
common mistake is to create note then do shallow copy. 
if free, will be corrupted. 

get complete copy of original. 

1st, 2nd, fourth, fifth, incorrect mean i do wrong. 


can write own local version and free

int allocateCount = 0; 
int*leaker = (int*)AllocateMemory(sizeof(int));
DeallocateMemory(leaker); 

void* AllocateMemory(unsigned int size)
{
  void* result = malloc(size); 
  ++allocateCount; 
  return result; 
} 

void DeallocateMemory(void*target)
{
  free(target);
  --allocateCount; 
}  

void PrintMemoryLeakCheck()
{
  if(allocateCount!=0)
  {
    printf("WARNING LEAKING MEMORY! %I instances.\n", allocateCount);
  }
}

int main(void)
{
  PrintMemoryLeakCheck();
  return 0;
};


scrcpy(v1.12.1)
mouse Recorder 

SG Game Dev

sggamedev.wordpress.com


#if 0 

#endif 

Preprocessor:
- is a piece of software 
- Edits c Programs/code prior to compilation 
- Powerful but potential source of bug (SSR diffficulty ones).

How it works:
preprocessory directives (Begins with #)
- #include, #define... wont be see by any pre compiler 
- Does not know C language specifications
- 4 Types:
  1. Macro definition: #define, #undef, #, ##
  2. File inclusion: #include
  3. Conditional Compilation: #if, #ifndef, #ifdef, #elif, #else, #elseif, defined(x)
  4. Misc: #error, #line, #pragma 
  
  #define LOL if(abc)\
  {
    //LOL 
  }
  
  Macro definitions
  - #define identifier replacement-list
    - Does a substitution of identifier w/ replacement-list
    - Warning: Do not put any = or ; into replacement-list
    
    #define N 10
    
    int k = 10;
    int arr[10;]; //Wrong
    
    Basic purpose: Give names to const values
    Advanced users: Modify C syntax )eg. #define BEGIN{ #define END}
    
    #define NODE struct Node 
    #define NODE*
    
Parameterized Macros / Function-Like Macros 
- #define identifier (x, x, x, x, x) replacement-list
- No spaces between identifier & (

#define MAX (x,y) ((x),(y)? (x):(y))
#define IS_EVEN(N) ((N) % 2 == 0)
MAX(4,8) -> (4>8? 4:8)

int k = MAX(4,8);
k = 8;

#define TOUPPER(C) ('a'<=(c) && (c)<='z'?(c)-'z'+'A':(c))
#define MUL(x,n)

MAX(++i, j) -> ((++i)>(j)? (++i):(j))

avoid side effects in Macro functions // ++i 

- Stringization #
  -Convers macro argument into string 
  
#define PRINT_INT(N) printf(#n "=%d\n",n)

PRINT_INT(i/j); means printf("i/j = %d\n", i/j); 

Token-Pasting: ##
- Combines tokens 

#define VAR(n) myVar##n 

VAR(1) -> myVar1

#define GENERIC_MAX(TYPE)\

type type##max (typex, type y) {return x>y?x:y;}

GENERIC_MAX(float)
- Float float_max (float x, float y) {return x>y?x:y;}

Lab 7:

Data Driving

CSV = Comma Seperated Values

row 0, 

cannot use comma for text based game. use tab instead of commas

2 things to test. 

read csv file function: 
should check if provided data file do they already contain data
should free before u start reading
dataFile.row == NULL and dataFile.size = 0; 

GetEntry function:
Need to support blank space. an array of blank string with a 0 inside. 

atoi to convert string into number 

***********************************Lecture 13***********************************
The Preprocessor (cont)

General Properties of Macros
- Macro replacement list may contain other macros
  eg. #define PI 3.142..
  e.g. define TWO_PI (2*PI)

- Replacement is based off tokens, not words
  #define SIZE 1024

- Remains in effect until end of file 
- Macros cannot be defined twice unless identical to old one 
  e.g. #undef to remove macros
  
Macro Parentesis Rules
- If it contains operators, always put (brackets) around it. 
  e.g. #define TWO_PI(2*PI)

- If parameters, put (bracket) around. e.g. #define SCALE(x) ((x)*10)

  j = SCALE(i+1*10); //Wrong way 
  j = SCALE((i+1)*10); //Correct way 
  
Long Macros
- Can chain via { } (compound statements)
  #define ECHO(s) {gets(s); puts(s);}
   
  if (echo_flag) ECHO(str); else gets(str); //Wrong 

-Chan via Comma   
  #define ECHO(s) (gets(s),puts(s)) 
    
- Wrapping w/ do-while 
  e.g. #define ECHO(s) 
  do {gets(s);put(s);} while(o)

__LINE__
__FILE__
__DATE__
__TIME__
__STDC__

Conditional Compilation 
- #if const-expr
- #endif 

e.g. #if DEBUG
    //DEBUG CODE 
    #endif 
    
- defined (x) operator -> Checks if marcro definition exists () are optional 
  e.g. #if defined DEBUG
  
  Alternative: 
  #ifdef & #ifndef
  #elif & #else (chain expressions)
  
  #if defined (WIN32)
  .......
  #elif define(MAC_OS)
  .......
  #elif define(LINUX)
  .......
  #else
  ....... //unsupported OS 
  #endif
  
#ifndef CSV_PARSER_h
#defined CSV_PARSER_h

//CSV Paser stuff

#endif //CSV_PARSER_H

Misc Directives
-#error Message 

#line name
#line n "File"

#pragma tokens
e.g. #pragma once 

******************************Lecture 14**************************************** 
Standard C Library

<stdarg.h> ->Variable Arguments
<type> va_arg(va_list ap, type);

void va_start(va_list ap, paromN);
void va_end(va_list ap);

va_list(type)

max_int(3, 10, 30, 20);
int max_int(int n, ...) //... is a legal index
{
  va_list ap; /*our variable list*/
  int i, current, largest;
  va_start(ap, n); //load up ap, start after variable n
  largest = va_arg(ap, int);  //Access the first value in ap, return as int*
  
  for(i=1;i<n;++i)
  {
    current = va_arg(op,int);
    if(current >largest)
      largest = current; 
  }
  
  va_end(ap); //clean up ap(MUST DO);
  return largest:
}

Lab 9: Memory Manager

char* is a void pointer 

all pointers are equal. just numbers that point to a memory location. 

Number has no difference

Malloc one big chunk of memory to char* allocatedMemory

char* isActive is array of bits

1 byte is 8 bits 

first bit is a 1, allocate memory then make it 0 . use bitwise

once allocated, turn bit from 1 to 0 so that others cannot use



unit size* count should have same size of memory

seekerIndex is where u left off. start from 0 scan upwards. Can stor last place
we read then continue where we left off. 


if all int, all must be same size. 

active bits is the char*, smallest is bit which is char so count will be round of to nearest number of 8. 
if 12, round of to 16. 

Allocate is the hardest function 

update bits accordingly. 

Deallocate, 0 out the memset location 

Both active bits and zero out the memory function 

with error check is 8 lines 

char index array[255]

*******************************Lecture 15***************************************
Stardard C library

<Stdlib.h> : Numeric Conversions (atoi, atof, atol, strtod, strtol, strtoul)

"123" ascii value '1' ascii value '2' ascii value '3' in an array then \0 

Pseudo-random sequence generation (int rand(void)) & void srand (unsigned int seed) and RAND_MAX macro 

srand(0);       //wrong 
result = rad(); // wrong

Communication w/ Environment (OS) - void abort (void) //Force close, doesnt erform atexit actions

atexit (void(*func)(void)); // Register function to be called upon closing. 

void exit (int status);

*getenu (const char* name); 

int system (const char* name); 


<Stdlib.h>

-Searching and sorting function 

  - void* bsearch(const void* key, const void* base, size_t nmemb, size_t size, int (*compar)(const void*, const void*))
  - Search in binary-split fashion a sorted array 
  
  -void qsort(void* base, size_t nmemb, size_t size, int(*compar)(const void*, const void*))
  
Type Erasure 

- Test Arithmetic (abs, labs, llabs, diu, ldiv, lldiv)

<time.h> : 3 data storing types for time 

- clock_t -> time alue in clock ticks (duration) 
- time_t -> calendar time (date & time)
- struct tm -> "Broken down" time in parts 
  - tm_ -> sec, min, hour, mday, mon, wday, yday, isdest
  
Time Manipulation Functions 

clock_t clock(void); //Gets the current clock value(CLOCKS_PER_SEC)

double difftime(time_t, time1, time_t, time2);

time_t time(time_t* timer); 
  - returns number of seconds since 1st Jan 1970

time(& resultTime);
resultTime = time(NULL); 

-Time Conversion Functions 

char* asctime(const struct tm* timeptr); 
char* ctime(const time_t* time);
size_t strftime(char*s, size_t maxsize, const char* format, const struct tm* timepptr); 

struct tm* gmtime (const time_t*timer); //UTC
struct tm* localtime (const time_t* timer); // Local time 

********************************Lecture 16**************************************
<assert.h>

void assert (scalar expression); 
Does nothing If non-zero results (TRUE)
- Abort IF zero result (False)

int a[10];
int i = ?;
assert(0<=i && i<10);
a[i]; 

Can be disabled via NDEBUG
assert((p=malloc(n))!=NULL);


<errno.h>

int errno; // int variable in errno.h

errno = 0; // responsibility of clearing by user(Library functions) set to 0;
y=sqrt(x);

if(errno!=0) //" " testing by user 
{
  //error
}

User marcos to define error code 
- EPOM (Domain error: eg. sqrt(-ve);)
- ERANGE(Rangeerror:e.g. exp(1000)); // results too big

stdio.h ->void perror(const char*s);
string.h -> char* strerror (int errnum);

<signal.h> 

- Runtime error 
- Events outside program 
  - OS has need to notify program of special circumstances
  
Common signal macros -> SIGABRT (Abromal termination)
SIGFPE (Arithmetic operation error)
SIGILL(Invalid instruction)
SIGINT(Interupt)
SIGSEGV(Invalid storage access)
SIGTERM (termination request)

void(*signal(int sign, void(*func)(int)))(int); // returns void (*func)(int);
e.g. orighandler = singal( SIGINT, handler); // installs a handler for SIGINT 

//resumes normal execution after handler is done 

Predefined Signal Handlers 
- SIG_DEL (Default (terminate))
- SIG_IGN (Ignore)

int raise(int sig); 

<setjmp.h>

int setjmp(jmp_huf env);
void longjmp(jmp_buf env, int val);
jmp_buf variable that stores context @point of execution 

jmp_buf env;

if(stnmp(env) == 0 ) //Anyway else. will jump over to saved point: longjmp(env, 1); 
{
  // OK
}
else
{
  //NOT OK
}

************************************Review 2************************************
Bitwise Operator:
int i = 2;

i<<1;  //10 ->100 = 4 i*2
i>>1; //10 ->01 = 1 i/2

i<<2; // 10->1000
i>>2; // 10-> 0001

int i = 5; // 101

i<< 1 // 101 ->1010
i>>1

void function(int i); // no ptr can only be an input 
void func2(int*pi); // ptr, input and output 
void func3(const int* pi); // input

const and * refers to an both input and output parameter
pass by copy, after function wont change anything 

recursive function:

int func()
{
 if(i>=3) return;
  int i = 0; 
  i+= func(i);
  i+= func(i);
}

func(0);

When u call a function, it will create a stack. If return, it will cancel the stack
  
                                                  funct(0)

                     func(1)                                            func(2)
funct(2)                            funct(3)          
funct(3) funct(4)


memset(void* ptr, int val, int size) in stdlib.h

init the val pointed by ptr  ----->size is number of bytes

char* ptr; 
memset(ptr,0,10); // all 10 bytes will set to 0. 

Array, strings:
are all pointers.  | | | | | | 

int a[] = {0}; is same as int *a; 

pointers: 
Stores address
every time we call malloc, ptr = malloc(10* sizeof(int));  // &100
In the heap, it will find a memory space and allocate 10 bytes. 

++ptr; //100+4 = 104
--ptr; //100-4 = 96
ptr++; //
ptr--; 

ptr+10; // rvalue; 
ptr2 = ptr+10; // ptr2 pointing to end of ptr, pt2 = 100+4*10=140

Dangling Pointers: (stackdump)
Pointers pointing to an invalid memory in the heap. 

func()
{
  int i;
  return &i; 
}

func()
{
  int * pi = malloc(10);
  free(pi);
  return pi;
}

Structures:

Can store different types into one type. 

Struct a
{
 int a;
}

Struct P
{
  int* pi;        // sizeof (int*) 8 or 4 bytes depend on 32 or 64 bit
  float fa[4];    // sizeof (fa[4]) = 4*sizeof(float) = 4*4 = 16
  char c;     // sizeof(char) = 1 bytes
                  //padding: 3 bytes
  a sa;            // sizeof(sa) = 4 bytes; 
};

struct p
{
  double pi;      //8
  double fa[4];  //8
  double c;        //8 
  char sa;            //1

sizeof(p) = 40

Enum:

enum day
{
  MON = 0,    //1 
  TUES,          //2
  WED,         
  THUR = 10,  //10 
  FRI,              //11
  SAT,             //12
  SUN            //13
);

Struct MP
{
 char* allocatedMemory;
}

Lab 9, 
char* allocatedMemory
char* isActive: for every 8 element in the allocated memory, isit being use


seekerIndex:

used |used |used| not used| used
                            point here

used | not used | used not used | used
           point here


one character is 1 byte. 1 byte got 8 bits 11111111
how to check if first bit is 1 or 0?

bitwise operators <<, >>,| &, ^
use last 3 operators to check bitwise

1<<1 // 10
1<<6; // 0100 0000
               1111 1111
              ------------
               0100 0000

use and gate. 

0100 0000
1100 1101 
-----------
1100 1101

1100 1101 ->1101 1101 (xor) gate or or gate

Allocate is hardest. 
do create pool, free, then inactive count, Deallocate

best to use visual studio to do for lab 9. 

debug->options>debugging->general->tick enable address level debuging. 

breakpoint -> debug->window->memory->memory 1. 




char AllocatedMemory is not a char, a pointer pointing to the start of the array

must be 16, 16 bits = 2 bytes 

 | | | | | | | | | | | | | | | | |

struct color[16], 16 is count, struct color is unitsize 

char[2] = |0|0|0|0|0|0|0|0|      |0|0|0|0|0|0|0|0|

char[2] = |1|1|1|1|1|1|1|1|      |1|1|1|1|1|1|1|1|


if |0|1|1|1|1|1|1|1|, first one is used up

if free first one, should turn back to 1 1 1 1 1 1 1 1 

Index = 1, 

#if 0
void DeleteRow(struct CSVData* target, int rowIndex)
{
  int i = 0, j = 0;
  int a = 0;
  
  struct CSVRow* NewRow =NULL;
  if(target == NULL)
    return;
  
  if(rowIndex < 0 || rowIndex >= target->size)
    return;

  NewRow = (struct CSVRow*)malloc(sizeof(struct CSVRow)*(unsigned long)(target->size -1));

  i =0; j=0;
  while(i < target->size)
  {
    if (i==rowIndex)
    {
      i++;
      continue;
    }
    NewRow[j] = target->rowData[i];
   
    
    i++;
    j++;
  }
  for(a=0; a < target->rowData[rowIndex].size;a++)
  {
    free(target->rowData[rowIndex].colData[a]);
  }
 
  free(target->rowData[rowIndex].colData);
  
  free(target->rowData);
  --(target->size);
  target->rowData = NewRow;
}
#endif

************************************Lecture 17**********************************
Input/Output 
Standard C libaray

<stdio.h> 
FILE* fp; 

standard streams: // can be redirected 
-stdin
-stdout
-stderr
 
myProg.exe < input.txt 

lab8.exe > exampleOutput.txt

Text vs Binary:

text mode: Will come out as ASCII (human-readable format)
'1'|'2'|'3'|'4'|'5'|'6'|

binary mode: is in binary (1010101010)

**********************************Lecture 18 ***********************************
Bit to Bit Map:

command Arguments: 

BMPToGray.c 

#define _CRT_SECURE_N0_WARNINGS
#include <stdio.h>
#include <stdlib.h>


const char BMP_FILETYPE_ID[2] = "BM";

struct BMP_HEADER_DATA
{
  char FileType[2];
  unsigned int FileSize; 
  char Reserved[4];
  unsigned int PixelOffsetData;
}

struct BMP_IMAGEINFO_DATA
{
  unsigned int HeaderSize; 
  int ImageWidth;
  unsigned short Planes;
  unsigned short BitsPerPixal; 
  unsigned int Compresion;
  unsigned int ImageSize; 
  int XPixelsPerMeter; 
  int YPixelsPerMeter;
  unsigned int TotalColors;
  unsigned int ImportantColors;
}

struct COLOR
{
  unsigned char R;
  unsigned char G;
  unsigned char B;
};

void ConvertToGrayscale(struct COLOR* target)
{
  int result = target-> R + target-> G + target->B;
  result/=3;
  target->R = result;
  target->G = result;
  target->B = result;
}

/*
Intended use case:
<ProgramName.exe <InputPath> <OutputPath> 
*/

int main(int argc, char* argv[])
{
  /*Variable Declaration*/

  FILE* inputFile = NULL, *outputFile = NULL;
  char fileTypeBuffer[2]; // Store initial 2 bytes here to test magic number
  char paddingArray[3] = {0,0,0}; /*Place to "discard padding data*/
  struct BMP_HEADER_DATA bmpHeader;
  struct BMP_IMAGEINFO_DATA bmpImgInfo;
  struct COLOR* pixelData;
  int allocationSize = 0;
  int pixelCount = 0;
  int i = 0;
  
  /*Input Argument Check*/
  if(argc!=3)
  {
    printf("Invalid usage: <ProgramName>.exe <InputPath> <OutputPath>\n");
    return 1; 
  }
  
  /*Opening of input BMP file*/
  input = fopen(argv[1],"rb");
  
  if(input == NULL)
  {
    printf("ERROR: Failed to open %s\n", argv[1]);
    return 1; 
  }
  
  while(!feof(inputFile))
  {
    printf("%c", fgetc(inputFile));
  }
  
  printf("Size of BMP Header: %i\n");
  
  /*Read and verifyy the BMP magic number to ensure*/
  fread(fileTypeBuffer, sizeof(char), 2, inputFile);
  
  if(fileTypeBuffer[0]!=BMP_FILETYPE_ID[0] || fileTypeBuffer[1]!=BMP_FILETYPE_ID[1])
  {
    printf("Error: %s is not a valid bmp file\n", argv[1]);
    fclose(inputFile);
    return 1;
  }
  else
  {
    printf("BMP File Type ID Verified...\n");
  }
  
  /*Read header and image info data from file*/
  fread(&bmpHeader, sizeof(struct BMP_HEADER_DATA),1, inputFile);
  fread(&bmpIngInfo, sizeof(sruct BMP_IMAGEINFO_DATA), 1, inputFile);
  
  /*Print some basic information abou the file read*/
  printf(File Size: %i bytes\n," bmpHeader.FileSize);
  printf("Image Width: %i\n", bmpImgInfo.ImageWidth);
  printf("Image Height: %i\n", bmpImgInfo.ImageHeight);
  printf("Compression used: %s\n", bmpImgInfo.Compression? "YES":"NO");
  printf("Color Pallet Size: %i\n", bmpImgInfo.TotalColors);
  printf("Bits Per Pixel: %i bites, bmpImgInfo.BitsPerPixal);
  
  /*Checks to verify it is a "basic" RGB BMP file*/
  if(bmpImgInfo.Compression || bmpImgInfo.BitsPerPixal!=24 || bmpImgInfo.TotalColors!=0)
  {
    printf("Error: %s is an unsupported BMP format\n", argc[1]);
    fclose(inputFile);
    return 1;
  }
  
  /*Computte and allocate the pixel data's memory*/
  pixelCount = bmpImgInfo.ImageWidth* bmpImgInfo.ImageHeight;
  allocationSize = sizeof(struct COLOR)*pixelCount;
  printf("Allocating %i bytes to store pixe data...\n", allocationSize);
  pixelData = (struct COLOR*)malloc(allocationSize); 
  
  if(pixcelData == NULL)
  {
    printf("Error: Unable to allocate %i bytes to store pixcel data\n", allocationSize);
    fclose(inputFile);
    return 1; 
  }
  
  /*Calculte the required padding amount*/
  paddingAmt = (bmpImgInfo.ImageWidth*sizeof(struct COLOR))%4;
  paddingAmt = paddingAmt ? 4 - paddingAmt: 0;
  printf("%i Padding Amount\n", paddingAmt);
  
  /*Read all pixcel information line by line, discarding padding*/
  for(i=0;i<bmpImgInfo.ImageHeight;++i)
  {
    fread(&pixelData[i*bmpImgInfo.ImageWidth],sizeof(struct COLOR), bmpImgInfo.ImageWidth, inputFile);
    fseek(inputFile, paddingAmt, SEEK_CUR);
  }
  
  /*Close the input file*/
  printf("Finished reading all pixel data, closing file...\n");
  fclose();
  
  /*Convert all pixcels to gray*/
  printf("Converting all pixels to gray...\n");
  
  for(i=0;i<pixelCount;++i)
  {
    ConvertToGrayscale(&pixelData[i]);
  }
  
  /*Opening the output file to write*/
  outputFile = fopen(argc[2], "wb");
  
  if(outputFile == NULL)
  {
    printf("Error:Unable to write to $s\n", argv[2]);
    free(pixelData);
    return 1; 
  }
  
  /*Write out the magic number, header and image data*/
  fwrite(fileTypeBuffer, sizeof(char), 2, outputFile);
  fwrite(&bmpHeader, sizeof(struct BMP_HEADER_DATA),1,outputFile);
  fwrite(&bmpImgInfo, sizeof(struct BMP_IMAGEINFO_DATA), 1, outputFile);
  
  /*Write out each pixcel scan-line with padding*/
  for(i = 0; i< bmpImgInfo.ImageHeight;++i)
  {
    fwrite(&pixelData[i*bmpImgInfo.ImageWidth].sizeof(struct COLOR), bmpImgInfo.ImageWidth, outputFile);
    fwrite(paddingArray, sizeof(char), paddingAmt, outputFile)
  }
  
  /*Clean up*/
  free(pixelData);

  return 0; 
}

******************************Lecture 19****************************************
BitFlagDemo.c

flag = 0110 0100
zero = 0000 0000


flag & 0000 0000 //
flag | 0000 0000

~ flag 1001 1011 

flag & = (1 <<index(2))
0000 0001
0000 0100

bitwise AND , OR, XOR, NOR 

& | ~ ^




#define_CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/*Bitwise Operators + Bit Flags*/

typedef unsigned int FLAG;
const unsigned short Flag_LIMIT = sizeof(FLAG)*8;

#ifdef _DEBUG
#define FLAG_LIMIT_CHECK(x) do {assert}(x< FLAG_LIMIT);} while(0)
#else
#define_FLAG_LIMIT_CHECK(x) do {} while(0)
#endif


bool CheckFlag(const Flag flag, unsigned short index)
{
  return flag & (1<<index);
}

void SetFlag(FLAG* flag, unsigned short int)
{
  // flag 0110 1011
  //      0000 0100 (Mask) 
  FLAG_LIMIT_CHECK(index);
  *flag |= (1<<index);
}

void ClearFlag(Flag* flag, unsigned short index)
{
  //flag = 0110 1100
  //     = 1111 1011 (MASK)
  
  FLAG_LIMIT_CHECK(index);
  flag &= ~(1<<index);     
}

void ToggleFlag(Flag* flag, unsigned short index)
{
  FLAG_LIMIT_CHECK(index);
  *flag^=(1<<index);
}

enum PLAYER_STATUS
{
  POISONED, 
  BURNING, 
  BLEEDING, 
  FROZEN, 
  SLOWED,
  PLAYER_STATUS_COUNT
};

const char* statusText[PLAYER_STATUS_COUNT] = {"POISONED","BURNING","BLEEDING","FROZEN", "SLOWED"};

void PrintPlayerStatus(FLAG flag)
{
  unsigned short i = 0;
  for (i = 0, i< PLAYER_STATUS_COUNT; ++i)
  {
    if(CheckFlag(flag, i))
    {
      printf("Player is %s\n", statusText[i]);
    }
  }
}

int main(void)
{
  FLAG playerFlags = 0;
  unsigned short i = 0;
  
  /*start of bit flag demo*/
  printf("Player got hit by an arrow in the knee!\n");
  
  SetFlag(&playerFlags, BLEEDING);
  SetFlag(&playerFlags, SLOWED);
  PrintPlayerStatus(playerFlags);
  printf("Player Flag Value: %u\n", playerFlags); //20
  
  // 0001 0100          4(slow) + 16(bleeding)

  printf("Healing player from bleeding\n");
  clearflag(&playerFlags, BLEEDING);
  PrintPlayerStatus(playerFlags);
  
  printf("Flipping all statuses on player \n");
  
  for(i = 0; i< PLAYER_STATUS_COUNT; ++i)
  {
    ToggleFlag(&playerFlags, i); 
  }
  
  PrintPlayerStatus(playerFlags);
  printf("Player Flag Value = %u\n", playerFlags);
  
   for(i = 0; i< PLAYER_STATUS_COUNT; ++i)
  {
    ToggleFlag(&playerFlags, i); 
  }
  
  PrintPlayerStatus(playerFlags);
  printf("Player Flag Value = %u\n", playerFlags);
  
  return 0;
}

--------------------------------------------------------------------------------
xor cipher program 

#define _CRT_SECURE_N0_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// xor  A^0 = A
//      A^A = 0 
//      B ^ (A ^ A) = B ^ 0 = B 

int maind(int argc, char*argv[])
{
  FILE* inputFILE = NULL, * outputFile = NULL;
  char* password = NULL;
  unsigned int passwordLength = 0;
  unsigned int fileLength = 0;
  char* buffer = NULL;
  unsigned int i = 0;
  
  
  /*Basic Input argument check*/
  if(argc!=4)
  {
    printf("Error: Invalid Usage = <ProgramName>.exe <InputPath> <OutputPath> <Password>");
    return 1;
  }
  
  /*Attempt to open file for reading*/
  input = fopen(argc[1], "rb");
  if(inputFile == NULL)
  {
    printf("ERROR: Unable to open %s for reading\n", argv[1]);
    return 1;
  }
  //Figure out the length of the file
  fseek(inputFile, 0 , SEEK_END);
  fileLength = ftell(inputFile)
  fseek(inputFile, 0 , SEEK_SET);
  
  //Allocate memory for the buffer to read the file 
  buffer = malloc(fileLength);
  
  if(buffer == NULL)
  {
    printf("Error: Unable to allocate %i bytes for reading file\n", fileLength);
    fclose(inputFile);
    return 1;
  }
  
  //Read the file to the buffer, then close the file 
  fread(buffer, sizeof(char), fileLength, inputFile);
  
  for(i = 0; i<fileLength;++i)
  {
    printf("%c", buffer[i]);
  }
  
  fclose(inputFile);
  
  //Figure out the password length
  password = argv[3];
  passwordLength = strlen(password);
  
  //Apply XOR cipher on the data
  for(i=0; i<fileLength;++i)
  {
    buffer[i] ^= password[i%passwordLength];
    buffer[i] = 0;
  }

  outputFile = fopen(argc[2], "wb");
  
  if(outputFile = NULL)
  {
    printf("ERROR: Unable to open %s for writing\n", argc[2]);
    free(buffer);
    return 1; 
  }

  fwrite(buffer, sizeof(char), fileLength, outputFile);

  printf("SUCCESS\n");
  fclose(outputFile);
  free(buffer);
}

********************************Lecture 20**************************************
Function pointers:

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_LENGTH 10

void FuncPtrHelloPirint(void)
{
  printf("Function Pointers are Cool!\n");
}

void ApplyOnArray(int* arrayPtr, unsigned int count, void(*func)(int*))
{
  unsigned int i = 0;
  for(i=0, i<count;++i)
  {
    func(&arrayPtr[i]);
  }
}

void QueryOnArray(int* arrayPtr, unsigned int count, void(*func)(int*))
{
  unsigned int i = 0;
  int result = 0;
  
  for(i=0, i<count;++i)
  {
    result += func(&arrayPtr[i]);
  }
  return result; 
}

int GetValue(int* value)
{
  return *value;
}

int IsOdd(int* value)
{
  return *value % 2;
}

int IsEven(int* value)
{
  return *value%2 == 0;
}

voidPrintValue(int* value)
{
  printf("%i", *value);
}

void AssignRandom(int* target)
{
  *target = rand()%100;
}

void PrintOdd(int* value)
{
  if(*value%2)
    printf("%i, *value);
}

void PrintEven(int* value)
{
  if(*value%2 == 0)
    printf("%i, *value);
}

int main(void)
{
  /*Variable Declaration*/
  void(*basicFuncPtr)(void) = FuncPtrHelloPirint; // dont put brackets 
  int arrayOfValues[ARRAY_LENGTH];

  /*seed Random value to the pseudo random number generator*/
  srand((unsigned int)time(NULL));
    
  /*To invoke a function pointer*/
  basicFuncPtr();
  
  printf("Assigning random values to my entire array...\n");
  ApplyOnArray(arrayOfValues, ARRAY_LENGTH, AssignRandom);
  
  printf("Printing out initial random values...\n");
  ApplyOnArray(arrayOfValues, ARRAY_LENGTH, PrintValue);
  printf("\n");
  
  printf("Printing out odd numbers only...\n");
  ApplyOnArray(arrayOfValues, ARRAY_LENGTH, PrintOdd);
  printf("\n");
  
  printf("Printing out even numbers only...\n");
  ApplyOnArray)arrayOfValues, ARRAY_LENGTH, PrintEven);
  printf("\n");
  
  printf("Sum of values = %i\n", QueryOnArray(arrayOfValues, ARRAY_LENGTH, GetValue);
  printf("\n");
  
  printf("Number of odd values = %i\n, QueryOnArray(arrayOfValues, ARRAY_LENGTH,IsOdd));
  printf("Number of odd values = %i\n, QueryOnArray(arrayOfValues, ARRAY_LENGTH,IsEven));
  
  return 0;
}

--------------------------------------------------------------------------------
Pokemon Simulator

#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define POKEMON_SKILL_COUNT 4
#define POKEMON_COUNT 3

struct Pokemon
{
  char Name[32];
  int HP;
  int ATK;
  void(*Skill[POKEMON_SKILL_COUNT])(struct Pokemon*, struct Pokemon*);
};

int QueryOnTrainer(struct Pokemon* trainer, unsigned int pokemonCount, (*func)(struct Pokemon*))
{
  unsigned int i = 0;
  int result = 0;
  
  for(i = 0; i< pokemonCount; ++i)
  {
    result += func(&trainer[i]));
  }
  return result; 
}

int IsActivePokemon(struct Pokemon* target)
{
  return target->HP > 0;
}

struct Pokemon* ChooseRandomPokemon(struct Pokemon* trainer)
{
  unsigned int i = rand()% POKEMON_COUNT;
  while(!isActivePokemon(&trainer[i]))
  {
    i = rand()% POKEMON_COUNT;
  }
  return &trainer[i];
}

void ChooseRandomAtack(struct Pokmon* attacker, struct Pokemon* defender)
{
  int i = rand()% POKEMON_SKILL_COUNT;
  
  while(attacker->Skill[i] == NULL)
  {
    i = rand() % POKEMON_SKILL_COUNT;
  }
  
  attacker->Skill[i](attacker, defender);
  if(!IsActivePokemon(defender))
  {
    printf("%s faints!\n", defender->Name);
  }
}

void Scratch(struct Pokemon* self, struct Pokemon* other)
{
  printf("%d used SCRACTCH on %s\n", self->Name, other->Name);
  other->HP -= sef->ATK;
}

void Psychic(struct Pokemon* self, struct Pokemon* other)
{
  printf("%d used PSYCHIC on %s\n", self->Name, other->Name);
  other->HP -= sef->ATK*2;
}

void Thunder(struct Pokemon* self, struct Pokemon* other)
{
  printf("%d used THUNDER on %s\n", self->Name, other->Name);
  other->HP -= sef->ATK*3;
}

void Hyperbeam(struct Pokemon* self, struct Pokemon* other)
{
  printf("%d used HYPERBEAM on %s\n", self->Name, other->Name);
  other->HP -= sef->ATK*5;
}


void Headache(struct Pokemon* self, struct Pokemon* other)
{
  printf("%d used HEADACHE on %s\n", self->Name, other->Name);
}

void Leech(struct Pokemon* self, struct Pokemon* other)
{
  printf("%d used LEECH on %s\n", self->Name, other->Name);
  other->HP -= sef->ATK*3;
  elf->HP += self->ATK;
}


int main(void)
{
  struct Pokemon trainerA[POKEMON_COUNT] = 
  {
    {"Pikachu",100, 20, Scratch, Thunder},
    {"MissingNo",1,999, Scratch, Thunder, Headache, Leech},
    {"Mewtwo", 300, 100, Psychic},
  };
  
  struct Pokemon trainerB[POKEMON_COUNT] = 
  {
    {"Dragonite",350, 30,Scratch, Hyperbeam},
    {"Gengar",150, 70, Scratch, Psychic},
    {"Psyduck", 120, 10, Headache}
  };
  
  int turn = 0;
  struct Pokemon* = activePlayer = NULL;
  struct Pokemon* defendPlayer = NULL; 
  struct Pokemon* activePokemon = NULL;
  struct Pokemon* defendPokemon = NULL; 
  
  srand((unsigned int)time(NULL));
  
  while(QueryOnTrainer(trainerA, POKEMON_COUNT, IsActivePokemon) && QueryOnTrainer(trainerB, POKEMON_COUNT, IsActivePokemon))
  {
    /*Game Loop*/
    printf("Turn %i: ", turn +1);
    
    // Get active player and defending player
    activePlayer = turn % 2 ? (defendPlayer = trainerA, trainerB): (defendPlayer = trainerB, trainerA);
    
    // Select pokemons to fight for both sides
    activePokemon = ChooseRandomPokemon(activePlayer);
    defendPokemon = ChooseRandomPokemon(defendPlayer);
    
    //Fight!
    ChooseRandomAtack(activePokemon, defendPokemon);
    
    //Advance turn counter
    ++turn;
  }
  
  //Battle is over
  if(QueryOnTrainer(trainerA, POKEMON_COUNT, IsActivePokemon))
  {
    printf("Trainer A is the winner of the battle!\n");
  }
  else
  {
    printf("Trainer B is the winner of the battle!\n");
  }
}

*****************************Lecture 21*****************************************
Software Design patterns


