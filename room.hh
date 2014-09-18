
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

      std::map<std::string, Exit> m_exits;

    public:

      Room( const std::string&, const std::vector<std::string>& ); 
      Room();

      std::string name();
      void describe( Interface&);
      
      std::map<std::string, Exit> & exits();

      boost::unordered_set<std::string>& items();
  };

}; // namespace ta

