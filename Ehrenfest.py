#!/usr/bin/env python
# Copyright (c) Geoffrey Lentner 2015. All Rights Reserved.
# See LICENSE (GPL v2)
"""
usage: Ehrenfest.py @function <data files> [**kwargs]

Visualize data output of EhrenfestModel code.

function: 

	One of three currently available functions to use (i.e., Equilibrium, 
	Poincare, Distribution).

	*Execute "Ehrenfest.py @function [-h, help]" for usage details.

files:

	Include the name(s) of data files as input to the function. Behavior
	changes depending on if there is only a single file name given or 
	multiple; only a single file is allowed for the Distribution function.
	See the usage of each function for more details.

**kwargs:

	Referring to the Python convention, these will be 'keyword' arguments 
	to be passed as options to the function being executed. Every function
	requires however that you at the very least specify the name of the 
	output file (e.g., output=myfigure.pdf).

Complete examples:

	Plot the dependence of N on the Poincare cycle times on a log scale 
	for all results of the simulation which output to a directory 'out':

		./Ehrenfest.py @Poincare out/*.dat outfile=pcplot.png yscale=log

	Plot the relative time spent in each configuration (past history) of 
	simulation results in 'out/EMS_30.dat' using pdflatex:

		./Ehrenfest.py @Distribution out/EMS_30.dat outfile=dist30.pdf \\
				usetex=True marker=r-
"""

from __future__ import print_function
import sys, numpy as np
from matplotlib import pyplot as plot

class ArgumentError(Exception):
	"""
	Derived from Exception
	"""
	pass

def GetData( infile, **kwargs ):
	"""
	Extract the data from the file, infile.
	"""
	try:
		return np.loadtxt( infile )

	except IOError as error:
		print( error )
		sys.exit(1)

def CyclePlot(data, N=None, **kwargs):
	"""
	CyclePlot( data, N=None, **kwargs):

	If N=None, *data* is assumed to represent a single data set representing 
	N particles. The elements are trials of the Ehrenfest model simulation. 
	The function outputs a plot to *outfile* of frequency as a function of 
	steps. If *N* is a list, representing the N particles for the data sets 
	in the list *data*, it outputs a plot of mean steps to the cycle time as 
	a function of N. 

	Arguments: data: 
	               A 1D array or a list of such arrays.

				  N: (default=None)
	               None or list of integer sizes.
	
	Options: resolution ( default = 1000 )
	             Number of points for resampled fixed grid.
				
	         marker:

	         xlabel:

	         ylabel:

	         title:

	         fontsize:

	         outfile:

	         usetex:

	         xscale:

	         yscale:

	"""
	# check argument types
	if type(data) is list:
		for subset in data:
			if type(subset) is not np.float_:
				raise TypeError('Expected numpy.ndarray for list.')
		if not N or len(N) != len(data):
			raise ValueError('N must have length equal to *data*.')
	elif type(data) is not np.ndarray:
		raise TypeError('Expected numpy.ndarray if not list.')

	# set default xlabel and ylabel
	if N:
		xlabel = 'N'
		ylabel = 'steps'
	else:
		xlabel = 'steps'
		ylabel = 'frequency'

	# define allowed kwargs 
	options = {
		'resolution': '1000',
		'marker':      'b-',
		'xlabel':     xlabel,
		'ylabel':     ylabel,
		'title':      '',
		'fontsize':   '14',
		'outfile':    '',
		'usetex':     'True',
		'xscale':     'linear',
		'yscale':     'linear'
	}

	# accept reassignments  
	for keyword in kwargs:
		if keyword not in options:
			raise ArgumentError('{} is not a valid argument!'.format(keyword))
		options[keyword] = kwargs[keyword]
	
	# assign with type correctness
	marker     = options['marker']
	xlabel     = options['xlabel']
	ylabel     = options['ylabel']
	title      = options['title']
	fontsize   = int( options['fontsize'])
	outfile    = options['outfile']
	xscale     = options['xscale']
	yscale     = options['yscale']
	usetex     = True
	if options['usetex'] == 'False':
		usetex = False

	if not N:
		# plot frequency curve vs steps 
		hist, edges = np.histogram(data)
		hist = np.array(hist, dtype=np.float_)
		centers = edges[:-1] + 0.5*( edges[1:] - edges[:-1] )
		x, y = centers, hist / hist.sum( ) 
	
	else:
		# plot mean steps vs N, must sort by N
		mixed = np.array([N, data])
		x, y = list( mixed[:, mixed[0].argsort( ) ] )

	# build plot
	plot.plot( x, y, marker )
	plot.title(title, fontsize=fontsize)
	plot.xlabel(xlabel, fontsize=fontsize, labelpad=10)
	plot.ylabel(ylabel, fontsize=fontsize, labelpad=10)
	plot.xscale(xscale)
	plot.yscale(yscale)

	if usetex:
		# latex type setting 
		plot.rc('text', usetex=True)
		plot.rc('font', family='serif')

	if outfile:
		# must be specified
		plot.savefig(outfile, format=outfile.split('.')[-1] )

def Poincare( data, N=None, **kwargs ):
	"""
	Handle for plotting Poincare cycle times. Derived from CyclePlot.
	"""
	CyclePlot( data, N, **kwargs )

