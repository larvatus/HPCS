
#include <mpi.h>

#include "bandDepth.hpp"
#include <cassert>
#include <fstream>

namespace HPCS
{

 /////////////////////////
 // 	Band Depth
 /////////////////////////
 
 // Constructor from BandDepthData type object
 BandDepth::
 BandDepth( const bdData_Type & bdData )
 :
 M_bdDataPtr( new bdData_Type( bdData ) ),
 M_mpiUtilPtr( new mpiUtility_Type() )
 {   
   if( bdData.readDataFromFile() ) this->readData();
 }
 
 BandDepth::
 BandDepth( const bdDataPtr_Type & bdDataPtr )
 :
 M_bdDataPtr( bdDataPtr ),
 M_mpiUtilPtr( new mpiUtility_Type() )
 {      
   if( this->M_bdDataPtr->readDataFromFile() ) this->readData();
 }
  
 // Reset BandDepthData type object contained 
 void 
 BandDepth::
 setBandDepthData( const bdData_Type & bdData )
 {
    this->M_bdDataPtr.reset( new bdData_Type( bdData ) );
    
    if ( bdData.readDataFromFile() ) this->readData();
 }
 
 // Reset dataSet pointer. This allow using the object without a file from which to read.
 void
 BandDepth::
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
 
 // Each proces reads data from data files.
 void 
 BandDepth::
 readData()
 {
   this->M_dataSetPtr.reset( new dataSet_Type( M_bdDataPtr->nbPz(), M_bdDataPtr->nbPts() ) );
   
   this->M_dataSetPtr->setOffset( M_bdDataPtr->leftOffset(), M_bdDataPtr->rightOffset() );
   
   this->M_dataSetPtr->readData( M_bdDataPtr->inputFilename() );
   
   return;
 }
 
 // Method for the computation of BDs
 void
 BandDepth::
 computeBDs()
 {
   const UInt myRank = this->M_mpiUtilPtr->myRank();
   const UInt nbThreads = this->M_mpiUtilPtr->nbThreads();
   
   const UInt MASTER = this->M_mpiUtilPtr->master();
   
   const UInt nbPz 	= this->M_bdDataPtr->nbPz();
   const UInt nbPts	= this->M_bdDataPtr->nbPts();
   const UInt J	   	= this->M_bdDataPtr->J();
   const UInt verbosity = this->M_bdDataPtr->verbosity();
   
   typedef dataSet_Type::dataPtr_Type dataSetPtr_Type;
   
   const dataSetPtr_Type dataPtr( this->M_dataSetPtr->getData() );  
            
   const UInt slaveProcNbPz = static_cast< UInt >( nbPz / nbThreads );
   const UInt masterProcNbPz = static_cast< UInt >( nbPz / nbThreads ) + static_cast< UInt >( nbPz % nbThreads );
   
   this->M_BDs.resize( nbPz );
   
   UInt nbMyPz;
   
   this->M_mpiUtilPtr->isMaster() ? nbMyPz = masterProcNbPz : nbMyPz = slaveProcNbPz;
   
   combinationFactory_Type combinationFactory( nbPz - 1, J );
   
   combinationFactory.generateCombinations();
   
   for ( UInt iPz(0); iPz < nbMyPz; ++iPz )
   {
      if( verbosity > 2 ) printf( "Proc %d is at %d / %d patients\n", myRank, iPz+1, nbMyPz );
	  
      UInt globalPzID;
     
      this->M_mpiUtilPtr->isMaster() ? globalPzID = iPz : globalPzID = masterProcNbPz  + ( myRank - 1 ) * slaveProcNbPz + iPz;
            
      Real comprisedCounter(0);
      
      combinationFactory.resetPointerToHeadCombination();      
      
      while( not( combinationFactory.hasTraversedAll() ) )
      {
	tuple_Type pzTupleIDs;

	combinationFactory.getNextCombination( pzTupleIDs );

	for ( UInt iJ(0); iJ < J; ++iJ )
	{	  
	   if ( pzTupleIDs[ iJ ] >= globalPzID ) 
	   {
	       ++pzTupleIDs[ iJ ];
	   }	  
	}
	
	std::vector< Real > currentValues( J );
	
	Real envMax, envMin, pzVal;
	
	for ( UInt iPt(0); iPt < nbPts; ++iPt )
	{
	  for ( UInt iJ(0); iJ < J; ++iJ )
	  {		
	    currentValues[ iJ ] = (*dataPtr)( pzTupleIDs[ iJ ], iPt );
	  } 
	  
	  envMax =  *( std::max_element( currentValues.begin(), currentValues.end() ) );
	  envMin =  *( std::min_element( currentValues.begin(), currentValues.end() ) );
	  pzVal = (*dataPtr)( globalPzID, iPt );
	  
	  if ( pzVal <= envMax && pzVal >= envMin ) ++comprisedCounter;  
	  
	}
      }

	this-> M_BDs[ iPz ] = comprisedCounter / static_cast< Real > ( ( nbPts - 1 ) * binomial( nbPz - 1, J ) );
  }	    
 
  // COMMUNICATING BAND DEPTHS
    
  for ( UInt iThread = 1; iThread < nbThreads; ++iThread )
  {
    if ( this->M_mpiUtilPtr->isMaster() )
    {
      MPI_Status status;

      MPI_Recv( & this->M_BDs[0] + masterProcNbPz + ( iThread - 1 ) * slaveProcNbPz, slaveProcNbPz, MPI_DOUBLE_PRECISION, iThread, iThread, MPI_COMM_WORLD, & status );
    }
    else if ( myRank == iThread )
    {
      MPI_Send( & this->M_BDs[0], slaveProcNbPz, MPI_DOUBLE_PRECISION, MASTER, myRank, MPI_COMM_WORLD );
    }
  }
  
  if ( verbosity > 2 && this->M_mpiUtilPtr->isMaster() ) 

    printf( " All depths have been gathered\n" );
  
   return;
  
} 

void
BandDepth::
writeBDs() const
{    
    if ( this->M_mpiUtilPtr->isMaster() )
    {
      std::ofstream output( this->M_bdDataPtr->outputFilename().data(), std::ios_base::out );
      
      for( UInt iPz(0); iPz < this->M_bdDataPtr->nbPz(); ++iPz )
      {
	  output << iPz << " " << this->M_BDs[ iPz ] << std::endl;
      }
      
      output.close();
    }
  
    return;
}

const
std::vector< Real > &
BandDepth::
getBDs() const
{
    return this->M_BDs;
}


UInt binomial( const UInt & N , const UInt & K )
{    
    UInt num( 1 );
    UInt denom( 1 );
    
    for ( UInt iK(0); iK < K; ++iK )
    {
	num *= N - iK;
	denom *= iK + 1;
    }
  
   return static_cast< UInt >( num/denom );
}

   
}