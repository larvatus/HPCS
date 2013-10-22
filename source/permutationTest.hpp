#ifndef __PERMUTATION_TEST_HPP__
#define __PERMUTATION_TEST_HPP__

#include <source/HPCSDefs.hpp>

#include <source/matrixDistances.hpp>
#include <source/varCovStructure.hpp>

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
    
//     PermutationTest();
    
//     PermutationTest( const matrixPtr_Type & dataPtr1, const matrixPtr_Type & dataPtr2, const UInt & NIter = 1000 );
  
//     PermutationTest( const matrixPtr_Type & dataPtr1, const matrixPtr_Type & dataPtr2, 
// 		     const matrixDistPtr_Type & distPtr, const UInt & NIter = 1000 );
    
    PermutationTest( const dataSetPtr_Type & dataSetPtr1, const dataSetPtr_Type & dataSetPtr2, const matrixDistPtr_Type & distPtr,
		     const UInt & NIter = 1000 );
    
    PermutationTest( const dataSetPtr_Type & dataSetPtr1, const dataSetPtr_Type & dataSetPtr2, const UInt & NIter = 1000 );
    
    void setDistance( const matrixDistPtr_Type & distPtr );

//     void setData( const matrixPtr_Type & dataPtr1, const matrixPtr_Type & dataPtr2 );
    
    void setData( const dataSetPtr_Type & dataPtr1, const dataSetPtr_Type & dataSetPtr2 );
    
    void setNIter( const UInt & niter );
    
    void apply();
    
    Real pValue() const { return this->M_pValue; }
    
    
  private:
    
    matrixDistPtr_Type M_distPtr;
    
//     varCovPtr_Type M_varCovPtr;
    
//      dataSetPtr_Type M_dataSetPtr;
    
    dataSetPtr_Type M_dataSetPtr1;
    
    dataSetPtr_Type M_dataSetPtr2;
    
//     matrixPtr_Type M_dataPtr1;
    
//     matrixPtr_Type M_dataPtr2;
    
    UInt M_niter;
    
    Real M_pValue;
    
    
  };
  
}


#endif