##Ehrenfest Model

This application performs a simple simulation in the Ehrenfest Model. 
It assumes all particles to be initially in one of the boxes and
moves them at random. The simulation quits when a Poincare cycle has
been measured. It tracks the history of the simulation by registering
every step in a histogram. The time to first equilibrium is also
measured.

**Dependencies:**
The code should compile in most unix environments. It is parallelized
using OpenMP. Therefore, *Clang* will not work out of the box, but must
be rebuilt with OpenMP support, or simply install the GNU compiler.
