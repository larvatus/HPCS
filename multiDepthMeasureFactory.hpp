#ifndef __MULTIDMFACTORY_HPP__
#define __MULTIDMFACTORY_HPP__

#include <factory.hpp>
#include <multiDepthMeasure.hpp>


namespace HPCS
{
  
  template < BDPolicy _policy >
  struct CreateMultiDM2 : public CreationRule< MultiDepthMeasureBase< _policy > >
  {
    CreateMultiDM2(){}
    
    MultiDepthMeasureBase< _policy > * operator()() const
    {    
      return new MultiDepthMeasure< 2, _policy >();
    }
  };
  
  template < BDPolicy _policy >
  struct CreateMultiDM3 : public CreationRule< MultiDepthMeasureBase< _policy > >
  {
    CreateMultiDM3(){}
    
    MultiDepthMeasureBase< _policy > * operator()() const
    {    
      return new MultiDepthMeasure< 3, _policy >();
    }
  };
  
  template < BDPolicy _policy >
  struct CreateMultiDM4 : public CreationRule< MultiDepthMeasureBase< _policy > >
  {
    CreateMultiDM4(){}
    
    MultiDepthMeasureBase< _policy > * operator()() const
    {    
      return new MultiDepthMeasure < 4, _policy >();
    }
  };
  
  template < BDPolicy _policy >
  struct CreateMultiDM5 : public CreationRule< MultiDepthMeasureBase< _policy > >
  {
    CreateMultiDM5(){}
    
    MultiDepthMeasureBase< _policy > * operator()() const
    {    
      return new MultiDepthMeasure< 5, _policy >();
    }
  };
  
  
  template < BDPolicy _policy >
  class MultiDepthMeasureFactory 
  : 
  public Factory< MultiDepthMeasureBase< _policy >, UInt, CreationRulePtrWrapper< MultiDepthMeasureBase< _policy > > >
  {
    
  public:
    
    typedef Factory< MultiDepthMeasureBase< _policy >, UInt, CreationRulePtrWrapper< MultiDepthMeasureBase< _policy > > > factoryBase_Type;

    typedef MultiDepthMeasureBase< _policy > mdmBase_Type;
	    
    MultiDepthMeasureFactory();
    
    ~MultiDepthMeasureFactory(){}
    
  private:
    
  };
  
  template < BDPolicy _policy >
  MultiDepthMeasureFactory< _policy >::
  MultiDepthMeasureFactory()
  :
  Factory< MultiDepthMeasureBase< _policy >, UInt, CreationRulePtrWrapper< MultiDepthMeasureBase< _policy > > >()
  {
    typedef boost::shared_ptr< CreationRule< MultiDepthMeasureBase< _policy > > > Ptr_Type;
    typedef CreationRulePtrWrapper< MultiDepthMeasureBase< _policy > > wrapper_Type;    
    
    bool flag = this->registerProduct( 2, wrapper_Type( Ptr_Type( new CreateMultiDM2 < _policy >() ) ) );
      
    flag = flag & ( this->registerProduct( 3, wrapper_Type( Ptr_Type( new CreateMultiDM3 < _policy >() ) ) ) );
	  
    flag = flag & ( this->registerProduct( 4, wrapper_Type( Ptr_Type( new CreateMultiDM4 < _policy >() ) ) ) );
    
    flag = flag & ( this->registerProduct( 5, wrapper_Type ( Ptr_Type( new CreateMultiDM5 < _policy >() ) ) ) );
    
    assert( flag == true );
      
  }
  
}



#endif /* __MULTIDMFACTORY_HPP__ */