
#include <source/varCovStructure.hpp>

namespace HPCS
{
  
  //////////////////////////////
  ////	 VAR COV STRUCTURE  ////
  //////////////////////////////
  
  VarCovStructure::
  VarCovStructure()
  :
  M_dataPtr( new matrix_Type() ),
  M_varCovMatrixComputedFlag( false ),
  M_corMatrixComputedFlag( false ),
  M_varCovMatrixPtr( new matrix_Type() ),
  M_corMatrixPtr( new matrix_Type() ),
  M_eigenSolverPtr( new eigenSolver_Type() )
  {}
  
  VarCovStructure::
  VarCovStructure( const matrixPtr_Type & data )
  :
  M_dataPtr( new matrix_Type( *data ) ),
  M_varCovMatrixComputedFlag( false ),
  M_corMatrixComputedFlag( false ),
  M_varCovMatrixPtr( new matrix_Type() ),
  M_corMatrixPtr( new matrix_Type() ),
  M_eigenSolverPtr( new eigenSolver_Type() )
  {}
  
  
  // Method to compute the variance matrix
  void 
  VarCovStructure::
  computeVarCovMatrix()
  {
    const UInt nbSamples( this->M_dataPtr->rows() );
    
    const UInt nbPts( this->M_dataPtr->cols() );
    
    
    if ( this->M_varCovMatrixComputedFlag == true )
    {
	return;
    }  
    
    this->M_varCovMatrixPtr->resize( nbPts, nbPts );  
    
    Real iPtAve, jPtAve;
    
    for ( UInt iPt(0); iPt < nbPts; ++iPt )
    {
      iPtAve = 0;
      
      for ( UInt iSample(0); iSample < nbSamples; ++iSample )
      {
 	iPtAve += (*this->M_dataPtr)( iSample, iPt );	   
      }
      
      iPtAve /= nbSamples;
      
      for ( UInt jPt(iPt); jPt < nbPts; ++jPt )
      {
	jPtAve = 0;
	
	(*this->M_varCovMatrixPtr)( iPt, jPt ) = 0;
	
	for ( UInt iSample(0); iSample < nbSamples; ++iSample )
	{
	   jPtAve += (*this->M_dataPtr)( iSample, jPt ); 
	}      
	
	jPtAve /= nbSamples;
	
	for ( UInt iSample(0); iSample < nbSamples; ++iSample )
	{
	  (*this->M_varCovMatrixPtr)( iPt, jPt ) += ( iPtAve - ( *this->M_dataPtr )( iSample, iPt ) ) 
						  * 
						( jPtAve - ( *this->M_dataPtr )( iSample, jPt )  ) ;
	}
	  
	(*this->M_varCovMatrixPtr)( iPt, jPt ) /= ( nbSamples - 1 ); 
	  
	(*this->M_varCovMatrixPtr)( jPt, iPt ) = (*this->M_varCovMatrixPtr)( iPt, jPt );
	
	}
    }  
    
    this->M_varCovMatrixComputedFlag = true;
    
    return;
  }

  // Method to compute the correlation matrix
  void 
  VarCovStructure::
  computeCorMatrix()
  {
    const UInt nbPts( this->M_dataPtr->cols() );

    
    if ( this->M_corMatrixComputedFlag == true )
    {
	return;
    }
    else if ( this->M_varCovMatrixComputedFlag == false )
    {
	this->computeVarCovMatrix();
    }
    
    this->M_corMatrixPtr.reset( new matrix_Type( *(this->M_varCovMatrixPtr) ) );  

    for ( UInt iPt(0); iPt < nbPts; ++iPt )
    {
      (*this->M_corMatrixPtr)( iPt, iPt ) = 1;
      
      for ( UInt jPt(iPt+1); jPt < nbPts; ++jPt )
      {	
	(*this->M_corMatrixPtr)( iPt, jPt ) /= std::sqrt( (*this->M_varCovMatrixPtr)( iPt, iPt )
							  *
							  (*this->M_varCovMatrixPtr)( jPt, jPt )
							);
	(*this->M_corMatrixPtr)( jPt, iPt ) = (*this->M_corMatrixPtr)( iPt, jPt ); 
	
	}
    }  

    this->M_corMatrixComputedFlag = true;
    
    return;
    
  }

  // Method to get the variance matrix
  void 
  VarCovStructure::
  varCovMatrix( matrixPtr_Type & matrixPtr )
  {    
    if ( this->M_varCovMatrixComputedFlag == false )
      
      this->computeVarCovMatrix();
    
    matrixPtr.reset( new matrix_Type( *this->M_varCovMatrixPtr) );
    
    
    
    return;
  }
  
  // Method to get the variance matrix
  VarCovStructure::matrixPtr_Type 
  VarCovStructure::
  varCovMatrix()
  {      
    
    if ( this->M_varCovMatrixComputedFlag == false )
      
      this->computeVarCovMatrix();
    
    matrixPtr_Type matrixPtr( new matrix_Type( *this->M_varCovMatrixPtr ) );
 
    return matrixPtr;
    
  }
  

  // Method to get the correlation matrix
  void 
  VarCovStructure::
  corMatrix( matrixPtr_Type & matrixPtr )
  {
      if ( this->M_corMatrixComputedFlag == false )
	
	this->computeCorMatrix();
    
      matrixPtr.reset( new matrix_Type( *this->M_corMatrixPtr ) );
      
      return;
  }
  
  // Method to get the correlation matrix
  VarCovStructure::matrixPtr_Type 
  VarCovStructure::
  corMatrix()
  {      
    
    if ( this->M_corMatrixComputedFlag == false )
      
      this->computeCorMatrix();
    
    matrixPtr_Type matrixPtr( new matrix_Type( *this->M_corMatrixPtr ) );
 
    return matrixPtr;
    
  }
	
  // Method to compute the spectral decomposition of the var-cov/correlation matrix
  void
  VarCovStructure::
  performSpectralDecomposition( const bool & varCov )
  {
    if ( varCov == true )
    {
      this->M_eigenSolverPtr.reset( new eigenSolver_Type( *this->M_varCovMatrixPtr ) );
    }
    else
    {
      this->M_eigenSolverPtr.reset( new eigenSolver_Type( *this->M_corMatrixPtr ) );
    }
    
    return;
  }
  
  // Method to get the shared pointer to the matrix of eigenvectors
  void
  VarCovStructure::
  eigenVectors( matrixPtr_Type & matrixPtr )
  const
  {
      matrixPtr.reset( new matrix_Type( this->M_eigenSolverPtr->eigenvectors() ) );
      
      return;
  }
  
  void
  VarCovStructure::
  setData( const matrixPtr_Type & matrixPtr )
  {
    this->M_dataPtr.reset( new matrix_Type( *matrixPtr ) );
    
    this->M_varCovMatrixComputedFlag = false;
    
    this->M_corMatrixComputedFlag = false;

    return;
  }
  
}