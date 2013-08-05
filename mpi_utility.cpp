
#include "mpi_utility.hpp"

namespace HPCS
{
  
  //////////////////
  // MPIUTILITY
  /////////////////
  
  
  mpiUtility::
  mpiUtility()
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
  leaderPrint( std::ostream & output, const std::string & string ) const
  {
    if ( this->M_myRank == mpiUtility::S_MASTER )
    {
	output << string << std::endl;
    }
    
    return;
  }
  
}