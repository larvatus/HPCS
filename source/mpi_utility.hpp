

#ifndef __MPI_UTILITY_HPP__
#define __MPI_UTILITY_HPP__

#include <source/HPCSDefs.hpp>

namespace HPCS
{

//! @class mpiUtility this class Implements an environment to collect important information about parallel execution
/*!
 * This class implements an environment where important information about parallel execution are collected and available to
 * to the extern, i.e. typically a bigger objcet whose methods benefit from a parallel execution.
 */
class mpiUtility
{
  
public:
  
   //! Default constructor
   mpiUtility();
   
   //! Default destructor
   ~mpiUtility();
   
   //! Getter of the rank of current thread.
   UInt myRank() const;
   
   //! Getter of the number of threads
   UInt nbThreads() const;
   
   //! Getter of the master ID
   UInt master() const;
   
   //! Am I the master?
   bool isMaster() const;
   
   //! Master processor prints the specified string to the specified output 
   void leaderPrint( const std::string & string, std::ostream & output = std::cout ) const;

   //! Master processor prints the specified string to the specified output 
   void leaderPrint( const char * string, std::ostream & output = std::cout ) const;
   
   //! Method to start the timer
   void startTimer();
   
   //! Method to stop the timer.
   /*!
    * 	Time from the previous start is summed to the elapsed time 
    */
   void stopTimer();
   
   //! Method to reset the timer
   void resetTimer();
   
   //! Getter of the elapsed time
   Real elapsedTime() const;
   
private:
   
   
   static const UInt S_MASTER = MASTER;
  
   //! Variable to store my rank
   int M_myRank;
   
   //! Variable to store the number of threads
   int M_nbThreads;
   
   //! Time of the last timer start
   Real M_startTime;
   
   //! Time of stop
   Real M_stopTime;
   
   //! Elapsed time
   Real M_elapsedTime;

};

}


#endif