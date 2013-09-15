

#include <source/HPCSDefs.hpp>

#include <source/dataSet.hpp>

using namespace std;
using namespace HPCS;

typedef boost::shared_ptr< DataSetLevelled > dataSetPtr_Type;
typedef DataSetLevelled dataSet_Type;

int main( int argc, char * argv[] )
{
   printf( "=======================================\n" );
   
   // INPUT OF DATA FROM FILEANAMES
   
   GetPot command_line( argc, argv );
      
   const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
     
   GetPot dataFile( data_file_name.data() );
   
   std::string baseName;
   
   baseName = "WILCOXON/SAMPLE";
   
   printf( "	READING DATA\n" );
   
   const UInt nbLeads = 8;
   
   const UInt seed = dataFile( ( baseName + "/seed" ).data(), 10 );
   
//    std::cout << " SEED IS " << seed << std::endl;
   
   const UInt nbPz = dataFile( ( baseName + "/nbPz" ).data(), 0 );
   
   const UInt nbPts = dataFile( ( baseName + "/nbPts" ).data(), 0);
   
   const UInt nbRefSamples = dataFile( ( baseName + "/nbReferenceSamples" ).data(), 0 );
   
   const std::string levelsExtremaFilename = dataFile( ( baseName + "/levelsExtremaFilename" ).data(), "levels.dat" );
  
   const std::string pathToData = dataFile( ( baseName + "/pathToData" ).data(), "./" );
   
   std::vector< std::string > fileIDs(nbLeads);
   std::vector< std::string > fileRefIDs(nbLeads);
   
   fileIDs[0] = "I.txt";
   fileIDs[1] = "II.txt";
   fileIDs[2] = "V1.txt";
   fileIDs[3] = "V2.txt";
   fileIDs[4] = "V3.txt";
   fileIDs[5] = "V4.txt";
   fileIDs[6] = "V5.txt";
   fileIDs[7] = "V6.txt";
   
   fileRefIDs[0] = "I.ref.txt";
   fileRefIDs[1] = "II.ref.txt";
   fileRefIDs[2] = "V1.ref.txt";
   fileRefIDs[3] = "V2.ref.txt";
   fileRefIDs[4] = "V3.ref.txt";
   fileRefIDs[5] = "V4.ref.txt";
   fileRefIDs[6] = "V5.ref.txt";
   fileRefIDs[7] = "V6.ref.txt";
   
   std::vector< UInt > IDs( nbRefSamples ); 
    
   srand48( seed );
   
   for ( UInt iSample(0); iSample < nbRefSamples; ++iSample )
   {
      IDs[ iSample ] = 101 * drand48();
      
//       std::cout << IDs[ iSample ] << "\t";
   }
   
//    std::cout << std::endl;
  
   dataSetPtr_Type dataSetPtr( new dataSet_Type( nbPz, nbPts, 2 ) );
   
   printf( " SAMPLING AND SAVING A REDUCED DATASET\n" );
   
   
   for ( UInt iLead(0); iLead < nbLeads; ++iLead )
   {
      dataSetPtr->readData( pathToData + fileIDs[ iLead ] ); 
      
      dataSetPtr_Type tempPtr( new dataSet_Type( dataSetPtr->getRowSubSet( IDs ), 2 ) );
      
      std::ofstream output( ( pathToData + fileRefIDs[ iLead ]).data(), std::ios_base::out );
      
//       std::cout << "writing to " << pathToData + fileRefIDs[ iLead ] << std::endl;
      
      tempPtr->writeData( output );
      
      output.close();
   }
   
    printf( "=======================================\n" );
  
  return EXIT_SUCCESS;
}