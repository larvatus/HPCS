
#include "user_fun.hpp"

namespace HPCS
{  /*
     void 
     computeBDs( const GetPot & dataFile, const std::string & section )
     {
	typedef DataSet dataSet_Type;
	typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
    
	typedef dataSet_Type::data_Type data_Type;
	typedef dataSet_Type::dataPtr_Type dataPtr_Type;
    
	typedef CombinationsID combinationsID_Type;
	typedef CombinationsID::tuple_Type tuple_Type;
       
       
	int nbThreads(0), myRank(0);
      
	MPI_Comm_size( MPI_COMM_WORLD, & nbThreads );
	MPI_Comm_rank( MPI_COMM_WORLD, & myRank );
   
	int nbPz(0), nbPts(0);
	int nbPtsFull(0);
	int leftOffset(0), rightOffset(0);
	int J(0);
	int verbosity(0);
    
	std::string leadFilename, outputFilename;

	int nChars(0);
	
	// MASTER PROC READS THE DATA SETUP FILE AND BROADCASTS THEM
    
	if ( myRank == MASTER )
	{
	    const std::string baseName( section );
	    
	    nbPz =  dataFile( ( baseName + "/nbPz"  ).data(), 10 );
	    nbPtsFull = dataFile( ( baseName + "/nbPts" ).data(), 100 ); 
	    leadFilename = dataFile( ( baseName + "/inputFilename" ).data(), "I" );
	    nChars = leadFilename.size() + 1;
	    leftOffset = dataFile( ( baseName + "/leftOffset" ).data(), 0 );
	    rightOffset = dataFile( ( baseName + "/rightOffset" ).data(), 0 );
		  
	    nbPts = nbPtsFull - leftOffset - rightOffset;
	    
	    J = dataFile( ( baseName + "/J" ).data(), 2 );
	  
	    outputFilename = dataFile( ( baseName + "/outputFilename" ).data(), "bd.dat" );

	    verbosity = dataFile( ( baseName + "/verbosity" ).data(), false );
	}     
    
	MPI_Bcast( &nbPz, 1, MPI_INT, MASTER, MPI_COMM_WORLD );
	MPI_Bcast( &nbPts, 1, MPI_INT, MASTER, MPI_COMM_WORLD );
	MPI_Bcast( &verbosity, 1, MPI_INT, MASTER, MPI_COMM_WORLD );   
	MPI_Bcast( &J, 1, MPI_INT, MASTER, MPI_COMM_WORLD );
	MPI_Bcast( &nChars, 1, MPI_INT, MASTER, MPI_COMM_WORLD );	

	char * leadFilenameAsCharArray = new char[ nChars ];
	if ( myRank == MASTER ) std::strcpy( leadFilenameAsCharArray, leadFilename.c_str() );
	MPI_Bcast( leadFilenameAsCharArray, nChars, MPI_CHAR, MASTER, MPI_COMM_WORLD);
	
	// EACH PROC. READS DATA	
	
	dataSetPtr_Type dataSetPtr( new dataSet_Type( nbPz, nbPts ) );
	
	dataSetPtr->readData( static_cast< std::string >( leadFilenameAsCharArray ) );
	
	delete[] leadFilenameAsCharArray;

	const dataPtr_Type dataPtr( dataSetPtr->getData() ); 
	
	const UInt slaveProcNbPz = static_cast< UInt >( nbPz / nbThreads );
	const UInt masterProcNbPz = static_cast< UInt >( nbPz / nbThreads ) + static_cast< UInt >( nbPz % nbThreads );
   
	UInt nbMyPz(0);
    
	myRank == MASTER ? nbMyPz = masterProcNbPz : nbMyPz = slaveProcNbPz;
    
	std::vector< Real > bandDepths( nbPz );
	
	// STARTING THE COMPUTATION
   
	combinationsID_Type combinationsID( nbPz - 1, J, true );
	
	combinationsID.generateCombinationsID();
   
	for ( UInt iPz(0); iPz < nbMyPz; ++iPz )
	{
	    if( verbosity > 2 ) printf( "Proc %d is at %d / %d patients\n", myRank, iPz+1, nbMyPz );
		
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
	      bandDepths[ iPz ] = comprisedCounter / static_cast< Real > ( ( nbPts - 1 ) * binomial( nbPz - 1, J ) );
	}	    
      
	// COMMUNICATING BAND DEPTHS
	  
	for ( UInt iThread = 1; iThread < nbThreads; ++iThread )
	{
	  if ( myRank == MASTER )
	  {
	    MPI_Status status;

	    MPI_Recv( & bandDepths[0] + masterProcNbPz + ( iThread - 1 ) * slaveProcNbPz, slaveProcNbPz, MPI_DOUBLE_PRECISION, iThread, iThread, MPI_COMM_WORLD, & status );
	  }
	  else if ( myRank == iThread )
	  {
	    MPI_Send( & bandDepths[0], slaveProcNbPz, MPI_DOUBLE_PRECISION, MASTER, myRank, MPI_COMM_WORLD );
	  }
	}
	
	// WRITING BAND DEPTHS TO OUTPUT FILENAME

      
	if ( myRank == MASTER ) printf( " FINISHED COMPUTATION IN DIRECT WAY \n" );
	
	return;
	
     }
     */
  
     
}