
#pragma once

#include <string>

namespace ta {

  class World;
  class Room;

  class Player {
  private:

    World *m_world;
    Room  *m_room;

  public:
    
    Player(World*);

    void start_in( std::string& );
    void start_in( char* );
    Room *current_room();
  };

}; // namespace ta


