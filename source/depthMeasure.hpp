

#ifndef __DEPTH_MEASURE_HPP__
#define __DEPTH_MEASURE_HPP__

#include <source/HPCSDefs.hpp>

#include <source/bandDepth.hpp>
#include <source/bandDepthFactory.hpp>
#include <source/bandDepthData.hpp>
#include <source/extendedSort.hpp>
#include <source/mpi_utility.hpp>


namespace HPCS
{

  //! @class DepthMeasureBase this class implements a base class for the derived DepthMeasure classes.
  /*!
   * This class is meant to be a base class for the derived classes DepthMeasure< _J, _policy >, computing the depth
   * measures of a dataset of functional data.
   * 
   * @param _policy is the parameter specifying if the depths have to be computed testing eagh signal against all the others or
   * 		    against only a subset of the original dataset, called reference set. The values All and Reference of the _policy
   * 		    stand for these two behaviors.
   * 
   */
  template < BDPolicy _policy >
  class DepthMeasureBase
  {
  public:
    
    //!@name Public Types
    //@{

      //! Typedef for the BandDepthBase class
      typedef BandDepthBase< _policy > bdBase_Type;
    
      //! Typedef for the shared pointer to the band Depth class
      typedef boost::shared_ptr< bdBase_Type > bdBasePtr_Type;
    
      //! Typedef for the data type to setup the corresponding BandDepth object
      typedef typename bdBase_Type::bdData_Type bdData_Type;
      
      //! Typedef for the shared pointer to the data type
      typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
    
      //! Typedef for the mpi utility object
      typedef mpiUtility mpiUtility_Type;
    
      //! Typedef for the shared pointer to the mpi utility object.
      typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
 
    //@}  
    
    //! @name Constructor & Destructor
    //@{

      //! Default constructor
      DepthMeasureBase();
    
      //! Standard default destructor
      virtual ~DepthMeasureBase(){};

    //@}
    
    //! @name Setters & getters
    //@{

      //! Getter of the computed depths, template wrt the container type shared pointer  
      template < typename _containerType >
	void getDepths( boost::shared_ptr< _containerType > & containerPtr  ) const;

      //! Getter of the computed depths, template wrt the container type
      template < typename _containerType >
	void getDepths( _containerType & container ) const;

      //! Getter of the computed depths, template wrt the iterator type 
      template < typename _iteratorType >
	void getDepths( _iteratorType begin, _iteratorType end ) const;    
      
      //! Getter of the computed ranks, template wrt the container pointer type
      template < typename _containerType >
	void getRanks( boost::shared_ptr< _containerType > &  containerPtr ) const;

      //! Getter of the computed ranks, template wrt the container type
      template < typename _containerType >
	void getRanks( _containerType &  container ) const;

      //! Getter of the computed ranks, template wrt the iterator type
      template < typename _iteratorType >
	void getRanks( _iteratorType begin, _iteratorType end ) const;
      
      //! Setters of the band depth Data object, to setup BandDepth* objects 
      virtual void setBandDepthData( const bdDataPtr_Type & bdDataPtr ){};
    
    //@}
    
    //!@name Misc. Public Methods
    //@{

      //! Method to compute the depths, overridden in the derived classes  
      virtual void computeDepths(){};
      
      //! Method to compute the ranks, overridden in the derived classes
      virtual void computeRanks(){};
      
      //! Method to write the depths, overridden in the derived class
      virtual void writeDepths(){};
  
      //! Method to write the computed ranks to some output, overridden in the derived class
      virtual void writeRanks( std::ostream & output = std::cout ){};
	
      //! Method to write the computed ranks to some output filename, overridden in the derived class
      virtual void writeRanks( const std::string & outputFilename ){};
      
    //@}
     
  protected:

    //!@name Protected Types
    //@{

      //! Typedef of the depths container
      typedef std::vector< Real > depths_Type;
      
      //! Typedef of the shared pointer to the depths container
      typedef boost::shared_ptr< depths_Type > depthsPtr_Type;
      
      //! Typedef of the ranks container
      typedef std::vector< UInt > ranks_Type;
      
      //! Tyepedef of the shared pointer to the ranks container
      typedef boost::shared_ptr< ranks_Type > ranksPtr_Type;  
      
    //@}

    //!@name Protected Members
    //@{

      //! The shared pointer to the depths
      depthsPtr_Type M_depthsPtr;
      
