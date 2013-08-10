
#include <mpi.h>

#include "bandDepth.hpp"
#include "dataSet.hpp"
#include <cassert>
#include <fstream>

namespace HPCS
{
  
 ///////////////////////////
 //	BD DATA
 //////////////////////////
  
 // Constructor from single variables.
 BandDepthData::
 BandDepthData( const UInt & nbPz, const UInt & nbPts, const UInt & leftOffset, 
	 const UInt & rightOffset, const UInt & J, const UInt & verbosity,
	 const bool & readDataFromFile )
 :
 M_nbPz( nbPz ),
 M_nbPts( nbPts ),
 M_leftOffset( leftOffset ),
 M_rightOffset( rightOffset ),
 M_J( J ),
 M_verbosity( verbosity ),
 M_readDataFromFile( readDataFromFile )
 {}
  
 // Constructor from Get Pot type object
 BandDepthData::
 BandDepthData( const data_Type & dataFile, const std::string & section )
 {
    const std::string baseName( section );
      
    this->M_nbPz =  dataFile( ( baseName + "/nbPz"  ).data(), 10 );
      
    const UInt nbPtsFull = dataFile( ( baseName + "/nbPts" ).data(), 100 );
      
    this->M_leftOffset = dataFile( ( baseName + "/leftOffset" ).data(), 0 );
      
    this->M_rightOffset = dataFile( ( baseName + "/rightOffset" ).data(), 0 );
      
    this->M_nbPts = nbPtsFull - M_leftOffset - M_rightOffset;
      
    this->M_J = dataFile( ( baseName + "/J" ).data(), 2 );

    this->M_verbosity = dataFile( ( baseName + "/verbosity" ).data(), false );
      
    this->M_inputFilename = dataFile( ( baseName + "/inputFilename" ).data(), "data.dat");
    
    this->M_readDataFromFile = true;
    
    this->M_outputFilename = dataFile( ( baseName + "/outputFilename" ).data(), "bd.dat" );
      
 }
 
 // Copy constructor
 BandDepthData::
 BandDepthData( const BandDepthData & bdData )
 {
    this->M_nbPz = bdData.nbPz();   
    
    this->M_leftOffset = bdData.leftOffset();
    
    this->M_rightOffset = bdData.leftOffset();
    
    this->M_nbPts = bdData.nbPts();
    
    this->M_J = bdData.J();
    
    this->M_verbosity = bdData.verbosity();
    
    this->M_inputFilename = bdData.inputFilename();
    
    this->M_outputFilename = bdData.outputFilename();
    
    this->M_readDataFromFile = bdData.readDataFromFile();
 }
 
 // Setter for the output filename
 void 
 BandDepthData::
 setInputFilename( const std::string & inputFilename )
 {
    this->M_inputFilename = inputFilename;
    
    this->M_readDataFromFile = true;
    
    return;
 }
 
 // Setter for the input filename  
 void 
 BandDepthData::
 setOutputFilename( const std::string & outputFilename )
 {
    this->M_outputFilename = outputFilename;
    
    return;
 }

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
      
      // IMPORTANT: I leave one patient out, so the actual N in the binomial coefficient is nbPz - 1
      //combinationFactory_Type combinationFactory( nbPz - 1, J );
	
      this->M_BDs[ iPz ] = 0;
      
      Real comprisedLength(0);
      
