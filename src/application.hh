
#pragma once

#include "interface.hh"
#include "description.hh"
#include "engine.hh"

namespace ta {
  
  class Application {
  private:

    enum loop_mode {
      LM_SPLASH,
      LM_MAIN_MENU,

      LM_PLAY_GAME,
      
      LM_LOAD_MENU,
      LM_SHOW_HELP,
      LM_SAVE_MENU,

      LM_CONFIRM_NEW,  // if game active
      LM_CONFIRM_LOAD, // if game active
      LM_CONFIRM_QUIT, // always

      LM_HALT
    };

    Interface  &m_interface;
    Engine      m_engine;
    bool        m_game_active;

    Description m_help;

    loop_mode   m_loop_mode;

    void show_splash();
    void main_menu();
    void play_game();
    void show_help();

    void load_game();
    void save_game();

    void confirm_quit();
    void confirm_new();
    void confirm_load();

  public:

    Application(Interface&);

    int main();
  };
};

