#include "mrilib.h"

#ifdef USE_OMP
#include <omp.h>
#endif

void DR_help(void)
{
   printf(
     "Program 3dDimRed does 'dimensional reduction' on a collection of input 3D+time\n"
     "datasets, and produces as output a smaller collection of 3D+time datasets.\n"
     "The process of dimensional reduction is via the Singular Value Decomposition\n"
     "(SVD), and the goal is to produce (at each voxel) a collection of time series\n"
     "vectors whose linear span captures most of the variability present in the\n"
     "input.  The number of output datasets is specified by the user, via the\n"
     "'-rdim' option.\n"
     "\n"
     "Usage: 3dDimRed [options] inset1 inset2 ...\n"
     "\n"
     "where 'inset1' is the first input 3D+time dataset, 'inset2' is the second\n"
     "input 3D+time dataset, and so forth.  All input datasets must be on the\n"
     "same 3D+time grid, and there must be at least 2 input datasets.\n"
     "\n"
     "Input datasets can actually be 3D+time datasets, or can be 1D files with\n"
     "a single column; 1D files with multiple columns are treated as datasets,\n"
     "where each ROW is a voxel (and so the time dimension is along each row,\n"
     "not down each column).\n"
     "\n"
     "--------\n"
     "OPTIONS:\n"
     "--------\n"
     " -rdim rr        = The number 'rr' is the number of dimensions to use for the\n"
     "                   output = the number of output 3D+time datasets.\n"
     "                   * 'rr' must be more than 0 and less than the number of\n"
     "                     input datasets.\n"
     "                   * If 'rr' is set to 0, or is not given, then no output\n"
     "                     3D+time datasets will be produced!\n"
     "\n"
     " -rprefix pp     = The string 'pp' is the root prefix for the output datasets.\n"
     "                   The first one will get the prefix 'pp_001', etc.\n"
     "                   * If '-rprefix' is not given, then the default root prefix\n"
     "                     is 'dimred'.\n"
     "\n"
     " -sing           = Save the singular values at each voxel into a dataset with\n"
     "                   prefix 'pp_sing', where 'pp' is the root prefix.\n"
     "                   * If this option is not given, the singular values are not\n"
     "                     saved.\n"
     "\n"
     " -polort qq      = Detrend the time series with polynomial basis of order 'qq'\n"
     "                   prior to further processing.\n"
     "                   * You cannot use this option with '-band'!\n"
     "                   * If neither '-polort' nor '-band' is given, then the mean\n"
     "                     of each time series is removed (e.g., '-polort 0').\n"
     "\n"
     " -band fbot ftop = This option specifies to bandpass the time series prior\n"
     "                   to further processing (as in program 3dBandpass).\n"
     "                   * You cannot use this option with '-polort'!\n"
     "\n"
     " -dt dd          = Set time step to 'dd' seconds (for use with '-band').\n"
     "   *OR*            * Usually the time step is taken from the 3D+time dataset\n"
     " -TR dd              header, but '-dt' (or '-TR') lets you over-ride that.\n"
     "                   * If all inputs are 1D files, then this option is needed\n"
     "                     to set the right time step; otherwise, TR is taken as 1.\n"
     "\n"
     " -despike        = Despike each input time series before other processing.\n"
     "                   * Hopefully, you don't need to do this, which is why it\n"
     "                     is optional.\n"
   ) ;
   PRINT_AFNI_OMP_USAGE("3dDimRed",NULL) ;
   PRINT_COMPILE_DATE ; exit(0) ;
}