
#include <source/mpi_utility.hpp>

namespace HPCS
{
  
  //////////////////
  // MPIUTILITY
  /////////////////
  
  
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
  
  mpiUtility::
  ~mpiUtility()
  {
  }
  
  mpiUtility::UInt
  mpiUtility::
  nbThreads() const
  {
      return this->M_nbThreads; 
  }
  
  mpiUtility::UInt
  mpiUtility::
  myRank() const
  {
      return this->M_myRank;
  }
  
  mpiUtility::UInt
  mpiUtility::
  master() const
  {
      return mpiUtility::S_MASTER;
  }
  
  bool
  mpiUtility::
  isMaster() const
  {
      return mpiUtility::S_MASTER == this->M_myRank;
  }
  
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
  
  void
  mpiUtility::
  startTimer()
  {   
     this->M_startTime = MPI_Wtime();
     
     return;
  }

  
  void
  mpiUtility::
  stopTimer()
  {
      this->M_stopTime = MPI_Wtime();
      
      this->M_elapsedTime += this->M_stopTime - this->M_startTime;
      
      return;
  }

  void
  mpiUtility::
  resetTimer()
  {
      this->M_stopTime = 0;
      
      this->M_startTime = 0;
      
      this->M_elapsedTime = 0;
    
      return;
  }
  
  Real
  mpiUtility::
  elapsedTime() const
  {
    return this->M_elapsedTime;
  }
    
}