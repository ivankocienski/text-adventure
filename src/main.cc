
#include "interface.hh"
#include "application.hh"

using namespace std;

int main( int argc, char ** argv ) {

  ta::Interface interface;
  interface.init( 640, 480 );

  ta::Application app( interface );

  return app.main();
}