      //! The shared pointer to the ranks
      ranksPtr_Type M_ranksPtr;

      //! The shared pointer to the mpi utility object
      mpiUtilityPtr_Type M_mpiUtilPtr;
    
    //@}
  private:
    
    
  };
  
  // Definition of the default constructor
  template < BDPolicy _policy >
  DepthMeasureBase< _policy >::
  DepthMeasureBase()
  :
  M_mpiUtilPtr( new mpiUtility_Type() ),
  M_depthsPtr( new depths_Type() ),
  M_ranksPtr( new ranks_Type() )
  {}
  
  // Definition of the getter of the depths from the template container pointer type
  template < BDPolicy _policy >
    template < typename _containerType >
    void
    DepthMeasureBase< _policy >::
    getDepths( boost::shared_ptr< _containerType > & containerPtr ) const
    {            
      assert( this->M_depthsPtr->size() != 0 );
	
      containerPtr->assign( this->M_depthsPtr->begin(), this->M_depthsPtr->end() );
	  
      return;
    }
  
  // Definition of the getter of the depths from the template container type
  template < BDPolicy _policy >
    template < typename _containerType >
    void
    DepthMeasureBase< _policy >::
    getDepths( _containerType & container ) const
    {
      assert( this->M_depthsPtr.size() != 0 );
	
      container->assign( this->M_depthsPtr->begin(), this->M_depthsPtr->end() );
	  
      return;
    }

  // Definition of the getter of the depths from the template iterator type
  template < BDPolicy _policy >
    template < typename _iteratorType >
    void
    DepthMeasureBase< _policy >::
    getDepths( _iteratorType begin, _iteratorType end ) const
    {
      assert( this->M_depthsPtr->size() != 0 );
	  
      std::copy( this->M_depthsPtr->begin(), this->M_depthsPtr->end(), begin );
	  
      return;
    }

  // Definition of the getter of the ranks from the template container ptr type
  template < BDPolicy _policy >
    template < typename _containerType >
    void
    DepthMeasureBase< _policy >::
    getRanks( boost::shared_ptr< _containerType > & containerPtr ) const
    {
      assert( this->M_ranksPtr.size() != 0 );
	
      containerPtr->assign( this->M_ranksPtr->begin(), this->M_ranksPtr->end() );
	  
      return;
    }
      
   // Definition of the getter of the ranks from the template container type
  template < BDPolicy _policy >
    template < typename _containerType >
    void
    DepthMeasureBase< _policy >::
    getRanks( _containerType & container ) const
    {
      assert( this->M_ranksPtr.size() != 0 );
	
      container.assign( this->M_ranksPtr->begin(), this->M_ranksPtr->end() );
	  
      return;
    }   
      
  // Definition of the getter of the ranks from the template iterator type
  template < BDPolicy _policy >
    template < typename _iteratorType >
    void
    DepthMeasureBase< _policy >::
    getRanks( _iteratorType begin, _iteratorType end ) const
    {
      assert( this->M_ranksPtr.size() != 0 );
      
      assert( begin != end );
	
      std::copy( this->M_ranksPtr->begin(), this->M_ranksPtr->end(), begin );
	    
      return;
    }
 
  //! @class DepthMeasure this class is the actual class meant to compute the depth measures of the dataset.
  /*!
   * This class is meant to compute the depth measure of a functional dataset in the two ways All and Reference.
   * See the report for a description of its use.
   */
  template < UInt _J, BDPolicy _policy  >
  class DepthMeasure : public DepthMeasureBase< _policy >
  {
  public:
    
    //!@name Public Types
    //@{
      
      //! Typedef of the band depth base type
      typedef BandDepthBase< _policy > bdBase_Type;
      
      //! Typedef of the shared pointer to the band depth base type
      typedef boost::shared_ptr< bdBase_Type > bdBasePtr_Type;
      
      //! Typedef of the proper band depth data type
      typedef typename bdBase_Type::bdData_Type bdData_Type;
      
      //! Typedef of the proper band depth data shared pointer type
      typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
    
    //@}
    
    //!@name Constructor & Destructor
    //@{
      
       //! Default constructor
       DepthMeasure();

       //! Constructor from a data object
       /*!
	* @param bdDataPtr is a shared pointer to a data object, that is used in the following to setup
	* 		   the band depth objects meant to compute the contributes needed to form the depths.
	*/
       DepthMeasure( const bdDataPtr_Type & bdDataPtr );
       
