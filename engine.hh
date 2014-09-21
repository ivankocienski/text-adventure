
#pragma once 

#include <string>
#include <vector>

#include "player.hh"
#include "world.hh"
#include "interface.hh"

namespace ta {

  class Engine {
    private:

      Interface  &m_interface;
      Player      m_player;
      World       m_world;

      bool        m_game_running;

      void handle_input( const std::vector<std::string>& );

    public:

      Engine(Interface&);

      // reset game to start
      void reset();

      // the main game loop
      void run();

      // has the game ended, did the player win/die?
      bool is_running();
  };

}; // namespace ta;

