
#include "input_handler.hh"

#include <ctype.h>
#include <string.h>

#include <string>
#include <vector>
#include <iostream>

/* TODO: make this more C++ ish */

using namespace std;

namespace ta {
  
  InputHandler::InputHandler() {
  }

  void InputHandler::read() {

    m_words.clear();

    string input;

    while( m_words.size() == 0 ) {

      cout << "> ";

      //cin.ignore();
      getline( cin, input );

      char word_buffer[1024];
      int  bp = 0;
      const char *c = input.c_str();
      //bool sp = isspace(*c);

      memset( word_buffer, 0, sizeof(word_buffer) );

      while(*c) {

        if( isspace(*c) ) {
          if(bp) {
            m_words.push_back( string( word_buffer ) );
            bp = 0;
            memset( word_buffer, 0, sizeof(word_buffer) );
          }
        } else {
          word_buffer[bp++] = *c;
        } 

        c++; 
      }

      if(bp) {
        m_words.push_back( string( word_buffer ) );
      }
    }

  }

  string InputHandler::word(int i) {

    if( i < 0 || i >= m_words.size() ) return string();

    return m_words[i];

  }
};
