
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

  void Room::describe( Interface &i ) {

    i.puts( "You are standing in " + m_name );

    {
      vector<string>::iterator it;

      for(it = m_description.begin(); it != m_description.end(); it++) {
        i.puts(*it);
      } 
    }
    
    if(m_exit_north.isset()) {
      i.puts( "To the north there is " + m_exit_north.target()->name() );
    }

    if(m_exit_east.isset()) {
      i.puts( "To the east there is " + m_exit_east.target()->name() );
    }

    if(m_exit_south.isset()) {
      i.puts( "To the south there is " + m_exit_south.target()->name() );
    }

    if(m_exit_west.isset()) {
      i.puts( "To the west there is " + m_exit_west.target()->name() );
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

  Exit & Room::exit_north() {
    return m_exit_north;
  }

  Exit & Room::exit_south() {
    return m_exit_south;
  }

  Exit & Room::exit_east() {
    return m_exit_east;
  }

  Exit & Room::exit_west() {
    return m_exit_west;
  }
  

}; // namespace ta
