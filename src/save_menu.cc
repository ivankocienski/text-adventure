
#include <iostream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/regex.hpp>

#include "interface.hh"
#include "engine.hh"
#include "save_menu.hh"

#define itos(n) (boost::lexical_cast<std::string>((n)))

namespace ta {

  SaveSlot::SaveSlot() { 
    m_active = false;
  }

  SaveSlot::SaveSlot(const boost::filesystem::path& p, int id ) {

    m_id     = id;
    m_active = false;

    std::string slotfile = "slot-" + itos(m_id);
    m_filename = p / slotfile;

    std::cout << "slot " << m_filename << std::endl; 
  }

  bool SaveSlot::active() {
    return m_active;
  }

  std::string SaveSlot::str() {

    std::string str;

    str = itos(m_id) + ". ";
    if( !m_active ) {
      str += "not saved";

    } else {
      str += m_date_started;
      str += " in ";
      str += m_room_in;
    }

    return str;
  }
  
  void SaveSlot::try_load( ) {

    if( !boost::filesystem::exists(m_filename) ) {
      m_active = false;
      return;
    }

    EngineState es( m_filename.native() );
    es.peek();
    if( !es.valid() ) return;

    // what do we keep here???
    //  current room.
    //  date game was started.

    m_date_started = es.get_start_date();
    m_room_in      = es.get_room_in();

    m_active = true;
  }

  bool SaveSlot::save_engine_state( Engine &e ) {
    EngineState es( m_filename.native() );
    return es.save( e );
  }

  bool SaveSlot::load_engine_state( Engine &e ) {
    EngineState es( m_filename.native() );
    return es.load( e ); 
  }

  /*
   *
   *  save menu stuff
   *
   */
  
  SaveMenu::SaveMenu( Interface &i, Engine &e ) : m_interface(i), m_engine(e) {

    boost::filesystem::path p;

    p  = boost::filesystem::current_path();
    p /= "saves";

    if( !boost::filesystem::is_directory(p) ) {
      if( !boost::filesystem::create_directory(p) ) {
        std::cout << "error: could not create directory " << p << std::endl;
      }
    }

    std::cout << "using save dir " << p << std::endl;

    m_slots.resize(10);

    for(int i = 0; i < 10; i++)
      m_slots[i] = SaveSlot(p, i);
  }

  void SaveMenu::rescan() {

    for( int i = 0; i < 10; i++ ) {
      m_slots[i].try_load();
    }
  }

  void SaveMenu::show_save() {

    boost::regex number_r( "^\\s*(\\d+)\\s+$" );
    boost::smatch capture;

    m_response = RC_CANCEL;

    rescan();
    
    //TODO: remember previous save from session.

    while(true) {

      m_interface.puts( 1, "" );
      m_interface.puts( 7, "Save game" );
      
      for( int i = 0; i < 10; i++) {
        int c = m_slots[i].active() ? 6 : 7;
        m_interface.puts( c, m_slots[i].str() );
      }

      m_interface.puts( 7, "x. cancel" );

      std::string input = m_interface.wait_for_input();

      if( input[0] == 'x' ) {
        return; 
      }

      if( boost::regex_match( input, capture, number_r ) ) {
        int n = boost::lexical_cast<int>(capture[0]);

        if( n >= 0 && n <= 9 ) {
          if( m_slots[n].save_engine_state( m_engine ) ) {
            m_interface.puts( 3, "Game has been saved to slot " + capture[0] );
            return;
          }
        }
      }
    }
  }

  void SaveMenu::show_load() {

    boost::regex number_r( "^\\s*(\\d+)\\s+$" );
    boost::smatch capture;

    m_response = RC_CANCEL;

    rescan();

    while(true) {

      m_interface.puts( 1, "" );
      m_interface.puts( 7, "Load game" );
      
      for( int i = 0; i < 10; i++) {
        int c = m_slots[i].active() ? 7 : 6;

        m_interface.puts( c, m_slots[i].str() );
      }

      m_interface.puts( 7, "x. cancel" );

      std::string input = m_interface.wait_for_input();

      if( input[0] == 'x' ) {
        return; 
      }

      if( boost::regex_match( input, capture, number_r ) ) {
        int n = boost::lexical_cast<int>(capture[0]);

        if( n >= 0 && n <= 9 ) {
          if( m_slots[n].load_engine_state( m_engine ) ) {
            m_response = RC_PLAY_GAME;
            return;
          }
        }
        
        
      }
      


    }
  }

  SaveMenu::response_code SaveMenu::response() {
    return m_response;
  }

};

