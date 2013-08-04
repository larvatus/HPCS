
#ifndef __BD_HPP__
#define __BD_HPP__

#include "dataSet.hpp"
#include "GetPot"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

/*!
    @file BandDepth.hpp
    @brief

    @author Nicholas Tarabelloni
    @date 04 Aug 2013
    
    @mantainer Nicholas Tarabelloni
 */

namespace HPCS
{
  
/*!
 * This class implements a data object for the setup of a BandDepth type object.
 * 
 */

class BDData
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
  BDData( const UInt & nbPz, const UInt & nbPts, const UInt & leftOffset, const UInt & rightOffset, const UInt & J, const UInt & verbosity );
  
  //! Constructor from a Get Pot variable
  /*!
   * \param dataFile Get Pot object
   * \param section Related section containing information
   */
  BDData( const data_Type & dataFile, const std::string & section ); 
 
  //! Copy constructor
  BDData( const BDData & bddata );
  
  //! Standard destructor
  ~BDData(){}
  
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
  
  std::string outputFilename() const { return this->M_outputFilename; }
  
  std::string inputFilename() const { return this->M_inputFilename; }
  
  //@}
  
private:
  
  UInt M_nbPz;
  
  UInt M_nbPts;
  
  UInt M_leftOffset;
  
  UInt M_rightOffset;
  
  UInt M_J;
  
  UInt M_verbosity;
  
  std::string M_outputFilename;
  
  std::string M_inputFilename;
  
};
  

/*!
 * This class implements an interface for the fast computation of Band Depths for functional data.
 * 
 */  
  
class BandDepth
{
public:
  
  //! @name Types definitions
  //@{
    
  typedef double Real;
  typedef GetPot inputData_Type;
  typedef unsigned int UInt;
  typedef BDData bdData_Type;
  typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
  typedef DataSet dataSet_Type;
  typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
  
  //@}
   
  //! @name Constructor & Destructor
  //@{
    
  //! Constructor from a BDData object
  BandDepth( const bdData_Type & bdData );
  
  //! Constructor from a Get Pot object
  /*!
   * It is an intermediate interface for the use of BDData object constructor.
   * 
   * \param dataFile Get Pot object
   * \param section  Related section containing information
   */
  BandDepth( const inputData_Type & dataFile, const std::string & section  );
  
  ~BandDepth(){};

  //@}
  
  //! @name Public Methods
  //@{
    
  void compute();
  
  //! Method for resetting the BDData object.
  /*!
   * It enables the re-use of the BandDepth object in order to compute other bandDepths, e.g. for
   * other input files. This is useful when a class contains a pointer to a BandDepth object that
   * may be used over different datasets.
   * 
   * \param bdData New BDData object that will replace the old one.
   */
  void resetBDData( const bdData_Type & bdData );
  
  //@}
  
private:
  
  //! @name Private methods
  //@{

  //! Each thread reads data from data files.
  void readData();
    
  //@}
  

  //! @name Private members
  //@{
  
  //! Shared pointer to a BDData type object
  bdDataPtr_Type M_bdDataPtr;
  
  //! Shared pointer to a dataSet type object, containing data.
  dataSetPtr_Type M_dataSetPtr;
  
  //! Computed band depths
  std::vector< Real > M_BDs;
  
  //@}
    
};


}

#endif
