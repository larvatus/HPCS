
#ifndef __BD_HPP__
#define __BD_HPP__

#include "dataSet.hpp"
#include "bandDepthData.hpp"
#include "combinations.hpp"
#include "mpi_utility.hpp"
#include "GetPot"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <map>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <algorithm>
#include <mpi.h>

/*!
    @file BandDepth.hpp
    @brief

    @author Nicholas Tarabelloni
    @date 04 Aug 2013
    
    @mantainer Nicholas Tarabelloni
 */

namespace HPCS
{  

typedef unsigned int UInt;
typedef double Real;  

class 
BandDepthBase
{
public:
  
  typedef double Real;
  
  typedef double UInt;
  
  BandDepthBase(){};
  
  ~BandDepthBase(){};
  
  virtual void computeBDs(){};
  
  virtual void setBandDepthData( const BandDepthData & bdData ){};
  
  virtual void writeBDs()const {};
  
protected:
  
};


/*!
 * This class implements an interface for the fast computation of Band Depths for functional data.
 * 
 */  
  
template < UInt _J >
class BandDepth : public BandDepthBase
{
public:
  
  //! @name Types definitions
  //@{
    
  typedef double Real;
  
  typedef GetPot inputData_Type;
  
  typedef unsigned int UInt;
  
  typedef BandDepthData bdData_Type;
  typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
  
  typedef DataSet dataSet_Type;
  typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
  
  typedef CombinationsID combinationsID_Type;
  typedef CombinationsID::tuple_Type tuple_Type;
  
  typedef mpiUtility mpiUtility_Type;
  typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
  
  //@}
   
  //! @name Constructor & Destructor
  //@{
    
  //! Default constructor
  BandDepth(){};
    
  //! Constructor from a BandDepthData object
  BandDepth( const bdData_Type & bdData );
  
  //! Constructor from a shared pointer to BandDepthData object
  BandDepth( const bdDataPtr_Type & bdDataPtr );
  
  ~BandDepth(){};

  //@}
  
  //! @name Public Methods
  //@{

    
  //! Method for the computation of BDs. Computation is made in parallel exploiting MPI.
  void computeBDs();
  
  //! Method for the printing of computed BDs.
  /*!
   * The computed BDs are written by MASTER processor
   * on the file specified by BandDepthData object.
   */
  void writeBDs() const;
  
  //! Getter of the Band Depths
  const std::vector< Real > & getBDs() const; 
  
  //! Method for resetting the BandDepthData object.
  /*!
   * It enables the re-use of the BandDepth object in order to compute other bandDepths, e.g. for
   * other input files. This is useful when a class contains a pointer to a BandDepth object that
   * may be used over different datasets.
   * 
   * \param bdData New BandDepthData object that will replace the old one.
   */
  void setBandDepthData( const bdData_Type & bdData );
  
  
  //! Method for the setting up or the resetting of the dataSet.
  /*!
   * The purpose of this method is to break the dependency of the object on
   * a specific data file containing data. With can set up (or reset) the data
   * constituting the data set, if the "dimensions" agree with those expressed by
   * band depth data object.
   * This use is meant to pair with a constructor taking a Band Depth Data with no
   * input filename, and thus M_readDataFromFile flag equal to false.
   * 
   * \param dataPtr Pointer to the new dataSet
   */
  void setDataSet( const dataSetPtr_Type & dataPtr );  
  
  //@}
  
private:
  
  //! @name Private methods
  //@{

  //! Each thread reads data from data files.
  void readData();
    
  //! To compute binomial coefficients
  UInt binomial( const UInt & N , const UInt & K );
  
  //@}
  

  //! @name Private members
  //@{
  
  //! Shared pointer to a BandDepthData type object
  bdDataPtr_Type M_bdDataPtr;
  
  //! Shared pointer to a dataSet type object, containing data.
  dataSetPtr_Type M_dataSetPtr;
  
