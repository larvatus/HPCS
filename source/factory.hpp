
#ifndef __FACTORY_HPP__
#define __FACTORY_HPP__

#include <source/HPCSDefs.hpp>

namespace HPCS
{

  //! @class Factory this class implements a generic abstract factory pattern
  /*!
   * @param ProductType is the product of produced types,
   * @param KeyType is the type for the indexing in the map structure,
   * @param CraetorType is the type for the creation rules.
   * 
   */
  template < class ProductType,
	    typename KeyType, 
	    typename CreatorType
	  >
  class Factory 
  {
  public:

     //! @name Public Types
     //@{

       //! Typedef for the product type
       typedef ProductType product_Type;
    
       //! Typedef for the key type
       typedef KeyType key_Type;
      
       //! Typedef for the creator type
       typedef CreatorType creator_Type;
       
       //! Typedef for the map type used to store creation rules      
       typedef std::map< key_Type, creator_Type> map_Type;
       
     //@}
    

     //! @name Constructor & Destructor
     //@{

	 //! Standard default constructor
         Factory() {}
  
	 //! Standard default destructor
	 virtual ~Factory() {}
       
     //@}

     //! @name Public Methods
     //@{
       
        //! Method to register products
	bool registerProduct( const key_Type & key, const creator_Type & rule );
  
	//! Method to unregister products
	bool unregisterProduct( const key_Type & key );
  
	//! Method to create objects
	product_Type * create( const key_Type & key );
      
	//! Method to show the status of the class
	virtual void showMe( std::ostream & output = std::cout ) const;

     //@}
      protected:
	
	//! The map object storing rules
	map_Type M_map;
	
  };

  // Definition of the registration method
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

  // Definition of the unregistration method
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

  // Definition of the create method
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

  // Definition of the showMe method
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


  //! @class CreationRule is the base class from which to derive the creation rules to be used inside factories.
  /*!
   * This is a tempalte class serving as a base class from which to derive the rules to create objects inside the
   * several factories. Basically, a functor class.
   */
  template < typename _returnType >
  struct CreationRule
  {
      //! Default constructor
      CreationRule(){}
      
      //! Overload of operator() to achieve functor behavior
      virtual _returnType * operator()() const
      {
	  return new _returnType();
      }
  };

  //! @class CreationRulePtrWrapper is the wrapper class used to have a common creation_Type inside classes using creationRules
  /*!
   *  CreationRules are arranged in a hierarchy that doesn't allow to have a unique creation type inside a class, thus a
   *  wrapper is needed, taking a pointer to a creation rule and returning the created objects. It gives a common type to setup
   *  a factory type.
   */
  template < typename _returnType >
  class CreationRulePtrWrapper
  {
  public:
    
    //! @name Public Types
    //@{
      //! Typedef for the shared pointer to a creation rule
      typedef boost::shared_ptr< CreationRule< _returnType > > creationRulePtr_Type;

    //@}
    
    //! @name Constructor and Destructor
    //@{
      
      //! Standard constructor from a pointer to a creation rule
      CreationRulePtrWrapper( const creationRulePtr_Type & rulePtr );
      
      //! Standard default destructor
      virtual ~CreationRulePtrWrapper(){}
      
    //@}
    
    //! Overload of operator() to give to the class a functor behavior, thus allowing the wrapping function.
    _returnType * operator()() const {  return (*(this->M_creationRulePtr))(); }
      
  private:

     //! Stored pointer to the creation rule
     creationRulePtr_Type M_creationRulePtr;
      
  };  

  // Definition of the constructor
  template < typename _returnType >
  CreationRulePtrWrapper< _returnType >::
  CreationRulePtrWrapper( const creationRulePtr_Type & rulePtr )
  :
  M_creationRulePtr( rulePtr )
  {}


  //////////////////////////////////
  ////	GENERIC PROXY CLASS	////
  /////////////////////////////////
  
  
//   template < typename T, typename ProductType >
//   class Proxy
//   {
//   public:
//     
//     Proxy( const char * const & name );
//     
//     virtual ~Proxy(){};
//     
//     static ProductType * build();
//     
//   private:
//     
//     Proxy( const Proxy & proxy ){};
//     
//     Proxy & operator=( Proxy const & proxy ){};
//     
//   };
  
}
#endif
