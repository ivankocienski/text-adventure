
#include "room.hh"
#include "interface.hh"

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

namespace ta {

  Room::Room( const string &n, const vector<string> &dl ) : m_name(n), m_description(dl) {
  }

  Room::Room() { 
  }

  string Room::name() {
    return m_name;
  }

  map<string, Exit> & Room::exits() {
    return m_exits;
  }

  void Room::describe( Interface &i ) {

    {
      vector<string>::iterator it;

      for(it = m_description.begin(); it != m_description.end(); it++) {
        i.puts(*it);
      } 
    }
    
    if( m_exits.size() ) {
      map<string, Exit>::iterator it;

      i.puts( 7, "You can go" );
      for( it = m_exits.begin(); it != m_exits.end(); it++ ) {
        i.puts( 7, "  " + (*it).first );
      }
    }

    if( m_items.size() ) {

      map<string, item_ptr>::iterator it;

      i.puts( "You can see" );
      for(it = m_items.begin(); it != m_items.end(); it++ ) {
        string name = (*it).first;

        i.puts( "  " + name );
      }
    }
    
  }

  void Room::place_item( item_ptr& i ) {
    m_items[i->name()] = i;
  }

  bool Room::has_item( const string &n ) {
    return m_items.count(n) > 0;
  }

  void Room::remove_item( const string &n ) {
    m_items.erase( n );
  }


}; // namespace ta
