


/*!
 *   @file BandDepthData.hpp
     @brief Declaration of BandDepthData classes.

     @date 08/2013
     @author Nicholas Tarabelloni  <nicholas.tarabelloni@gmail.com>
     @maintainer Nicholas Tarabelloni  <nicholas.tarabelloni@gmail.com>
*/



#ifndef __BANDDEPTH_DATA_HPP__
#define __BANDDEPTH_DATA_HPP__

#include <source/dataSet.hpp>

#include <source/HPCSDefs.hpp>


namespace HPCS
{ 
  
//! @class BandDepthData this class Implements data objects encapsulating information about the Band Depth.
/*!
 * 
 * This class builds data objects encapsulating information about the Band Depth to be computed, and is a base class of BandDepthRefData.
 * There are two constructor, one that is direct and sets the specified variables by explicitly receiving their values, and the other
 * that takes a GetPot object to parse an external data files, specifying the variables. The latter is very useful if paired with 
 * a run-time construction of BandDepth ojects through a factory-wise creation.
 * This is the data object for Band Depth "All" objects, i.e. objects performing the computation of depth measures testing each signal
 * against all the others.
 */
class BandDepthData
{
  
public:
  
  
  //! @name Public Types
  //@{
  
  //! Get Pot objects typedef  
  typedef GetPot data_Type;
  
  //@}
  
  //! @name Constructors & Destructor
  //@{
    
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
  
  //@}
  
  //! @name Setters & Getters 
  //@{

  //! Method for setting up the input filename, containing the raw (numeric) data to be read.
  void setInputFilename( const std::string & inputFilename );
  
  //! Method for setting up the output filename, that will contain the computed band depths.
  void setOutputFilename( const std::string & outputFilename );  
   
  //! Getter of the number of patients, i.e. samples.
  UInt nbPz() const { return this->M_nbPz; }

  //! Getter of the number of points of each functional data
  UInt nbPts() const { return this->M_nbPts; }
  
  //! Unified common interface to get the number of testing samples, i.e. the samples for which the Band Depth will be computed.
  virtual UInt nbTestSamples() const { return this->M_nbPz; }
  
  //! Getter of the leftOffset variable
  UInt leftOffset() const { return this->M_leftOffset; }
  
  //! Getter of the rightOffset variable
  UInt rightOffset() const { return this->M_rightOffset; }
  
  //! Getter of the J variable
  UInt J() const { return this->M_J; }
  
  //! Getter of the verbosity level
  UInt verbosity() const { return this->M_verbosity; }
  
  //! Getter of the flag specifying whether data are to be read from external file or not.
  bool readDataFromFile() const { return this->M_readDataFromFile; }
  
   //! Getter of the input filemane from which, if specified,  data are to be read
  std::string inputFilename() const { return this->M_inputFilename; }
  
  //! Getter of the output filename in which results will be printed.
  std::string outputFilename() const { return this->M_outputFilename; }
  
  //@}
  
  //! @name Misc. public methods
  //@{
    
 /*!
  *  ShowMe methods, prompting the status of the BandDepthObject. The standard argument is std::cout. 
  *  It may be overridden in the derived classes.
  */
  virtual void showMe( std::ostream & output = std::cout ) const;
  
  //@}
  
protected:
  
  //! @name Protected members
  //@{
    
  //! Number of samples of the dataset
  UInt M_nbPz;
  
  //! Number of points constituting each functional data
  UInt M_nbPts;
  
  //! Number of left offset characters at each row of dataset when reading data from file.
  UInt M_leftOffset;
  
  //! Number of right offset characters at each row of dataset when reading data from file.
  UInt M_rightOffset;
  
  //! J parameter (see report)
  UInt M_J;
  
  //! Verbosity level
  UInt M_verbosity;
  
  //! Flag specifying whether data are to be read from external file or not.
  bool M_readDataFromFile;
  
  //! Output filename in which results will be printed.
  std::string M_outputFilename;
  
  //! Input filemane from which, if specified,  data are to be read
  std::string M_inputFilename;
  
  //@}
  
};

//! @class BandDepthRefData Class derived from BandDepthData adapted to the setup of Band Depth Reference data.
/*!
 * This class implements a data object for the setup of a Band Depth objects performing the computation of depth measure in
 * a "Reference" fashion, i.e. splitting the dataset into a reference set and a test set. Each signal in the test set is
 * tested against the whole reference set, which determines its depth measure. 
 */
class BandDepthRefData : public BandDepthData
{
  
public:
  
  //!@name Public Typedefs
  //@{
  
  //! Get Pot typedef
  typedef GetPot data_Type;
  
  //@}
  
  //! @name Constructors & Destructors
  //@{
    
  //! Constructor from single specified variables, allowing public interface compatibility with the base class.
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
  
  //@}
  
  //! @name Setters & Getters 
  //@{

  //! Method for setting up the file name containing the extrema to be used to build dataSetLevelled.
  void setLevelsExtremaFilename( const std::string & inputFilename );
  
  //! Method for resetting the number of Reference samples.
  void setNbReferenceSamples( const UInt & nbRefSamples );
  
  //! Getter of the levels filename
  std::string levelsExtremaFilename() const;
  
  //! Getter of the flag specifying whether levels are to be read from an external file.
  bool readLevelsExtremaFromFile() const { return this->M_readLevelsExtremaFromFile; }
  
  //! Getter for the number of reference samples
  UInt nbReferenceSamples() const;
  
  //! Getter for the number of test samples (i.e. all the non-reference samples )
  virtual UInt nbTestSamples() const;
  
  //@}
  
  //! @name Misc. public methods
  //@{
  
  //! ShowMe method which prints the internal status of the object. It overrides the corresponding base class method.
  virtual void showMe( std::ostream & output = std::cout ) const;
  
  //@}
  
protected:

  //! @name Protected Members
  //@{
  
  //! Number of samples constituting the reference set
  UInt M_nbRefSamples;
  
  //! Variable containing the filename specifying the extrema IDs of the levels in the file.
  std::string M_levelsExtremaFilename;
  
  //! Flag specifying whether levels are to be read from an external file.
  bool M_readLevelsExtremaFromFile;
  
  //@}
  
private:
  
  
};

}

#endif /* __BANDDEPTH_DATA_HPP__ */