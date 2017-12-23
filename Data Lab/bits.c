/* 
 * CS:APP Data Lab 
 * 
 * Jonathan Chon  UID:104 780 881
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif

int bang(int x) {
	/*
	This code splits the code in half and "ors" both halves together.
	Thus, if there are any 1's, they will end up in the half. Continue
	halfing until only 1 digit left. If there was a 1, it will be in last position.
	*/
	x |= (x >> 16);
	x |= (x >> 8);
	x |= (x >> 4);
	x |= (x >> 2);
	x |= (x >> 1);
	return (~x & 1);
}

int bitCount(int x) {
	/*
		First, I initialized three variables in order to to get every other bit,
		then groups of two bits, then groups of 4 bits. Then, I essentially counted
		the amount of bits in groups of 2, 4, 8, 16, then 32. These were all put in
		the lowest 8 bits. I referenced population count.
	*/
	
	int first = 0x55 + (0x55 << 8) + (0x55 << 16) + (0x55 << 24); //0x55555555
	int second = 0x33 + (0x33 << 8) + (0x33 << 16) + (0x33 << 24);	//0x33333333
	int third = 0x0f + (0x0f << 8) + (0x0f << 16) + (0x0f << 24); 	//0x0f0f0f0f

	x = x + (~((x >> 1) & first) + 1);
	x = (x & second) + ((x >> 2) & second);
	x = (x + (x >> 4)) & third;
	x = x + (x >> 8);
	x = x + (x >> 16);
	return x & 0x7f;
}

int bitOr(int x, int y) {
  /* Using DeMorgan's Law, this is equivalent to x|y */
  return ~(~x & ~y);
}

int bitRepeat(int x, int n) {
	/*
	Use mask to get only the portion we want. Shift by n and paste. copy that and shift
	by 2*n and paste. Continue until 16*n. Used (nShift/32 * nShift%32) to fill in leftover
	space if a nShift was greater than 32. Then, return taking into account special case
	when n = 32.
	*/
   int is32 = n/32; 
   int shift =  n;
   int bitMask = ~(~0 << shift);
   int nVal = x & bitMask;

   is32 = -is32;

   nVal |= nVal << shift;  // n

   shift = (shift * 2);  // n * 2
   shift = shift - (shift/32 * shift%32);  
   nVal |= nVal << shift; 

   shift = (shift * 2);  // n * 4
   shift = shift - (shift/32 * shift%32);  
   nVal |= nVal << shift; 

   shift = (shift * 2);  // n * 8
   shift = shift - (shift/32 * shift%32);  
   nVal |= nVal << shift; 

   shift = (shift * 2);  // n * 16
   shift = shift - (shift/32 * shift%32);  
   nVal |= nVal << shift; 

   return (is32 & x)  | (~is32 & nVal);
}


int fitsBits(int x, int n) {
  /*
  	  Shift x n-1 times so that only the last digit remains. If x is negative,
  	  then the last number should be 1, and 0 if x is positive. Using exclusive
  	  or with a mask of the most significant digit will change it to all 0's if
  	  x does fit in n bits. If not, there will be a 1 somewhere. Thus, using 
  	  bang will give the correct answer, as all zero's will return 1 and any other
  	  number will return 0.
  */
  return !((x >> (n+ (~1 + 1))) ^ (x >> 31));
}

int getByte(int x, int n) {
  	/*
      n << 3 essentially multiplies n by 8, so we shift x until the byte that we
      want is at the very right. using 0xff & makes makes sure that anything to 
      the left of the byte that we want becomes irrelevent, since 0xff is the same
      as 00...0011111111.
  	*/
  return 0x000000ff & (x >> (n << 3));
}

int isLessOrEqual(int x, int y) {
	/*
	compare signs and use the result to check for whether the x is less than y, and
	then use ! ^ to check for equal to. If either is true, return true.
	*/
	int signX = x >> 31;
	int signY = y >> 31;
	int testLess = (!(signX ^ signY) & ((~y+x) >> 31)) | (signX & !signY);
  	return testLess | !(x^y) ;
}

int isPositive(int x) {
  /* 
      x & (1<<31) checks to see if the number is negative, and !x checks to see if 
      the number is zero. These check whether the number is negative or zero, so ! 
      must be used.
  */
  	return !((x & (1 << 31)) | !x);
}

int logicalShift(int x, int n) {
/*
	First, x is shifted normally. Then, to switch the 1's into zero's, I made a 
	mask that was 1 longer than n, removed the extra 1, and then flipped it over
	and used & to change the 1's to 0.
*/	
	return ((x >> n) & ~(((1 << 31) >> n) << 1));
}

int tmin(void) {
  /*  
      Shifts 00...001 left to become 10...000, since in two's complement that is the 
      smallest number
  */
  return 1 << 31;
}
