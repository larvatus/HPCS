
#include <bandDepthRef.hpp>
#include <bandDepthData.hpp>
#include <bandDepthFactory.hpp>
#include <dataSet.hpp>

#include <HPCSDefs.hpp>

using namespace std;
using namespace HPCS;

typedef BandDepthRefData bandDepthData_Type;
typedef BDFactory< Reference > bdFactory_Type;
typedef boost::shared_ptr< BandDepthBase< Reference > > bdBasePtr_Type; 

int main( int argc, char * argv[] )
{

MPI_Init( &argc, &argv );
  
   GetPot command_line( argc, argv );
      
   const string data_file_name = command_line.follow( "data", 2, "-f", "--file" );
     
   GetPot dataFile( data_file_name.data() );

   bandDepthData_Type bdData( dataFile, "BDREF" );
 
   bdFactory_Type factory;
   
   bdBasePtr_Type bdPtr( factory.create( bdData.J() ) );
   
   bdPtr->setBandDepthData( bdData );
   
   bdPtr->addToReferenceSet( 0, 50 );
   
   bdPtr->setTestSet();
       
   bdPtr->computeBDs();
  
   bdPtr->writeBDs();

MPI_Finalize();

  return EXIT_SUCCESS;
  
}
