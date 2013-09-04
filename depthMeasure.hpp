

#ifndef __DEPTH_MEASURE_HPP__
#define __DEPTH_MEASURE_HPP__

#include <HPCSDefs.hpp>



#include <bandDepth.hpp>
#include <bandDepthFactory.hpp>
#include <bandDepthData.hpp>
#include <extendedSort.hpp>
#include <mpi_utility.hpp>


namespace HPCS
{

  template < BDPolicy _policy >
  class DepthMeasureBase
  {
  public:
    
    typedef BandDepthBase< _policy > bdBase_Type;
    
    typedef boost::shared_ptr< bdBase_Type > bdBasePtr_Type;
    
    typedef typename bdBase_Type::bdData_Type bdData_Type;
    
    typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
    
    typedef mpiUtility mpiUtility_Type;
    
    typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
    
    DepthMeasureBase();
    
    virtual ~DepthMeasureBase(){};
    
    //! LI METTO QUI ANCHE PERCHÉ NELLA CLASSE DERIVATA NON POTREBBERO ESSERE USATI IN MANIERA POLIMORFICA! (TEMPLATE NON POSSONO ESSERE VIRTUAL!)
    template < typename _containerType >
      void getDepths( boost::shared_ptr< _containerType > & containerPtr  ) const;

    template < typename _containerType >
      void getDepths( _containerType & container ) const;

    template < typename _iteratorType >
      void getDepths( _iteratorType begin, _iteratorType end ) const;    
    
    template < typename _containerType >
      void getRanks( boost::shared_ptr< _containerType > &  containerPtr ) const;

    template < typename _containerType >
      void getRanks( _containerType &  container ) const;

    template < typename _iteratorType >
      void getRanks( _iteratorType begin, _iteratorType end ) const;
    
    virtual void setBandDepthData( const bdDataPtr_Type & bdDataPtr ){};
    
    virtual void computeDepths(){};
    
    virtual void computeRanks(){};
    
    virtual void writeDepths(){};
 
    virtual void writeRanks( std::ostream & output = std::cout ){};
      
    virtual void writeRanks( const std::string & outputFilename ){};
     
  protected:
    
    typedef std::vector< Real > depths_Type;
    
    typedef boost::shared_ptr< depths_Type > depthsPtr_Type;
    
    typedef std::vector< UInt > ranks_Type;
    
    typedef boost::shared_ptr< ranks_Type > ranksPtr_Type;
    
    depthsPtr_Type M_depthsPtr;
    
    ranksPtr_Type M_ranksPtr;

    mpiUtilityPtr_Type M_mpiUtilPtr;
    
  private:
    
    
  };
  
  template < BDPolicy _policy >
  DepthMeasureBase< _policy >::
  DepthMeasureBase()
  :
  M_mpiUtilPtr( new mpiUtility_Type() ),
  M_depthsPtr( new depths_Type() ),
  M_ranksPtr( new ranks_Type() )
  {}
  
  
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
 
  
  template < UInt _J, BDPolicy _policy  >
  class DepthMeasure : public DepthMeasureBase< _policy >
  {
  public:
    
    typedef BandDepthBase< _policy > bdBase_Type;
    
    typedef boost::shared_ptr< bdBase_Type > bdBasePtr_Type;
    
    typedef typename bdBase_Type::bdData_Type bdData_Type;
    
    typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
    
    DepthMeasure();
    
    virtual ~DepthMeasure(){};
    
    DepthMeasure( const bdDataPtr_Type & bdDataPtr );
    
    //! polymorphic use of bdData
    void setBandDepthData( const bdDataPtr_Type & bdDataPtr );

    void computeDepths();

    void computeRanks();
     
    void writeDepths();
 
    void writeRanks( std::ostream & output = std::cout );
      
    void writeRanks( const std::string & outputFilename );
     
  protected:
 
    typedef typename DepthMeasureBase< _policy >::depths_Type depths_Type;
    
    typedef typename DepthMeasureBase< _policy >::depthsPtr_Type depthsPtr_Type;
    
    typedef typename DepthMeasureBase< _policy >::ranks_Type ranks_Type;
    
    typedef typename DepthMeasureBase< _policy >::ranksPtr_Type ranksPtr_Type;
    
    bdDataPtr_Type M_bdDataPtr;
    
  private:
    
    typedef ExtendedSort< Real > sort_Type;
    
    typedef boost::shared_ptr< sort_Type > sortPtr_Type;
    
    sortPtr_Type M_sortPtr;
      
   };
  
  template < UInt _J, BDPolicy _policy >
  DepthMeasure< _J, _policy >::
  DepthMeasure()
  :
  DepthMeasureBase< _policy >(),
  M_sortPtr( new sort_Type() )
  {
  }
  
