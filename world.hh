
#pragma once

#include <string>
#include <map>
#include "room.hh"

namespace ta {
  
  class World {
  private:

    typedef std::map<std::string, Room> room_map_t;

    room_map_t m_rooms;

  public:
    World();

    void build_room( char*, char* );
    void list_rooms();
    Room* get(std::string&);
  };

}; // namespace ta

