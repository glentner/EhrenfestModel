//
// Random.cc
// EhrenfestModel
//
// Source file for "Random" object. This class wraps
// the mt19937_64 pseudorandom number generator.
//
// Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
// See LICENSE file (GPLv2)
//

#include <Random.hh>

namespace Ehrenfest {

Random::Random( const unsigned &seed_value, const int &size ) {

   _size = size;
	_generator.seed( seed_value );
}

int Random::Particle( ) {

	return (int)( (double)( _size ) * _generator( ) /
			_generator.max( ) );
}

} // namespace Ehrenfest