       //! Standard destructor
       virtual ~DepthMeasure(){};
	
    //@}

    //!@name Misc. Public Methods
    //@{
       
      //! Setter of the band depth data pointer type
      void setBandDepthData( const bdDataPtr_Type & bdDataPtr );

      //! Method to compute the depths.
      /*!
       * @note: this method is implemented in an external structure to specialize it for the values of policy.
       */
      void computeDepths();

      //! Method to compute the ranks according to the depths
      void computeRanks();
      
      //! Method to write the depths to the filename specified in the data object
      void writeDepths();
  
      //! Method to write the ranks to a specified output
      void writeRanks( std::ostream & output = std::cout );
	
      //! Method to write the ranks to a specified output filename
      void writeRanks( const std::string & outputFilename );

    //@}
    
  protected:
 
    //!@name Protected Types
    //@{
      
      //! Typedef to avoid the full qualification of depths
      typedef typename DepthMeasureBase< _policy >::depths_Type depths_Type;
      
      //! Typedef to avoid the full qualification of depths pointer      
      typedef typename DepthMeasureBase< _policy >::depthsPtr_Type depthsPtr_Type;
      
      //! Typedef to avoid the full qualification of ranks
      typedef typename DepthMeasureBase< _policy >::ranks_Type ranks_Type;
      
      //! Typedef to avoid the full qualification of ranks pointer
      typedef typename DepthMeasureBase< _policy >::ranksPtr_Type ranksPtr_Type;

    //@}  
      
    //! Shared pointer to the data object   
    bdDataPtr_Type M_bdDataPtr;
    
  private:
    
    //!@name Protected Types
    //@{
      
      //! Typedef for the extended sort type
      typedef ExtendedSort< Real > sort_Type;
      
      //! Typedef for the shared pointer to the sort pointer type
      typedef boost::shared_ptr< sort_Type > sortPtr_Type;
    
    //@}
      
    //! Shared pointer to the sorter object
    sortPtr_Type M_sortPtr;
      
   };
  
  // Definition of the default constructor
  template < UInt _J, BDPolicy _policy >
  DepthMeasure< _J, _policy >::
  DepthMeasure()
  :
  DepthMeasureBase< _policy >(),
  M_sortPtr( new sort_Type() )
  {
  }
  
  // Definition of the constructor from shared pointer to data object
  template < UInt _J, BDPolicy _policy >
  DepthMeasure< _J, _policy >::
  DepthMeasure( const bdDataPtr_Type & bdDataPtr )
  :
  DepthMeasureBase< _policy >(),
  M_bdDataPtr( bdDataPtr ),
  M_sortPtr( new sort_Type() )
  {
  }
  
  // Definition of the setter of the data object
  template< UInt _J, BDPolicy _policy >
  inline
  void
  DepthMeasure< _J, _policy >::
  setBandDepthData( const bdDataPtr_Type & bdDataPtr )
  {
      this->M_bdDataPtr = bdDataPtr;
      
      depths_Type().swap( (*this->M_depthsPtr) ); 
      
      ranks_Type().swap( (*this->M_ranksPtr) ); 
      
  }
  
  // Definition of the method to compute the ranks
  template < UInt _J, BDPolicy _policy >
  void
  DepthMeasure< _J, _policy >::
  computeRanks()
  {
      assert( this->M_depthsPtr->size() != 0 );
    
      this->M_ranksPtr->assign( this->M_depthsPtr->size(), 0 );
	  
      this->M_sortPtr->setData( this->M_depthsPtr->begin(), this->M_depthsPtr->end() );
	  
      this->M_sortPtr->getSortedIDs( this->M_ranksPtr );
    
      return;
  }
  
  // Definition of the method to write the depths to the filename specified by the data object
  template < UInt _J, BDPolicy _policy >
  void
  DepthMeasure< _J, _policy >::
  writeDepths()
  {
    if ( this->M_mpiUtilPtr->isMaster() )
    {
      std::ofstream output( this->M_bdDataPtr->outputFilename().data(), std::ios_base::out );
      
      typename depths_Type::const_iterator it;
      
      UInt count(0);
      
      for ( it = this->M_depthsPtr->begin(); it != this->M_depthsPtr->end(); ++it )
      {
	  output << count << "\t" << *it << std::endl;
	  
	  ++count;
      }
      
      output.close();
      
    }
    
    return;
  }

