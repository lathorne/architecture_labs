/* 
 * CS:APP Data Lab 
 * 
 * Logan Thorneloe and Hyrum Gunther
 * net ID: lathorne and hygunth 
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
/* Copyright (C) 1991-2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* We do support the IEC 559 math functionality, real and complex.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
// W 2018 Selection  //
///////////////////////
// Level 1
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
	//the minimum two's complement integer is all ones
	return (0x1 << 31);
}
/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {
	//This creates a mask of 01010101... which is all the even bits
  return (((((0x55 << 8) | 0x55) << 8) | 0x55) << 8) | 0x55;
}
// Level 2
/* 
 * anyEvenBit - return 1 if any even-numbered bit in word set to 1
 *   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyEvenBit(int x) {
	//We use a mask of even bits and if it is non zero then there is at least one even bit
	int mask = (((((0x55 << 8) | 0x55) << 8) | 0x55) << 8) | 0x55;
  	return !!(x & mask);
}
/* 
 * oddBits - return word with all odd-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 2
 */
int oddBits(void) {
	//This creates a mask of 1010101... which is all the odd bits
  return (((((0xaa << 8) | 0xaa) << 8) | 0xaa) << 8) | 0xaa;
}
/* 
 * isNotEqual - return 0 if x == y, and 1 otherwise 
 *   Examples: isNotEqual(5,5) = 0, isNotEqual(4,5) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNotEqual(int x, int y) {
	//if x ^ y is all zero then x and y are equal
	//so if x ^ y returns a nonzero number then x and y are not equal
  return !!(x ^ y);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
	//returning the negative of a number can be done by flipping the bits and adding 1
  return ~x + 1;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
	//for this we use the sign bit to tell whether x is positive or negative
	//we shift x by n-1 and depending on whether it is positive or negative we and it with the sign and return
	int sign = x >> 31;
	int res = (~sign & (x >> (n + ~1 + 1))) + (sign & ~((x >> (n + ~1 + 1))));
  	return !res;
}
// Level 3
/*
 * ezThreeFourths - multiplies by 3/4 rounding toward 0,
 *   Should exactly duplicate effect of C expression (x*3/4),
 *   including overflow behavior.
 *   Examples: ezThreeFourths(11) = 8
 *             ezThreeFourths(-9) = -6
 *             ezThreeFourths(1073741824) = -268435456 (overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3
 */
int ezThreeFourths(int x) {
	//multiply by three, grab the sign bit and then divide by four
  	int xby3 = x + x + x;
	int sign = xby3 >> 31;
	return (xby3 + (sign & ((1 << 2) + ~0))) >> 2; 	
}
/* 
 * rempwr2 - Compute x%(2^n), for 0 <= n <= 30
 *   Negative arguments should yield negative remainders
 *   Examples: rempwr2(15,2) = 3, rempwr2(-35,3) = -3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int rempwr2(int x, int n) {
	int sign = (x >> 31); //grab sign
	int new_x = (sign & (~x + 1)) + (~sign & x); //make x positive if not positive
	int divided = (new_x >> n) << n; //divide by 2^n
	int rem = new_x + (~divided + 1); //get the remainder
	return (sign & (~rem + 1)) + (~sign & rem);  //calculate the sign of the remainder based on the sign from earlier
}
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
	//figuring out if x fits the bit pattern of a 0x30 through 0x39
	return !((x>>2) ^ 0x0c) + !((x>>2) ^ 0x0d) + !((x>>1) ^ 0x1c); 
}
// Level 4
/* 
 * tc2sm - Convert from two's complement to sign-magnitude 
 *   where the MSB is the sign bit
 *   You can assume that x > TMin
 *   Example: tc2sm(-5) = 0x80000005.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 */
