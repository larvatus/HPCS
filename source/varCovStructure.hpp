
#ifndef __VARCOVSTRUCTURE_HPP__
#define __VARCOVSTRUCTURE_HPP__

#include <source/dataSet.hpp>

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
    
    //@}
	
    
protected:
   
      //! @name Protected members

      //@{

	//! Variance-Covariance matrix
	matrixPtr_Type M_varCovMatrixPtr;
	
	//! Correlation matrix
	matrixPtr_Type M_corMatrixPtr;
	
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

}

#endif