  // Definition of the method to write the ranks to a specified output
  template < UInt _J, BDPolicy _policy >
  void
  DepthMeasure< _J, _policy >::
  writeRanks( std::ostream & output )
  {
    if ( this->M_mpiUtilPtr->isMaster() )
    {      
      typename ranks_Type::const_iterator it;
      
      UInt count(0);
      
      for ( it = this->M_ranksPtr->begin(); it != this->M_ranksPtr->end(); ++it )
      {
	  output << count << "\t" << *it << std::endl;
	  
	  ++count;
      }
      
    }
    
    return;
  } 
  
  // Definition of the method to write the ranks to a specified output filename
  template < UInt _J, BDPolicy _policy >
  void
  DepthMeasure< _J, _policy >::
  writeRanks( const std::string & outputFilename )
  {
    if ( this->M_mpiUtilPtr->isMaster() )
    {      
      std::ofstream output( outputFilename.data(), std::ios_base::out );
      
      typename ranks_Type::const_iterator it;
      
      UInt count(0);
      
      for ( it = this->M_ranksPtr->begin(); it != this->M_ranksPtr->end(); ++it )
      {
	  output << count << "\t" << *it << std::endl;
	  
	  ++count;
      }
      
      output.close();
      
    }
    
    return;
  } 

  
  //! @class computeImplement this class is meant to give the implementation of the computeDepths method inside the DepthMeasure class.
  /*!
   *  This class is needed since DepthMeasure class cannot have a computeDepths() method common to both all ad reference values of policy,
   *  but it cannot be partially specialized without giving the specialization of the full class. Thus this class is created to allow for that
   *  different definition.
   */
  template < UInt _J, BDPolicy _policy >
  class computeImplement 
  {
  public:
    
    //! @name Public Types
    //@{
      
      //! Typedef for the band depth base type
      typedef BandDepthBase< _policy > bdBase_Type;
    
      //! Typedef for the shared pointer to the band depth base class
      typedef boost::shared_ptr< bdBase_Type > bdBasePtr_Type;
    
      //! Typedef for the proper data object
      typedef typename bdBase_Type::bdData_Type bdData_Type;
    
      //! Typedef for the shared pointer to the data object
      typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
      
      //! Typedef for the mpi utility object
      typedef mpiUtility mpiUtility_Type;
    
      //! Typedef for the shared pointer to the mpi utility object
      typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
      
      //! Typedef for the container type
      typedef std::vector< Real > container_Type;
      
      //! Typedef for the shared pointer to the container pointer type
      typedef boost::shared_ptr< container_Type > containerPtr_Type;
    
    //@}
      
    //!@name Misc. Public Methods
    //@{
     
      //! Constructor from a shared pointer to a data object
      computeImplement( const bdDataPtr_Type & bdDataPtr );
    
      //! Method that implements the compute method of DepthMeasure class      
      void compute();
   
      //! Getter of the computed depths
      void getComputedDepths( containerPtr_Type & contPtr );
    
   //@}  
  private:
    
      //!@name Private Types
      //@{

	//! Typedef for the band depth factory type
	typedef BDFactory< _policy > bdFactory_Type;

	//! Tyepdef for the shared pointer to the factory pointer type
	typedef boost::shared_ptr< bdFactory_Type > bdFactoryPtr_Type;

      //@}

      //! @name Private Members
      //@{
	
	//! Object containing the pointer to the band depth factory object
	bdFactoryPtr_Type M_bdFactoryPtr;
      
	//! Object containing the pointer to the data object
	bdDataPtr_Type M_bdDataPtr;
	
	//! Shared pointer to the mpi utility object
	mpiUtilityPtr_Type M_mpiUtilPtr;
	
	//! Shared pointer to the container type
	containerPtr_Type M_containerPtr;

     //@}
  };
  
  //! @class computeImplement this is the specialization of the computeImplement class to the All policy case.
  template < UInt _J >
  class computeImplement< _J, All > 
  {
  public:
    
     //! @name Public Types
    //@{
      
      //! Typedef for the band depth base type
      typedef BandDepthBase< All > bdBase_Type;
    
      //! Typedef for the shared pointer to the band depth base class
      typedef boost::shared_ptr< bdBase_Type > bdBasePtr_Type;
    
