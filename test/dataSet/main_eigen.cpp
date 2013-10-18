
#include <source/dataSet.hpp>
#include <source/varCovStructure.hpp>

#include <source/HPCSDefs.hpp>


using namespace std;

using namespace HPCS;

typedef DataSet dataSet_Type;
typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;

typedef DataSet::matrix_Type matrix_Type;
typedef DataSet::matrixPtr_Type matrixPtr_Type;

typedef VarCovStructure varCovStructure_Type;

int main( int argc, char * argv[] )
{ 
    GetPot command_line( argc, argv );
      
    const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
    
    GetPot dataFile( data_file_name.data() );

    const std::string baseName( "EIGEN" );
    
    const UInt nbSamples = dataFile( ( baseName + "/nbSamples" ).data(), 1 );
    
    const UInt nbPts = dataFile( ( baseName + "/nbPts" ).data(), 1 );
   
    const std::string inputFile = dataFile( ( baseName + "/inputFile" ).data(), "dataSet.dat" );
    
    
    std::cout << "===============================================" << std::endl;
    
    std::cout << " *** DATASET MATRIX *** " << std::endl << std::endl;
    
    dataSetPtr_Type dataSetPtr( new dataSet_Type( nbSamples, nbPts ) );
    
    dataSetPtr->readData( inputFile );
        
    dataSetPtr->showMe();

    varCovStructure_Type varCov( dataSetPtr );
    
    matrixPtr_Type varMatrixPtr;
    matrixPtr_Type corMatrixPtr;
    
    varCov.varCovMatrix( varMatrixPtr );
    varCov.corMatrix( corMatrixPtr );
    
    std::cout << "*** VARIANCE MATRIX ***" << std::endl;
    
    std::cout << *varMatrixPtr << std::endl << std::endl;
    
    std::cout << "*** CORRELATION MATRIX ***" << std::endl;
  
    std::cout << *corMatrixPtr << std::endl;
    
    std::cout << "===============================================" << std::endl << std::endl;
    
    std::cout << " *** SPECTRAL DECOMPOSITION OF VAR-COV STRUCTURE " << std::endl << std::endl;
    
      std::vector< Real > eigenvalues;
//     boost::shared_ptr< std::vector< Real > > eigenPtr( new std::vector< Real >() );
//     std::vector< Real > eigenvalues( nbPts );
    
    varCov.performSpectralDecomposition();
    
    varCov.eigenValues( eigenvalues );
    
//     varCov.eigenValues( eigenPtr );
    
//     varCov.eigenValues( eigenvalues.begin(), eigenvalues.end() );
    
    std::cout << " EIGENVALUES: " << std::endl << std::endl;
    
    for ( UInt iValue(0); iValue < nbPts; ++iValue )
    {
 	std::cout << eigenvalues[ iValue ] << "\t";

// 	std::cout << (*eigenPtr)[ iValue ] << "\t";
	
    }
 
    std::cout << std::endl << std::endl;
    
    std::cout << " EIGENVECTORS: " << std::endl << std::endl;
    
    matrixPtr_Type eigenvectorsPtr;
    
    varCov.eigenVectors( eigenvectorsPtr );
    
    std::cout << *eigenvectorsPtr << std::endl;
 
    std::cout << std::endl;
    
    std::cout << "===============================================" << std::endl;
    
    return EXIT_SUCCESS;
}


