

#ifndef __USER_FUN_HPP_
#define __USER_FUN_HPP_

#include <mpi_utility.hpp>
#include <dataSet.hpp>
#include <combinations.hpp>
#include <HPCSDefs.hpp>

namespace HPCS
{    
 
  template < BDPolicy _policy >
  class GentonTest 
  {
  public:
    
      typedef GetPot getPot_Type;
    
      typedef CombinationsID combinationsID_Type;
      typedef CombinationsID::tuple_Type tuple_Type;
    
      typedef mpiUtility mpiUtility_Type;
      
      typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
    
      GentonTest();
      
      virtual ~GentonTest(){};
      
      void computeBDs( const getPot_Type & data, const std::string & section  );
      
      void writeBDs( const std::string & outputFilename );
    
  private:
    
    std::vector< Real > M_BDs;
    
    mpiUtilityPtr_Type M_mpiUtilPtr;
    
    UInt binomial( const UInt & N, const UInt & K );
    
  };
  
  template < BDPolicy _policy >
  GentonTest< _policy >::
  GentonTest()
  :
  M_mpiUtilPtr( new mpiUtility_Type() )
  {}
  
  template < BDPolicy _policy >
  void
  GentonTest< _policy >::
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
  
  template < BDPolicy _policy >
  UInt 
  GentonTest< _policy >::
  binomial( const UInt & N , const UInt & K )
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
 
 
  template <>
  void
  GentonTest< All >::
  computeBDs( const getPot_Type & dataFile, const std::string & section  )
  { 	
    
	typedef DataSet dataSet_Type;
	typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
	
	typedef dataSet_Type::data_Type data_Type;
	typedef dataSet_Type::dataPtr_Type dataPtr_Type;
    
	const std::string baseName( section );
	
	const UInt nbThreads( this->M_mpiUtilPtr->nbThreads() );
	
	const UInt myRank( this->M_mpiUtilPtr->myRank() );
	
	const UInt nbPz( dataFile( ( baseName + "/nbPz"  ).data(), 10 ) );
	
	const UInt nbPtsFull( dataFile( ( baseName + "/nbPts" ).data(), 100 ) );
	
	const UInt leftOffset( dataFile( ( baseName + "/leftOffset" ).data(), 0 ) );
	
	const UInt rightOffset( dataFile( ( baseName + "/rightOffset" ).data(), 0 ) );
	
	const UInt nbPts = nbPtsFull - leftOffset - rightOffset;
	
	const UInt J( dataFile( ( baseName + "/J" ).data(), 2 ) );
	
	const UInt verbosity( dataFile( ( baseName + "/verbosity" ).data(), false ) );
 
	const std::string inputFilename = dataFile( ( baseName + "/inputFilename" ).data(), "I" );

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
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
}






#endif
