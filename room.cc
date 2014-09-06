
#include "room.hh"
#include "interface.hh"

#include <string>
#include <sstream>
#include <vector>

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

    {
      stringstream sb;
      sb << "You are standing in " << m_name;
      i.puts( sb.str() );
    }

    vector<string>::iterator it;

    for(it = m_description.begin(); it != m_description.end(); it++) {
      i.puts(*it);
    } 
    
    if(m_exit_north) {
      stringstream sb;
      sb << "To the north there is " << m_exit_north->m_name;
      i.puts( sb.str() );
    }

    if(m_exit_east) {
      stringstream sb;
      sb << "To the east there is " << m_exit_east->m_name;
      i.puts( sb.str() );
    }

    if(m_exit_south) {
      stringstream sb;
      sb << "To the south there is " << m_exit_south->m_name;
      i.puts( sb.str() );
    }

    if(m_exit_west) {
      stringstream sb;
      sb << "To the west there is " << m_exit_west->m_name;
      i.puts( sb.str() );
    }
    
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
