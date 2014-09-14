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

    SDL_SetPalette( 
        m_screen, 
        SDL_LOGPAL,
        m_font->format->palette->colors,
        0,
        m_font->format->palette->ncolors 
    ); 
  }

  string Interface::wait_for_input() {

    SDL_Event event;

    m_buffer.clear();
    m_buffer_pos = m_buffer.begin();

    m_get_events = true;

    while(m_get_events) {

      while(SDL_PollEvent(&event)) {
        switch (event.type) {

          case SDL_KEYDOWN:

            handle_char( event.key.keysym.sym & 0x7F );

            break;

        case SDL_QUIT:
            exit(0);
            break;


        }
      }

      SDL_FillRect( m_screen, NULL, 0 );

      draw_buffer();
      draw_history();

      SDL_Flip(m_screen);
      SDL_Delay(50);
    }

    string ret;

    ret.resize( m_buffer.size() );

    int pos = 0;

    for( std::list<char>::iterator it = m_buffer.begin(); it != m_buffer.end(); it++) 
      ret[pos++] = *it;

    return ret;
  }

  void Interface::puts( int x, int y, const string &str ) {

    SDL_Rect s = { 0, 0, 7, 7 };
    SDL_Rect d = { (Sint16)x, (Sint16)y, 0, 0 };

    const char *c = str.c_str();

    while(*c) {

      s.x = ((*c - 32) & 0x0f) << 3;
      s.y = ((*c - 32) & 0xf0) >> 1;

      SDL_BlitSurface( m_font, &s, m_screen, &d );

      d.x += 8;

      c++; 
    }

  }

  void Interface::draw_history() {
    int pos = 460;

    list<string>::iterator it = m_history.begin();

    while( it != m_history.end() ) {

      puts( 0, pos, *it );

      pos -= 10;
      it++;
    }
  }

  void Interface::draw_buffer() {

    list<char>::iterator c = m_buffer.begin();

    SDL_Rect s = { 0,   0, 7, 7 };
    SDL_Rect d = { 0, 470, 0, 0 };

    while(c != m_buffer.end() ) {

      s.x = ((*c - 32) & 0x0f) << 3;
      s.y = ((*c - 32) & 0xf0) >> 1;

      SDL_BlitSurface( m_font, &s, m_screen, &d );

      d.x += 8;

      c++; 
    }

  }

  void Interface::puts( const string &s ) { 
    m_history.push_front(s);
  }

  void Interface::handle_char( int c ) {

    cout << "c=" << c << endl;

    switch(c) {
      case 0:
        break;

      case SDLK_RETURN:
        m_get_events = false;
        break;

      case SDLK_BACKSPACE:
        if( m_buffer.size() > 0 ) {
          m_buffer_pos--;
          m_buffer_pos = m_buffer.erase(m_buffer_pos);
        }
        break;

      default:
        m_buffer.insert( m_buffer_pos, (char)c );
        break;
    } 
  }

}; // namespace std

