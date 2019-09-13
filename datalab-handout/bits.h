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
int tmin();
int test_tmin();
int evenBits();
int test_evenBits();
// Level 2
int anyEvenBit();
int test_anyEvenBit();
int oddBits();
int test_oddBits();
int isNotEqual(int, int);
int test_isNotEqual(int, int);
int negate(int);
int test_negate(int);
int fitsBits(int, int);
int test_fitsBits(int, int);
// Level 3
int ezThreeFourths(int);
int test_ezThreeFourths(int);
int rempwr2(int, int);
int test_rempwr2(int, int);
int isAsciiDigit(int);
int test_isAsciiDigit(int);
// Level 4
int tc2sm(int);
int test_tc2sm(int);
int bitParity(int);
int test_bitParity(int);
int ilog2(int);
int test_ilog2(int);
int trueThreeFourths(int);
int test_trueThreeFourths(int);
int leftBitCount(int);
int test_leftBitCount(int);
// Should be 15 puzzles, 2 level 1 puzzles, 5 level 2 puzzles, 3 level 3 puzzles, 5 level 4 puzzles
