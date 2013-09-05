
#include <source/mpi_utility.hpp>

namespace HPCS
{
  
  //////////////////
  // MPIUTILITY
  /////////////////
  
  // Default constructor
  mpiUtility::
  mpiUtility()
  :
  M_startTime(0),
  M_stopTime(0),
  M_elapsedTime(0)
  {
      MPI_Comm_size( MPI_COMM_WORLD, & (this->M_nbThreads) );
      
      MPI_Comm_rank( MPI_COMM_WORLD, & (this->M_myRank) );
  }
  
  // Default destructor
  mpiUtility::
  ~mpiUtility()
  {
  }
  
  // Getter of the number of threads
  mpiUtility::UInt
  mpiUtility::
  nbThreads() const
  {
      return this->M_nbThreads; 
  }
  
  // Getter of the current rank
  mpiUtility::UInt
  mpiUtility::
  myRank() const
  {
      return this->M_myRank;
  }
  
  // Getter of the current master ID
  mpiUtility::UInt
  mpiUtility::
  master() const
  {
      return mpiUtility::S_MASTER;
  }
  
  // Am I the master?
  bool
  mpiUtility::
  isMaster() const
  {
      return mpiUtility::S_MASTER == this->M_myRank;
  }
  
  // Master processor prints specified string to specified output
  void
  mpiUtility::
  leaderPrint( const std::string & string, std::ostream & output ) const
  {
    if ( this->M_myRank == mpiUtility::S_MASTER )
    {
	output << string << std::endl;
    }
    
    return;
  }
  
 // Master processor prints specified string to specified output
 void
  mpiUtility::
  leaderPrint( const char * string, std::ostream & output ) const
  {
    if ( this->M_myRank == mpiUtility::S_MASTER )
    {
	output << string << std::endl;
    }
    
    return;
  }
  
  // Method to start the timer
  void
  mpiUtility::
  startTimer()
  {   
     this->M_startTime = MPI_Wtime();
     
     return;
  }

  // Method to stop the timer
  void
  mpiUtility::
  stopTimer()
  {
      this->M_stopTime = MPI_Wtime();
      
      this->M_elapsedTime += this->M_stopTime - this->M_startTime;
      
      return;
  }

  // Method to reset the timer
  void
  mpiUtility::
  resetTimer()
  {
      this->M_stopTime = 0;
      
      this->M_startTime = 0;
      
      this->M_elapsedTime = 0;
    
      return;
  }
  
  // Getter of the elapsed time
  Real
  mpiUtility::
  elapsedTime() const
  {
    return this->M_elapsedTime;
  }
    
}