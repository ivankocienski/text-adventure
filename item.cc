
#include <string>

#include "description.hh"
#include "interface.hh"
#include "item.hh"

using namespace std;

namespace ta {

  Item::Item() { 
  }

  Item::Item( const string &n ) {
    m_name = n;
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

}; // namespace ta

