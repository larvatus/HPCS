#ifndef __MATRIX_DISTANCES_HPP__
#define __MATRIX_DISTANCES_HPP__

#include <source/HPCSDefs.hpp>

#include <Eigen/Dense>

#include <Eigen/Eigenvalues>

namespace HPCS
{
  
  class MatrixDistanceBase
  {
    public:
    
      typedef Eigen::Matrix< Real, Eigen::Dynamic, Eigen::Dynamic > matrix_Type;
      
      typedef boost::shared_ptr< matrix_Type > matrixPtr_Type;
    
      MatrixDistanceBase();
      
      MatrixDistanceBase( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
      
      MatrixDistanceBase( const matrix_Type & matrix1, const matrix_Type & matrix2 );
      
  protected:
    
      matrixPtr_Type M_matrixPtr1;
      
      matrixPtr_Type M_matrixPtr2;
      
  };
  
  class FrobeniusDistance : public MatrixDistanceBase
  {
  public:
    
    typedef MatrixDistanceBase::matrix_Type matrix_Type;
    
    typedef MatrixDistanceBase::matrixPtr_Type matrixPtr_Type;
    
    FrobeniusDistance();
    
    FrobeniusDistance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
    
    FrobeniusDistance( const matrix_Type & matrix1, const matrix_Type & matrix2 );
    
    Real compute();
    
    Real compute( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
    
    Real compute( const matrix_Type & matrix1, const matrix_Type & matrix2 );
    
  private:
    
    Real M_distance;    
    
  };
  
  class L2Distance : public MatrixDistanceBase
  {
  public:
    
    typedef MatrixDistanceBase::matrix_Type matrix_Type;
    
    typedef MatrixDistanceBase::matrixPtr_Type matrixPtr_Type;
    
    L2Distance();
    
    L2Distance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
    
    L2Distance( const matrix_Type & matrix1, const matrix_Type & matrix2 );
    
    Real compute();
    
    Real compute( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
    
    Real compute( const matrix_Type & matrix1, const matrix_Type & matrix2 );
    
  private:
    
    Real M_distance;
    
  };
  
  
  class SqrtDistance : public MatrixDistanceBase
  {
  
  public:
  
      typedef MatrixDistanceBase::matrix_Type matrix_Type;
      
      typedef MatrixDistanceBase::matrixPtr_Type matrixPtr_Type;
      
      SqrtDistance();
      
      SqrtDistance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
      
      SqrtDistance( const matrix_Type & matrix1, const matrix_Type & matrix2 );
      
      Real compute();
      
      Real compute( const matrix_Type & matrix1, const matrix_Type & matrix2 );
      
      Real compute( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
      
  private:
    
      matrixPtr_Type operatorSqrt( const matrixPtr_Type & matrixPtr ) const;
    
      Real M_distance;      
    
  };
  
  class SpectralDistance : public MatrixDistanceBase
  {
  public:

      typedef MatrixDistanceBase::matrix_Type matrix_Type;
      
      typedef MatrixDistanceBase::matrixPtr_Type matrixPtr_Type;
      
      SpectralDistance();
      
      SpectralDistance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
      
      SpectralDistance( const matrix_Type & matrix1, const matrix_Type & matrix2 );
      
      Real compute();
      
      Real compute( const matrix_Type & matrix1, const matrix_Type & matrix2 );
      
      Real compute( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );

   
  private:
    
      Real M_distance;      
  };
  
  
  class ProcrustesDistance : public MatrixDistanceBase
  {
  public:

      typedef MatrixDistanceBase::matrix_Type matrix_Type;
      
      typedef MatrixDistanceBase::matrixPtr_Type matrixPtr_Type;
      
      ProcrustesDistance();
      
      ProcrustesDistance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
      
      ProcrustesDistance( const matrix_Type & matrix1, const matrix_Type & matrix2 );
      
      Real compute();
      
      Real compute( const matrix_Type & matrix1, const matrix_Type & matrix2 );
      
      Real compute( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );

   
  private:
    
      matrixPtr_Type operatorSqrt( const matrixPtr_Type & matrixPtr ) const;
    
      Real M_distance;      
  };
  
  
}



#endif