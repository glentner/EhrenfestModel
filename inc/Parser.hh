//
// Parser.hh
// EhrenfestModel
//
// Header file for the Parser. This objects takes the
// arguments passed to 'main' and interprets them for
// later retrieval by other objects.
//
// Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
// See LICENSE file (GPLv2)
//

#ifndef _PARSER_HH_
#define _PARSER_HH_

#include <vector>

namespace Ehrenfest {

class Parser {

public:

	static Parser* GetInstance( );
	static void Release( );
	~Parser( ) { }

	void Interpret( const int argc, const char *argv[] );

	// retrieval functions, "getters"
	inline int GetVerbosity( ) const {
		return _verbose;
	}
	inline int GetNumThreads( ) const { 
		return _num_threads;
	}
	inline int GetNumTrials( ) const { 
		return _num_trials;
	}
	inline int GetNumParticles( ) const { 
		return _num_particles;
	}
	inline std::string GetFileName( ) const { 
		return _file_name;
	}
	inline unsigned GetSeed( int index ) const {
		return _seed[index];
	}

private:

	static Parser* instance;
	Parser( ) { }

	int _verbose, _num_threads, _num_trials, _num_particles;
	std::vector<unsigned> _seed;
	std::string _file_name;
};

} // namespace Ehrenfest

#endif
