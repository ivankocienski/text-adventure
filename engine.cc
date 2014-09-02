
#include <string>

#include "engine.hh"
#include "room.hh"
#include "input_handler.hh"
#include "interface.hh"

namespace ta {

  Engine::Engine() {
  }

  void Engine::run() {

    Room room;

    while( true ) {
      room.describe(m_interface);

      m_input.read();

      if(m_input.word(0) == "quit") break; 

    }

  }
}
