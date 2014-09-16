
#pragma once

#include <SDL/SDL.h>
#include <boost/noncopyable.hpp>

#include "surface.hh"

namespace SDL {

  class Screen : private boost::noncopyable {
  private:
    
    SDL_Surface *m_screen;

  public:

    Screen();
    ~Screen();

    bool open( int, int );

    void flip();
    void clear();

    void blit( int, int, const Surface& );
    void blit( SDL_Rect&, const Surface&, SDL_Rect& );
    void load_palette( const Surface& );
  };

};

