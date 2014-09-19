
#pragma once

#include <string>
#include <vector>

#include "interface.hh"
#include "description.hh"
#include "exit.hh"
#include "item.hh"

namespace ta { 

  class Room {

    private:

      //Description m_description;

      
      std::string m_name;
      std::vector<std::string> m_description;
      
      std::map<std::string, item_ptr> m_items;

      std::map<std::string, Exit> m_exits;

    public:

      Room( const std::string&, const std::vector<std::string>& ); 
      Room();

      std::string name();
      void describe( Interface&);
      
      std::map<std::string, Exit> & exits();

      void place_item( item_ptr& );
      bool has_item( const std::string& );
      void remove_item( const std::string & );
  };

}; // namespace ta

