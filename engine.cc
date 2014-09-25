#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "loader.hh"
#include "engine.hh"
#include "room.hh"
#include "interface.hh"

using namespace std;

namespace ta {

  /*  things to load are
   
   date started.
   current room in.
   items in knapsack (remove them from world).
   doors openned (remove keys from world)
  */

  EngineState::EngineState() { }

  EngineState::EngineState( const boost::filesystem::path& p ) : m_source_path(p) { } 

  void EngineState::parse_file() {

    boost::regex line_r( "^(\\w+)\\s+(.*)$" );
    boost::smatch capture;
    int line_number = 0;
    int version_okay = 0; 
    string line;

    ifstream file( m_source_path.native().c_str() );

    // not as sophisticated as the loader

    while( !file.eof() ) {

      line_number++; 
      getline( file, line );

      // yeah, comments
      if( line[0] == '#' ) continue;

      if( boost::regex_match( line, capture, line_r ) ) {

        if( capture[0] == "VERSION" ) {

          if( capture[1] == "1" ) {
            version_okay = 1;

          } else {
            cerr << "warning: (line " << line_number << ") version not recognised" << endl;
            return;
          }
        }

        if( capture[0] == "START_DATE" ) {

          if( version_okay == 1 ) { 
            m_game_started = capture[1]; 
          } else {
            cerr << "warning: (line " << line_number << ") version not defined! ignoring" << endl;
          }
        }

        if( capture[0] == "IN_ROOM" ) {
          if( version_okay == 1 ) { 
            m_player_room_in = capture[1]; 
          } else {
            cerr << "warning: (line " << line_number << ") version not defined! ignoring" << endl;
          } 
        }

        if( capture[0] == "HAS_ITEM" ) {
          if( version_okay == 1 ) { 
            m_has_items.push_back( capture[1] );
          } else {
            cerr << "warning: (line " << line_number << ") version not defined! ignoring" << endl;
          } 
        }

        if( capture[0] == "DOOR_OPEN" ) {

          if( version_okay == 1 ) { 
            vector<string> parts;
            boost::split( parts, (const string &)capture[1], boost::is_any_of(" \t") );
            m_door_open[parts[0]].push_back(parts[1]);

          } else {
            cerr << "warning: (line " << line_number << ") version not defined! ignoring" << endl;
          } 
        }

        cerr << "warning: (line " << line_number << ") save had invalid word" << endl; 
      } 
    }
  }

  void EngineState::peek() { 
    m_valid = true; 
    parse_file();
  }


  void EngineState::validate( Engine &e ) {

    // check player is in a room that exists.
    Room *room = e.world().get_room(m_player_room_in);
    if( !room ) {
      m_valid = false;
      return;
    }

    // items exists
    for( list<string>::iterator it = m_has_items.begin(); it != m_has_items.end(); it++ ) {
      if( e.world().has_item(*it) ) continue;

      m_valid = false;
      return;
    }

    // rooms exists, doors within rooms exist
    door_open_map_t::iterator it;
    for( it = m_door_open.begin(); it != m_door_open.end(); it++ ) {

      string name = (*it).first;

      Room *room = e.world().get_room(name);
      if( !room ) {
        m_valid = false;
        return;
      }

      list<string> doors = (*it).second;

      list<string>::iterator door_it;
      for( door_it = doors.begin(); door_it != doors.end(); door_it++ ) {

        if( !room->has_exit(*door_it) ) {
          m_valid = false;
          return;
        }
      }

      m_valid = false;
      return;
    }

  }

  void EngineState::load_to_engine( Engine& e ) {

    Room *r = e.world().get_room( m_player_room_in );
    e.player().start_in( r );

    for( list<string>::iterator it = m_has_items.begin(); it != m_has_items.end(); it++ ) {
      e.player().give_item( *it );
    }

    door_open_map_t::iterator it;
    for( it = m_door_open.begin(); it != m_door_open.end(); it++ ) {

      string       name  = (*it).first;
      list<string> doors = (*it).second;
      Room         *room = e.world().get_room(name);

      list<string>::iterator door_it;
      for( door_it = doors.begin(); door_it != doors.end(); door_it++ ) {
        Exit &ex = room->exits()[name];
        ex.unlock();

        Item &item = e.world().get_item(ex.locked_with());
        item.set_room(NULL);
      } 
    } 
  }

  // from disk to ram
  bool EngineState::load( Engine &e ) {

    m_valid = true;

    parse_file();
    validate(e);

    if( m_valid ) {
      e.reset();
    
      load_to_engine(e); 

      return true;
    }

    return false;
  }

