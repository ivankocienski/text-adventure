
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>
#include <list>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "sdl/event.hh"
#include "sdl/screen.hh"
#include "sdl/surface.hh"
#include "interface.hh"

using namespace std;

namespace ta {
  
  Interface::Interface() : m_screen( new SDL::Screen ), m_debug_font( m_screen ) { 
    m_cursor_show  = true;
    m_cursor_count = 20;
  }

  Interface::~Interface() {
    m_history.clear();
    m_fonts.clear();
  }

  void Interface::init(int w, int h) { 

    if( !m_screen->open( w, h ) ) {
      cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
      exit(-1);
    }

    if( !m_debug_font.load( "font.png" ) ) {
      cerr << "Failed to load font" << endl;
      exit(-1);
    }

    m_debug_font.load_palette();

    m_fonts.resize(8);

    for(int i = 0; i < 8; i++) {
      m_fonts[i] = m_debug_font;
      m_fonts[i].remap_color_to( i ); 
    }
  }

  void Interface::press_any_key() {

    SDL::Event event;

    m_hold_count   = 2;

    m_get_events   = true;
    m_cursor_show  = true;
    m_cursor_count = 20;
    m_repaint      = true;

    while(m_get_events) {

      if(m_cursor_count) {
        m_cursor_count--;

      } else {
        m_cursor_count = 20;
        m_cursor_show  = !m_cursor_show;
        m_repaint = true; 
      }

      while(event.poll()) {
        switch (event.type()) {

          case SDL_KEYUP: 
            if( !m_hold_count && event.key_sym() == SDLK_SPACE ) m_get_events = false;
            break;

          case SDL_QUIT:
            exit(0);
            break; 
        }
      }

      if(m_repaint) {
        m_screen->clear();

        m_fonts[1].puts( 31, 0, "A D V E N T U R E" );

        draw_prompt();
        draw_history();
        
        m_screen->flip();

        m_repaint = false;
      }

      SDL_Delay(50);

      if( m_hold_count ) m_hold_count--;
    }
  }

