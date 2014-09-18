
#pragma once

#include <string>
#include <vector>
#include <boost/unordered_set.hpp>

#include "interface.hh"
#include "exit.hh"

namespace ta {

  class World;
  class Room;

  class Player {
  private:

    World *m_world;
    Room  *m_room;

    boost::unordered_set<std::string> m_knapsack;

    void try_go( Interface&, Exit&, const std::string & );
    void try_unlock( Interface&, Exit&, const std::string& );

  public:
    
    Player(World*);

    void start_in( Room* );
    Room *current_room();

    void where( Interface& );
    void describe(Interface&);
    void go( const std::vector<std::string>&, Interface&);
    void show_holding( Interface& );
    void unlock( const std::vector<std::string>&, Interface&);

    void pickup( const std::vector<std::string>&, Interface&);
    void putdown( const std::vector<std::string>&, Interface&);
  };

}; // namespace ta


