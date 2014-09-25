
#include <string>

#include "interface.hh"
#include "description.hh"
#include "character.hh"

using namespace std;

namespace ta {

Character::Character() {
}

Character::Character( const std::string &n ) : m_name(n) { 
}

std::string Character::name() {
  return m_name; 
}

Description& Character::talk_obj() {
  return m_talk; 
}

void Character::talk( Interface &i ) {
  m_talk.show( i ); 
}

Description& Character::description() {
  return m_description; 
}

void Character::describe( Interface &i ) {
  m_description.show(i); 
}

}; // namespace ta
