
#pragma once

#include <string>
#include "interface.hh"

namespace ta {

  class Room {

    private:

      std::string m_description;
      std::string m_name;

    public:

      Room( char* ); 
      Room();

      std::string name();
      void describe( Interface&);
  };

}; // namespace ta

