
#include <source/permutationTest.hpp>


namespace HPCS
{   /////////////////////////
    //// PermutationTest ////
    /////////////////////////
    
//     PermutationTest::
//     PermutationTest()
//     :
//     M_niter(1000),
//     M_distPtr( new matrixDist_Type() )
//     {}
    
//      PermutationTest::
//      PermutationTest( const matrixPtr_Type & dataPtr1, const matrixPtr_Type & dataPtr2, 
// 		      const matrixDistPtr_Type & matrixDistPtr, const UInt & NIter /* = 1000 */ )
//      :
//      M_dataPtr1( dataPtr1 ),
//      M_dataPtr2( dataPtr2 ),
//      M_distPtr( matrixDistPtr ),
//      M_niter( NIter )
//      {}
    
//      PermutationTest::
//      PermutationTest( const matrixPtr_Type & dataPtr1, const matrixPtr_Type & dataPtr2,
// 		      const UInt & NIter /* = 1000 */ )
//      :
//      M_dataPtr1( dataPtr1 ),
//      M_dataPtr2( dataPtr2 ),
//      M_distPtr( new matrixDist_Type() ),
//      M_niter( NIter )
//      {}    
    
     PermutationTest::
     PermutationTest( const dataSetPtr_Type & dataSetPtr1, const dataSetPtr_Type & dataSetPtr2, 
 		     const matrixDistPtr_Type & matrixDistPtr, const UInt & NIter /* = 1000 */ )
     :
     M_dataSetPtr1( dataSetPtr1 ),
     M_dataSetPtr2( dataSetPtr2 ),
     M_distPtr( matrixDistPtr ),
     M_niter( NIter )
     {       
	assert( dataSetPtr1->nbPts() == dataSetPtr2->nbPts() );
     }
 
     PermutationTest::
     PermutationTest( const dataSetPtr_Type & dataSetPtr1, const dataSetPtr_Type & dataSetPtr2, 
 		     const UInt & NIter /* = 1000 */ )
     :
     M_dataSetPtr1( dataSetPtr1 ),
     M_dataSetPtr2( dataSetPtr2 ),
     M_distPtr( new matrixDist_Type() ),
     M_niter( NIter )
     {       
	assert( dataSetPtr1->nbPts() == dataSetPtr2->nbPts() );
     }
    
//     void
//     PermutationTest::
//     setData( const matrixPtr_Type & matrixPtr1, const matrixPtr_Type & matrixPtr2 )
//     {
// 
//       this->M_dataPtr1 = matrixPtr1;
//       
//       this->M_dataPtr2 = matrixPtr2;
// 	
//       return;
//     }
    
    void
    PermutationTest::
    setData( const dataSetPtr_Type & dataSetPtr1, const dataSetPtr_Type & dataSetPtr2 )
    {       
      assert( dataSetPtr1->nbPts() == dataSetPtr2->nbPts() );

      this->M_dataSetPtr1 = dataSetPtr1;
      
      this->M_dataSetPtr2 = dataSetPtr1;
	
      return;
    }

    void
    PermutationTest::
    setDistance( const matrixDistPtr_Type & matrixDistPtr )
    {
	this->M_distPtr = matrixDistPtr;
	
	return;
    }
    
    void
    PermutationTest::
    setNIter( const UInt & niter )
    {
	this->M_niter = niter;
      
	return;
    }
        
