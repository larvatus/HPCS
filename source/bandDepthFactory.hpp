

#ifndef __BDFACTORY_HPP__
#define __BDFACTORY_HPP__

#include <source/factory.hpp>
#include <source/bandDepth.hpp>
#include <source/bandDepthRef.hpp>

namespace HPCS
{  
  
  // BD ALL
  
  //! @class  CreateBDAll2 this struct implements a functor creation rule for a BandDepth object with _J = 2
  struct 
  CreateBDAll2 : public CreationRule< BandDepthBase< All > >
  {
    //! Default constructor
    CreateBDAll2(){}

    //! Overload of operator() for functor functionality
    BandDepthBase< All > * operator()() const
    {    
      return new BandDepth< 2 >();
    }
  };


  //! @class CreateBDAll3 this struct implements a functor creation rule for a BandDepth object with _J = 3
  struct 
  CreateBDAll3 : public CreationRule< BandDepthBase< All > >
  {
    //! Default constructor
    CreateBDAll3(){}

    //! Overload of operator() for functor functionality
    BandDepthBase< All > * operator()() const
    {
      return new BandDepth< 3 >();
    }
  };
  
  //! @class CreateBDAll4 this struct implements a functor creation rule for a BandDepth object with _J = 4
  struct 
  CreateBDAll4 : public CreationRule< BandDepthBase< All > >
  {
    //! Default constructor
    CreateBDAll4(){}
    
    //! Overload of operator() for functor functionality
    BandDepthBase< All > * operator()() const
    {
      return new BandDepth< 4 >();
    }
  };

  //! @class CreateBDAll5 this struct implements a functor creation rule for a BandDepth object with _J = 4
  struct 
  CreateBDAll5 : public CreationRule< BandDepthBase< All > >
  {
    //! Default constructor
    CreateBDAll5(){}
    
    //! Overload of operator() for functor functionality
    BandDepthBase< All > * operator()() const
    {
      return new BandDepth< 5 >();
    }
  };

  
  // BD REFERENCE

  //! @class CreateBDRef2 this struct implements a functor creation rule for a BandDepthRef object with _J = 2
  struct 
  CreateBDRef2 : public CreationRule< BandDepthBase< Reference > >
  {
    //! Default constructor   
    CreateBDRef2(){}
    
    //! Overload of operator() for functor functionality
    BandDepthBase< Reference > * operator()() const
    {
       return new BandDepthRef< 2 >();
     }
   };
   
   //! @class CreateBDRef3 this struct implements a functor creation rule for a BandDepthRef object with _J = 3
   struct 
   CreateBDRef3 : public CreationRule< BandDepthBase < Reference > >
   {
     //! Default constructor
     CreateBDRef3(){}
     
     //! Overload of operator() for functor functionality
     BandDepthBase< Reference > * operator()() const
     {
       return new BandDepthRef< 3 >();
     }
   };

   //! @class CreateBDRef4 this struct implements a functor creation rule for a BandDepthRef object with _J = 4
   struct 
   CreateBDRef4 : public CreationRule< BandDepthBase< Reference > >
   {
      //! Default constructor   
     CreateBDRef4(){}
     
     //! Overload of operator() for functor functionality
     BandDepthBase< Reference > * operator()() const
     {
       return new BandDepthRef< 4 >();
     }
   };

   //! @class CreateBDRef5 this struct implements a functor creation rule for a BandDepthRef object with _J = 5
   struct 
   CreateBDRef5 : public CreationRule< BandDepthBase < Reference > >
   {
     //! Default constructor   
     CreateBDRef5(){}
     
     //! Overload of operator() for functor functionality
     BandDepthBase< Reference > * operator()() const
     {
       return new BandDepthRef< 5 >();
     }
   };


  //! @class BDFactory This class implements a factory for the flexible creation of BandDepth objects
  /*!
   *  This class implements a factory useful to istantiate BandDepth products with information available at
   *  run time. The constructor_type of this class is a wrapper to a creation rule, needed to give a unified
   *  type to the hierarchy of creationRule functors.
   */
  template < BDPolicy _policy >
  class BDFactory 
  : 
  public Factory< BandDepthBase< _policy >, UInt, CreationRulePtrWrapper< BandDepthBase< _policy > > >
  {
    
  public:
   
   //! @name Public Types 
   //@{
   
    //! Typedef for policy type 
    typedef BDPolicy bdPolicy_Type;
	
    //! Self typedef 
    typedef Factory< BandDepthBase< _policy >, UInt,  CreationRulePtrWrapper< BandDepthBase< _policy > >  > factoryBase_Type;

    //! Typedef of the band depth base class
    typedef BandDepthBase< _policy > bdBase_Type;

   //@}
   
   //! @name Constructor & Destructor 
   //@{
     
    //! Default void constructor
    /*!
     * This method is specialized for the values of All and Reference of the BDPolicy, since they need different creation rules.
     * Moreover, rules for J=2,3,4,5 are automatically registered at creation time.
     */
    BDFactory();
    
    //! Standard destructor
    ~BDFactory(){}
   //@}
    
  private:
    
  };
    
  // Specialization of the constructor for All value of _policy
  template <>
  BDFactory< All >::
  BDFactory()
  :
  Factory< BandDepthBase< All >, UInt,  CreationRulePtrWrapper< BandDepthBase< All > > >()
  {    
    // Useful typedefs
    typedef boost::shared_ptr< CreationRule< BandDepthBase< All > > > Ptr_Type;
    
    typedef CreationRulePtrWrapper< BandDepthBase< All > > wrapper_Type;
    
    // Registering rules, exploiting the wrapper to use the different creator functors
    
    bool flag = this->registerProduct( 2, wrapper_Type( Ptr_Type( new CreateBDAll2() ) ) );
      
    flag = flag & ( this->registerProduct( 3, wrapper_Type( Ptr_Type( new CreateBDAll3() ) ) ) );
    
    flag = flag & ( this->registerProduct( 4, wrapper_Type( Ptr_Type( new CreateBDAll4() ) ) ) );
    
    flag = flag & ( this->registerProduct( 5, wrapper_Type( Ptr_Type( new CreateBDAll5() ) ) ) );
	  
    // Final assert
    assert( flag == true );
      
  }

  // Specialization of the constructor for All value of _policy
  template <>
  BDFactory< Reference >::
  BDFactory()
  :
  Factory< BandDepthBase< Reference >, UInt,  CreationRulePtrWrapper< BandDepthBase< Reference > > >()
  {    
    // Useful typedefs
    typedef boost::shared_ptr< CreationRule< BandDepthBase< Reference > > > Ptr_Type;
    
    typedef CreationRulePtrWrapper< BandDepthBase< Reference > > wrapper_Type;
    
    // Registering rules, exploiting the wrapper to use the different creator functors
    
    bool flag = this->registerProduct( 2, wrapper_Type( Ptr_Type( new CreateBDRef2() ) ) );
      
    flag = flag & ( this->registerProduct( 3, wrapper_Type( Ptr_Type( new CreateBDRef3() ) ) ) );
    
    flag = flag & ( this->registerProduct( 4, wrapper_Type( Ptr_Type( new CreateBDRef4() ) ) ) );
    
    flag = flag & ( this->registerProduct( 5, wrapper_Type( Ptr_Type( new CreateBDRef5() ) ) ) );
	  
    // Final assert
    assert( flag == true );
      
  }

}

#endif /* __BDFACTORY_HPP__ */