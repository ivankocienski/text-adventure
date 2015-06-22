
#pragma once 

#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <string>
#include <vector>
#include <map>

#include "player.hh"
#include "world.hh"
#include "interface.hh"

namespace ta {

  class Engine;

  /* EngineState is actually responsible for dumping / loading engine state to
     a file */
  class EngineState {
  private:

    boost::filesystem::path m_source_path;

    std::string m_game_started;
    std::string m_game_saved;
    std::string m_player_room_in;

    std::list<std::string> m_has_items;

    typedef std::map<std::string, std::list<std::string> > door_open_map_t;
    door_open_map_t m_door_open;
    //list<string> m_door_open;

    bool m_valid;

    // load helpers
    void parse_file();
    void validate(Engine&);
    void load_to_engine(Engine&);

  public:

    EngineState();
    EngineState( const boost::filesystem::path& );

    // load auxiliary data for menu
    void peek();

    // from disk to ram
    bool load( Engine& );

    // from ram to disk
    bool save( Engine& );

    bool valid();
    std::string get_start_date();
    std::string get_room_in();

  };

  class Engine {
    public:

      typedef boost::posix_time::ptime engine_time_t;

    private:

      Interface    &m_interface;
      Player        m_player;
      World         m_world;
      
      engine_time_t m_started_on;

      bool          m_game_running;

      void handle_input( const std::vector<std::string>& );

    public:

      Engine(Interface&);

      World& world();
      Player& player();

      // reset game to start
      void reset();

      // the main game loop
      void run();

      // has the game ended, did the player win/die?
      bool is_running();

      void set_start_date( const std::string& );
      std::string start_date();
  };

}; // namespace ta;

