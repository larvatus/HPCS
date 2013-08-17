#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "GetPot"
#include "dataSet.hpp"
#include <string>
#include <sstream>
#include <algorithm>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <iomanip>

#include "combinationFactory.hpp"

#define MASTER 0

using namespace std;

typedef double Real;
typedef unsigned int UInt;
typedef DataSet dataSet_Type;
typedef boost::shared_ptr< DataSet > dataSetPtr_Type;
typedef CombinationFactory combinationFactory_Type;
typedef combinationFactory_Type::tuple_Type tuple_Type;

int main( int argc, char * argv[] )
{
    MPI_Init( & argc, & argv );
  
    int nbThreads(1), myRank(0);
  
    MPI_Comm_size( MPI_COMM_WORLD, & nbThreads );
    MPI_Comm_rank( MPI_COMM_WORLD, & myRank );
   
    int nbPz(0), nbPts(0);
    int nbPtsFull(0);
    int leftOffset(0), rightOffset(0);
    int J(0);
    int verbosity(0);
    
    std::string leadID;

    UInt nbMyPz(0);
    
    std::string outputFilename;
    
    // SETUP OF CONSTANT VARIABLES
        
    std::map< std::string, std::string > leadFilenames;
    
    leadFilenames.insert( std::pair< std::string, std::string >("I", "leads/I.txt") );
    leadFilenames.insert( std::pair< std::string, std::string >("II", "leads/II.txt") );
    leadFilenames.insert( std::pair< std::string, std::string >("V1", "leads/V1.txt") );
    leadFilenames.insert( std::pair< std::string, std::string >("V2", "leads/V2.txt") );
    leadFilenames.insert( std::pair< std::string, std::string >("V3", "leads/V3.txt") );
    leadFilenames.insert( std::pair< std::string, std::string >("V4", "leads/V4.txt") );
    leadFilenames.insert( std::pair< std::string, std::string >("V5", "leads/V5.txt") );
    leadFilenames.insert( std::pair< std::string, std::string >("V6", "leads/V6.txt") );    
    leadFilenames.insert( std::pair< std::string, std::string >("Ider", "leads/I.der.txt" ) );
    leadFilenames.insert( std::pair< std::string, std::string >("IIder", "leads/II.der.txt" ) );
    leadFilenames.insert( std::pair< std::string, std::string >("V1der", "leads/V1.der.txt" ) );
    leadFilenames.insert( std::pair< std::string, std::string >("V2der", "leads/V2.der.txt" ) );
    leadFilenames.insert( std::pair< std::string, std::string >("V3der", "leads/V3.der.txt" ) );
    leadFilenames.insert( std::pair< std::string, std::string >("V4der", "leads/V4.der.txt" ) );
    leadFilenames.insert( std::pair< std::string, std::string >("V5der", "leads/V5.der.txt" ) );	
    leadFilenames.insert( std::pair< std::string, std::string >("V6der", "leads/V6.der.txt") );

   // MASTER PROC READS THE DATA SETUP FILE
    
   if ( myRank == MASTER )
   {
      GetPot command_line( argc, argv );
      
      const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
      
      GetPot dataFile( data_file_name.data() );
      
      const string baseName( "BDALL" );
      
      nbPz =  dataFile( ( baseName + "/nbPz"  ).data(), 10 );
      nbPtsFull = dataFile( ( baseName + "/nbPts" ).data(), 100 ); 
      leadID = dataFile( ( baseName + "/leadID" ).data(), "I" );
      leftOffset = dataFile( ( baseName + "/leftOffset" ).data(), 0 );
      rightOffset = dataFile( ( baseName + "/rightOffset" ).data(), 0 );
            
      nbPts = nbPtsFull - leftOffset - rightOffset;
      
      std::cout << " NBPTS = " << nbPts << " leftOffset = " << leftOffset << " rightOffset = " << rightOffset << std::endl;
      
      J = dataFile( ( baseName + "/J" ).data(), 2 );
     
      outputFilename = dataFile( ( baseName + "/outputFilename" ).data(), "bd.dat" );

      verbosity = dataFile( ( baseName + "/verbosity" ).data(), false );
  }     
    
   MPI_Bcast( &nbPz, 1, MPI_INT, MASTER, MPI_COMM_WORLD );
   MPI_Bcast( &nbPts, 1, MPI_INT, MASTER, MPI_COMM_WORLD );
   MPI_Bcast( &verbosity, 1, MPI_INT, MASTER, MPI_COMM_WORLD );   
   MPI_Bcast( &J, 1, MPI_INT, MASTER, MPI_COMM_WORLD );
   
   UInt slaveProcNbPz = static_cast< UInt >( nbPz / nbThreads );
   UInt masterProcNbPz = static_cast< UInt >( nbPz / nbThreads ) + static_cast< UInt >( nbPz % nbThreads );
   
   if( myRank != MASTER )
   {
      nbMyPz = slaveProcNbPz;
   }
   if ( myRank == MASTER )
   {
      nbMyPz = masterProcNbPz;
   }   
   
   Real * myData;
   Real * myBandDepths = new Real[ nbMyPz ];

   if ( verbosity > 0 ) printf( " Assigned %d Pz to proc %d\n", nbMyPz, myRank );
   
   // MASTER PROC. READS THE TRACES of leadID FROM .dat FILES, THEN BROADCASTS THEM
   
   dataSetPtr_Type dataSetPtr;   
     
   if ( myRank == MASTER )
   {
      dataSetPtr.reset( new dataSet_Type( nbPz, nbPtsFull, 1, leftOffset, rightOffset ) );        
	  
      if ( verbosity > 1 )
      {
	printf( " Proc %d is reading %s\n", myRank, leadFilenames.at( leadID ).data() ); 
      }
	    
      dataSetPtr->readCSV( leadFilenames.at( leadID ) );	
	     
      if ( verbosity > 0 ) printf( std::string( " Master Proc has read its ECG traces from " + leadFilenames.at( leadID ) + std::string(" file\n") ).data() );
      
      myData = dataSetPtr->getData();
      
  }
  else
  {    
      myData = new Real[ nbPz * nbPts ];
  }    
       
  MPI_Bcast( myData, nbPts * nbPz, MPI_DOUBLE_PRECISION, MASTER, MPI_COMM_WORLD );

// COMPUTATION OF BAND DEPTHS : NEW VERSION, J = 2,3,4,...
 
  combinationFactory_Type combinationFactory( nbPz - 1, J );
   
  combinationFactory.generateCombinations();
  
 
  for ( UInt iPz(0); iPz < nbMyPz; ++iPz )
  {
      if( verbosity > 2 ) printf( "Proc %d is at %d / %d patients\n", myRank, iPz+1, nbMyPz );
	  
      UInt globalPzID;
      
      if ( myRank == MASTER )
      {
	globalPzID = iPz;
      }
      else
      {
	globalPzID = masterProcNbPz  + ( myRank - 1 ) * slaveProcNbPz + iPz;
      }
            
      // IMPORTANT: I leave one patient out, so the actual N in the binomial coefficient is nbPz - 1
      //combinationFactory_Type combinationFactory( nbPz - 1, J );
	
      myBandDepths[ iPz ] = 0;
      
      Real comprisedLength(0);
      
      combinationFactory.resetPointerToHeadCombination();
	      
//       UInt iteration(0);
      
      
//    while( not( combinationFactory.hasGeneratedAll() ) )
      while( not( combinationFactory.hasTraversedAll() ) )
      {
// 	++iteration;
	
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
	  currentValues[ iJ ] = myData[ pzTupleIDs[ iJ ] * nbPts + 0 ];
	} 

	envMaxPrev =  *( std::max_element( currentValues.begin(), currentValues.end() ) );
	envMinPrev =  *( std::min_element( currentValues.begin(), currentValues.end() ) );
	
	flagMaxPrev = myData[ globalPzID * nbPts + 0 ] <= envMaxPrev;
	flagMinPrev = myData[ globalPzID * nbPts + 0 ] >= envMinPrev;
	
	    for ( UInt iPt(1); iPt < nbPts; ++iPt )
	    {  
	      for ( UInt iJ(0); iJ < J; ++iJ )
	      {		
		  currentValues[ iJ ] = myData[ pzTupleIDs[ iJ ] * nbPts + iPt ];
	      } 
	      
	      envMaxCurr = *( std::max_element( currentValues.begin(), currentValues.end() ) );
	      envMinCurr = *( std::min_element( currentValues.begin(), currentValues.end() ) );
	      
      	      flagMaxCurr = myData[ globalPzID * nbPts + iPt ] <= envMaxCurr;
	      flagMinCurr = myData[ globalPzID * nbPts + iPt ] >= envMinCurr;

	      if ( flagMaxCurr && flagMinCurr && flagMaxPrev && flagMinPrev )
	      {
		  comprisedLength += 1;
	      }
	      else
	      {
		  Real valueCurr = myData[ globalPzID * nbPts + iPt ];
		  Real valuePrev = myData[ globalPzID * nbPts + iPt - 1];

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

 	myBandDepths[ iPz ] = comprisedLength / static_cast< Real > ( ( nbPts - 1 ) * binomial( nbPz - 1, J ) );
  }	    
    
    
  // COMMUNICATING BAND DEPTHS TO MASTER PROC
  
  Real * allDepths;

  if ( myRank == MASTER )
  {
      allDepths = new Real [ nbPz ];
  }
    
  for ( UInt iThread(1); iThread < nbThreads; ++iThread )
  {
      if ( myRank == iThread )
      {
	MPI_Send( myBandDepths, nbMyPz, MPI_DOUBLE_PRECISION, MASTER, myRank, MPI_COMM_WORLD );  
      }
      
      if ( myRank == MASTER )
      {
	
	MPI_Status status;
	
	MPI_Recv( allDepths + nbMyPz + ( iThread - 1 ) * slaveProcNbPz, slaveProcNbPz, MPI_DOUBLE_PRECISION, iThread, iThread, MPI_COMM_WORLD, & status );  
      
	if ( verbosity > 2 )
	  
	  printf( " MASTER Proc has received dephts from Proc %d\n", iThread );
      }
  }

  
  if ( myRank == MASTER )
  {
      for ( UInt iPz(0); iPz < nbMyPz; ++iPz )
      
	allDepths[ iPz ] = myBandDepths[ iPz ];
  }
  
  MPI_Barrier( MPI_COMM_WORLD );
    
  if ( verbosity > 2 && myRank == MASTER ) 
    printf( " All depths have been gathered\n" );
  
  if ( myRank == MASTER )
  {
      std::ofstream output( outputFilename.data(), std::ios_base::out );
      
      output.precision(15);
      
      for ( UInt iPz(0); iPz < nbPz; ++iPz )
      {
	  output << iPz + 1 << " " << allDepths[ iPz ] << std::endl;
      }
      
      output.close();
 
      if ( verbosity > 2 ) printf( ( std::string(" MASTER Proc has written dephts to file ") + outputFilename + std::string("\n") ).c_str() );
      
  }
 


MPI_Barrier( MPI_COMM_WORLD );

delete[] myBandDepths;

if ( myRank != MASTER )
  {
       delete[] myData;   
  }
 // else I'm MASTER proc
 else
 {
       delete[] allDepths;
 }

  MPI_Finalize();    
  
  return EXIT_SUCCESS;
}
