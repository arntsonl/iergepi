#pragma once

// System includes
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Local includes
#include "Global.h"

enum
{
    STATE_IDLE = 0,
    STATE_NEXT,
    STATE_PREV,
    STATE_QUIT,
};

class State
{
public:
    State(){}
    ~State(){}

    // These are unique to state
    State * next(){return m_next;}
    State * prev(){return m_prev;}
    void next(State* n){m_next = n;}
    void prev(State* p){m_prev = p;}
    std::string name(){return m_name;}

    // All corresponding states can use these
    virtual void Input(uint press, uint held, uint mpress, uint mheld, sf::Vector2i mpos) = 0;
    virtual uint Update(sf::Time) = 0; // do some updating
    virtual void Render(sf::RenderWindow *) = 0; // render things onto the screen
    virtual void Reset(sf::Vector2i) = 0; // this should happen at the beginning of a state change

    std::string m_name;  // name of the state
    State * m_next;  // next state
    State * m_prev; // previous state
};
