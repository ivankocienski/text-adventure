
#pragma once

#include <SDL/SDL.h>
#include <string>

namespace SDL {

  class Surface {
  private:

    SDL_Surface *m_surface;

  public:
    
    Surface();
    ~Surface();
    Surface( const Surface& );

    bool load( const std::string& ); 
    bool valid() const;

    int width()  const;
    int height() const;

    SDL_Surface *surface() const;

    Surface & operator=( const Surface& );

    void lock();
    void unlock();
    unsigned char *pixel_bytes();

  };
};

