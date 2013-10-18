
#include <source/matrixDistances.hpp>

namespace HPCS
{
  
  /////////////////////////////////
  ////	MATRIX DISTANCE BASE  ////
  ////////////////////////////////
  
  MatrixDistanceBase::
  MatrixDistanceBase()
  :
  M_matrixPtr1( new matrix_Type() ),
  M_matrixPtr2( new matrix_Type() )
  {}
  
  MatrixDistanceBase::
  MatrixDistanceBase( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
  :
  M_matrixPtr1( matrixPtr1 ),
  M_matrixPtr2( matrixPtr2 )
  {}
  
  MatrixDistanceBase::
  MatrixDistanceBase( const matrix_Type & matrix1, const matrix_Type & matrix2 )
  :
  M_matrixPtr1( new matrix_Type( matrix1 ) ),
  M_matrixPtr2( new matrix_Type( matrix2 ) )
  {}
  
  /////////////////////////////////
  ////	 FROBENIUS  DISTANCE   ////
  ////////////////////////////////
  
  FrobeniusDistance::
  FrobeniusDistance()
  :
  MatrixDistanceBase(),
  M_distance( -1 )
  {}
  
  FrobeniusDistance::
  FrobeniusDistance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
  :
  MatrixDistanceBase( matrixPtr1, matrixPtr2 ),
  M_distance( -1 )
  {}
  
  FrobeniusDistance::
  FrobeniusDistance( const matrix_Type & matrix1, const matrix_Type & matrix2 )
  :
  MatrixDistanceBase( matrix1, matrix2 ),
  M_distance( -1 )
  {}
  
  Real
  FrobeniusDistance::
  compute()
  {    
    this->M_distance = ( *this->M_matrixPtr1 - *this->M_matrixPtr2 ).norm();
    
    return this->M_distance;
  }
  
  Real
  FrobeniusDistance::
  compute( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
  {
      this->M_matrixPtr1 = matrixPtr1;
      
      this->M_matrixPtr2 = matrixPtr2;
    
      this->M_distance = ( *matrixPtr1 - *matrixPtr2 ).norm();
      
      return this->M_distance;
  }
  

  Real
  FrobeniusDistance::
  compute( const matrix_Type & matrix1, const matrix_Type & matrix2 )
  {
      this->M_matrixPtr1.reset( new matrix_Type( matrix1 ) );
      
      this->M_matrixPtr2.reset( new matrix_Type( matrix2 ) );
    
      this->M_distance = ( *this->M_matrixPtr1 - *this->M_matrixPtr2 ).norm();
      
      return this->M_distance;
  }

  
}