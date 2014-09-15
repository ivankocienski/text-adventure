
#pragma once

#include <string>
#include <map>
#include <fstream>
#include <list>
#include <boost/regex.hpp>

namespace ta {
  
  class Player;
  class World;

  class Loader {
  private:

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
        boost::regex         m_blank_r;
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
    } global_t, *p_global_t;

    typedef struct room_s {

      decleration_t name; 
      decleration_t description; 
      decleration_t exit_north;
      decleration_t exit_south;
      decleration_t exit_east;
      decleration_t exit_west; 

      decleration_map_t items;
    } room_t, *p_room_t;

    typedef std::map<std::string, room_t> room_map_t;

    global_t   m_globals;
    room_map_t m_rooms;
    
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
      TC_VERSION
    };

    void moan( const Parser&, const std::string& );
    void moan( const decleration_t&, const std::string& );

    void set_decleration( decleration_t&, const Parser& );

    void parse_room_describe( decleration_t&, Parser& );
    void parse_room( Parser& );
    void parse_toplevel( Parser& );
    void upload();

  public:
    Loader( World*, Player* );

    void parse( const std::string& ); 
  };

}; // namespace ta

