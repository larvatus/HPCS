
#include <bandDepthRef.hpp>

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
 BandDepthRef( const bdRefData_Type & bdRefData )
 {
    this->M_bdRefDataPtr.reset( new bdRefData_Type( bdRefData ) );
   
    this->M_mpiUtilPtr.reset( new mpiUtility_Type() );
   
    this->M_seed = 0;

    if ( bdRefData.readDataFromFile() ) this->readData();
    
    if ( bdRefData.readLevelsFromFile() ) this->readLevels();
   
 }

 // Method for reading data from input file
 void
 BandDepthRef::
 readData()
 {
    const UInt nbLevels = BandDepthRef::S_nbLevels;
   
    this->M_dataSetPtr.reset( new dataSet_Type( M_bdRefDataPtr->nbPz(), M_bdRefDataPtr->nbPts(), nbLevels ) );

    this->M_dataSetPtr->setOffset(  M_bdRefDataPtr->leftOffset(), M_bdRefDataPtr->rightOffset() );
    
    this->M_dataSetPtr->readData( M_bdRefDataPtr->inputFilename() );
   
    return;
 }
 
 // Method for reading levels from input file 
 void
 BandDepthRef::
 readLevels()
 {   
    this->M_dataSetPtr->readLevelsExtrema( M_bdRefDataPtr->levelsFilename() );    
    
    return;
 }
 
 // Method for resetting the Band Depth Data object
 void
 BandDepthRef::
 setBandDepthData( const bdRefData_Type & bdRefData )
 {
    this->M_bdRefDataPtr.reset( new bdRefData_Type( bdRefData ) );
    
    if ( bdRefData.readDataFromFile() ) this->readData();
    
    if ( bdRefData.readLevelsFromFile() ) this->readLevels();
   
    return;   
 }
 
  // Method for resetting the Band Depth Data object
