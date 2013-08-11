
#ifndef __BD_HPP__
#define __BD_HPP__

#include "dataSet.hpp"
#include "bandDepthData.hpp"
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
  
  //! Constructor from a shared pointer to BandDepthData object
  BandDepth( const bdDataPtr_Type & bdDataPtr );
  
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
  
  //! Getter of the Band Depths
  const std::vector< Real > & getBDs() const; 
  
  //! Method for resetting the BandDepthData object.
  /*!
   * It enables the re-use of the BandDepth object in order to compute other bandDepths, e.g. for
   * other input files. This is useful when a class contains a pointer to a BandDepth object that
   * may be used over different datasets.
   * 
   * \param bdData New BandDepthData object that will replace the old one.
   */
  void setBandDepthData( const bdData_Type & bdData );
  
  
  //! Method for the setting up or the resetting of the dataSet.
  /*!
   * The purpose of this method is to break the dependency of the object on
   * a specific data file containing data. With can set up (or reset) the data
   * constituting the data set, if the "dimensions" agree with those expressed by
   * band depth data object.
   * This use is meant to pair with a constructor taking a Band Depth Data with no
   * input filename, and thus M_readDataFromFile flag equal to false.
   * 
   * \param dataPtr Pointer to the new dataSet
   */
  void setDataSet( const dataSetPtr_Type & dataPtr );  
  
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
