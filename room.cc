
#include "room.hh"
#include "interface.hh"

#include <string>
#include <vector>
#include <algorithm>
#include <boost/unordered_set.hpp>

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

      boost::unordered_set<string>::iterator it;

      i.puts( "You can see" );
      for(it = m_items.begin(); it != m_items.end(); it++ ) {
        i.puts( "  " + *it );
      }
    }
    
  }

  boost::unordered_set<string> & Room::items() {
    return m_items;
  }

}; // namespace ta
