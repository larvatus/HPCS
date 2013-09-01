

#ifndef __USER_FUN_HPP__
#define __USER_FUN_HPP__

#include <combinations.hpp>
#include <mpi_utility.hpp>
#include <dataSet.hpp>
#include <bandDepthData.hpp>

#include <HPCSDefs.hpp>

namespace HPCS
{    
  class BDDirectComputation
  {
    public:
    
      typedef GetPot getPot_Type;
    
      typedef CombinationsID combinationsID_Type;
      typedef CombinationsID::tuple_Type tuple_Type;
    
      typedef mpiUtility mpiUtility_Type;
      
      typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
      
      BDDirectComputation();
      
      virtual ~BDDirectComputation(){};
    
      void writeBDs( const std::string & outputFilename );

      virtual void computeBDs( const getPot_Type & dataFile, const std::string & section ){};
      
    protected:
      
      std::vector< Real > M_BDs;
      
      mpiUtilityPtr_Type M_mpiUtilPtr;
      
      UInt M_J;
      
    private:

  };
  
  
  class BDAllDirectComputation : public BDDirectComputation 
  {
    public:
      
      typedef BDDirectComputation::getPot_Type getPot_Type;
      
      BDAllDirectComputation();
      
      virtual ~BDAllDirectComputation(){};
      
      virtual void computeBDs( const getPot_Type & dataFile, const std::string & section );
      
    protected:
    
    private:
        
  };
  
  class BDRefDirectComputation : public BDDirectComputation 
  {
    public:
      
      typedef BDDirectComputation::getPot_Type getPot_Type;
      
      BDRefDirectComputation();
      
      virtual ~BDRefDirectComputation(){};
      
      virtual void computeBDs( const getPot_Type & dataFile, const std::string & section );
      
    protected:
    
    private:
        
  };
  
  
 
 /*

  class BDDirectComputation : public BandDepth< 3 >
  {
  public:
    
    typedef GetPot data_Type;
    
    typedef BandDepthData bdData_Type;
    
    BDDirectComputation();
    
    BDDirectComputation( const bdData_Type & bdData );
    
    virtual ~BDDirectComputation(){};
    
    void computeTHEBDs();
    
    void setJ( const UInt & _J );
    
  protected:
    
    Real M_J;
    
  private:
   
  };
 
  class BDRefDirectComputation : public BandDepthRef< 2 >
  {
  public:
    
    typedef GetPot data_Type;
    
    typedef BandDepthRefData bdRefData_Type;
        
    BDRefDirectComputation(){};
    
    BDRefDirectComputation( const bdRefData_Type & bdRefData ){};
    
    virtual ~BDRefDirectComputation(){};
    
    void computeTHEBDs(){};
    
    void setJ( const UInt & _J ){};
    
  protected:
    
    Real M_J;
    
  private:
    
  };
 */
}


#endif
