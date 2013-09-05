

#ifndef __HPCS_DEFS_HPP__
#define __HPCS_DEFS_HPP__


// COMMONLY USED CONTAINERS & ALGORITHMS
#include <vector>
#include <list>
#include <map>
#include <set>
#include <functional>
#include <utility>
#include <algorithm>

// IOUTPUT & ASSERTS & OTHERS
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cassert>
#include <fstream>


//STRINGS & THEIR MANIPULATION
#include <string>
#include <cstring>
#include <sstream>

//COMMON BOOST HEADERS
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/static_assert.hpp>

// MPI
#include "mpi.h"

// GETPOT
#include <source/GetPot>


// DEFINITION OF MASTER PROC
#define MASTER 0

// TIMING OF MPI IS GLOBAL
#define MPI_WTIME_IS_GLOBAL 1

namespace HPCS
{
    typedef unsigned int UInt;
    typedef double Real;
    
    enum BDPolicy { Reference, All };

}


#endif /* __HPCS_DEFS_HPP__ */
