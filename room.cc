
#include "room.hh"
#include "interface.hh"

#include <iostream>

namespace ta {

  Room::Room() {
  }

  void Room::describe( Interface &i ) {
    i.puts("A room");
  }

}; // namespace ta
