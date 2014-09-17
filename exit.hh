
#pragma once

#include "description.hh"
#include "interface.hh"

namespace ta {
  
  class Room;

  class Exit {
    private:
      
      Description m_description;

      Room *m_target;

    public:

      Exit();
      bool isset();

      Description &description();
      void describe( Interface& );

      void set_target( Room* );
      Room* target();
  };
};

