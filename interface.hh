
#pragma once

#include <string>
#include <list>
#include <vector>
#include <SDL/SDL.h>

namespace ta {
  
  class Interface {
    private:

      SDL_Surface* m_screen;
      SDL_Surface* m_font;
      bool m_get_events;

      void handle_char(int);
      void break_buffer_into_words();
      void draw_buffer();

      std::list<char> m_buffer;
      std::list<char>::iterator m_buffer_pos;

      std::vector<std::string> m_words;

    public:

      Interface();
      ~Interface();

      void init(int, int);

      void puts( const std::string& );
      void puts( int, int, const std::string& );

      void wait_for_input();
      std::string word(int);
  };

}; // namespace ta;
