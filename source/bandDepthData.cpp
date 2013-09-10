
#include <source/bandDepthData.hpp>

namespace HPCS
{

 /*!
 *   @file BandDepthData.cpp
     @brief Definition of BandDepthData classes.

     @date 08/2013
     @author Nicholas Tarabelloni  <nicholas.tarabelloni@gmail.com>
     @maintainer Nicholas Tarabelloni  <nicholas.tarabelloni@gmail.com>
*/
  
  
 ///////////////////////////
 //	BD DATA
 //////////////////////////
  
 // Constructor from single variables.
 BandDepthData::
 BandDepthData( const UInt & nbPz, const UInt & nbPts, const UInt & leftOffset, 
	 const UInt & rightOffset, const UInt & J, const UInt & verbosity,
	 const bool & readDataFromFile )
 :
 M_nbPz( nbPz ),
 M_nbPts( nbPts ),
 M_leftOffset( leftOffset ),
 M_rightOffset( rightOffset ),
 M_J( J ),
 M_verbosity( verbosity ),
 M_readDataFromFile( readDataFromFile )
 {}
  
 // Constructor from Get Pot type object
 BandDepthData::
 BandDepthData( const data_Type & dataFile, const std::string & section )
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
    
    this->M_readDataFromFile = true;
    
    this->M_outputFilename = dataFile( ( baseName + "/outputFilename" ).data(), "bd.dat" );
      
 }
 
 // Copy constructor
 BandDepthData::
 BandDepthData( const BandDepthData & bdData )
 {
    this->M_nbPz = bdData.nbPz();   
    
    this->M_leftOffset = bdData.leftOffset();
    
    this->M_rightOffset = bdData.rightOffset();
    
    this->M_nbPts = bdData.nbPts();
    
    this->M_J = bdData.J();
    
    this->M_verbosity = bdData.verbosity();
    
    this->M_inputFilename = bdData.inputFilename();
    
    this->M_outputFilename = bdData.outputFilename();
    
    this->M_readDataFromFile = bdData.readDataFromFile();
 }
 
 // Show me method
 void
 BandDepthData::
 showMe( std::ostream & output ) const
 {
   output << "--------------------------------" << std::endl;
   output << " ### BAND DEPTH DATA ### " << std::endl;

   output << " nbPz = " 		<< this->M_nbPz 		<< std::endl;
   output << " leftOffset = " 		<< this->M_leftOffset 		<< std::endl;
   output << " rightOffset = " 		<< this->M_rightOffset 		<< std::endl;
   output << " nbPts = " 		<< this->M_nbPts 		<< std::endl;
   output << " J = " 			<< this->M_J 			<< std::endl;
   output << " verbosity = " 		<< this->M_verbosity 		<< std::endl;
   output << " inputFilename = "	<< this->M_inputFilename 	<< std::endl;
   output << " outputFilename = "	<< this->M_outputFilename 	<< std::endl;
   output << " readDataFromFile = " 	<< this->M_readDataFromFile 	<< std::endl;
   
   output << "---------------------------------" << std::endl;
   
   return;
   
 }
 
 // Setter for the output filename
 void 
 BandDepthData::
 setInputFilename( const std::string & inputFilename )
 {
    this->M_inputFilename = inputFilename;
    
    this->M_readDataFromFile = true;
    
    return;
 }
 
 // Setter for the input filename  
 void 
 BandDepthData::
 setOutputFilename( const std::string & outputFilename )
 {
    this->M_outputFilename = outputFilename;
    
    return;
 }
 
 
 ///////////////////////////
 //	BDREF DATA
 //////////////////////////
 
 // Constructor from single variables.
 BandDepthRefData::
 BandDepthRefData( const UInt & nbPz, const UInt & nbPts, const UInt & leftOffset, 
	 const UInt & rightOffset, const UInt & J, const UInt & verbosity,
	 const bool & readDataFromFile, const bool & readLevelsExtremaFromFile, const UInt & seed )
 :
 BandDepthData(nbPz, nbPts, leftOffset, rightOffset, J, verbosity, readDataFromFile),
 M_readLevelsExtremaFromFile( readLevelsExtremaFromFile ),
 M_seed( seed )
 {}
  
 // Constructor from Get Pot type object
 BandDepthRefData::
 BandDepthRefData( const data_Type & dataFile, const std::string & section )
 :
 BandDepthData( dataFile, section )
 {
    const std::string baseName( section );
    
    this->M_levelsExtremaFilename =  dataFile( ( baseName + "/levelsExtremaFilename"  ).data(), "levels.dat" );
    
    this->M_nbRefSamples = dataFile( ( baseName + "/nbReferenceSamples" ).data(), 0 );
    
    this->M_seed = dataFile( ( baseName + "/seed" ).data(), 1 );
    
    this->M_readLevelsExtremaFromFile = true;
      
 }
 
 // Setter for the input filename containing extrema data
 void 
 BandDepthRefData::
 setLevelsExtremaFilename( const std::string & inputFilename )
 {
    this->M_levelsExtremaFilename = inputFilename;
    
    this->M_readLevelsExtremaFromFile = true;
    
    return;
}

 // Method for resetting the number of Reference samples.
 void 
 BandDepthRefData::
 setNbReferenceSamples( const UInt & nbRefSamples )
 {
    this->M_nbRefSamples = nbRefSamples;
    
    return;
 }

 // Copy constructor
 BandDepthRefData::
 BandDepthRefData( const BandDepthRefData & bdRefData )
 :
 BandDepthData( bdRefData.nbPz(), bdRefData.nbPts(), bdRefData.leftOffset(), 
		bdRefData.rightOffset(), bdRefData.J(), bdRefData.verbosity(), bdRefData.readDataFromFile() )
 {   
    this->M_inputFilename = bdRefData.inputFilename();
    
    this->M_outputFilename = bdRefData.outputFilename();
    
    this->M_levelsExtremaFilename = bdRefData.levelsExtremaFilename();
    
    this->M_readLevelsExtremaFromFile = bdRefData.readLevelsExtremaFromFile();
 }
 
 // Redefinition of the showMe method for this derived class.
 void
 BandDepthRefData::
 showMe( std::ostream & output ) const
 {
   output << "--------------------------------" << std::endl;
   output << " ### BAND DEPTH REF DATA ### " << std::endl;

   output << " nbPz = " 			<< this->M_nbPz 		<< std::endl;
   output << " leftOffset = " 			<< this->M_leftOffset 		<< std::endl;
   output << " rightOffset = " 			<< this->M_rightOffset 		<< std::endl;
   output << " nbPts = " 			<< this->M_nbPts 		<< std::endl;
   output << " J = " 				<< this->M_J 			<< std::endl;
   output << " verbosity = " 			<< this->M_verbosity 		<< std::endl;
   output << " inputFilename = "		<< this->M_inputFilename 	<< std::endl;
   output << " outputFilename = "		<< this->M_outputFilename 	<< std::endl;
   output << " readDataFromFile = " 		<< this->M_readDataFromFile 	<< std::endl;
   output << " nbRefSamples = "			<< this->M_nbRefSamples		<< std::endl;
   output << " levelsExtremaFilename = "	<< this->M_levelsExtremaFilename << std::endl;
   output << " readLevelsExtremaFromFile = " 	<< this->M_readLevelsExtremaFromFile << std::endl;
   
   output << "---------------------------------" << std::endl;
    
   
    return;
 }
  
 
}