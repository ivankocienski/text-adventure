
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "interface.hh"
#include "player.hh"
#include "world.hh"
#include "room.hh"
#include "exit.hh"
#include "item.hh"

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

  void Player::describe_room( Interface &i ) {
    m_room->describe(i);
  }

  void Player::describe_exit( Interface &i, const string &w ) {

    if( m_room->exits().count(w) == 0 ) {
      i.puts( 1, "Room does not have exit " + w );
      return;
    }

    m_room->exits()[w].describe(i); 
  }

  //can only describe items in room :(
  void Player::describe_item( Interface &i, const string &w ) {

    if( !m_room->is_item_in_inventory( w ) ) {
      i.puts( 1, "Room does not have exit " + w );
      return;
    }

    m_world->get_item( w ).describe(i);
  }

  void Player::describe_person( Interface &i, const string &w ) {

    if( !m_room->has_character( w ) ) {
      i.puts( 1, "That person is not in this room");
      return;
    }

    m_world->get_character( w ).describe(i);
  }

  void Player::go( const std::vector<std::string>& words, Interface &i ) {

    if( words.size() < 2 ) {
      i.puts( 1, "Which direction should I go in, sir?" );
      return;
    }

    string dir = words[1];

    if( !m_room->has_exit(dir) ) {
      i.puts( 1, "I do not know direction " + dir );
      return; 
    }

    Exit & exit = m_room->exits()[dir];

    if( exit.islocked() ) {
      i.puts( 1, "This door is locked, you need " + exit.locked_with() + " to unlock" );
      return;
    }

    m_room = exit.target();
    where(i); 
  }

  void Player::show_holding( Interface &i ) {

    if( m_knapsack.size() == 0 ) {
      i.puts( "You are not holding anything" );
      return;
    }

    boost::unordered_set<string>::iterator it;

    i.puts( "You are holding" );
    for( it = m_knapsack.begin(); it != m_knapsack.end(); it++ ) {
      i.puts( "  " + *it ); 
    }
  }

  void Player::pickup( const std::vector<std::string>& words, Interface &i ) {

    if( words.size() < 2 ) {
      i.puts( 1, "What should be picked up?" );
      return;
    }

    string what = words[1];

    if( m_knapsack.count( what ) ) {
      i.puts( 1, "You are already holding " + what );
      return;
    }

    if( !m_room->is_item_in_inventory( what ) ) {
      i.puts( 1, "Could not find '" + what + "' to pick up" );
      return;
    }

    m_world->get_item(what).set_room( NULL );
    m_knapsack.insert( what );

    i.puts( 7, "You have picked up " + what );
  }

  void Player::putdown( const std::vector<std::string>& words, Interface &i ) {

    if( words.size() < 2 ) {
      i.puts( 1, "What should be put down?" );
      return;
    }

    string what = words[1];

    if( m_knapsack.count( what ) == 0 ) {
      i.puts( 1, "You are not holding that" );
      return;
    }

    if( m_room->is_item_in_inventory( what ) ) {
      i.puts( 1, "There is already a " + what + " in this room" );
      return;
    }

    m_world->get_item(what).set_room( m_room );
    m_knapsack.erase( what );

    i.puts( 7, "You have put down " + what );

  }

  void Player::give_item( const string &what ) {
    m_world->get_item(what).set_room( NULL );
    m_knapsack.insert( what );
  }

  const Player::knapsack_t & Player::knapsack() const {
    return m_knapsack;
  }

  void Player::where( Interface &i ) {
    i.puts( 7, "You are standing in " + m_room->name() ); 
  }

  void Player::unlock( const std::vector<std::string>& words, Interface &i ) {

    if( words.size() < 2 ) {
      i.puts( 1, "What should I unlock, sir?" );
      return;
    }

    string dir = words[1];

    if( m_room->exits().count(dir) == 0 ) { 
      i.puts( 1, "Did not understand direction " + dir );
      return;
    }

    Exit & exit = m_room->exits()[dir];

    switch( exit.unlock( m_knapsack ) ) {

      case Exit::LR_NOTLOCKED:
        i.puts( 1, "That door was not locked" );
        break;

      case Exit::LR_NOLOCK:
        i.puts( 1, "That door does not have a lock" );
        break;

      case Exit::LR_NOTKEY:
        i.puts( 1, "You do not have the key for the " + dir + " door" );
        break;

      case Exit::LR_UNLOCKED:
        i.puts( 7, "The " + dir + " door has been unlocked" );
        break;
    } 
  }

  void Player::talk( const vector<string> &words, Interface &i ) {
    if( words.size() < 2 ) {
      i.puts( 1, "Who should I talk to, sir?" );
      return; 
    }

    string name = words[1];

    if( !m_room->has_character(name) ) {
      i.puts( 1, "Could not find them in this room" );
      return;
    }

    m_world->get_character( name ).talk( i );

  }
}; // namespace ta;
