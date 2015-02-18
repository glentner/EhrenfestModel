#!/usr/bin/env python
"""
Visualize data output of EhrenfestModel code.
"""

from __future__ import print_function
from sys import version_info

import matplotlib, numpy as np, os, sys, fnmatch
from matplotlib import pyplot as plot 
from scipy.interpolate import interp1d as interp

matplotlib.rcParams['figure.facecolor'] = 'w'
plot.ion()

class ArgumentError(Exception):
	pass

def GetData( input_file, **kwargs ):
	"""
	Extract the data from the file, input_file.

	Arguments: input_file: <class 'str'>
	              Full path to input file 

	Returns:   equilibrium: <class 'numpy.ndarray'>
	              Array containing number of steps to equilibrium for each 
					  iteration.

	           poincare: <class 'numpy.ndarray'>
				     Array containing number of steps of poincare cycle for each
					  iteration.

	           history: <class 'numpy.ndarray'>
	              2D array with rows for each iteration. Columns number one more
					  than size of N particles. Represents system state history.

	Options: None
	"""

	try:
		data = np.loadtxt( input_file )

	except IOError as err:
		print(err.message)
		print('\n Error loading %' % input_file)

	equilibrium = data[:,0]
	poincare    = data[:,1]
	history     = data[:,2:]
	iterations  = np.shape(data)[0]

	return equilibrium, poincare, history, iterations

def GetFiles( top_level, **kwargs ):
	"""
	Fetch the full path names of data (.dat) files below the 'top_level'
	directory.

	Arguments: top_level: <class 'str'>
	              Name or relative path of top level directory to search
					  under for data files.

	Returns:   file_paths: <class 'list'>
	              List of full paths to data files below top level
	
	Options:   pattern: <class 'str'>
					Default is '*.dat'
	"""

	# dictionary of available parameters and default arguments.
	params = {
		'pattern':'*.dat'		
	}

	# check key word arguments
	for argument in kwargs:
		if argument not in params:
			raise ArgumentError('% is not a recognized parameter!' % argument)
		if type(kwargs[argument]) is not type(params[argument]):
			raise ArgumentError('% expects %' % (argument, type(params[argument])))
		params[argument] = kwargs[argument]

	# walk directory tree and find data files
	file_paths = []
	for dirname, dirnames, filenames in os.walk(top_level):
		for filename in fnmatch.filter(filenames, params['pattern']):
			file_paths.append( os.path.join(dirname, filename) )

	return file_paths

def PoincarePlot(data, **kwargs):
	"""
	Create smooth curve of Poincare cycle times.

	Arguments: data: <class 'numpy.ndarray'> or <class 'list'>
	              A one dimensional array containing the counts for the number 
	              of steps to the poincare cycle. Can also be a list containing
					  such arrays.

	Options:
	"""

	if data is list:
		for i, sample in enumerate(data):
			mean   = sample.sum() / len(sample)
			x      = range(  )
			smooth = interp(x, sample, kind='cubic')
			plot.plot(x, smooth(x))
	else:
		x = range( np.shape( data )[0])
		smooth = interp(x, data, kind='cubic')
		plot.plot(x, smooth(x))

#if __name__ == '__main__':
