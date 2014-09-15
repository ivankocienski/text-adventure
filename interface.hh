
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
      bool m_repaint;

      void handle_char(int);
      void draw_buffer();
      void draw_history();

      std::list<char> m_buffer;
      std::list<char>::iterator m_buffer_pos;

      std::list<std::string> m_history;

      void draw_box( int, int, int, int );

    public:

      Interface();
      ~Interface();

      void init(int, int);

      void puts( const std::string& );
      void puts( int, int, const std::string& );

      std::string wait_for_input();
  };

}; // namespace ta;
