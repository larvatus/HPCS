

#include <source/HPCSDefs.hpp>

#include <source/multiDepthMeasure.hpp>
#include <source/multiDepthMeasureFactory.hpp>

using namespace std;
using namespace HPCS;

typedef BandDepthRefData bdData_Type;
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
   
   GetPot command_line( argc, argv );
      
   const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
     
   GetPot dataFile( data_file_name.data() );
   
   std::string baseName = "FBPLOT";
   
   if ( myRank == MASTER )
   {
      printf( " *** DATA SETUP *** \n" );  
   }
   
   
   const UInt J( dataFile( ( baseName + "/J" ).data(), 2 ) );
      
   mdmFactory_Type factory;
	    
    mdmBasePtr_Type mdmPtr( factory.create( J ) );
	    
    mdmPtr->setData( dataFile, baseName );

    if ( myRank == MASTER )
    {
	printf( " *** ADDING ALL DIMENSIONS *** \n" );  
    }
	      
    mdmPtr->addDimension( dataFile, baseName + "/I" );
	
    mdmPtr->addDimension( dataFile, baseName + "/II" );
	
    mdmPtr->addDimension( dataFile, baseName + "/V1" );
	
    mdmPtr->addDimension( dataFile, baseName + "/V2" );
	
    mdmPtr->addDimension( dataFile, baseName + "/V3" );
	
    mdmPtr->addDimension( dataFile, baseName + "/V4" );
	
    mdmPtr->addDimension( dataFile, baseName + "/V5" );
	
    mdmPtr->addDimension( dataFile, baseName + "/V6" );
	   
	
    mdmPtr->setWeights( dataFile, baseName );

    if ( myRank == MASTER )
    {
      printf( " *** COMPUTATION OF DEPTHS *** \n" );
    }
	    
    mdmPtr->computeMultiDepths();
	    
    mdmPtr->writeMultiDepths();
	
    mdmPtr->computeRanks();
	
    const std::string ranksFilename = dataFile( ( baseName + "/ranksFilename" ).data(), "ranks.dat" );
	
    mdmPtr->writeRanks( ranksFilename );     
      
   
   /////////////////////////////////////////////////////////
   /////
   /////	VISUALIZATION VIA GNUPLOT
   ////
   /////////////////////////////////////////////////////////

   if ( myRank == MASTER  )
   {      
     const std::string outputMBD( dataFile( ( baseName + "/outputFilename" ).data(), "mbd.dat" ) );
     
     const std::string outputRKS( dataFile( ( baseName + "/ranksFilename" ).data(), "rks.dat" ) );
          
     std::string commandString = "gnuplot -p -e \"mbd=\'" + outputMBD + "\'; rks=\'" + outputRKS + "\'\" fbplot.plot";
      
     system( commandString.data() );
    
   }
   
   if ( myRank == MASTER )
   {
      printf( "=======================================\n" );
   }
   
  
MPI_Finalize();
  
  return EXIT_SUCCESS;
}