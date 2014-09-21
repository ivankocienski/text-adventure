
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

  public:

    SaveSlot();
    SaveSlot(int);

    std::string str();
    bool active();
    void try_load( const boost::filesystem::path& );

  };

  class SaveMenu {
  private:

    Interface &m_interface;
    Engine &m_engine;

    boost::filesystem::path m_save_path;

    std::vector<SaveSlot> m_slots;
    
    void rescan();


  public:
    
    SaveMenu(Interface&, Engine&);

    void show_save();
    void show_load();
  };

};
