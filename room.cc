
#include "room.hh"
#include "interface.hh"

#include <string>

using namespace std;

namespace ta {

  Room::Room( char *d) : m_description(d) {
  }

  Room::Room() { 
  }

  string Room::name() {
    return m_name;
  }

  void Room::describe( Interface &i ) {
    i.puts(m_description);
  }

}; // namespace ta
