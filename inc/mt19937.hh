//
// mt19937.hh
// drakeProject
//
// Header file for the "mt19937" class used in the DrakeApp project.
// This class is used to define instances of random number generators
// using the 64-bit mt19937 algorithm developed by Nishimura and 
// Matsumoto in 20004. Their original C code has been pushed into a 
// C++ class. The necessary inclusions are provided at the end of
// this header file.
//
// Copyright (C) Geoffrey Lentner 2014. All rights reserved.
// See LICENCE file. (GPL v2.0)
//
// contact: Geoffrey Lentner, B.S.
//          Graduate Student / Researcher
//          LL15 Natural Science Building
//          Department of Physics & Astronomy
//          University of Louisville
//          Louisville, KY 40292 USA
//
// email:   geoffrey.lentner@louisville.edu
//

#ifndef MT19937_HH
#define MT19937_HH

#define NN       312
#define MM       156
#define MATRIX_A 0xB5026F5AA96619E9ULL
#define UM       0xFFFFFFFF80000000ULL //  most significant 33 bits
#define LM       0x7FFFFFFFULL         // least significant 31 bits

//  M E R S E N N E   T W I S T E R   P R N G   C L A S S   D E C L A R A T I O N    /
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - /
class mt19937 {
    
    public:

        // M E M B E R   V A R I A B L E S * * * * * * * * * * * * * * * * * * * * * /
        unsigned long long mt[NN]; // the array for the state vector
        int mti;                   // == NN+1 means mt[NN] uninitialized
    
        // M E M B E R   F U N C T I O N S * * * * * * * * * * * * * * * * * * * * * /
		mt19937 ( ) { }                                    // default constructor
		mt19937 ( unsigned long long seed = 19650218ULL ); // initialize by single ULL
        mt19937 ( unsigned long long init_key[],
                  unsigned long long key_length );         // initialize by array
       ~mt19937 ( ) { } ;                                  // default destructor
    
        unsigned long long random_int ( void );
        double random_real ( void );
};

#endif

// BELOW IS THE ACKNOWLEDGEMENT OF USE FOR THE ORIGINAL SOURCE OF
// THE 64-BIT MERSENNE TWISTER PSEUDORANDOM NUMBER GENERATOR

/* 
   A C-program for MT19937-64 (2004/9/29 version).
   Coded by Takuji Nishimura and Makoto Matsumoto.

   This is a 64-bit version of Mersenne Twister pseudorandom number
   generator.

   Before using, initialize the state by using init_genrand64(seed)  
   or init_by_array64(init_key, key_length).

   Copyright (C) 2004, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   References:
   T. Nishimura, ``Tables of 64-bit Mersenne Twisters''
     ACM Transactions on Modeling and 
     Computer Simulation 10. (2000) 348--357.
   M. Matsumoto and T. Nishimura,
     ``Mersenne Twister: a 623-dimensionally equidistributed
       uniform pseudorandom number generator''
     ACM Transactions on Modeling and 
     Computer Simulation 8. (Jan. 1998) 3--30.

   Any feedback is very welcome.
   http://www.math.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove spaces)
*/
