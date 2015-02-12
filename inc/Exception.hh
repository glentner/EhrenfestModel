//
// Exception.hh
// EhrenfestModel
//
// Objects for throwing exceptions
//
// Copyright (c) Geoffrey Lentner. All Rights Reserved
// See LICENCE file (GPLv2)
//

#ifndef _EXCEPTION_HH_
#define _EXCEPTION_HH_

#include <exception>
#include <string>

namespace Ehrenfest {

class BaseException : public std::exception {
	
public:
	
	explicit BaseException( const char* msg ): _msg( msg ) { }
	virtual ~BaseException( ) throw( ) { }
	virtual const char* what( ) const throw( ) { 
		return ( "\n Ehrenfest::" +  _msg ).c_str( ); 
	}

protected:

	std::string _msg;
};

class IOError : public BaseException {
public:
        
	IOError( const std::string& msg ): BaseException(
		("IOError\n --> " + msg + "\n" ).c_str( ) ) { }
};
    
class ValueError : public BaseException {
public:
        
	ValueError( const std::string& msg ): BaseException( 
		( "ValueError\n --> " + msg + "\n" ).c_str( ) ) { }
};

class InputError : public BaseException {
public:

	InputError( const std::string& msg ): BaseException(
		( "InputError\n --> " + msg + "\n" ).c_str( ) ) { }
};

// not an error, but halt execution
class Usage : public BaseException {
public:

	Usage( ): BaseException( "Usage" ) { }
};

}// namespace Ehrenfest 

#endif
