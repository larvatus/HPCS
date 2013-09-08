

#ifndef __BANDDEPTH_REF_HPP__
#define __BANDDEPTH_REF_HPP__

#include <source/dataSet.hpp>
#include <source/mpi_utility.hpp>
#include <source/bandDepthData.hpp>
#include <source/bandDepth.hpp>
#include <source/extendedSort.hpp>
#include <source/combinations.hpp>

#include <source/HPCSDefs.hpp>

namespace HPCS
{
  
  
//! @class BandDepthRef this class Implements an interface for the fast computation of Band Depths for functional data in the Reference case.
/*!
 *  This class implements an interface for the actual, fast computation of the band depths of a given dataset.
 *  It benefits from built in, parallel implementation exploting MPI. The corresponding band depths are computed
 *  testing each sample in the dataset with a subset of reference samples belonging to the dataset, i.e. in a "Reference" fashio, which is expressed
 *  deriving this class from BandDepthBase< Reference >.
 *  The value _J used as a template parameter stands for the corresponding J value in the definition of the band depth
 *  (see the report, for details). It controls the size of the tuple constituting the envelope used to compute the depth
 *  of the current signal. 
 */  
  template < UInt _J >
  class BandDepthRef : public BandDepthBase< Reference >
  {
    
  public:
    
    //!@name Public Types
    //@{
      
      //! Typedef for the data type
      typedef BandDepthRefData bdRefData_Type;
      
      //! Typedef for a shared pointer to the band depth ref data type
      typedef boost::shared_ptr< bdRefData_Type > bdRefDataPtr_Type;
      
      //! Typedef for the dataset with levels
      typedef DataSetLevelled dataSet_Type;
      
      //! Typedef for the shared pointer to the data set type
      typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
      
      //! Typedef for the object containing MPI information
      typedef mpiUtility mpiUtility_Type;
      
      //! Typedef for the shared pointer to the mpi utility type
      typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
      
      //! Typedef for the container of signals' IDs
      typedef std::set< UInt > IDContainer_Type;
      
      //! Tyepdef for the shared pointer to the container of IDs
      typedef boost::shared_ptr< IDContainer_Type > IDContainerPtr_Type;
      
      //! Typedef for the object computing combinations
      typedef CombinationsID combinationsID_Type;
      
      //! Typedef for the tuple type inside the combination computer
      typedef CombinationsID::tuple_Type tuple_Type;
    //@}
    
    //! @name Constructors & Destructor  
    //@{
      
      //! Default constructor
      BandDepthRef();
    
      //! Constructor from data type
      BandDepthRef( const bdRefData_Type & bdRefData );
	
      //! Standard virtual destructor
      virtual ~BandDepthRef();
  
   //@}   
      
   //! @name Setters & Getters   
   //@{

    //! Setter of the data object
    void setBandDepthData( const bdRefData_Type & bdRefData );
    
    //! Setter of the data objcet from shared pointer
    void setBandDepthData( const bdDataPtr_Type & bdDataPtr );
  
    //! Setter of the dataset
    void setDataSet( const dataSetPtr_Type & dataPtr );
    
    template < typename _containerType >
      void getReferenceSetIDs( _containerType & idCont ) const;

    template < typename _containerType >
      void getReferenceSetIDs( boost::shared_ptr< _containerType > & idContPtr ) const;

    template < typename _iteratorType >
      void getReferenceSetIDs( _iteratorType begin, _iteratorType end ) const;

    template < typename _containerType >
      void getTestSetIDs( _containerType & idCont ) const;

    template < typename _containerType >
      void getTestSetIDs( boost::shared_ptr< _containerType > & idContPtr ) const;

    template < typename _iteratorType >
      void getTestSetIDs( _iteratorType begin, _iteratorType end ) const;

    //! Overrides the corresponding base class method
    void getBDs(  std::vector< Real > & bds ) const;
    
    //! The method writing the BDs to the file name specified inside the BD Data Object.
    void writeBDs() const;
    