  template < UInt _J, BDPolicy _policy >
  DepthMeasure< _J, _policy >::
  DepthMeasure( const bdDataPtr_Type & bdDataPtr )
  :
  DepthMeasureBase< _policy >(),
  M_bdDataPtr( bdDataPtr ),
  M_sortPtr( new sort_Type() )
  {
  }
  
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

  
  //! Poiché non è possibile dare specializzazioni parziali di singoli metodi di una classe con multipli parametri
  //! Template, costruisco questa classe che implementa il compute:
  template < UInt _J, BDPolicy _policy >
  class computeImplement 
  {
  public:
    
      typedef BandDepthBase< _policy > bdBase_Type;
    
      typedef boost::shared_ptr< bdBase_Type > bdBasePtr_Type;
    
      typedef typename bdBase_Type::bdData_Type bdData_Type;
    
      typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
      
      typedef mpiUtility mpiUtility_Type;
    
      typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
      
      typedef std::vector< Real > container_Type;
      
      typedef boost::shared_ptr< container_Type > containerPtr_Type;
      
      computeImplement( const bdDataPtr_Type & bdDataPtr );
    
      void compute();
   
      void getComputedDepths( containerPtr_Type & contPtr );
      
  private:
    
      typedef BDFactory< _policy > bdFactory_Type;
    
      typedef boost::shared_ptr< bdFactory_Type > bdFactoryPtr_Type;

      bdFactoryPtr_Type M_bdFactoryPtr;
    
      bdDataPtr_Type M_bdDataPtr;
      
      mpiUtilityPtr_Type M_mpiUtilPtr;
      
      containerPtr_Type M_containerPtr;
      
  };
  
  template < UInt _J >
  class computeImplement< _J, All > 
  {
  public:
    
      typedef BandDepthBase< All > bdBase_Type;
    
      typedef boost::shared_ptr< bdBase_Type > bdBasePtr_Type;
    
      typedef typename bdBase_Type::bdData_Type bdData_Type;
    
      typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
      
      typedef mpiUtility mpiUtility_Type;
    
      typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
      
      typedef std::vector< Real > container_Type;
      
      typedef boost::shared_ptr< container_Type > containerPtr_Type;
      
      computeImplement( const bdDataPtr_Type & bdDataPtr );
    
      void compute();
   
      void getComputedDepths( containerPtr_Type & contPtr );
      
  private:
    
      typedef BDFactory< All > bdFactory_Type;
    
      typedef boost::shared_ptr< bdFactory_Type > bdFactoryPtr_Type;
    
      bdFactoryPtr_Type M_bdFactoryPtr;
      
      bdDataPtr_Type M_bdDataPtr;
      
      mpiUtilityPtr_Type M_mpiUtilPtr;
      
      containerPtr_Type M_containerPtr;
      
  };
  
  template < UInt _J >
  computeImplement< _J, All >::
  computeImplement( const bdDataPtr_Type & bdDataPtr )
  :
  M_bdDataPtr( bdDataPtr ),
  M_mpiUtilPtr( new mpiUtility_Type() )
  {
  }
  
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
  
  template < UInt _J >
  void
  computeImplement< _J, All >::
  getComputedDepths( containerPtr_Type & contPtr )
  {
      contPtr = this->M_containerPtr;
      
      return;
  }
  
  
  template < UInt _J >
  class computeImplement< _J, Reference > 
  {
  public:
    
      typedef BandDepthBase< Reference > bdBase_Type;
    
      typedef boost::shared_ptr< bdBase_Type > bdBasePtr_Type;
    
      typedef typename bdBase_Type::bdData_Type bdData_Type;
    
      typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
      
      typedef mpiUtility mpiUtility_Type;
    
      typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
      
      typedef std::vector< Real > container_Type;
      
      typedef boost::shared_ptr< container_Type > containerPtr_Type;
      
      computeImplement( const bdDataPtr_Type & bdDataPtr );
    
      void compute();
   
      void getComputedDepths( containerPtr_Type & contPtr );
      
  private:
    
      typedef BDFactory< Reference > bdFactory_Type;
    
      typedef boost::shared_ptr< bdFactory_Type > bdFactoryPtr_Type;
    
      bdDataPtr_Type M_bdDataPtr;
      
      mpiUtilityPtr_Type M_mpiUtilPtr;
      
      containerPtr_Type M_containerPtr;
      
  };
  
  template < UInt _J >
  computeImplement< _J, Reference >::
  computeImplement( const bdDataPtr_Type & bdDataPtr )
  :
  M_bdDataPtr( bdDataPtr ),
  M_mpiUtilPtr( new mpiUtility_Type() )
  {
  }
  
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
	 
	 bdBasePtr->addToReferenceSet( 0, this->M_bdDataPtr->nbReferenceSamples() );
   
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
  
  template < UInt _J >
  void
  computeImplement< _J, Reference >::
  getComputedDepths( containerPtr_Type & contPtr )
  {
      contPtr = this->M_containerPtr;
      
      return;
  }

  
  //! Ora è possibile implementare il compute di DepthMeasure
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
