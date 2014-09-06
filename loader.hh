
#pragma once

#include <string>
#include <boost/regex.hpp>

#include "world.hh"
#include "player.hh"

namespace ta {
  
  class Loader {
  private:

    typedef struct decleration {
      std::vector<std::string> parts;
      size_t line; 
      bool   assigned;
 
      decleration() { assigned = false; }
      std::string& value() { return parts[0]; }
    } decleration_t;

    typedef struct global_s {
      decleration_t name;
      decleration_t author;
      decleration_t copyright;
      decleration_t start;
    } global_t, *p_global_t;

    typedef struct room_s {

      decleration_t name; 
      decleration_t description; 
      decleration_t exit_north;
      decleration_t exit_south;
      decleration_t exit_east;
      decleration_t exit_west; 
    } room_t, *p_room_t;

    typedef std::map<std::string, room_t> room_map_t;

    global_t   m_globals;
    room_map_t m_rooms;
    

    World  &m_world;
    Player &m_player;
    int     m_warnings;

    enum read_state { 
      RS_TOPLEVEL,
      RS_ROOM,
      RS_DESCRIPTION 
    };

    void moan( size_t, const std::string& );
    void set_decleration( decleration_t&, boost::smatch&, size_t);

    void parse( const std::string& ); 
    void upload();

  public:
    Loader( World&, Player& );

    void read( const std::string& );
  };

}; // namespace ta

