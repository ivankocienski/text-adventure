
#include <iostream>
#include <string>

#include "interface.hh"

using namespace std;

namespace ta {
  
  Interface::Interface() {
  }

  void Interface::puts( const char *s ) {
    cout << s << endl;
  }

  void Interface::puts( string &s ) { 
    cout << s << endl;
  }

  
}; // namespace std

