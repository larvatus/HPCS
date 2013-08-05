////////////////////////////////////////////////////////////
//							  //
//	This file contains the definition of the class	  //
// 	CombinationFactory, creating a data structure	  //
// 	collecting the combinations of N objects taken	  //
// 	at groups of K					  //
//							  //
////////////////////////////////////////////////////////////


#ifndef _COMBINATIONFACTORY_HPP__
#define _COMBINATIONFACTORY_HPP__

#include <vector>
#include <list>
#include "GetPot"
#include <cassert>

namespace HPCS
{

class CombinationFactory
{
public:

  typedef unsigned int UInt;

  typedef double Real;
  
  typedef std::vector< UInt > tuple_Type;
  
  typedef std::list< tuple_Type > tupleList_Type;
  
  typedef std::list< tuple_Type >::iterator tupleListIterator_Type;
  
  typedef std::list< tuple_Type >::const_iterator tupleListConstIterator_Type;
  
  CombinationFactory(){};
  
  CombinationFactory( const UInt & N, const UInt & K );
  
  ~CombinationFactory();
  
  void reset( const UInt & N, const UInt & K );
  
  void generateCombinations();
  
  void writeCombinations( std::ostream & output ) const;  
  
  bool hasTraversedAll() const;
  
  bool hasGeneratedAll() const;
  
  void resetPointerToHeadCombination();
  
  void generateNextCombination( tuple_Type & tuple);
  
  void getNextCombination( tuple_Type & tuple);
  
protected:
  
  UInt M_N;
  UInt M_K;
  
  UInt M_generatedCombinations;
//   UInt M_askedCombinations;
  UInt M_storedCombinations;
  
  UInt M_totalNumberOfCombinations;
  
  tupleListIterator_Type M_currTupleIt;  
  
  tuple_Type M_seed;
  
  tupleList_Type M_tupleList; 
  
private:
  
  void initialise();
   
  UInt NChooseK( const UInt & N, const UInt & K ) const;
  
  UInt factorial( const UInt & K ) const;
};

}

#endif /* _COMBINATIONFACTORY_HPP__*/