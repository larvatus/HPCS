

#include <bandDepth.hpp>
#include <bandDepthData.hpp>
#include <dataSet.hpp>

#include "user_fun.hpp"

#include <HPCSDefs.hpp>


using namespace std;
using namespace HPCS;


typedef BandDepthData bandDepthData_Type;
typedef BandDepth< 2 > bandDepth_Type;
typedef DataSet dataSet_Type;
typedef bandDepth_Type::dataSetPtr_Type dataSetPtr_Type;
typedef DataSetLevelled dataSetLevelled_Type;

int main( int argc, char * argv[] )
{
   // SETTING UP PARALLEL EXECUTION
  
MPI_Init( &argc, &argv );
 
 
MPI_Finalize();

  return EXIT_SUCCESS;
  
}