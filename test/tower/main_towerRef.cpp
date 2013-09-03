

#include <HPCSDefs.hpp>
#include <multiDepthMeasure.hpp>
#include <multiDepthMeasureFactory.hpp>

using namespace std;
using namespace HPCS;

typedef BandDepthData bdData_Type;
typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
typedef MultiDepthMeasureBase< All > MDMBase_Type;
typedef boost::shared_ptr< MDMBase_Type > mdmBasePtr_Type;
typedef MultiDepthMeasureFactory< All > mdmFactory_Type;

int main( int argc, char * argv[] )
{

MPI_Init( & argc, & argv );
  
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
     
   GetPot dataFile( data_file_name.data() );
   
   const std::string baseName( "MBDALL" );
   
   const UInt J( dataFile( ( baseName + "/J" ).data(), 2 ) );
   
   mdmFactory_Type factory;
   
   mdmBasePtr_Type mdmPtr( factory.create( J ) );
   
   mdmPtr->setData( dataFile, "MBDALL" );

    if ( myRank == MASTER )
   {
      printf( " *** ADDING ALL DIMENSIONS *** \n" );
   }
   
   mdmPtr->addDimension( dataFile, baseName + "/I" );

   mdmPtr->addDimension( dataFile, baseName + "/II" );
   
   mdmPtr->setWeights( dataFile, baseName );
   

   if ( myRank == MASTER )
   {
      printf( " *** COMPUTATION OF DEPTHS *** \n" );
   }
   
   mdmPtr->computeMultiDepths();
   
   mdmPtr->computeRanks();

   mdmPtr->writeMultiDepths();

   if ( myRank == MASTER )
   {
      printf( " *** RANKS INDUCED BY DEPTHS *** \n" );
   }
   
   mdmPtr->writeRanks();
   
    /////////////////////////////////////////////////////////
    /////
    /////	VISUALIZATION VIA GNUPLOT
    ////
    /////////////////////////////////////////////////////////

    if ( myRank == MASTER  )
    {      
      const std::string outputBD1( dataFile( (baseName + "/I/outputFilename" ).data(), "bd.I.dat" ) ); 
      
      const std::string outputBD2( dataFile( (baseName + "/II/outputFilename" ).data(), "bd.II.dat" ) ); 
    
      const std::string outputMBD( dataFile( (baseName + "/outputFilename" ).data(), "mbd.dat" ) );
      
      std::string commandString = "gnuplot -p -e \"bd_I=\'" + outputBD1 + "\'; bd_II=\'" + outputBD2 + "\'; mbd=\'" + outputMBD + "\'\" multiDepthMeasureAll.plot";
      
      system( commandString.data() );
    
   }
   
  
MPI_Finalize();
  
  return EXIT_SUCCESS;
}