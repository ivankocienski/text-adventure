
#pragma once

#include <string>
#include <vector>
#include <boost/unordered_set.hpp>

#include "interface.hh"
#include "description.hh"
#include "exit.hh"

namespace ta { 

  class Room {

    private:

      //Description m_description;

      std::string m_name;
      std::vector<std::string> m_description;
      
      boost::unordered_set<std::string> m_items;

      Exit m_exit_north;
      Exit m_exit_south;
      Exit m_exit_east;
      Exit m_exit_west;

    public:

      Room( const std::string&, const std::vector<std::string>& ); 
      Room();

      std::string name();
      void describe( Interface&);
      
      Exit & exit_north();
      Exit & exit_south();
      Exit & exit_east();
      Exit & exit_west();

      boost::unordered_set<std::string>& items();
  };

}; // namespace ta

