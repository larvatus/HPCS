

#ifndef __EXTENDED_SORT_HPP__
#define __EXTENDED_SORT_HPP__

#include <source/HPCSDefs.hpp>

namespace HPCS
{

 //! @class LessThanPairSecond this class implements a less ordering method for std::pair
 /*!
  * According to this class, two std::pair variables are ordered only by the second entry of the pair.
  */
  template < typename _T, typename _U >
  struct LessThanPairSecond :
  public std::binary_function< std::pair< _T, _U >, std::pair< _T, _U >, bool >
  {
      //! Typedef for the pair type
      typedef std::pair< _T, _U > data_Type;
      
      //! Overload of the operator(), to achieve a functor functionality
      bool operator()( const data_Type & D1, const data_Type & D2 );
      
  };

  // Definition of the operator() overload
  template < typename _T, typename _U >
  inline
  bool
  LessThanPairSecond< _T, _U >::
  operator()( const data_Type & D1, const data_Type & D2 )
  {
      // Exploiting the ordering of the second entry
      return ( D1.second < D2.second );
  }
  
 //! @class LessThanPairFirst this class implements a less ordering method for std::pair
 /*!
  * According to this class, two std::pair variables are ordered only by the first entry of the pair.
  */  
  template < typename _T, typename _U >
  struct LessThanPairFirst : 
  public std::binary_function< std::pair< _T, _U >, std::pair< _T, _U >, bool > 
  {
    //! Typedef for the pair type
    typedef std::pair< _T, _U > data_Type;
    
    //! Overload of the operator(), to achieve a functor functionality
    bool operator()( const data_Type & D1, const data_Type & D2 );
  };

  // Definition of the operator() overload
  template < typename _T, typename _U >
  inline
  bool
  LessThanPairFirst< _T, _U >::
  operator()( const data_Type & D1, const data_Type & D2 )
  {
      // Exploiting the ordering of the first entry
      return ( D1.first < D2.first );
  }
  
  //! @class ExtendedSort this class allows to order set of objects T and to get the indexes of the ordered set.
  /*!
   * Given a set of objects of type _T, this class allow to order them in ascending order, and to get the IDs in the
   * sorted structure.
   */
  template < typename _T >
  class ExtendedSort
    {
    public:
      
      //! @name Public Types
      //@{

	//! Typedef of the pair type, used to store IDs and objects
	typedef std::pair< _T, UInt > tuple_Type;
	
	//! Typedef for the list of pairs
	typedef std::list< tuple_Type > data_Type;
	
	//! Typedef for the shared pointer to the data type
	typedef boost::shared_ptr< data_Type > dataPtr_Type;
	
	//! Typedef for the vector of IDs
	typedef std::vector< UInt > ID_Type;
	
	//! Typedef for the shared pointer to the ID container
	typedef boost::shared_ptr< ID_Type > IDPtr_Type;
      
      //@}
	

      //! @name Constructor & Destructor
      //@{
	
	  //! Standard default constructor
	  ExtendedSort();
      
	  //! Standard default destructor
	  ~ExtendedSort(){};

	  //! Constructor from a template container type
	  template < typename _containerType >
	    ExtendedSort( const _containerType & cont );

	  //! Constructor from a template iterator type
	  template < typename _iteratorType >
	    ExtendedSort( const _iteratorType & begin, const _iteratorType & end );
	  
      //@}
	
      //! @name Setters and Getters
      //@{

	  //! Setter of the data from a template container
	  template < typename _containerType >	
	    void setData( const _containerType & cont );
	    
	  //! Setter of the data from a template iterator type  
	  template < typename _iteratorType >
	    void setData( const _iteratorType & begin, const _iteratorType & end );
	  
	  //! Getter of the sorded IDs from a container type
	  template < typename _containerType > 
	    void getSortedIDs( _containerType & container ); 
	  
	  //! Getter of the sorted IDs from a shared pointer to a template container type
	  template < typename _containerType > 
	    void getSortedIDs( boost::shared_ptr< _containerType > & containerPtr );

	  //! Getter of the sorted IDs from a template iterator type  
	  template < typename _iteratorType >
	    void getSortedIDs( _iteratorType begin, _iteratorType end );
	
      //@}
	  
    protected:
      
      //! @name Protected Members
      //@{
	
	  //! Shared pointer to the ID structure
	  IDPtr_Type M_IDsPtr; 
      
	  //! Shared pointer to the data structure
	  dataPtr_Type M_dataPtr;
      
      //@}
      
    private:
      
      //! @name Private Members & Methods
      //@{

	  //! Flag to know if the structure has been sorted already or not.
	  bool M_sortedFlag;
      
	  //! Method to perform the sorting
	  void performSort();
	
      //@}
      
      
    };
    
    // Definition of the default constructor
    template < typename _T >
    ExtendedSort< _T >::
    ExtendedSort()
    :
    M_dataPtr( new data_Type() ),
    M_IDsPtr( new ID_Type() )
    {
    }
     
    // Definition of the constructor from a template container type
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

    // Definition of the constructor from the template iterator type
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
      
    // Definition of the template container type setter of data
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
	
    // Definition of the template iterator type setter of data
    template < typename _T >
      template < typename _iteratorType >
      void
      ExtendedSort< _T >::
      setData( const _iteratorType & begin, const _iteratorType & end )
      {
	this->M_sortedFlag = false;
	
	this->M_dataPtr->clear();
	
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

      // Definition of the template container type getter of sorted IDS
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
      
      // Definition of the template shared pointer container type getter of sorted IDS
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

      // Definition of the template iterator type getter of sorted IDS
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

    // Definition of the method to perform the sorting
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