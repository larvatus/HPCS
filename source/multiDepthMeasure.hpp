
#ifndef __MULTI_DEPTH_MEASURE_HPP__
#define __MULTI_DEPTH_MEASURE_HPP__

#include <source/HPCSDefs.hpp>
#include <source/depthMeasureFactory.hpp>
#include <source/depthMeasure.hpp>
#include <source/extendedSort.hpp>

namespace HPCS
{
  template < BDPolicy _policy >
  class MultiDepthMeasureBase
  {
  public:
    
    typedef GetPot getPot_Type;
     
    MultiDepthMeasureBase();
    
    virtual ~MultiDepthMeasureBase(){};
    
    virtual void addDimension( const getPot_Type & dataFile, const std::string & section ){};
    
    virtual void computeMultiDepths(){};
    
    virtual void computeRanks(){};
    
    virtual void writeMultiDepths(){};
    
    virtual void writeRanks( std::ostream & output = std::cout ){};
      
    virtual void writeRanks( const std::string & outputFilename ){};
    
    virtual void setWeights( const getPot_Type & dataFile, const std::string & section ){};
    
    //! SETTA PRINCIPALMENTE L'OUTPUT FILENAME
    virtual void setData( const getPot_Type & dataFile, const std::string & section ){};
    
    template < typename _containerType >
      void setWeights( const boost::shared_ptr< _containerType > &  containerPtr );

    template < typename _containerType >
      void setWeights( const _containerType &  container );

    template < typename _iteratorType >
      void setWeights( const _iteratorType begin, _iteratorType end );
    
     template < typename _containerType >
      void getMultiDepths( boost::shared_ptr< _containerType > & containerPtr  ) const;

    template < typename _containerType >
      void getMultiDepths( _containerType & container ) const;

    template < typename _iteratorType >
      void getMultiDepths( _iteratorType begin, _iteratorType end ) const;    
    
    template < typename _containerType >
      void getMultiRanks( boost::shared_ptr< _containerType > &  containerPtr ) const;

    template < typename _containerType >
      void getMultiRanks( _containerType &  container ) const;

    template < typename _iteratorType >
      void getMultiRanks( _iteratorType begin, _iteratorType end ) const;
      
    template < typename _containerType >
      void getWeights( boost::shared_ptr< _containerType > &  containerPtr ) const;

    template < typename _containerType >
      void getWeights( _containerType &  container ) const;

    template < typename _iteratorType >
      void getWeights( _iteratorType begin, _iteratorType end ) const;
    
  protected:
    
    typedef std::vector< Real > depths_Type;
    
    typedef boost::shared_ptr< depths_Type > depthsPtr_Type;
    
    typedef std::vector< UInt > ranks_Type;
    
    typedef boost::shared_ptr< ranks_Type > ranksPtr_Type;
    
    typedef mpiUtility mpiUtility_Type;
  
    typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
    
    typedef std::vector< Real > weights_Type;
    
    typedef boost::shared_ptr< weights_Type > weightsPtr_Type;
    
    mpiUtilityPtr_Type M_mpiUtilPtr;
    
    depthsPtr_Type M_depthsPtr;
    
    ranksPtr_Type M_ranksPtr;
    
    weightsPtr_Type M_weightsPtr;
    
  private:
    
  };
  
  
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

  
  template < UInt _J, BDPolicy _policy >
  class MultiDepthMeasure : public MultiDepthMeasureBase< _policy >
  {
  public:
    
    typedef typename MultiDepthMeasureBase< _policy >::getPot_Type getPot_Type;
    
    typedef DepthMeasureBase< _policy > DMBase_Type;
   
    typedef boost::shared_ptr< DMBase_Type > DMBasePtr_Type;
    
    typedef typename DMBase_Type::bdData_Type bdData_Type;
    
    typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
    
    typedef std::list< bdDataPtr_Type > dataContainer_Type;
    
    typedef boost::shared_ptr< dataContainer_Type > dataContainerPtr_Type;
    
    typedef typename MultiDepthMeasureBase< _policy >::weights_Type weights_Type;
    
    typedef typename MultiDepthMeasureBase< _policy >::weightsPtr_Type weightsPtr_Type;
    
    MultiDepthMeasure();
    
    virtual ~MultiDepthMeasure(){};
    
    void addDimension( const getPot_Type & dataFile, const std::string & section );
    
    void setWeights( const getPot_Type & dataFile, const std::string & section  );
    
    void computeMultiDepths();
    
    void computeRanks();
    
    void writeMultiDepths();
    
    void writeRanks( std::ostream & output = std::cout );
      
    void writeRanks( const std::string & outputFilename );
    
    //! SETTA PRINCIPALMENTE L'OUTPUT FILENAME
    void setData( const getPot_Type & dataFile, const std::string & section );
    
  protected:
    
    typedef typename MultiDepthMeasureBase< _policy >::ranks_Type ranks_Type;

    typedef typename MultiDepthMeasureBase< _policy >::ranksPtr_Type ranksPtr_Type;
    
    typedef typename MultiDepthMeasureBase< _policy >::depthsPtr_Type depthsPtr_Type;
	    
    typedef typename MultiDepthMeasureBase< _policy >::depths_Type depths_Type;
    
    
    
    UInt M_nbDimensions;
    
    dataContainerPtr_Type M_dataContainerPtr;   
    
    std::string M_outputFilename;
    
  private:
    
    typedef ExtendedSort< Real > sort_Type;
    
    typedef boost::shared_ptr< sort_Type > sortPtr_Type;
    
    sortPtr_Type M_sortPtr;
    
  };
  
  
  template < UInt _J, BDPolicy _policy >
  MultiDepthMeasure< _J, _policy >::
  MultiDepthMeasure()
  :
  M_sortPtr( new sort_Type() ),
  M_dataContainerPtr( new dataContainer_Type() ),
  M_nbDimensions( 0 )
  {}
 
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