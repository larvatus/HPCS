

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
    
    typedef DataSet dataSet_Type;
    
    typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
    
    typedef mpiUtility mpiUtility_Type;
    
    typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
    
    typedef bandDepth bandDepth_Type;
    
    typedef boost::shared_ptr< bandDepth_Type > bandDepthPtr_Type;
    
    typedef std::map< UInt, UInt > IDContainer_Type;
    
    typedef boost::shared_ptr< IDContainer_Type > IDContainerPtr_Type;
    
    //@}
    
    //! @name Constructors and Destructor
    //@{
    
    BandDepthRef( const bdData_Type & bdData );
	
    ~BandDepthRef();
  
    //@}

    //! @name Public methods
    //@{
	
    void compute();
	
    void writeBDs();
	  
    //@}
     
     
  private:
    
    //! @name Private methods
    //@{

    //! Each thread reads data from data files.
    void readData();
 
     //! It allows to set a particular seed for the initialisation of the pseudo-random number generator.
    void setSeed( const UInt & seed );
    
    //! Method launching a pseudo-random number generators determining the initial IDs of reference set (referenceSetIDs)
    void setReferenceSet( const UInt & size );
    
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
    IDContainer_Type testSetIDs;
    
    //! Vector containing the IDs of the reference set
    IDContainer_Type referenceSetIDs;
    
    //@}
    
    
  }
  
  
}



#endif