
#pragma once

#include <string>
#include <vector>
#include <boost/unordered_set.hpp>

#include "interface.hh"
#include "description.hh"
#include "exit.hh"

namespace ta { 

  class Room {

    public:

      typedef std::map<std::string, Exit> exit_map_t;

    private:

      //Description m_description;

      
      std::string m_name;
      std::vector<std::string> m_description;
      
      boost::unordered_set<std::string> m_inventory;

      exit_map_t m_exits;

    public:

      Room( const std::string&, const std::vector<std::string>& ); 
      Room();

      std::string name();
      void describe( Interface&);
      
      bool has_exit( const std::string& );
      exit_map_t & exits();

      void add_item_to_inventory( const std::string& );
      bool is_item_in_inventory( const std::string& );
      void remove_item_from_inventory( const std::string & );
  };

}; // namespace ta

