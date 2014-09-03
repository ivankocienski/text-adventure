
#include <string>
#include <map>
#include <iostream>

#include "engine.hh"
#include "room.hh"
#include "input_handler.hh"
#include "interface.hh"

using namespace std;

namespace ta {

  Engine::Engine() : m_player( &m_world ) {
  }

  void Engine::run() {

    m_world.build_room( "hallway", "a room of doom" );

    m_world.build_room( "bedroom", "cozy" );

    m_player.start_in( "hallway" );

    run_room( ); 
  }



  void Engine::run_room() {

    Room *room = m_player.current_room();

    room->describe(m_interface);

    while( true ) {

      m_input.read();

      if(m_input.word(0) == "list") {
        m_world.list_rooms();
      }

      if(m_input.word(0) == "quit") break; 

    }

  }
}
