
#pragma once

#include <string>

#include "interface.hh"

namespace ta {

  class World;
  class Room;

  class Player {
  private:

    World *m_world;
    Room  *m_room;

  public:
    
    Player(World*);

    void start_in( Room* );
    Room *current_room();

    void describe(Interface&);
    void go(Interface&, const std::string& );
  };

}; // namespace ta


