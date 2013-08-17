
#ifndef __FACTORY_HPP__
#define __FACTORY_HPP__

#include <boost/function.hpp>
#include <map>
#include <cassert>

namespace HPCS
{

template < class ProductType,
	   typename KeyType, 
	   typename CreatorType = boost::function< ProductType * () >
	 >
class Factory 
{
public:

     typedef ProductType product_Type;
  
     typedef KeyType key_Type;
     
     typedef CreatorType creator_Type;
     
     typedef std::map< key_Type, creator_Type> map_Type;

     Factory() {}
 
     virtual ~Factory() {}
 
     bool registerProduct( const key_Type & key, const creator_Type & rule );
 
     bool unregisterProduct( const key_Type & key );
 
     product_Type * create( const key_Type & key );

     protected:
      
      map_Type M_map;
      
};

template < class ProductType, 
	   typename KeyType, 
	   typename CreatorType >
inline
bool
Factory< ProductType, KeyType, CreatorType >::
registerProduct( const key_Type & key, const creator_Type & rule )
{
  return M_map.insert( typename map_Type::value_type( key, rule ) ).second;
}

template < class ProductType, 
	   typename KeyType, 
	   typename CreatorType >
inline
bool
Factory< ProductType, KeyType, CreatorType >::
unregisterProduct( const key_Type & key )
{
   return 1 == M_map.erase( key );
}

template < class ProductType, 
	   typename KeyType, 
	   typename CreatorType >
typename Factory< ProductType, KeyType, CreatorType >::product_Type *
Factory< ProductType, KeyType, CreatorType >::
create( const key_Type & key )
{
   typename map_Type::const_iterator it( M_map.find( key ) );
	 
    assert( it != M_map.end() );
	 
    return ( it->second )();
}


}


#endif
