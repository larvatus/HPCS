
#ifndef __MULTI_DEPTH_MEASURE_HPP__
#define __MULTI_DEPTH_MEASURE_HPP__

#include <source/HPCSDefs.hpp>
#include <source/depthMeasureFactory.hpp>
#include <source/depthMeasure.hpp>
#include <source/extendedSort.hpp>

namespace HPCS
{
  
  //! @class MultiDepthMeasureBase this class implements a base class from which to derive the actual classes mean to compute the multivariate depths measure of a functional dataset.
  template < BDPolicy _policy >
  class MultiDepthMeasureBase
  {
  public:
    
    //! Typedef for the GetPot type
    typedef GetPot getPot_Type;
     
    //! @name Misc. Public Types
    //@{

    //! Default constructor
    MultiDepthMeasureBase();
    
    //! Default destructor
    virtual ~MultiDepthMeasureBase(){};
    
    //! Method to add dimension to the current object.
    virtual void addDimension( const getPot_Type & dataFile, const std::string & section ){};
    
    //! Method to compute the depths
    virtual void computeMultiDepths(){};
    
    //! Method to compute the ranks
    virtual void computeRanks(){};
    
    //! Method to write the depths to the specified filename
    virtual void writeMultiDepths(){};
    
    //! Method to write ranks to some output
    virtual void writeRanks( std::ostream & output = std::cout ){};
      
    //! Method to write the ranks to some output file
    virtual void writeRanks( const std::string & outputFilename ){};

    //@}
    

    //! @name Setters & Getters
    //@{
      
      //! Setter of the weights from a getpot variable
      virtual void setWeights( const getPot_Type & dataFile, const std::string & section ){};
    
      //! Setter of the data from a getpot file
      virtual void setData( const getPot_Type & dataFile, const std::string & section ){};

      //! Setter of the weights from a shared pointer to a container    
      template < typename _containerType >
	void setWeights( const boost::shared_ptr< _containerType > &  containerPtr );

      //! Setter of the weights from a container
      template < typename _containerType >
	void setWeights( const _containerType &  container );

      //! Setter of the weights from a container in terms of iterators
      template < typename _iteratorType >
	void setWeights( const _iteratorType begin, _iteratorType end );
    
      //! Getter of depths from a shared pointer to a container
      template < typename _containerType >
	void getMultiDepths( boost::shared_ptr< _containerType > & containerPtr  ) const;

      //! Getter of depths from a container
      template < typename _containerType >
	void getMultiDepths( _containerType & container ) const;

      //! Getter of depths into a structure given by iterators
      template < typename _iteratorType >
	void getMultiDepths( _iteratorType begin, _iteratorType end ) const;    
    
      //! Getter of the ranks from a shared pointer to a container
      template < typename _containerType >
	void getMultiRanks( boost::shared_ptr< _containerType > &  containerPtr ) const;

      //! Getter of the ranks from the container
      template < typename _containerType >
	void getMultiRanks( _containerType &  container ) const;

      //! Getter of the ranks into a structure given by iterators
      template < typename _iteratorType >
	void getMultiRanks( _iteratorType begin, _iteratorType end ) const;
      
      //! Getter of weights from shared pointer to a container
      template < typename _containerType >
	void getWeights( boost::shared_ptr< _containerType > &  containerPtr ) const;

      //! Getter of weights from a container
      template < typename _containerType >
	void getWeights( _containerType &  container ) const;

      //! Getter of weights into a structure given by iterators
      template < typename _iteratorType >
	void getWeights( _iteratorType begin, _iteratorType end ) const;
      
    //@}
    
  protected:
    
    //!@name Protected Types
    //@{

      //! Typedef of the depths container
      typedef std::vector< Real > depths_Type;
      
      //! Typedef of the shared pointer to the depths type
      typedef boost::shared_ptr< depths_Type > depthsPtr_Type;
      
      //! Typedef of the ranks container
      typedef std::vector< UInt > ranks_Type;
      
