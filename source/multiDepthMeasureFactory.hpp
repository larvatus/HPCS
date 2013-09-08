#ifndef __MULTIDMFACTORY_HPP__
#define __MULTIDMFACTORY_HPP__

#include <source/factory.hpp>
#include <source/multiDepthMeasure.hpp>


namespace HPCS
{
  
  //! @class  CreateMultiDM2 this struct implements a functor creation rule for a MultiDepthMeasure object with _J = 2
  template < BDPolicy _policy >
  struct CreateMultiDM2 : public CreationRule< MultiDepthMeasureBase< _policy > >
  {
    //! Default construuctor
    CreateMultiDM2(){}
    
    //! Overload of operator() to reach the functor functionality
    MultiDepthMeasureBase< _policy > * operator()() const
    {    
      return new MultiDepthMeasure< 2, _policy >();
    }
  };
  
  //! @class  CreateMultiDM3 this struct implements a functor creation rule for a MultiDepthMeasure object with _J = 3
  template < BDPolicy _policy >
  struct CreateMultiDM3 : public CreationRule< MultiDepthMeasureBase< _policy > >
  {
    //! Default construuctor
    CreateMultiDM3(){}
    
    //! Overload of operator() to reach the functor functionality    
    MultiDepthMeasureBase< _policy > * operator()() const
    {    
      return new MultiDepthMeasure< 3, _policy >();
    }
  };

  //! @class  CreateMultiDM4 this struct implements a functor creation rule for a MultiDepthMeasure object with _J = 4  
  template < BDPolicy _policy >
  struct CreateMultiDM4 : public CreationRule< MultiDepthMeasureBase< _policy > >
  {
    //! Default construuctor
    CreateMultiDM4(){}
    
    //! Overload of operator() to reach the functor functionality
    MultiDepthMeasureBase< _policy > * operator()() const
    {    
      return new MultiDepthMeasure < 4, _policy >();
    }
  };

  //! @class  CreateMultiDM5 this struct implements a functor creation rule for a MultiDepthMeasure object with _J = 5 
  template < BDPolicy _policy >
  struct CreateMultiDM5 : public CreationRule< MultiDepthMeasureBase< _policy > >
  {
    //! Default construuctor
    CreateMultiDM5(){}
    
    //! Overload of operator() to reach the functor functionality
    MultiDepthMeasureBase< _policy > * operator()() const
    {    
      return new MultiDepthMeasure< 5, _policy >();
    }
  };
  
 //! @class MultiDepthMeasureFactory This class implements a factory for the flexible creation of MultiDepthMeasure objects
 /*!
  *  This class implements a factory useful to istantiate MultiDepthMeasure products with information available at
  *  run time. The constructor_type of this class is a wrapper to a creation rule, needed to give a unified
  *  type to the hierarchy of creationRule functors.
  */
  template < BDPolicy _policy >
  class MultiDepthMeasureFactory 
  : 
  public Factory< MultiDepthMeasureBase< _policy >, UInt, CreationRulePtrWrapper< MultiDepthMeasureBase< _policy > > >
  {
    
  public:
    
    //! @name Public Types
    //@{

      //! Self typedef
      typedef Factory< MultiDepthMeasureBase< _policy >, UInt, CreationRulePtrWrapper< MultiDepthMeasureBase< _policy > > > factoryBase_Type;

      //! Typedef for the multiDepthMeasure type
      typedef MultiDepthMeasureBase< _policy > mdmBase_Type;
      
    //@}
    
    //! @name Constructor & Destructor
    //@{

     //! Default void constructor
     /*!
      *  This method is specialized for the values of All and Reference of the BDPolicy, since they need different creation rules.
      *  Moreover, rules for J=2,3,4,5 are automatically registered at creation time.
      */
      MultiDepthMeasureFactory();
    
      //! Default destructor
      ~MultiDepthMeasureFactory(){}

    //@}
	    
    
  private:
    
  };
  
  
  // Definition of the void default constructor
  template < BDPolicy _policy >
  MultiDepthMeasureFactory< _policy >::
  MultiDepthMeasureFactory()
  :
  Factory< MultiDepthMeasureBase< _policy >, UInt, CreationRulePtrWrapper< MultiDepthMeasureBase< _policy > > >()
  {
    // Useful typedefs
    
    typedef boost::shared_ptr< CreationRule< MultiDepthMeasureBase< _policy > > > Ptr_Type;

    typedef CreationRulePtrWrapper< MultiDepthMeasureBase< _policy > > wrapper_Type;    
    
    // Registering rules, exploiting the wrapper to use the different creator functors 
    
    bool flag = this->registerProduct( 2, wrapper_Type( Ptr_Type( new CreateMultiDM2 < _policy >() ) ) );
      
    flag = flag & ( this->registerProduct( 3, wrapper_Type( Ptr_Type( new CreateMultiDM3 < _policy >() ) ) ) );
	  
    flag = flag & ( this->registerProduct( 4, wrapper_Type( Ptr_Type( new CreateMultiDM4 < _policy >() ) ) ) );
    
    flag = flag & ( this->registerProduct( 5, wrapper_Type ( Ptr_Type( new CreateMultiDM5 < _policy >() ) ) ) );
    
    // Final assert 
    
    assert( flag == true );
      
  }
  
}



#endif /* __MULTIDMFACTORY_HPP__ */