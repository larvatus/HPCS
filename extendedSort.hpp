

#ifndef __EXTENDED_SORT_HPP__
#define __EXTENDED_SORT_HPP__

#include <HPCSDefs.hpp>

namespace HPCS
{

  template < typename _T, typename _U >
  struct LessThanPairSecond :
  public std::binary_function< std::pair< _T, _U >, std::pair< _T, _U >, bool >
  {
      typedef std::pair< _T, _U > data_Type;
      
      bool operator()( const data_Type & D1, const data_Type & D2 );
      
  };
  
  template < typename _T, typename _U >
  inline
  bool
  LessThanPairSecond< _T, _U >::
  operator()( const data_Type & D1, const data_Type & D2 )
  {
      return ( D1.second < D2.second );
  }
  
  
  template < typename _T, typename _U >
  struct LessThanPairFirst : 
  public std::binary_function< std::pair< _T, _U >, std::pair< _T, _U >, bool > 
  {
    typedef std::pair< _T, _U > data_Type;
    
    bool operator()( const data_Type & D1, const data_Type & D2 );
  };
  
  template < typename _T, typename _U >
  inline
  bool
  LessThanPairFirst< _T, _U >::
  operator()( const data_Type & D1, const data_Type & D2 )
  {
      return ( D1.first < D2.first );
  }
  
  
  
  //! REQUIRES THE LESS THAN RELATION ON _T
  //! NON LO FACCIO DOPPIO O TRIPLO TEMPLATE PERCHÈ in questo caso non sarebbe riutilizzabile
  //! resettando con un diverso contenitore!
  
  template < typename _T >
  class ExtendedSort
    {
    public:
      
      typedef std::pair< _T, UInt > tuple_Type;
      
      typedef std::list< tuple_Type > data_Type;
      
      typedef boost::shared_ptr< data_Type > dataPtr_Type;
      
      typedef std::vector< UInt > ID_Type;
      
      typedef boost::shared_ptr< ID_Type > IDPtr_Type;
      
      ExtendedSort(){};
      
      ~ExtendedSort(){};
      
      template < typename _containerType >
	ExtendedSort( const _containerType & cont );

      template < typename _iteratorType >
	ExtendedSort( const _iteratorType & begin, const _iteratorType & end );
	
      template < typename _containerType >	
	void setData( const _containerType & cont );
	
      template < typename _iteratorType >
	void setData( const _iteratorType & begin, const _iteratorType & end );
         
      template < typename _containerType > 
	void getSortedIDs( _containerType & container ); 
      
      template < typename _containerType > 
	void getSortedIDs( boost::shared_ptr< _containerType > & containerPtr );

      template < typename _iteratorType >
	void getSortedIDs( _iteratorType begin, _iteratorType end );
	
    protected:
      
      IDPtr_Type M_IDsPtr; 
      
      dataPtr_Type M_dataPtr;
      
    private:
      
      bool M_sortedFlag;
      
      void performSort();
      
    };
     
    template < typename _T >
      template < typename _containerType >
      ExtendedSort< _T >::
      ExtendedSort( const _containerType & cont )
      :
      M_sortedFlag( false ),
      M_dataPtr( new data_Type() ),
      M_IDsPtr( new ID_Type() )
      {	
	typedef typename _containerType::const_iterator it_Type;
	
	UInt count(0);
	
	for( it_Type it( cont.begin() ); it != cont.end(); it++ )
	{
	    this->M_dataPtr->push_back( std::make_pair< _T, UInt >( *it, count ) );
	    
	    count++;
	}

	this->M_IDsPtr->resize( count ); 
      }

    template < typename _T >
      template < typename _iteratorType >
      ExtendedSort< _T >::
      ExtendedSort( const _iteratorType & begin, const _iteratorType & end )
      :
      M_sortedFlag( false ),
      M_dataPtr( new data_Type() ),
      M_IDsPtr( new ID_Type() )
      {
	
	typedef _iteratorType it_Type;
	
	UInt count(0);
	
	for( it_Type it( begin ); it != end; it++ )
	{
	    this->M_dataPtr->push_back( std::make_pair< _T, UInt >( *it, count ) );
	    
	    count++;
	}
	
	this->M_IDsPtr->resize( count ); 
      }      
      
    template < typename _T >
      template < typename _containerType >
      void
      ExtendedSort< _T >::
      setData( const _containerType & cont )
      {
	this->M_sortedFlag = false;
	
	this->M_dataPtr->erase();
	
	typedef typename _containerType::iterator it_Type;
	
	UInt count(0);
	
	for( it_Type it( cont.begin() ); it != cont.end(); it++ )
	{
	    this->M_dataPtr->push_back( std::make_pair< _T, UInt >( *it, count ) );
	    
	    count++;
	}
	
	this->M_IDsPtr->resize( count ); 
	
	return;
      }
	
    template < typename _T >
      template < typename _iteratorType >
      void
      ExtendedSort< _T >::
      setData( const _iteratorType & begin, const _iteratorType & end )
      {
	this->M_sortedFlag = false;
	
	this->M_dataPtr->erase();
	
	typedef _iteratorType it_Type;
	
	UInt count(0);
	
	for( it_Type it( begin ); it != end; it++ )
	{
	    this->M_dataPtr->push_back( std::make_pair< _T, UInt >( *it, count ) );
	    
	    count++;
	}
	
	this->M_IDsPtr->resize( count ); 
	
	return;
      }

      template < typename _T >
	template < typename _containerType >
	void
	ExtendedSort< _T >::
	getSortedIDs( _containerType & container ) 
	{
	    if( this->M_sortedFlag != true )
	    {
		this->performSort();
	    }
	    
	    container.assign( this->M_IDsPtr->begin(), this->M_IDsPtr->end() );
	  
	    return;
	}
      
      template < typename _T >
	template < typename _containerType > 
	void 
	ExtendedSort< _T >::
	getSortedIDs( boost::shared_ptr< _containerType > & containerPtr ) 
	{
	    if( this->M_sortedFlag != true )
	    {
		this->performSort();
	    }
	    
	    containerPtr->assign( this->M_IDsPtr->begin(), this->M_IDsPtr->end() );
	    
	    return;
	}

      template < typename _T >
	template < typename _iteratorType >
	void
	ExtendedSort< _T >::
	getSortedIDs( _iteratorType begin, _iteratorType end ) 
	{
	    if( this->M_sortedFlag != true )
	    {
		this->performSort();
	    }
	    
	    assert( begin != end );
	    
	    std::copy( this->M_IDsPtr->begin(), this->M_IDsPtr->end(), begin );
	    
	    return;
	}
	
    template < typename _T >
    void
    ExtendedSort< _T >::
    performSort()
    {      
	this->M_dataPtr->sort( LessThanPairFirst< _T, UInt >() );
      
	this->M_sortedFlag = true;

	const UInt size( this->M_dataPtr->size() );
	
	assert( this->M_IDsPtr->size() == size );
	
	typename data_Type::iterator it = this->M_dataPtr->begin(); 
	
	for ( UInt i(0); i < size; ++i )
	{ 
	    (*this->M_IDsPtr)[ it->second ] = i; 
	 
	    it++;
	}
	
	return;
    }
          
}

#endif /* __EXTENDED_SORT_HPP__*/