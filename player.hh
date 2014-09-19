
#pragma once

#include <string>
#include <vector>
#include <map>

#include "interface.hh"
#include "exit.hh"
#include "item.hh"

namespace ta {

  class World;
  class Room;

  class Player {
  private:

    World *m_world;
    Room  *m_room;

    std::map<std::string, item_ptr> m_knapsack;

  public:
    
    Player(World*);

    void start_in( Room* );
    Room *current_room();

    void describe_room(Interface&);
    void describe_exit(Interface&, const std::string&);
    void describe_item(Interface&, const std::string&);

    void where( Interface& );
    void go( const std::vector<std::string>&, Interface&);
    void show_holding( Interface& );
    void unlock( const std::vector<std::string>&, Interface&);

    void pickup( const std::vector<std::string>&, Interface&);
    void putdown( const std::vector<std::string>&, Interface&);
  };

}; // namespace ta


