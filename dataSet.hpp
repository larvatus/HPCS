
#ifndef _DATASET_HPP__
#define _DATASET_HPP__

#include <string>
#include <cassert>
#include <vector>
#include <fstream>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/storage.hpp>
#include <set>

namespace HPCS
{

typedef unsigned int UInt;
typedef double Real;
  
class DataSet
{
public:

    typedef unsigned int UInt;

    typedef double Real;
    
    typedef boost::numeric::ublas::matrix< Real > data_Type;
    
    typedef boost::shared_ptr< data_Type > dataPtr_Type;
    
    typedef boost::numeric::ublas::matrix_slice< data_Type > dataSubSet_Type;
    
    typedef boost::numeric::ublas::slice slice_Type;
    
    
    DataSet( const UInt & nbSamples, const UInt & nbPts );
    
    DataSet( const Real * data, const UInt & nbSamples, const UInt & nPts );
    
    DataSet( const std::vector< Real > & data, const UInt & nbSamples, const UInt & nbPts );
 
    DataSet( const data_Type & data );
 
    DataSet( const dataPtr_Type & dataPtr );
    
    ~DataSet();

    void readData( const std::string & filename );

    
    void writeData( std::ostream & output = std::cout ) const;
    
    void showMe( std::ostream & output = std::cout ) const;
    
    void setData( const Real * data, const UInt & nbSamples, const UInt & nPts);
    
    void setData( const std::vector< Real > & data, const UInt & nbSamples, const UInt & nPts);

    void setData( const data_Type & data);
    
    void setData( const dataPtr_Type & dataPtr );

    void setOffset( const UInt & leftOffset, const UInt & rightOffset );
    
    Real operator()( const UInt & sample, const UInt & pt ) const;
    
    dataPtr_Type getData(){ return this->M_data; };

//     dataPtr_Type getSubSet( const slice_Type & sampleSlice ) const;
    
    dataPtr_Type getRowSubSet( const std::vector< UInt > & IDs ) const;
     
    UInt nbSamples() const { return this->M_nbSamples; }
    
    UInt nbPts() const { return this->M_nbPts; }
    
protected:

  dataPtr_Type M_data;

  UInt M_nbSamples;

  UInt M_nbPts;
  
  UInt M_leftOffset;
  
  UInt M_rightOffset;
  
};


class DataSetLevelled : public DataSet 
{
  
public:
  
  typedef unsigned int UInt;
  
  typedef double Real;
  
  typedef DataSet::data_Type data_Type;
    
  typedef DataSet::dataPtr_Type dataPtr_Type;
  
  typedef std::set< UInt > IDContainer_Type;
  
  typedef std::vector< IDContainer_Type > levelsContainer_Type;
  
  typedef boost::shared_ptr< levelsContainer_Type > levelsContainerPtr_Type;
  
  
  DataSetLevelled( const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels );
 
  DataSetLevelled( const Real * data, const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels );
  
  DataSetLevelled( const std::vector< Real > & data, const UInt & nbSamples, const UInt & nbPts, const UInt & nbLevels );
  
  DataSetLevelled( const data_Type & data, const UInt & nbLevels );
  
  DataSetLevelled( const dataPtr_Type & dataPtr, const UInt & nbLevels );
  
  ~DataSetLevelled();
  
  
  void setLevels( const levelsContainerPtr_Type & levelsPtr );
  
  void setLevels( const std::vector< UInt > & linearExtrema );
  
  void readLevelsExtrema( const std::string & levelsExtremaFilename );

  void showMe( std::ostream & output = std::cout ) const;  
  
  IDContainer_Type & levelIDs( const UInt lev ) const;
  
  UInt cardinality( const UInt & levelID );
  
  
private:
  
  levelsContainerPtr_Type M_levelsPtr;
  
  UInt M_nbLevels;
  
  std::vector< UInt > M_cardinality;
  
  
};




}

#endif /* _DATASET_HPP__ */