    void
    PermutationTest::
    apply()
    {
	this->M_pValue = 0;
	
	this->M_dataSetPtr1->subtractMeanValue();
	
	this->M_dataSetPtr2->subtractMeanValue();
	
	varCov_Type varCov( this->M_dataSetPtr1->getData() );
	
	matrixPtr_Type varMatrixPtr1( varCov.varCovMatrix() );

	varCov.setData( this->M_dataSetPtr2->getData() );
	
	matrixPtr_Type varMatrixPtr2( varCov.varCovMatrix() );
	
	const Real dist0 = this->M_distPtr->compute( varMatrixPtr1, varMatrixPtr2 );
	
	const UInt N1 = this->M_dataSetPtr1->nbSamples();
	const UInt N2 = this->M_dataSetPtr2->nbSamples()	;
	
	std::vector< UInt > drawnSampleIDs;	
	std::vector< UInt > allSampleIDs( N1 + N2 );
	
	for ( UInt iN(0); iN < N1 + N2; ++iN )
	{
	  allSampleIDs[ iN ] = iN;
	}
	
	srand48( time( NULL ) );
	
  	for ( UInt it(0); it < this->M_niter; ++it )
  	{
	  std::cout << " Iteration #" << it << std::endl;	
	 
	  drawnSampleIDs.clear();
	  
	  drawnSampleIDs.reserve( N1 );
	  
	  while( drawnSampleIDs.size() != N1 )
 	  {	    
 	    const UInt temp( static_cast< UInt >( ( N1 + N2 - 1) * drand48() ) );
 		    
 	    if ( std::find( drawnSampleIDs.begin(), drawnSampleIDs.end(), temp ) == drawnSampleIDs.end() )
 	    {
 		drawnSampleIDs.push_back( temp );
		
 	    } 	    
 	  }
 	  
 	  std::sort( drawnSampleIDs.begin(), drawnSampleIDs.end() );
 	  
	  std::vector< UInt > remainingSampleIDs( N2 );
	  
	  std::set_difference( allSampleIDs.begin(), allSampleIDs.end(), 
			       drawnSampleIDs.begin(), drawnSampleIDs.end(), 
			       remainingSampleIDs.begin() );
	  
	  dataSetPtr_Type dataUnionPtr( new dataSet_Type( M_dataSetPtr1->getData() ) );
	  
	  dataUnionPtr->addSamples( this->M_dataSetPtr2->getData() );	  
/*	  
 	  std::cout << " SAMPLES " << std::endl;
	  
	  std::cout << " ** pop 1 " << std::endl;
 	  for ( std::vector< UInt >::iterator it( drawnSampleIDs.begin() ); it != drawnSampleIDs.end(); ++it )
 	  {
 	      std::cout << *it << " ";
 	  }
 	  
 	  std::cout << std::endl;
 	  std::cout << " ** pop 2 " << std::endl;
 	  
 	  for ( std::vector< UInt >::iterator it( remainingSampleIDs.begin() ); it != remainingSampleIDs.end(); ++it )
 	  {
 	      std::cout << *it << " ";
 	  }
 	  
 	  std::cout << std::endl;
	  
	  dataUnionPtr->showMe();
*/
  	  matrixPtr_Type G1 = dataUnionPtr->getRowSubSet( drawnSampleIDs );
 
  	  matrixPtr_Type G2 = dataUnionPtr->getRowSubSet( remainingSampleIDs );
	  
//    std::cout << " Matrix 1 " << std::endl << *G1 << std::endl << std::endl;
	  
//    std::cout << " Matrix 2 " << std::endl << *G2 << std::endl << std::endl;
	  
	  matrixPtr_Type varG1, varG2;
	  
	  varCov.setData( G1 ); G1.reset();
	  
	  varG1 = varCov.varCovMatrix();
	  
	  varCov.setData( G2 );	  G2.reset();
	  
	  varG2 = varCov.varCovMatrix();
	  
//    std::cout << " Var-Cov 1 " << std::endl << *varG1 << std::endl << std::endl;
	  
//    std::cout << " Var-Cov 2 " << std::endl << *varG2 << std::endl << std::endl;
	  
	  const Real distCurr = this->M_distPtr->compute( varG1, varG2 );
	  
	  std::cout << "\t DistCurr = " << distCurr << "\t Dist0 = " << dist0 << std::endl;
	  
	  if( distCurr >= dist0 ) 
	  {  
	    this->M_pValue += 1./this->M_niter;
	  }	  
  	}
	
	return;
    }
  
}