    //! The method writing the BDs to an output file.
    void writeBDs( std::ostream & output ) const;
	

    //!@name Misc. Public Methods
    //@{
      
      virtual void computeBDs();

      //! Method to add signals to the group of reference samples
      /*!
       *  This method allows to increases the reference set by adding a number of elements equal to "size",
       *  taken from the level specified by levelID. If size is less than the cardinality of the specified
       *  levelID they are extracted randomly with a pseudo-random number generator, initialised by "seed".
       *  This method can be called in sequence, thus allowing to add to the reference set elements from different levels
       *  (up to a maximum number of nbReferenceSamples, contained inside the bdRefData object received during
       *  the setup of the present object. Once the reference set is full, the method "setTestset()" can be called
       *  to finalize the setup and fills the test-set with ALL the remaining samples. Depending on the use of this
       *  method, it may also contain elements from the levelID used to draw reference elements.
       * 
       * @param levelID is the level ID from which to take the samples added to the reference set.
       * @param size is the number of elements to add to the reference set.
       * @param seed is the seed to initialise the pseudo-random number generator
       * 
       */
      void addToReferenceSet( const UInt & levelID, const UInt & size, const UInt & seed = 1 );
      
      //! Method to clear the reference set
      /*!
       * This method clears the reference set from the previously added elements.
       */
      void clearReferenceSet();
      
      //! Method for the setup of the test setBandDepthData
      /*!
       *  This method ideally finalizes the creation of reference set and builds the test set by drawing from the dataset
       *  all the elements not assigned to the reference set (also from the levels to which its data belong) and adds them 
       *  to the test set.
       */
      void setTestSet();
    
    //@}
     
  protected:


    //! @name Protected members
    //@{
      
      //! Shared pointer to a BandDepthData type object
      bdRefDataPtr_Type M_bdRefDataPtr;
      
      //! Shared pointer to a dataSet type object, containing data.
      dataSetPtr_Type M_dataSetPtr;
      
      //! Computed band depths
      std::vector< Real > M_BDs;
      
      //! MPI utility pointer object
      mpiUtilityPtr_Type M_mpiUtilPtr;

      //! Vector containing the IDs of the test set
      IDContainer_Type M_testSetIDs;
      
      //! Vector containing the IDs of the reference set
      IDContainer_Type M_referenceSetIDs;
      
      //! Number of levels
      static const UInt S_nbLevels = 2 ;

    //@}
    
  private:
       
    //! @name Private methods
    //@{

      //! Each thread reads data from data file.
      void readData();
      
      //! Each thread reads levels from levels file.
      void readLevels();

    //@}
    
  };
  
  // Default constructor
  template < UInt _J >
  BandDepthRef< _J >::
  BandDepthRef()
  :
  M_mpiUtilPtr( new mpiUtility_Type() )
  {
  }

 // Stadard destructor
 template < UInt _J >
 BandDepthRef< _J >::
 ~BandDepthRef()
 {
 }
  
 // Constructor from single variables.
 template < UInt _J >
 BandDepthRef< _J >::
 BandDepthRef( const bdRefData_Type & bdRefData )
 {
    this->M_bdRefDataPtr.reset( new bdRefData_Type( bdRefData ) );
   
    this->M_mpiUtilPtr.reset( new mpiUtility_Type() );

    if ( bdRefData.readDataFromFile() ) this->readData();
    
    if ( bdRefData.readLevelsExtremaFromFile() )  this->readLevels();
     
 }

 // Method for reading data from input file
 template < UInt _J >
 void
 BandDepthRef< _J >::
 readData()
 {   
    const UInt nbLevels = BandDepthRef::S_nbLevels;
   
    this->M_dataSetPtr.reset( new dataSet_Type( M_bdRefDataPtr->nbPz(), M_bdRefDataPtr->nbPts(), nbLevels ) );

    this->M_dataSetPtr->setOffset(  M_bdRefDataPtr->leftOffset(), M_bdRefDataPtr->rightOffset() );
    
    this->M_dataSetPtr->readData( M_bdRefDataPtr->inputFilename() );
   
    return;
 }
 
