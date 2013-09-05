
#include <dataSet.hpp>

#include <HPCSDefs.hpp>

using namespace std;

using namespace HPCS;

typedef double Real;
typedef unsigned int UInt;
typedef DataSet dataSet_Type;
typedef DataSet::data_Type data_Type;
typedef boost::shared_ptr< data_Type > dataPtr_Type;

int main( int argc, char * argv[] )
{ 
    GetPot command_line( argc, argv );
      
    const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
    
    GetPot dataFile( data_file_name.data() );

    const std::string baseName( "DATASET" );
    
    const UInt nbSamples = dataFile( ( baseName + "/nbSamples" ).data(), 1 );
    
    const UInt nbPts = dataFile( ( baseName + "/nbPts" ).data(), 1 );
   
    const std::string inputFilename = dataFile( ( baseName + "/inputFilename" ).data(), "dataSet.dat" );
    
    // FIRST WAY TO INITIALIZE A DATASET OBJECT
    
    std::cout << "===============================================" << std::endl;
    
    std::cout << "		HILBERT MATRIX : " << std::endl << std::endl;
    
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
    
    dataSet1.showMe();
  
    std::cout << "===============================================" << std::endl << std::endl;
    
    // SECOND WAY TO INITIALIZE A DATASET OBJECT
    
    std::cout << "===============================================" << std::endl;
    
    std::cout << "		DATASET MATRIX : " << std::endl << std::endl;
    
    dataSet_Type dataSet2( nbSamples, nbPts );
    
    dataSet2.readData( "dataSet.dat" );
        
    dataSet2.showMe();
  
    std::cout << "===============================================" << std::endl << std::endl;

    
    return EXIT_SUCCESS;
}


