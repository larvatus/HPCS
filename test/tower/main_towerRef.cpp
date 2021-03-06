

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
   
   std::vector< std::string > baseNames(3);
   
   baseNames[ 0 ] = "MBDREF/first";
   baseNames[ 1 ] = "MBDREF/second";
   baseNames[ 2 ] = "MBDREF/third";
   
   for ( UInt iJ(0); iJ < 3; ++iJ )
    {
	if ( myRank == MASTER )
	{
	    printf( " *** DATA SETUP *** \n" );
	}
      
      const UInt J( dataFile( ( baseNames[ iJ ] + "/J" ).data(), 2 ) );
      
      mdmFactory_Type factory;
      
      mdmBasePtr_Type mdmPtr( factory.create( J ) );
      
      mdmPtr->setData( dataFile, baseNames[ iJ ] );

	if ( myRank == MASTER )
	{
	    printf( " *** ADDING ALL DIMENSIONS *** \n" );
	}
	
      mdmPtr->addDimension( dataFile, baseNames[ iJ ] + "/I" );

      mdmPtr->addDimension( dataFile, baseNames[ iJ ] + "/II" );
      
      mdmPtr->setWeights( dataFile, baseNames[ iJ ] );
      

      if ( myRank == MASTER )
      {
	  printf( " *** COMPUTATION OF DEPTHS *** \n" );
      }
      
      mdmPtr->computeMultiDepths();
      
      mdmPtr->writeMultiDepths();
   }
   
    /////////////////////////////////////////////////////////
    /////
    /////	VISUALIZATION VIA GNUPLOT
    ////
    /////////////////////////////////////////////////////////

    if ( myRank == MASTER  )
    {      
      const std::string outputMBD1( dataFile( ( baseNames[ 0 ] + "/outputFilename" ).data(), "mbd.1.dat" ) );
    
      const std::string outputMBD2( dataFile( ( baseNames[ 1 ] + "/outputFilename" ).data(), "mbd.2.dat" ) );
    
      const std::string outputMBD3( dataFile( ( baseNames[ 2 ] + "/outputFilename" ).data(), "mbd.3.dat" ) );
      
      std::string commandString = "gnuplot -p -e \"mbd1=\'" + outputMBD1 + "\'; mbd2=\'" + outputMBD2 + "\'; mbd3=\'" + outputMBD3 + "\'\" tower.plot";
      
      system( commandString.data() );
    
   }
   
  
MPI_Finalize();
  
  return EXIT_SUCCESS;
}