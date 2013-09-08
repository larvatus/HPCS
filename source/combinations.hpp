
# ifndef __COMBINATIONS_HPP__
# define __COMBINATIONS_HPP__

#include <source/HPCSDefs.hpp>

namespace HPCS
{

//! Free function for the computation of binomial coefficients;
UInt binomial( const UInt & N , const UInt & K );  
  
//! @class CombinationsID this class implements the generation of the combinations of N objects choose k.
/*!
 * This class implements a generator of the IDs of the combinations of N objects by k
 */
class CombinationsID
{
public:

  //! @name Public Types
  //@{

    //! Typedef of the tuple type containing combinated IDs
    typedef std::vector< UInt > tuple_Type;
    
    //! Typedef of the list of combinated IDs
    typedef std::list< tuple_Type > tupleList_Type;
    
    //! Typedef of the iterator to the combinated IDs list
    typedef std::list< tuple_Type >::iterator tupleListIterator_Type;
    
    //! Typedef of the const iterator to the combinated IDs list
    typedef std::list< tuple_Type >::const_iterator tupleListConstIterator_Type;  
   
  //@}
  
  //! @name Constructor and Destructor
  //@{

    //! Standard default constructor
    CombinationsID(){};
    
    //! Standard constructor from specified variables
    /*!
     * @param N is the number of elements
     * @param K is the size of combinations
     * @param storeFlag tells if the class has to store the combination once they're computed or not.
     */
    CombinationsID( const UInt & N, const UInt & K, const bool & storeFlag = false );
    
    //! Standard default destructor
    virtual ~CombinationsID();
  
  //@}
  
  //! @name Misc. Public Methods
  //@{

    //! Method to reset the the parameters
    void reset( const UInt & N, const UInt & K, const bool & storeFlag = false );
    
    //! Method to generate all the combinations
    void generateCombinationsID();
    
    //! Method to write to some output the computed combinations
    void writeCombinationsID( std::ostream & output ) const;  
    
    //! Method to ask if the class has traversed the whole list of computed combinations or not
    bool hasTraversedAll() const;
    
    //! Method to ask if the class has generated all the combinations or not
    bool hasGeneratedAll() const;
    
    //! Method to reset the iterator the the head of the computed combinations list
    void resetPointerToHeadCombination();
    
    //! Method to get the next combination, computed or read according to the storeFlag specified.
    void getNextCombinationID( tuple_Type & tuple);
  
  //@}
    
protected:
  
  //! @name Protected Members
  //@{
    
    //! The number of elements
    UInt M_N;
    
    //! The size of the combination
    UInt M_K;
    
    //! The number of generated combinations
    UInt M_generatedCombinations;

    //! The number of stored combinations
    UInt M_storedCombinations;
    
    //! The number of total combinations to be generated
    UInt M_totalNumberOfCombinations;
    
    //! The iterator to the list of combinations
    tupleListIterator_Type M_currTupleIt;  
    
    //! The seed tuple, setup by the initialise method.
    tuple_Type M_seed;
    
    //! The list of combinations already generated.
    tupleList_Type M_tupleList; 

    //! The flag specifying if the class has to store the combinations computed or not.
    bool M_storeFlag;
  
  //@}
    
private:
  
  //! @name Private Methods
  //@{
  
  //! Method to initialize the algorithm of the computation of combinations with the first, standard combination.
  void initialise();
   
  //! Method to compute the value of N choose K.
  UInt NChooseK( const UInt & N, const UInt & K ) const;
  
  //! Method to compute the factorial of a number
  UInt factorial( const UInt & K ) const;
  
  //@}
};

//! @class Combinations is the class implementing the combinations of N objects by K, returning the objects and not the IDs.
/*!
 *  It evolves the base class CombinationsID to return the combinated objects and not the combinated IDs.
 */
template < class _T >
class 
Combinations : public CombinationsID
{

public:
  
  //! @name Public Types
  //@{

    //! Typedef of the pointer to the data type
    typedef boost::shared_ptr< std::vector< _T > > dataPtr_Type;
  
  //@}
  
  //! @name Constructor & Destructor
  //@{
    
  //! Standard default constructor
  Combinations< _T >(){};
  
  //! Constructor from specified variables
  /*!
   * @param N is the number of elements
   * @param K is the size of combinations
   * @param storeFlag is the flag specifying if the class has to store computed combinations or not.
   */
  Combinations< _T >( const UInt & N, const UInt & K, 
		      const dataPtr_Type & dataPtr, const bool & storeFlag = false );
  
  //! Standard default destructor
  ~Combinations(){}
  
  //@}
  
  //! @name Misc. Public Methods
  //@{

    //! Method to reset the class with data and parameters
    void reset( const UInt & N, const UInt & K, 
		const dataPtr_Type & dataPtr, const bool & storeFlag = false );
    
    //! Method to get the next combination of objects, computed or read according to the storing policy.
    void getNextCombination( const dataPtr_Type & dataPtr );
  
  //@}
  
private:
  
  //!@name Private Members
  //@{
  
    //! The shared pointer to data.
    dataPtr_Type M_dataPtr;
  
  //@}
  
};

// Definition of the constructor
template < class _T >
Combinations< _T >::
Combinations( const UInt & N, const UInt & K, 
		    const dataPtr_Type & dataPtr, const bool & storeFlag )
:
CombinationsID( N, K, storeFlag),
M_dataPtr( dataPtr )
{  
}

// Definition of the reset method
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

// Definition of the method to get the next combination
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