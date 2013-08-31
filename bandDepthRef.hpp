

#ifndef __BANDDEPTH_REF_HPP__
#define __BANDDEPTH_REF_HPP__

#include <bandDepthData.hpp>
#include <bandDepth.hpp>
#include <mpi_utility.hpp>
#include <dataSet.hpp>

#include <HPCSDefs.hpp>

namespace HPCS
{
  
  template < UInt _J >
  class BandDepthRef : public BandDepthBase
  {
    
  public:
    
    typedef GetPot data_Type;
    
    typedef BandDepthData bdData_Type;
    
    typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
    
    typedef BandDepthRefData bdRefData_Type;
    
    typedef boost::shared_ptr< bdRefData_Type > bdRefDataPtr_Type;
    
    typedef DataSetLevelled dataSet_Type;
    
    typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
    
    typedef mpiUtility mpiUtility_Type;
    
    typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
    
    typedef std::set< UInt > IDContainer_Type;
    
    typedef boost::shared_ptr< IDContainer_Type > IDContainerPtr_Type;
    
    //! Default constructor
    BandDepth();
    
    BandDepthRef( const bdRefData_Type & bdRefData );
	
    ~BandDepthRef();
  
    void computeBDs();

    void setSeed( const UInt & seed );

    void setBandDepthData( const bdRefData_Type & bdRefData );
    
    //! To be used polymorphically.
//     void setBandDepthData( const bdDataPtr_Type & bdDataPtr );
  
    void setDataSet( const dataSetPtr_Type & dataPtr ); 

    void setReferenceSet( const UInt & size, const UInt & referenceLevel = 0 );
      
    //TODO MODIFICA: restituisce void e modifica.
    const IDContainer_Type & getReferenceSet() const;
    
    //TODO MODIFICA: restituisce void e modifica.
    const IDContainer_Type & getTestSet() const;

    //!TODO @todo MODIFICA! Tre template, rispetto al contenitore, alla coppia di iteratori e al puntatore al contenitore (?).
    void getBDs( std::vector< Real > & bds ) const;
    
    //! The method writing the BDs to the file name specified inside the BD Data Object.
    void writeBDs() const;
    
    //! TODO IMPLEMENTA!
    //! The method writing the BDs to an output file.
    void writeBDs( std::ostream & output ) const;
    
    //@}
     
  private:
    
    //! @name Private methods
    //@{

    //! Each thread reads data from data file.
    void readData();
    
    //! Each thread reads levels from levels file.
    void readLevels();
        
    //! Method allowing to determine the remaining part of dataSet other than the reference set, i.e. the "test set".
    void setTestSet();

    //@}

    //! @name Private members
    //@{
    
    //! Shared pointer to a BandDepthData type object
    bdRefDataPtr_Type M_bdRefDataPtr;
    
    //! Shared pointer to a dataSet type object, containing data.
    dataSetPtr_Type M_dataSetPtr;
    
    //! Computed band depths
    std::vector< Real > M_BDs;
    
    //! MPI utility pointer object
    mpiUtilityPtr_Type M_mpiUtilPtr;
    
//     //! Shared pointer to a bandDepth object, used as a tool to compute full bandDepths on reduced parts of data set.
//     bandDepthPtr_Type M_bandDepthPtr;
    
    //! Seed for the pseudo-random number generator
    UInt M_seed;
    
    //! Vector containing the IDs of the test set
    IDContainer_Type M_testSetIDs;
    
    //! Vector containing the IDs of the reference set
    IDContainer_Type M_referenceSetIDs;
    
    //! ID of the reference level
    UInt M_referenceLevel;
    
    //! Number of levels
    static const UInt S_nbLevels = 2 ;
    
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
   
    this->M_seed = 0;

    if ( bdRefData.readDataFromFile() ) this->readData();
    
    if ( bdRefData.readLevelsFromFile() ) this->readLevels();
   
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
 
