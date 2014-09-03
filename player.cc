
#include <string>

#include "interface.hh"
#include "player.hh"
#include "world.hh"
#include "room.hh"

using namespace std;

namespace ta {

  Player::Player(World *w) {
    m_world = w;
  }

  void Player::start_in(string& w) {
    m_room = m_world->get(w); 
  }

  void Player::start_in(char * w) {

    string s(w);

    start_in(s);
  }

  Room* Player::current_room() {
    return m_room;
  }

  void Player::describe( Interface &i ) {
    m_room->describe(i);
  }
}; // namespace ta;
