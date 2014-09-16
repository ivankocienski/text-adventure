
#pragma once

#include <boost/shared_ptr.hpp>
#include <string>
#include <list>
#include <vector>

#include "sdl/event.hh"
#include "sdl/screen.hh"
#include "sdl/surface.hh"

namespace ta {
  
  class Interface {
    private:

      class Font {

        private:

        boost::shared_ptr<SDL::Screen> m_screen;
        SDL::Surface  m_surface;

        public:

        Font();
        Font( boost::shared_ptr<SDL::Screen>& );
        Font( boost::shared_ptr<SDL::Screen>&, SDL::Surface& );

        bool load( const std::string& );
        void clone_remap_color( SDL_Surface*, int );
        void load_palette();

        void remap_color_to( int );

        void puts( int, int, const std::string& ); 
        void put_char( int, int, int );
        void draw_box( int, int, int, int );
      };

      struct history_s {
        std::string string;
        Font &font;
      };

      boost::shared_ptr<SDL::Screen> m_screen;
      
      std::vector<Font> m_fonts;

      Font m_debug_font;

      bool m_get_events;
      bool m_repaint;

      void handle_char(int);
      void draw_buffer();
      void draw_history();

      std::list<char> m_buffer;
      std::list<char>::iterator m_buffer_pos;

      std::list<struct history_s> m_history;

      void draw_box( int, int, int, int );

    public:

      enum {
        C_BLACK,
        C_RED,
        C_YELLOW,
        C_GREEN,
        C_BLUE,
        C_DARK_GREY,
        C_LIGHT_GREY,
        C_WHITE
      };

      Interface();
      ~Interface();

      void init(int, int);

      void puts( int, const std::string& );
      void puts( const std::string& );

      std::string wait_for_input();
  };

}; // namespace ta;
