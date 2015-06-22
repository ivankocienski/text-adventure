
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

    typedef std::list<description_line_s> description_lines_t;

    description_lines_t m_lines;

  public:
    Description();

    bool empty();
    void push_string( const std::string& );
    void push_pause();

    void show( Interface& );
  };

};

