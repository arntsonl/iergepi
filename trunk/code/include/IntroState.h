#pragma once

#include "Global.h"
#include "State.h"

#include <iostream>

class State;

class IntroState : public State
{
public:
    IntroState()
    {
        m_name = "Intro";
        m_bgTex.loadFromFile("resources/dummy.jpg");
        m_bgSpr = new sf::Sprite(m_bgTex);
        m_start = false;
    }
    ~IntroState()
    {
        delete m_bgSpr;
    }

    // All corresponding states can use these
    void Input(uint press, uint held, uint mpress, uint mheld, sf::Vector2i mpos, sf::Vector2i mdiff)
    {
        if ( press & KEY_U )
        {
            m_start = true;
        }
    }

    uint Update(sf::Time elapsed)
    {
        if ( m_start == true)
            return STATE_NEXT;
        return STATE_IDLE;
    } // do some updating

    void Render(sf::RenderWindow * window)
    {
        // Draw the background
        window->pushGLStates();
        window->draw(*m_bgSpr);
        window->popGLStates();
    } // render things onto the screen

    void Reset(sf::Vector2i)
    {

    } // this should happen at the beginning of a state change
private:

    sf::Texture m_bgTex;
    sf::Sprite * m_bgSpr;

    bool m_start;
};
