

#include <HPCSDefs.hpp>

#include <depthMeasure.hpp>
#include <depthMeasureFactory.hpp>
#include <bandDepthData.hpp>

using namespace std;
using namespace HPCS;

typedef DepthMeasureBase< Reference > dmBase_Type;
typedef boost::shared_ptr< dmBase_Type > dmBasePtr_Type;
typedef DMFactory< Reference > dmFactory_Type;
typedef BandDepthRefData bdData_Type;
typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;

int main( int argc, char * argv[] )
{ 
  MPI_Init( &argc, &argv );
  
   int nbThreads, myRank;
   
   MPI_Comm_rank( MPI_COMM_WORLD, & myRank );
   
   MPI_Comm_size( MPI_COMM_WORLD, & nbThreads );
   
   if ( myRank == MASTER )
   {
      printf( "=======================================\n" );
   }
   
   // INPUT OF DATA FROM FILEANAMES
   
   if ( myRank == MASTER )
   {
      printf( " *** DATA SETUP *** \n" );
   }
   
   GetPot command_line( argc, argv );
      
   const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
   
   const std::string baseName( "BDREFERENCE" );
     
   GetPot dataFile( data_file_name.data() );
   
   bdDataPtr_Type bdDataPtr( new bdData_Type( dataFile, baseName ) );
   
   dmFactory_Type factory;
   
   dmBasePtr_Type dmPtr( factory.create( bdDataPtr->J() ) );
   
   dmPtr->setBandDepthData( bdDataPtr );

   if ( myRank == MASTER )
   {
      printf( " *** COMPUTATION OF DEPTHS *** \n" );
   }
   
   dmPtr->computeDepths();
   
   dmPtr->computeRanks();

   dmPtr->writeDepths();

   if ( myRank == MASTER )
   {
      printf( " *** RANKS INDUCED BY DEPTHS *** \n" );
   }
   
   dmPtr->writeRanks();
   
   if ( myRank == MASTER )
   {
      printf( "=======================================\n" );
   }
   
   
   if ( myRank == MASTER )
   {
      const std::string outputBD( dataFile( (baseName + "/outputFilename" ).data(), "bd.I.dat" ) );
      
      std::string commandString = "gnuplot -p -e \"bd_I=\'" + outputBD + "\'\" depthMeasure.plot";
      
      system( commandString.data() );
   }
   
 MPI_Finalize();
  
  return EXIT_SUCCESS;
  
}