//   void
//   BandDepthRef::
//   setBandDepthData( const bdDataPtr_Type & bdDataPtr )
//   {
//       //! @todo AGGIUNGI UN ASSERT STATICO SUL TIPO PUNTATO DA BDDATAPTR, CHE DEVE ESSERE
//       //! QUELLO DERIVATO BDDATAREF 
//     
//       this->M_bdRefDataPtr = bdDataPtr;
//       
//       if ( bdDataPtr->readDataFromFile() ) this->readData();
//       
//       if ( bdDataPtr->readLevelsFromFile() ) this->readLevels();
//     
//       return;   
//   } 
 
 // Reset dataSet pointer. This allow using the object without a file from which to read.
 void
 BandDepthRef::
 setDataSet( const dataSetPtr_Type & dataPtr )
 {
   assert( 
	  dataPtr->nbSamples() == this->M_bdRefDataPtr->nbPz() 
	  && 
	  dataPtr->nbPts() == this->M_bdRefDataPtr->nbPts() 
	 );
   
   assert( not( this->M_bdRefDataPtr->readDataFromFile() ) );
   
   assert( not( this->M_bdRefDataPtr->readLevelsFromFile() ) );
   
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
 setReferenceSet( const UInt & size, const UInt & referenceLevel )
 {
    assert( this->M_dataSetPtr->cardinality( referenceLevel ) >= size );
   
    this->M_referenceLevel = referenceLevel;
    
    srand48( this->M_seed );
      
    while( this->M_referenceSetIDs.size() != size )
    {
      const UInt temp( static_cast< UInt >( this->M_dataSetPtr->cardinality( this->M_referenceLevel ) * drand48() ) );
      
      if ( this->M_dataSetPtr->levelIDs( this->M_referenceLevel ).find( temp ) != this->M_dataSetPtr->levelIDs( this->M_referenceLevel ).end() )
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
    for( UInt iPz(0); iPz < this->M_bdRefDataPtr->nbPz(); ++iPz )
    {
      if ( this->M_referenceSetIDs.find( iPz ) == this->M_referenceSetIDs.end() )
      {
	this->M_testSetIDs.insert( iPz );	
      }
    }

    return;
 }
 
 // Getter of the reference set IDs.
 inline
 const
 BandDepthRef::IDContainer_Type &
 BandDepthRef::
 getReferenceSet() const
 {
    return this->M_referenceSetIDs;
 }
 
 // Getter of the test set IDs.
 inline
 const
 BandDepthRef::IDContainer_Type &
 BandDepthRef::
 getTestSet() const
 {
    return this->M_testSetIDs;
 }
 
 // Getter of the BDs
 inline
 void
 BandDepthRef::
 getBDs( std::vector< Real > & bds ) const
 {
    bds = this->M_BDs;
    
    return;
 }
 
 
// TODO FINISH ME!! 
void
BandDepthRef::
computeBDs()
{
/*   this->setTestSet();
   
   const UInt nbThreads( this->M_mpiUtilPtr->nbThreads() );
   
   const UInt myRank( this->M_mpiUtilPtr->myRank() );
   
   const UInt MASTER( this->M_mpiUtilPtr->master() );
   
           
   const UInt nbTestPz( static_cast< UInt >( this->M_testSetIDs.size() ) );
      
   const UInt slaveProcNbTestPz( static_cast< UInt >( nbTestPz / nbThreads ) );
   
   const UInt masterProcNbTestPz( static_cast< UInt >( nbTestPz / nbThreads ) + static_cast< UInt >( nbTestPz % nbThreads ) );
   
   const UInt verbosity( this->M_bdRefDataPtr->verbosity() );

   this->M_BDs.resize( nbTestPz );
   
   UInt nbMyPz;
   
   this->M_mpiUtilPtr->isMaster() ? nbMyPz = masterProcNbTestPz : nbMyPz = slaveProcNbTestPz;   
   
   IDContainer_Type::const_iterator it = this->M_testSetIDs.begin();

   // Advancing to my part of dataSet to process
   if ( not( this->M_mpiUtilPtr->isMaster() ) )
   {
     for( UInt iCount(0); iCount < masterProcNbTestPz + ( myRank - 1 ) * slaveProcNbTestPz; ++iCount )
     {
	++it;
     }
   }
   
   std::vector< UInt > subSetIDs( this->M_referenceSetIDs.size() + 1 );
   
   std::copy( this->M_referenceSetIDs.begin(), this->M_referenceSetIDs.end(), subSetIDs.begin() ); 
   
   typedef BandDepthData bdData_Type;
   
   typedef BandDepth::dataSet_Type dataSetSimple_Type;
   
   typedef BandDepth::dataSetPtr_Type dataSetSimplePtr_Type;
    
   this->M_bandDepthPtr.reset( new BandDepth( bdData_Type(  this->M_referenceSetIDs.size() + 1, this->M_bdRefDataPtr->nbPts(), 
						 0, 0, this->M_bdRefDataPtr->J(), 0, false )
					     )
			     );
   
   for ( UInt iPz(0); iPz < nbMyPz; ++iPz )
   {
      if( verbosity > 2 )  printf( "Proc %d is at %d / %d patients\n", myRank, iPz + 1, nbMyPz );   
      
      dataSetSimplePtr_Type dataSubSetPtr( new  dataSetSimple_Type( this->M_dataSetPtr->getRowSubSet( subSetIDs ) ) );
      
      this->M_bandDepthPtr->setDataSet( dataSubSetPtr );
      
      this->M_bandDepthPtr->computeBDs();
      
      this->M_BDs[ iPz ] = this->M_bandDepthPtr->getBDs()[ this->M_referenceSetIDs.size() ];
 
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
    */
}

void
BandDepthRef::
writeBDs() const
{
    if ( this->M_mpiUtilPtr->isMaster() );
    {
      std::ofstream output( this->M_bdRefDataPtr->outputFilename().data(), std::ios_base::out );
      
      for ( UInt iBD(0); iBD != this->M_bdRefDataPtr->nbPz(); ++iBD )
      {
	  output << iBD << " " << this->M_BDs[ iBD ] << std::endl;
      }
      
      output.close();
    }
    
    return;
}
 
 
 
}