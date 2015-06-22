
#include "description.hh"

#include <list>
#include <string>

#include "interface.hh"

using namespace std;

namespace ta {

  Description::Description() { 
  }

  bool Description::empty() {
    return m_lines.size() == 0;
  }

  void Description::push_string( const string& s ) {

    struct description_line_s d = {
      s,
      LT_LINE
    };

    m_lines.push_back( d ); 
  }

  void Description::push_pause() {
    struct description_line_s d = {
      "",
      LT_PAUSE
    };

    m_lines.push_back( d ); 
  }

  void Description::show( Interface &i ) {
    
    bool has_paused = false;

    description_lines_t::iterator it;
    for( it = m_lines.begin(); it != m_lines.end(); it++ ) {

      if( (*it).type == LT_PAUSE ) {

        if( !has_paused ) {
          i.press_any_key();
          has_paused = true;
        }

        continue; 
      }

      i.puts( (*it).line );
      has_paused = false;
    }

    if( !has_paused )
      i.press_any_key();
  }

};
