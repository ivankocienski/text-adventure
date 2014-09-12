
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

    std::vector<std::string> m_holding;

  public:
    
    Player(World*);

    void start_in( Room* );
    Room *current_room();

    void describe(Interface&);
    void go(Interface&, const std::string& );
    void show_holding( Interface& );
    void use_item( Interface&, const std::string& );

    void pickup( Interface&, const std::string& );
    void putdown( Interface&, const std::string& );
  };

}; // namespace ta


