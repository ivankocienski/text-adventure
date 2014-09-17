
#include "description.hh"
#include "interface.hh"
#include "room.hh"
#include "exit.hh"

namespace ta {
  
  Exit::Exit() {
    m_target = NULL;
  }

  bool Exit::isset() {
    return m_target != NULL;
  }

  Description& Exit::description() {
    return m_description;
  }

  void Exit::describe( Interface& i ) {
    m_description.show(i);
  }

  void Exit::set_target( Room* t ) {
    m_target = t;
  }

  Room* Exit::target() {
    return m_target;
  }

};
