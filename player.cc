
#include <string>
#include <vector>
#include <algorithm>

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

  void Player::go( const std::vector<std::string>& words, Interface &i ) {

    string dir = words[1];

    if(dir.empty()) {
      i.puts( "Which direction should I go in, sir?" );
      return;
    }

    if( dir == "north" ) {

      if( m_room->exit_north().isset() ) {
        m_room = m_room->exit_north().target();

      } else 
        i.puts( 1, "There is no north exit" ); 

      return; 
    }

    if( dir == "south" ) {

      if( m_room->exit_south().isset() ) {
        m_room = m_room->exit_south().target();

      } else 
        i.puts( 1, "There is no south exit" );

      return; 
    }

    if( dir == "east" ) {

      if( m_room->exit_east().isset() ) {
        m_room = m_room->exit_east().target();

      } else 
        i.puts( 1, "There is no east exit" );

      return; 
    }

    if( dir == "west" ) {

      if( m_room->exit_west().isset() ) {
        m_room = m_room->exit_west().target();

      } else
        i.puts( 1, "There is no west exit" );

      return; 
    }

    i.puts( 1, "I do not know direction '" + dir + "'" );
  }

  void Player::show_holding( Interface &i ) {

    if( m_holding.size() == 0 ) {
      i.puts( "You are not holding anything" );
      return;
    }

    vector<string>::iterator it;

    i.puts( "You are holding" );
    for( it = m_holding.begin(); it != m_holding.end(); it++ ) {
      i.puts( "  " + *it ); 
    }
  }

  void Player::pickup( const std::vector<std::string>& words, Interface &i ) {

    string what = words[1];

    if( !m_room->has_item( what ) ) {
      i.puts( "could not find '" + what + "' to pick up" );
      return;
    }

    m_room->discard_item( what ); 

    m_holding.push_back( what );
  }

  void Player::putdown( const std::vector<std::string>& words, Interface &i ) {

    string what = words[1];

    if( what == "" ) {
      i.puts( "Pick up what, sir?" );
      return;
    }

    vector<string>::iterator it;

    it = find( m_holding.begin(), m_holding.end(), what );

    if( it == m_holding.end() ) {
      i.puts( "You are not holding that" );
      return;
    }

    m_holding.erase( it );

    m_room->has_item( what );

  }

  void Player::use_item( const std::vector<std::string>& words, Interface &i ) {

  }

}; // namespace ta;
