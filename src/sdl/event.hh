
#pragma once

#include <SDL/SDL.h>

namespace SDL {
  
  class Event { 
  private:

    SDL_Event m_event;

  public:
    
    Event();

    bool poll();
    int type();
    int key_sym() const;
  };
};

