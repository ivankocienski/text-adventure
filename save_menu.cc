
#include <iostream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "interface.hh"
#include "engine.hh"
#include "save_menu.hh"

#define itos(n) (boost::lexical_cast<std::string>((n)))

namespace ta {

  SaveSlot::SaveSlot() { 
    m_active = false;
  }

  SaveSlot::SaveSlot( int id ) {
    m_active = false;
    m_id = id;
  }

  bool SaveSlot::active() {
    return m_active;
  }

  std::string SaveSlot::str() {

    std::string str;

    str = itos(m_id) + ". ";
    if( !m_active ) {
      str += "not saved";
    }

    return str;
  }
  
  void SaveSlot::try_load( const boost::filesystem::path &p ) {
    std::string slotfile = "slot-" + itos(m_id);
    boost::filesystem::path my_path = p / slotfile;
    std::cout << "my_path=" << my_path << std::endl;

    if( !boost::filesystem::exists(my_path) ) {
      m_active = false;
      return;
    }

    // now attempt to peek inside and see what its about...

    m_active = true;




  }

  /*
   *
   *  save menu stuff
   *
   */
  
  SaveMenu::SaveMenu( Interface &i, Engine &e ) : m_interface(i), m_engine(e) {

    m_save_path  = boost::filesystem::current_path();
    m_save_path /= "saves";

    if( !boost::filesystem::is_directory(m_save_path) ) {
      if( !boost::filesystem::create_directory(m_save_path ) ) {
        std::cout << "error: could not create directory " << m_save_path  << std::endl;
      }
    }

    std::cout << "using save dir " << m_save_path << std::endl;

    m_slots.resize(10);

    for(int i = 0; i < 10; i++)
      m_slots[i] = SaveSlot(i);
  }

  void SaveMenu::rescan() {

    for( int i = 0; i < 10; i++ ) {
      m_slots[i].try_load( m_save_path );
    }

  }

  void SaveMenu::show_save() {
    rescan();


  }

  void SaveMenu::show_load() {
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


    }
  }

};

