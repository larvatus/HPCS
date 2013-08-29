
#ifndef __FACTORY_HPP__
#define __FACTORY_HPP__

#include <HPCSDefs.hpp>

#include <bandDepth.hpp>

namespace HPCS
{

template < class ProductType,
	   typename KeyType, 
	   typename CreatorType
	 >
class Factory 
{
public:

     typedef ProductType product_Type;
  
     typedef KeyType key_Type;
     
     typedef CreatorType creator_Type;
     
//      typedef boost::shared_ptr< creator_Type > creatorPtr_Type;
     
     typedef std::map< key_Type, creator_Type> map_Type;

     Factory() {}
 
     virtual ~Factory() {}
 
     bool registerProduct( const key_Type & key, const creator_Type & rule );
 
     bool unregisterProduct( const key_Type & key );
 
     product_Type * create( const key_Type & key );
     
     virtual void showMe( std::ostream & output = std::cout ) const;

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
    
    return (*(it->second))();
}

template < class ProductType, 
	   typename KeyType, 
	   typename CreatorType >
void
Factory< ProductType, KeyType, CreatorType >::
showMe( std::ostream & output ) const
{
  output << " Factory contains " << this->M_map.size() << std::endl;
  
  return;
}

template < BDPolicy _policy >
struct creationRule
{
  creationRule(){}
  
  virtual BandDepthBase * operator()() const
  {
      return new BandDepthBase();
  }
};

struct 
createBDAll2 : public creationRule< All >
{
  createBDAll2(){}
  
  BandDepthBase * operator()() const
  {    
    return new BandDepth< 2 >();
  }
};


struct 
createBDAll3 : public creationRule< All >
{
  createBDAll3(){}
  
  BandDepthBase * operator()() const
  {
    return new BandDepth< 3 >();
  }
};

 
// struct 
// createBDRef2 : public creationRule< All >
// {
//   BandDepthBase * operator()() const
//   {
//     return new BandDepthRef< 2 >();
//   }
// };
// 
// 
// struct 
// createBDRef3 : public creationRule< All >
// {
//   BandDepthBase * operator()() const
//   {
//     return new BandDepthRef< 3 >();
//   }
// };

template < BDPolicy _policy >
class BDFactory 
: 
public Factory< BandDepthBase, UInt, boost::shared_ptr< creationRule< _policy > > >
{
  
public:
  
   typedef BDPolicy bdPolicy_Type;
      
   typedef Factory< BandDepthBase, UInt, boost::shared_ptr< creationRule< _policy > > > factoryBase_Type;

   typedef BandDepthBase bdBase_Type;
          
   BDFactory(){};
  
   ~BDFactory(){}
  
private:
  
};
  
template <>
BDFactory< All >::
BDFactory()
:
Factory< BandDepthBase, UInt, boost::shared_ptr< creationRule< All > > >()
{
   typedef boost::shared_ptr< creationRule< All > > Ptr_Type;
   
   bool flag = ( this->registerProduct( 2, Ptr_Type( new createBDAll2() ) ));
    
   flag = flag & ( this->registerProduct( 3, Ptr_Type( new createBDAll3() ) ));
        
   assert( flag == true );
    
}

// template <>
// BDFactory< Reference >::
// BDFactory()
// :
// Factory< BandDepthBase, UInt, creationRule< Reference > >()
// {
//     
//    typedef typename factoryBase_Type::creator_Type creator_Type;
//    
//     bool flag = ( this->registerProduct( 2, createBDRef2 ));
//     
//     flag = flag & ( this->registerProduct( 3, createBDRef3 ));
//         
//     assert( flag == true )
//     
// }

}

#endif
