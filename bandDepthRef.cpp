
#include "bandDepthRef.hpp"
#include <utility>

namespace HPCS
{
 ///////////////////////////
 //	BAND DEPTH REF
 //////////////////////////
  
 // Constructor from single variables.
 BandDepthRef::
 BandDepthRefs( const bdData_Type & bdData )
 :
 {
    this->M_bdDataPtr.reset( new bdData_Type( bdData ) );
   
    this->M_mpiUtilPtr.reset( new mpiUtility_Type() );
   
    this->M_seed = 0;
    
    this->readData();
   
 }

 // Method for reading data from input file and 
 void
 BandDepthRef::
 readData()
 {
    this->M_dataSetPtr.reset( new dataSet_Type( M_bdDataPtr->nbPz(), M_bdDataPtr->nbPts() ) );

    this->M_dataSetPtr->setOffset(  M_bdDataPtr->leftOffset(), M_bdDataPtr->rightOffset() );
    
    this->M_dataSetPtr->readCSV( M_bdDataPtr->inputFilename() );
   
    return;
 }
 
 // Method for the setup of the seed for the pseudo-random number generator
 void
 BandDepthRef::
 setSeed( const UInt & seed )
 {
    this->M_seed = seed;
   
    return;
 }
 
 //
 void
 BandDepthRef::
 setReferenceSet( const UInt & size )
 {
    srand48( this->M_seed );
      
    while( this->M_referenceSetIDs.size() != this->M_bdDataPtr->nbPz() )
    {
      UInt temp( static_cast< UInt >( this->M_dataPtr->cardinality[0] * drand48() ) );
      
      std::pair< UInt, UInt > tempPair( temp, temp );
      
      if ( this->M_dataPtr->level[0].find( tempPair ) != this->M_dataPtr->level[0].end() )
      {
	referenceSetIDs.insert( std::pair< UInt, UInt >( temp, temp ) );	
      }
    }
   
    return;
 }
 

 
 
 
 
}