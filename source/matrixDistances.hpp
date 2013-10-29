#ifndef __MATRIX_DISTANCES_HPP__
#define __MATRIX_DISTANCES_HPP__

#include <source/HPCSDefs.hpp>

#pragma GCC diagnostic ignored "-Wenum-compare"
#include <Eigen/Dense>

#include <Eigen/Eigenvalues>
#pragma GCC diagnostic warning "-Wenum-compare"

#include <source/factory.hpp>

namespace HPCS
{
  
  class MatrixDistanceBase
  {
    public:
    
      typedef Eigen::Matrix< Real, Eigen::Dynamic, Eigen::Dynamic > matrix_Type;
      
      typedef boost::shared_ptr< matrix_Type > matrixPtr_Type;
    
      MatrixDistanceBase();
      
      virtual ~MatrixDistanceBase(){};
      
      MatrixDistanceBase( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
      
      MatrixDistanceBase( const matrix_Type & matrix1, const matrix_Type & matrix2 );
      
      virtual Real compute(){};
    
      Real compute( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
    
      Real compute( const matrix_Type & matrix1, const matrix_Type & matrix2 );
      
  protected:
    
      matrixPtr_Type M_matrixPtr1;
      
      matrixPtr_Type M_matrixPtr2;
      
      Real M_distance;
      
  };
  
  class FrobeniusDistance : public MatrixDistanceBase
  {
  public:
    
    typedef MatrixDistanceBase::matrix_Type matrix_Type;
    
    typedef MatrixDistanceBase::matrixPtr_Type matrixPtr_Type;
    
    FrobeniusDistance();
    
    virtual ~FrobeniusDistance(){};
    
    FrobeniusDistance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
    
    FrobeniusDistance( const matrix_Type & matrix1, const matrix_Type & matrix2 );
    
    Real compute();
            
  };
  
  class L2Distance : public MatrixDistanceBase
  {
  public:
    
    typedef MatrixDistanceBase::matrix_Type matrix_Type;
    
    typedef MatrixDistanceBase::matrixPtr_Type matrixPtr_Type;
    
    L2Distance();
    
    virtual ~L2Distance(){};
    
    L2Distance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
    
    L2Distance( const matrix_Type & matrix1, const matrix_Type & matrix2 );
    
    Real compute();

  };
  
  
  class SqrtDistance : public MatrixDistanceBase
  {
  
  public:
  
      typedef MatrixDistanceBase::matrix_Type matrix_Type;
      
      typedef MatrixDistanceBase::matrixPtr_Type matrixPtr_Type;
      
      SqrtDistance();
      
      virtual ~SqrtDistance(){};
      
      SqrtDistance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
      
      SqrtDistance( const matrix_Type & matrix1, const matrix_Type & matrix2 );
      
      Real compute();
          
  private:
    
      matrixPtr_Type operatorSqrt( const matrixPtr_Type & matrixPtr ) const;    
    
  };
  
  class SpectralDistance : public MatrixDistanceBase
  {
  public:

      typedef MatrixDistanceBase::matrix_Type matrix_Type;
      
      typedef MatrixDistanceBase::matrixPtr_Type matrixPtr_Type;
      
      SpectralDistance();
      
      virtual ~SpectralDistance(){};
      
      SpectralDistance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
      
      SpectralDistance( const matrix_Type & matrix1, const matrix_Type & matrix2 );
      
      Real compute();
    
  };
  
  
  class ProcrustesDistance : public MatrixDistanceBase
  {
  public:

      typedef MatrixDistanceBase::matrix_Type matrix_Type;
      
      typedef MatrixDistanceBase::matrixPtr_Type matrixPtr_Type;
      
      ProcrustesDistance();
      
      virtual ~ProcrustesDistance(){};
      
      ProcrustesDistance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 );
      
      ProcrustesDistance( const matrix_Type & matrix1, const matrix_Type & matrix2 );
      
      Real compute();
      
  private:
    
      matrixPtr_Type operatorSqrt( const matrixPtr_Type & matrixPtr ) const;
    
  };
  /*
  template < typename T >
  struct MatrixDistanceWrapper
  {
  public:
    
      MatrixDistanceWrapper(){};
      
      MatrixDistanceBase * build() const;
      
      virtual ~MatrixDistanceWrapper(){};
    
  };
  
  template < typename T >
  MatrixDistanceBase *
  MatrixDistanceWrapper< T >::
  build()
  const
  {
      return new T;
  }
  
  
  class MatrixDistanceFactory
  :
  public Factory< MatrixDistanceBase, std::string, CreationRule< : > >
  {
  public:
    
    MatrixDistanceFactory();
    
    virtual ~MatrixDistanceFactory(){};
    
  };
  */
  
  
}



#endif