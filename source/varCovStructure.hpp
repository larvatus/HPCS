
#ifndef __VARCOVSTRUCTURE_HPP__
#define __VARCOVSTRUCTURE_HPP__

#include <source/dataSet.hpp>

#include <source/HPCSDefs.hpp>
#include <Eigen/Eigenvalues>

namespace HPCS
{

class VarCovStructure
{
  
public:
  
    //! @name Public Types
    //@{
      
    //! Typedef for the data set type  
    typedef DataSet dataSet_Type;
    
    //! Typedef for the shared pointer to the data set type
    typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
    
    //! Typedef for the matrix type
    typedef DataSet::matrix_Type matrix_Type;
    
    //! Typedef for the shared pointer to a matrix object
    typedef DataSet::matrixPtr_Type matrixPtr_Type;

    //@}
 
    //! Constructor from a (polymorphic) data set pointer
    VarCovStructure( const dataSetPtr_Type & dataSetPtr );

    //! @name Public methods for statistical computations
    //@{

    
    //! Method to get the variance matrix (via shared pointer)
    /*!
     * @todo Implement a parallel version for distributed computation of variance, maybe exploiting a distributed storage of dataset
     */
     void varCovMatrix( matrixPtr_Type & matrixPtr );

    //! Method to get the correlation matrix (via shared pointer)
    /*!
     * @todo Implement a parallel version for distributed computation of correlation, maybe exploiting a distributed storage of dataset
      */
     void corMatrix( matrixPtr_Type & matrixPtr );
    
    
    //! Method to perform the spectral decomposition of the var-cov or correlation matrix
    /*!
     * This method instantiates a proper SelfAdjointEigenSolver from the Eigen libraries from
     * either the var-cov matrix or the correlation matrix, depending on the value of the flag varCov.
     * 
     * @param varCov is the parameter specifying wegther the variance-covariance or the correlation matrix has to be used
     * 		     for the spectral decomposition.
     */
     void performSpectralDecomposition( const bool & varCov = true );
    
    
    //! Method to get the eigenvectors computed during the spectral decomposition
     void eigenVectors( matrixPtr_Type & matrixPtr ) const;
    
    //! Method to get the eigenvalues computed during the spectral decomposition
    template < typename _containerType > 
       void eigenValues( boost::shared_ptr< _containerType > & contPtr ) const;
      
    template < typename _containerType >
       void eigenValues( _containerType & container ) const;
      
    template < typename _iteratorType >
       void eigenValues( const _iteratorType & begin, const _iteratorType & end ) const;

    //@}
	
    
 protected:
   
     //! @name Protected types definition
     //@{
       
        typedef Eigen::SelfAdjointEigenSolver< matrix_Type > eigenSolver_Type;
	
	typedef boost::shared_ptr< eigenSolver_Type > eigenSolverPtr_Type;
       
     //@}
  
      //! @name Protected members

      //@{

	//! Shared pointer to the variance-covariance matrix
	matrixPtr_Type M_varCovMatrixPtr;
	
	//! Shared pointer to the correlation matrix
	matrixPtr_Type M_corMatrixPtr;
	
	//! Shared pointer to the eigensolver object
 	eigenSolverPtr_Type M_eigenSolverPtr;
	
      //@}
	
  
private:
     
  //! @name Private members
  //@{

    //! The shared pointer to the data set object
    dataSetPtr_Type M_dataSetPtr;
    
    //! Flag indicating wether the variance matrix has been already computed or not.
    bool M_varCovMatrixComputedFlag;
    
    //! Flag indicating wether the correlation matrix has been already computed or not.
    bool M_corMatrixComputedFlag;
    
  //@}
    
  //! @name Private methods
  
  //@{
    
    //! Method to compute the variance-covariance matrix.
    /*!
     * @todo Implement a parallel version for distributed computation of variance, maybe exploiting a distributed storage of dataset
     */
     void computeVarCovMatrix();
	
    //! Method to compute the correlation matrix.
    /*!
     * @todo Implement a parallel version for distributed computation of correlation, maybe exploiting a distributed storage of dataset
     */
     void computeCorMatrix();
  
  //@}
    
};


   template < typename _containerType >
     void
     VarCovStructure::
     eigenValues( boost::shared_ptr< _containerType > & contPtr ) 
     const
     {
       
 	contPtr->clear();
       
 	contPtr->reserve( this->M_varCovMatrixPtr->rows() );
 	
 	for ( UInt iValue(0); iValue < this->M_varCovMatrixPtr->rows(); ++iValue )
 	{
 	    contPtr->push_back( this->M_eigenSolverPtr->eigenvalues()( iValue ) );
 	}
 	
 	return;
     }

  template < typename _containerType >
    void
    VarCovStructure::
    eigenValues( _containerType & container )
    const
    {
      container.clear();
      
      container.reserve( this->M_varCovMatrixPtr->rows() );
      
      for ( UInt iValue(0); iValue < this->M_varCovMatrixPtr->rows(); ++iValue )
      {
	  container.push_back( this->M_eigenSolverPtr->eigenvalues()( iValue )  );
      }
      
      return;
    }
    
   template < typename _iteratorType >
     void
     VarCovStructure::
     eigenValues( const _iteratorType & begin, const _iteratorType & end )
     const
     {
 	_iteratorType it( begin );
 	
 	for ( UInt iValue(0); iValue < this->M_varCovMatrixPtr->rows(); ++iValue )
 	{
 	    (*it) = ( this->M_eigenSolverPtr->eigenvalues() )( iValue );
 	    
 	    ++it;
 	}
 	
 	return;
     }

}

#endif /* __VARCOVSTRUCTURE_HPP__ */