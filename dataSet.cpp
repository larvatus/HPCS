

#include "dataSet.hpp"

namespace HPCS
{
  
DataSet::
DataSet( const UInt & nbSamples, const UInt & nbPts, const UInt & leftOffset, const UInt & rightOffset )
:
M_nbSamples( nbSamples),
M_nbPts( nbPts - leftOffset - rightOffset ),
M_leftOffset( leftOffset ),
M_rightOffset( rightOffset )
{
  M_data = new Real[ M_nbSamples * M_nbPts ];
}

DataSet::
DataSet( Real * data, const UInt & nRows, const UInt & nCols )
:
M_leftOffset( 0 ),
M_rightOffset( 0 )
{
  this->setData( data, nRows, nCols ) ;
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
setData( Real * data, const UInt & nRows, const UInt & nCols )
{
    this->M_nbSamples = nRows;
    
    this->M_nbPts = nCols;
  
    this->M_data = data;
    
    this->M_leftOffset = 0;
    
    this->M_rightOffset = 0;
    
    return;
}

}