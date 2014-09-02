
#pragma once 

#include "interface.hh"
#include "input_handler.hh"

namespace ta {

  class Engine {
    private:

      Interface m_interface;
      InputHandler m_input;

    public:

      Engine();
      void run();
  };

}; // namespace ta;

