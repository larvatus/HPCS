
#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include "bandDepth.hpp"
#include "bandDepthData.hpp"
#include "factory.hpp"
#include "dataSet.hpp"
#include "GetPot"


#define MASTER 0

using namespace std;
using namespace HPCS;

typedef double Real;
typedef unsigned int UInt;
typedef BandDepthData bandDepthData_Type;
typedef BDFactory< All > bdFactory_Type;
typedef boost::shared_ptr< BandDepthBase > bdBasePtr_Type; 

int main( int argc, char * argv[] )
{

MPI_Init( &argc, &argv );
  
   GetPot command_line( argc, argv );
      
   const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
     
   GetPot dataFile( data_file_name.data() );

   bandDepthData_Type bdData( dataFile, "BDALL" );
 
   bdFactory_Type factory;
   
   bdBasePtr_Type bdPtr( factory.create( bdData.J() ) );
   
   bdPtr->setBandDepthData( bdData );
       
   bdPtr->computeBDs();
  
   bdPtr->writeBDs();

MPI_Finalize();

  return EXIT_SUCCESS;
  
}