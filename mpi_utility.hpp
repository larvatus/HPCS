

#ifndef __MPI_UTILITY_HPP__
#define __MPI_UTILITY_HPP__

#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <string>

namespace HPCS
{

class mpiUtility
{
  
public:
  
   typedef double Real;
   
   typedef unsigned int UInt;
   
   mpiUtility();
   
   ~mpiUtility();
   
   UInt myRank() const;
   
   UInt nbThreads() const;
   
   UInt master() const;
   
   bool isMaster() const;
   
   void leaderPrint( std::ostream & output, const std::string & string ) const;   
   
private:
   
   //! Do NOT change this, otherwise the code using it could stop running.
   static const UInt S_MASTER = 0;
  
   int M_myRank;
   
   int M_nbThreads;

};

}


#endif