/**************************
filename OperatorExercise.c
author Howard Sin
email howard.sin@digipen.edu
date created 22 May 2019
Brief Description: A series of questions that covers operators in C
**************************/
#include <stdio.h>

int main(void)
{
  int A = 3, B = 7, C = 13;

  /* Section A : Fill in the output with the correct single line code 
  statement matching the text (Arithmetic and Assignment)*/
  printf("// Section A -------------------------------------\n");
  printf("Given numbers A = %d, B = %d, C = %d\n", A, B, C);
  printf("Add A and B : %d\n", A + B); /*Example*/
  printf("Subtract B from C : %d\n", C-B);
  printf("Multiply A with C : %d\n", A*C);
  printf("Find the whole number of C divide by A : %d\n", C/A);
  printf("Find the remainder of C divide by A : %d\n", C%A);
  printf("Add A and B, multiply that result with C : %d\n", (A+B)*C);
  printf("Add 1 to A (This will update A) : %d\n", ++A);
  printf("Multiply B to C (This will update C) : %d\n", C*=B);
  printf("Add B and C to A (This will update A) then subtract 1 from B and C" 
  "(This will update B and C)  :  %d\n\n", A+= (B-- + C--));

  /* Section B : Fill in the output with the correct single line code 
  statement matching the text (Relational and Logical)*/
  printf("// Section B -------------------------------------\n");
  printf("Given numbers A = %d, B = %d, C = %d\n", A, B, C);
  printf("Is A not zero? (TRUE/FALSE): %s\n", A?"TRUE":"FALSE");
  printf("Is A greater than C? (TRUE/FALSE): %s\n", A>C?"TRUE":"FALSE");
  printf("Is B lesser than A and C? (TRUE/FALSE) : %s\n",
  (B<A && B<C)?"TRUE":"FALSE");
  printf("Is C an odd number? (TRUE/FALSE): %s\n", C%2?"TRUE":"FALSE");
  printf("Do A, B and C have the same value? (TRUE/FALSE) : %s\n"
  , (A==B && B==C)? "TRUE":"FALSE");
  printf("Is there a zero in A, B or C? (YES/NO)" 
  "(0 not allowed in the statement) : %s\n\n", (!A||!B||!C)?"TRUE":"FALSE");

  /* Section C : Fill in the output with the correct single line code 
  statement matching the text (Bitwise)
  Do not update the values in this section, print the decimal value of the 
  result expression.*/
  printf("// Section C -------------------------------------\n");
  printf("Given numbers A = %d, B = %d, C = %d\n", A, B, C);
  printf("Flip all the bits of B : %d\n", ~B);
  printf("Find the common bits of A and C : %d\n", A&C);
  printf("Find the different bits of A and C : %d\n", A^C);
  printf("Shift all the bits in A to the left by 1 : %d\n", A<<1);
  printf("Is the 4th rightmost bit of A a 1? (TRUE/FALSE): %s\n",
  1&(A>>3)?"TRUE":"FALSE");
  
  return 0;
}