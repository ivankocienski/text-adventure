
#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>

#include "surface.hh"

using namespace std;

namespace SDL {

  Surface::Surface() {
    cout << "Surface: create" << endl;
    m_surface = NULL;
  }

  Surface::~Surface() {
    cout << "Surface: destroy" << endl;
    if( m_surface )
      SDL_FreeSurface( m_surface );
  }

  Surface::Surface( const Surface &from ) {

    m_surface = NULL;

    cout << "Surface: copy" << endl;

    if( !from.valid() ) return;
    
    m_surface = SDL_CreateRGBSurface(
        SDL_SWSURFACE,
        from.width(),
        from.height(),
        8, 0, 0, 0, 0);

    if( m_surface ) {
      SDL_SetPalette( 
          m_surface, 
          SDL_LOGPAL,
          from.surface()->format->palette->colors,
          0,
          from.surface()->format->palette->ncolors 
          ); 

      SDL_BlitSurface( from.surface(), NULL, m_surface, NULL ); 
    }
  }

  Surface & Surface::operator=( const Surface &from ) {

    cout << "Surface:: assign" << endl;

    if( m_surface ) {
      SDL_FreeSurface(m_surface);
      m_surface = NULL;
    }

    if( !from.valid() ) return *this;

    m_surface = SDL_CreateRGBSurface(
        SDL_SWSURFACE,
        from.width(),
        from.height(),
        8, 0, 0, 0, 0);

    if( m_surface ) {
      SDL_SetPalette( 
          m_surface, 
          SDL_LOGPAL,
          from.surface()->format->palette->colors,
          0,
          from.surface()->format->palette->ncolors 
          ); 

      SDL_BlitSurface( from.surface(), NULL, m_surface, NULL ); 
    }

    return *this;
  }

  bool Surface::load( const std::string& s ) {
    const char *c = s.c_str();
    m_surface = IMG_Load( c );
    return m_surface != NULL;
  }

  bool Surface::valid() const {
    return m_surface != NULL;
  }

  SDL_Surface* Surface::surface() const {
    return m_surface;
  }

  int Surface::width() const {
    return m_surface->w;
  }

  int Surface::height() const {
    return m_surface->h;
  }

  void Surface::lock() {
    SDL_LockSurface( m_surface );
  }

  void Surface::unlock() {
    SDL_UnlockSurface( m_surface );
  }

  unsigned char* Surface::pixel_bytes() {
    return (unsigned char*)m_surface->pixels;
  }

};
