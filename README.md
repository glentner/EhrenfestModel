##Ehrenfest Model

This application performs a simple simulation of the Ehrenfest model of 
diffusion for N particles in two boxes. 

**Dependencies:**
The code is parallelized using OpenMP, so this should be available to 
your compiler. Also, the -std=c++11 flag is used for the random number
generator. The Python module for visualizing the data output by the code
uses Numpy, Matplotlib and will require a latex installation to use the 
'usetex=True' option.
