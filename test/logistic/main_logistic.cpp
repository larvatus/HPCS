

#include <source/HPCSDefs.hpp>

#include <source/multiDepthMeasure.hpp>
#include <source/multiDepthMeasureFactory.hpp>

using namespace std;
using namespace HPCS;

typedef BandDepthRefData bdData_Type;
typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
typedef MultiDepthMeasureBase< Reference > MDMBase_Type;
typedef boost::shared_ptr< MDMBase_Type > mdmBasePtr_Type;
typedef MultiDepthMeasureFactory< Reference > mdmFactory_Type;

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
   
   std::vector< std::string > baseNames(2);
   
   baseNames[ 0 ] = "MBDREF/primitive";
   baseNames[ 1 ] = "MBDREF/derivative";
   
   if ( myRank == MASTER )
   {
      printf( " *** DATA SETUP *** \n" );  
   }
   
   for ( UInt iCase(0); iCase < 2; ++iCase )
   {
        const UInt J( dataFile( ( baseNames[ iCase ] + "/J" ).data(), 2 ) );
      
	mdmFactory_Type factory;
	    
	mdmBasePtr_Type mdmPtr( factory.create( J ) );
	    
	mdmPtr->setData( dataFile, baseNames[ iCase ] );

	if ( myRank == MASTER )
	{
	    printf( " *** ADDING ALL DIMENSIONS *** \n" );  
	}
	      
	mdmPtr->addDimension( dataFile, baseNames[ iCase ] + "/I" );
	
	mdmPtr->addDimension( dataFile, baseNames[ iCase ] + "/II" );
	
	mdmPtr->addDimension( dataFile, baseNames[ iCase ] + "/V1" );
	
	mdmPtr->addDimension( dataFile, baseNames[ iCase ] + "/V2" );
	
	mdmPtr->addDimension( dataFile, baseNames[ iCase ] + "/V3" );
	
	mdmPtr->addDimension( dataFile, baseNames[ iCase ] + "/V4" );
	
	mdmPtr->addDimension( dataFile, baseNames[ iCase ] + "/V5" );
	
	mdmPtr->addDimension( dataFile, baseNames[ iCase ] + "/V6" );
	   
	
	mdmPtr->setWeights( dataFile, baseNames[ iCase ] );

	if ( myRank == MASTER )
	{
	    printf( " *** COMPUTATION OF DEPTHS *** \n" );
	}
	    
	  mdmPtr->computeMultiDepths();
	    
	  mdmPtr->writeMultiDepths();
	
	  mdmPtr->computeRanks();
	
	const std::string ranksFilename = dataFile( ( baseNames[ iCase ] + "/ranksFilename" ).data(), "ranks.dat" );
	
	  mdmPtr->writeRanks( ranksFilename );     
    }
      
   
   /////////////////////////////////////////////////////////
   /////
   /////	VISUALIZATION VIA GNUPLOT
   ////
   /////////////////////////////////////////////////////////

   if ( myRank == MASTER  )
   {      
     const std::string outputMBD1( dataFile( ( baseNames[ 0 ] + "/outputFilename" ).data(), "mbd.dat" ) );

     const std::string outputMBD2( dataFile( ( baseNames[ 1 ] + "/outputFilename" ).data(), "mbd.dat" ) );
     
//      const std::string outputRKS( dataFile( ( baseName + "/ranksFilename" ).data(), "rks.dat" ) );
          
     std::string commandString = "gnuplot -p -e \"mbd1=\'" + outputMBD1 + "\'; mbd2=\'" + outputMBD2 + "\'\" logistic.plot";
      
     system( commandString.data() );
    
   }
   
   if ( myRank == MASTER )
   {
      printf( "=======================================\n" );
   }
   
  
MPI_Finalize();
  
  return EXIT_SUCCESS;
}