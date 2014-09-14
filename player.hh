
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
    void go( const std::vector<std::string>&, Interface&);
    void show_holding( Interface& );
    void use_item( const std::vector<std::string>&, Interface&);

    void pickup( const std::vector<std::string>&, Interface&);
    void putdown( const std::vector<std::string>&, Interface&);
  };

}; // namespace ta


