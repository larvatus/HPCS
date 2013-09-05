
# ifndef __COMBINATIONS_HPP__
# define __COMBINATIONS_HPP__

#include <source/HPCSDefs.hpp>

namespace HPCS
{

//! Free function for the computation of binomial coefficients;
UInt binomial( const UInt & N , const UInt & K );  
  
  
class CombinationsID
{
public:

  typedef unsigned int UInt;

  typedef double Real;
  
  typedef std::vector< UInt > tuple_Type;
  
  typedef std::list< tuple_Type > tupleList_Type;
  
  typedef std::list< tuple_Type >::iterator tupleListIterator_Type;
  
  typedef std::list< tuple_Type >::const_iterator tupleListConstIterator_Type;  
    
  CombinationsID(){};
  
  CombinationsID( const UInt & N, const UInt & K, const bool & storeFlag = false );
  
  ~CombinationsID();
  
  void reset( const UInt & N, const UInt & K, const bool & storeFlag = false );
  
  void generateCombinationsID();
  
  void writeCombinationsID( std::ostream & output ) const;  
  
  bool hasTraversedAll() const;
  
  bool hasGeneratedAll() const;
  
  void resetPointerToHeadCombination();
  
  void getNextCombinationID( tuple_Type & tuple);
  
protected:
  
  UInt M_N;
  
  UInt M_K;
  
  UInt M_generatedCombinations;

  UInt M_storedCombinations;
  
  UInt M_totalNumberOfCombinations;
  
  tupleListIterator_Type M_currTupleIt;  
  
  tuple_Type M_seed;
  
  tupleList_Type M_tupleList; 

  bool M_storeFlag;
  
private:
  
  void initialise();
   
  UInt NChooseK( const UInt & N, const UInt & K ) const;
  
  UInt factorial( const UInt & K ) const;
};


template < class _T >
class 
Combinations : public CombinationsID
{

public:
  
  typedef double Real;
  
  typedef unsigned int UInt;
  
  typedef boost::shared_ptr< std::vector< _T > > dataPtr_Type;
  
  
  Combinations< _T >(){};
  
  Combinations< _T >( const UInt & N, const UInt & K, 
		      const dataPtr_Type & dataPtr, const bool & storeFlag = false );
  
  
  void reset( const UInt & N, const UInt & K, 
	      const dataPtr_Type & dataPtr, const bool & storeFlag = false );
  
  void getNextCombination( const dataPtr_Type & dataPtr );
  
private:
  
  dataPtr_Type M_dataPtr;
  
};

template < class _T >
Combinations< _T >::
Combinations( const UInt & N, const UInt & K, 
		    const dataPtr_Type & dataPtr, const bool & storeFlag )
:
CombinationsID( N, K, storeFlag),
M_dataPtr( dataPtr )
{  
}


template < class _T >
void
Combinations< _T >::
reset( const UInt & N, const UInt & K, 
       const dataPtr_Type & dataPtr, const bool & storeFlag )
{
    this->reset( N, K, storeFlag );
    
    this->M_dataPtr.reset( dataPtr );
    
    return;
}

template < class _T >
void 
Combinations< _T >::
getNextCombination( const dataPtr_Type & dataPtr )
{
    assert( dataPtr->size() == this->M_K );
    
    CombinationsID::tuple_Type tupleIDs;
    
    this->getNextCombinationID( tupleIDs );
    
    for ( UInt iK(0); iK < this->M_K; ++iK )
    {
	(*dataPtr)[ iK ] = (*this->M_dataPtr)[ tupleIDs[ iK ] ];
    }
    
    return;
}

}





#endif /* __COMBINATIONS_HPP__ */