
#pragma once

#include <string>
#include <map>
#include <boost/unordered_set.hpp>

#include "description.hh"
#include "interface.hh"
#include "item.hh"

namespace ta {
  
  class Room;

  class Exit {
    private:
      
      Description m_description;

      Room *m_target;
      bool  m_locked;
      
      std::string m_unlock_with;

    public:

      enum lock_response {
        LR_NOLOCK,
        LR_NOTLOCKED,
        LR_NOTKEY,
        LR_UNLOCKED
      };

      Exit();

      void set_lock( std::string& );
      bool islocked();
      void unlock();
      lock_response unlock( boost::unordered_set<std::string>& );
      std::string& locked_with();

      Description &description();
      void describe( Interface& );

      void set_target( Room* );
      Room* target();
  };
};

