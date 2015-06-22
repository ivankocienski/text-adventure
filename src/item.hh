
#pragma once

#include <string>

#include "description.hh"
#include "interface.hh"


namespace ta {

  class Room;
  
  class Item {
  private:

    std::string m_name;
    Description m_description;

    Room  *m_in_room;
    
  public:

    Item(); 
    Item(const std::string&);

    Description& description();
    std::string & name();
    void describe( Interface& );

    void set_room( Room* );
  };


}; // namepspace ta;

