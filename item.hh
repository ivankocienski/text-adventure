
#pragma once

#include <string>
#include <boost/shared_ptr.hpp>

#include "description.hh"
#include "interface.hh"

namespace ta {
  
  class Item {
  private:

    std::string m_name;
    Description m_description;

  public:

    Item(); 
    Item(const std::string&);

    Description& description();

    std::string & name();
    void set_name( const std::string& );

    void describe( Interface& );
  };

  typedef boost::shared_ptr<Item> item_ptr;

}; // namepspace ta;