  //! Computed band depths
  std::vector< Real > M_BDs;
  
  //! MPI utility pointer object
  mpiUtilityPtr_Type M_mpiUtilPtr;
  
  //@}
    
};

 /////////////////////////
 // 	Band Depth
 /////////////////////////
 
 // Constructor from BandDepthData type object
 template < UInt _J > 
 BandDepth< _J >::
 BandDepth( const bdData_Type & bdData )
 :
 M_bdDataPtr( new bdData_Type( bdData ) ),
 M_mpiUtilPtr( new mpiUtility_Type() )
 {   
   if( bdData.readDataFromFile() ) this->readData();
 }
 
 template < UInt _J >
 BandDepth< _J >::
 BandDepth( const bdDataPtr_Type & bdDataPtr )
 :
 M_bdDataPtr( bdDataPtr ),
 M_mpiUtilPtr( new mpiUtility_Type() )
 {      
   if( this->M_bdDataPtr->readDataFromFile() ) this->readData();
 }
  
 // Reset BandDepthData type object contained 
 template < UInt _J >
 inline
 void 
 BandDepth< _J >::
 setBandDepthData( const bdData_Type & bdData )
 {
    this->M_bdDataPtr.reset( new bdData_Type( bdData ) );

    this->M_mpiUtilPtr.reset( new mpiUtility_Type() );
    
    if ( bdData.readDataFromFile() ) this->readData();
 }
 
 // Reset dataSet pointer. This allow using the object without a file from which to read.
 template < UInt _J >
 void
 BandDepth< _J >::
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
 template < UInt _J >
 void 
 BandDepth< _J >::
 readData()
 {
   this->M_dataSetPtr.reset( new dataSet_Type( M_bdDataPtr->nbPz(), M_bdDataPtr->nbPts() ) );
   
   this->M_dataSetPtr->setOffset( M_bdDataPtr->leftOffset(), M_bdDataPtr->rightOffset() );
   
   this->M_dataSetPtr->readData( M_bdDataPtr->inputFilename() );
   
   return;
 }
 
