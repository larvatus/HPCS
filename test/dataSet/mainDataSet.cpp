
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
typedef DataSet dataSet_Type;
typedef DataSet::data_Type data_Type;
typedef boost::shared_ptr< data_Type > dataPtr_Type;

// typedef DataSetLevelled dataSetLevelled_Type;

int main( int argc, char * argv[] )
{
    MPI_Init( & argc, & argv );
    
    int nbThreads, myRank;
    
    MPI_Comm_size( MPI_COMM_WORLD, & nbThreads );

    MPI_Comm_rank( MPI_COMM_WORLD, & myRank );
 
    const UInt nbSamples = 9;
    
    const UInt nbPts = 9;
    
    dataSet_Type dataSet1( nbSamples, nbPts );
    
    dataPtr_Type dataPtr( new data_Type( nbSamples, nbPts) );
    
    for ( UInt iSample(0); iSample < nbSamples; ++iSample)
    {
	for ( UInt iPt(0); iPt < nbPts; ++iPt )
	{
	  (*dataPtr)( iSample, iPt ) = 1./( iSample + iPt + 1 );
	}
    }
    
    dataSet1.setData( dataPtr );
    
    if ( myRank == MASTER ) dataSet1.showMe();
  
    
    if ( myRank == MASTER ) std::cout << std::endl << std::endl;
    
    dataSet_Type dataSet2( nbSamples, nbPts );
    
    dataSet2.readData( "dataSet.dat" );
    
    if ( myRank == MASTER )  dataSet2.showMe();    
    
    MPI_Finalize();
    
    return EXIT_SUCCESS;
}


