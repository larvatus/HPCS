

#include "user_fun.hpp"


namespace HPCS
{
  
  // BD DIRECT COMPUTATION
  
  BDDirectComputation::
  BDDirectComputation()
  :
  M_mpiUtilPtr( new mpiUtility_Type() )
  {
  }
  
  void
  BDDirectComputation::
  writeBDs( const std::string & outputFilename )
  {
      if ( this->M_mpiUtilPtr->isMaster() )
      {
	  std::ofstream output( outputFilename.data(), std::ios_base::out );
	  
	  std::vector< Real >::const_iterator it;
	  
	  UInt count(0);
	  
	  for(  it = this->M_BDs.begin(); it != this->M_BDs.end(); ++it )
	  {
	      output << count << " " << *it << std::endl;
	      
	      ++count;
	  }
	
	  output.close();
      }
    
      return;   
  }
  
  // BD ALL DIRECT COMPUTATION
  
  BDAllDirectComputation::
  BDAllDirectComputation()
  :
  BDDirectComputation()
  {
  }
  
  void
  BDAllDirectComputation::
  computeBDs( const getPot_Type & dataFile, const std::string & section  )
  { 	
    typedef DataSet dataSet_Type;
    typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
	
    typedef dataSet_Type::data_Type data_Type;
    typedef dataSet_Type::dataPtr_Type dataPtr_Type;
	
    const UInt nbThreads( this->M_mpiUtilPtr->nbThreads() );
	
    const UInt myRank( this->M_mpiUtilPtr->myRank() );
	
    const UInt nbPz( dataFile( ( section + "/nbSamples"  ).data(), 10 ) );
	
    const UInt nbPtsFull( dataFile( ( section + "/nbPts" ).data(), 100 ) );
	
    const UInt leftOffset( dataFile( ( section + "/leftOffset" ).data(), 0 ) );
	
    const UInt rightOffset( dataFile( ( section + "/rightOffset" ).data(), 0 ) );
	
    const UInt nbPts = nbPtsFull - leftOffset - rightOffset;
	
    const UInt J( dataFile( ( section + "/J" ).data(), 2 ) );
	
    const UInt verbosity( dataFile( ( section + "/verbosity" ).data(), false ) );
 
    const std::string inputFilename = dataFile( ( section + "/inputFilename" ).data(), "I" );

    // EACH PROC. READS DATA	
	
    dataSetPtr_Type dataSetPtr( new dataSet_Type( nbPz, nbPts ) );
	
    dataSetPtr->readData( inputFilename );

    const dataPtr_Type dataPtr( dataSetPtr->getData() ); 
	  
    const UInt slaveProcNbPz = static_cast< UInt >( nbPz / nbThreads );
    
    const UInt masterProcNbPz = static_cast< UInt >( nbPz / nbThreads ) + static_cast< UInt >( nbPz % nbThreads );
   
    UInt nbMyPz(0);
    
    myRank == MASTER ? nbMyPz = masterProcNbPz : nbMyPz = slaveProcNbPz;
    
    this->M_BDs.assign( nbPz, 0 );
	
    // STARTING THE COMPUTATION
   
    combinationsID_Type combinationsID( nbPz - 1, J, true );
	
    combinationsID.generateCombinationsID();
   
    for ( UInt iPz(0); iPz < nbMyPz; ++iPz )
    {
      if( verbosity > 2 ) printf( "Proc %d is at %d / %d patients\n", myRank, iPz + 1, nbMyPz );
		
      UInt globalPzID;
	  
      myRank == MASTER ? globalPzID = iPz : globalPzID = masterProcNbPz  + ( myRank - 1 ) * slaveProcNbPz + iPz;
		  
      Real comprisedCounter(0);
	    
      combinationsID.resetPointerToHeadCombination();      
	    
      while( not( combinationsID.hasTraversedAll() ) )
      {
	tuple_Type pzTupleIDs;

	combinationsID.getNextCombinationID( pzTupleIDs );
		      
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

      this->M_BDs[ iPz ] = comprisedCounter / static_cast< Real > ( ( nbPts - 1 ) * binomial( nbPz - 1, J ) );
      }	    
      
      // COMMUNICATING BAND DEPTHS
	  
      for ( UInt iThread = 1; iThread < nbThreads; ++iThread )
      {
	if ( myRank == MASTER )
	{
	  MPI_Status status;

	  MPI_Recv( & this->M_BDs[0] + masterProcNbPz + ( iThread - 1 ) * slaveProcNbPz, slaveProcNbPz, MPI_DOUBLE_PRECISION, iThread, iThread, MPI_COMM_WORLD, & status );
	}
	else if ( myRank == iThread )
	{
	  MPI_Send( & this->M_BDs[0], slaveProcNbPz, MPI_DOUBLE_PRECISION, MASTER, myRank, MPI_COMM_WORLD );
	}
      }
	
    }

  // BD REF DIRECT COMPUTATION
  
  BDRefDirectComputation::
  BDRefDirectComputation()
  :
  BDDirectComputation()
  {
  }
    
  void
  BDRefDirectComputation::
  computeBDs( const getPot_Type & dataFile, const std::string & section )
  {
    typedef DataSetLevelled dataSet_Type;
    typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
	
    typedef dataSet_Type::data_Type data_Type;
    typedef dataSet_Type::dataPtr_Type dataPtr_Type;
	
    const UInt nbThreads( this->M_mpiUtilPtr->nbThreads() );
	
    const UInt myRank( this->M_mpiUtilPtr->myRank() );
	
    const UInt nbSamples( dataFile( ( section + "/nbSamples"  ).data(), 10 ) );
    
    const UInt nbRefSamples( dataFile( ( section + "/nbReferenceSamples"  ).data(), 10 ) );
    
    const UInt nbTestSamples = nbSamples - nbRefSamples;
	
    const UInt nbPtsFull( dataFile( ( section + "/nbPts" ).data(), 100 ) );
	
    const UInt leftOffset( dataFile( ( section + "/leftOffset" ).data(), 0 ) );
	
    const UInt rightOffset( dataFile( ( section + "/rightOffset" ).data(), 0 ) );
	
    const UInt nbPts = nbPtsFull - leftOffset - rightOffset;
	
    const UInt J( dataFile( ( section + "/J" ).data(), 2 ) );
	
    const UInt verbosity( dataFile( ( section + "/verbosity" ).data(), false ) );
 
    const std::string inputFilename = dataFile( ( section + "/inputFilename" ).data(), "I" );
    
    const std::string levelsExtremaFilename = dataFile( ( section + "/levelsExtremaFilename" ).data(), "levels.dat" );

    const UInt seed = dataFile( ( section + "/seed").data(), 1 );
    
    // EACH PROC. READS DATA	
	
    dataSetPtr_Type dataSetPtr( new dataSet_Type( nbSamples, nbPts, 2 ) );
	
    dataSetPtr->readData( inputFilename );
    
    dataSetPtr->setLevelsFromExtrema( levelsExtremaFilename );

    const dataPtr_Type dataPtr( dataSetPtr->getData() ); 
    
    
    // SETTING UP THE REFERENCE SET AND THE TESTSET
    
    srand48( seed );

    typedef std::set< UInt >::const_iterator iterator_Type;
    
    std::set< UInt > referenceSetIDs;
    
    std::set< UInt > testSetIDs;
    
    
    for ( UInt iInsert(0); iInsert < nbRefSamples; )
    {	
      UInt temp = static_cast< UInt >( dataSetPtr->cardinality( 0 ) * drand48() );
	
      if ( temp != dataSetPtr->cardinality( 0 )  && ( referenceSetIDs.insert( temp ).second == true)  )
      {
	  ++iInsert;
      }	
    }
    
    for( UInt iSample(0); iSample < nbSamples; ++iSample )
    {
      if ( referenceSetIDs.find( iSample ) == referenceSetIDs.end() )
      {
	  testSetIDs.insert( iSample );	
      }
    }
    
    // SETTING UP THE JOB
    
    const UInt slaveProcNbSamples = static_cast< UInt >( nbTestSamples / nbThreads );
    
    const UInt masterProcNbSamples = static_cast< UInt >( nbTestSamples / nbThreads ) + static_cast< UInt >( nbTestSamples % nbThreads );
   
    this->M_BDs.assign( nbTestSamples, 0 );
   
    // FIRST STAGE:
   
    combinationsID_Type combinationsID( nbRefSamples, J, true );
   
    combinationsID.generateCombinationsID();
  
    UInt nbMySamples;
   
    this->M_mpiUtilPtr->isMaster() ? nbMySamples = masterProcNbSamples : nbMySamples = slaveProcNbSamples;
   
    for ( UInt iSample(0); iSample < nbMySamples; ++iSample )
    {
	if( verbosity > 2 ) printf( "Proc %d is at %d / %d samples\n", myRank, iSample + 1, nbMySamples );
	
	UInt nbOffsets;
	
	this->M_mpiUtilPtr->isMaster() ? nbOffsets = iSample : nbOffsets = masterProcNbSamples + ( myRank - 1 ) * slaveProcNbSamples + iSample; 
	
	iterator_Type globalSampleIDIter = testSetIDs.begin();
	
	std::advance( globalSampleIDIter, nbOffsets );
	
	Real comprisedCounter(0);
	
	combinationsID.resetPointerToHeadCombination();      
	
	while( not( combinationsID.hasTraversedAll() ) )
	{
	  tuple_Type pzTupleIDs;

	  combinationsID.getNextCombinationID( pzTupleIDs );
		  
	  // IMPORTANT: mapping the IDs of the tuple to the IDs of the reference sub-population
	  for ( UInt iJ(0); iJ < J; ++iJ )
	  {	  
	    iterator_Type it = referenceSetIDs.begin();
	    
	    std::advance( it, pzTupleIDs[ iJ ] );
	    
	    pzTupleIDs[ iJ ] = *it;  	  
	  }
	  
	  std::vector< Real > currentValues( J );
	  
	  Real envMax, envMin, sampleVal;
	  
	  for ( UInt iPt(0); iPt < nbPts; ++iPt )
	  {
	    for ( UInt iJ(0); iJ < J; ++iJ )
	    {		
	      currentValues[ iJ ] = (*dataPtr)( pzTupleIDs[ iJ ], iPt );
	    } 
	    
	    envMax =  *( std::max_element( currentValues.begin(), currentValues.end() ) );
	    envMin =  *( std::min_element( currentValues.begin(), currentValues.end() ) );
	    sampleVal = (*dataPtr)( *globalSampleIDIter, iPt );
	    
	    if ( sampleVal <= envMax && sampleVal >= envMin ) ++comprisedCounter;  
    
	  }
	}

	this->M_BDs[ iSample ] = comprisedCounter / static_cast< Real > ( ( nbPts - 1 ) * binomial( nbRefSamples, J ) );
    }	    
 
    // COMMUNICATING BAND DEPTHS
      
    for ( UInt iThread = 1; iThread < nbThreads; ++iThread )
    {
      if ( this->M_mpiUtilPtr->isMaster() )
      {
	MPI_Status status;

	MPI_Recv( & this->M_BDs[0] + masterProcNbSamples + ( iThread - 1 ) * slaveProcNbSamples, slaveProcNbSamples, MPI_DOUBLE_PRECISION, iThread, iThread, MPI_COMM_WORLD, & status );
      }
      else if ( myRank == iThread )
      {
	MPI_Send( & this->M_BDs[0], slaveProcNbSamples, MPI_DOUBLE_PRECISION, MASTER, myRank, MPI_COMM_WORLD );
      }
    }
    
    if ( verbosity > 2 && this->M_mpiUtilPtr->isMaster() ) 

      printf( " All depths have been gathered\n" );
    
    return;
      
  }

}