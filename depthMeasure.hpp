

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
    
    virtual void getDepths( std::vector< Real > & dephts ) const{};
    
    virtual void getRanks( std::vector< Real > & dephts ){};
    
    virtual void setBandDepthData( const bdDataPtr_Type & bdDataPtr ){};
    
    virtual void computeDepths(){};
    
    virtual void computeRanks(){};
    
    virtual void writeDepths(){};
    
    virtual void writeRanks( std::ostream & output = std::cout ){};
     
    virtual  void writeRanks( const std::string & outputFilename ){};
     
  protected:
    
    mpiUtilityPtr_Type M_mpiUtilPtr;
    
  private:
    
    
  };
  
  template < BDPolicy _policy >
  DepthMeasureBase< _policy >::
  DepthMeasureBase()
  :
  M_mpiUtilPtr( new mpiUtility_Type() )
  {}
  
  template < UInt _J, BDPolicy _policy  >
  class DepthMeasure : public DepthMeasureBase< _policy >
  {
  public:
    
    typedef BandDepthBase< _policy > bdBase_Type;
    
    typedef boost::shared_ptr< bdBase_Type > bdBasePtr_Type;
    
    typedef typename bdBase_Type::bdData_Type bdData_Type;
    
    typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
    
    typedef std::vector< UInt > depths_Type;
    
    typedef boost::shared_ptr< depths_Type > depthsPtr_Type;
    
    DepthMeasure();
    
    virtual ~DepthMeasure(){};
    
    DepthMeasure( const bdDataPtr_Type & bdDataPtr );
    
    //! polymorphic use of bdData
    void setBandDepthData( const bdDataPtr_Type & bdDataPtr );
   
    template < typename _containerType >
      void getDepths( boost::shared_ptr< _containerType > & containerPtr  ) const;

    template < typename _containerType >
      void getDepths( _containerType & container ) const;

    template < typename _iteratorType >
      void getDepths( _iteratorType begin, _iteratorType end ) const;    
    
    //! NON SONO CONST PERCHÉ potrei doverli calcolare
    template < typename _containerType >
      void getRanks( boost::shared_ptr< _containerType > &  containerPtr );

    template < typename _containerType >
      void getRanks( _containerType &  container );

    template < typename _iteratorType >
      void getRanks( _iteratorType begin, _iteratorType end );

     void computeDepths();

     void computeRanks();
     
     void writeDepths();

     void writeRanks( std::ostream & output = std::cout );
     
     void writeRanks( const std::string & outputFilename );
     
  protected:
    
    bdDataPtr_Type M_bdDataPtr;
    
    boost::shared_ptr< std::vector< Real > > M_depthsPtr;
    
    boost::shared_ptr< std::vector< UInt > > M_ranksPtr;

  private:
    
    typedef ExtendedSort< Real > sort_Type;
    
    typedef boost::shared_ptr< sort_Type > sortPtr_Type;
    
    sortPtr_Type M_sortPtr;
    
   };
  
  template < UInt _J, BDPolicy _policy >
  DepthMeasure< _J, _policy >::
  DepthMeasure()
  :
  M_sortPtr( new sort_Type() ),
  M_depthsPtr( new std::vector< Real >() ),
  M_ranksPtr( new std::vector< UInt >() ),
  DepthMeasureBase< _policy >()
  {
  }
  
  template < UInt _J, BDPolicy _policy >
  DepthMeasure< _J, _policy >::
  DepthMeasure( const bdDataPtr_Type & bdDataPtr )
  :
  M_bdDataPtr( bdDataPtr ),
   M_depthsPtr( new std::vector< Real >() ),
  M_ranksPtr( new std::vector< UInt >() ),
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
      
      std::vector< Real >().swap( (*this->M_depthsPtr) ); 
      
      std::vector< UInt >().swap( (*this->M_ranksPtr) ); 
      
  }
  
  template < UInt _J, BDPolicy _policy >
  void
  DepthMeasure< _J, _policy >::
  computeRanks()
  {
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
      
      std::vector< Real >::const_iterator it;
      
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
      std::vector< UInt >::const_iterator it;
      
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
      
      std::vector< UInt >::const_iterator it;
      
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
      template < typename _containerType >
      void
      DepthMeasure< _J, _policy >::
      getDepths( boost::shared_ptr< _containerType > & containerPtr ) const
      {
	  assert( this->M_depthsPtr->size() != 0 );
	
	  containerPtr->assign( this->M_depthsPtr->begin(), this->M_depthsPtr->end() );
	  
	  return;
      }
      
    template < UInt _J, BDPolicy _policy >
      template < typename _containerType >
      void
      DepthMeasure< _J, _policy >::
      getDepths( _containerType & container ) const
      {
	  assert( this->M_depthsPtr.size() != 0 );
	
	  container->assign( this->M_depthsPtr->begin(), this->M_depthsPtr->end() );
	  
	  return;
      }
      
    template < UInt _J, BDPolicy _policy >
      template < typename _iteratorType >
      void
      DepthMeasure< _J, _policy >::
      getDepths( _iteratorType begin, _iteratorType end ) const
      {
	  assert( this->M_depthsPtr->size() != 0 );
	  
	  std::copy( this->M_depthsPtr->begin(), this->M_depthsPtr->end(), begin );
	  
	  return;
      }

       
    template < UInt _J, BDPolicy _policy >
      template < typename _containerType >
      void
      DepthMeasure< _J, _policy >::
      getRanks( boost::shared_ptr< _containerType > & containerPtr )
      {
	  assert( this->M_ranksPtr.size() != 0 );
	
	  containerPtr->assign( this->M_ranksPtr->begin(), this->M_ranksPtr->end() );
	  
	  return;
      }
      
    template < UInt _J, BDPolicy _policy >
      template < typename _containerType >
      void
      DepthMeasure< _J, _policy >::
      getRanks( _containerType & container )
      {
	  assert( this->M_ranksPtr.size() != 0 );
	
	  container.assign( this->M_ranksPtr->begin(), this->M_ranksPtr->end() );
	  
	  return;
      }   
      
   
    template < UInt _J, BDPolicy _policy >
      template < typename _iteratorType >
      void
      DepthMeasure< _J, _policy >::
      getRanks( _iteratorType begin, _iteratorType end )
      {
	  assert( this->M_ranksPtr.size() != 0 );
	
	  std::copy( this->M_ranksPtr->begin(), this->M_ranksPtr->end(), begin );
	  
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
	 
	 std::cout << "SIZEEEEE " << currBDs.size() << std::endl;
	 
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
