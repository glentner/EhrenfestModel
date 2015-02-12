//
// mt19937.cc
// drakeProject
//
// Source file for the "mt19937" class used in the DrakeApp project.
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
// updated: 2014-12-12 09:42:56 EST
//

#include "mt19937.hh"

// M E M B E R   F U N C T I O N   D E F I N I T I O N S * * * * * * * * * * * * * * /
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - /

// C O N S T R U C T   V I A   S I N G L E   U L L   I N T E G E R * * * * * * * * * /
mt19937::mt19937 ( unsigned long long seed ) {
    
    mti = NN + 1;
    
    mt[0] = seed;
    
    for (mti = 1; mti < NN; mti++)
        mt[mti] = (6364136223846793005ULL * (mt[mti-1] ^ (mt[mti-1] >> 62)) + mti);
}

// C O N S T R U C T   V I A   A R R A Y * * * * * * * * * * * * * * * * * * * * * * /
mt19937::mt19937 ( unsigned long long init_key[], unsigned long long key_length) {
    
    unsigned long long i, j, k;
    
    mt[0] = 19650218ULL;
    
    for (mti = 1; mti < NN; mti++) {
        
        mt[mti] = (6364136223846793005ULL * (mt[mti-1] ^ (mt[mti-1] >> 62)) + mti);
    }
    
    i = 1; j = 0;
    
    k = (NN>key_length ? NN : key_length);
    
    for (; k; k--) {
    
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 62)) * 3935559000370003845ULL))
        + init_key[j] + j; /* non linear */
        i++; j++;
        
        if (i >= NN) {
        
            mt[0] = mt[NN-1]; i=1;
        }
        
        if (j >= key_length) {
        
            j=0;
    	}
	}
    
    for (k = NN - 1; k; k--) {
    
		// non linear
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 62)) * 2862933555777941757ULL)) - i; 

        i++;
        
        if (i >= NN) { mt[0] = mt[NN-1]; i = 1; }
    }
    
    mt[0] = 1ULL << 63; /* MSB is 1; assuring non-zero initial array */
}

// generates a random number on [0, 2^64-1]-interval * * * * * * * * * * * * * * * * /
unsigned long long mt19937::random_int( void ) {
    
	int i;
	unsigned long long x;
	unsigned long long mag01[2]={0ULL, MATRIX_A};
    
    if (mti >= NN) { /* generate NN words at one time */
        
        for (i=0;i<NN-MM;i++) {
            x = (mt[i]&UM)|(mt[i+1]&LM);
            mt[i] = mt[i+MM] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
        }
        for (;i<NN-1;i++) {
            x = (mt[i]&UM)|(mt[i+1]&LM);
            mt[i] = mt[i+(MM-NN)] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
        }
        x = (mt[NN-1]&UM)|(mt[0]&LM);
        mt[NN-1] = mt[MM-1] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
        
        mti = 0;
    }
    
    x = mt[mti++];
    
    x ^= (x >> 29) & 0x5555555555555555ULL;
    x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
    x ^= (x << 37) & 0xFFF7EEE000000000ULL;
    x ^= (x >> 43);
    
    return x;
}

// generates a random number on [0,1]-real-interval * * * * * * * * * * * * * *  * * /
double mt19937::random_real ( void ) {
    
    return (random_int() >> 11) * (1.0/9007199254740991.0);
}
