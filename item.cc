
#include <string>

#include "description.hh"
#include "interface.hh"
#include "item.hh"
#include "room.hh"

using namespace std;

namespace ta {

  Item::Item() {
    m_in_room = NULL;
  }

  Item::Item( const string &n ) {
    m_in_room = NULL;
    m_name    = n;
  }

  Description & Item::description() {
    return m_description;
  }

  void Item::describe( Interface &i ) {
    m_description.show( i );
  }

  string & Item::name() {
    return m_name;
  }

  void Item::set_room( Room* r ) {
    if( m_in_room ) { 
      m_in_room->remove_item_from_inventory( m_name ); 
    }

    m_in_room = r;
    if( m_in_room ) {
      m_in_room->add_item_to_inventory( m_name ); 
    } 
  }

}; // namespace ta

