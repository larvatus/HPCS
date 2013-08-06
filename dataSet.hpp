
#ifndef _DATASET_HPP__
#define _DATASET_HPP__

#include <string>
#include <cassert>
#include <vector>
#include <fstream>
#include <iostream>

namespace HPCS
{

class DataSet
{
public:

    typedef unsigned int UInt;

    typedef double Real;
  
    DataSet( const UInt & nbSamples, const UInt & nbPts, const UInt & leftOffset = 0, const UInt & rightOffset = 0 );
    
    DataSet( Real * data, const UInt & nRows, const UInt & nCols );
    
    ~DataSet()
    {
      
      delete[] M_data; 

    };

    void readCSV( const std::string & filename );
    
    void writeCSV( std::ostream & output = std::cout ) const;
    
    void showMe( std::ostream & ) const;
    
    Real * getData(){ return M_data; };
    
    void setData( Real * data, const UInt & nRows, const UInt & nCols);
   
    Real operator()( const UInt & row, const UInt & col ) const;
    
    UInt nbSamples() const { return this->M_nbSamples; }
    
    UInt nbPts() const { return this->M_nbPts; }
    
private:

  Real * M_data;

  UInt M_nbSamples;

  UInt M_nbPts;
  
  UInt M_leftOffset;
  
  UInt M_rightOffset;
  
};

}

#endif /* _DATASET_HPP__ */

