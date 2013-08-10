
#include "../../dataSet.hpp"
#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <boost/shared_ptr.hpp>

#define MASTER 0

using namespace std;

using namespace HPCS;

typedef double Real;
typedef unsigned int UInt;
typedef DataSetLevelled dataSetLevelled_Type;
typedef DataSet::data_Type data_Type;
typedef boost::shared_ptr< data_Type > dataPtr_Type;

int main( int argc, char * argv[] )
{
    MPI_Init( & argc, & argv );
    
    int nbThreads, myRank;
    
    MPI_Comm_size( MPI_COMM_WORLD, & nbThreads );

    MPI_Comm_rank( MPI_COMM_WORLD, & myRank );
 
    const UInt nbSamples = 9;
    
    const UInt nbPts = 9;
    
    dataSetLevelled_Type dataSet1( nbSamples, nbPts, 2 );
    
    dataPtr_Type dataPtr( new data_Type( nbSamples, nbPts) );
    
    for ( UInt iSample(0); iSample < nbSamples; ++iSample)
    {
	for ( UInt iPt(0); iPt < nbPts; ++iPt )
	{
	  (*dataPtr)( iSample, iPt ) = 1./( iSample + iPt + 1 );
	}
    }
    
    dataSet1.setData( dataPtr );
    
    std::vector< UInt > linearExtrema( 3 );
    
    linearExtrema[ 0 ] = 0;
    linearExtrema[ 1 ] = 3;
    linearExtrema[ 2 ] = 9;
    
    dataSet1.setLevels( linearExtrema );
    
    if ( myRank == MASTER ) dataSet1.showMe();
    
    if ( myRank == MASTER ) std::cout << std::endl << std::endl;
    
    dataSetLevelled_Type dataSet2( nbSamples, nbPts, 2 );
    
    dataSet2.readData( "dataSet.dat" );
    
    dataSet2.setLevels( linearExtrema );
    
    if ( myRank == MASTER )  dataSet2.showMe();    
    
    MPI_Finalize();
    
    return EXIT_SUCCESS;
}
