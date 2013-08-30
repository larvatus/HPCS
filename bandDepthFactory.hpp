

#ifndef __BDFACTORY_HPP__
#define __BDFACTORY_HPP__

#include <factory.hpp>
#include <bandDepth.hpp>
// #include <bandDepthRef.hpp>

namespace HPCS
{
  
  //! TODO MAKE THIS STRUCT A BDPolicy TEMPLATE PUBLIC DERIVATION FROM creationRule< BandDepthBase, _policy >
  
  struct 
  CreateBDAll2 : public CreationRule< BandDepthBase, All >
  {
    CreateBDAll2(){}
    
    BandDepthBase * operator()() const
    {    
      return new BandDepth< 2 >();
    }
  };


  struct 
  CreateBDAll3 : public CreationRule< BandDepthBase, All >
  {
    CreateBDAll3(){}
    
    BandDepthBase * operator()() const
    {
      return new BandDepth< 3 >();
    }
  };

  
  // struct 
  // createBDRef2 : public creationRule< BandDepthBase, All >
  // {
  //   BandDepthBase * operator()() const
  //   {
  //     return new BandDepthRef< 2 >();
  //   }
  // };
  // 
  // 
  // struct 
  // createBDRef3 : public creationRule< BandDepthBase, All >
  // {
  //   BandDepthBase * operator()() const
  //   {
  //     return new BandDepthRef< 3 >();
  //   }
  // };

  template < BDPolicy _policy >
  class BDFactory 
  : 
  public Factory< BandDepthBase, UInt, CreationRulePtrWrapper< BandDepthBase, _policy > >
  {
    
  public:
    
    typedef BDPolicy bdPolicy_Type;
	
    typedef Factory< BandDepthBase, UInt,  CreationRulePtrWrapper< BandDepthBase, _policy >  > factoryBase_Type;

    typedef BandDepthBase bdBase_Type;
	    
    BDFactory();
    
    ~BDFactory(){}
    
  private:
    
  };
    
  template <>
  BDFactory< All >::
  BDFactory()
  :
  Factory< BandDepthBase, UInt,  CreationRulePtrWrapper< BandDepthBase, All > >()
  {    
    typedef boost::shared_ptr< CreationRule< BandDepthBase, All > > Ptr_Type;
    typedef CreationRulePtrWrapper< BandDepthBase, All > wrapper_Type;
    
    bool flag = this->registerProduct( 2, wrapper_Type( Ptr_Type( new CreateBDAll2() ) ) );
      
    flag = flag & ( this->registerProduct( 3, wrapper_Type( Ptr_Type( new CreateBDAll3() ) ) ) );
	  
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

#endif /* __BDFACTORY_HPP__ */