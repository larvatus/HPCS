
#ifndef __BANDDEPTH_HPP__
#define __BANDDEPTH_HPP__

#include <source/HPCSDefs.hpp>

#include <source/dataSet.hpp>
#include <source/bandDepthData.hpp>
#include <source/combinations.hpp>
#include <source/mpi_utility.hpp>


/*!
    @file BandDepth.hpp
    @brief

    @author Nicholas Tarabelloni
    @date 04 Aug 2013
    
    @mantainer Nicholas Tarabelloni
 */

namespace HPCS
{  

//! @class BandDepthBase this class Is the base class of the hierarchical structure of BandDepth objects.
/*!
 * This is the base class of the hierarchical structure of the Band Depth objects. It is specialized for
 * the values All and Reference of the enum type BDPolicy
 */
template < BDPolicy _policy >
class BandDepthBase
{
};


//! @class BandDepthBase< All > this class Is the specialization of BandDepthBase class for the value All of the enum type BDPolicy
/*!
 *  This is the full specialization of BandDepthBase class for the All value of BDPolicy enum type. It serves as a common base class
 *  from which to derive the objects meant to compute band depths of all the signals in the dataset. It is useful to create BandDepth objects through the corresponding factory.
 */
template <>
class BandDepthBase< All >
{
public:
    
    //! @name Public Types
    //@{
      
      //! Typedef for the corresponding band depth data object
      typedef BandDepthData bdData_Type;
  
      //! Typedef for the shared pointer to the band depth data object.
      typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
      
   //@} 
      
   //! @name Constructors & Destructor   
   //@{
    
    //! Default constructor
    BandDepthBase(){};
    
    //! Standard virtual destructor
    virtual ~BandDepthBase(){};
   
   //@} 
   
   //! @name Public methods
   //@{
    
    //! Method to compute the band depths, overridden in the derived classes
    virtual void computeBDs(){};
    
    //! Method to set the band depth data objects
    virtual void setBandDepthData( const bdData_Type & bdData ){};      

    //! Method to set the band depth data object from shared pointers
    virtual void setBandDepthData( const bdDataPtr_Type & bdDataPtr ){};
    
    //! Method to write the computed band depths to the output file specified in the BDData object.
    virtual void writeBDs() const {};
    
    //! Getter for the computed band depths
    virtual void getBDs( std::vector< Real > & bds ) const {};
 
   //@}
    
};

//! @class BandDepthBase< Ref > this class Is the specialization of BandDepthBase class for the value Reference of the enum type BDPolicy
/*!
 *  This is the full specialization of BandDepthBase class for the Reference value of BDPolicy enum type. It serves as a common base class
 * from which to derive the objects meant to compute the band depths of a dataset with respect to a reference subset. 
 * It is useful to create BandDepth objects through the corresponding factory.
 */
template <>
class BandDepthBase< Reference >
{
public:
    
    //! @name Public Types
    //@{
      
      //! Typedef for the corresponding band depth objects related to the Reference policy of band depths.
      typedef BandDepthRefData bdData_Type;
  
      //! Typedef for the shared pointer to band depth data objects.
      typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
   
    //@}
    
    //! @name Constructors & Destructor
    //@{
      
      //! Default constructor
      BandDepthBase(){};
    
      //! Standard virtual destructor
      virtual ~BandDepthBase(){};
    
    //@}
      
    //! @name Public methods
    //@{ 
    
      //! Method to compute the band depths with a reference set, overridden in the derived classes.
      virtual void computeBDs(){};
      
      //! Setter of the band depth data object for reference computation
      virtual void setBandDepthData( const bdData_Type & bdData ){};      

      //! Setter of the band depth data object for the reference computation
      virtual void setBandDepthData( const bdDataPtr_Type & bdDataPtr ){};
      
      //! Method to write the computed band depths to the output file specified in the band depth data object.
      virtual void writeBDs() const {};
      
      //! Getter of the computed band depths.
      virtual void getBDs( std::vector< Real > & bds ) const {};
      
      //! Method to randomly add samples to the reference set, overridden in the derived classes.
      virtual void addToReferenceSet( const UInt & levelID, const UInt & size, const UInt & seed = 1){};
      
      //! Method for the setup of the test set.
      virtual void setTestSet(){};
      
