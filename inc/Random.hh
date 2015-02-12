//
// Random.hh
// EhrenfestModel
//
// Header file for "Random" object. This class wraps
// the mt19937_64 pseudorandom number generator.
//
// Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
// See LICENSE file (GPLv2)
//

#ifndef _RANDOM_HH_
#define _RANDOM_HH_

#include <random>

namespace Ehrenfest {

class Random {

public:

	Random( ) { }
	Random( const unsigned &seed, const int &size );

	~Random( ) { }

	int Particle( );

private:

    std::mt19937_64 _generator;
    int _size;

};

}

#endif
