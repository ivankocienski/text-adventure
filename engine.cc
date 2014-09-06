
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

/*     m_world.build_room( "hallway", "a room of doom" );
 * 
 *     m_world.build_room( "bedroom", "cozy" );
 * 
 *     m_world.link_rooms( "hallway", "north", "bedroom" );
 * 
 *     m_player.start_in( "hallway" );
 * 
 */
    run_room( ); 
  }

  void Engine::show_help() {
    cout << "ADVENTURE (v0)" << endl;
    cout << "commands help:" << endl;
    cout << "  quit - exit" << endl;
    cout << "  list - list current rooms in world" << endl;
    cout << "  describe - where you are" << endl;
    cout << endl;
  } 

  void Engine::run_room() {

    Room *room = m_player.current_room();

    room->describe(m_interface);

    while( true ) {

      m_input.read();

      if(m_input.word(0) == "help") {
        show_help();
      }

      if(m_input.word(0) == "list") {
        m_world.list_rooms();
      }

      if(m_input.word(0) == "describe") {
        m_player.describe( m_interface );
      }

      if(m_input.word(0) == "go") {
        m_player.go( m_interface, m_input.word(1) );
      }

      if(m_input.word(0) == "quit") break; 

    }

  }
}
