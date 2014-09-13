
#pragma once

#include <string>
#include <SDL/SDL.h>

namespace ta {
  
  class Interface {
    private:

      SDL_Surface* m_screen;
      SDL_Surface* m_font;

      void handle_char(char);

    public:

      Interface();
      ~Interface();

      void init(int, int);

      void puts( const char* );
      void puts( const std::string& );

      void wait_for_input();
      std::string word(int);
  };

}; // namespace ta;
