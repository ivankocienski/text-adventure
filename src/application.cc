
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include "application.hh"
#include "interface.hh"

#include "save_menu.hh"

using namespace std;

namespace ta {
  
  Application::Application( Interface& i ) : m_interface(i), m_engine(i) {
    m_loop_mode   = LM_SPLASH;
    m_game_active = false;
  }

  void Application::show_splash() {

    m_interface.puts( 2, "Adventure engine 0.0" );
    m_interface.puts( 1, "By IK (c) 2014" );
    m_interface.puts( 1, "" );
    m_interface.puts( 1, "" );

    m_interface.press_any_key();

    m_loop_mode = LM_MAIN_MENU;
  }

  void Application::main_menu() {

    boost::regex number_r( "^\\s*(\\d+)\\s*$" );
    boost::smatch matches;

    while( m_loop_mode == LM_MAIN_MENU ) {

      int c = m_game_active ? 7 : 6;

      m_interface.puts( 1, "" );
      m_interface.puts( 7, "]=== Main Menu ===[" );
      m_interface.puts( 7, "  1. new game" );
      m_interface.puts( c, "  2. resume game" );
      m_interface.puts( 7, "  3. load game" );
      m_interface.puts( c, "  4. save game" );
      m_interface.puts( 7, "  5. help!" );
      m_interface.puts( 7, "  6. quit" );
      m_interface.puts( 7, "" );
      m_interface.puts( 7, "Your input (1-6) >" );

      int option;
      
      while(true ) {

        string input = m_interface.wait_for_input();

        if( boost::regex_match( input, matches, number_r ) ) {
          option = boost::lexical_cast<int>(matches[0]);
          break; 
        }

        m_interface.puts( 1, "Was not a number" ); 
      }

      cout << "option=" << option << endl;

      switch( option ) {

      // new
      case 1:
        if( m_game_active )
          m_loop_mode = LM_CONFIRM_NEW;
        else
          m_loop_mode = LM_PLAY_GAME;      
        break; 

      // resume
      case 2:
        if( m_game_active ) m_loop_mode = LM_LOAD_MENU;
        break;

      // load
      case 3:
        if( m_game_active ) 
          m_loop_mode = LM_CONFIRM_LOAD;
        else
          m_loop_mode = LM_LOAD_MENU; 
        break; 

      // save
      case 4:
        if( m_game_active ) m_loop_mode = LM_SAVE_MENU;
        break;

      // help
      case 5:
        m_loop_mode = LM_SHOW_HELP;
        break;

      // quit
      case 6:
        m_loop_mode = LM_CONFIRM_QUIT;
        break;

      default:
        m_interface.puts( 1, "Did not understand input, must be between 1 and 6" ); 
        break;
      }
    } 
  }

  void Application::play_game() {

    m_engine.run(); 

    m_game_active = m_engine.is_running();
  }

  void Application::show_help() {

    if( m_help.empty() ) {
      
      m_help.push_string( "YES, THIS IS HELP" );

    }

    m_help.show( m_interface );

    m_loop_mode = LM_MAIN_MENU;
  }

  void Application::load_game() {

    SaveMenu sm( m_interface, m_engine );

    sm.show_load();

    if( sm.response() == SaveMenu::RC_CANCEL ) {
      m_loop_mode = LM_MAIN_MENU;
    }

    if( sm.response() == SaveMenu::RC_PLAY_GAME ) {
      m_loop_mode = LM_PLAY_GAME;
    }
  }

  void Application::save_game() {

    SaveMenu sm( m_interface, m_engine );

    sm.show_save();
  }

  void Application::confirm_quit() {

    while(true) {

      m_interface.puts( 7, "Quit? Are you sure (yes or no)?" );

      string input = m_interface.wait_for_input();

      if( input == "yes" ) {
        m_loop_mode = LM_HALT;
        return;
      }

      if( input == "no" ) {
        m_loop_mode = LM_MAIN_MENU;
        return;
      }
    } 
  }

  void Application::confirm_new() {

    while(true) {

      m_interface.puts( 7, "Game is running, start new anyway (yes or no)?" );

      string input = m_interface.wait_for_input();

      if( input == "yes" ) {
        m_engine.reset();
        m_loop_mode = LM_PLAY_GAME;
        return;
      }

      if( input == "no" ) {
        m_loop_mode = LM_MAIN_MENU;
        return;
      }
    } 
  }

  void Application::confirm_load() {

    while(true) {

      m_interface.puts( 7, "Game is running, load game anyway (yes or no)?" );

      string input = m_interface.wait_for_input();

      if( input == "yes" ) {
        m_loop_mode = LM_LOAD_MENU;
        return;
      }

      if( input == "no" ) {
        m_loop_mode = LM_MAIN_MENU;
        return;
      }
    } 
  }

  int Application::main() {

    while( true ) {

      switch( m_loop_mode ) {
        case LM_HALT:  
          return 0;

        case LM_SPLASH:    show_splash(); break;
        case LM_MAIN_MENU: main_menu();   break;
        case LM_PLAY_GAME: play_game();   break;

        case LM_LOAD_MENU: load_game();   break;
        case LM_SAVE_MENU: save_game();   break;

        case LM_SHOW_HELP: show_help();   break;

        case LM_CONFIRM_QUIT: confirm_quit(); break;
        case LM_CONFIRM_LOAD: confirm_load(); break;
        case LM_CONFIRM_NEW:  confirm_new();  break;
      }
    }

    return 0;
  }

};