 // Method for reading levels from input file
 template < UInt _J >
 inline
 void
 BandDepthRef< _J >::
 readLevels()
 {   
    this->M_dataSetPtr->setLevelsFromExtrema( M_bdRefDataPtr->levelsExtremaFilename() );    
    
    return;
 }
 
 // Method for resetting the Band Depth Data object
 template < UInt _J >
 void
 BandDepthRef< _J >::
 setBandDepthData( const bdRefData_Type & bdRefData )
 {
   
    this->M_bdRefDataPtr.reset( new bdRefData_Type( bdRefData ) );
    
    if ( bdRefData.readDataFromFile() ) this->readData();
    
    if ( bdRefData.readLevelsExtremaFromFile() ) this->readLevels();
   
    return;   
 }
 
 // Method for resetting the Band Depth Data object
 template < UInt _J >
 void
 BandDepthRef< _J >::
 setBandDepthData( const bdRefDataPtr_Type & bdRefDataPtr )
 {   
    this->M_bdRefDataPtr = bdRefDataPtr;
    
    if ( bdRefDataPtr->readDataFromFile() ) this->readData();
    
    if ( bdRefDataPtr->readLevelsExtremaFromFile() ) this->readLevels();
   
    return;   
 }
 
 // Reset dataSet pointer. This allow using the object without a file from which to read.
 template < UInt _J >
 void
 BandDepthRef< _J >::
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
 
 // Method to add elements to the reference set
 template < UInt _J >
 void
 BandDepthRef< _J >::
 addToReferenceSet ( const UInt & levelID, const UInt & size, const UInt & seed )
 {
     assert( size <= this->M_dataSetPtr->cardinality( levelID ) );

     srand48( seed );
           
     for ( UInt iInsert(0); iInsert < size; )
     {	
	UInt temp = static_cast< UInt >( this->M_dataSetPtr->cardinality( levelID ) * drand48() );
	
	if ( temp != this->M_dataSetPtr->cardinality( levelID ) )
	{
	
	   for ( UInt iLevel(0); iLevel < levelID; ++iLevel )
	   {
	      temp += this->M_dataSetPtr->cardinality( levelID );
	   }
	    
	   if ( this->M_referenceSetIDs.insert( temp ).second == true )
	   {
	      ++iInsert;
	   }
	}
      }

    return;
 }
 
 // Method to clear the reference set
 template < UInt _J >
 void
 BandDepthRef< _J >::
 clearReferenceSet ()
 {
     this->M_referenceSetIDs.clear();
     
     this->M_testSetIDs.clear();
     
     return;
 }
    
 // Method to finalize the setup of the refernece and test set.
 template < UInt _J >
 void
 BandDepthRef< _J >::
 setTestSet()
 {
    for( UInt iSample(0); iSample < this->M_bdRefDataPtr->nbPz(); ++iSample )
    {
      if ( this->M_referenceSetIDs.find( iSample ) == this->M_referenceSetIDs.end() )
      {
	this->M_testSetIDs.insert( iSample );	
      }
    }

    return;
 }
 
 // Getter of the reference set IDs.
 template < UInt _J >
 template < typename _containerType >
 inline
 void
 BandDepthRef< _J >::
 getReferenceSetIDs( _containerType & idCont ) 
 const
 {    
     idCont.assign( this->M_referenceSetIDs.begin(), this->M_referenceSetIDs.end() );
 }
    
 // Getter of the reference set IDs.
  template < UInt _J >
   template < typename _containerType >
   inline
   void
   BandDepthRef< _J >::
   getReferenceSetIDs( boost::shared_ptr< _containerType > & idContPtr )
   const
   {
	idContPtr->assign( this->M_referenceSetIDs.begin(), this->M_referenceSetIDs.end() );
   }
    