      //! Typedef of the shared pointer to the ranks container
      typedef boost::shared_ptr< ranks_Type > ranksPtr_Type;
      
      //! Typedef ot the mpi utility type
      typedef mpiUtility mpiUtility_Type;
    
      //! Typedef of the shared pointer to the mpi utility type
      typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
      
      //! Typedef of the weights container
      typedef std::vector< Real > weights_Type;
      
      //! Typedef of the shared pointer to the weights container
      typedef boost::shared_ptr< weights_Type > weightsPtr_Type;
            
    //@}
    
    //!@name Protected Members
    //@{
 
      //! Object for the mpi utility
      mpiUtilityPtr_Type M_mpiUtilPtr;
    
      //! Object for the depths container
      depthsPtr_Type M_depthsPtr;
      
      //! Objects for the ranks container
      ranksPtr_Type M_ranksPtr;
    
      //! Objects for the weights container
      weightsPtr_Type M_weightsPtr;
      
    //@}
    
  private:
    
  };
  
  // Definition of the default constructor
  template < BDPolicy _policy >
  MultiDepthMeasureBase< _policy >::
  MultiDepthMeasureBase()
  :
  M_mpiUtilPtr( new mpiUtility_Type() ),
  M_depthsPtr( new depths_Type() ),
  M_ranksPtr( new ranks_Type() ),
  M_weightsPtr( new weights_Type() )
  {
  } 

  // Definition of a setter of the weights
  template < BDPolicy _policy >
    template < typename _containerType >
    void 
    MultiDepthMeasureBase< _policy >::
    setWeights( const boost::shared_ptr< _containerType > & containerPtr )
    {
	assert( containerPtr->size() != 0 );
      
	this->M_weightsPtr->assign( containerPtr->begin(), containerPtr->end() );
      
	return;
    }
  
  // Definition of a setter of the weights
  template < BDPolicy _policy >
    template < typename _containerType >
    void 
    MultiDepthMeasureBase< _policy >::
    setWeights( const _containerType &  container )
    {
	assert( container.size() != 0 );
	
	this->M_weightsPtr->assign( container.begin(), container.end() );
      
	return;
    }
   
    // Definition of a setter of the weights
    template < BDPolicy _policy >
      template < typename _iteratorType >
      void
      MultiDepthMeasureBase< _policy >::
      setWeights( const _iteratorType begin, _iteratorType end )
      {
	  assert( begin != end );
	  
	  this->M_weightsPtr->assign( begin, end );
	
	  return;
      }
    
    // Definition of a getter of the depths
    template < BDPolicy _policy >
      template < typename _containerType >
      void
      MultiDepthMeasureBase< _policy >::
      getMultiDepths( boost::shared_ptr< _containerType > & containerPtr ) const
      {
	  assert( this->M_depthsPtr->begin() != this->M_depthsPtr->end() );
	
	  containerPtr->assign( this->M_depthsPtr->begin(), this->M_depthsPtr->end() );
	  
	  return;
      }
    
    // Definition of a getter of the depths
    template < BDPolicy _policy >
      template < typename _containerType >
      void
      MultiDepthMeasureBase< _policy >::
      getMultiDepths( _containerType & container ) const
      {
	  assert( this->M_depthsPtr->begin() != this->M_depthsPtr->end() );
	  
	  container.assign( this->M_depthsPtr->begin(), this->M_depthsPtr->end() );
	  
	  return;
	  
      }
      
    // Definition of a getter of the depths
    template < BDPolicy _policy >
      template < typename _iteratorType >
      void
      MultiDepthMeasureBase< _policy >::
      getMultiDepths( _iteratorType begin, _iteratorType end) const
      {
	assert( this->M_depthsPtr->begin() != this->M_depthsPtr->end() );
	
	assert( begin != end );
	
	std::copy( this->M_depthsPtr->begin(), this->M_depthsPtr->end(), begin );
	
	return;
	
      }
      