def Equilibrium( data, N=None, **kwargs ):
	"""
	Handle for plotting Equilibrium cycle times. Derived from CyclePlot.
	"""
	CyclePlot( data, N, **kwargs )

def Distribution( data, **kwargs ):
	"""
	Construct plot of history for distribution of particles
	"""
	# available kwarg options 
	options = {
		'title'    : '',
		'xlabel'   : 'distribution (box1:box2)',
		'ylabel'   : 'frequency',
		'marker'   : 'b-',
		'fontsize' : '14',
		'outfile'  : '',
		'usetex'   : 'True',
		'xscale'   : 'linear',
		'yscale'   : 'linear'
	}

	# accept overides from user
	for keyword in kwargs:
		if keyword not in options:
			raise ArgumentError(
				'{} is not a valid argument for Distribution function.'
				''.format(keyword) )
		
		options[keyword] = kwargs[keyword]
	
	# define parameters with correct type
	title    = options['title']
	xlabel   = options['xlabel']
	ylabel   = options['ylabel']
	marker   = options['marker']
	fontsize = int( options['fontsize'])
	outfile  = options['outfile']
	xscale   = options['xscale']
	yscale   = options['yscale']

	if options['usetex'] == 'True':
		usetex = True
	elif options['usetex'] == 'False':
		usetex = False
	else:
		raise TypeError(
			'Distribution function expects type bool for '
			'usetex option.'
		)

	# the user must provide outfile to save the figure
	if not outfile:
		raise ArgumentError(
			'The user must provide the name of the output file '
			'(e.g., "outfile=myplot.pdf").'
		)

	# check *data* type
	if type(data) is not np.ndarray or len( np.shape(data) ) != 2:
		raise TypeError(
			'Distribution function expects 2D numpy.ndarray. See usage '
			'for more details. Given numpy.shape(data)'
			' = {}'.format( np.shape(data) )
		)

	# The sum of many trials avoids bias
	plot.plot( data.sum(axis=0) / data.sum( ), marker )
	plot.title( title, fontsize=fontsize )
	plot.xlabel( xlabel, fontsize=fontsize, labelpad=10 )
	plot.ylabel( ylabel, fontsize=fontsize, labelpad=10 )
	plot.xscale( xscale )
	plot.yscale( yscale )

	if usetex:
		plot.rc('text', usetex=True)
		plot.rc('font', family='serif')

	plot.savefig( outfile, format=outfile.split('.')[-1] )

def Parse( *argv ):
	"""
	Parse sys.argv to seperate args and kwargs
	"""
	args = [ ]
	kwargs = { }
	for item in list( *argv ):
		if type(item) is not str:
			raise TypeError('Parse function only takes string arguments!')
		part = item.split('=')
		if len(part) == 1:
			args.append(part[0])
		elif len(part) == 2:
			kwargs[part[0]] = part[1]
		else:
			raise ArgumentError('Too many "=" in <{}>.'.format(item))

	return args, kwargs

if __name__ == '__main__':

	# display usage
	if len( sys.argv ) == 1:
		print( __doc__ )
		sys.exit(0)

	# available executables ( @functions )
	executables = {
		'Equilibrium':  Equilibrium.__doc__ + CyclePlot.__doc__,
		'Poincare':     Poincare.__doc__   + CyclePlot.__doc__,
		'Distribution': Distribution.__doc__
		}

	# get function name
	function = sys.argv[1].split('@')
	if len(function) != 2 or function[0] != '':
		print('Error in function argument. Execute "Ehrenfest.py" without'
				'arguments to see usage.'
		)
		sys.exit(1)
	elif function[1] not in executables:
		print('Error in function argument. That was not a recognized call'
				' to an available function.'
		)
		sys.exit(1)
	else: function = function[1]

	# display function usage if no arguments
	if len(sys.argv) < 3 or sys.argv[2] == "-h" or sys.argv[2] == "help":
		print( executables[function] )
		sys.exit(0)

	# convert remaining arguments
	args, kwargs = Parse( sys.argv[2:] )

	if function == 'Poincare' or function == 'Equilibrium':
		
		# import data from files
		data = [ GetData(fpath) for fpath in args ]
		
		# extract cycle time column from data sets
		col = 0 if function == 'Equilibrium' else 1
		cp_data = [ subset[:,col] for subset in data ]
		
		# if there are multiple data files (args) plot dependence
		# on 'N', otherwise plot frequency vs steps.
		if len( cp_data ) == 1:
			cp_data = cp_data[0]
			N = None
		else:
			cp_data = [ sample.mean( ) for sample in cp_data ]
			N = [ np.shape(subset[:,2:])[1] - 1 for subset in data ]

		# Choose function for plotting
		Plot = Equilibrium if function == 'Equilibrium' else Poincare
		
		try:
			Plot( cp_data, N, **kwargs )

		except ArgumentError as error:
			print( error )
			sys.exit(1)

	elif function == 'Distribution':

		# Check for single input file argument
		if len(args) > 1:
			raise ArgumentError(
				'Only one data file can be used at a time with '
				'Distribution function. See usage for me details.'
			)
		
		# import data from file 
		data = GetData( args[0] )

		# extract simulation history from data, 
		# summing sufficient number of trials avoids bias
		Distribution( data[:,2:], **kwargs )
