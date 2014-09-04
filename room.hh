
#pragma once

#include <string>
#include "interface.hh"

namespace ta {

  class Room {

    private:

      std::string m_name;
      std::string m_description;

      Room *m_exit_north;
      Room *m_exit_south;
      Room *m_exit_east;
      Room *m_exit_west;

    public:

      Room( const std::string&, const std::string& ); 
      Room();

      std::string name();
      void describe( Interface&);

      void exit_north( Room* );
      void exit_south( Room* );
      void exit_east( Room* );
      void exit_west( Room* );


      Room* get_north_exit();
      Room* get_south_exit();
      Room* get_east_exit();
      Room* get_west_exit();
  };

}; // namespace ta

