
#pragma once

#include <string>
#include <vector>
#include <boost/unordered_set.hpp>

#include "interface.hh"
#include "description.hh"
#include "exit.hh"
#include "character.hh"

namespace ta { 

  class Room {

    public:

      typedef std::map<std::string, Exit> exit_map_t;
      typedef boost::unordered_set<std::string> inventory_t;
      typedef boost::unordered_set<std::string> character_set_t;

    private:

      Description      m_description;
      std::string      m_name;
      inventory_t      m_inventory;
      exit_map_t       m_exits;
      character_set_t  m_characters;

    public:

      Room( const std::string& ); 
      Room();

      std::string name();
      Description& description();
      void describe( Interface&);
      
      bool has_exit( const std::string& );
      exit_map_t & exits();

      void add_item_to_inventory( const std::string& );
      bool is_item_in_inventory( const std::string& );
      void remove_item_from_inventory( const std::string & );

      void place_character( const std::string& );
      bool has_character( const std::string& );
  };

}; // namespace ta

