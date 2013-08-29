

#ifndef __DEPTH_MEASURE_HPP__
#define __DEPTH_MEASURE_HPP__

#include <HPCSDefs.hpp>

#include <bandDepth.hpp>


#include <factory.hpp>
#include <bandDepthData.hpp>
#include <extendedSort.hpp>

namespace HPCS
{  
  template < UInt _J, BDPolicy _policy >
  class DepthMeasure
  {
  public:
    
    typedef BandDepthBase bdBase_Type;
    
    typedef boost::shared_ptr< bdBase_Type > bdBasePtr_Type;
    
    typedef BandDepthData bdData_Type;
    
    typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;
    
    typedef std::vector< UInt > depths_Type;
    
    typedef boost::shared_ptr< depths_Type > depthsPtr_Type;
    
    DepthMeasure(){};
    
    ~DepthMeasure(){};
    
    DepthMeasure( const bdDataPtr_Type & bdDataPtr );
    
    //! polymorphic use of bdData
    void setBDData( const bdDataPtr_Type & bdDataPtr );
   
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
    
    //! TO BE SPECIALIZED!
    void compute();

  protected:
    
    bdDataPtr_Type M_bdDataPtr;
    
    std::vector< Real > M_depths;
    
    std::vector< UInt > M_ranks;

  private:
    
    typedef ExtendedSort< Real > sort_Type;
    
    typedef boost::shared_ptr< sort_Type > sortPtr_Type;
    
    typedef BDFactory< _policy > bdFactory_Type;
    
    typedef boost::shared_ptr< bdFactory_Type > bdFactoryPtr_Type;
    
    sortPtr_Type M_sortPtr;
    
    bdFactoryPtr_Type M_bdFactoryPtr;
    
  };
  
  template < UInt _J, BDPolicy _policy >
  DepthMeasure< _J, _policy >::
  DepthMeasure( const bdDataPtr_Type & bdDataPtr )
  :
  M_bdDataPtr( bdDataPtr ),
  M_sortPtr( new sort_Type() ),
  M_bdFactoryPtr( new bdFactory_Type() )
  {    
  }
  
  template< UInt _J, BDPolicy _policy >
  inline
  void
  DepthMeasure< _J, _policy >::
  setBDData( const bdDataPtr_Type & bdDataPtr )
  {
      this->M_bdDataPtr = bdDataPtr;
      
      std::vector< Real >().swap( this->M_depths ); 
      
      std::vector< UInt >().swap( this->M_ranks ); 
      
  }
    
    template < UInt _J, BDPolicy _policy >
      template < typename _containerType >
      void
      DepthMeasure< _J, _policy >::
      getDepths( boost::shared_ptr< _containerType > & containerPtr ) const
      {
	  assert( this->M_depths.size() != 0 );
	
	  containerPtr->assign( this->M_depths.begin(), this->M_depths.end() );
	  
	  return;
      }
      
    template < UInt _J, BDPolicy _policy >
      template < typename _containerType >
      void
      DepthMeasure< _J, _policy >::
      getDepths( _containerType & container ) const
      {
	  assert( this->M_depths.size() != 0 );
	
	  container->assign( this->M_depths.begin(), this->M_depths.end() );
	  
	  return;
      }
      
    template < UInt _J, BDPolicy _policy >
      template < typename _iteratorType >
      void
      DepthMeasure< _J, _policy >::
      getDepths( _iteratorType begin, _iteratorType end ) const
      {
	  assert( this->M_depths.size() != 0 );
	  
	  std::copy( this->M_depths.begin(), this->M_depths.end(), begin );
	  
	  return;
      }

       
    template < UInt _J, BDPolicy _policy >
      template < typename _containerType >
      void
      DepthMeasure< _J, _policy >::
      getRanks( boost::shared_ptr< _containerType > & containerPtr )
      {
	if ( this->M_ranks.size() == 0 )
	{
	  this->M_ranks.resize( this->M_depths.size() );
	  
	  this->M_sortPtr->setData( this->M_depths->begin(), this->M_depths->end() );
	  
	  this->M_sortPtr->getSortedIDs( this->M_ranks );
	}
	
	  containerPtr->assign( this->M_ranks.begin(), this->M_ranks.end() );
	  
	  return;
      }
      
    template < UInt _J, BDPolicy _policy >
      template < typename _containerType >
      void
      DepthMeasure< _J, _policy >::
      getRanks( _containerType & container )
      {
	if ( this->M_ranks.size() == 0 )
	{
	  this->M_ranks.resize( this->M_depths.size() );
	  
	  this->M_sortPtr->setData( this->M_depths->begin(), this->M_depths->end() );
	  
	  this->M_sortPtr->getSortedIDs( this->M_ranks );
	}
	
	  container.assign( this->M_ranks.begin(), this->M_ranks.end() );
	  
	  return;
      }   
      
   
    template < UInt _J, BDPolicy _policy >
      template < typename _iteratorType >
      void
      DepthMeasure< _J, _policy >::
      getRanks( _iteratorType begin, _iteratorType end )
      {
	if ( this->M_ranks.size() == 0 )
	{
	  this->M_ranks.resize( this->M_depths.size() );
	  
	  this->M_sortPtr->setData( this->M_depths->begin(), this->M_depths->end() );
	  
	  this->M_sortPtr->getSortedIDs( this->M_ranks );
	}
	
	  std::copy( this->M_ranks.begin(), this->M_ranks.end(), begin );
	  
	  return;
      }
  
  template< UInt _J, BDPolicy _policy >
  void
  DepthMeasure< _J, _policy >::
  compute()
  {
     std::vector< UInt >().swap( this->M_ranks ); 
      
     std::vector< Real > currBDs;
      
     this->M_depths.resize( this->M_bdDataPtr->nbPz() );
     
     bdBasePtr_Type bdBasePtr( new bdBase_Type() );
     
     for ( UInt j(2); j <= _J; ++j )
     {	 
	 std::cout << " J = " << j << std::endl;
       
	 bdBasePtr.reset( this->M_bdFactoryPtr->create( j ) );
       
	 bdBasePtr->setBandDepthData( this->M_bdDataPtr );
	  
	 bdBasePtr->computeBDs();
	  
	 bdBasePtr->getBDs( currBDs );
	 	  
	 for ( UInt iPz(0); iPz < this->M_bdDataPtr->nbPz(); ++iPz )
	 {
	     this->M_depths[ iPz ] += currBDs[ iPz ]; 
	 }
      }

    return;
  }
  
//!       IN OGNI COMPUTE SWAPPA A ZERO I RANGHI E CANCELLA LE PROFONDITA.
//! 	  FARE LA VERSIONE CON REFERENCE!
}




#endif /* __DEPTH_MEASURE_HPP__ */
