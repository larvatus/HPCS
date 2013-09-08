#ifndef __DMFACTORY_HPP__
#define __DMFACTORY_HPP__

#include <source/factory.hpp>
#include <source/depthMeasure.hpp>


namespace HPCS
{
  
  //! @class  CreateDM2 this struct implements a functor creation rule for a DepthMeasure object with _J = 2
  template < BDPolicy _policy >
  struct CreateDM2 : public CreationRule< DepthMeasureBase< _policy > >
  {
    //! Default constructor
    CreateDM2(){}
    
    //! Overload of operator() to reach the functor functionality
    DepthMeasureBase< _policy > * operator()() const
    {    
      return new DepthMeasure< 2, _policy >();
    }
  };
  
  //! @class  CreateDM2 this struct implements a functor creation rule for a DepthMeasure object with _J = 3
  template < BDPolicy _policy >
  struct CreateDM3 : public CreationRule< DepthMeasureBase< _policy > >
  {
     //! Default constructor
    CreateDM3(){}
    
    //! Overload of operator() to reach the functor functionality
    DepthMeasureBase< _policy > * operator()() const
    {    
      return new DepthMeasure< 3, _policy >();
    }
  };

  //! @class  CreateDM2 this struct implements a functor creation rule for a DepthMeasure object with _J = 4
  template < BDPolicy _policy >
  struct CreateDM4 : public CreationRule< DepthMeasureBase< _policy > >
  {
    //! Default constructor
    CreateDM4(){}
    
    //! Overload of operator() to reach the functor functionality
    DepthMeasureBase< _policy > * operator()() const
    {    
      return new DepthMeasure < 4, _policy >();
    }
  };

  //! @class  CreateDM2 this struct implements a functor creation rule for a DepthMeasure object with _J = 5
  template < BDPolicy _policy >
  struct CreateDM5 : public CreationRule< DepthMeasureBase< _policy > >
  {
    //! Default constructor
    CreateDM5(){}
    
     //! Overload of operator() to reach the functor functionality
    DepthMeasureBase< _policy > * operator()() const
    {    
      return new DepthMeasure< 5, _policy >();
    }
  };
  
 //! @class DMFactory This class implements a factory for the flexible creation of DepthMeasure objects
 /*!
  *  This class implements a factory useful to istantiate DepthMeasure products with information available at
  *  run time. The constructor_type of this class is a wrapper to a creation rule, needed to give a unified
  *  type to the hierarchy of creationRule functors.
  */
  template < BDPolicy _policy >
  class DMFactory 
  : 
  public Factory< DepthMeasureBase< _policy >, UInt, CreationRulePtrWrapper< DepthMeasureBase< _policy > > >
  {
    
  public:
    
    //! @name Public Types
    //@{

      //! Self typedef
      typedef Factory< DepthMeasureBase< _policy >, UInt, CreationRulePtrWrapper< DepthMeasureBase< _policy > > > factoryBase_Type;

      //! Typedef for the depth measure base type
      typedef DepthMeasureBase< _policy > dmBase_Type;

    //@}
      
    //! @name Constructor & Destructor 
    //@{

     //! Default void constructor
     /*!
      * This method is specialized for the values of All and Reference of the BDPolicy, since they need different creation rules.
      * Moreover, rules for J=2,3,4,5 are automatically registered at creation time.
      */
      DMFactory();
      
      //! Default destructor
      ~DMFactory(){}
    
    //@}
    
  private:
    
  };
  
  // Definition of the void default constructor
  template < BDPolicy _policy >
  DMFactory< _policy >::
  DMFactory()
  :
  Factory< DepthMeasureBase< _policy >, UInt, CreationRulePtrWrapper< DepthMeasureBase< _policy > > >()
  {
    // Useful typedefs
    
    typedef boost::shared_ptr< CreationRule< DepthMeasureBase< _policy > > > Ptr_Type;
    
    typedef CreationRulePtrWrapper< DepthMeasureBase< _policy > > wrapper_Type;    
    
    // Registering rules, exploiting the wrapper to use the different creator functors
    
    bool flag = this->registerProduct( 2, wrapper_Type( Ptr_Type( new CreateDM2 < _policy >() ) ) );
      
    flag = flag & ( this->registerProduct( 3, wrapper_Type( Ptr_Type( new CreateDM3 < _policy >() ) ) ) );
	  
    flag = flag & ( this->registerProduct( 4, wrapper_Type( Ptr_Type( new CreateDM4 < _policy >() ) ) ) );
    
    flag = flag & ( this->registerProduct( 5, wrapper_Type ( Ptr_Type( new CreateDM5 < _policy >() ) ) ) );
    
    // Final assert
    
    assert( flag == true );
      
  }
  
}



#endif /* __DMFACTORY_HPP__ */