      //! Typedef for the proper data object
      typedef typename bdBase_Type::bdData_Type bdData_Type;
    
      //! Typedef for the shared pointer to the data object
      typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
      
      //! Typedef for the mpi utility object
      typedef mpiUtility mpiUtility_Type;
    
      //! Typedef for the shared pointer to the mpi utility object
      typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
      
      //! Typedef for the container type
      typedef std::vector< Real > container_Type;
      
      //! Typedef for the shared pointer to the container pointer type
      typedef boost::shared_ptr< container_Type > containerPtr_Type;
    
    //@}
    
     //!@name Misc. Public Methods
    //@{
     
      //! Constructor from a shared pointer to a data object
      computeImplement( const bdDataPtr_Type & bdDataPtr );
    
      //! Method that implements the compute method of DepthMeasure class      
      void compute();
   
      //! Getter of the computed depths
      void getComputedDepths( containerPtr_Type & contPtr );
    
   //@}  
      
  private:
    
      //!@name Private Types
      //@{

	//! Typedef for the band depth factory type
	typedef BDFactory< All > bdFactory_Type;

	//! Tyepdef for the shared pointer to the factory pointer type
	typedef boost::shared_ptr< bdFactory_Type > bdFactoryPtr_Type;

      //@}

      //! @name Private Members
      //@{
	
	//! Object containing the pointer to the band depth factory object
	bdFactoryPtr_Type M_bdFactoryPtr;
      
	//! Object containing the pointer to the data object
	bdDataPtr_Type M_bdDataPtr;
	
	//! Shared pointer to the mpi utility object
	mpiUtilityPtr_Type M_mpiUtilPtr;
	
	//! Shared pointer to the container type
	containerPtr_Type M_containerPtr;

     //@}
      
  };
  
  // Definition of the constructor from a sahred pointer to a data object.
  template < UInt _J >
  computeImplement< _J, All >::
  computeImplement( const bdDataPtr_Type & bdDataPtr )
  :
  M_bdDataPtr( bdDataPtr ),
  M_mpiUtilPtr( new mpiUtility_Type() )
  {
  }
  
  // Definition of the compute method
  template < UInt _J >
  void
  computeImplement< _J, All >::
  compute()
  {    
     bdBasePtr_Type bdBasePtr;

     bdFactoryPtr_Type bdFactoryPtr( new bdFactory_Type() );
     
     this->M_containerPtr.reset( new container_Type() );
     
     this->M_containerPtr->assign( this->M_bdDataPtr->nbPz(), 0 );
          
     std::vector< Real > currBDs;
     
     for ( UInt j(2); j <= _J; ++j )
     {	 
         if ( this->M_bdDataPtr->verbosity() > 2 && this->M_mpiUtilPtr->isMaster() )
	 {
	    printf( " PROCESSING BAND DEPTH J = %d\n", j );
	 }
       
	 bdBasePtr.reset( bdFactoryPtr->create( j ) );
       
	 bdBasePtr->setBandDepthData( this->M_bdDataPtr );
	  
	 bdBasePtr->computeBDs();
	  
	 bdBasePtr->getBDs( currBDs );
	 	  
	 for ( UInt iPz(0); iPz < this->M_bdDataPtr->nbPz(); ++iPz )
	 {
	     (*this->M_containerPtr)[ iPz ] += currBDs[ iPz ]; 
	 }
      }
      
      return;
  }
  
  // Definition of the getter of the computed depths
  template < UInt _J >
  void
  computeImplement< _J, All >::
  getComputedDepths( containerPtr_Type & contPtr )
  {
      contPtr = this->M_containerPtr;
      
      return;
  }
  
   //! @class computeImplement this is the specialization of the computeImplement class to the Reference policy case.
  template < UInt _J >
  class computeImplement< _J, Reference > 
  {
  public:
    
      //! @name Public Types
      //@{
      
      //! Typedef for the band depth base type
      typedef BandDepthBase< Reference > bdBase_Type;
    
      //! Typedef for the shared pointer to the band depth base class
      typedef boost::shared_ptr< bdBase_Type > bdBasePtr_Type;
    
      //! Typedef for the proper data object
      typedef typename bdBase_Type::bdData_Type bdData_Type;
    
      //! Typedef for the shared pointer to the data object
      typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
      
