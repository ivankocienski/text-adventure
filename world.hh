
#pragma once

#include <string>
#include <map>
#include <vector>

#include "room.hh"
#include "item.hh"

namespace ta {
  
  class World {
  private:

    typedef std::map<std::string, Room> room_map_t;
    room_map_t     m_rooms;

    std::map<std::string, Item> m_items;

    std::string m_name;
    std::string m_author;
    std::string m_copyright;

    Description m_description;

  public:
    World();

    void build_room( const std::string&, const std::vector<std::string>& );
    void list_rooms();
    Room* get(const std::string&);

    void set_name(      const std::string &);
    void set_author(    const std::string &);
    void set_copyright( const std::string &);

    void introduce(Interface&);

    Description& description();

    Item & create_item( const std::string & );
    Item & get_item( const std::string & );
  };

}; // namespace ta

