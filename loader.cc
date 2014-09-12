
#include <cstdlib>

#include <string>
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>

#include "loader.hh"

using namespace std;


namespace ta { 

  Loader::Loader( World& w, Player& p) : m_world(w), m_player(p) { 
    m_warnings = 0;
  }

  void Loader::moan( const Parser &p, const string& r ) {
    cerr << "warning: (line " << p.line_number() << ") " << r << endl;
    m_warnings++;
  }

  void Loader::moan( const decleration_t& d, const string& r ) {
    cerr << "warning: (line " << d.line << ") " << r << endl;
    m_warnings++;
  }

  void Loader::set_decleration( decleration_t &t, const Parser &p ) {

    if( t.assigned ) { 
      string msg;

      msg = "assignment already assigned (see line ";
      msg += t.line;
      msg += ")";

      moan( p, msg );
      return; 
    }

    t.parts    = p.matches(); 
    t.line     = p.line_number();
    t.assigned = true;
  }

  void Loader::parse_room_describe( decleration_t &desc, Parser &parser ) {

    set_decleration( desc, parser );

    while( true ) {

      switch( parser.next_token() ) {
        case Parser::TT_EOF:
          moan( parser, "Unexpected end of file in DESCRIBE" );
          return;

        case Parser::TT_STRING:
          desc.parts.push_back( parser.line() );
          break;

        case Parser::TT_TOKEN:
          switch( parser.token_code() ) {

            case TC_ENDDESCRIBE:
              return;

            default:
              moan( parser, "bad token at DESCRIBE" );
              break;
          }

          break;
      }
    }
  }

  void Loader::parse_room( Parser &parser ) {

    room_t &room = m_rooms[ parser.first_match() ];

    set_decleration( room.name, parser );

    while( true ) {

      switch( parser.next_token() ) {
        case Parser::TT_EOF:
          moan( parser, "Unexpected end of file in ROOM" );
          return;

        case Parser::TT_STRING:
          moan( parser, "Unexpected string in ROOM" );
          break;

        case Parser::TT_TOKEN: 
          {
          switch( parser.token_code() ) {
              
            case TC_ENDROOM:
              return;

            case TC_EXIT: {

              string dir = parser.first_match();
              if( dir == "north" )
                set_decleration( room.exit_north, parser );
              else
              //
              if( dir == "south" )
                set_decleration( room.exit_south, parser );
              else
              //
              if( dir == "east" )
                set_decleration( room.exit_east, parser );
              else
              //
              if( dir == "west" )
                set_decleration( room.exit_west, parser );
              //
              else
                moan( parser, "unknown direction for exit in ROOM" );

              } break;
              
            case TC_ITEM:
              set_decleration( room.items[ parser.first_match() ], parser ); 
              break;

            case TC_DESCRIBE:
              parse_room_describe( room.description, parser );
              break;

            default:
              moan( parser, "bad token at ROOM" );
              break;
          }

          break;
          }

      }
    }


  }

  void Loader::parse_toplevel( Parser &parser ) {

    while( true ) {

      switch( parser.next_token() ) {
        case Parser::TT_EOF:
          return;

        case Parser::TT_STRING:
          moan( parser, "Unexpected string" );
          break;

        case Parser::TT_TOKEN:

          switch( parser.token_code() ) {

            case TC_NAME:
              set_decleration( m_globals.name, parser );
              break;
              
            case TC_AUTHOR:
              set_decleration( m_globals.author, parser );
              break;

            case TC_COPYRIGHT:
              set_decleration( m_globals.copyright, parser );
              break;

            case TC_START:
              set_decleration( m_globals.start, parser );
              break;

            case TC_ROOM:
              parse_room( parser );
              break;

            default:
              moan( parser, "bad token at toplevel" );
              break;
          }

          break;
      } 
    } 
  }


