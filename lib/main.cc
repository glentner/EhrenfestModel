//
// main.cc
// EhrenfestModel
//
// Main source file for the Ehrenfest Model application.
//
// Copyright (c) Geoffrey Lentner 2015, All Rights Reserved
// See LICENSE file (GPLv2)
//

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <omp.h>

#include <Parser.hh>
#include <Model.hh>
#include <Monitor.hh>
#include <Exception.hh>

int main( const int argc, const char *argv[] ) {

	try {
	
		// create display manager immediately
		Ehrenfest::Monitor *display = Ehrenfest::Monitor::GetInstance( );
		display -> Start( );

		// Create the parser instance and interpret arguments
		Ehrenfest::Parser *parser = Ehrenfest::Parser::GetInstance( );
		parser -> Interpret( argc, argv );

		// Retrieve variables from parser
		int num_trials    = parser -> GetNumTrials( );
		int num_particles = parser -> GetNumParticles( );
		int verbose       = parser -> GetVerbosity( );

		// display greeting
		if ( verbose )
			std::cout <<
				"\n EhrenfestModel initialized. Running " << num_trials <<
				" trials of N = " << num_particles << " ... \n";

		// set parallelism for OpenMP
		omp_set_num_threads( parser -> GetNumThreads( ) );

		// declare the models
		Ehrenfest::Model **model = new Ehrenfest::Model *[num_trials];
		for ( int i = 0; i < num_trials; i++ )
			model[i] = new Ehrenfest::Model( num_particles, i );

		// evolve systems
		#pragma omp parallel for shared(model)
		for ( int i = 0; i < num_trials; i++ ){
			
			// progress bar (displayed immediately)
			if ( !omp_get_thread_num( ) && verbose > 1 )
				display -> Progress( i, num_trials, omp_get_num_threads( ) );
		
			model[i] -> Evolve( );
		}

		// complete progress bar
		if( verbose > 1 )
			display -> Progress( num_trials, num_trials );

		// save data to file
		if ( verbose ) {
			std::cout << "\n\n Writing data to \"" << parser -> GetFileName( );
			std::cout << "\" ... ";
		}
		
		std::ofstream out_file( ( parser -> GetFileName( ) ).c_str( ) );
		if ( out_file.is_open( ) ) {

			for ( int i = 0; i < num_trials; i++ )
				model[i] -> WriteTo( out_file );

			out_file.close( );

		} else throw Ehrenfest::IOError("Failed to open output file!");

		// display time elapsed
		if ( verbose ) {
			std::cout << "done\n";
			display -> TotalElapsedTime( );
		}

		// clean up memory
		Ehrenfest::Parser::Release( );
		Ehrenfest::Monitor::Release( );
		delete[] model;

	} catch ( const Ehrenfest::Usage& NoError ) {
		// usage statement was printed in Parser	
		return 0;

	} catch ( const Ehrenfest::IOError& Error ) {
//		if ( out_file.is_open( ) )
//			out_file.close( );
		std::cerr << Error.what( ) << std::endl;
		return -1;
	
	} catch ( const std::exception& Error ) {
		std::cerr << Error.what( ) << std::endl;
		return -1;
	}

	return 0;
}
