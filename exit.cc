
#include <string>
#include <map>
#include <boost/unordered_set.hpp>

#include "description.hh"
#include "interface.hh"
#include "room.hh"
#include "exit.hh"
#include "item.hh"

using namespace std;

namespace ta {
  
  Exit::Exit() {
    m_target = NULL;
    m_locked = false;
  }

  bool Exit::islocked() {
    return m_locked;
  }

  void Exit::unlock() {
    m_locked = false;
  }

  string & Exit::locked_with() {
    return m_unlock_with;
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

  void Exit::set_lock( string &l ) {
    m_locked      = true;
    m_unlock_with = l;
  }

  Exit::lock_response Exit::unlock( boost::unordered_set<string> &ks ) {

    if( m_unlock_with == "" )
      return LR_NOLOCK;

    if( !m_locked) 
      return LR_NOTLOCKED;

    if( !ks.count( m_unlock_with ) )
      return LR_NOTKEY;

    ks.erase( m_unlock_with );
    m_locked = false;

    return LR_UNLOCKED; 
  }

};
