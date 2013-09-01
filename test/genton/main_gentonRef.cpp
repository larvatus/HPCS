

#include <bandDepthRef.hpp>
#include <bandDepthData.hpp>
#include <bandDepthFactory.hpp>

#include "user_fun.hpp"

#include <HPCSDefs.hpp>


using namespace std;
using namespace HPCS;

typedef boost::shared_ptr< BandDepthBase< Reference > > BDBasePtr_Type;

int main( int argc, char * argv[] )
{
   // SETTING UP PARALLEL EXECUTION

MPI_Init( &argc, &argv );
   
   int nbThreads, myRank;
   
   MPI_Comm_rank( MPI_COMM_WORLD, & myRank );
   
   MPI_Comm_size( MPI_COMM_WORLD, & nbThreads );
   
   // INPUT OF DATA FROM FILEANAMES
   
   const std::string baseSection = "BDREFERENCE";
   
   GetPot command_line( argc, argv );
      
   const std::string data_file_name = command_line.follow( "data", 2, "-f", "--file" );

   const std::string gentonSection = baseSection + "/GENTON";
   
   GetPot dataFile( data_file_name.data() );
   
   // STARTING COMPUTATION VIA GENTON METHOD
   
   if ( myRank == MASTER ) printf( " ***** STARTING COMPUTATION VIA GENTON ALGORITHM \n" );

   BandDepthRefData bdDataGenton( dataFile, gentonSection );
   
   BDFactory< Reference > bdFactory;
   
   BDBasePtr_Type bdPtr( bdFactory.create( bdDataGenton.J() ) );
   
   bdPtr->setBandDepthData( bdDataGenton );
   
   const UInt nbRefSamples( dataFile( ( gentonSection + "/nbReferenceSamples" ).data(), 5 ) );
   
   bdPtr->addToReferenceSet( 0, nbRefSamples );
   
   bdPtr->setTestSet();
    
   bdPtr->computeBDs();
  
   bdPtr->writeBDs();
   
   if ( myRank == MASTER ) printf( " ***** FINISHED COMPUTATION VIA GENTON ALGORITHM \n" );

   // STARTING COMPUTATION IN DIRECT WAY
   
   if ( myRank == MASTER ) printf( " ***** STARTING COMPUTATION IN DIRECT WAY \n" );
   
   const std::string directSection = baseSection + "/DIRECT";
   
   const std::string outputFilenameDirect( dataFile ( ( directSection + "/outputFilename" ).data(), "bd.all.direct.dat" ) ); 
   
   BDRefDirectComputation bdDirect;
   
   bdDirect.computeBDs( dataFile, directSection );
   
   bdDirect.writeBDs( outputFilenameDirect );

   if ( myRank == MASTER ) printf( " ***** FINISHED COMPUTATION IN DIRECT WAY \n" );
   
    /////////////////////////////////////////////////////////
    /////
    /////	GRAPHICAL COMPARISON SECTION VIA GNUPLOT
    ////
    /////////////////////////////////////////////////////////

    if ( myRank == MASTER  )
    {
      const std::string outputGenton( bdDataGenton.outputFilename() ); 
      
      const std::string outputReference( outputFilenameDirect ); 
    
      FILE *gnuplotPipe = popen("gnuplot -persist", "w");
      fprintf(gnuplotPipe, "\n");
      
      std::string gnuPlotCommand(" set term wxt 0\n");
      
      gnuPlotCommand += "set key box\n";
      gnuPlotCommand += "set grid\n";
      
      fprintf( gnuplotPipe, gnuPlotCommand.data() );
      
      gnuPlotCommand = "plot \"./" + outputGenton + "\" using 2 lt 4 title 'Genton', \"./" + outputReference + "\" using 2 lt 5 title 'Reference'\n";
      
      fprintf( gnuplotPipe, gnuPlotCommand.data() );

      fflush(gnuplotPipe);  

      fclose(gnuplotPipe);

    }
  
MPI_Finalize();

  return EXIT_SUCCESS;
  
}