
#pragma once

#include <string>
#include <vector>

#include "interface.hh"

namespace ta {

  class World;
  class Room;

  class Player {
  private:

    World *m_world;
    Room  *m_room;

    std::vector<std::string> m_holding;

  public:
    
    Player(World*);

    void start_in( Room* );
    Room *current_room();

    void describe(Interface&);
    void go(Interface&);
    void show_holding( Interface& );
    void use_item( Interface&);

    void pickup( Interface&);
    void putdown( Interface&);
  };

}; // namespace ta


