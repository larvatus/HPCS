
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
  M_matrixPtr2( new matrix_Type() ),
  M_distance(-1)
  {}
  
  MatrixDistanceBase::
  MatrixDistanceBase( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
  :
  //! @TODO RIFLETTICI SOPRA
  M_matrixPtr1( new matrix_Type( *matrixPtr1 ) ),
  //! @TODO RIFLETTICI SOPRA
  M_matrixPtr2( new matrix_Type( *matrixPtr2 ) ),
  M_distance(-1)
  {}
  
  MatrixDistanceBase::
  MatrixDistanceBase( const matrix_Type & matrix1, const matrix_Type & matrix2 )
  :
  M_matrixPtr1( new matrix_Type( matrix1 ) ),
  M_matrixPtr2( new matrix_Type( matrix2 ) ),
  M_distance(-1)
  {}
  
  Real
  MatrixDistanceBase::
  compute( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
  {
      //! TODO Riflettici sopra
      this->M_matrixPtr1.reset( new matrix_Type( *matrixPtr1 ) );
 
      //! TODO Riflettici sopra       
      this->M_matrixPtr2.reset( new matrix_Type( *matrixPtr2 ) );
    
      this->M_distance = ( *matrixPtr1 - *matrixPtr2 ).norm();
   
      return this->M_distance;
  }
  
 
  Real
  MatrixDistanceBase::
  compute( const matrix_Type & matrix1, const matrix_Type & matrix2 )
  {
      //! TODO Riflettici sopra
      this->M_matrixPtr1.reset( new matrix_Type( matrix1 ) );

      //! TODO Riflettici sopra
      this->M_matrixPtr2.reset( new matrix_Type( matrix2 ) );
    
      this->M_distance = ( *this->M_matrixPtr1 - *this->M_matrixPtr2 ).norm();
      
      return this->M_distance;
  }

  
  
  /////////////////////////////////
  ////	 FROBENIUS  DISTANCE   ////
  /////////////////////////////////
  
  FrobeniusDistance::
  FrobeniusDistance()
  :
  MatrixDistanceBase()
  {}
  
  FrobeniusDistance::
  FrobeniusDistance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
  :
  MatrixDistanceBase( matrixPtr1, matrixPtr2 )
  {}
  
  FrobeniusDistance::
  FrobeniusDistance( const matrix_Type & matrix1, const matrix_Type & matrix2 )
  :
  MatrixDistanceBase( matrix1, matrix2 )
  {}
  
  Real
  FrobeniusDistance::
  compute()
  {    
    this->M_distance = ( *this->M_matrixPtr1 - *this->M_matrixPtr2 ).norm();
    
    return this->M_distance;
  }
  
//   Real
//   FrobeniusDistance::
//   compute( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
//   {
//	 //! TODO Riflettici sopra
//       this->M_matrixPtr1.reset( new matrix_Type( *matrixPtr1 ) );
// 
//	 //! TODO Riflettici sopra       
//       this->M_matrixPtr2.reset( new matrix_Type( *matrixPtr2 ) );
//     
//       this->M_distance = ( *matrixPtr1 - *matrixPtr2 ).norm();
//             
//       return this->M_distance;
//   }
//   
// 
//   Real
//   FrobeniusDistance::
//   compute( const matrix_Type & matrix1, const matrix_Type & matrix2 )
//   {
//       //! TODO Riflettici sopra
//       this->M_matrixPtr1.reset( new matrix_Type( matrix1 ) );
// 
//       //! TODO Riflettici sopra
//       this->M_matrixPtr2.reset( new matrix_Type( matrix2 ) );
//     
//       this->M_distance = ( *this->M_matrixPtr1 - *this->M_matrixPtr2 ).norm();
//       
//       return this->M_distance;
//   }

  /////////////////////////////////
  ////	    L2	    DISTANCE   ////
  /////////////////////////////////
  
  L2Distance::
  L2Distance()
  :
  MatrixDistanceBase()//,
//   M_distance( -1 )
  {}
  
  L2Distance::
  L2Distance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
  :
  MatrixDistanceBase( matrixPtr1, matrixPtr2 )//,
//   M_distance( -1 )
  {}
  
  L2Distance::
  L2Distance( const matrix_Type & matrix1, const matrix_Type & matrix2 )
  :
  MatrixDistanceBase( matrix1, matrix2 )//,
//   M_distance( -1 )
  {}
  
//   Real
//   L2Distance::
//   compute( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
//   {
//       this->M_matrixPtr1 = matrixPtr1;
//       
//       this->M_matrixPtr2 = matrixPtr2;
//     
//       return this->compute();
//   }
//   
// 
//   Real
//   L2Distance::
//   compute( const matrix_Type & matrix1, const matrix_Type & matrix2 )
//   {
//      
//       this->M_matrixPtr1.reset( new matrix_Type( matrix1 ) );
// 
//      
//       this->M_matrixPtr2.reset( new matrix_Type( matrix2 ) );
// 
//     
//       return this->compute();
//     
//   }
//   
  Real
  L2Distance::
  compute()
  {    
    const UInt nbRows( this->M_matrixPtr1->rows() );
    
    const UInt nbCols( this->M_matrixPtr1->cols() );
    
    assert( nbRows == this->M_matrixPtr2->rows() && nbCols == this->M_matrixPtr2->cols() );
    
    this->M_distance = 0;
    
    for ( UInt iRow(1); iRow < nbRows; ++iRow )
    {
	Real rowSum(0); 

	for ( UInt iCol(1); iCol < nbCols; ++iCol )
	{
	  	rowSum += (*this->M_matrixPtr1)( iRow, iCol ) - (*this->M_matrixPtr2)( iRow, iCol ); 
	}
	
	this->M_distance += rowSum * rowSum;
    }
    
    this->M_distance = std::sqrt( this->M_distance ); 
    
    return this->M_distance;
  }
  
  /////////////////////////////////
  ////	   SQRT     DISTANCE   ////
  /////////////////////////////////
  
  SqrtDistance::
  SqrtDistance()
  :
  MatrixDistanceBase()//,
//   M_distance( -1 )
  {}
  
  SqrtDistance::
  SqrtDistance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
  :
  MatrixDistanceBase( matrixPtr1, matrixPtr2 )//,
//   M_distance( -1 )
  {}
  
  SqrtDistance::
  SqrtDistance( const matrix_Type & matrix1, const matrix_Type & matrix2 )
  :
  MatrixDistanceBase( matrix1, matrix2 )//,
//   M_distance( -1 )
  {}
  
  Real
  SqrtDistance::
  compute()
  {    
    
    matrixPtr_Type sqrtMatrixPtr1( this->operatorSqrt( this->M_matrixPtr1 ) );
    matrixPtr_Type sqrtMatrixPtr2( this->operatorSqrt( this->M_matrixPtr2 ) );
    
    FrobeniusDistance frobeniusDistance( sqrtMatrixPtr1, sqrtMatrixPtr2 );
    
    this->M_distance = frobeniusDistance.compute();
    
    return this->M_distance;
  }
  
  SqrtDistance::matrixPtr_Type
  SqrtDistance::
  operatorSqrt( const matrixPtr_Type & matrixPtr ) const
  {
      typedef Eigen::SelfAdjointEigenSolver< matrix_Type > eigenSolver_Type;
           
      eigenSolver_Type eigenSolver;
            
      eigenSolver.compute( *matrixPtr );
            
      matrix_Type diag( eigenSolver.eigenvalues().array().abs().sqrt().matrix().asDiagonal() );
      
      matrixPtr_Type matrixSqrtPtr( new matrix_Type( eigenSolver.eigenvectors() * diag * eigenSolver.eigenvectors().transpose() ) );
      
      return matrixSqrtPtr;
      
  }
  
  
  /////////////////////////////////
  //// SPECTRAL     DISTANCE   ////
  /////////////////////////////////
  
  SpectralDistance::
  SpectralDistance()
  :
  MatrixDistanceBase()//,
//   M_distance( -1 )
  {}
  
  SpectralDistance::
  SpectralDistance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
  :
  MatrixDistanceBase( matrixPtr1, matrixPtr2 )//,
//   M_distance( -1 )
  {}
  
  SpectralDistance::
  SpectralDistance( const matrix_Type & matrix1, const matrix_Type & matrix2 )
  :
  MatrixDistanceBase( matrix1, matrix2 )//,
//   M_distance( -1 )
  {}
  
//   Real
//   SpectralDistance::
//   compute( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
//   {
//       this->M_matrixPtr1 = matrixPtr1;
//       
//       this->M_matrixPtr2 = matrixPtr2;
//     
//       return this->compute();
//   }
//   
// 
//   Real
//   SpectralDistance::
//   compute( const matrix_Type & matrix1, const matrix_Type & matrix2 )
//   {
//       this->M_matrixPtr1.reset( new matrix_Type( matrix1 ) );
//       
//       this->M_matrixPtr2.reset( new matrix_Type( matrix2 ) );
//     
//       return this->compute();
//     
//   }
  
  Real
  SpectralDistance::
  compute()
  {    
    typedef Eigen::SelfAdjointEigenSolver< matrix_Type > eigenSolver_Type;
    
    eigenSolver_Type eigenSolver;
    
    eigenSolver.compute( *this->M_matrixPtr1 - *this->M_matrixPtr2 );
    
    this->M_distance = eigenSolver.eigenvalues().array().abs().maxCoeff();
       
    return this->M_distance;
  }
  
  
  /////////////////////////////////
  //// PROCRUSTES   DISTANCE   ////
  /////////////////////////////////
  
  ProcrustesDistance::
  ProcrustesDistance()
  :
  MatrixDistanceBase()//,
//   M_distance( -1 )
  {}
  
  ProcrustesDistance::
  ProcrustesDistance( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
  :
  MatrixDistanceBase( matrixPtr1, matrixPtr2 )//,
//   M_distance( -1 )
  {}
  
  ProcrustesDistance::
  ProcrustesDistance( const matrix_Type & matrix1, const matrix_Type & matrix2 )
  :
  MatrixDistanceBase( matrix1, matrix2 )//,
//   M_distance( -1 )
  {}
  
//   Real
//   ProcrustesDistance::
//   compute( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
//   {
//       this->M_matrixPtr1 = matrixPtr1;
//       
//       this->M_matrixPtr2 = matrixPtr2;
//     
//       return this->compute();
//   }
//   
// 
//   Real
//   ProcrustesDistance::
//   compute( const matrix_Type & matrix1, const matrix_Type & matrix2 )
//   {
//       this->M_matrixPtr1.reset( new matrix_Type( matrix1 ) );
//       
//       this->M_matrixPtr2.reset( new matrix_Type( matrix2 ) );
//     
//       return this->compute();
//     
//   }
  
  Real
  ProcrustesDistance::
  compute()
  {    
    typedef Eigen::JacobiSVD< matrix_Type > jacobiSVD_Type;;
    
    matrixPtr_Type L1Ptr = this->operatorSqrt( this->M_matrixPtr1 );
    matrixPtr_Type L2Ptr = this->operatorSqrt( this->M_matrixPtr2 );
    
    matrix_Type C( L2Ptr->transpose() * (*L1Ptr) );  
    
    jacobiSVD_Type svd(C, Eigen::ComputeFullU | Eigen::ComputeFullV );
    
    C = svd.matrixU() * svd.matrixV().transpose();
    
    this->M_distance = ( *L1Ptr - (*L2Ptr) * C ).norm();
	    
    return this->M_distance;
  }
  
  ProcrustesDistance::matrixPtr_Type
  ProcrustesDistance::
  operatorSqrt( const matrixPtr_Type & matrixPtr ) const
  {
      typedef Eigen::SelfAdjointEigenSolver< matrix_Type > eigenSolver_Type;
           
      eigenSolver_Type eigenSolver;
            
      eigenSolver.compute( *matrixPtr );
            
      matrix_Type diag( eigenSolver.eigenvalues().array().abs().sqrt().matrix().asDiagonal() );
      
      matrixPtr_Type matrixSqrtPtr( new matrix_Type( eigenSolver.eigenvectors() * diag * eigenSolver.eigenvectors().transpose() ) );
      
      return matrixSqrtPtr;
      
  }
  
}