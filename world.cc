
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

  void World::build_room( const string &n, const string &d ) {
    m_rooms[n] = Room(n, d);
  }

  void World::list_rooms() {

    room_map_t::iterator it;

    cout << m_rooms.size() << " rooms defined" << endl;

    for(it = m_rooms.begin(); it != m_rooms.end(); it++) {

      string n = (*it).first;
      
      cout << "  " << n << endl;
    } 
  }

  Room* World::get( const string &n ) {

    room_map_t::iterator it = m_rooms.find(n);

    if(it == m_rooms.end()) {
      cerr << "could not find room: " << n << endl;
      exit(-1);
    }

    return &(*it).second;
  }

/*   void World::link_rooms( const string& from, const string& dir, const string &to) {
 * 
 *     Room *r1 = get( string(from) );
 *     Room *r2 = get( string(to) );
 * 
 *     if( dir == "south" ) { 
 *       r1->exit_south(r2);
 *       r2->exit_north(r1); 
 *       return;
 *     }
 * 
 *     if( dir == "north" ) {
 *       r1->exit_north(r2);
 *       r2->exit_south(r1);
 *       return; 
 *     }
 * 
 *     if( dir == "east" ) {
 *       r1->exit_east(r2);
 *       r2->exit_west(r1);
 *       return; 
 *     }
 * 
 *     if( dir == "west" ) {
 *       r1->exit_west(r2);
 *       r2->exit_east(r1);
 *       return; 
 *     }
 * 
 *     cout << "World::link_rooms: did not understand direction '" << dir << "'" << endl;
 * 
 *   }
 */

  void World::set_name( const string &s ) {
    m_name = s;
  }

  void World::set_author( const string &s ) {
    m_author = s;
  }

  void World::set_copyright( const string &s ) {
    m_copyright = s;
  }

}; // namespace ta