      combinationFactory.resetPointerToHeadCombination();      
      
//    while( not( combinationFactory.hasGeneratedAll() ) )
      while( not( combinationFactory.hasTraversedAll() ) )
      {
	
	tuple_Type pzTupleIDs;

	combinationFactory.getNextCombination( pzTupleIDs );
		
	// IMPORTANT: mapping the IDs of the tuple to the IDs of the population
	// The problem is that leaving one patient out will produce a tuple in
	// the range [ 0, 1, ..., nbPz - 1], while the IDs out of the combinationFactory
	// are in the range [ 0, 1, ..., nbPz ] except for globalPzID.
	for ( UInt iJ(0); iJ < J; ++iJ )
	{	  
	   if ( pzTupleIDs[ iJ ] >= globalPzID ) 
	   {
	       ++pzTupleIDs[ iJ ];
	   }
	  
	}
	
 	bool flagMaxCurr, flagMinCurr, flagMaxPrev, flagMinPrev;
 	Real envMaxPrev, envMinPrev, envMaxCurr, envMinCurr;
	
	std::vector< Real > currentValues( J );
			      
	for ( UInt iJ(0); iJ < J; ++iJ )
	{		
	  // TODO CHECK THIS: THERE WAS A nbPts INSTEAD OF 0
 	  currentValues[ iJ ] = (*dataPtr)( pzTupleIDs[ iJ ], 0 );
	} 

 	envMaxPrev =  *( std::max_element( currentValues.begin(), currentValues.end() ) );
 	envMinPrev =  *( std::min_element( currentValues.begin(), currentValues.end() ) );
 	
  	flagMaxPrev = (*dataPtr)( globalPzID, 0 ) <= envMaxPrev;
  	flagMinPrev = (*dataPtr)( globalPzID, 0 ) >= envMinPrev;
 	
 	    for ( UInt iPt(1); iPt < nbPts; ++iPt )
 	    {  
 	      for ( UInt iJ(0); iJ < J; ++iJ )
 	      {		
  		  currentValues[ iJ ] = (*dataPtr)( pzTupleIDs[ iJ ], iPt );
 	      } 
 	      
  	      envMaxCurr = *( std::max_element( currentValues.begin(), currentValues.end() ) );
  	      envMinCurr = *( std::min_element( currentValues.begin(), currentValues.end() ) );
 	      
       	      flagMaxCurr = (*dataPtr)( globalPzID, iPt ) <= envMaxCurr;
   	      flagMinCurr = (*dataPtr)( globalPzID, iPt ) >= envMinCurr;
 
  	      if ( flagMaxCurr && flagMinCurr && flagMaxPrev && flagMinPrev )
 	      {
 		  comprisedLength += 1;
 	      }
 	      else
 	      {
 		  Real valueCurr = (*dataPtr)( globalPzID, iPt );
 		  Real valuePrev = (*dataPtr)( globalPzID, iPt - 1);
 
 		  if ( not( flagMaxCurr ) )
 		  {
 		    if ( flagMaxPrev )
 		    {
 			comprisedLength += ( valuePrev - envMaxPrev ) / ( (envMaxCurr - envMaxPrev) - ( valueCurr - valuePrev ) );
 		    } 
 		  }
 		  else if ( not( flagMaxPrev ) )
 		  {
 		      if ( flagMaxCurr )
 		      {
 			  comprisedLength += 1. - ( valuePrev - envMaxPrev ) / ( (envMaxCurr - envMaxPrev) - ( valueCurr - valuePrev ) );
 		      }
 		  }
 		  
 		  if( not( flagMinCurr ) )
 		  {
 		    if ( flagMinPrev )
 		    {
 			comprisedLength += ( valuePrev - envMinPrev ) / ( (envMinCurr - envMinPrev) - ( valueCurr - valuePrev ) );
 		    }
		  }
 		  else if ( not( flagMinPrev ) )
 		  {
 		      if ( flagMinCurr )
 		      {
 			comprisedLength += 1. - ( valuePrev - envMinPrev ) / ( (envMinCurr - envMinPrev) - ( valueCurr - valuePrev ) );
 		      }
 		  }
 	      }
 	      
 	      flagMaxPrev = flagMaxCurr;
 	      flagMinPrev = flagMinCurr;
 
 	      envMaxPrev = envMaxCurr;
 	      envMinPrev = envMinCurr;
	    }
	}

 	this->M_BDs[ iPz ] = comprisedLength / static_cast< Real > ( ( nbPts - 1 ) * binomial( nbPz - 1, J ) );
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
 
   MPI_Barrier( MPI_COMM_WORLD );
  
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