    // Definition of a getter of the ranks
    template < BDPolicy _policy >
      template < typename _containerType >
      void
      MultiDepthMeasureBase< _policy >::
      getMultiRanks( boost::shared_ptr< _containerType > & containerPtr ) const
      {
	  assert( this->M_ranksPtr->begin() != this->M_ranksPtr->end() );
	
	  containerPtr->assign( this->M_ranksPtr->begin(), this->M_ranksPtr->end() );
	  
	  return;
      }
    
    // Definition of a getter of the ranks    
    template < BDPolicy _policy >
      template < typename _containerType >
      void
      MultiDepthMeasureBase< _policy >::
      getMultiRanks( _containerType & container ) const
      {
	  assert( this->M_ranksPtr->begin() != this->M_ranksPtr->end() );
	  
	  container.assign( this->M_ranksPtr->begin(), this->M_ranksPtr->end() );
	  
	  return;
	  
      }

    // Definition of a getter of the ranks
    template < BDPolicy _policy >
      template < typename _iteratorType >
      void
      MultiDepthMeasureBase< _policy >::
      getMultiRanks( _iteratorType begin, _iteratorType end) const
      {
	assert( this->M_ranksPtr->begin() != this->M_ranksPtr->end() );
	
	assert( begin != end );
	
	std::copy( this->M_ranksPtr->begin(), this->M_ranksPtr->end(), begin );
	
	return;
	
      }
      
    // Definition of a getter of the weights
    template < BDPolicy _policy >
      template < typename _containerType >
      void
      MultiDepthMeasureBase< _policy >::
      getWeights( boost::shared_ptr< _containerType > & containerPtr ) const
      {
	  assert( this->M_weightsPtr->begin() != this->M_weightsPtr->end() );
	
	  containerPtr->assign( this->M_weightsPtr->begin(), this->M_weightsPtr->end() );
	  
	  return;
      }
    
    // Definition of a getter of the weights
    template < BDPolicy _policy >
      template < typename _containerType >
      void
      MultiDepthMeasureBase< _policy >::
      getWeights( _containerType & container ) const
      {
	  assert( this->M_weightsPtr->begin() != this->M_weightsPtr->end() );
	  
	  container.assign( this->M_weightsPtr->begin(), this->M_weightsPtr->end() );
	  
	  return;
	  
      }

    // Definition of a getter of the weights
    template < BDPolicy _policy >
      template < typename _iteratorType >
      void
      MultiDepthMeasureBase< _policy >::
      getWeights( _iteratorType begin, _iteratorType end) const
      {
	assert( this->M_weightsPtr->begin() != this->M_weightsPtr->end() );
	
	assert( begin != end );
	
	std::copy( this->M_weightsPtr->begin(), this->M_weightsPtr->end(), begin );
	
	return;
	
      }

  
  
  //! @class MultiDepthMeasure this class implements the computation of multivariate depths measure of a functional dataset.
  /*!
   * The class is derived from the base class MultiDepthMeasureBase and implements the computation of the functional depths in the
   * multivariate case. The different dimensions are set up via addDimension method, and by using external GetPot data.
   */
  template < UInt _J, BDPolicy _policy >
  class MultiDepthMeasure : public MultiDepthMeasureBase< _policy >
  {
  public:
    
    //!@name Public Types
    //@{

      //! Typedef of the base class
      typedef typename MultiDepthMeasureBase< _policy >::getPot_Type getPot_Type;
    
      //! Typedef of the base depth measure class
      typedef DepthMeasureBase< _policy > DMBase_Type;
    
      //! Typedef of the shared pointer to the base depth class
      typedef boost::shared_ptr< DMBase_Type > DMBasePtr_Type;
      
      //! Typedef for the data object
      typedef typename DMBase_Type::bdData_Type bdData_Type;
      
      //! Typedef for the shared pointer to the data object
      typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
      
      //! Typedef of the list to data pointer objects
      typedef std::list< bdDataPtr_Type > dataContainer_Type;
      
      //! Typedef of the shared pointer to the data container
      typedef boost::shared_ptr< dataContainer_Type > dataContainerPtr_Type;
      
