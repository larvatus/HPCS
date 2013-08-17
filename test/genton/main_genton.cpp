

#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include "bandDepth.hpp"
#include "bandDepthData.hpp"
#include "dataSet.hpp"
#include "user_fun.hpp"
#include "GetPot"


#define MASTER 0

using namespace std;
using namespace HPCS;

typedef double Real;
typedef unsigned int UInt;
typedef BandDepthData bandDepthData_Type;
typedef BandDepth< 2 > bandDepth_Type;
typedef DataSet dataSet_Type;
typedef bandDepth_Type::dataSetPtr_Type dataSetPtr_Type;
typedef DataSetLevelled dataSetLevelled_Type;

int main( int argc, char * argv[] )
{
   // SETTING UP PARALLEL EXECUTION
  
MPI_Init( &argc, &argv );
 
   int nbThreads, myRank;
   
   MPI_Comm_rank( MPI_COMM_WORLD, & myRank );
   
   MPI_Comm_size( MPI_COMM_WORLD, & nbThreads );
   
   // INPUT OF DATA FROM FILEANAMES
   
   GetPot command_line( argc, argv );
      
   const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
     
   GetPot dataFile( data_file_name.data() );
   
   // STARTING COMPUTATION VIA GENTON METHOD
   
   if ( myRank == MASTER ) printf( " ***** STARTING COMPUTATION VIA GENTON ALGORITHM \n" );

   bandDepthData_Type bdData( dataFile, "GENTON" );
   
   bandDepth_Type bd( bdData );
    
   bd.computeBDs();
  
   bd.writeBDs();
   
   if ( myRank == MASTER ) printf( " ***** FINISHED COMPUTATION VIA GENTON ALGORITHM \n" );

   // STARTING COMPUTATION IN DIRECT WAY
   
   if ( myRank == MASTER ) printf( " ***** STARTING COMPUTATION IN DIRECT WAY \n" );
   
   GentonTest::computeBDs( dataFile, "REFERENCE" );

   
    /////////////////////////////////////////////////////////
    /////
    /////	GRAPHICAL COMPARISON SECTION VIA GNUPLOT
    ////
    /////////////////////////////////////////////////////////

 if ( myRank == MASTER  )
 {
    const std::string outputGenton( dataFile( "GENTON/outputFilename", "bd.I.genton.dat" ) ); 
    const std::string outputReference( dataFile( "REFERENCE/outputFilename", "bd.I.reference.dat" ) ); 
   
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    fprintf(gnuplotPipe, "\n");
    
    std::string gnuPlotCommand(" set term wxt 0\n");
    
    gnuPlotCommand += "set key box\n";
    gnuPlotCommand += "set grid\n";
    
    fprintf( gnuplotPipe, gnuPlotCommand.data() );
    
    gnuPlotCommand = "plot \"./" + outputGenton + "\" using 2 lt 2 title 'Genton', \"./" + outputReference + "\" using 2 lt 3 title 'Reference'\n";
    
    fprintf( gnuplotPipe, gnuPlotCommand.data() );

    fflush(gnuplotPipe);  

    fclose(gnuplotPipe);

 }
 
MPI_Finalize();

  return EXIT_SUCCESS;
  
}