    // Getter of the reference set IDs.
  template < UInt _J >
   template < typename _iteratorType >
   void
   BandDepthRef< _J >::
   getReferenceSetIDs( _iteratorType begin, _iteratorType end )
   const
   {
	assert( begin != end );
	
	std::copy( this->M_referenceSetIDs.begin(), this->M_referenceSetIDs.end(), begin );
   }
 
  // Getter of the test set IDs.
  template < UInt _J >
   template < typename _containerType >
   inline
   void 
   BandDepthRef< _J >::
   getTestSetIDs( _containerType & idCont ) 
   const
   {
	idCont.assign( this->M_testSetIDs.begin(), this->M_testSetIDs.end() );
   }
	
  // Getter of the test set IDs.
  template < UInt _J >
   template < typename _containerType >
   void
   BandDepthRef< _J >::
   getTestSetIDs( boost::shared_ptr< _containerType > & idContPtr )
   const
   {
      idContPtr->assign( this->M_testSetIDs.begin(), this->M_testSetIDs.end() );
   }

  // Getter of the test set IDs.
  template < UInt _J >
   template < typename _iteratorType >
   void
   BandDepthRef< _J >::
   getTestSetIDs( _iteratorType begin, _iteratorType end ) 
   const
   {
      assert( begin != end );
      
      std::copy( this->M_testSetIDs.begin(), this->M_testSetIDs.end(), begin );
   }
   
  // Getter of the BDs
  template < UInt _J >
  inline
  void
  BandDepthRef< _J >::
  getBDs( std::vector< Real > & container ) 
  const
  {     
      container.assign( this->M_BDs.begin(), this->M_BDs.end() );
	
      return;
  }
   
  // The method writing the BDs to the output. 
  template < UInt _J >
  void
  BandDepthRef< _J >::
  writeBDs() 
  const
  {
      if ( this->M_mpiUtilPtr->isMaster() );
      {
	std::ofstream output( this->M_bdRefDataPtr->outputFilename().data(), std::ios_base::out );
	
	typedef IDContainer_Type::const_iterator IDiter_Type;
	
	typedef std::list< std::pair< UInt, Real > > list_Type;
	
	typedef list_Type::const_iterator listIt_Type;
	
	list_Type bdList;
		
	UInt iBD(0);
	
	for ( IDiter_Type it = this->M_testSetIDs.begin(); it != this->M_testSetIDs.end(); ++it )
	{
	    bdList.push_back( std::make_pair< UInt, Real >( *it, this->M_BDs[ iBD ] ) );
	    
	    ++iBD;
	}
	
	bdList.sort( LessThanPairFirst< UInt, Real >() );	
	
	for ( listIt_Type it = bdList.begin(); it != bdList.end(); ++it )
	{
	  
   	    output << it->first << "\t" << it->second << std::endl;

	}
	
	output.close();
      }
      
      return;
  }

