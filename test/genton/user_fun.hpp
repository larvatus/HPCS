

#ifndef __USER_FUN_HPP_
#define __USER_FUN_HPP_

#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iomanip>

#include <mpi.h>

#include <boost/smart_ptr/shared_ptr.hpp>

#include "dataSet.hpp"
#include "GetPot"
#include "combinations.hpp"


namespace HPCS
{

  namespace GentonTest
  {


    typedef double Real;
    typedef unsigned int UInt;
    
    typedef DataSet dataSet_Type;
    typedef boost::shared_ptr< dataSet_Type > dataSetPtr_Type;
    
    typedef dataSet_Type::data_Type data_Type;
    typedef dataSet_Type::dataPtr_Type dataPtr_Type;
    
    typedef CombinationsID combinationsID_Type;
    typedef CombinationsID::tuple_Type tuple_Type;
    
//     void computeBDs( int argc, char * argv[] );

    void computeBDs( const GetPot & dataFile, const std::string & section );
    
    UInt binomial( const UInt & N , const UInt & K );
  
  }
}






#endif