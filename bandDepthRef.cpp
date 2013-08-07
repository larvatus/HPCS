
#include "bandDepthRef.hpp"
#include <utility>

namespace HPCS
{
 ///////////////////////////
 //	BAND DEPTH REF
 //////////////////////////
  
 // Constructor from single variables.
 BandDepthRef::
 BandDepthRef( const bdData_Type & bdData )
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
    const UInt nbLevels = BandDepthRef::S_nbLevels;
   
    this->M_dataSetPtr.reset( new dataSet_Type( M_bdDataPtr->nbPz(), M_bdDataPtr->nbPts(), nbLevels ) );

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
      const UInt temp( static_cast< UInt >( this->M_dataSetPtr->cardinality( 0 ) * drand48() ) );
      
      if ( this->M_dataSetPtr->level( 0 ).find( temp ) != this->M_dataSetPtr->level( 0 ).end() )
      {
	this->M_referenceSetIDs.insert( std::pair< UInt, UInt >( temp, temp ) );	
      }
    }
   
    return;
 }
 

 
 
 
 
}