
#include "room.hh"
#include "interface.hh"

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

namespace ta {

  Room::Room( const string &n, const vector<string> &dl ) : m_name(n), m_description(dl) {

    m_exit_north = NULL;
    m_exit_south = NULL;
    m_exit_east  = NULL;
    m_exit_west  = NULL;
  }

  Room::Room() { 

    m_exit_north = NULL;
    m_exit_south = NULL;
    m_exit_east  = NULL;
    m_exit_west  = NULL;
  }


  string Room::name() {
    return m_name;
  }

  void Room::describe( Interface &i ) {

    i.puts( "You are standing in " + m_name );

    vector<string>::iterator it;

    for(it = m_description.begin(); it != m_description.end(); it++) {
      i.puts(*it);
    } 
    
    if(m_exit_north) {
      i.puts( "To the north there is " + m_exit_north->m_name );
    }

    if(m_exit_east) {
      i.puts( "To the east there is " + m_exit_east->m_name );
    }

    if(m_exit_south) {
      i.puts( "To the south there is " + m_exit_south->m_name );
    }

    if(m_exit_west) {
      i.puts( "To the west there is " + m_exit_west->m_name );
    }

    if(m_items.size() ) {
      i.puts( "In room you can see" );
      for(it = m_items.begin(); it != m_items.end(); it++ ) {
        i.puts( "  " + *it );
      }
    }
    
  }

  void Room::place_item( const string &s ) {
    m_items.push_back(s);
  }

  bool Room::has_item( const string &s ) {
    vector<string>::iterator it;

    it = find( m_items.begin(), m_items.end(), s ); 

    return it != m_items.end();
  }

  void Room::discard_item( const string &s ) {

    vector<string>::iterator it;

    it = find( m_items.begin(), m_items.end(), s );

    if( it == m_items.end() )
      return;

    m_items.erase(it);
  }

  void Room::exit_north( Room* r ) {
    m_exit_north = r; 
  }

  void Room::exit_south( Room* r ) {
    m_exit_south = r; 
  }

  void Room::exit_east( Room* r ) {
    m_exit_east = r; 
  }

  void Room::exit_west( Room* r ) {
    m_exit_west = r; 
  }
  
  Room* Room::get_north_exit() {
    return m_exit_north; 
  }

  Room* Room::get_south_exit() {
    return m_exit_south; 
  }

  Room* Room::get_east_exit() {
    return m_exit_east; 
  }

  Room* Room::get_west_exit() {
    return m_exit_west;
  }


}; // namespace ta
