
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

  void Interface::puts( const string &s ) { 
    cout << s << endl;
  }

  void operator<<( Interface& i, char* p ) { 
    i.puts(p);
  }

  void operator<<( Interface& i, std::string s ) { 
    i.puts(s); 
  }

}; // namespace std