  string Interface::wait_for_input() {

    SDL::Event event;

    m_buffer.clear();
    m_buffer_pos = m_buffer.begin();

    m_get_events = true;
    m_repaint = true;

    while(m_get_events) {

      if(m_cursor_count) {
        m_cursor_count--;

      } else {
        m_cursor_count = 20;
        m_cursor_show  = !m_cursor_show;
        m_repaint = true; 
      }

      while(event.poll()) {
        switch (event.type()) {

          case SDL_KEYDOWN: 
            handle_char( event.key_sym() ); 
            break;

          case SDL_QUIT:
            exit(0);
            break; 
        }
      }

      if(m_repaint) {
        m_screen->clear();

        m_fonts[1].puts( 31, 0, "A D V E N T U R E" );

        draw_buffer();
        draw_history();
        
        m_screen->flip();

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

  void Interface::puts( int c, const string &str ) {

    struct history_s hs = { str, m_fonts[c] };

    m_history.push_front(hs); 

    if(m_history.size() > 51) 
      m_history.pop_back();
  }

  void Interface::puts( const string &s ) {
    puts( 7, s );
  }

  void Interface::draw_prompt() {

    m_fonts[7].draw_box( 0, 57, 79, 2 );

    if( m_hold_count ) return;

    if(m_cursor_show)
      m_fonts[2].puts( 1, 58, "Press SPACEBAR to continue" );
  }

  void Interface::draw_history() {
    int pos = 53;

    list<struct history_s>::iterator it = m_history.begin();

    m_fonts[7].draw_box( 0, 2, 79, 52 );

    while( it != m_history.end() ) {

      (*it).font.puts( 1, pos, (*it).string );
      pos--;
      it++;
    }
  }

  void Interface::draw_buffer() {

    list<char>::iterator c = m_buffer.begin();
    int x = 1; 

    m_fonts[7].draw_box( 0, 57, 79, 2 );

    while(c != m_buffer.end() ) {

      m_fonts[7].put_char( x, 58, *c );

      x++;
      c++; 
    }

    if(m_cursor_show)
      m_screen->fill_rect( x * 8, 58 * 8, 8, 8, 7);
  }

  void Interface::handle_char( int c ) {

    //cout << "c=" << c << endl;
    m_cursor_show  = true;
    m_cursor_count = 20;

    switch(c) {
      case 0:
        break;

      case SDLK_RETURN:
        {
          bool is_blank = true;
          list<char>::iterator it;

          for( it = m_buffer.begin(); it != m_buffer.end(); it++ ) {
            if( isspace( *it ) ) continue;

            is_blank = false;
            break;
          }

          if( is_blank ) {
            m_buffer.clear();
            m_buffer_pos   = m_buffer.begin();
            m_cursor_count = 20;
            m_cursor_show  = m_cursor_show;
            m_repaint      = true; 

          } else 
            m_get_events = false;

        }
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

  /* font stuff */

  Interface::Font::Font() { } 

  Interface::Font::Font( boost::shared_ptr<SDL::Screen> &scr_p ) : m_screen( scr_p ) { 
  }

  Interface::Font::Font( boost::shared_ptr<SDL::Screen> &scr_p, SDL::Surface &sfc ) : m_screen( scr_p ), m_surface( sfc ) {
  }

  bool Interface::Font::load( const string &from ) {
    return m_surface.load( from );
  }

  void Interface::Font::puts( int x, int y, const string &str ) {

    SDL_Rect s = { 0, 0, 8, 8 };
    SDL_Rect d = { (Sint16)(x * 8), (Sint16)(y * 8), 0, 0 };

    const char *c = str.c_str();

    while(*c) {

      s.x = ((*c - 32) & 0x0f) << 3;
      s.y = ((*c - 32) & 0xf0) >> 1;

      m_screen->blit( d, m_surface, s );

      d.x += 8;

      c++; 
    }
  }

  void Interface::Font::put_char( int x, int y, int c ) {

    SDL_Rect s = { 0, 0, 8, 8 };
    SDL_Rect d = { 0, 0, 0, 0 };

    d.x = x << 3;
    d.y = y << 3;

    s.x = ((c - 32) & 0x0f) << 3;
    s.y = ((c - 32) & 0xf0) >> 1;

    m_screen->blit( d, m_surface, s );
  }

  void Interface::Font::remap_color_to( int color ) {

    m_surface.lock();

    unsigned char *p = m_surface.pixel_bytes();
    int c = m_surface.height() * m_surface.width(); 

    while(c) {
      *p = *p ? color : 0;

      c--;
      p++;
    }

    m_surface.unlock();
  }

  void Interface::Font::load_palette() {
    m_screen->load_palette( m_surface );
  }

  void Interface::Font::draw_box( int x, int y, int w, int h ) {

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
    m_screen->blit( dst, m_surface, top_l );

    dst.x = (x + w) * 8; 
    m_screen->blit( dst, m_surface, top_r );

    dst.x = x * 8;
    dst.y = (y + h) * 8;
    m_screen->blit( dst, m_surface, bot_l );
    
    dst.x = (x + w) * 8;
    m_screen->blit( dst, m_surface, bot_r );

    dst.x = x * 8 + 8;
    for(i = 0; i < w - 1; i++ ) {
      dst.y = y * 8;
      m_screen->blit( dst, m_surface, top );
      
      dst.y = (y + h) * 8;
      m_screen->blit( dst, m_surface, bot );
      
      dst.x += 8;
    }

    dst.y = y * 8 + 8;
    for( i = 0; i < h - 1; i++ ) {

      dst.x = x * 8;
      m_screen->blit( dst, m_surface, left );

      dst.x = (x + w) * 8;
      m_screen->blit( dst, m_surface, right );

      dst.y += 8; 
    } 
  }


}; // namespace std

