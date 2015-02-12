//
// Model.hh
// EhrenfestModel
//
// Header file for the Model class. This object embodies the
// Ehrenfest model and evolves the system forward in time
// checking for equilibrium and halting at the first
// Poincare cycle.
//
// Copyright (c) Geoffrey Lentner 2015. All Rights Reserved
// See LICENSE file (GPLv2)
//



#ifndef _MODEL_HH_
#define _MODEL_HH_

#include <string>
#include <vector>

namespace Ehrenfest {

class Model {

public:

	Model( ) { }
	Model( const int &num_particles, const int &index );

	~Model( ) { }

	void Evolve( );
	void WriteTo( std::ofstream &out_file );

protected:

	int  Total( );
	bool Poincare( );
	bool Equilibrium( );

	std::vector<int> _state, _history;
	int _size, _index;
	long _equilibrium, _poincare;

};

} // namespace Ehrenfest

#endif
