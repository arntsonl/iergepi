#include "GameState.h"


GameState::GameState()
{
    entManager = new EntityManager();

    //Test Entities
    entManager->AddPlayer("TestPlayer", new Player("TestPlayer", sf::Vector2f(0,0),"resources/Charsheet.png",  true));
    entManager->AddPlayer("TestOpponent", new Player("TestOpponent", sf::Vector2f(0,0),"resources/Charsheet.png",  false));

    //Filling up the space with random opponent entities
    for(int i = 0; i < 40; i++){
        entManager->AddPlayer("TestOpponent"+i, new Player("TestOpponent"+i, sf::Vector2f(rand()%20-10,rand()%20-10),"resources/Charsheet.png",  false));
    }


    clientPlayer = ((Player*)entManager->GetPlayer("TestPlayer"));
    activeCamera = clientPlayer->getCamera();

}

GameState::~GameState()
{
//TODO
}

void GameState::Input(uint press, uint held, uint mpress, uint mheld, sf::Vector2i mpos)
{

    // Some temporary crap just moving forward/backwards, will need to put back in mouse angle based movement
        if ( press & KEY_U || held & KEY_U)
        {
            cout<<"Pressing Forward\n";
			 clientPlayer->SetVelocity(sf::Vector2f(0,10));
        }
		else if ( press & KEY_D || held & KEY_D)
        {
            cout<<"Pressing Backward\n";
			 clientPlayer->SetVelocity(sf::Vector2f(0,-10));
        }

        if ( press & KEY_L || held & KEY_L)
        {
            clientPlayer->AddAngle(-1.f);
        }
		else if ( press & KEY_R || held & KEY_R)
        {
			clientPlayer->AddAngle(1.f);
        }
}

uint GameState::Update(sf::Time elapsed)
{
    entManager->Update(elapsed);
    return STATE_IDLE;
}

void GameState::Render(sf::RenderWindow * window)
{
    activeCamera->activeWindow = window;

    glPolygonMode(GL_FRONT, GL_FILL);

    //Setup FPS Drawing | Why do I need to do this again?  Gotta walk down the pipeline- RL
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, window->getSize().x/window->getSize().y, 0.1, 100);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    sf::Vector3f cameraPos = activeCamera->position;

    //gluLookAt(cameraPos.x,cameraPos.y,cameraPos.z, cameraAngle.x, cameraAngle.y, cameraAngle.z, 0,1,0);

    glRotatef(activeCamera->angleDeg,0,1,0);
    glTranslatef(cameraPos.x, cameraPos.y, cameraPos.z);


    //DRAW SKYBOX
    //window->pushGLStates();
    //window->draw(*m_bgSpr);
    //window->popGLStates();




    //DRAW LEVEL
    //Temporary Floor
    //window->pushGLStates();
        //glTranslatef(0,-1,0);
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_QUADS);
            glVertex3f(-50.0f, -1.0f, -50.0f);
            glVertex3f(-50.0f, -1.0f,  50.0f);
            glVertex3f( 50.0f, -1.0f,  50.0f);
            glVertex3f( 50.0f, -1.0f, -50.0f);
        glEnd();
        glTranslatef(0,0,0);
    //window->popGLStates();

    //DRAW ENTITIES
    entManager->Render(activeCamera);



    //DRAW HUD
    window->pushGLStates();

        //Active Opponent Texture
        Player* temp = (Player*)entManager->GetPlayer("TestOpponent");
        window->draw(sf::Sprite(temp->renderTexture.getTexture()));

        sf::Text text("in game state");
        text.setColor(sf::Color(255, 255, 255, 200));
        text.setPosition(250.f, 450.f);
        window->draw(text);
    window->popGLStates();
}

void GameState::Reset(sf::Vector2i mpos)
{

}
