

#ifndef __HPCS_DEFS_HPP
#define __HPCS_DEFS_HPP__


// COMMONLY USED CONTAINERS & ALGORITHMS
#include <vector>
#include <list>
#include <map>
#include <set>
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

// MPI
#include <mpi.h>

// GETPOT
#include <ioutput/GetPot>


// DEFINITION OF MASTER PROC
#define MASTER 0

namespace HPCS
{
    typedef unsigned int UInt;
    typedef double Real;

}


#endif /* __HPCS_DEFS_HPP__ */