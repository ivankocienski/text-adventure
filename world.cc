
#include <string>
#include <iostream>
#include <map>
#include <vector>

#include <stdlib.h>

#include "room.hh"
#include "world.hh"

using namespace std;

namespace ta {

  World::World() {
  }

  void World::build_room( const string &n, const vector<string> &dl ) {
    m_rooms[n] = Room(n, dl);
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

    if(it == m_rooms.end())
      return NULL;
    

    return &(*it).second;
  }

  void World::introduce( Interface &i ) {

    i.puts( m_name );
    i.puts( "by " + m_author );
    i.puts( "(c) " + m_copyright );
    i.puts( "" );

    m_description.show( i );

  }

  void World::set_name( const string &s ) {
    m_name = s;
  }

  void World::set_author( const string &s ) {
    m_author = s;
  }

  void World::set_copyright( const string &s ) {
    m_copyright = s;
  }

  Description& World::description() {
    return m_description;
  }

}; // namespace ta
