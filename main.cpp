

#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include "bandDepth.hpp"
#include "GetPot"


#define MASTER 0

using namespace std;
using namespace HPCS;

typedef double Real;
typedef unsigned int UInt;
typedef BandDepthData bandDepthData_Type;
typedef BandDepth bandDepth_Type;

int main( int argc, char * argv[] )
{
  MPI_Init( &argc, &argv );
  
  GetPot command_line( argc, argv );
      
  const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
      
  GetPot dataFile( data_file_name.data() );

  bandDepthData_Type bdData( dataFile, "BDALL" ); 
  
  bandDepth_Type bd( bdData );
  
  bd.computeBDs();
  
  bd.writeBDs();
  
  MPI_Finalize();

  return EXIT_SUCCESS;
  
}