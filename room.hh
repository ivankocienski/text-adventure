
#pragma once

#include <string>
#include <vector>

#include "interface.hh"
#include "description.hh"

namespace ta {


  class Room {

    private:

      //Description m_description;

      std::string m_name;
      std::vector<std::string> m_description;
      std::vector<std::string> m_items;

      Room *m_exit_north;
      Room *m_exit_south;
      Room *m_exit_east;
      Room *m_exit_west;

    public:

      Room( const std::string&, const std::vector<std::string>& ); 
      Room();

      std::string name();
      void describe( Interface&);

      void exit_north( Room* );
      void exit_south( Room* );
      void exit_east( Room* );
      void exit_west( Room* );

      void place_item(   const std::string& );
      bool has_item(     const std::string& );
      void discard_item( const std::string& );


      Room* get_north_exit();
      Room* get_south_exit();
      Room* get_east_exit();
      Room* get_west_exit();
  };

}; // namespace ta

