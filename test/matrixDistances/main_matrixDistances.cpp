
#include <source/dataSet.hpp>
#include <source/varCovStructure.hpp>
#include <source/matrixDistances.hpp>

#include <source/HPCSDefs.hpp>

using namespace std;

using namespace HPCS;

typedef DataSet dataSet_Type;
typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;

typedef VarCovStructure varCovStructure_Type;

typedef MatrixDistanceBase::matrix_Type matrix_Type;
typedef MatrixDistanceBase::matrixPtr_Type matrixPtr_Type;

typedef ProcrustesDistance dist_Type;

int main( int argc, char * argv[] )
{
    GetPot command_line( argc, argv );
      
    const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
    
    GetPot dataFile( data_file_name.data() );

    const std::string baseName( "MATRIXDISTANCES" );
    
    const UInt nbSamples = dataFile( ( baseName + "/nbSamples" ).data(), 1 );
    
    const UInt nbPts = dataFile( ( baseName + "/nbPts" ).data(), 1 );
   
    const std::string inputFile1 = dataFile( ( baseName + "/inputFile1" ).data(), "dataSet1.dat" );
    
    const std::string inputFile2 = dataFile( ( baseName + "/inputFile2" ).data(), "dataSet2.dat" );
  
    std::cout << " *** READING DATA " << std::endl;
    
    dataSetPtr_Type dataSetPtr1( new dataSet_Type( nbSamples, nbPts ) );
    dataSetPtr_Type dataSetPtr2( new dataSet_Type( nbSamples, nbPts ) );    
    
    std::cout << " ### Dataset 1 " << std::endl;
    
    dataSetPtr1->readData( inputFile1 );
    dataSetPtr1->showMe();
    
    std::cout << std::endl << std::endl;
    std::cout << " ### Dataset 1 " << std::endl;
    
    dataSetPtr2->readData( inputFile2 );
    dataSetPtr2->showMe();

    std::cout << std::endl << std::endl;
    
    std::cout << " *** COMPUTING VAR-COV MATRICES " << std::endl;
    
    varCovStructure_Type varCovStructure1( dataSetPtr1 );
    
    varCovStructure_Type varCovStructure2( dataSetPtr2 );
       
    matrixPtr_Type varPtr1( new matrix_Type( nbPts, nbPts ) );
    matrixPtr_Type varPtr2( new matrix_Type( nbPts, nbPts ) );
/*   
      for ( UInt iPt(0); iPt < nbPts; ++iPt )
      {
	for ( UInt jPt(0); jPt < nbPts; ++jPt )
	{
	    (*varPtr1)( iPt, jPt ) = 1;
	    
	    (*varPtr2)( iPt, jPt ) = 2;
	}
      }*/

    std::cout << " ### Var-Cov matrix 1 " << std::endl;
    
    varCovStructure1.varCovMatrix( varPtr1 );
    
    std::cout << *varPtr1 << std::endl;
    
    std::cout << std::endl << std::endl;
    
    std::cout << " ### Var-Cov matrix 2 " << std::endl;
    
   varCovStructure2.varCovMatrix( varPtr2 );
    
    std::cout << *varPtr2 << std::endl;
    
    std::cout << std::endl << std::endl;
    
    std::cout << " *** COMPUTING THE DISTANCE " << std::endl;
    
    dist_Type varCovDistance( varPtr1, varPtr2 );
    
    std::cout << " #### DISTANCE is " << varCovDistance.compute() << std::endl;
    
    std::cout << "##################################################" << std::endl;
    
  
    return EXIT_SUCCESS;
}