      //! Typedef of the wegihts type
      typedef typename MultiDepthMeasureBase< _policy >::weights_Type weights_Type;
      
      //! Typedef of the weights pointer type
      typedef typename MultiDepthMeasureBase< _policy >::weightsPtr_Type weightsPtr_Type;
      
    //@}

       
    //! Default constructor  
    MultiDepthMeasure();
    
    //! Default destructor
    virtual ~MultiDepthMeasure(){};
    
    //! Method to add a dimension to the object
    /*!
     * Diensions are treated indipendntly as single dataset of which to compute the marginal depths. Dimensions can be added to
     * the object by calling this method, which need a GetPot file to build a data object type, pushed back to the container list.
     * When needed the data object can serve as initializer of the corresponding depthMeasure object.
     * 
     * @param dataFile is the GetPot variable
     * @param sectio is the section where information lie.
     */
    void addDimension( const getPot_Type & dataFile, const std::string & section );
    
    //! Setter of weights from data coming through getpot variable
    void setWeights( const getPot_Type & dataFile, const std::string & section  );
    
    //! Method to compute the depths
    void computeMultiDepths();
    
    //! Method to compute the ranks
    void computeRanks();
    
    //! Method to write the depths to the specified filename
    void writeMultiDepths();
    
    //! Method to write ranks to some output
    void writeRanks( std::ostream & output = std::cout );
      
    //! Method to write the ranks to a given filename
    void writeRanks( const std::string & outputFilename );
    
    //! Setter of some data useful for the class, packed inside a getpot variable
    void setData( const getPot_Type & dataFile, const std::string & section );
    
  protected:

    //!@name Protected Types
    //@{

       //! Typedef of the ranks type
       typedef typename MultiDepthMeasureBase< _policy >::ranks_Type ranks_Type;

       //! Typedef of the ranks pointer type
       typedef typename MultiDepthMeasureBase< _policy >::ranksPtr_Type ranksPtr_Type;
    
       //! Typedef of the depths type
       typedef typename MultiDepthMeasureBase< _policy >::depths_Type depths_Type;

       //! Typedef of the depths pointer type
       typedef typename MultiDepthMeasureBase< _policy >::depthsPtr_Type depthsPtr_Type;
       
    //@}
    
    //! Number of dimensions
    UInt M_nbDimensions;
    
    //! Data container type
    dataContainerPtr_Type M_dataContainerPtr;   
    
    //! Output filename in which to print the results
    std::string M_outputFilename;
    
  private:

    //!@name Private Types
    //@{
      
      //! Typedef of the sorted
      typedef ExtendedSort< Real > sort_Type;
    
      //! Typedef of the sorter pointer
      typedef boost::shared_ptr< sort_Type > sortPtr_Type;

    //@}

    //! Sorter pointer object
    sortPtr_Type M_sortPtr;
    
  };
  
  // Definition of the standard constructor
  template < UInt _J, BDPolicy _policy >
  MultiDepthMeasure< _J, _policy >::
  MultiDepthMeasure()
  :
  M_sortPtr( new sort_Type() ),
  M_dataContainerPtr( new dataContainer_Type() ),
  M_nbDimensions( 0 )
  {}
 
  // Definition of the method to add dimensions
  template < UInt _J, BDPolicy _policy >
  void
  MultiDepthMeasure< _J, _policy >::
  addDimension( const getPot_Type & dataFile, const std::string & section )
  {
    ++this->M_nbDimensions;
    
    bdDataPtr_Type bdDataPtr( new bdData_Type( dataFile, section ) );
    
    if ( this->M_dataContainerPtr->size() != 0 )
    {
	typename dataContainer_Type::const_iterator it = this->M_dataContainerPtr->begin();
      
	assert( bdDataPtr->nbPz() == (*it)->nbPz() );
	
	assert( bdDataPtr->J() == _J );
	
	assert( bdDataPtr->nbTestSamples() == (*it)->nbTestSamples() );
    }
    
    
    this->M_dataContainerPtr->push_back( bdDataPtr );
   
    return;
    
  }  
  
