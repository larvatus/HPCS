
#ifndef __BD_HPP__
#define __BD_HPP__

#include "dataSet.hpp"
#include "combinationFactory.hpp"
#include "mpi_utility.hpp"
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

typedef unsigned int UInt;
typedef double Real;
  
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
  BandDepthData( const UInt & nbPz, const UInt & nbPts, const UInt & leftOffset, const UInt & rightOffset, const UInt & J, const UInt & verbosity );
  
  //! Constructor from a Get Pot variable
  /*!
   * \param dataFile Get Pot object
   * \param section Related section containing information
   */
  BandDepthData( const data_Type & dataFile, const std::string & section ); 
 
  //! Copy constructor
  BandDepthData( const BandDepthData & bddata );
  
  //! Standard destructor
  ~BandDepthData(){}
  
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
  
  typedef BandDepthData bdData_Type;
  typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
  
  typedef DataSet dataSet_Type;
  typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
  
  typedef CombinationFactory combinationFactory_Type;
  typedef combinationFactory_Type::tuple_Type tuple_Type;
  
  typedef mpiUtility mpiUtility_Type;
  typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
  
  //@}
   
  //! @name Constructor & Destructor
  //@{
    
  //! Constructor from a BandDepthData object
  BandDepth( const bdData_Type & bdData );
  
  ~BandDepth(){};

  //@}
  
  //! @name Public Methods
  //@{

    
  //! Method for the computation of BDs. Computation is made in parallel exploiting MPI.
  void computeBDs();
  
  //! Method for the printing of computed BDs.
  /*!
   * The computed BDs are written by MASTER processor
   * on the file specified by BandDepthData object.
   */
  void writeBDs() const;
  
  //! Method for resetting the BandDepthData object.
  /*!
   * It enables the re-use of the BandDepth object in order to compute other bandDepths, e.g. for
   * other input files. This is useful when a class contains a pointer to a BandDepth object that
   * may be used over different datasets.
   * 
   * \param bdData New BandDepthData object that will replace the old one.
   */
  void resetBandDepthData( const bdData_Type & bdData );
  
  //@}
  
private:
  
  //! @name Private methods
  //@{

  //! Each thread reads data from data files.
  void readData();
    
  //@}
  

  //! @name Private members
  //@{
  
  //! Shared pointer to a BandDepthData type object
  bdDataPtr_Type M_bdDataPtr;
  
  //! Shared pointer to a dataSet type object, containing data.
  dataSetPtr_Type M_dataSetPtr;
  
  //! Computed band depths
  std::vector< Real > M_BDs;
  
  //! MPI utility pointer object
  mpiUtilityPtr_Type M_mpiUtilPtr;
  
  //@}
    
};

UInt binomial( const UInt & N , const UInt & K );

}

#endif
