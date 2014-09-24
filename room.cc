
#include "room.hh"
#include "interface.hh"

#include <string>
#include <vector>
#include <algorithm>
#include <boost/unordered_set.hpp>

using namespace std;

namespace ta {

  Room::Room( const string &n ) : m_name(n) {
  }

  Room::Room() { 
  }

  string Room::name() {
    return m_name;
  }

  Room::exit_map_t & Room::exits() {
    return m_exits;
  }
  
  bool Room::has_exit( const string &dir ) {
    return m_exits.count(dir) > 0;
  }

  Description& Room::description() {
    return m_description;
  }

  void Room::describe( Interface &i ) {

    m_description.show(i);

    if( m_exits.size() ) {
      map<string, Exit>::iterator it;

      i.puts( 7, "You can go" );
      for( it = m_exits.begin(); it != m_exits.end(); it++ ) {
        i.puts( 7, "  " + (*it).first );
      }
    }

    if( m_inventory.size() ) {

      inventory_t::iterator it;

      i.puts( "You can see" );
      for(it = m_inventory.begin(); it != m_inventory.end(); it++ ) {
        string name = *it;

        i.puts( "  " + name );
      }
    }
    
  }

  void Room::add_item_to_inventory( const string &s ) {
    m_inventory.insert(s);
  }

  bool Room::is_item_in_inventory( const string &w ) {
    return m_inventory.count(w) == 1; 
  }

  void Room::remove_item_from_inventory( const string &w ) {
    m_inventory.erase(w); 
  }


}; // namespace ta