      //! Typedef for the mpi utility object
      typedef mpiUtility mpiUtility_Type;
    
      //! Typedef for the shared pointer to the mpi utility object
      typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
      
      //! Typedef for the container type
      typedef std::vector< Real > container_Type;
      
      //! Typedef for the shared pointer to the container pointer type
      typedef boost::shared_ptr< container_Type > containerPtr_Type;
    
    //@}
    
     //!@name Misc. Public Methods
    //@{
     
      //! Constructor from a shared pointer to a data object
      computeImplement( const bdDataPtr_Type & bdDataPtr );
    
      //! Method that implements the compute method of DepthMeasure class      
      void compute();
   
      //! Getter of the computed depths
      void getComputedDepths( containerPtr_Type & contPtr );
    
   //@}  
      
  private:
    
      //!@name Private Types
      //@{

	//! Typedef for the band depth factory type
	typedef BDFactory< Reference > bdFactory_Type;

	//! Tyepdef for the shared pointer to the factory pointer type
	typedef boost::shared_ptr< bdFactory_Type > bdFactoryPtr_Type;

      //@}

      //! @name Private Members
      //@{
	
	//! Object containing the pointer to the band depth factory object
	bdFactoryPtr_Type M_bdFactoryPtr;
      
	//! Object containing the pointer to the data object
	bdDataPtr_Type M_bdDataPtr;
	
	//! Shared pointer to the mpi utility object
	mpiUtilityPtr_Type M_mpiUtilPtr;
	
	//! Shared pointer to the container type
	containerPtr_Type M_containerPtr;

     //@}
      
      
  };
  
  //Definition of the constructor from a shared pointer to a data object
  template < UInt _J >
  computeImplement< _J, Reference >::
  computeImplement( const bdDataPtr_Type & bdDataPtr )
  :
  M_bdDataPtr( bdDataPtr ),
  M_mpiUtilPtr( new mpiUtility_Type() )
  {
  }
  
  // Definition of the compute method in the case of Reference policy
  template < UInt _J >
  void
  computeImplement< _J, Reference >::
  compute()
  {    
     bdBasePtr_Type bdBasePtr;

     bdFactoryPtr_Type bdFactoryPtr( new bdFactory_Type() );
     
     std::vector< Real > currBDs;
     
     this->M_containerPtr.reset( new container_Type() );
     
     this->M_containerPtr->assign( this->M_bdDataPtr->nbTestSamples(), 0 );
     
     for ( UInt j(2); j <= _J; ++j )
     {
       
         if ( this->M_bdDataPtr->verbosity() > 2 && this->M_mpiUtilPtr->isMaster() )
	 {
	    printf( " PROCESSING BAND DEPTH J = %d\n", j );
	 }
       
	 bdBasePtr.reset( bdFactoryPtr->create( j ) );
       
	 bdBasePtr->setBandDepthData( this->M_bdDataPtr );
	 
	 bdBasePtr->addToReferenceSet( 0, this->M_bdDataPtr->nbReferenceSamples(), this->M_bdDataPtr->seed() );
   
	 bdBasePtr->setTestSet();    
	  
	 bdBasePtr->computeBDs();
	  
	 bdBasePtr->getBDs( currBDs );
	 	 
	 std::vector< Real >::const_iterator it1;

 	 std::vector< Real >::iterator it2 = this->M_containerPtr->begin();;
	 
	 for ( it1 = currBDs.begin(); it1 != currBDs.end(); ++it1 )
	 {
	   
	    (*it2) += (*it1);
	    
	    it2++;
	 }
      }
      
      return;
  }
  
  // Definition of the getter for the computed depths
  template < UInt _J >
  void
  computeImplement< _J, Reference >::
  getComputedDepths( containerPtr_Type & contPtr )
  {
      contPtr = this->M_containerPtr;
      
      return;
  }

  
  // Definition of computeDepths di DepthMeasure
  template < UInt _J, BDPolicy _policy  >
  void
  DepthMeasure < _J, _policy >::
  computeDepths()
  {
     std::vector< UInt >().swap( (*this->M_ranksPtr) ); 
     
     computeImplement< _J, _policy > cmpt( this->M_bdDataPtr );
     
     cmpt.compute();
     
     cmpt.getComputedDepths( this->M_depthsPtr );
    
    return;
  }
  

  
 }



#endif /* __DEPTH_MEASURE_HPP__ */