 // The method to compute the band depths, in the general case (i.e. for _J > 2 ). 
 template < UInt _J >
 void
 BandDepthRef< _J >::
 computeBDs()
 {
   const UInt myRank = this->M_mpiUtilPtr->myRank();
   const UInt nbThreads = this->M_mpiUtilPtr->nbThreads();
   
   const UInt nbSamples = this->M_bdRefDataPtr->nbPz();
   const UInt nbRefSamples = this->M_referenceSetIDs.size();
   const UInt nbTestSamples = this->M_testSetIDs.size();
   
   const UInt nbPts = this->M_bdRefDataPtr->nbPts();
   
   const UInt verbosity = this->M_bdRefDataPtr->verbosity();
   
   typedef dataSet_Type::dataPtr_Type dataSetPtr_Type;
   
   typedef IDContainer_Type::const_iterator iter_Type;
   
   const dataSetPtr_Type dataPtr( this->M_dataSetPtr->getData() );  
            
   const UInt slaveProcNbSamples = static_cast< UInt >( nbTestSamples / nbThreads );
   const UInt masterProcNbSamples = static_cast< UInt >( nbTestSamples / nbThreads ) + static_cast< UInt >( nbTestSamples % nbThreads );
   
   this->M_BDs.assign( nbTestSamples, 0 );
   
   // FIRST STAGE:
   
   combinationsID_Type combinationsID( nbRefSamples, _J, true );
   
   combinationsID.generateCombinationsID();
  
   UInt nbMySamples;
   
   this->M_mpiUtilPtr->isMaster() ? nbMySamples = masterProcNbSamples : nbMySamples = slaveProcNbSamples;
   
   for ( UInt iSample(0); iSample < nbMySamples; ++iSample )
   {
      if( verbosity > 2 ) printf( "Proc %d is at %d / %d samples\n", myRank, iSample + 1, nbMySamples );
      
      UInt nbOffsets;
	
      this->M_mpiUtilPtr->isMaster() ? nbOffsets = iSample : nbOffsets = masterProcNbSamples + ( myRank - 1 ) * slaveProcNbSamples + iSample; 
	
      IDContainer_Type::const_iterator globalSampleIDIter = this->M_testSetIDs.begin();
	
      std::advance( globalSampleIDIter, nbOffsets );
      
      Real comprisedCounter(0);
      
      combinationsID.resetPointerToHeadCombination();      
      
      while( not( combinationsID.hasTraversedAll() ) )
      {
	tuple_Type pzTupleIDs;

	combinationsID.getNextCombinationID( pzTupleIDs );
		
	// IMPORTANT: mapping the IDs of the tuple to the IDs of the reference sub-population
	for ( UInt iJ(0); iJ < _J; ++iJ )
	{	  
	  IDContainer_Type::const_iterator it = this->M_referenceSetIDs.begin();
	  
	  std::advance( it, pzTupleIDs[ iJ ] );
	  
	  pzTupleIDs[ iJ ] = *it;  	  
	}
	
	std::vector< Real > currentValues( _J );
	
	Real envMax, envMin, sampleVal;
	
	for ( UInt iPt(0); iPt < nbPts; ++iPt )
	{
	  for ( UInt iJ(0); iJ < _J; ++iJ )
	  {		
	    currentValues[ iJ ] = (*dataPtr)( pzTupleIDs[ iJ ], iPt );
	  } 
	  
	  envMax =  *( std::max_element( currentValues.begin(), currentValues.end() ) );
	  envMin =  *( std::min_element( currentValues.begin(), currentValues.end() ) );
	  sampleVal = (*dataPtr)( *globalSampleIDIter, iPt );
	  
	  if ( sampleVal <= envMax && sampleVal >= envMin ) ++comprisedCounter;  
  
	}
      }

	this->M_BDs[ iSample ] = comprisedCounter / static_cast< Real > ( ( nbPts - 1 ) * binomial( nbRefSamples, _J ) );
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
    
    MPI_Barrier( MPI_COMM_WORLD );
    
    return;
 }
  
 // The method to compute the band depths in the case of J=2
 template <>
 void
 BandDepthRef< 2 >::
 computeBDs()
 {
   const UInt myRank = this->M_mpiUtilPtr->myRank();
   const UInt nbThreads = this->M_mpiUtilPtr->nbThreads();
   
   const UInt nbSamples = this->M_bdRefDataPtr->nbPz();
   const UInt nbRefSamples = this->M_referenceSetIDs.size();
   const UInt nbTestSamples = this->M_testSetIDs.size();
   
   const UInt nbPts = this->M_bdRefDataPtr->nbPts();
   
   const UInt verbosity = this->M_bdRefDataPtr->verbosity();
   
   typedef dataSet_Type::dataPtr_Type dataSetPtr_Type;
   
   typedef IDContainer_Type::const_iterator iter_Type;
   
   const dataSetPtr_Type dataPtr( this->M_dataSetPtr->getData() );  
            
   const UInt slaveProcNbSamples = static_cast< UInt >( nbTestSamples / nbThreads );
   const UInt masterProcNbSamples = static_cast< UInt >( nbTestSamples / nbThreads ) + static_cast< UInt >( nbTestSamples % nbThreads );
   
   const UInt slaveProcNbPts = static_cast< UInt >( nbPts / nbThreads );
   const UInt masterProcNbPts = static_cast< UInt >( nbPts / nbThreads ) + static_cast< UInt >( nbPts % nbThreads );
   
   this->M_BDs.assign( nbTestSamples, 0 );
   
   // FIRST STAGE:
  
   UInt nbMySamples, nbMyPts;
   
   this->M_mpiUtilPtr->isMaster() ? nbMySamples = masterProcNbSamples : nbMySamples = slaveProcNbSamples;
   
   this->M_mpiUtilPtr->isMaster() ? nbMyPts = masterProcNbPts : nbMyPts = slaveProcNbPts;
    
   for ( UInt iPt(0); iPt < nbMyPts; ++iPt )
   {
       if( verbosity > 2 && ( iPt % 50  == 0 ) ) printf( "Proc %d is at %d / %d points\n", myRank, iPt + 1, nbMyPts );
       
       UInt globalPtID;
	    
       this->M_mpiUtilPtr->isMaster() ? globalPtID = iPt : globalPtID = masterProcNbPts + ( myRank - 1 ) * slaveProcNbPts + iPt; 
     
       std::vector< Real > referenceSetTimes;
       
       referenceSetTimes.reserve( nbRefSamples );
       
       for ( iter_Type it( this->M_referenceSetIDs.begin() ); it != this->M_referenceSetIDs.end(); ++it )
       {
	  referenceSetTimes.push_back( (*dataPtr)( *it, globalPtID ) );  
       }
       
       UInt iCurrSample(0);
       
       for ( iter_Type it( this->M_testSetIDs.begin() ); it != this->M_testSetIDs.end(); ++it )
       {	 	 
	 const Real timeCurr( (*dataPtr)( *it, globalPtID ) ); 
	 
	 const UInt howManyBefore = std::count_if( referenceSetTimes.begin(), referenceSetTimes.end(), std::bind2nd( std::less< Real >(), timeCurr ) );
	 
	 const UInt howManyAfter( nbRefSamples - howManyBefore );
	 
 	 this->M_BDs[ iCurrSample ] += howManyBefore * howManyAfter / static_cast< Real > ( ( nbPts - 1 ) * binomial( nbRefSamples, 2 ) ) ;
	 
	 ++iCurrSample;
 	
       }
   } 

   // NOW REDUCING, BUT WITH A LARGE COMMUNICATION, RATHER THAN A VECTOR OF SINGLE
   
   for ( UInt iThread(1); iThread < nbThreads; ++iThread )
   {
      if ( myRank == MASTER )  //! I'm MASTER
      {
	MPI_Status status;
	  
	  std::vector< Real > temp( nbTestSamples );

	  MPI_Recv( & temp[0], nbTestSamples, MPI_DOUBLE_PRECISION, iThread, iThread, MPI_COMM_WORLD, & status );
	  
	  for ( UInt iSample(0); iSample < nbTestSamples; ++iSample )
	  {
	      this->M_BDs[ iSample ] += temp[ iSample ];
	  }	  
      }
      else if ( myRank == iThread )
      {
	  MPI_Send( & this->M_BDs[0], nbTestSamples, MPI_DOUBLE_PRECISION, MASTER, iThread, MPI_COMM_WORLD );
      }
   }
   
   MPI_Bcast( & this->M_BDs[0], nbTestSamples, MPI_DOUBLE_PRECISION, MASTER, MPI_COMM_WORLD );
   
   if ( verbosity > 2 && this->M_mpiUtilPtr->isMaster() ) 
   {
      printf( " Depths of the %d test samples have been gathered\n", nbTestSamples );
   }
   MPI_Barrier( MPI_COMM_WORLD );
      
   return;   
   
  }
}


#endif