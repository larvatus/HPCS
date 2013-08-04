
#include "BandDepth.hpp"
#include "dataSet.hpp"

namespace HPCS
{
  
 ///////////////////////////
 //	BD DATA
 //////////////////////////
  
 // Constructor from single variables.
 BDData::
 BDData( const UInt & nbPz, const UInt & nbPts, const UInt & leftOffset, 
	 const UInt & rightOffset, const UInt & J, const UInt & verbosity )
 :
 M_nbPz( nbPz ),
 M_nbPts( nbPts ),
 M_leftOffset( leftOffset ),
 M_rightOffset( rightOffset ),
 M_J( J ),
 M_verbosity( verbosity )
 {}
  
 // Constructor from Get Pot type object
 BDData::
 BDData( const data_Type & dataFile, const std::string & section )
 {
    const std::string baseName( section );
      
    this->M_nbPz =  dataFile( ( baseName + "/nbPz"  ).data(), 10 );
      
    const UInt nbPtsFull = dataFile( ( baseName + "/nbPts" ).data(), 100 );
      
    this->M_leftOffset = dataFile( ( baseName + "/leftOffset" ).data(), 0 );
      
    this->M_rightOffset = dataFile( ( baseName + "/rightOffset" ).data(), 0 );
      
    this->M_nbPts = nbPtsFull - M_leftOffset - M_rightOffset;
      
    this->M_J = dataFile( ( baseName + "/J" ).data(), 2 );

    this->M_verbosity = dataFile( ( baseName + "/verbosity" ).data(), false );
      
    this->M_inputFilename = dataFile( ( baseName + "/inputFilename" ).data(), "data.dat");
      
    this->M_outputFilename = dataFile( ( baseName + "/outputFilename" ).data(), "bd.dat" );
      
 }
 
 // Copy constructor
 BDData::
 BDData( const BDData & bdData )
 {
    this->M_nbPz = bdData.nbPz();   
    
    this->M_leftOffset = bdData.leftOffset();
    
    this->M_rightOffset = bdData.leftOffset();
    
    this->M_nbPts = bdData.nbPts();
    
    this->M_J = bdData.J();
    
    this->M_verbosity = bdData.verbosity();
    
    this->M_inputFilename = bdData.inputFilename();
    
    this->M_outputFilename = bdData.outputFilename();
 }
 
 // Setter for the output filename
 void 
 BDData::
 setInputFilename( const std::string & inputFilename )
 {
    this->M_inputFilename = inputFilename;
    
    return;
 }
 
 // Setter for the input filename  
 void 
 BDData::
 setOutputFilename( const std::string & outputFilename )
 {
    this->M_outputFilename = outputFilename;
    
    return;
 }

 /////////////////////////
 // 	Band Depth
 /////////////////////////
 
 // Constructor from Get Pot type object
 BandDepth::
 BandDepth( const inputData_Type & dataFile, const std::string & section  )
 {  
   this->M_bdDataPtr.reset( new BDData( dataFile, section ) );   

   this->readData();
 }
 
 // Constructor from BDData type object
 BandDepth::
 BandDepth( const bdData_Type & bdData )
 {
   this->M_bdDataPtr.reset( new bdData_Type( bdData ) );
   
   this->readData();

 } 
  
 // Reset BDData type object contained 
 void 
 BandDepth::
 resetBDData( const bdData_Type & bdData )
 {
    this->M_bdDataPtr.reset( new bdData_Type( bdData ) );
    
    this->readData();
 }
   
 // Each proces reads data from data files.
 void 
 BandDepth::
 readData()
 {
   this->M_dataSetPtr.reset( new dataSet_Type( M_bdDataPtr->nbPz(), M_bdDataPtr->nbPts(), M_bdDataPtr->leftOffset(), M_bdDataPtr->rightOffset() ) );
   
   this->M_dataSetPtr->readCSV( M_bdDataPtr->inputFilename() );
   
 }
 
   
}