      //! Method to clear the reference set.
      virtual void clearReferenceSet(){};
   
    //@}   
};




//! @class BandDepth this class Implements an interface for the fast computation of Band Depths for functional data in the All case.
/*!
 *  This class implements an interface for the actual, fast computation of the band depths of a given dataset.
 *  It benefits from built in, parallel implementation exploting MPI. The corresponding band depths are computed
 *  testing each sample in the dataset with all the other samples, i.e. in a "All" fashio, which is expressed
 *  deriving this class from BandDepthBase< All >.
 *  The value _J used as a template parameter stands for the corresponding J value in the definition of the band depth
 *  (see the report, for details). It controls the size of the tuple constituting the envelope used to compute the depth
 *  of the current signal. 
 */  
template < UInt _J >
class BandDepth : public BandDepthBase< All >
{
public:
  
  //! @name Public Types
  //@{
  
    //! Typedef to avoid full qualification of band depth data object.
    typedef BandDepthBase< All >::bdData_Type bdData_Type;
    
    //! Typedef for the shared pointer to a band depth data objects.
    typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
  
    //! Typedef for the dataset object
    typedef DataSet dataSet_Type;
    
    //! Typedef for the shared pointer to a dataset object
    typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
  
    //! Typedef for the objects used to compute the combinations of samples IDs
    typedef CombinationsID combinationsID_Type;
    
    //! Tyepdef to avoid full qualificatio of tule types inside combinationsID class.
    typedef CombinationsID::tuple_Type tuple_Type;
  
    //! Typedef for the mpi utility object
    typedef mpiUtility mpiUtility_Type;
    
    //! Typedef for the shared pointer to a mpi utility object
    typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
  
  //@}
   
  //! @name Constructor & Destructor
  //@{
    
    //! Default constructor
    BandDepth();
    
    //! Constructor from a BandDepthData object
    BandDepth( const bdData_Type & bdData );
  
    //! Constructor from a shared pointer to BandDepthData object
    BandDepth( const bdDataPtr_Type & bdDataPtr );
  
    //! Standard virtual destructor
    virtual ~BandDepth(){};

  //@}
  
  //! @name Public Methods
  //@{
 
    //! Method for the computation of BDs. Computation is made in parallel exploiting MPI.
    virtual void computeBDs();
    
    //! Method to print the computed BDs.
    /*!
    * The computed BDs are written by MASTER processor
    * on the file specified by BandDepthData object.
    */
    void writeBDs() const;
    
    //! The method writing the BDs to an output file.
    void writeBDs( std::ostream & output ) const;
    
    //! Getter of the computed band depths    
    void getBDs(  std::vector< Real > & bds ) const; 
    
    //! Method to reset the BandDepthData object.
    /*!
    * It enables the re-use of the BandDepth object in order to compute other bandDepths, e.g. for
    * other datasets files. This is useful when a class contains a pointer to a BandDepth object that
    * may be used over different datasets.
    * 
    * \param bdData New BandDepthData object that will replace the old one.
    */
    void setBandDepthData( const bdData_Type & bdData );
    
    //! Method to reset the BandDepthData object.
    /*!
    * It enables the re-use of the BandDepth object in order to compute other bandDepths, e.g. for
    * other datasets files. This is useful when a class contains a pointer to a BandDepth object that
    * may be used over different datasets.
    * 
    * \param bdDataPtr is a shared pointer to the new bandDepthData object.
    */
    void setBandDepthData( const bdDataPtr_Type & bdDataPtr );
    
    
    //! Method for the setup or the resetting of the dataSet.
    /*!
    * The purpose of this method is to break the dependency of the object on
    * a specific input file containing data. This method can set up (or reset) the data set, 
    * if the "dimensions" agree with those expressed by
    * band depth data object.
    * This use is meant to pair with a constructor taking a Band Depth Data with no
    * input filename, and thus M_readDataFromFile flag equal to false.
    * 
    * \param dataPtr is the shared pointer to the new dataSet
    */
    void setDataSet( const dataSetPtr_Type & dataPtr );  
  
  //@}
  
protected:
  
  //! @name Protected members
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
  
private:
  
  //! @name Private methods
  //@{

    //! Each thread reads data from data files.
    void readData();
    