  void Loader::upload() {

    m_world.set_name(      m_globals.name.parts.front() );
    m_world.set_author(    m_globals.author.parts.front() );
    m_world.set_copyright( m_globals.copyright.parts.front() );

    for( room_map_t::iterator ri = m_rooms.begin(); ri != m_rooms.end(); ri++ ) {

      room_t &r = (*ri).second;

      m_world.build_room(
        r.name.value(),
        r.description.parts
      );
    }

    for( room_map_t::iterator ri = m_rooms.begin(); ri != m_rooms.end(); ri++ ) {

      room_t &rd = (*ri).second;

      Room *r = m_world.get( rd.name.value() );

      for( decleration_map_t::iterator it = rd.items.begin(); it != rd.items.end(); it++ ) {

        decleration_t &id = (*it).second;
        r->place_item( id.value() );
      }

      if( rd.exit_north.assigned ) {
        Room *other = m_world.get( rd.exit_north.parts[1] );
        r->exit_north( other );
        continue; 
      }

      if( rd.exit_south.assigned ) {
        Room *other = m_world.get( rd.exit_south.parts[1] );
        r->exit_south( other );
        continue; 
      }

      if( rd.exit_east.assigned ) {
        Room *other = m_world.get( rd.exit_east.parts[1] );
        r->exit_east( other );
        continue; 
      }

      if( rd.exit_west.assigned ) {
        Room *other = m_world.get( rd.exit_west.parts[1] );
        r->exit_west( other );
        continue; 
      }

      moan( rd.name, "no exits defined in room" );
    }

    Room *r = m_world.get( m_globals.start.value() );

    m_player.start_in(r); 
  }

  void Loader::parse( const string &fn ) {

    Parser parser( fn );

    // not the best way to write a parser, but hey...
    parser.define_matcher( TC_NAME,        "^%NAME\\s+(.+)" );
    parser.define_matcher( TC_AUTHOR,      "^%AUTHOR\\s+(.+)" );
    parser.define_matcher( TC_COPYRIGHT,   "^%COPYRIGHT\\s+(.+)" );
    parser.define_matcher( TC_START,       "^%START\\s+(.+)" );
    parser.define_matcher( TC_ROOM,        "^%ROOM\\s+(.+)" );
    parser.define_matcher( TC_DESCRIBE,    "^%DESCRIBE\\b" );
    parser.define_matcher( TC_EXIT,        "^%EXIT\\s+(\\w+)\\s+(\\w+)" );
    parser.define_matcher( TC_ENDDESCRIBE, "^%ENDDESCRIBE\\b" );
    parser.define_matcher( TC_ENDROOM,     "^%ENDROOM\\b" );
    parser.define_matcher( TC_ITEM,        "^%ITEM\\s+(\\w+)" );

    parse_toplevel( parser );

    if(m_warnings) {
      cerr << "there where " << m_warnings << " warnings." << endl;
      exit(-1);
    }

    upload();
  }


  /****************\
   *              *
   *  the parser  *
   *              * 
  \****************/

  Loader::Parser::Parser( const std::string &fn ) : 
    m_line_number(0),
    m_file( fn.c_str() ),
    m_blank_r( "^\\s*$" ) { }

  Loader::Parser::token_type Loader::Parser::next_token() {

    m_token_code = -1;

    string::size_type start;

    while(true) {

      if(m_file.eof()) 
        return TT_EOF;

      m_line_number++; 
      getline( m_file, m_line );

      // ignore leading whitespace
      start = m_line.find_first_not_of( " \t\n\r" );
      if( start != string::npos && start > 0 ) {
        m_line.erase(0, start); 
      }

      // blank line
      if(boost::regex_match( m_line, m_capture, m_blank_r) ) continue;

      // comment line
      if(m_line[0] == '#') continue;

      // assume its a command
      if(m_line[0] == '%') return TT_TOKEN;

      break;
    }

    return TT_STRING; 
  }

  void Loader::Parser::define_matcher( int code, const string &p ) {

    matcher_t m;

    m.return_code = code;
    m.pattern     = boost::regex( p );

    m_matchers.push_back(m);
  }

  int Loader::Parser::token_code() {

    list<matcher_t>::iterator it;

    for( it = m_matchers.begin(); it != m_matchers.end(); it++ ) {

      if( boost::regex_match( m_line, m_capture, (*it).pattern ) ) {
        return (*it).return_code;
      } 
    }

    return -1;
  }

  int Loader::Parser::line_number() const {
    return m_line_number;
  }

  string Loader::Parser::line() const {
    return m_line;
  }

  vector<string> Loader::Parser::matches() const {

    vector<string> vs;

    vs.resize( m_capture.size() - 1 );
    for( size_t i = 1; i < m_capture.size(); i++) {
      vs[i - 1] = m_capture[i]; 
    }

    return vs;
  }

  string Loader::Parser::first_match() const {
    return m_capture[1];
  }


}; // namespace ta
