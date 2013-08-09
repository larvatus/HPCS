

#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include "bandDepth.hpp"
#include "bandDepthRef.hpp"
#include "dataSet.hpp"
#include "GetPot"


#define MASTER 0

using namespace std;
using namespace HPCS;

typedef double Real;
typedef unsigned int UInt;
typedef BandDepthData bandDepthData_Type;
typedef BandDepth bandDepth_Type;
typedef BandDepthRef bandDepthRef_Type;
typedef DataSet dataSet_Type;
typedef BandDepth::dataSetPtr_Type dataSetPtr_Type;
typedef DataSetLevelled dataSetLevelled_Type;

int main( int argc, char * argv[] )
{
   MPI_Init( &argc, &argv );
  
   GetPot command_line( argc, argv );
      
   const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
     
   GetPot dataFile( data_file_name.data() );

   bandDepthData_Type bdData( dataFile, "BDALL" );
     
//    bandDepthRef_Type bdRef( bdData );
   
//    bdRef.compute();
   
//     bandDepth_Type bd( bdData );
    
//     bd.computeBDs();
  
//    bd.writeBDs();
    
//   const UInt nbSamples( 10 );
//   
//   const UInt nbPts( 30 );
//   
//   const UInt nbLevels( 2 );
//   
//   dataSetLevelled_Type dataLevelled( nbSamples, nbPts, nbLevels );
//   
//   std::vector< UInt > extrema( nbLevels + 1 );
//   
//   extrema[ 0 ] = 0;
//   extrema[ 1 ] = 10;
//   extrema[ 2 ] = 20;
//   
//   dataLevelled.setLevels( extrema );
//   
//   dataLevelled.showMe();
  
   MPI_Finalize();

  return EXIT_SUCCESS;
  
}