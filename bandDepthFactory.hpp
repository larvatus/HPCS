

#ifndef __BDFACTORY_HPP__
#define __BDFACTORY_HPP__

#include <factory.hpp>
#include <bandDepth.hpp>
#include <bandDepthRef.hpp>

namespace HPCS
{  
  
  // BD ALL
  
  struct 
  CreateBDAll2 : public CreationRule< BandDepthBase< All > >
  {
    CreateBDAll2(){}
    
    BandDepthBase< All > * operator()() const
    {    
      return new BandDepth< 2 >();
    }
  };


  struct 
  CreateBDAll3 : public CreationRule< BandDepthBase< All > >
  {
    CreateBDAll3(){}
    
    BandDepthBase< All > * operator()() const
    {
      return new BandDepth< 3 >();
    }
  };
  
  struct 
  CreateBDAll4 : public CreationRule< BandDepthBase< All > >
  {
    CreateBDAll4(){}
    
    BandDepthBase< All > * operator()() const
    {
      return new BandDepth< 4 >();
    }
  };
  
  struct 
  CreateBDAll5 : public CreationRule< BandDepthBase< All > >
  {
    CreateBDAll5(){}
    
    BandDepthBase< All > * operator()() const
    {
      return new BandDepth< 5 >();
    }
  };

  
  // BD REFERENCE
  
  struct 
  CreateBDRef2 : public CreationRule< BandDepthBase< Reference > >
  {
    BandDepthBase< Reference > * operator()() const
    {
       return new BandDepthRef< 2 >();
     }
   };
   
   struct 
   CreateBDRef3 : public CreationRule< BandDepthBase < Reference > >
   {
     BandDepthBase< Reference > * operator()() const
     {
       return new BandDepthRef< 3 >();
     }
   };
   
   struct 
   CreateBDRef4 : public CreationRule< BandDepthBase< Reference > >
   {
     BandDepthBase< Reference > * operator()() const
     {
       return new BandDepthRef< 4 >();
     }
   };

   struct 
   CreateBDRef5 : public CreationRule< BandDepthBase < Reference > >
   {
     BandDepthBase< Reference > * operator()() const
     {
       return new BandDepthRef< 5 >();
     }
   };


  template < BDPolicy _policy >
  class BDFactory 
  : 
  public Factory< BandDepthBase< _policy >, UInt, CreationRulePtrWrapper< BandDepthBase< _policy > > >
  {
    
  public:
    
    typedef BDPolicy bdPolicy_Type;
	
    typedef Factory< BandDepthBase< _policy >, UInt,  CreationRulePtrWrapper< BandDepthBase< _policy > >  > factoryBase_Type;

    typedef BandDepthBase< _policy > bdBase_Type;
	    
    BDFactory();
    
    ~BDFactory(){}
    
  private:
    
  };
    
  template <>
  BDFactory< All >::
  BDFactory()
  :
  Factory< BandDepthBase< All >, UInt,  CreationRulePtrWrapper< BandDepthBase< All > > >()
  {    
    typedef boost::shared_ptr< CreationRule< BandDepthBase< All > > > Ptr_Type;
    
    typedef CreationRulePtrWrapper< BandDepthBase< All > > wrapper_Type;
    
    bool flag = this->registerProduct( 2, wrapper_Type( Ptr_Type( new CreateBDAll2() ) ) );
      
    flag = flag & ( this->registerProduct( 3, wrapper_Type( Ptr_Type( new CreateBDAll3() ) ) ) );
    
    flag = flag & ( this->registerProduct( 4, wrapper_Type( Ptr_Type( new CreateBDAll4() ) ) ) );
    
    flag = flag & ( this->registerProduct( 5, wrapper_Type( Ptr_Type( new CreateBDAll5() ) ) ) );
	  
    assert( flag == true );
      
  }

  template <>
  BDFactory< Reference >::
  BDFactory()
  :
  Factory< BandDepthBase< Reference >, UInt,  CreationRulePtrWrapper< BandDepthBase< Reference > > >()
  {    
    typedef boost::shared_ptr< CreationRule< BandDepthBase< Reference > > > Ptr_Type;
    
    typedef CreationRulePtrWrapper< BandDepthBase< Reference > > wrapper_Type;
    
    bool flag = this->registerProduct( 2, wrapper_Type( Ptr_Type( new CreateBDRef2() ) ) );
      
    flag = flag & ( this->registerProduct( 3, wrapper_Type( Ptr_Type( new CreateBDRef3() ) ) ) );
    
    flag = flag & ( this->registerProduct( 4, wrapper_Type( Ptr_Type( new CreateBDRef4() ) ) ) );
    
    flag = flag & ( this->registerProduct( 5, wrapper_Type( Ptr_Type( new CreateBDRef5() ) ) ) );
	  
    assert( flag == true );
      
  }

}

#endif /* __BDFACTORY_HPP__ */