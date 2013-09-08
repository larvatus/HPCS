
#include <source/combinations.hpp>

namespace HPCS
{
  
  // Free function for the computation of binomial coefficients
  UInt
  binomial( const UInt & N , const UInt & K )
  {    
      UInt num( 1 );
      UInt denom( 1 );
      
      for ( UInt iK(0); iK < K; ++iK )
      {
	  num *= N - iK;
	  denom *= iK + 1;
      }
    
    return static_cast< UInt >( num/denom );
  }
  
  // Constructor from specified variables
  CombinationsID::
  CombinationsID( const UInt & N, const UInt & K, const bool & storeFlag  )
  :
  M_N( N ),
  M_K( K ),
  M_storedCombinations( 0 ),
  M_generatedCombinations( 0 ),
  M_storeFlag( storeFlag )
  {
    assert( N >= K );
  
    M_seed.resize( M_K );

    this->M_totalNumberOfCombinations = this->NChooseK( this->M_N, this->M_K );
  
    this->initialise();
  }


  // Default destructor
  CombinationsID::
  ~CombinationsID()
  {
  }
  
  // Definition of the method to reset the class
  void
  CombinationsID::
  reset( const UInt & N, const UInt & K, const bool & storeFlag)
  {
    assert( N >= K );
  
    this->M_N = N;
    this->M_K = K;
    this->M_storeFlag = storeFlag;
    
    this->M_storedCombinations = 0;
    this->M_generatedCombinations = 0;
    
    this->M_totalNumberOfCombinations = this->NChooseK( this->M_N, this->M_K );
    
    this->M_seed.resize( M_K );
  
    this->initialise();
    
    this->M_tupleList.clear();
    
    this->M_currTupleIt = this->M_tupleList.begin();
    
    return;
  }
  
  // Definition of the method to initialise the combination sequence
  void
  CombinationsID::
  initialise()
  {    
      this->M_seed.resize( this->M_K );
   
      // Initialisation. First seed must be of type [0,1,2,..K] out of [0,1,2,...,K,...N].
      for ( UInt iK(0); iK < this->M_K; ++iK )
      {
	  this->M_seed[ iK ] = iK;
      }
    
      this->M_tupleList.push_back( this->M_seed );
    
      this->M_storedCombinations++;
    
      return;
  }
  
  // Definition of the method to write the combinations to some output
  void
  CombinationsID::
  writeCombinationsID( std::ostream & output ) const
  {
      tupleListConstIterator_Type it;
    
      // Writing the saved combinations in a .dat format
      for ( it = this->M_tupleList.begin(); it != this->M_tupleList.end(); ++it )
      {
	  for ( UInt iK(0); iK < this->M_K - 1; ++iK )
	  {
		output << (*it)[ iK ] << " ";
	  }
	  
	  output << (*it)[ this->M_K - 1]<< std::endl; 
      }
 
      return;
  }

  // Definition of the method to ask if the iterator sweeping the list of computed combinations has traversed the whole list
  bool
  CombinationsID::
  hasTraversedAll() const
  {
      return ( this->M_currTupleIt == this->M_tupleList.end() );
  }

  // Definition of the method to ask if all the combinations have already been computed.
  bool
  CombinationsID::
  hasGeneratedAll() const
  {
      return ( this->M_generatedCombinations == this->M_totalNumberOfCombinations );
  }

  // Definition of the method to reset the iterator to the head of the combination list
  void
  CombinationsID::
  resetPointerToHeadCombination()
  {
      this->M_currTupleIt = this->M_tupleList.begin();
  }
  
  // Definition of the method to get the next combination (in terms of IDs)
  void
  CombinationsID::
  getNextCombinationID( tuple_Type & tuple  )
  {
    
      if ( this->M_storeFlag == true )
      {
	assert( not( this->hasTraversedAll() ) );
      
	tuple = *M_currTupleIt;
      
	this->M_currTupleIt++;
        
      }
      else
      {    
	assert( this->M_seed.size() == this->M_K );  
	
	assert( not( this->hasGeneratedAll() ) );
	
	if ( this->M_generatedCombinations == 0 )
	{
	    tuple = this->M_seed;
	    
	    this->M_generatedCombinations++;
	}
	else
	{
	  if ( M_seed[ 0 ] < M_N - M_K + 1 )
	  {
	    int iK = M_K - 1;
	    
	      M_seed[ iK ]++;
	      
	      for ( ; iK > 0; --iK )
	      {
		  if( M_seed[ iK ] >= M_N - ( M_K - iK ) )
		  {
		      M_seed[ iK - 1 ]++;
		      M_seed[ iK ] = M_seed[ iK - 1] + 1;
		      
		      for ( UInt iJ( iK + 1); iJ < M_K; ++iJ )
		      {		
			  M_seed[ iJ ] = M_seed[ iJ - 1 ] + 1;
		      }
		  }
		  else
		  {
		    break;
		  }
		
	      }
	      
	  this->M_generatedCombinations++;
	  
	  tuple = M_seed;
	  }
	}
	
      }
      
      return;
  }
  
  // Definition of the method to generate all the combinations of IDs
  void
  CombinationsID::
  generateCombinationsID()
  {
    assert( this->M_storeFlag == true );
    
    assert( this->M_storedCombinations == 1 );
    
    assert( this->M_generatedCombinations == 0 );
    
    assert( this->M_seed.size() == this->M_K );  
    
    while( M_seed[ 0 ] < M_N - M_K )
    {
      int iK = M_K - 1;
      
	M_seed[ iK ]++;
	
	for ( ; iK > 0; --iK )
	{
	    if( M_seed[ iK ] > M_N - ( M_K - iK ) )
	    {		
		M_seed[ iK - 1 ]++;
		M_seed[ iK ] = M_seed[ iK - 1] + 1;
		
		for ( UInt iJ( iK + 1); iJ < M_K; ++iJ )
		{		  
		    M_seed[ iJ ] = M_seed[ iJ - 1 ] + 1;
		}
	    }
	    else
	    {
	      break;
	    }
	  
	}
    
    // Pushing back into the tuple list
    M_tupleList.push_back( M_seed );
    
    this->M_storedCombinations++;
    
    }
        
    assert( this->M_storedCombinations == this->M_totalNumberOfCombinations );
    
    this->M_generatedCombinations = this->M_storedCombinations;
    
    this->M_currTupleIt = this->M_tupleList.begin();
    
    return;  
  }

  // Definition of the method to compute the factorial
  UInt
  CombinationsID::
  factorial( const UInt & K ) const
  {
      UInt factorial(1);
    
      for ( UInt iK(2); iK <= K; ++iK )
      {
	  factorial *= iK;
      }
      
      return factorial;
  }

  // Definition of the method to compute the binomial coefficient N choose k
  UInt
  CombinationsID::
  NChooseK( const UInt & N, const UInt & K ) const
  {
      UInt num( 1 );
      UInt denom( 1 );
      
      for ( UInt iK(0); iK < K; ++iK )
      {
	  num *= N - iK;
	  denom *= iK + 1;
      }
    
    return static_cast< UInt >( num/denom );
  }

}