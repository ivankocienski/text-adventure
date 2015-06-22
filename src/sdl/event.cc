
#include <SDL/SDL.h>
#include "event.hh"

namespace SDL {

  Event::Event() {
  }

  bool Event::poll() {
    return SDL_PollEvent(&m_event);
  }

  int Event::type() {
    return m_event.type;
  }

  int Event::key_sym() const {
    return m_event.key.keysym.sym;
  }

};

