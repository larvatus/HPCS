

#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include "bandDepth.hpp"
#include "bandDepthData.hpp"
#include "factory.hpp"
// #include "bandDepthRef.hpp"
#include "dataSet.hpp"
#include "GetPot"


#define MASTER 0

using namespace std;
using namespace HPCS;

typedef double Real;
typedef unsigned int UInt;
typedef BandDepthData bandDepthData_Type;
// typedef BandDepthRefData bandDepthRefData_Type;
typedef BandDepth< 2 > bandDepth_Type;
// typedef BandDepthRef bandDepthRef_Type;
typedef DataSet dataSet_Type;
typedef bandDepth_Type::dataSetPtr_Type dataSetPtr_Type;
typedef DataSetLevelled dataSetLevelled_Type;

int main( int argc, char * argv[] )
{
   MPI_Init( &argc, &argv );
  
   GetPot command_line( argc, argv );
      
   const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
     
   GetPot dataFile( data_file_name.data() );


   
//    bandDepthRefData_Type bdRefData( dataFile, "BDREF" );
     
//    bandDepthRef_Type bdRef( bdRefData );
    
//    bdRef.setReferenceSet( 50 );
    
//    bdRef.setSeed( 0 );
   
//    bdRef.computeBDs();

//    bdRef.writeBDs();

      bandDepthData_Type bdData( dataFile, "BDALL" );
   
      bandDepth_Type bd( bdData );
    
      bd.computeBDs();
  
      bd.writeBDs();



MPI_Finalize();

  return EXIT_SUCCESS;
  
}
