
#include <source/dataSet.hpp>

#include <source/HPCSDefs.hpp>

using namespace std;
using namespace HPCS;

typedef DataSetLevelled dataSetLevelled_Type;
typedef DataSet::data_Type data_Type;
typedef boost::shared_ptr< data_Type > dataPtr_Type;

int main( int argc, char * argv[] )
{
    GetPot command_line( argc, argv );
      
    const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
    
    GetPot dataFile( data_file_name.data() );

    const std::string baseName( "DATASETLEVELLED" );
    
    const UInt nbSamples = dataFile( ( baseName + "/nbSamples" ).data(), 1 );
    
    const UInt nbPts = dataFile( ( baseName + "/nbPts" ).data(), 1 );
    
    const UInt nbLevels = dataFile( ( baseName + "/nbLevels" ).data(), 2 );
   
    const std::string inputFilename = dataFile( ( baseName + "/inputFilename" ).data(), "dataSet.dat" );
    
    const std::string levelsFilename = dataFile( ( baseName + "/levels.dat" ).data(), "levels.dat" );
   
    // FIRST WAY TO INITIALIZE A DATASET LEVELLED OBJECT
    
    std::cout << "===============================================" << std::endl;
    
    std::cout << "		HILBERT MATRIX : " << std::endl << std::endl;
    
    dataSetLevelled_Type dataSet1( nbSamples, nbPts, nbLevels );
    
    dataPtr_Type dataPtr( new data_Type( nbSamples, nbPts) );
    
    for ( UInt iSample(0); iSample < nbSamples; ++iSample)
    {
	for ( UInt iPt(0); iPt < nbPts; ++iPt )
	{
	  (*dataPtr)( iSample, iPt ) = 1./( iSample + iPt + 1 );
	}
    }
    
    dataSet1.setData( dataPtr );
    
    // Setting up the linear extrema
    std::vector< UInt > linearExtrema( 3 );
    
    linearExtrema[ 0 ] = 0;
    linearExtrema[ 1 ] = static_cast< UInt >( nbSamples/2 );
    linearExtrema[ 2 ] = nbSamples;
    
    dataSet1.setLevels( linearExtrema );
    
    dataSet1.showMe();
  
    std::cout << "===============================================" << std::endl << std::endl;
    
    // SECOND WAY TO INITIALIZE A DATASET LEVELLED OBJECT
    
    std::cout << "===============================================" << std::endl;
    
    std::cout << "		DATASET MATRIX : " << std::endl << std::endl;
    
    dataSetLevelled_Type dataSet2( nbSamples, nbPts, nbLevels );
    
    dataSet2.readData( inputFilename );
    
    dataSet2.setLevelsFromExtrema( levelsFilename  );
        
    dataSet2.showMe();
  
    std::cout << "===============================================" << std::endl << std::endl;

    return EXIT_SUCCESS;
}
