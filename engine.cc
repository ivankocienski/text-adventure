
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>

#include "loader.hh"
#include "engine.hh"
#include "room.hh"
#include "interface.hh"

using namespace std;

namespace ta {

  Engine::Engine(Interface &i) : m_interface(i), m_player( &m_world ) {
    reset();
  }

  void Engine::reset() {
    Loader loader( &m_world, &m_player );
    loader.parse( "game.world" );
    //TODO: reset player, world
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
}; // namespace ta
