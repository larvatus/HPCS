
#include <source/bandDepth.hpp>
#include <source/bandDepthData.hpp>
#include <source/bandDepthFactory.hpp>
#include <source/dataSet.hpp>

#include <source/HPCSDefs.hpp>

using namespace std;
using namespace HPCS;

typedef BandDepthData bandDepthData_Type;
typedef BDFactory< All > bdFactory_Type;
typedef boost::shared_ptr< BandDepthBase< All > > bdBasePtr_Type; 

int main( int argc, char * argv[] )
{

MPI_Init( &argc, &argv );
  
   GetPot command_line( argc, argv );
      
   const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
     
   GetPot dataFile( data_file_name.data() );

   bandDepthData_Type bdData( dataFile, "BDALL" );
 
   bdFactory_Type factory;
   
   bdBasePtr_Type bdPtr( factory.create( bdData.J() ) );
   
   bdPtr->setBandDepthData( bdData );
       
   bdPtr->computeBDs();
  
   bdPtr->writeBDs();

MPI_Finalize();

  return EXIT_SUCCESS;
  
}
