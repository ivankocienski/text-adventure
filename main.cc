
#include "interface.hh"
#include "engine.hh"

int main( int argc, char ** argv ) {

  ta::Interface interface;
  interface.init( 640, 480 );

  ta::Engine engine(interface);
  engine.run();

  return 0;
}

