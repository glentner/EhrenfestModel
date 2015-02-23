//
// Parser.cc
// EhrenfestModel
//
// Source file for the Parser. This objects takes the
// arguments passed to 'main' and interprets them for
// later retrieval by other objects.
//
// Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
// See LICENSE file (GPLv2)
//

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <random>
#include <omp.h>

#include <Parser.hh>
#include <Exception.hh>

namespace Ehrenfest {

// static pointer for singleton class
Parser* Parser::instance = NULL;

// retrieval of pointer
Parser* Parser::GetInstance( ) {

	if ( !instance ) 
		instance = new Parser( );

	return instance;
}

// destroy the singleton
void Parser::Release( ) {

	if ( instance ) {
		delete instance;
		instance = NULL;
	}
}

// parser the arguments passed from 'main'
void Parser::Interpret( const int argc, const char *argv[] ) {

	//display usage
	if ( argc == 1 ) {
		std::cout << 
		"\n usage: EhrenfestModel --num-particles=<int> [Options]\n\n"
		" A simulation of the Ehrenfest model of diffusion for N particles in"
		" two boxes.\n"
		"\n"
		"    Arguments: --num-particles= <int>\n"
		"                   Number of particles for the simulation (N).\n"
		"\n"
		"    Options:   --num-trials= <int>\n"
		"                   Number of trials to run for the model, default=30.\n"
		"\n"
		"               --num-threads= <int>\n"
		"                   Number of threads for OpenMP, default=1.\n"
		"\n"
		"               --output-file= <char*>\n"
		"                   Name of output file, default=\"EhrenfestModel.dat\".\n"
		"\n"
		"               --set-verbose= <int>\n"
		"                   Verbosity (choose 0, 1, 2), default=2\n\n";

		throw Ehrenfest::Usage( );
	}

	// declare parameters
	std::map<std::string,bool> given;
	std::map<std::string,std::string> argument;

	// default arguments
	argument["--num-particles"] = "0"; // necessarily reset
	argument["--num-threads"]   = "1";
	argument["--set-verbose"]   = "2";
	argument["--output-file"]   = "EhrenfestModel.dat";
	argument["--num-trials"]    = "30";

	// track if argument provided at runtime
	given["--num-particles"] = false;
	given["--num-threads"]   = false;
	given["--num-trials"]    = false;
	given["--output-file"]   = false;
	given["--set-verbose"]   = false;

	// parser input arguments
	for ( int i = 1; i < argc; i++ ){
	
		// string for this argument
		std::string arg( argv[i] );

		// split by '=' sign
		std::size_t pos = arg.find("=");
		if ( pos == std::string::npos )
			throw InputError( "Missing assignment for " + arg + "!");

		// substring containing keyword argument
		std::string keyword = arg.substr(0, pos);

		// check for keyword argument in main map
		if ( argument.find(keyword) == argument.end( ) )
			throw InputError( keyword + " is not a recognized parameter!" );

		// check that we have more slack in string
		if ( pos == arg.length( ) - 1 )
			throw InputError( "No assignment given for " + keyword + "!" );

		// value given by remainder of string
		std::string value = arg.substr( pos + 1, arg.length( ) - 1 );

		// valid assignment
		argument[keyword] = value;
		given[keyword] = true;
	}

	// set output file name 
	_file_name = argument["--output-file"];

	// stringstream used to convert between types
	std::stringstream convert;
	
	// check verbosity
	convert << argument["--set-verbose"];
	if ( !( convert >> _verbose ) || _verbose < 0 || _verbose > 2 )
		throw InputError("verbose takes 0, 1, or 2.");

	// check particle numbers;
	if ( !given["--num-particles"])
		throw InputError("User must provide --num-particles for model!");
	
	convert.clear( );
	convert.str( argument["--num-particles"] );
	if ( !( convert >> _num_particles ) || _num_particles < 2 )
		throw InputError("--num-particles must take integer value > 2.");

	// check num threads
	convert.clear( );
	convert.str( argument["--num-threads"] );
	if ( !( convert >> _num_threads ) || _num_threads < 1 )
		throw InputError("--num-threads must be a positive integer!");
	if ( _num_threads > omp_get_max_threads( ) )
		throw InputError( "OpenMP says you have less than " + 
			argument["--num-threads"] + " available!" );

	// check trial numbers
	convert.clear( );
	convert.str( argument["--num-trials"] );
	if ( !( convert >> _num_trials ) || _num_trials < 1 )
		throw InputError( "--num-trials needs a postive integer value!" );

	// build array of random seeds
	_seed.push_back( time(NULL) );
	for ( int i = 1; i < _num_trials; i++ )
		_seed.push_back( _seed[i-1] + 2 );
}

} // namespace Ehrenfest
