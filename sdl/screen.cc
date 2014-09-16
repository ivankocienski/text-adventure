
#include "screen.hh"
#include "surface.hh"

#include <SDL/SDL.h>

namespace SDL {

  Screen::Screen() {
    m_screen = NULL;
  }

  Screen::~Screen() {
    if(m_screen) SDL_Quit();
  }

  bool Screen::open( int w, int h ) {
    m_screen = SDL_SetVideoMode(w, h, 8, SDL_SWSURFACE); 
    return m_screen != NULL;
  }
  
  void Screen::clear() {
    SDL_FillRect( m_screen, NULL, 0 );
  }

  void Screen::flip() {
    SDL_Flip( m_screen );
  }

  void Screen::blit( int x, int y, const Surface &from ) {
    SDL_Rect d = { x, y, 0, 0 };
    SDL_BlitSurface( from.surface(), NULL, m_screen, &d );
  }

  void Screen::blit( SDL_Rect &d, const Surface &from, SDL_Rect &s ) { 
    SDL_BlitSurface(
      from.surface(),
      &s,
      m_screen,
      &d
    ); 
  }

  void Screen::load_palette( const Surface &src ) {
    SDL_SetPalette( 
      m_screen, 
      SDL_LOGPAL,
      src.surface()->format->palette->colors,
      0,
      src.surface()->format->palette->ncolors 
    ); 
  }
};

