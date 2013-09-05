
#ifndef __FACTORY_HPP__
#define __FACTORY_HPP__

#include <source/HPCSDefs.hpp>

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
    
    return (it->second)();
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

  
  
  
  
  template < typename _returnType >
  struct CreationRule
  {
      CreationRule(){}
      
      virtual _returnType * operator()() const
      {
	  return new _returnType();
      }
  };

  //! QUI HO BISOGNO DI UN WRAPPER Ad un puntatore a creation rule< _returnType, _policy >, per costruire una factory che
  //! che possa usare in ingresso delle regole di creazione polimorfiche.
  
  template < typename _returnType >
  class CreationRulePtrWrapper
  {
  public:
    
      typedef boost::shared_ptr< CreationRule< _returnType > > creationRulePtr_Type;
    
      CreationRulePtrWrapper( const creationRulePtr_Type & rulePtr );
  
      _returnType * operator()() const {  return (*(this->M_creationRulePtr))(); }
      
  private:
    
     creationRulePtr_Type M_creationRulePtr;
      
  };  

  template < typename _returnType >
  CreationRulePtrWrapper< _returnType >::
  CreationRulePtrWrapper( const creationRulePtr_Type & rulePtr )
  :
  M_creationRulePtr( rulePtr )
  {}

  
}
#endif
