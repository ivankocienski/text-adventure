
#pragma once 

#include <string>
#include <vector>

#include "player.hh"
#include "world.hh"
#include "interface.hh"

namespace ta {

  class Engine {
    private:

      Interface &m_interface;
      Player     m_player;
      World      m_world;

      void show_help(); 
      void announce(); 
      void handle_input( const std::vector<std::string>& );

    public:

      Engine(Interface&);

      void run();
  };

}; // namespace ta;

