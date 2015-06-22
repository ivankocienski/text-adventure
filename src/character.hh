
#pragma once

#include <string>

#include "interface.hh"
#include "description.hh"

namespace ta {

class Character {
private:

  std::string m_name;

  Description m_description;
  Description m_talk;

public:

  Character();
  Character(const std::string&);
  std::string name();

  Description& talk_obj();
  void talk( Interface& );

  Description& description();
  void describe( Interface & );
};

}; // namespace ta
