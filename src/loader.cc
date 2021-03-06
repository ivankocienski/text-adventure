
#include <cstdlib>

#include <string>
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>

#include "loader.hh"
#include "world.hh"
#include "player.hh"
#include "description.hh"
#include "exit.hh"
#include "item.hh"

/* i hope this does not turn into a shitty lisp implementation... */

using namespace std;

namespace ta { 

  boost::regex Loader::s_blank_r( "^\\s*$" );

  Loader::Loader( World* w, Player* p) : m_world(w), m_player(p) { 
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

  void Loader::parse_describe( decleration_t &desc, Parser &parser ) {

    set_decleration( desc, parser );

    while( true ) {

      switch( parser.next_token() ) {
        case Parser::TT_EOF:
          moan( parser, "Unexpected end of file in DESCRIBE" );
          return;

        case Parser::TT_STRING:
          if( parser.line() == "." )
            desc.parts.push_back( "" );
          else
            desc.parts.push_back( parser.line() );
          break;

        case Parser::TT_TOKEN:
          switch( parser.token_code() ) {

            case TC_ENDDESCRIBE:
              return;

            case TC_PAUSE: 
              desc.parts.push_back( " " );
              break;

            default:
              moan( parser, "bad token at DESCRIBE" );
              break;
          }

          break;
      }
    }
  }

  void Loader::parse_talk( decleration_t &talk, Parser &parser ) {

    set_decleration( talk, parser );

    while( true ) {

      switch( parser.next_token() ) {
        case Parser::TT_EOF:
          moan( parser, "Unexpected end of file in TALK" );
          return;

        case Parser::TT_STRING:
          if( parser.line() == "." )
            talk.parts.push_back( "" );
          else
            talk.parts.push_back( parser.line() );
          break;

        case Parser::TT_TOKEN:
          switch( parser.token_code() ) {

            case TC_ENDTALK:
              return;

            case TC_PAUSE: 
              talk.parts.push_back( " " );
              break;

            default:
              moan( parser, "bad token at TALK" );
              break;
          }

          break;
      }
    }
  }

  void Loader::parse_item( item_t &item, string &r, Parser &parser ) {

    if( item.assigned ) {
      moan( parser, "Item already exists in world, see line " + item.line );
      return;
    }

    item.assigned = true;
    item.line     = parser.line_number();
    item.in_room  = r;

    set_decleration( item.name, parser );

    while( true ) {

      switch( parser.next_token() ) {
        case Parser::TT_EOF:
          moan( parser, "Unexpected end of file in ITEM" );
          return;

        case Parser::TT_STRING:
          moan( parser, "Unexpected string in ITEM" );
          break; 

        case Parser::TT_TOKEN:
          switch( parser.token_code() ) {

            case TC_ENDITEM:
              return;

            case TC_DESCRIBE:
              parse_describe( item.description, parser );
              break;

            default:
              moan( parser, "bad token at ITEM" );
              break;
          }

          break;
      }
    }


  }

  void Loader::parse_exit( exit_t &exit, Parser &parser ) {

    if( exit.assigned ) {
      moan( parser, "exit is already defined in ROOM" );
      return;
    }

    exit.line = parser.line_number();
    exit.assigned = true;

    while( true ) {

      switch( parser.next_token() ) {
        case Parser::TT_EOF:
          moan( parser, "Unexpected end of file in EXIT" );
          return;

        case Parser::TT_STRING:
          moan( parser, "Unexpected string in EXIT" );
          break; 

        case Parser::TT_TOKEN:
          switch( parser.token_code() ) {

            case TC_ENDEXIT:
              return;

            case TC_GOTO: 
              set_decleration( exit.room_goto, parser );
              break;

            case TC_LOCKEDBY:
              set_decleration( exit.locked_by, parser );
              break;

            case TC_DESCRIBE:
              parse_describe( exit.description, parser );
              break;

            default:
              moan( parser, "bad token at EXIT" );
              break;
          }

          break;
      }
    }
  }

  void Loader::parse_character( character_t &chr, string &room_name, Parser &parser ) {

    if( chr.assigned ) {
      moan( parser, "Character already exists in world, see line " + chr.line );
      return;
    }

    chr.assigned = true;
    chr.line     = parser.line_number();
    chr.in_room  = room_name;

    set_decleration( chr.name, parser );

    while( true ) {

      switch( parser.next_token() ) {
        case Parser::TT_EOF:
          moan( parser, "Unexpected end of file in CHARACTER" );
          return;

        case Parser::TT_STRING:
          moan( parser, "Unexpected string in CHARACTER" );
          break; 

        case Parser::TT_TOKEN:
          switch( parser.token_code() ) {

            case TC_ENDCHARACTER:
              return;

            case TC_TALK: 
              parse_talk( chr.talk, parser );
              break;

            case TC_DESCRIBE:
              parse_describe( chr.description, parser );
              break;

            default:
              moan( parser, "bad token at CHARACTER" );
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
              parse_exit( room.exits[dir], parser );
              }
              break;
              
            case TC_ITEM:
              parse_item( 
                m_items[ parser.first_match() ], 
                room.name.value(),
                parser 
              );
              break;
              
            case TC_CHARACTER:
              parse_character(
                m_characters[ parser.first_match() ],
                room.name.value(),
                parser
              );
              break;

            case TC_DESCRIBE:
              parse_describe( room.description, parser );
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

  void Loader::parse_intro( Parser &parser ) {

    while( true ) {

      switch( parser.next_token() ) {
        case Parser::TT_EOF:
          moan( parser, "Unexpected end of file in INTRO" );
          return;

        case Parser::TT_STRING:
          moan( parser, "Unexpected string in INTRO" );
          break;

        case Parser::TT_TOKEN: {

          switch( parser.token_code() ) {

            case TC_ENDINTRO:
              return;

            case TC_DESCRIBE:
              parse_describe( m_globals.introduction, parser );
              break;

            case TC_START:
              set_decleration( m_globals.start, parser );
              break;

            default:
              moan( parser, "bad token at INTRO" );
              break;
          }
        }
        break;

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

            case TC_VERSION:
              if( parser.first_match() != "1" ) {
                cerr << "worldfile not version 1" << endl;
                exit(-1);
              }
              
              break;

            case TC_NAME:
              set_decleration( m_globals.name, parser );
              break;
              
            case TC_AUTHOR:
              set_decleration( m_globals.author, parser );
              break;

            case TC_COPYRIGHT:
              set_decleration( m_globals.copyright, parser );
              break;

            case TC_INTRO:
              parse_intro( parser );
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

  void Loader::upload_description( Description& d, decleration_t &dec ) {

    for( vector<string>::iterator it = dec.parts.begin(); it != dec.parts.end(); it++) {

      if( *it == " " ) {
        d.push_pause();
        continue;
      }

      d.push_string( (const string &)*it );
    } 
  }

  bool Loader::upload_exit( Exit& exit, exit_t& from ) {

    if( !from.assigned ) return false;

    if( from.description.assigned )
      upload_description( exit.description(), from.description );

    if( from.locked_by.assigned ) {
      cout << "door is locked" << endl;
      exit.set_lock( from.locked_by.value() );
    }

    if( from.room_goto.assigned ) {

      string name = from.room_goto.value();

      Room *other = m_world->get_room( name );
      if( !other ) {
        moan( from.room_goto, "could not lookup ROOM " + name + " in GOTO" );
        return false;
      }

      exit.set_target( other ); 
    }
        
    return true; 
  }

  void Loader::upload_items() {

    for( item_map_t::iterator ri = m_items.begin(); ri != m_items.end(); ri++ ) {

      item_t &id = (*ri).second;

      Item & item = m_world->create_item( (const string)id.name.value() );
      upload_description( item.description(), id.description );

      // this should never fail.
      Room *r = m_world->get_room( id.in_room );

      item.set_room( r );
    }
  }

  void Loader::upload_characters() {

    for( character_map_t::iterator ri = m_characters.begin(); ri != m_characters.end(); ri++ ) {

      character_t &cd = (*ri).second;

      Character & character = m_world->create_character( (const string)cd.name.value() );
      upload_description( character.description(), cd.description );

      // nasty; talk is basically just a description
      // but at least it could be something else in future.
      upload_description( character.talk_obj(), cd.talk );

      // this should never fail.
      Room *r = m_world->get_room( cd.in_room );

      r->place_character( cd.name.value() );
    }
  }
  
  void Loader::upload_globals() {

    m_world->set_name(      m_globals.name.parts.front() );
    m_world->set_author(    m_globals.author.parts.front() );
    m_world->set_copyright( m_globals.copyright.parts.front() );

    if(m_globals.introduction.assigned) 
      upload_description( m_world->description(), m_globals.introduction );

    Room *r = m_world->get_room( m_globals.start.value() );

    m_player->start_in(r); 
  }

  void Loader::upload_rooms() {

    for( room_map_t::iterator ri = m_rooms.begin(); ri != m_rooms.end(); ri++ ) {

      room_t &r = (*ri).second;

      Room *room = m_world->build_room( r.name.value() ); 
      upload_description( room->description(), r.description ); 
    }

    for( room_map_t::iterator ri = m_rooms.begin(); ri != m_rooms.end(); ri++ ) {

      room_t &rd = (*ri).second;

      Room *r = m_world->get_room( rd.name.value() );

      for( map<string, exit_t>::iterator it = rd.exits.begin(); it != rd.exits.end(); it++ ) {

        string name = (*it).first;
        exit_t & et   = (*it).second;

        upload_exit( r->exits()[name], et );
      }

      if( rd.exits.size() == 0 )
        moan( rd.name, "no exits defined in room" );
    }
  }

  void Loader::parse( const string &fn ) {

    Parser parser( fn );

    // not the best way to write a parser, but hey...
    parser.define_matcher( TC_NAME,         "^%NAME\\s+(.+)" );
    parser.define_matcher( TC_AUTHOR,       "^%AUTHOR\\s+(.+)" );
    parser.define_matcher( TC_COPYRIGHT,    "^%COPYRIGHT\\s+(.+)" );
    parser.define_matcher( TC_START,        "^%START\\s+(.+)" );
    parser.define_matcher( TC_ROOM,         "^%ROOM\\s+(.+)" );
    parser.define_matcher( TC_DESCRIBE,     "^%DESCRIBE\\b" );
    parser.define_matcher( TC_EXIT,         "^%EXIT\\s+(\\w+)" );
    parser.define_matcher( TC_ENDDESCRIBE,  "^%ENDDESCRIBE\\b" );
    parser.define_matcher( TC_ENDROOM,      "^%ENDROOM\\b" );
    parser.define_matcher( TC_ITEM,         "^%ITEM\\s+(\\w+)" );
    parser.define_matcher( TC_ENDITEM,      "^%ENDITEM\\b" );
    parser.define_matcher( TC_VERSION,      "^%VERSION\\s+(\\w+)" );
    parser.define_matcher( TC_INTRO,        "^%INTRO\\b" );
    parser.define_matcher( TC_ENDINTRO,     "^%ENDINTRO\\b" );
    parser.define_matcher( TC_PAUSE,        "^%PAUSE\\b" );
    parser.define_matcher( TC_GOTO,         "^%GOTO\\s+(\\w+)" );
    parser.define_matcher( TC_ENDEXIT,      "^%ENDEXIT\\b" );
    parser.define_matcher( TC_LOCKEDBY,     "^%LOCKEDBY\\s+(\\w+)" );
    parser.define_matcher( TC_CHARACTER,    "^%CHARACTER\\s+(\\w+)" );
    parser.define_matcher( TC_ENDCHARACTER, "^%ENDCHARACTER\\b" );
    parser.define_matcher( TC_TALK,         "^%TALK\\b" );
    parser.define_matcher( TC_ENDTALK,      "^%ENDTALK\\b" );

    parse_toplevel( parser );

    if(m_warnings) {
      cerr << "there where " << m_warnings << " warnings." << endl;
      exit(-1);
    }

    upload_rooms();
    upload_items();
    upload_characters();
    upload_globals();
  }


  /****************\
   *              *
   *  the parser  *
   *              * 
  \****************/

  Loader::Parser::Parser( const std::string &fn ) : 
    m_line_number(0),
    m_file( fn.c_str() )
    { }

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
      if(boost::regex_match( m_line, m_capture, Loader::s_blank_r) ) continue;

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
