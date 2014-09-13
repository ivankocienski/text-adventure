#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <cstdlib>
#include <iostream>
#include <string>

#include "interface.hh"

using namespace std;

namespace ta {
  
  Interface::Interface() { 
    m_screen = NULL;
    m_font   = NULL;
  }

  Interface::~Interface() {

    if(m_font) 
      SDL_FreeSurface(m_font);

    if(m_screen) 
      SDL_Quit();
  }

  void Interface::init(int w, int h) { 

    m_screen = SDL_SetVideoMode(w, h, 8, SDL_SWSURFACE); 
    if( !m_screen ) {
      cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
      exit(-1);
    }

    m_font = IMG_Load( "font.png" );
    if( !m_font ) {
      cerr << "Failed to load font: " << SDL_GetError() << endl;
      exit(-1);
    }

  }

  void Interface::wait_for_input() {

    SDL_Event event;

    while(true) {

      while(SDL_PollEvent(&event)) {
        switch (event.type) {

          case SDL_KEYDOWN:

            if((event.key.keysym.unicode & 0xFF80) == 0 ) 
              handle_char( event.key.keysym.unicode & 0x7F );

            break;

        case SDL_QUIT:
            exit(0);
            break;


        }
      }

      SDL_Flip(m_screen);
      SDL_Delay(50);
    }
  }

  void Interface::puts( const char *s ) {
    cout << s << endl;
  }

  void Interface::puts( const string &s ) { 
    cout << s << endl;
  }

  string Interface::word( int i ) {
    return "";
  }

  void Interface::handle_char( char c ) {
    
  }

}; // namespace std

