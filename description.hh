
#pragma once

#include <list>
#include <string>

#include "interface.hh"

namespace ta {
  
  class Description {
  private:

    enum line_type {
      LT_LINE,
      LT_PAUSE 
    };

    struct description_line_s {
      std::string line;
      line_type type;
    };

    std::list<description_line_s> m_lines;

  public:
    Description();

    void push_string( std::string& );
    void push_pause();

    void show( Interface& );
  };

};

