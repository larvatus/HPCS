#ifndef __DMFACTORY_HPP__
#define __DMFACTORY_HPP__

#include <source/factory.hpp>
#include <source/depthMeasure.hpp>


namespace HPCS
{
  
  template < BDPolicy _policy >
  struct CreateDM2 : public CreationRule< DepthMeasureBase< _policy > >
  {
    CreateDM2(){}
    
    DepthMeasureBase< _policy > * operator()() const
    {    
      return new DepthMeasure< 2, _policy >();
    }
  };
  
  template < BDPolicy _policy >
  struct CreateDM3 : public CreationRule< DepthMeasureBase< _policy > >
  {
    CreateDM3(){}
    
    DepthMeasureBase< _policy > * operator()() const
    {    
      return new DepthMeasure< 3, _policy >();
    }
  };
  
  template < BDPolicy _policy >
  struct CreateDM4 : public CreationRule< DepthMeasureBase< _policy > >
  {
    CreateDM4(){}
    
    DepthMeasureBase< _policy > * operator()() const
    {    
      return new DepthMeasure < 4, _policy >();
    }
  };
  
  template < BDPolicy _policy >
  struct CreateDM5 : public CreationRule< DepthMeasureBase< _policy > >
  {
    CreateDM5(){}
    
    DepthMeasureBase< _policy > * operator()() const
    {    
      return new DepthMeasure< 5, _policy >();
    }
  };
  
  
  template < BDPolicy _policy >
  class DMFactory 
  : 
  public Factory< DepthMeasureBase< _policy >, UInt, CreationRulePtrWrapper< DepthMeasureBase< _policy > > >
  {
    
  public:
	
    typedef Factory< DepthMeasureBase< _policy >, UInt, CreationRulePtrWrapper< DepthMeasureBase< _policy > > > factoryBase_Type;

    typedef DepthMeasureBase< _policy > dmBase_Type;
	    
    DMFactory();
    
    ~DMFactory(){}
    
  private:
    
  };
  
  template < BDPolicy _policy >
  DMFactory< _policy >::
  DMFactory()
  :
  Factory< DepthMeasureBase< _policy >, UInt, CreationRulePtrWrapper< DepthMeasureBase< _policy > > >()
  {
    typedef boost::shared_ptr< CreationRule< DepthMeasureBase< _policy > > > Ptr_Type;
    typedef CreationRulePtrWrapper< DepthMeasureBase< _policy > > wrapper_Type;    
    
    bool flag = this->registerProduct( 2, wrapper_Type( Ptr_Type( new CreateDM2 < _policy >() ) ) );
      
    flag = flag & ( this->registerProduct( 3, wrapper_Type( Ptr_Type( new CreateDM3 < _policy >() ) ) ) );
	  
    flag = flag & ( this->registerProduct( 4, wrapper_Type( Ptr_Type( new CreateDM4 < _policy >() ) ) ) );
    
    flag = flag & ( this->registerProduct( 5, wrapper_Type ( Ptr_Type( new CreateDM5 < _policy >() ) ) ) );
    
    assert( flag == true );
      
  }
  
}



#endif /* __DMFACTORY_HPP__ */