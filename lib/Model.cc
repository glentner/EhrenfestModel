//
// Model.cc
// EhrenfestModel
//
// Source file for the Model class. This object embodies the
// Ehrenfest model and evolves the system forward in time
// checking for equilibrium and halting at the first
// Poincare cycle.
//
// Copyright (c) Geoffrey Lentner 2015. All Rights Reserved
// See LICENSE file (GPLv2)
//

#include <iostream>
#include <string>
#include <fstream>

#include <Model.hh>
#include <Parser.hh>
#include <Random.hh>
#include <Exception.hh>

namespace Ehrenfest {

Model::Model( const int &num_particles, const int &index ) {

	// read in size and index
	_size  = num_particles;
	_index = index;

	// initialize vectors.
	_state.assign( _size, 1 ); 
	_history.assign( _size + 1, 0 );

	// set steps to equilibrium as null
	_equilibrium = _poincare = 0;
}

void Model::Evolve( ) {

	// get seed value from parser
	Ehrenfest::Parser *parser = Ehrenfest::Parser::GetInstance( );
	unsigned seed  = parser -> GetSeed( _index );

	// declare random number generator
	Ehrenfest::Random random( seed, _size );

	// initialize count
	int iter = 0;

	// evolve until we have both poincare and equilibrium
	while( !_poincare || !_equilibrium ) {

		// count step 
		iter++;

		// choose particle
		int particle = random.Particle( );

		// move particle (flip switch)
		if ( _state[ particle ] ) _state[ particle ] = 0;
		else                      _state[ particle ] = 1;

		// append history
		_history[ Total( ) ] += 1;

		// check for equilibrium
		if ( Equilibrium( ) && !_equilibrium )
			_equilibrium = iter;

		// check for Poincare cycle
		if ( Poincare( ) && !_poincare )
			_poincare = iter;
	}
}

int Model::Total( ){

	int sum = 0;

	for ( int i = 0; i < _size; i++ )
		sum += _state[i];

	return sum;
}

bool Model::Equilibrium( ) {

	if ( Total( ) == _size / 2 )
		return true;

	else
		return false;
}

bool Model::Poincare( ){

	if ( Total( ) == _size )
		return true;

	else
		return false;
}

void Model::WriteTo( std::ofstream &out_file ) {

	if ( !_equilibrium || !_poincare )
		throw Ehrenfest::ValueError( 
			"Ehrenfest::Model::Evolve( ) was either "
			"never run or failed to complete properly!");

	if ( out_file.is_open( ) ) {

		out_file << _equilibrium << " " << _poincare;

		for ( int i = 0; i <= _size; i++ )
			out_file << " " << _history[i];

		out_file << std::endl;

	// TODO the IOError doesn't catch the open file!
	} else throw Ehrenfest::IOError("Output file is not open!");
	
}

} // namespace Ehrenfest
