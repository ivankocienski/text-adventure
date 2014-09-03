
#include <string>
#include <iostream>
#include <map>

#include <stdlib.h>

#include "room.hh"
#include "world.hh"

using namespace std;

namespace ta {

  World::World() {
  }

  void World::build_room( char *n, char *d ) {
    m_rooms[string(n)] = Room(d);
  }

  void World::list_rooms() {

    room_map_t::iterator it;

    cout << m_rooms.size() << " rooms defined" << endl;

    for(it = m_rooms.begin(); it != m_rooms.end(); it++) {

      string n = (*it).first;
      
      cout << "  " << n << endl;
    } 
  }

  Room* World::get( string &n ) {

    room_map_t::iterator it = m_rooms.find(n);

    if(it == m_rooms.end()) {
      cerr << "could not find room: " << n << endl;
      exit(-1);
    }

    return &(*it).second;
  }
}; // namespace ta
