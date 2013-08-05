
#include "combinationFactory.hpp"

namespace HPCS
{  
  
CombinationFactory::
CombinationFactory( const UInt & N, const UInt & K )
:
M_N(N),
M_K(K),
M_storedCombinations(0),
M_generatedCombinations(0)/*,
M_askedCombinations(0)*/
{
  assert( N >= K );
  
  M_seed.resize( M_K );

 this->M_totalNumberOfCombinations = this->NChooseK( this->M_N, this->M_K );
  
 this->initialise();
}

CombinationFactory::
~CombinationFactory()
{
}

void
CombinationFactory::
reset( const UInt & N, const UInt & K )
{
    assert( N >= K );
  
    this->M_N = N;
    this->M_K = K;
    
    this->M_storedCombinations = 0;
    this->M_generatedCombinations = 0;
    
    this->M_totalNumberOfCombinations = this->NChooseK( this->M_N, this->M_K );
    
    this->M_seed.resize( M_K );
  
    this->initialise();
    
    this->M_tupleList.clear();
    
    this->M_currTupleIt = this->M_tupleList.begin();
    
    return;
}

void
CombinationFactory::
initialise()
{    
    M_seed.resize( this->M_K );
   
    // Initialisation. First seed must be of type [0,1,2,..K] out of [0,1,2,...,K,...N].
    for ( UInt iK(0); iK < this->M_K; ++iK )
    {
	M_seed[ iK ] = iK;
    }
    
    this->M_tupleList.push_back( M_seed );
    
    this->M_storedCombinations++;
    
    return;
}

void
CombinationFactory::
writeCombinations( std::ostream & output ) const
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

bool
CombinationFactory::
hasTraversedAll() const
{
    return ( this->M_currTupleIt == this->M_tupleList.end() );
}

bool
CombinationFactory::
hasGeneratedAll() const
{
    return ( this->M_generatedCombinations == this->M_totalNumberOfCombinations );
}

void
CombinationFactory::
resetPointerToHeadCombination()
{
    this->M_currTupleIt = this->M_tupleList.begin();
}

void
CombinationFactory::
getNextCombination( tuple_Type & tuple  )
{
    assert( not( this->hasTraversedAll() ) );
    
//     this->M_askedCombinations++;
    
    tuple = *M_currTupleIt;
    
    this->M_currTupleIt++;
    
    return;
}

void
CombinationFactory::
generateNextCombination( tuple_Type & currentTuple )
{  
  assert( this->M_seed.size() == this->M_K );  
  
  assert( not( this->hasGeneratedAll() ) );
  
  
  if ( this->M_generatedCombinations == 0 )
  {
      currentTuple = this->M_seed;
      
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
    
    currentTuple = M_seed;
    }
  }
  
  return;
}


void
CombinationFactory::
generateCombinations()
{
  assert( this->M_storedCombinations == 1 );
  
  assert( this->M_generatedCombinations == 0 );
  
  assert( this->M_seed.size() == this->M_K );  
//   std::cout << " M_N = " << M_N << "  M_K = " << M_K << std::endl;
  
  while( M_seed[ 0 ] < M_N - M_K )
  {
     int iK = M_K - 1;
     
      M_seed[ iK ]++;
      
      for ( ; iK > 0; --iK )
      {
	  if( M_seed[ iK ] > M_N - ( M_K - iK ) )
	  {
// 	      std::cout << " IK = " << iK << std::endl;
	      
	      M_seed[ iK - 1 ]++;
 	      M_seed[ iK ] = M_seed[ iK - 1] + 1;
	      
	      for ( UInt iJ( iK + 1); iJ < M_K; ++iJ )
	      {
// 		  std::cout << " IJ = " << iJ << std::endl;
		
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
   
//   printf( " Number of combinations: stored %d | total expected %d\n", this->M_storedCombinations, this->M_totalNumberOfCombinations ); 
   
  assert( this->M_storedCombinations == this->M_totalNumberOfCombinations );
  
  this->M_generatedCombinations = this->M_storedCombinations;
  
  this->M_currTupleIt = this->M_tupleList.begin();
  
  return;  
}

CombinationFactory::UInt
CombinationFactory::
factorial( const UInt & K ) const
{
    UInt factorial(1);
  
    for ( UInt iK(2); iK <= K; ++iK )
    {
	factorial *= iK;
    }
    
    return factorial;
}

CombinationFactory::UInt
CombinationFactory::
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