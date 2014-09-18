
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

    Loader loader( &m_world, &m_player );

    loader.parse( "game.world" );
  }

  void Engine::announce() {

    m_interface.puts( "ADVENTURE (v0)" );

    m_world.introduce(m_interface);
    m_interface.puts("");
  }

  void Engine::show_help() {

    cout 
      << "commands help:" << endl
      << "  quit - exit" << endl
      //<< "  list - list current rooms in world" << endl
      << "  describe - where you are" << endl
      << endl;
  } 

  void Engine::handle_input( const vector<string> &words ) {

    if(words[0] == "help") {
      show_help();
      return;
    }

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

    if(words[0] == "intro") {
      m_world.introduce( m_interface );
      return;
    }

    if(words[0] == "describe") {
      m_player.describe( m_interface );
      return;
    }

    if(words[0] == "go") {
      m_player.go( words, m_interface );
      m_player.describe( m_interface );
      return;
    }

    if(words[0] == "quit") {
      //...
      exit(0);
    }

    m_interface.puts( 1, "Did not understand you" );

  }

  void Engine::run() {

    boost::regex space_r( "\\s+" );
    vector<string> words;

    announce();
    m_player.where( m_interface );

    while(true) {

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
}
