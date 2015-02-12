//
// Monitor.cc
// EhrenfestModel
//
// Source file for the Monitor class.
// This class contains tools and functions for displaying progress
// statements and monitoring computation time.
//
// Copyright (C) Geoffrey Lentner 2015. All Rights Reserved.
// See Licence file (GPLv2)
//

#include <iostream>
#include <string>
#include <omp.h>

#include <Monitor.hh>

namespace Ehrenfest {

Monitor* Monitor::instance = NULL;
bool     Monitor::started  = false;

Monitor* Monitor::GetInstance( ){

	if ( !instance )
		instance = new Monitor( );

	return instance;
}

void Monitor::Release( ) {

	if ( instance )
		delete instance;

	instance = NULL;
}

void Monitor::Start( double freq, double bar_width ) {

	// start initial clocks
	_start = _last = omp_get_wtime( );

	// keep local values
	_freq      = freq;
	_bar_width = bar_width;

	// flag initialization
	started = true;	
}

void Monitor::Progress( long iter, long max_iter, int threads, 
	long first_iter ){

	// check initialization
	if ( !started ) Start( );

	// elapsed time since last invocation
	_elapsed = omp_get_wtime( ) - _last;

	// set local clock if on first instance	
	if ( iter == first_iter )
		_local_start_time = omp_get_wtime( );

	// percent complete in loop
	double percent = ( (double)(threads) * (double)(iter) / 
			(double)(max_iter) );

	// if appropriate to display progress bar
	if ( _elapsed > _freq || percent == 1. ){

		// reset iteration time
		_last = omp_get_wtime( );

		// number of characters to display
		int bars = (int)( _bar_width * percent );

		// empty progress bar
		std::string progress_bar = "";

		// write bars
		for ( int i = 0; i < bars; i++ )
			progress_bar += "=";

		// add arrow
		progress_bar += ">";

		// add empty space
		for ( int i = _bar_width; i > bars; i-- )
			progress_bar += " ";

		// write bar
		std::cout << "\r \033[K \r [" << progress_bar << "]" << " ";
	
		// display numerical percent	
		if ( iter != max_iter ) 
			std::cout << percent * 100.0 << "\t% ";
		
		else std::cout << "100.0 %\t ";
	
		// display ETC if not first iteration	
		if ( iter > first_iter && iter < max_iter ) {
			double local_elapsed_time = omp_get_wtime( ) - _local_start_time;
			double ave_time = local_elapsed_time / (double)(iter);
			int steps_remaining = max_iter - iter;
			int etc = (int)( (double)(steps_remaining) * ave_time );
			std::cout << "[ETC: " << CompletionTime( etc ) << "]   ";
		}  std::cout.flush( );
	}
}

void Monitor::TotalElapsedTime( ){

	// get elapsed time and initialize higher units
	double seconds = omp_get_wtime( ) - _start; 
	int minutes = 0, hours = 0, days = 0;

	// convert to time structure
	while ( seconds > 86400.0 ) { seconds -= 86400.0; days++; }
	while ( seconds > 3600.0  ) { seconds -= 3600.0; hours++; }
	while ( seconds > 60.0    ) { seconds -= 60.0; minutes++; }

	// print header
	std::cout << 
		" --------------------------------------------------\n"
		" Total Elapsed Time: ";

	// conditional display of higher units
	if ( days    ) std::cout << days    << " d ";
	if ( hours   ) std::cout << hours   << " h ";
	if ( minutes ) std::cout << minutes << " m ";
	
	// display seconds elapsed
	std::cout << seconds << " s \n\n";
}

std::string Monitor::CompletionTime( int local_elapsed_time ) {
	
	time_t     now = time(0) + local_elapsed_time;
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
					    
	strftime(buf, sizeof(buf), "%m-%d %X", &tstruct);
						    
	return buf;
}

} // namespace Ehrenfest
