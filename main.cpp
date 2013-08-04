

#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include "BandDepth.hpp"
#include "GetPot"


#define MASTER 0

using namespace std;
using namespace HPCS;

typedef double Real;
typedef unsigned int UInt;
typedef BandDepth bandDepth_Type;

int main( int argc, char * argv[] )
{
  MPI_Init( &argc, &argv );
  
  GetPot command_line( argc, argv );
      
  const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
      
  GetPot dataFile( data_file_name.data() );

  bandDepth_Type bd( dataFile, "ECG" );
  
  MPI_Finalize();
  
  
}