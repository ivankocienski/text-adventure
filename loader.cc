
#include <stdlib.h>

#include <string>
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>

#include "loader.hh"

using namespace std;


namespace ta { 

  Loader::Loader( World& w, Player& p) : m_ptr_world(&w), m_ptr_player(&p) { 
    m_warnings = 0;
  }

  void Loader::moan( size_t l, const std::string& r ) {
    cerr << "warning: (line " << l << ") " << r << endl;
    m_warnings++;
  }

  void Loader::set_decleration( decleration_t &t, boost::smatch &sm, size_t line ) {

    if( t.assigned ) { 
      moan( line, "assignment already assigned" );
      return; 
    }

    for(size_t i = 1; i < sm.size(); i++) 
      t.parts.push_back( sm[i].str() );

    t.line     = line;
    t.assigned = true;
  }

  void Loader::parse( const string &fn ) { 

    // not the best way to write a parser, but hey...
    boost::regex       blank_r( "^\\s*$" );
    boost::regex        name_r( "^%NAME\\s+(.+)" );
    boost::regex      author_r( "^%AUTHOR\\s+(.+)" );
    boost::regex   copyright_r( "^%COPYRIGHT\\s+(.+)" );
    boost::regex     include_r( "^%INCLUDE\\s+(.+)" );
    boost::regex       start_r( "^%START\\s+(.+)" );
    boost::regex        room_r( "^%ROOM\\s+(.+)" );
    boost::regex    describe_r( "^%DESCRIBE" );
    boost::regex        exit_r( "^%EXIT\\s+(\\w+)\\s+(\\w+)" );
    boost::regex enddescribe_r( "^%ENDDESCRIBE" );
    boost::regex     endroom_r( "^%ENDROOM" );
    boost::smatch capture;

    string current_room;

    ifstream file( fn.c_str() );

    string line;
    int line_num = 0;

    read_state state = RS_TOPLEVEL;

    while(!file.eof()) {

      line_num++;

      getline(file, line);

      string::size_type start = line.find_first_not_of( " \t\n\r" );

      if( start != string::npos && start > 0 ) {
        line.erase(0, start); 
      }

      if(boost::regex_match( line, capture, blank_r) ) continue;
      if(line[0] == '#') continue;

      if( state == RS_DESCRIPTION ) {

        if( line[0] == '%' ) {

          if( boost::regex_match(line, capture, enddescribe_r ) ) {
            state = RS_ROOM;
            continue;
          }

          moan( line_num, "command found in DESCRIBE block is not ENDDESCRIBE" );
          continue; 
        }

/*         if( !m_rooms[current_room].description.assigned ) {
 *           m_rooms[current_room].description.assigned = true;
 *           m_rooms[current_room].description.line = line_num;
 *         }
 */

        m_rooms[current_room].description.parts.push_back( line );

      } else {

        if(line[0] == '%') {

          if( boost::regex_match(line, capture, name_r) ) {
            if( state == RS_TOPLEVEL )
              set_decleration( m_globals.name, capture, line_num );
            else
              moan( line_num, "unexpected NAME keyword" );
            continue;
          }

          if( boost::regex_match(line, capture, author_r ) ) {
            if( state == RS_TOPLEVEL ) 
              set_decleration( m_globals.author, capture, line_num );
            else
              moan( line_num, "unexpected AUTHOR keyword" ); 
            continue; 
          }

          if( boost::regex_match(line, capture, copyright_r ) ) {
            if( state == RS_TOPLEVEL ) 
              set_decleration( m_globals.copyright, capture, line_num );
            else
              moan( line_num, "unexpected COPYRIGHT keyword" ); 

            continue; 
          }

          if( boost::regex_match(line, capture, include_r ) ) {
            //TODO: ignore these until implemented
            continue; 
          }

          if( boost::regex_match(line, capture, start_r ) ) {
            if( state == RS_TOPLEVEL ) 
              set_decleration( m_globals.start, capture, line_num );
            else
              moan( line_num, "unexpected START keyword" ); 
            continue; 
          }

          if( boost::regex_match(line, capture, room_r ) ) {
            if( state == RS_TOPLEVEL ) {

              room_t load_room;
              string name = capture[1].str();

              set_decleration( m_rooms[name].name, capture, line_num );

              current_room = name; 
              state        = RS_ROOM;

            } else
              moan( line_num, "unexpected START keyword" ); 

            continue; 
          }

          if( boost::regex_match(line, capture, describe_r ) ) {
            if( state == RS_TOPLEVEL ) {
              moan( line_num, "unexpected DESCRIBE keyword" ); 
            } else {
              set_decleration( m_rooms[current_room].description, capture, line_num );
              state = RS_DESCRIPTION;
            }

            continue; 
          }

          if( boost::regex_match(line, capture, exit_r ) ) {
            if( state == RS_TOPLEVEL ) {
              moan( line_num, "unexpected EXIT keyword" ); 
            } else {

              if( capture[1].str() == "north" ) {
                set_decleration( m_rooms[current_room].exit_north, capture, line_num);
                continue; 
              }

              if( capture[1].str() == "south" ) {
                set_decleration( m_rooms[current_room].exit_south, capture, line_num);
                continue; 

              }

              if( capture[1].str() == "east" ) {
                set_decleration( m_rooms[current_room].exit_east, capture, line_num);
                continue; 

              }

              if( capture[1].str() == "west" ) {
                set_decleration( m_rooms[current_room].exit_west, capture, line_num);
                continue; 
                
              }

              moan( line_num, "EXIT has unknown direction. Should be one of north, south, east or west" );
              
            }

            continue; 
          }

          if( boost::regex_match(line, capture, enddescribe_r ) ) {
            moan( line_num, "unexpected ENDDESCRIBE keyword" ); 
            continue; 
          }

          if( boost::regex_match(line, capture, endroom_r ) ) {
            if( state == RS_TOPLEVEL ) {
              moan( line_num, "unexpected ENDROOM keyword" ); 
            } else {
              state        = RS_TOPLEVEL;
              current_room = "";
            }

            continue; 
          }

          moan( line_num, "unknown or misspelled command given, ignoring." );
          continue;
        }

        moan( line_num, "syntax error" ); 
      } 
    } 
  }

  void Loader::upload() {

    m_world->name      = m_globals.name.parts[0];
    m_world->author    = m_globals.author.parts[0];
    m_world->copyright = m_globals.copyright.parts[0];

    for( room_map_t::iterator ri = m_rooms.begin(); ri != m_rooms.end(); ri++ ) {

      decleration_t &dec = ri.second;

      m_world->build_room(
        dec.name.parts[0],
        dec.descroption.parts[0] 
      );
    }

    for( room_map_t::iterator ri = m_rooms.begin(); ri != m_rooms.end(); ri++ ) {

      decleration_t &dec = ri.second;

      if( dec.exit_north.assigned ) {

      }

      if( dec.exit_south.assigned ) {

      }

      if( dec.exit_east.assigned ) {

      }

      if( dec.exit_west.assigned ) {
        
      }

      moan( dec.name.line, "no exits defined in room" );
    }

    m_player->start_in();


    
  }

  void Loader::read( const string &fn ) {

    parse( fn );

    if(m_warnings) {
      cerr << "there where " << m_warnings << " warnings." << endl;
      exit(-1);
    }

    upload();
  }

}; // namespace ta
