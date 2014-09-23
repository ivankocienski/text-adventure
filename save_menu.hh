
#pragma once

#include <vector>
#include <string>
#include <boost/filesystem.hpp>

#include "interface.hh"
#include "engine.hh"

namespace ta {

  class SaveSlot {
  private:

    int  m_id;
    bool m_active;

    boost::filesystem::path m_filename;
    std::string m_date_started;
    std::string m_room_in;

  public:

    SaveSlot();
    SaveSlot(const boost::filesystem::path&, int);

    std::string str();
    bool active();
    void try_load(  );

    bool save_engine_state( Engine& );
    bool load_engine_state( Engine& );
  };

  class SaveMenu {
  public:

    enum response_code {
      RC_UNDEFINED,
      RC_PLAY_GAME,
      RC_CANCEL
    };

  private:

    Interface &m_interface;
    Engine &m_engine;

    std::vector<SaveSlot> m_slots;
    
    response_code m_response;

    void rescan();


  public:

    
    response_code response();

    SaveMenu(Interface&, Engine&);

    void show_save();
    void show_load();
  };

};
