

#include <HPCSDefs.hpp>

#include <depthMeasure.hpp>
#include <bandDepthData.hpp>

using namespace std;
using namespace HPCS;

typedef DepthMeasure< 3, All > depthMeasure_Type;
typedef BandDepthData bdData_Type;
typedef boost::shared_ptr< bdData_Type > bdDataPtr_Type;

int main( int argc, char * argv[] )
{ 
  MPI_Init( &argc, &argv );
  
   int nbThreads, myRank;
   
   MPI_Comm_rank( MPI_COMM_WORLD, & myRank );
   
   MPI_Comm_size( MPI_COMM_WORLD, & nbThreads );
   
   // INPUT OF DATA FROM FILEANAMES
   
   GetPot command_line( argc, argv );
      
   const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
     
   GetPot dataFile( data_file_name.data() );
   
   // STARTING COMPUTATION VIA GENTON METHOD
   
   bdDataPtr_Type bdDataPtr( new bdData_Type( dataFile, "BDALL") );
   
   depthMeasure_Type DM( bdDataPtr );
   
   DM.compute();
   
 MPI_Finalize();
  
  return EXIT_SUCCESS;
  
}