  // Definition of the method to set weights
  template < UInt _J, BDPolicy _policy >
  void
  MultiDepthMeasure< _J, _policy >::
  setWeights( const getPot_Type & dataFile, const std::string & section )
  {
      const std::string weightsFilename( dataFile( ( section + "/weightsFilename" ).data(), "weights.dat" ) );
    
      std::ifstream input( weightsFilename.data(), std::ios_base::in );
      
      this->M_weightsPtr->reserve( this->M_nbDimensions );
      
      for ( UInt counter(0); counter < this->M_nbDimensions; ++counter )
      {
	Real temp;
	
	input >> temp;
	
	this->M_weightsPtr->push_back( temp );
	
      }
      
      input.close();
      
      return;
      
  }
  
  // Definition of the method to set data
  template < UInt _J, BDPolicy _policy >
  void
  MultiDepthMeasure< _J, _policy >::
  setData( const getPot_Type & dataFile, const std::string & section )
  {
      this->M_outputFilename = dataFile( ( section + "/outputFilename" ).data(), "mbd.dat" );
      
      const UInt J = dataFile( ( section + "/J" ).data(), 2 );
      
      assert( _J == J );
      
      return;
  }
  
  // Definition of the method to write depths
  template < UInt _J, BDPolicy _policy >
  void
  MultiDepthMeasure< _J, _policy >::
  writeMultiDepths()
  {
    if ( this->M_mpiUtilPtr->isMaster() )
    {
      std::ofstream output( this->M_outputFilename.data(), std::ios_base::out );
      
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

  // Definition of the methods to write ranks to some output
  template < UInt _J, BDPolicy _policy >
  void
  MultiDepthMeasure< _J, _policy >::
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
  
  // Definition of the method to write ranks to some output file
  template < UInt _J, BDPolicy _policy >
  void
  MultiDepthMeasure< _J, _policy >::
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
  
  // Definition of the method to compute the depths
  template < UInt _J, BDPolicy _policy >
  void
  MultiDepthMeasure< _J, _policy >::
  computeMultiDepths()
  {
     typedef typename dataContainer_Type::const_iterator iter_Type;
     
     DMFactory< _policy > dmFactory;
     
     DMBasePtr_Type dmBasePtr;
 
     depthsPtr_Type depthsPtr( new depths_Type() );
     
     const UInt nbTestSamples( (*this->M_dataContainerPtr->begin())->nbTestSamples() );
     
     this->M_depthsPtr->assign( nbTestSamples, 0 );
     
     UInt countDim(0);
     
     for( iter_Type it = this->M_dataContainerPtr->begin(); it != this->M_dataContainerPtr->end(); ++it )
     {
       
	  dmBasePtr.reset( dmFactory.create( (*it)->J() ) );
	  
	  dmBasePtr->setBandDepthData( *it );
	  
	  dmBasePtr->computeDepths();
	  
	  dmBasePtr->getDepths( depthsPtr );
	  
	  dmBasePtr->writeDepths();
	  
	  
	  for ( UInt iSample(0); iSample < nbTestSamples; ++iSample )
	  {
	      (*this->M_depthsPtr)[ iSample ] += (*this->M_weightsPtr)[ countDim ] * (*depthsPtr)[ iSample ];
	  
	  }
	  
	  countDim++;
      }
     
     return;
  }
  
  // Definition of the method to comute the ranks
  template < UInt _J, BDPolicy _policy >
  void
  MultiDepthMeasure< _J, _policy >::
  computeRanks()
  {
      assert( this->M_depthsPtr->size() != 0 );
    
      this->M_ranksPtr->assign( this->M_depthsPtr->size(), 0 );
	  
      this->M_sortPtr->setData( this->M_depthsPtr->begin(), this->M_depthsPtr->end() );
	  
      this->M_sortPtr->getSortedIDs( this->M_ranksPtr );
    
    return;
  }
  
  
}


#endif /* __MULTI_DEPTH_MEASURE_HPP__ */