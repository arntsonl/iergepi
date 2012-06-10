#include "Game.h"

Game::Game() :
	m_quit(false)
{
	// Create the main window
	window = new sf::RenderWindow(sf::VideoMode(800, 600), "Dethsn4ke", sf::Style::Default, sf::ContextSettings(32));
    window->setVerticalSyncEnabled(true);
    window->setKeyRepeatEnabled(false);
}

Game::~Game()
{
    delete m_state;
    delete window;
}

void Game::Init()
{
    IntroState * introState = new IntroState();
    GameState * gameState = new GameState();
    m_state = (State*)introState;
    m_state->next(gameState);
    //m_statePool.push_back(m_state);

    keysDown = 0;

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearDepth(1.f);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);
}

void Game::Run()
{
	while(m_quit == false)
	{
		Update();
		Render();
		Tick();
	}
}

// Update everything running in the game
void Game::Update()
{
    uint ret;
    uint keysPressed = 0; // keys pressed
    uint keysHeld = 0; // keys held
    uint keysTmp = 0;
    uint mousePressed = 0;
    uint mouseHeld = 0;
    uint mouseTmp = 0;

    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

    // Reset our timing clock
    clock.restart();

	// Process events
	sf::Event event;
	while (window->pollEvent(event))
	{
		// Close window : exit
		if (event.type == sf::Event::Closed)
		{
			ShutDown();
			return;
		}

		// Escape key : exit
		if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
		{
			ShutDown();
			return;
		}

		// Get our inputs
		if (event.type == sf::Event::KeyPressed)
		{
		    if ( event.key.code == sf::Keyboard::W )
		    {
                keysTmp |= KEY_U;
		    }
		    else if ( event.key.code == sf::Keyboard::S )
		    {
                keysTmp |= KEY_D;
		    }
		    else if ( event.key.code == sf::Keyboard::A )
		    {
                keysTmp |= KEY_L;
		    }
		    else if ( event.key.code == sf::Keyboard::D )
		    {
                keysTmp |= KEY_R;
		    }
		}

        // Removed released keys
		if (event.type == sf::Event::KeyReleased )
		{
		    if ( event.key.code == sf::Keyboard::W )
		    {
                keysDown = keysDown & ~KEY_U;
		    }
		    else if ( event.key.code == sf::Keyboard::S )
		    {
                keysDown = keysDown & ~KEY_D;
		    }
		    else if ( event.key.code == sf::Keyboard::A )
		    {
                keysDown = keysDown & ~KEY_L;
		    }
		    else if ( event.key.code == sf::Keyboard::D )
		    {
                keysDown = keysDown & ~KEY_R;
		    }
		}

		if ( event.type == sf::Event::MouseButtonPressed )
		{

		}

		// Adjust the viewport when the window is resized
		if (event.type == sf::Event::Resized)
			glViewport(0, 0, event.size.width, event.size.height);
	}

    // Fix me
    keysPressed = keysTmp & ~keysDown;
    keysHeld = keysDown & ~keysPressed;
    keysDown = keysTmp | keysDown;

    m_state->Input(keysPressed, keysHeld, mousePressed, mouseHeld, mousePos);
	ret = m_state->Update(m_elapsedTime);
	switch(ret)
	{
        case(STATE_NEXT):
            m_state = m_state->next();
            m_state->Reset(mousePos);
            break;
        case(STATE_PREV):
            m_state = m_state->prev();
            m_state->Reset(mousePos);
            break;
        case(STATE_QUIT):
            ShutDown();
            return;
	    case(STATE_IDLE): // do nothing in an idle state
	    default:
            break;
	}
}

// Render everything in the game using SFML
void Game::Render()
{
	// Draw the background
	//window->pushGLStates();
	//window->draw(background);
	//window->popGLStates();

	// Activate the window before using OpenGL commands.
	// This is useless here because we have only one window which is
	// always the active one, but don't forget it if you use multiple windows
	//window->setActive();

	// Clear the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    m_state->Render(window);


	// We get the position of the mouse cursor, so that we can move the box accordingly
	float x =  sf::Mouse::getPosition(*window).x * 200.f / window->getSize().x - 100.f;
	float y = -sf::Mouse::getPosition(*window).y * 200.f / window->getSize().y + 100.f;

	// Apply some transformations
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glTranslatef(x, y, -100.f);
//	glRotatef(50.f, 1.f, 0.f, 0.f);
//	glRotatef(30.f, 0.f, 1.f, 0.f);
//	glRotatef(90.f, 0.f, 0.f, 1.f);
/*
	// Draw a cube
	float size = 20.f;
	glBegin(GL_QUADS);
		glColor3ub(255,0,255); glVertex3f(-size, -size, -size);
		glColor3ub(255,0,255); glVertex3f(-size,  size, -size);
		glColor3ub(255,0,255); glVertex3f( size,  size, -size);
		glColor3ub(255,0,255); glVertex3f( size, -size, -size);

		glColor3ub(0,255,255); glVertex3f(-size, -size, size);
		glColor3ub(0,255,255); glVertex3f(-size,  size, size);
		glColor3ub(0,255,255); glVertex3f( size,  size, size);
		glColor3ub(0,255,255); glVertex3f( size, -size, size);

		glColor3ub(255,255,0); glVertex3f(-size, -size, -size);
		glColor3ub(255,255,0); glVertex3f(-size,  size, -size);
		glColor3ub(255,255,0); glVertex3f(-size,  size,  size);
		glColor3ub(255,255,0); glVertex3f(-size, -size,  size);

		glColor3ub(0,0,255); glVertex3f(size, -size, -size);
		glColor3ub(0,0,255); glVertex3f(size,  size, -size);
		glColor3ub(0,0,255); glVertex3f(size,  size,  size);
		glColor3ub(0,0,255); glVertex3f(size, -size,  size);

		glColor3ub(255,0,0); glVertex3f(-size, -size,  size);
		glColor3ub(255,0,0); glVertex3f(-size, -size, -size);
		glColor3ub(255,0,0); glVertex3f( size, -size, -size);
		glColor3ub(255,0,0); glVertex3f( size, -size,  size);

		glColor3ub(0,255,0); glVertex3f(-size, size,  size);
		glColor3ub(0,255,0); glVertex3f(-size, size, -size);
		glColor3ub(0,255,0); glVertex3f( size, size, -size);
		glColor3ub(0,255,0); glVertex3f( size, size,  size);

	glEnd();
*/
	// Finally, display the rendered frame on screen
	window->display();
}

// Wait for the next frame
void Game::Tick()
{
    m_elapsedTime = clock.getElapsedTime();
    while ( m_elapsedTime.asMicroseconds() < FPS_MICRO )
    {
        sf::sleep(sf::microseconds(500)); // wait 0.5 ms, check again
        m_elapsedTime = clock.getElapsedTime();
    }
}

// Take care of memory clean-ups, window handling, etc.
void Game::ShutDown()
{
	window->close();
	m_quit = true;
}
