

#ifndef __MPI_UTILITY_HPP__
#define __MPI_UTILITY_HPP__

#include <HPCSDefs.hpp>

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
   
   void leaderPrint( const std::string & string, std::ostream & output = std::cout ) const;

   void leaderPrint( const char * string, std::ostream & output = std::cout ) const;
   
private:
   
   //! Do NOT change this, otherwise the code using it could stop running.
   static const UInt S_MASTER = 0;
  
   int M_myRank;
   
   int M_nbThreads;

};

}


#endif