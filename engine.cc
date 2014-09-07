
#include <string>
#include <map>
#include <iostream>

#include "loader.hh"
#include "engine.hh"
#include "room.hh"
#include "input_handler.hh"
#include "interface.hh"

using namespace std;

namespace ta {

  Engine::Engine() : m_player( &m_world ) {
  }

  void Engine::run() {

    Loader loader( m_world, m_player );

    loader.read( "game.world" );

    m_world.introduce(m_interface);

    input_loop();
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

  void Engine::input_loop() {

    Room *room = m_player.current_room();

    room->describe(m_interface);

    while( true ) {

      m_input.read();

      if(m_input.word(0) == "help") {
        show_help();
      }

      if(m_input.word(0) == "holding" ) {
        m_player.show_holding(m_interface);
      }

      if(m_input.word(0) == "pickup" ) {
        m_player.pickup( m_interface, m_input.word(1) );
      }

      if(m_input.word(0) == "putdown" ) {
        m_player.putdown( m_interface, m_input.word(1) );
      }

      if(m_input.word(0) == "use" ) {
        m_player.use_item( m_interface, m_interface.word(1) );
      }


      if(m_input.word(0) == "intro") {
        m_world.introduce(m_interface);
      }

      //if(m_input.word(0) == "list") {
      //  m_world.list_rooms();
      //}

      if(m_input.word(0) == "describe") {
        m_player.describe( m_interface );
      }

      if(m_input.word(0) == "go") {
        m_player.go( m_interface, m_input.word(1) );
        m_player.describe( m_interface );
      }

      if(m_input.word(0) == "quit") break; 

    }

  }
}
