
#include <string>

#include "interface.hh"
#include "engine.hh"

using namespace std;

int main( int argc, char ** argv ) {

  ta::Interface interface;
  interface.init( 640, 480 );

  //ta::Engine engine(interface);
  //engine.run();

  while(true) {
    //interface.puts( 10, 10, "ABC abc 123 Hello, World" );
    string str = interface.wait_for_input();

    interface.puts( str );
  }

  return 0;
}