int tc2sm(int x) {
	//grab sign, shift the sign back to get the sign and all zeroes, add x positive or negative on the end and return
	int sign = x >> 31;  
	int shifted_sign = sign << 31;
	return shifted_sign + ((sign & (~x + 1)) + (~sign & x));
}
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
	//xor the bits of x in groups of 2, 4, 8, 16 in order to get the parity of the bits in the LSB
	int sub_x = (x ^ (x >> 1));
	sub_x = (sub_x ^ (sub_x >> 2));
	sub_x = (sub_x ^ (sub_x >> 4));
        sub_x = (sub_x ^ (sub_x >> 8));
	sub_x = (sub_x ^ (sub_x >> 16));
   	return sub_x & 1;
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  //make masks to use to get the hamming weight at the end of the function
  int firstmask, secondmask, thirdmask;  

  //making a few masks
  firstmask = 0x55 | (0x55 << 8);
  firstmask = firstmask | (firstmask << 16);
  secondmask = 0x33 | (0x33 << 8);
  secondmask = secondmask | (secondmask << 16);
  thirdmask = 0x0f | (0x0f << 8);
  thirdmask = thirdmask | (thirdmask << 16);
    
  //use bit shifting and OR's to set all the bits right of the left most one equal to one
  x = x | (x >> 1);
  x = x | (x >> 2);
  x = x | (x >> 4);
  x = x | (x >> 8);
  x = x | (x >> 16);
  
  //now we need to count the number of one's in the binary representation and that's our number
  //need to use an algorithm to find the Hamming Weight
  //use the masks from earlier to determine the number of one's
  //subtracting one from this will give us the index of the left-most 1, which is our answer
  
  x = x + (~((x >> 1) & firstmask) + 1);             
  x = (x & secondmask) + ((x >> 2) & secondmask); 
  x = (x + (x >> 4)) & thirdmask;        
  x += x >>  8;  
  x += x >> 16;   
  return (x & 0x7f) + ~0;       

}
/*
 * trueThreeFourths - multiplies by 3/4 rounding toward 0,
 *   avoiding errors due to overflow
 *   Examples: trueThreeFourths(11) = 8
 *             trueThreeFourths(-9) = -6
 *             trueThreeFourths(1073741824) = 805306368 (no overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int trueThreeFourths(int x)
{	
	int sign, remainder, offset, y, divide, multiply;
	sign = x >> 31; //grab sign bit
	remainder = x & 0x03; //grab the remainder
	offset = (remainder << 1) + remainder; //multiply remainder by 3
	offset = offset >> 2; //divide by four
        offset = offset + (sign & ~1); //calculate the offset based on the sign
	
	y= !remainder; //y will tell us if we need to add an offset or not
	y = ~((~y) + 1); //if y is zero, no offset will be added
	
	divide = (x + ((sign & (0x03)))) >> 2; //divide by 4		
	multiply = divide + divide + divide; //multiply by three
	return multiply + (offset & y); //add the offset if y determines we need to
		 	
}


/*
 * leftBitCount - returns count of number of consective 1's in
 *     left-hand (most significant) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Rating: 4
 */
int leftBitCount(int x) {
  //determine the left bit count by using the definition of the logarithm
  //this means we can reuse code from our ilog function
  //31 - ilog2(~x) gives us the answer we need
  //this is done below
  int firstmask, secondmask, thirdmask;

  x = ~x;

  //making a few masks
  firstmask = 0x55 | (0x55 << 8);
  firstmask = firstmask | (firstmask << 16);
  secondmask = 0x33 | (0x33 << 8);
  secondmask = secondmask | (secondmask << 16);
  thirdmask = 0x0f | (0x0f << 8);
  thirdmask = thirdmask | (thirdmask << 16);
    
  x = x | (x >> 1);
  x = x | (x >> 2);
  x = x | (x >> 4);
  x = x | (x >> 8);
  x = x | (x >> 16);
  
  //now we need to count the number of one's in the binary representation and that's our number
  //need to use an algorithm to find the Hamming Weight
  
  x = x + (~((x >> 1) & firstmask) + 1);             
  x = (x & secondmask) + ((x >> 2) & secondmask); 
  x = (x + (x >> 4)) & thirdmask;        
  x += x >>  8;  
  x += x >> 16;   
  return 31 + (~((x & 0x7f) + ~0) + 1);  
  
}
// Should be 15 puzzles, 2 level 1 puzzles, 5 level 2 puzzles, 3 level 3 puzzles, 5 level 4 puzzles
