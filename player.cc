
#include <string>
#include <sstream>

#include "interface.hh"
#include "player.hh"
#include "world.hh"
#include "room.hh"

using namespace std;

namespace ta {

  Player::Player(World *w) {
    m_world = w;
  }

  void Player::start_in( Room *r ) {
    m_room = r;
  }

  Room* Player::current_room() {
    return m_room;
  }

  void Player::describe( Interface &i ) {
    m_room->describe(i);
  }

  void Player::go( Interface &i, const string &dir ) {

    if(dir.empty()) {
      i.puts( "Which direction should I go in, sir?" );
      return;
    }

    if( dir == "north" ) {

      Room *r = m_room->get_north_exit();
      if( !r ) {
        i.puts( "There is no north exit" );

      } else {
        m_room = r;
      }

      return; 
    }

    if( dir == "south" ) {

      Room *r = m_room->get_south_exit();
      if( !r ) {
        i.puts( "There is no south exit" );

      } else {
        m_room = r;
      }

      return; 
    }

    if( dir == "east" ) {

      Room *r = m_room->get_east_exit();
      if( !r ) {
        i.puts( "There is no east exit" );

      } else {
        m_room = r;
      }

      return; 
    }

    if( dir == "west" ) {

      Room *r = m_room->get_west_exit();
      if( !r ) {
        i.puts( "There is no west exit" );

      } else {
        m_room = r;
      }

      return; 
    }

    stringstream ss;

    ss << "I do not know direction '" << dir << "'";

    i.puts( ss.str() );
  }

}; // namespace ta;
