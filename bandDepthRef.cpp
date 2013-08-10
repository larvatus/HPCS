
#include "bandDepthRef.hpp"
#include <utility>

namespace HPCS
{
 ///////////////////////////
 //	BAND DEPTH REF
 //////////////////////////
 
 // Stadard destructor
 BandDepthRef::
 ~BandDepthRef()
 {
 }
  
 // Constructor from single variables.
 BandDepthRef::
 BandDepthRef( const bdData_Type & bdData )
 {
    this->M_bdDataPtr.reset( new bdData_Type( bdData ) );
   
    this->M_mpiUtilPtr.reset( new mpiUtility_Type() );
   
    this->M_seed = 0;

    if ( bdData.readDataFromFile() ) this->readData();
   
 }

 // Method for reading data from input file and 
 void
 BandDepthRef::
 readData()
 {
    const UInt nbLevels = BandDepthRef::S_nbLevels;
   
    this->M_dataSetPtr.reset( new dataSet_Type( M_bdDataPtr->nbPz(), M_bdDataPtr->nbPts(), nbLevels ) );

    this->M_dataSetPtr->setOffset(  M_bdDataPtr->leftOffset(), M_bdDataPtr->rightOffset() );
    
    this->M_dataSetPtr->readData( M_bdDataPtr->inputFilename() );
   
    return;
 }
 
 // Method for resetting the Band Depth Data object
 void
 BandDepthRef::
 setBandDepthData( const bdData_Type & bdData )
 {
    this->M_bdDataPtr.reset( new bdData_Type( bdData ) );
    
    if ( bdData.readDataFromFile() ) this->readData();
   
    return;   
 }
 
 // Reset dataSet pointer. This allow using the object without a file from which to read.
 void
 BandDepthRef::
 setDataSet( const dataSetPtr_Type & dataPtr )
 {
   assert( 
	  dataPtr->nbSamples() == this->M_bdDataPtr->nbPz() 
	  && 
	  dataPtr->nbPts() == this->M_bdDataPtr->nbPts() 
	 );
   
   assert( not( this->M_bdDataPtr->readDataFromFile() ) );
   
   this->M_dataSetPtr = dataPtr;
   
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
    assert( this->M_bdDataPtr->nbPz() > size );
   
    srand48( this->M_seed );
      
    while( this->M_referenceSetIDs.size() != size )
    {
      const UInt temp( static_cast< UInt >( this->M_dataSetPtr->cardinality( 0 ) * drand48() ) );
      
      if ( this->M_dataSetPtr->levelIDs( 0 ).find( temp ) != this->M_dataSetPtr->levelIDs( 0 ).end() )
      {
	this->M_referenceSetIDs.insert( temp );	
      }
    }
   
    return;
 }
 
 
 void
 BandDepthRef::
 setTestSet()
 {
    for( UInt iPz(0); iPz < this->M_bdDataPtr->nbPz(); ++iPz )
    {
      if ( this->M_referenceSetIDs.find( iPz ) == this->M_referenceSetIDs.end() )
      {
	this->M_testSetIDs.insert( iPz );	
      }
    }

    return;
 }
 
 //! Getter of the reference set IDs.
 const
 BandDepthRef::IDContainer_Type &
 BandDepthRef::
 getReferenceSet() const
 {
    return this->M_referenceSetIDs;
 }
 
 //! Getter of the test set IDs.
 const
 BandDepthRef::IDContainer_Type &
 BandDepthRef::
 getTestSet() const
 {
    return this->M_testSetIDs;
 }
 
 //! Getter of the BDs
 const
 std::vector< Real > &
 BandDepthRef::
 getBDs() const
 {
    return this->M_BDs;
 }
 
 
//! TODO FINISH ME!! 
void
BandDepthRef::
computeBDs()
{
   this->setTestSet();
   
   const UInt nbThreads = this->M_mpiUtilPtr->nbThreads();
   
   const UInt myRank = this->M_mpiUtilPtr->myRank();
   
   const UInt MASTER = this->M_mpiUtilPtr->master();
   
           
   const UInt nbTestPz = static_cast< UInt >( this->M_testSetIDs.size() );
      
   const UInt slaveProcNbTestPz = static_cast< UInt >( nbTestPz / nbThreads );
   
   const UInt masterProcNbTestPz = static_cast< UInt >( nbTestPz / nbThreads ) + static_cast< UInt >( nbTestPz % nbThreads );

   this->M_BDs.resize( nbTestPz );
   
   
   const UInt verbosity = this->M_bdDataPtr->verbosity();
   
   UInt nbMyPz;
   
   this->M_mpiUtilPtr->isMaster() ? nbMyPz = masterProcNbTestPz : nbMyPz = slaveProcNbTestPz;   
   
   IDContainer_Type::const_iterator it = this->M_testSetIDs.begin();
   
   if ( not( this->M_mpiUtilPtr->isMaster() ) )
   {
     for( UInt iCount(0); iCount < masterProcNbTestPz + ( myRank - 1 ) * slaveProcNbTestPz; ++iCount )
     {
	++it;
     }
   }
   
   for ( UInt iPz(0); iPz < nbMyPz; ++iPz )
   {
//       const IDCurrent( *it );
   }
   
   
   
   
   
   
   
   

   
   // COMMUNICATING BAND DEPTHS
    
   for ( UInt iThread = 1; iThread < nbThreads; ++iThread )
   {
    if ( this->M_mpiUtilPtr->isMaster() )
    {
      MPI_Status status;

      MPI_Recv( & this->M_BDs[0] + masterProcNbTestPz + ( iThread - 1 ) * slaveProcNbTestPz, slaveProcNbTestPz, MPI_DOUBLE_PRECISION, iThread, iThread, MPI_COMM_WORLD, & status );
    }
    else if ( myRank == iThread )
    {
      MPI_Send( & this->M_BDs[0], slaveProcNbTestPz, MPI_DOUBLE_PRECISION, MASTER, myRank, MPI_COMM_WORLD );
    }
   }
  
   if ( verbosity > 2 && this->M_mpiUtilPtr->isMaster() ) 
      
     printf( " All depths have been gathered\n" );   
 
   MPI_Barrier( MPI_COMM_WORLD );

    return;
}
 
 
void
BandDepthRef::
writeBDs() const
{
    if ( this->M_mpiUtilPtr->isMaster() );
    {
      std::ofstream output( this->M_bdDataPtr->outputFilename().data(), std::ios_base::out );
      
      for ( UInt iBD(0); iBD != this->M_bdDataPtr->nbPz(); ++iBD )
      {
	  output << iBD << " " << this->M_BDs[ iBD ] << std::endl;
      }
      
      output.close();
    }
    
    return;
}
 
 
 
}