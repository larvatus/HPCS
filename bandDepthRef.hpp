

#ifndef __BANDDEPTH_REF_HPP__
#define __BANDDEPTH_REF_HPP__

#include "bandDepth.hpp"
#include "dataSet.hpp"
#include <boost/shared_ptr.hpp>
#include "GetPot"
#include <string>
#include <vector>
#include <map>

namespace HPCS
{
  class BandDepthRef
  {
  public:
    
    //! @name Typedefs
    //@{
    
    typedef unsigned int UInt;	
    
    typedef double Real;
    
    typedef GetPot data_Type;
    
    typedef BandDepthData bdData_Type;
    
    typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
    
    typedef DataSetLevelled dataSet_Type;
    
    typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
    
    typedef mpiUtility mpiUtility_Type;
    
    typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
    
    typedef BandDepth bandDepth_Type;
    
    typedef boost::shared_ptr< bandDepth_Type > bandDepthPtr_Type;
    
    typedef std::set< UInt, UInt > IDContainer_Type;
    
    typedef boost::shared_ptr< IDContainer_Type > IDContainerPtr_Type;
    
    //@}
    
    //! @name Constructors and Destructor
    //@{
    
    BandDepthRef( const bdData_Type & bdData );
	
    ~BandDepthRef();
  
    //@}

    //! @name Generic public methods
    //@{
      
    //! The method for the computation of Band Depths, executing the computation in parallel.
    void compute();
    
    //@}

    //! @name Public Setters & Getters
    //@{

    //! It allows to set a particular seed for the initialisation of the pseudo-random number generator.
    void setSeed( const UInt & seed );

    //! Method for resetting the BandDepthData object.
    /*!
    * It enables the re-use of the BandDepthRef object in order to compute other bandDepths, e.g. for
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
    * \param dataPtr Pointer to the new dataSet.
    */
    void setDataSet( const dataSetPtr_Type & dataPtr ); 
    
    //! Method for choosing the reference set IDs in the population. 
    /*! It launches a pseudo-random number generators determining the 
    *  initial IDs of reference set (referenceSetIDs) of size specified
    *  by "size" variable.
    */
    void setReferenceSet( const UInt & size );
      
    //! Getter of the reference set.
    const IDContainer_Type & getReferenceSet() const;
    
    //! Getter of the test set.
    const IDContainer_Type & getTestSet() const;

    //! Getter of the computed BDs
    const std::vector< Real > & getBDs() const;
    
    //! The method writing the BDs to the file name specified inside the BD Data Object.
    void writeBDs() const;
    
    //@}
     
  private:
    
    //! @name Private methods
    //@{

    //! Each thread reads data from data files.
    void readData();
        
    //! Method allowing to determine the remaining part of dataSet other than the reference set, i.e. the "test set".
    void setTestSet();
    
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
    
    //! Shared pointer to a bandDepth object, used as a tool to compute full bandDepths on reduced parts of data set.
    bandDepthPtr_Type M_bandDepthPtr;
    
    //! Seed for the pseudo-random number generator
    UInt M_seed;
    
    //! Vector containing the IDs of the test set
    IDContainer_Type M_testSetIDs;
    
    //! Vector containing the IDs of the reference set
    IDContainer_Type M_referenceSetIDs;
    
    //! Number of levels
    static const UInt S_nbLevels = 2 ;
    
    //@}
    
    
  };
  
  
}



#endif