  //! Method for resetting the Band Depth Data object with a polymorphic pointer
  template < UInt _J >
  void
  BandDepthRef< _J >::
  setBandDepthData( const bdDataPtr_Type & bdDataPtr )
  {
    
    //! @TODO SCIOGLI QUESTO NODO!
    this->M_bdRefDataPtr = bdDataPtr;
    
    
    return;
  }
  
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
 
  
  
  
  
  
  
  
//   /*!
//    * This class allow to compute the Band Depth of a functional data set with respect to a
//    * subset of the data set itself.
//    * 
//    * The data set used to build a BandDepthRef object is arranged into levels, 
//    * identifying the different groups among functional data.
//    * 
//    * Before computing, it is possible to specify both the size and the level from which building 
//    * the reference subset, that is used in the next to compute the Band Depth of the test set 
//    * (i.e. each of the remaining functional data).
//    * 
//    */
//   class BandDepthRef : public BandDepthBase
//   {
//   public:
//     
//     //! @name Typedefs
//     //@{
//     
//     typedef unsigned int UInt;	
//     
//     typedef double Real;
//     
//     typedef GetPot data_Type;
//     
//     typedef BandDepthData bdData_Type;
//     
//     typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
//     
//     typedef BandDepthRefData bdRefData_Type;
//     
//     typedef boost::shared_ptr< bdRefData_Type > bdRefDataPtr_Type;
//     
//     typedef DataSetLevelled dataSet_Type;
//     
//     typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
//     
//     typedef mpiUtility mpiUtility_Type;
//     
//     typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
//     
// //     typedef BandDepth bandDepth_Type;
//     
// //     typedef boost::shared_ptr< bandDepth_Type > bandDepthPtr_Type;
//     
//     typedef std::set< UInt > IDContainer_Type;
//     
//     typedef boost::shared_ptr< IDContainer_Type > IDContainerPtr_Type;
//     
//     //@}
//     
//     //! @name Constructors and Destructor
//     //@{
//     
//     BandDepthRef( const bdRefData_Type & bdRefData );
// 	
//     ~BandDepthRef();
//   
//     //@}
// 
//     //! @name Generic public methods
//     //@{
//       
//     //! The method for the computation of Band Depths, executing the computation in parallel.
//     void computeBDs();
//     
//     //@}
// 
//     //! @name Public Setters & Getters
//     //@{
// 
//     //! It allows to set a particular seed for the initialisation of the pseudo-random number generator.
//     void setSeed( const UInt & seed );
// 
//     //! Method for resetting the BandDepthData object.
//     /*!
//     * It enables the re-use of the BandDepthRef object in order to compute other bandDepths, e.g. for
//     * other input files. This is useful when a class contains a pointer to a BandDepth object that
//     * may be used over different datasets.
//     * 
//     * \param bdRefData New BandDepthData object that will replace the old one.
//     */
//     void setBandDepthData( const bdRefData_Type & bdRefData );
//     
//     //! To be used polymorphically.
// //     void setBandDepthData( const bdDataPtr_Type & bdDataPtr );
//   
//     //! Method for the setting up or the resetting of the dataSet.
//     /*!
//     * The purpose of this method is to break the dependency of the object on
//     * a specific data file containing data. With can set up (or reset) the data
//     * constituting the data set, if the "dimensions" agree with those expressed by
//     * band depth data object.
//     * This use is meant to pair with a constructor taking a Band Depth Data with no
//     * input filename nor levels filename, and thus M_readDataFromFile flag and 
//     * M_readLevelsFromFime equal to false.
//     * 
//     * \param dataPtr Pointer to the new dataSet.
//     */
//     void setDataSet( const dataSetPtr_Type & dataPtr ); 
//     
//     //! Method for choosing the reference set IDs in the population. 
//     /*! It launches a pseudo-random number generators determining the 
//     *  initial IDs of reference set (referenceSetIDs) of size specified
//     *  by "size" variable. The level ID of the reference set is specified by
//     *  "referenceLevel".
//     * 
//     * \param size Size of the reference subset
//     * \param referenceLevel ID of the reference level in dataSetLevelled data structure,
//     * 			    this will be stored in M_referenceLevel.
//     */
//     void setReferenceSet( const UInt & size, const UInt & referenceLevel = 0 );
//       
//     //! Getter of the reference set.
//     const IDContainer_Type & getReferenceSet() const;
//     
//     //! Getter of the test set.
//     const IDContainer_Type & getTestSet() const;
// 
//     //! Getter of the computed BDs
//     void getBDs( std::vector< Real > & bds ) const;
//     
//     //! The method writing the BDs to the file name specified inside the BD Data Object.
//     void writeBDs() const;
//     
//     //@}
//      
//   private:
//     
//     //! @name Private methods
//     //@{
// 
//     //! Each thread reads data from data file.
//     void readData();
//     
//     //! Each thread reads levels from levels file.
//     void readLevels();
//         
//     //! Method allowing to determine the remaining part of dataSet other than the reference set, i.e. the "test set".
//     void setTestSet();
// 
//     //@}
// 
//     //! @name Private members
//     //@{
//     
//     //! Shared pointer to a BandDepthData type object
//     bdRefDataPtr_Type M_bdRefDataPtr;
//     
//     //! Shared pointer to a dataSet type object, containing data.
//     dataSetPtr_Type M_dataSetPtr;
//     
//     //! Computed band depths
//     std::vector< Real > M_BDs;
//     
//     //! MPI utility pointer object
//     mpiUtilityPtr_Type M_mpiUtilPtr;
//     
// //     //! Shared pointer to a bandDepth object, used as a tool to compute full bandDepths on reduced parts of data set.
// //     bandDepthPtr_Type M_bandDepthPtr;
//     
//     //! Seed for the pseudo-random number generator
//     UInt M_seed;
//     
//     //! Vector containing the IDs of the test set
//     IDContainer_Type M_testSetIDs;
//     
//     //! Vector containing the IDs of the reference set
//     IDContainer_Type M_referenceSetIDs;
//     
//     //! ID of the reference level
//     UInt M_referenceLevel;
//     
//     //! Number of levels
//     static const UInt S_nbLevels = 2 ;
//     
//     //@}
//     
//     
//   };
//   
//   
}



#endif