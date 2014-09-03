
#pragma once 

#include <map>
#include <string>

#include "interface.hh"
#include "input_handler.hh"
#include "room.hh"
#include "player.hh"
#include "world.hh"

namespace ta {

  class Engine {
    private:

      Interface m_interface;
      InputHandler m_input;

      Player m_player;
      World m_world;

      void do_list();

    public:

      Engine();
      void run();

      void build_room( char *, char * );
      void run_room( );

      
  };

}; // namespace ta;

