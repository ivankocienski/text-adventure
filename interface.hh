
#pragma once

#include <string>

namespace ta {
  
  class Interface {
    private:

    public:

      Interface();

      void puts( const char* );
      void puts( const std::string& );
  };

  void operator<<( Interface&, char* );
  void operator<<( Interface&, std::string );
}; // namespace ta;
