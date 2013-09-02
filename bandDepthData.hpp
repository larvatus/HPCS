

#ifndef __BANDDEPTH_DATA_HPP__
#define __BANDDEPTH_DATA_HPP__

#include <dataSet.hpp>

#include <HPCSDefs.hpp>

namespace HPCS
{ 
    
/*!
 * This class implements a data object for the setup of a BandDepth type object.
 * 
 */

class BandDepthData
{
  
public:
  
  typedef unsigned int UInt;
  
  typedef GetPot data_Type;
  
  //! Constructor from single specified variables
  /*!
   * \param nbPz Number of patients, i.e. number of samples
   * \param nbPts Number of points for each functional data in the input file (i.e. including right and left offset blocks )
   * \param leftOffset Size of possible left offset block to be jumped when reading input file
   * \param rightOffset Size of possible right offset block to be jumped when reading input file
   * \param J Parameter specifying the tuple size for the computation of band depths
   * \param verbosity Variable specifying the level of verbosity
   */
  BandDepthData( const UInt & nbPz, const UInt & nbPts, const UInt & leftOffset, const UInt & rightOffset, const UInt & J, const UInt & verbosity, const bool & readDataFromFile = false );
  
  //! Constructor from a Get Pot variable
  /*!
   * \param dataFile Get Pot object
   * \param section Related section containing information
   */
  BandDepthData( const data_Type & dataFile, const std::string & section ); 
 
  //! Copy constructor
  BandDepthData( const BandDepthData & bddata );
  
  //! Standard destructor
  virtual ~BandDepthData(){}
  
  //! @name Setters & Getters 
  //@{

  //! Method for setting up the input filename, containing data to be read.
  void setInputFilename( const std::string & inputFilename );
  
  //! Method for setting up the output filename, containing the computed band depths.
  void setOutputFilename( const std::string & outputFilename );  
   
  UInt nbPz() const { return this->M_nbPz; }

  UInt nbPts() const { return this->M_nbPts; }
  
  UInt leftOffset() const { return this->M_leftOffset; }
  
  UInt rightOffset() const { return this->M_rightOffset; }
  
  UInt J() const { return this->M_J; }
  
  UInt verbosity() const { return this->M_verbosity; }
  
  bool readDataFromFile() const { return this->M_readDataFromFile; }
  
  std::string outputFilename() const { return this->M_outputFilename; }
  
  std::string inputFilename() const { return this->M_inputFilename; }
  
  virtual void showMe( std::ostream & output = std::cout ) const;
  
  //@}
  
protected:
  
  UInt M_nbPz;
  
  UInt M_nbPts;
  
  UInt M_leftOffset;
  
  UInt M_rightOffset;
  
  UInt M_J;
  
  UInt M_verbosity;
  
  bool M_readDataFromFile;
  
  std::string M_outputFilename;
  
  std::string M_inputFilename;
  
};

/*!
 * This class implements a data object for the setup of a BandDepthRef type object.
 * 
 * TODO Is it a kind of "decorator" of BandDepthData?
 * 
 */

class BandDepthRefData : public BandDepthData
{
  
public:
  
  typedef unsigned int UInt;
  
  typedef GetPot data_Type;
  
  //! Constructor from single specified variables
  /*!
   * \param nbPz Number of patients, i.e. number of samples
   * \param nbPts Number of points for each functional data in the input file (i.e. including right and left offset blocks )
   * \param leftOffset Size of possible left offset block to be jumped when reading input file
   * \param rightOffset Size of possible right offset block to be jumped when reading input file
   * \param J Parameter specifying the tuple size for the computation of band depths
   * \param verbosity Variable specifying the level of verbosity
   */
  BandDepthRefData( const UInt & nbPz, const UInt & nbPts, const UInt & leftOffset, const UInt & rightOffset, 
		    const UInt & J, const UInt & verbosity, const bool & readDataFromFile = false, 
		    const bool & readLevelsFromFile = false  );
  
  //! Constructor from a Get Pot variable
  /*!
   * \param dataFile Get Pot object
   * \param section Related section containing information
   */
  BandDepthRefData( const data_Type & dataFile, const std::string & section ); 
 
  //! Copy constructor
  BandDepthRefData( const BandDepthRefData & bddata );
  
  //! Standard destructor
  virtual ~BandDepthRefData(){}
  
  //! @name Setters & Getters 
  //@{

  //! Method for setting up the file name containing the extrema to be used to build dataSetLevelled.
  void setLevelsExtremaFilename( const std::string & inputFilename );
  
  //! Method for resetting the number of Reference samples.
  void setNbReferenceSamples( const UInt & nbRefSamples );
  
  //! Getter of the levels filename
  std::string levelsExtremaFilename() const;
  
  //! Do I read levels extrema from file?
  bool readLevelsExtremaFromFile() const { return this->M_readLevelsExtremaFromFile; }
  
  //! Getter for the number of reference samples
  UInt nbReferenceSamples() const;
  
  //! Getter for the number of test samples (i.e. all the non-reference samples )
  UInt nbTestSamples() const;
  
  virtual void showMe( std::ostream & output = std::cout ) const;
  
  //@}
  
protected:

  UInt M_nbRefSamples;
  
  std::string M_levelsExtremaFilename;
  
  bool M_readLevelsExtremaFromFile;
  
private:
  
  
};

}

#endif /* __BANDDEPTH_DATA_HPP__ */