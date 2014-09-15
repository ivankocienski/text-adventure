#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <cctype>
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
    m_repaint = true;

    while(m_get_events) {

      while(SDL_PollEvent(&event)) {
        switch (event.type) {

          case SDL_KEYDOWN:

            handle_char( event.key.keysym.sym );

            break;

        case SDL_QUIT:
            exit(0);
            break;


        }
      }

      if(m_repaint) {
        SDL_FillRect( m_screen, NULL, 0 );

        puts( 31, 0, "A D V E N T U R E" );

        draw_buffer();
        draw_history();
        
        SDL_Flip(m_screen);

        m_repaint = false;
      }

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

    SDL_Rect s = { 0, 0, 8, 8 };
    SDL_Rect d = { (Sint16)x * 8, (Sint16)y * 8, 0, 0 };

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
    int pos = 54;

    list<string>::iterator it = m_history.begin();

    draw_box( 0, 2, 79, 52 );

    while( it != m_history.end() ) {
      puts( 1, pos, *it );
      pos--;
      it++;
    }
  }

  void Interface::draw_buffer() {

    list<char>::iterator c = m_buffer.begin();

    SDL_Rect s = { 0,   0, 8, 8 };
    SDL_Rect d = { 8, 463, 0, 0 };

    draw_box( 0, 57, 79, 2 );

    while(c != m_buffer.end() ) {

      s.x = ((*c - 32) & 0x0f) << 3;
      s.y = ((*c - 32) & 0xf0) >> 1;

      SDL_BlitSurface( m_font, &s, m_screen, &d );

      d.x += 8;

      c++; 
    }
  }

  void Interface::draw_box( int x, int y, int w, int h ) {

    SDL_Rect top_l = {  0, 48, 8, 8 };
    SDL_Rect top   = {  8, 48, 8, 8 };
    SDL_Rect top_r = { 16, 48, 8, 8 };
    SDL_Rect left  = { 24, 48, 8, 8 };
    SDL_Rect right = { 24, 56, 8, 8 };
    SDL_Rect bot_l = {  0, 56, 8, 8 };
    SDL_Rect bot   = {  8, 56, 8, 8 };
    SDL_Rect bot_r = { 16, 56, 8, 8 };

    SDL_Rect dst;
    
    int i; 

    dst.y = y * 8;
    dst.x = x * 8;
    SDL_BlitSurface( m_font, &top_l, m_screen, &dst );

    dst.x = (x + w) * 8; 
    SDL_BlitSurface( m_font, &top_r, m_screen, &dst );

    dst.x = x * 8;
    dst.y = (y + h) * 8;
      SDL_BlitSurface( m_font, &bot_l, m_screen, &dst );
    
    dst.x = (x + w) * 8;
    SDL_BlitSurface( m_font, &bot_r, m_screen, &dst );

    dst.x = x * 8 + 8;
    for(i = 0; i < w - 1; i++ ) {
      dst.y = y * 8;
      SDL_BlitSurface( m_font, &top, m_screen, &dst );
      
      dst.y = (y + h) * 8;
      SDL_BlitSurface( m_font, &bot, m_screen, &dst );
      
      dst.x += 8;
    }

    dst.y = y * 8 + 8;
    for( i = 0; i < h - 1; i++ ) {

      dst.x = x * 8;
      SDL_BlitSurface( m_font, &left, m_screen, &dst );

      dst.x = (x + w) * 8;
      SDL_BlitSurface( m_font, &right, m_screen, &dst ); 

      dst.y += 8; 
    }


  }

  void Interface::puts( const string &s ) { 
    m_history.push_front(s);
    if(m_history.size() > 10) 
      m_history.pop_back();
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
          m_repaint = true;
        }
        break;

      default:
        if(m_buffer.size() < 75 ) {
          if(isascii(c)) {
            m_buffer.insert( m_buffer_pos, (char)c );
            m_repaint = true;
          }
        }
        break;
    } 
  }

}; // namespace std