  bool EngineState::save( Engine &e ) {

    ofstream file( m_source_path.native().c_str() );

    file << "# adventure savefile" << endl;
    file << "# no peeking! cheat" << endl;
    file << endl;
    
    file << "VERSION 1 " << endl;
    file << endl;
    
    file << "START_DATE " << e.start_date() << endl;
    file << endl;

    file << "IN_ROOM " << e.player().current_room()->name() << endl;
    file << endl;

    {
      boost::unordered_set<std::string>::iterator it;
      for( it = e.player().knapsack().begin(); it != e.player().knapsack().end(); it++ )
        file << "HAS_ITEM " << *it << endl;
    }

    {
      World::room_map_t::iterator it;
      for( it = e.world().rooms().begin(); it != e.world().rooms().end(); it++ ) {

        Room::exit_map_t &exits = (*it).second.exits();
        Room::exit_map_t::iterator eit;

        for( eit = exits.begin(); eit != exits.end(); eit++ ) {

          if( (*eit).second.locked_with() == "" ) continue;

          file << "DOOR_OPEN " << (*it).first << " " << (*eit).first << endl; 
        } 
      }
    } 

    return true; 
  }

  string EngineState::get_start_date() {
    return m_game_started;
  }

  string EngineState::get_room_in() {
    return m_player_room_in;
  }

  bool EngineState::valid() {
    return m_valid;
  }
  
  /*
   *
   * The engine
   *
   */ 

  Engine::Engine(Interface &i) : m_interface(i), m_player( &m_world ) {
    reset();
  }

  void Engine::reset() {
    Loader loader( &m_world, &m_player );
    loader.parse( "game.world" );

    //TODO: reset player, world
    m_started_on = boost::posix_time::second_clock::local_time();
  }

  void Engine::handle_input( const vector<string> &words ) {

    if(words[0] == "holding" ) {
      m_player.show_holding(m_interface);
      return;
    }

    if(words[0] == "pickup" ) {
      m_player.pickup( words, m_interface );
      return;
    }

    if(words[0] == "putdown" ) {
      m_player.putdown( words, m_interface );
      return;
    }

    if(words[0] == "unlock" ) {
      m_player.unlock( words, m_interface );
      return;
    } 

    if(words[0] == "talk" ) {
      m_player.talk( words, m_interface );
      return;
    }

    if(words[0] == "describe") {

      if( words.size() == 1 || words[1] == "room" ) {
        m_player.describe_room( m_interface ); 
        return;
      }

      if( words[1] == "item" ) {
        if( words.size() != 3 ) {
          m_interface.puts( 1, "Which item should I describe to you?" );
          return;
        }

        m_player.describe_item( m_interface, words[2] );
        return;
      }


      if( words[1] == "exit" ) {
        if( words.size() != 3 ) {
          m_interface.puts( 1, "Which exit should I describe to you?" );
          return;
        }

        m_player.describe_exit( m_interface, words[2] );
        return;
      }

      if( words[1] == "person" ) {
        if( words.size() != 3 ) {
          m_interface.puts( 1, "Which person should I describe to you?" );
          return;
        }

        m_player.describe_person( m_interface, words[2] );
        return;
      }

      m_interface.puts( 1, "I do not know how to describe " + words[1] );
      return; 
    }

    if(words[0] == "go") {
      m_player.go( words, m_interface );
      return;
    }

    if(words[0] == "menu") {
      m_game_running = false;
      return;
    }

    m_interface.puts( 1, "Did not understand you" );
  }

  void Engine::run() {

    boost::regex space_r( "\\s+" );
    vector<string> words;

    m_player.where( m_interface );

    m_game_running = true;

    while(m_game_running) {

      string input = m_interface.wait_for_input();

      split_regex( words, input, space_r );

      if( words.size() == 0 ) continue;

      if( words[0] == "" ) words.erase( words.begin() ); // HACK

      if( words.size() == 0 ) continue;

      m_interface.puts( Interface::C_LIGHT_GREY, input );
      handle_input( words );
      m_interface.puts( "" );
    }


  }

  bool Engine::is_running() {
    return m_game_running;
  }

  World& Engine::world() {
    return m_world;
  }

  Player& Engine::player() {
    return m_player;
  }

  void Engine::set_start_date( const string &s ) {
    m_started_on = boost::posix_time::from_iso_string(s); 
  }

  std::string Engine::start_date() {
    return boost::posix_time::to_iso_string(m_started_on);
  }

}; // namespace ta