  //@}
    
};

 /////////////////////////
 // 	Band Depth
 /////////////////////////

// Default constructor
template < UInt _J >
BandDepth< _J >::
BandDepth()
:
M_mpiUtilPtr( new mpiUtility_Type() )
{
}

 
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
 
 // Reset BandDepthData type object contained 
 template < UInt _J >
 inline
 void 
 BandDepth< _J >::
 setBandDepthData( const bdDataPtr_Type & bdDataPtr )
 {
    this->M_bdDataPtr = bdDataPtr;

    this->M_mpiUtilPtr.reset( new mpiUtility_Type() );
    
    if ( bdDataPtr->readDataFromFile() ) this->readData();
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
void
BandDepth< _J >::
getBDs(  std::vector< Real > & bds ) const
{
    bds.assign( this->M_BDs.begin(), this->M_BDs.end() );
  
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
   
   const UInt nbPz 	= this->M_bdDataPtr->nbPz();
   const UInt nbPts	= this->M_bdDataPtr->nbPts();
   
   const UInt verbosity = this->M_bdDataPtr->verbosity();
   
   typedef dataSet_Type::dataPtr_Type dataSetPtr_Type;
   
   const dataSetPtr_Type dataPtr( this->M_dataSetPtr->getData() );  
            
   const UInt slaveProcNbPz = static_cast< UInt >( nbPz / nbThreads );
   const UInt masterProcNbPz = static_cast< UInt >( nbPz / nbThreads ) + static_cast< UInt >( nbPz % nbThreads );
   
   this->M_BDs.assign( nbPz, 0 );
   
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

	this->M_BDs[ iPz ] = comprisedCounter / static_cast< Real > ( ( nbPts - 1 ) * binomial( nbPz - 1, _J ) );
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

// Specialization of method computeBDs for the value J=2 of the parameters.
// It exploits the Genton algorithm, advanced for the case of J=2, to reduce the computational effort.
template <>
void
BandDepth< 2 >::
computeBDs()
{    
   const UInt myRank = this->M_mpiUtilPtr->myRank();
   const UInt nbThreads = this->M_mpiUtilPtr->nbThreads();
   
   const UInt nbPz 	= this->M_bdDataPtr->nbPz();
   const UInt nbPts	= this->M_bdDataPtr->nbPts();
   const UInt verbosity = this->M_bdDataPtr->verbosity();
   
   typedef dataSet_Type::dataPtr_Type dataSetPtr_Type;
   
   const dataSetPtr_Type dataPtr( this->M_dataSetPtr->getData() );  
            
   const UInt slaveProcNbPz = static_cast< UInt >( nbPz / nbThreads );
   const UInt masterProcNbPz = static_cast< UInt >( nbPz / nbThreads ) + static_cast< UInt >( nbPz % nbThreads );
   
   const UInt slaveProcNbPts = static_cast< UInt >( nbPts / nbThreads );
   const UInt masterProcNbPts = static_cast< UInt >( nbPts / nbThreads ) + static_cast< UInt >( nbPts % nbThreads );
     
   this->M_BDs.assign( nbPz, 0 );
   
   // FIRST STAGE:
  
   UInt nbMyPz, nbMyPts;
   
   this->M_mpiUtilPtr->isMaster() ? nbMyPz = masterProcNbPz : nbMyPz = slaveProcNbPz;
   this->M_mpiUtilPtr->isMaster() ? nbMyPts = masterProcNbPts : nbMyPts = slaveProcNbPts;
   
   std::vector< int > sortedIDsByPoint( nbPts * nbPz );
   
   for ( UInt iPt(0); iPt < nbMyPts; ++iPt )
   {
       UInt globalPtID;
     
       this->M_mpiUtilPtr->isMaster() ? globalPtID = iPt : globalPtID = masterProcNbPts  + ( myRank - 1 ) * slaveProcNbPts + iPt;
       
       // MODIFY THIS with a view + extendedSort
       
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
       
       this->M_BDs[ globalPzID ] = comprisedCounter / static_cast< Real > ( ( nbPts - 1 ) * binomial( nbPz - 1, 2 ) );
     
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
   
   MPI_Barrier( MPI_COMM_WORLD );
  
   return;
   
}



}

#endif