 // Method for the computation of BDs
 template < UInt _J >
 void
 BandDepth< _J >::
 computeBDs()
 {
   const UInt myRank = this->M_mpiUtilPtr->myRank();
   const UInt nbThreads = this->M_mpiUtilPtr->nbThreads();
   
   const UInt MASTER = this->M_mpiUtilPtr->master();
   
   const UInt nbPz 	= this->M_bdDataPtr->nbPz();
   const UInt nbPts	= this->M_bdDataPtr->nbPts();
//    const UInt J	   	= this->M_bdDataPtr->J();
   const UInt verbosity = this->M_bdDataPtr->verbosity();
   
   typedef dataSet_Type::dataPtr_Type dataSetPtr_Type;
   
   const dataSetPtr_Type dataPtr( this->M_dataSetPtr->getData() );  
            
   const UInt slaveProcNbPz = static_cast< UInt >( nbPz / nbThreads );
   const UInt masterProcNbPz = static_cast< UInt >( nbPz / nbThreads ) + static_cast< UInt >( nbPz % nbThreads );
   
   this->M_BDs.resize( nbPz );
   
   UInt nbMyPz;
   
   this->M_mpiUtilPtr->isMaster() ? nbMyPz = masterProcNbPz : nbMyPz = slaveProcNbPz;
   
   combinationsID_Type combinationsID( nbPz - 1, _J, true );
   
   combinationsID.generateCombinationsID();
   
   for ( UInt iPz(0); iPz < nbMyPz; ++iPz )
   {
      if( verbosity > 2 ) printf( "Proc %d is at %d / %d patients\n", myRank, iPz+1, nbMyPz );
	  
      UInt globalPzID;
     
      this->M_mpiUtilPtr->isMaster() ? globalPzID = iPz : globalPzID = masterProcNbPz  + ( myRank - 1 ) * slaveProcNbPz + iPz;
            
      Real comprisedCounter(0);
      
      combinationsID.resetPointerToHeadCombination();      
      
//    while( not( combinationsID.hasGeneratedAll() ) )
      while( not( combinationsID.hasTraversedAll() ) )
      {
	tuple_Type pzTupleIDs;

	combinationsID.getNextCombinationID( pzTupleIDs );
		
	// IMPORTANT: mapping the IDs of the tuple to the IDs of the population
	// The problem is that leaving one patient out will produce a tuple in
	// the range [ 0, 1, ..., nbPz - 1], while the IDs out of the combinationsID
	// are in the range [ 0, 1, ..., nbPz ] except for globalPzID.
	for ( UInt iJ(0); iJ < _J; ++iJ )
	{	  
	   if ( pzTupleIDs[ iJ ] >= globalPzID ) 
	   {
	       ++pzTupleIDs[ iJ ];
	   }	  
	}
	
	std::vector< Real > currentValues( _J );
	
	Real envMax, envMin, pzVal;
	
	for ( UInt iPt(0); iPt < nbPts; ++iPt )
	{
	  for ( UInt iJ(0); iJ < _J; ++iJ )
	  {		
	    currentValues[ iJ ] = (*dataPtr)( pzTupleIDs[ iJ ], iPt );
	  } 
	  
	  envMax =  *( std::max_element( currentValues.begin(), currentValues.end() ) );
	  envMin =  *( std::min_element( currentValues.begin(), currentValues.end() ) );
	  pzVal = (*dataPtr)( globalPzID, iPt );
	  
	  if ( pzVal <= envMax && pzVal >= envMin ) ++comprisedCounter;  
	  
	}
      }

	this->M_BDs[ iPz ] = comprisedCounter / static_cast< Real > ( ( nbPts - 1 ) * this->binomial( nbPz - 1, _J ) );
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

// Method to write the computed BDs
template < UInt _J >
void
BandDepth< _J >::
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


// Method to get the computed BDs
template < UInt _J >
inline 
const
std::vector< Real > &
BandDepth< _J >::
getBDs() const
{
    return this->M_BDs;
}


// Method for the computation of binomial coefficients
template < UInt _J >
UInt
BandDepth< _J >::
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


// UPGRADE: USE A VIEW OF DATASET

template <>
void
BandDepth< 2 >::
computeBDs()
{
    // UPGRADE: USE A VIEW OF DATASET
    
   const UInt myRank = this->M_mpiUtilPtr->myRank();
   const UInt nbThreads = this->M_mpiUtilPtr->nbThreads();
   
   const UInt MASTER = this->M_mpiUtilPtr->master();
   
   const UInt nbPz 	= this->M_bdDataPtr->nbPz();
   const UInt nbPts	= this->M_bdDataPtr->nbPts();
   const UInt verbosity = this->M_bdDataPtr->verbosity();
   
   typedef dataSet_Type::dataPtr_Type dataSetPtr_Type;
   
   const dataSetPtr_Type dataPtr( this->M_dataSetPtr->getData() );  
            
   const UInt slaveProcNbPz = static_cast< UInt >( nbPz / nbThreads );
   const UInt masterProcNbPz = static_cast< UInt >( nbPz / nbThreads ) + static_cast< UInt >( nbPz % nbThreads );
   
   const UInt slaveProcNbPts = static_cast< UInt >( nbPts / nbThreads );
   const UInt masterProcNbPts = static_cast< UInt >( nbPts / nbThreads ) + static_cast< UInt >( nbPts % nbThreads );
     
   this->M_BDs.resize( nbPz );
   
   // FIRST STAGE: // MOVE THIS INSIDE A NEW CLASS
  
   UInt nbMyPz, nbMyPts;
   
   this->M_mpiUtilPtr->isMaster() ? nbMyPz = masterProcNbPz : nbMyPz = slaveProcNbPz;
   this->M_mpiUtilPtr->isMaster() ? nbMyPts = masterProcNbPts : nbMyPts = slaveProcNbPts;
   
   std::vector< int > sortedIDsByPoint( nbPts * nbPz );
   
   for ( UInt iPt(0); iPt < nbMyPts; ++iPt )
   {
       UInt globalPtID;
     
       this->M_mpiUtilPtr->isMaster() ? globalPtID = iPt : globalPtID = masterProcNbPts  + ( myRank - 1 ) * slaveProcNbPts + iPt;
       
       // TODO MODIFY THIS with a view
       
       std::multimap< Real, int > map;
       
       for ( int iPz(0); iPz < nbPz; ++iPz )
       
	  map.insert( std::pair< Real, int >( (*dataPtr)( iPz, globalPtID ), iPz ) );
        
       assert( map.size() == nbPz );
      
       UInt offset = globalPtID * nbPz;
       
       std::vector< int >::iterator vectIt = sortedIDsByPoint.begin() + offset;
       
       //IMPROVE THIS
       for ( std::multimap< Real, int >::iterator mapIt( map.begin() ); mapIt != map.end(); ++mapIt )
       {
 	  (*vectIt) = mapIt->second;
 	  
 	  vectIt++;
       }     
   }
    
  // COMMUNICATING SORTED IDs
   for ( UInt iThread(0); iThread < nbThreads; ++iThread )
   {
       int bcastOffset, bcastNbPts;
       
       iThread == MASTER ? bcastOffset = 0 : bcastOffset = masterProcNbPts * nbPz + slaveProcNbPts * nbPz * ( iThread - 1 );
     
       iThread == MASTER ? bcastNbPts = masterProcNbPts * nbPz : bcastNbPts = slaveProcNbPts * nbPz;
    
       MPI_Bcast( & sortedIDsByPoint[0] + bcastOffset, bcastNbPts, MPI_INT, iThread, MPI_COMM_WORLD );
   }
 
   for ( UInt iPz(0); iPz < nbMyPz; ++iPz )
   {
       if( verbosity > 2 ) printf( "Proc %d is at %d / %d patients\n", myRank, iPz+1, nbMyPz );
     
       UInt comprisedCounter(0);
      
       int globalPzID;
     
       this->M_mpiUtilPtr->isMaster() ? globalPzID = iPz : globalPzID = masterProcNbPz  + ( myRank - 1 ) * slaveProcNbPz + iPz;
      
       for ( UInt iPt(0); iPt < nbPts; ++iPt )
       {
 	UInt offset = iPt * nbPz;
 	
	int howManyBefore(0);
	
	std::vector< int >::const_iterator it( sortedIDsByPoint.begin() + offset );
	
	while ( *it != globalPzID )
	{
	    howManyBefore++;
	    ++it;
	}
	
 	int howManyAfter = nbPz - howManyBefore - 1;
 	
 	comprisedCounter += howManyBefore * howManyAfter;
 	
       }
       
       this->M_BDs[ globalPzID ] = comprisedCounter / static_cast< Real > ( ( nbPts - 1 ) * this->binomial( nbPz - 1, 2 ) );
     
  }
   
   // COMMUNICATING BAND DEPTHS
    for ( UInt iThread(0); iThread < nbThreads; ++iThread )
   {
       int bcastOffset, bcastNbPz;
       
       iThread == MASTER ? bcastOffset = 0 : bcastOffset = masterProcNbPz + slaveProcNbPz * ( iThread - 1 );
     
       iThread == MASTER ? bcastNbPz = masterProcNbPz : bcastNbPz = slaveProcNbPz;
    
       MPI_Bcast( & this->M_BDs[0] + bcastOffset, bcastNbPz, MPI_DOUBLE_PRECISION, iThread, MPI_COMM_WORLD );
   }
   
   if ( verbosity > 2 && this->M_mpiUtilPtr->isMaster() ) 
 
     printf( " All depths have been gathered\n" );
  
   return;
   
}



}

#endif
