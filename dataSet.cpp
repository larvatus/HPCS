

#include "dataSet.hpp"
#include <cassert>

namespace HPCS
{
    
////////////////////////
// DATA SET
////////////////////////
  
DataSet::
DataSet( const UInt & nbSamples, const UInt & nbPts )
:
M_nbSamples( nbSamples),
M_nbPts( nbPts ),
M_leftOffset( 0 ),
M_rightOffset( 0 )
{
  M_data = new Real[ M_nbSamples * M_nbPts ];
}

DataSet::
DataSet( Real * data, const UInt & nbSamples, const UInt & nbPts )
:
M_leftOffset( 0 ),
M_rightOffset( 0 )
{
  this->setData( data, nbSamples, nbPts ) ;
}

void
DataSet::
readCSV( const std::string & filename)
{   
   std::ifstream input( filename.data(), std::ios_base::in );

   Real trash;
   
   for ( UInt iSample(0); iSample < M_nbSamples; ++iSample )
   {    
	for ( UInt jPt(0); jPt < M_leftOffset; ++jPt )
	  
	  input >> trash;
     
	for ( UInt jPt(0); jPt < M_nbPts; ++jPt )
	{
	  input >> M_data[ iSample * M_nbPts + jPt ]; 
	}
	
	for ( UInt jPt(0); jPt < M_rightOffset; ++jPt )
	  
	  input >> trash;
    }

   input.close();
   
   return;
}

void
DataSet::
writeCSV( std::ostream & output ) const
{
  for ( UInt iSample(0); iSample < this->M_nbSamples; ++iSample )
  {
    for ( UInt iPt(0); iPt < this->M_nbPts - 1; ++iPt )
    {
	 output << M_data[ ( M_nbSamples + iSample ) * M_nbPts + iPt ] << " "; 
    }
     
    output << M_data[ ( M_nbSamples + iSample ) * M_nbPts + M_nbPts - 1 ] << std::endl; 
    
  }

  return;
}

DataSet::Real
DataSet::
operator()( const UInt & row, const UInt & col ) const
{  
  assert( row <= this->M_nbSamples && col <= this->M_nbPts );
      
  return this->M_data[ row * this->M_nbPts + col ];
}

void
DataSet::
showMe( std::ostream & output  ) const
{
  // TODO FINISH ME!!
  output << " ****** DataSet content ****** " << std::endl;

  output << " # Samples  \t = " << M_nbSamples << std::endl;
  output << " # Points \t = " << M_nbPts << std::endl;
  output << " Left offset in input \t = " << M_leftOffset << std::endl;
  output << " Right offset in input \t = " << M_rightOffset << std::endl;
  output << " ************************* " << std::endl;

  return;
}

void
DataSet::
setData( Real * data, const UInt & nbSamples, const UInt & nbPts )
{
    this->M_nbSamples = nbSamples;
    
    this->M_nbPts = nbPts;
  
    this->M_data = data;
    
    this->setOffset( 0, 0 );
    
    return;
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
{
}

DataSetLevelled::
DataSetLevelled( const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels )
:
DataSet( nbSamples, nbPts ),
M_nbLevels( nbLevels )
{ 
  this->setOffset( 0, 0 );
}

DataSetLevelled::
DataSetLevelled( Real * data, const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels )
:
DataSet( data, nbSamples, nbPts ),
M_nbLevels( nbLevels )
{  
  this->setOffset( 0, 0 ); 
}

DataSetLevelled::IDContainer_Type & 
DataSetLevelled::
level( const UInt lev ) const
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
 	std::pair< UInt, UInt > idCurrent( ID, ID );
	
 	(*this->M_levelsPtr)[ iLevel ].insert( idCurrent );
	  
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
    for( UInt iLevel(0); iLevel < this->M_nbLevels; ++iLevel )
    {
	output << " *** LEVEL # " << iLevel << std::endl;
	
	typedef IDContainer_Type::iterator iterator_Type;
	
	for ( iterator_Type it = (*this->M_levelsPtr)[ iLevel ].begin(); it != (*this->M_levelsPtr)[ iLevel ].end(); ++it  )
	{
	  output << it->first << " ";
	}
	
	output << std::endl;
      
    }
  
    return;
}



}