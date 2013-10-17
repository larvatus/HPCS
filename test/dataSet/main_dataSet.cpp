
#include <source/dataSet.hpp>
#include <source/varCovStructure.hpp>

#include <source/HPCSDefs.hpp>


using namespace std;

using namespace HPCS;

typedef DataSet dataSet_Type;
typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;

typedef DataSet::data_Type data_Type;
typedef DataSet::dataPtr_Type dataPtr_Type;

typedef DataSet::matrix_Type matrix_Type;
typedef DataSet::matrixPtr_Type matrixPtr_Type;

typedef VarCovStructure varCovStructure_Type;

int main( int argc, char * argv[] )
{ 
    GetPot command_line( argc, argv );
      
    const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
    
    GetPot dataFile( data_file_name.data() );

    const std::string baseName( "DATASET" );
    
    const UInt nbSamples = dataFile( ( baseName + "/nbSamples" ).data(), 1 );
    
    const UInt nbPts = dataFile( ( baseName + "/nbPts" ).data(), 1 );
    
    // FIRST WAY TO INITIALIZE A DATASET OBJECT
    
    std::cout << "===============================================" << std::endl;
    
    std::cout << " *** HILBERT MATRIX : " << std::endl << std::endl;
    
    dataSetPtr_Type dataSetPtr1( new dataSet_Type( nbSamples, nbPts ) );
    
    dataPtr_Type dataPtr( new data_Type( nbSamples, nbPts) );
    
    for ( UInt iSample(0); iSample < nbSamples; ++iSample)
    {
	for ( UInt iPt(0); iPt < nbPts; ++iPt )
	{
	  (*dataPtr)( iSample, iPt ) = 1./( iSample + iPt + 1 );
	}
    }
    
    dataSetPtr1->setData( dataPtr );
    
    dataSetPtr1->showMe();
    
    varCovStructure_Type varCov1( dataSetPtr1 );
    
    matrixPtr_Type varMatrixPtr;
    matrixPtr_Type corMatrixPtr;

    varCov1.varCovMatrix( varMatrixPtr );
    varCov1.corMatrix( corMatrixPtr );
    
    std::cout << "*** VARIANCE MATRIX ***" << std::endl;
    
    std::cout << *varMatrixPtr << std::endl << std::endl;
    
    std::cout << "*** CORRELATION MATRIX ***" << std::endl;
  
    std::cout << *corMatrixPtr << std::endl;
    
    std::cout << "===============================================" << std::endl << std::endl;
    
    // SECOND WAY TO INITIALIZE A DATASET OBJECT
    
    std::cout << "===============================================" << std::endl;
    
    std::cout << " *** DATASET MATRIX *** " << std::endl << std::endl;
    
    dataSetPtr_Type dataSetPtr2( new dataSet_Type( nbSamples, nbPts ) );
    
    const std::string inputFile = dataFile( ( baseName + "/inputFile" ).data(), "dataSet.dat" );
    
    dataSetPtr2->readData( inputFile );
        
    dataSetPtr2->showMe();

    varCovStructure_Type varCov2( dataSetPtr2 );
    
    varCov2.varCovMatrix( varMatrixPtr );
    varCov2.corMatrix( corMatrixPtr );
    
    std::cout << "*** VARIANCE MATRIX ***" << std::endl;
    
    std::cout << *varMatrixPtr << std::endl << std::endl;
    
    std::cout << "*** CORRELATION MATRIX ***" << std::endl;
  
    std::cout << *corMatrixPtr << std::endl;
    
    std::cout << "===============================================" << std::endl << std::endl;
    
    std::cout << "===============================================" << std::endl;
    
    std::cout << " *** RANDOM SELECTION OF DATASET ROWS *** " << std::endl << std::endl;
    
    srand48(505);
    
    const UInt nbIDs( static_cast< UInt > (nbSamples/3) );
    
    std::vector< UInt > IDs( nbIDs );
    
    std::cout << " Sorted IDs: " << std::endl;
    
    for ( UInt iRand(0); iRand < IDs.size(); ++iRand )
    {
      IDs[ iRand ] = 1 + static_cast< UInt > ( ( nbSamples -1 )* drand48() );
      
      std::cout << IDs[ iRand ] << "\t";
    }
    
    std::cout << std::endl << std::endl;
    
    dataPtr_Type dataSubSet( dataSetPtr2->getRowSubSet( IDs ) );
    
    std::cout << *dataSubSet << std::endl;
    
    std::cout << "===============================================" << std::endl;   
    
    return EXIT_SUCCESS;
}


