
#pragma once

#include <string>
#include <map>
#include <fstream>
#include <list>
#include <boost/regex.hpp>

#include "description.hh"
#include "exit.hh"

namespace ta {
  
  class Player;
  class World;

  class Loader {
  private:

    static boost::regex s_blank_r;

    class Parser {
      public:

        enum token_type {
          TT_TOKEN,
          TT_EOF,
          TT_STRING
        };

      private:

        typedef struct matcher_s {
          int return_code;
          boost::regex pattern; 
        } matcher_t;

        int m_line_number;
        int m_token_code;

        std::ifstream        m_file;
        std::string          m_line;
        std::list<matcher_t> m_matchers;
        boost::smatch        m_capture;
        
      public:

        Parser( const std::string& );

        void define_matcher( int, const std::string& );

        token_type next_token();
        int token_code();

        std::vector<std::string> matches() const;
        std::string first_match() const;
        std::string line() const;

        int line_number() const;

    };

    typedef struct decleration {
      std::vector<std::string> parts;
      size_t line; 
      bool   assigned;
 
      decleration() { assigned = false; }

      std::string& value() { return parts[0]; }
    } decleration_t;

    typedef std::map<std::string, decleration_t> decleration_map_t;

    typedef struct global_s {
      decleration_t name;
      decleration_t author;
      decleration_t copyright;
      decleration_t start;
      decleration_t introduction;
    } global_t, *p_global_t;

    typedef struct exit_s {
      bool assigned;
      size_t line;

      exit_s() : assigned( false ), line( 0 ) {}

      decleration_t room_goto;
      decleration_t description; 
      decleration_t locked_by;
    } exit_t;

    typedef struct item_s {
      bool assigned;
      size_t line;

      item_s() : assigned( false ), line(0) {}

      decleration_t name;
      decleration_t description;

      // cannot be blank
      std::string in_room; 
    } item_t;
    typedef std::map<std::string, item_t> item_map_t;

    typedef struct character_s {
      bool assigned;
      size_t line;

      character_s() : assigned( false ), line(0) {}

      decleration_t name;
      decleration_t talk; 
      decleration_t description;

      std::string in_room;
    } character_t;
    typedef std::map<std::string, character_t> character_map_t;

    typedef struct room_s {
      decleration_t name; 
      decleration_t description; 

      std::map<std::string, exit_t> exits;

    } room_t, *p_room_t;

    typedef std::map<std::string, room_t> room_map_t;

    global_t         m_globals;
    room_map_t       m_rooms;
    item_map_t       m_items;
    character_map_t  m_characters;
    
    /* Engine owns World and Player, we're just borrowing them
       here, so its okay. i hope. */

    World  *m_world;
    Player *m_player;
    int     m_warnings;


    enum token_code {
      TC_NAME,
      TC_AUTHOR,
      TC_COPYRIGHT,
      TC_START,
      TC_ROOM,
      TC_ENDROOM,
      TC_DESCRIBE,
      TC_ENDDESCRIBE,
      TC_EXIT,
      TC_ITEM,
      TC_ENDITEM,
      TC_VERSION,
      TC_INTRO,
      TC_ENDINTRO,
      TC_PAUSE,
      TC_ENDEXIT,
      TC_GOTO,
      TC_LOCKEDBY,
      TC_CHARACTER,
      TC_ENDCHARACTER,
      TC_TALK,
      TC_ENDTALK
    };

    void moan( const Parser&, const std::string& );
    void moan( const decleration_t&, const std::string& );

    void set_decleration( decleration_t&, const Parser& );

    void parse_describe( decleration_t&, Parser& );
    void parse_talk( decleration_t&, Parser& ); 

    void parse_exit( exit_t&, Parser& );
    void parse_item( item_t&, std::string&, Parser& );
    void parse_room( Parser& );
    void parse_character( character_t&, std::string&, Parser& );
    void parse_intro( Parser& );
    void parse_toplevel( Parser& );

    void upload_description( Description&, decleration_t& );
    bool upload_exit( Exit&, exit_t& );
    void upload_items();
    void upload_characters();
    void upload_rooms();
    void upload_globals();

  public:
    Loader( World*, Player* );

    void parse( const std::string& ); 
  };

}; // namespace ta

