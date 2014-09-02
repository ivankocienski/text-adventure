
#pragma once

#include <string>
#include <vector>

namespace ta {

  class InputHandler {
    private:

      std::vector<std::string> m_words;

    public:

      InputHandler();

      void read();

      std::string word(int);
  };
};

