#ifndef __PERMUTATION_TEST_HPP__
#define __PERMUTATION_TEST_HPP__

#include <source/HPCSDefs.hpp>

#include <source/matrixDistances.hpp>
#include <source/varCovStructure.hpp>

#include <source/mpi_utility.hpp>

#include <source/dataSet.hpp>

namespace HPCS
{
    
  class PermutationTest
  {
  public:
    
    typedef MatrixDistanceBase matrixDist_Type;
    
    typedef boost::shared_ptr< matrixDist_Type > matrixDistPtr_Type;
    
    typedef MatrixDistanceBase::matrix_Type matrix_Type;
    
    typedef MatrixDistanceBase::matrixPtr_Type matrixPtr_Type;
    
    typedef VarCovStructure varCov_Type;
    
    typedef boost::shared_ptr< varCov_Type > varCovPtr_Type;
    
    typedef DataSet dataSet_Type;
    
    typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
    
    typedef mpiUtility mpiUtility_Type;
    
    typedef boost::shared_ptr< mpiUtility_Type > mpiUtilityPtr_Type;
       
    PermutationTest( const dataSetPtr_Type & dataSetPtr1, const dataSetPtr_Type & dataSetPtr2, const matrixDistPtr_Type & distPtr,
		     const UInt & NIter = 1000 );
    
    PermutationTest( const dataSetPtr_Type & dataSetPtr1, const dataSetPtr_Type & dataSetPtr2, const UInt & NIter = 1000 );
    
    void setDistance( const matrixDistPtr_Type & distPtr );
    
    void setData( const dataSetPtr_Type & dataPtr1, const dataSetPtr_Type & dataSetPtr2 );
    
    void setFirstSubDataSet( const UInt & nbSubSamples, const UInt & seed = 1 );
    
    void setSecondSubDataSet( const UInt & nbSubSamples, const UInt & seed = 1 ); 
    
    void setNIter( const UInt & niter );
    
    void apply();
    
    Real pValue() const { return this->M_pValue; }
    
    
  private:
    
    matrixDistPtr_Type M_distPtr;

    mpiUtilityPtr_Type M_mpiUtilityPtr;
        
    dataSetPtr_Type M_dataSetPtr1;
    
    dataSetPtr_Type M_dataSetPtr2;
        
    UInt M_niter;
    
    Real M_pValue;
    
  };
  
}


#endif