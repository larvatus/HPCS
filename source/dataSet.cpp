
/*!
 *   @file DataSet.cpp
     @brief Definition of DataSet and DataSetLevelled classes.

     @date 08/2013
     @author Nicholas Tarabelloni  <nicholas.tarabelloni@gmail.com>
     @maintainer Nicholas Tarabelloni  <nicholas.tarabelloni@gmail.com>
*/

#include <source/dataSet.hpp>

namespace HPCS
{
    
//////////////////////////////////////////
//					//
//					//
// 	      DATA SET 			//
//					//
//					//
//////////////////////////////////////////


// Default constructor
DataSet::
~DataSet()
{}

// Constructor from the number of samples and the number of points
DataSet::
DataSet( const UInt & nbSamples, const UInt & nbPts )
:
M_nbSamples( nbSamples),
M_nbPts( nbPts ),
M_leftOffset( 0 ),
M_rightOffset( 0 ),
M_data( new data_Type( nbSamples, nbPts ) )
// M_corMatrixPtr( new matrix_Type() ),
// M_varMatrixPtr( new matrix_Type() ),
// M_varMatrixComputedFlag( false ),
// M_corMatrixComputedFlag( false )
{
}

// Constructor from the raw data array, the number of samples and the number of points
DataSet::
DataSet( const Real * data, const UInt & nbSamples, const UInt & nbPts )
:
M_leftOffset( 0 ),
M_rightOffset( 0 )
// M_corMatrixPtr( new matrix_Type() ),
// M_varMatrixPtr( new matrix_Type() ),
// M_varMatrixComputedFlag( false ),
// M_corMatrixComputedFlag( false )
{
  this->setData( data, nbSamples, nbPts ) ;
}

// Constructor from the std::vector data array, the number of samples and the number of points
DataSet::
DataSet( const std::vector< Real > & data, const UInt & nbSamples, const UInt & nbPts )
:
M_leftOffset( 0 ),
M_rightOffset( 0 )
// M_corMatrixPtr( new matrix_Type() ),
// M_varMatrixPtr( new matrix_Type() ),
// M_varMatrixComputedFlag( false ),
// M_corMatrixComputedFlag( false )
{
  this->setData( data, nbSamples, nbPts );
}

// Constructor from a matrix object
DataSet::
DataSet( const data_Type & data )
:
M_leftOffset( 0 ),
M_rightOffset( 0 ),
M_data( new data_Type( data ) ),
M_nbSamples( data.rows() ),
M_nbPts( data.cols() )
// M_corMatrixPtr( new matrix_Type() ),
// M_varMatrixPtr( new matrix_Type() ),
// M_varMatrixComputedFlag( false ),
// M_corMatrixComputedFlag( false )
{}

// Constructor from a shared pointer to a matrix object
DataSet::
DataSet( const dataPtr_Type & dataPtr )
:
M_leftOffset( 0 ),
M_rightOffset( 0 ),
M_data( dataPtr ),
M_nbSamples( dataPtr->rows() ),
M_nbPts( dataPtr->cols() )
// M_corMatrixPtr( new matrix_Type() ),
// M_varMatrixPtr( new matrix_Type() ),
// M_varMatrixComputedFlag( false ),
// M_corMatrixComputedFlag( false )
{}

// Method to read data from a filename
void
DataSet::
readData( const std::string & filename)
{   
   std::ifstream input( filename.data(), std::ios_base::in );

   Real trash;
   
   for ( UInt iSample(0); iSample < this->M_nbSamples; ++iSample )
   {    
	for ( UInt jPt(0); jPt < this->M_leftOffset; ++jPt )
	  
	  input >> trash;
     
	for ( UInt jPt(0); jPt < this->M_nbPts; ++jPt )
	{
	  input >> (*this->M_data)( iSample, jPt ); 
	}
	
	for ( UInt jPt(0); jPt < this->M_rightOffset; ++jPt )
	  
	  input >> trash;
    }

   input.close();
   
   return;
}

// Method to write data to some output destination
void
DataSet::
writeData( std::ostream & output ) const
{
   for ( UInt iSample(0); iSample < this->M_nbSamples; ++iSample )
   {
      for ( UInt iPt(0); iPt < this->M_nbPts - 1 ; ++iPt )
      {
	  output << (*this->M_data)( iSample, iPt )  << " ";
      }
      
      output << (*this->M_data)( iSample, this->M_nbPts - 1) << std::endl;
      
   }

  return;
}

// Method to access elements of the data set
Real
DataSet::
operator()( const UInt & row, const UInt & col ) const
{    
  assert( row <= this->M_nbSamples && col <= this->M_nbPts );
  
  return (*this->M_data)( row, col );
}

// Showme method writing the current status to some destination
void
DataSet::
showMe( std::ostream & output  ) const
{
  output << " ****** DataSet content ****** " << std::endl;

  output << " # Samples  \t = " << M_nbSamples << std::endl;
  
  output << " # Points \t = " << M_nbPts << std::endl;
  
  output << " Left offset in input \t = " << M_leftOffset << std::endl;
  
  output << " Right offset in input \t = " << M_rightOffset << std::endl;
  
  output << " Data : " << std::endl;
  
  this->writeData( output );
  
  output << " ************************* " << std::endl;

  return;
}

// Setter of data from raw array of data, number of samples, number of points.
void
DataSet::
setData( const Real * data, const UInt & nbSamples, const UInt & nbPts )
{
    this->M_nbSamples = nbSamples;
    
    this->M_nbPts = nbPts;
  
    this->M_data.reset( new data_Type( nbSamples, nbPts ) );
    
    for ( UInt iSample(0); iSample < nbSamples; ++iSample )
    {
	for ( UInt iPt(0); iPt < nbPts; ++iPt )
	{
	    (*this->M_data)( iSample, iPt ) = data[ iSample * nbPts + iPt ];
	}
    }
    
    this->setOffset( 0, 0 );
    
    return;
}

// Setter of data from std::vector array of data, number of samples, number of points.
void
DataSet::
setData( const std::vector< Real > & data, const UInt & nbSamples, const UInt & nbPts )
{
    this->M_nbSamples = nbSamples;
    
    this->M_nbPts = nbPts;
  
    this->M_data.reset( new data_Type( nbSamples, nbPts ) );
    
    for ( UInt iSample(0); iSample < nbSamples; ++iSample )
    {
	for ( UInt iPt(0); iPt < nbPts; ++iPt )
	{
	    (*this->M_data)( iSample, iPt ) = data[ iSample * nbPts + iPt ];
	}
    }
    
    this->setOffset( 0, 0 );
    
    return;
    
}

// Setter of data from a boost::numric::ublas::matrix object.
void
DataSet::
setData( const data_Type & data )
{
 this->M_nbSamples = data.rows();
  
 this->M_nbPts = data.cols();
 
 this->M_data.reset( new data_Type( data ) );
 
 this->setOffset( 0, 0 );
 
 return;
 
}

// Setter of data from a shared pointer to a boost::numric::ublas::matrix object.
void
DataSet::
setData( const dataPtr_Type & dataPtr )
{  
 this->M_nbSamples = dataPtr->rows();
  
 this->M_nbPts = dataPtr->cols();
 
 this->M_data = dataPtr;
 
 this->setOffset( 0, 0 );
 
 return;
}

// Getter of a subset of the dataset indexed by rows IDs.
DataSet::dataPtr_Type
DataSet::
getRowSubSet( const std::vector< UInt > & IDs ) const
{
  dataPtr_Type dataPtr( new data_Type( IDs.size(), this->M_nbPts ) );

  for ( UInt i(0); i < IDs.size(); ++i )
  {
    dataPtr->row( i ) = this->M_data->row( IDs[i] );
  }
  
  return dataPtr;
}

// Setter of the offset variables
void
DataSet::
setOffset( const UInt & leftOffset, const UInt & rightOffset )
{
    this->M_leftOffset = leftOffset;
    
    this->M_rightOffset = rightOffset;
       
    this->M_nbPts = this->M_nbPts - leftOffset - rightOffset;
    
    return;
}

/////////////////////////////////////////////////////////////////


//////////////////////////////////////////
//					//
//					//
// 	   DATA SET LEVELLED		//
//					//
//					//
//////////////////////////////////////////


// Default contructor
DataSetLevelled::
~DataSetLevelled()
{}

// Constructor from the number of samples, the number of points and the number of levels
DataSetLevelled::
DataSetLevelled( const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels )
:
DataSet( nbSamples, nbPts ),
M_nbLevels( nbLevels )
{}

// Constructor from a raw array data, the number of samples, the number of points and the number of levels.
DataSetLevelled::
DataSetLevelled( const Real * data, const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels )
:
DataSet( data, nbSamples, nbPts ),
M_nbLevels( nbLevels )
{}

// Constructor from a std::vector array of data, the number of samples, the number of points and the number of levels.
DataSetLevelled::
DataSetLevelled( const std::vector< Real > & data, const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels )
:
DataSet( data, nbSamples, nbPts ),
M_nbLevels( nbLevels )
{}

// Constructor from a base DataSet object and the number of levels
DataSetLevelled::
DataSetLevelled( const data_Type & data, const UInt & nbLevels )
:
DataSet( data ),
M_nbLevels( nbLevels )
{}

// Constructo from a shared pointer to a base DataSet object and the number of levels.
DataSetLevelled::
DataSetLevelled( const dataPtr_Type & dataPtr, const UInt & nbLevels )
:
DataSet( dataPtr ),
M_nbLevels( nbLevels )
{}

// Getter of the IDContainer objcet corresponding to the specified level ID
DataSetLevelled::IDContainer_Type & 
DataSetLevelled::
levelIDs( const UInt lev ) const
{
  assert( lev <= M_nbLevels );
  
  return (*M_levelsPtr)[ lev ];
  
}

// Setter of the levels from a levels container object.
void
DataSetLevelled::
setLevels( const levelsContainerPtr_Type & levelsPtr )
{
    this->M_levelsPtr = levelsPtr;
  
    return;
}

// Setter of levels from linear extrema
void
DataSetLevelled::
setLevels( const std::vector< UInt > & linearExtrema )
{
    assert( linearExtrema.size() == this->M_nbLevels + 1 );

    this->M_levelsPtr.reset( new levelsContainer_Type() );
    
    this->M_levelsPtr->resize( this->M_nbLevels );
    
    UInt iLevel(0);    
    
    for ( UInt iExtrema(0); iExtrema < this->M_nbLevels; ++iExtrema )
    {
      for ( UInt ID( linearExtrema[ iExtrema ] ); ID < linearExtrema[ iExtrema + 1 ]; ++ID )
      {	
 	(*this->M_levelsPtr)[ iLevel ].insert( ID );
	  
      }      
      ++iLevel;      
    }
  
    return;
}

// Getter of the cardinality of the specified levelID
UInt
DataSetLevelled::
cardinality( const UInt & levelID )
{
    assert( levelID < this->M_nbLevels );
    
    if ( this->M_cardinality.size() != this->M_nbLevels )
    {
	this->M_cardinality.resize( this->M_nbLevels );
      
	for ( UInt iLevel(0); iLevel < this->M_nbLevels; ++iLevel )
	{
	    this->M_cardinality[ iLevel ] = (*M_levelsPtr)[ iLevel ].size();
	}
    }
  
    return this->M_cardinality[ levelID ];
}

// Showme method to print to some destination the current status of the object.
void
DataSetLevelled::
showMe( std::ostream & output ) const
{
    this->DataSet::showMe( output );
  
    for( UInt iLevel(0); iLevel < this->M_nbLevels; ++iLevel )
    {
	output << " *** IDs LEVEL # " << iLevel << std::endl;
	
	typedef IDContainer_Type::iterator iterator_Type;
	
	for ( iterator_Type it = (*this->M_levelsPtr)[ iLevel ].begin(); it != (*this->M_levelsPtr)[ iLevel ].end(); ++it  )
	{
	  output << *it << " ";
	}
	
	output << std::endl;
      
    }
  
    return;
}

// Setter of levels from linear extrema contained inside a specified file
void
DataSetLevelled::
setLevelsFromExtrema( const std::string & filename)
{  
   std::ifstream input( filename.data(), std::ios_base::in );
   
   std::vector< UInt > linearExtrema( this->M_nbLevels + 1);
   
   for ( UInt iLevel(0); iLevel < this->M_nbLevels + 1; ++iLevel )
   {    
	input >> linearExtrema[ iLevel ];
   }

   input.close();
   
   this->setLevels( linearExtrema );
 
   return;
}



}