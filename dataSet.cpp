

#include "dataSet.hpp"
#include <cassert>

namespace HPCS
{
    
////////////////////////
// DATA SET
////////////////////////

DataSet::
~DataSet()
{}

DataSet::
DataSet( const UInt & nbSamples, const UInt & nbPts )
:
M_nbSamples( nbSamples),
M_nbPts( nbPts ),
M_leftOffset( 0 ),
M_rightOffset( 0 ),
M_data( new data_Type( nbSamples, nbPts ) )
{
  
}

DataSet::
DataSet( const Real * data, const UInt & nbSamples, const UInt & nbPts )
:
M_leftOffset( 0 ),
M_rightOffset( 0 )
{
  this->setData( data, nbSamples, nbPts ) ;
}

DataSet::
DataSet( const std::vector< Real > & data, const UInt & nbSamples, const UInt & nbPts )
:
M_leftOffset( 0 ),
M_rightOffset( 0 )
{
  this->setData( data, nbSamples, nbPts ) ;
}

DataSet::
DataSet( const data_Type & data )
:
M_leftOffset( 0 ),
M_rightOffset( 0 ),
M_data( new data_Type( data ) ),
M_nbSamples( data.size1() ),
M_nbPts( data.size2() )
{}

DataSet::
DataSet( const dataPtr_Type & dataPtr )
:
M_leftOffset( 0 ),
M_rightOffset( 0 ),
M_data( dataPtr ),
M_nbSamples( dataPtr->size1() ),
M_nbPts( dataPtr->size2() )
{}

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

DataSet::Real
DataSet::
operator()( const UInt & row, const UInt & col ) const
{  
  assert( row <= this->M_nbSamples && col <= this->M_nbPts );
      
  return (*this->M_data)( row, col );
}

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

void
DataSet::
setData( const data_Type & data )
{
 this->M_nbSamples = data.size1();
 
 this->M_nbPts = data.size2();
 
 this->M_data.reset( new data_Type( data ) );
 
 this->setOffset( 0, 0 );
 
 return;
 
}

void
DataSet::
setData( const dataPtr_Type & dataPtr )
{
  this->M_nbSamples = dataPtr->size1();
 
 this->M_nbPts = dataPtr->size2();
 
 this->M_data = dataPtr;
 
 this->setOffset( 0, 0 );
 
 return;
}

DataSet::dataPtr_Type
DataSet::
getSubSet( const slice_Type & sampleSlice ) const
{
  using namespace boost::numeric::ublas;
  
  typedef matrix_slice< data_Type > dataSlice_Type;
  
  dataSlice_Type temp( *this->M_data, sampleSlice, slice( 0, 1, this->M_nbPts ) );
  
  return dataPtr_Type( new data_Type()  );
}

void
DataSet::
setOffset( const UInt & leftOffset, const UInt & rightOffset )
{
    this->M_leftOffset = leftOffset;
    
    this->M_rightOffset = rightOffset;
       
    this->M_nbPts = this->M_nbPts - leftOffset - rightOffset;
    
    return;
}


////////////////////////
// DATA SET LEVELLED
////////////////////////

DataSetLevelled::
~DataSetLevelled()
{}

DataSetLevelled::
DataSetLevelled( const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels )
:
DataSet( nbSamples, nbPts ),
M_nbLevels( nbLevels )
{}

DataSetLevelled::
DataSetLevelled( const Real * data, const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels )
:
DataSet( data, nbSamples, nbPts ),
M_nbLevels( nbLevels )
{}

DataSetLevelled::
DataSetLevelled( const std::vector< Real > & data, const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels )
:
DataSet( data, nbSamples, nbPts ),
M_nbLevels( nbLevels )
{}

DataSetLevelled::
DataSetLevelled( const data_Type & data, const UInt & nbLevels )
:
DataSet( data ),
M_nbLevels( nbLevels )
{}

DataSetLevelled::
DataSetLevelled( const dataPtr_Type & dataPtr, const UInt & nbLevels )
:
DataSet( dataPtr ),
M_nbLevels( nbLevels )
{}

DataSetLevelled::IDContainer_Type & 
DataSetLevelled::
levelIDs( const UInt lev ) const
{
  assert( lev <= M_nbLevels );
  
  return (*M_levelsPtr)[ lev ];
  
}


void
DataSetLevelled::
setLevels( const levelsContainerPtr_Type & levelsPtr )
{
    this->M_levelsPtr = levelsPtr;
  
    return;
}

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


DataSetLevelled::UInt
DataSetLevelled::
cardinality( const UInt & levelID )
{
    assert( levelID < this->M_nbLevels );
    
    if ( this->M_cardinality.size() != this->M_nbLevels )
    {
	for ( UInt iLevel(0); iLevel < this->M_nbLevels; ++iLevel )
	{
	    this->M_cardinality[ iLevel ] = (*M_levelsPtr)[ iLevel ].size();
	}
    }
  
    return this->M_cardinality[ levelID ];
}

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



}