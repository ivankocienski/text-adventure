
#include <cstdlib>
#include <string>
#include <map>
#include <iostream>

#include "loader.hh"
#include "engine.hh"
#include "room.hh"
#include "interface.hh"

using namespace std;

namespace ta {

  Engine::Engine(Interface &i) : m_interface(i), m_player( &m_world ) {

    Loader loader( &m_world, &m_player );

    loader.parse( "game.world" );
  }

  void Engine::announce() {

    m_world.introduce(m_interface);
  }

  void Engine::show_help() {

    cout 
      << "ADVENTURE (v0)" << endl
      << "commands help:" << endl
      << "  quit - exit" << endl
      //<< "  list - list current rooms in world" << endl
      << "  describe - where you are" << endl
      << endl;
  } 

  void Engine::handle_input() {

    if(m_interface.word(0) == "help") {
      show_help();
    }

    if(m_interface.word(0) == "holding" ) {
      m_player.show_holding(m_interface);
    }

    if(m_interface.word(0) == "pickup" ) {
      m_player.pickup( m_interface );
    }

    if(m_interface.word(0) == "putdown" ) {
      m_player.putdown( m_interface );
    }

    if(m_interface.word(0) == "use" ) {
      m_player.use_item( m_interface );
    } 

    if(m_interface.word(0) == "intro") {
      m_world.introduce( m_interface );
    }

    if(m_interface.word(0) == "describe") {
      m_player.describe( m_interface );
    }

    if(m_interface.word(0) == "go") {
      m_player.go( m_interface );
      m_player.describe( m_interface );
    }

    if(m_interface.word(0) == "quit") {
      //...
      exit(0);
    }
  }

  void Engine::run() {

    announce();

    while(true) {
      m_interface.wait_for_input();

      handle_input();
    }

  }
}
