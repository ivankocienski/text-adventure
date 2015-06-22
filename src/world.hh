
#pragma once

#include <string>
#include <map>
#include <vector>

#include "room.hh"
#include "item.hh"
#include "character.hh"

namespace ta {
  
  class World {
  public:

    typedef std::map<std::string, Room> room_map_t;
    typedef std::map<std::string, Item> item_map_t;
    typedef std::map<std::string, Character> character_map_t;

  private:

    room_map_t      m_rooms;
    item_map_t      m_items;
    character_map_t m_characters;

    std::string m_name;
    std::string m_author;
    std::string m_copyright;

    Description m_description;

  public:
    World();

    Room* build_room( const std::string& );
    void list_rooms();
    Room* get_room(const std::string&);
    room_map_t & rooms();

    void set_name(      const std::string &);
    void set_author(    const std::string &);
    void set_copyright( const std::string &);

    void introduce(Interface&);

    Description& description();

    bool has_item( const std::string & );
    Item & create_item( const std::string & );
    Item & get_item( const std::string & );

    bool has_character( const std::string & );
    Character& create_character( const std::string & );
    Character& get_character( const std::string& );
  };

}; // namespace ta

