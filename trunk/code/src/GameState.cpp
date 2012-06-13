#include "GameState.h"
#include <cstdio>


GameState::GameState(std::string playerNam, Networker* ne, Player* clientPlaye)
{
    playerName = playerNam;
    net = ne;
    clientPlayer = clientPlaye;

    entManager = new EntityManager();
    net->SetupUpdater(clientPlayer, entManager);
    //Test Entities
    entManager->AddPlayer(playerName, clientPlayer);
    entManager->AddPlayer("Test1", new Player("Test1", sf::Vector2f(4,4),"resources/Charsheet.png",  false));
    //entManager->AddPlayer("Test2", new Player("Test2", sf::Vector2f(4,-4),"resources/Charsheet.png",  false));
    //entManager->AddPlayer("Test3", new Player("Test3", sf::Vector2f(-4,4),"resources/Charsheet.png",  false));
    //entManager->AddPlayer("Test4", new Player("Test4", sf::Vector2f(-4,-4),"resources/Charsheet.png",  false));
    //Filling up the space with random opponent entities
    //char tempName[64];

    //for(int i = 0; i < 40; i++){
    //    sprintf(tempName, "Player-%d",rand());
    //    entManager->AddPlayer(tempName, new Player(tempName, sf::Vector2f(rand()%20-10,rand()%20-10),"resources/Charsheet.png",  false));
    //}


    //clientPlayer = ((Player*)entManager->GetPlayer("TestPlayer"));
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
            sf::Vector2f direction = sf::Vector2f(cos((activeCamera->angleDeg+90)*(3.14159265358979323846/180.0)),sin((activeCamera->angleDeg+90)*(3.14159265358979323846/180.0)));
            clientPlayer->SetVelocity(Utilities::normalize(direction, 10.f));
        }
		else if ( press & KEY_D || held & KEY_D)
        {
            sf::Vector2f direction = sf::Vector2f(cos((activeCamera->angleDeg+90)*(3.14159265358979323846/180.0)),sin((activeCamera->angleDeg+90)*(3.14159265358979323846/180.0)));
            clientPlayer->SetVelocity(Utilities::normalize(direction, -10.f));
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
    net->Update(elapsed);
    entManager->Update(elapsed);
    return STATE_IDLE;
}

void GameState::Render(sf::RenderWindow * window)
{
    activeCamera->activeWindow = window;
    window->setActive();
    //glPolygonMode(GL_FRONT, GL_LINE);

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
    //window->popGLStates();

    //DRAW ENTITIES
    entManager->Render(activeCamera);



    //DRAW HUD
    window->pushGLStates();

        //Active Opponent Texture
        Player* temp = (Player*)entManager->GetPlayer("Test1");
        window->draw(sf::Sprite(temp->renderTexture.getTexture()));
        sf::Text text;
        char debugTXT[128];
        sprintf(debugTXT,"Camera x:%.2f z:%.2f",activeCamera->position.x, activeCamera->position.z);
        text.setString(debugTXT);
        text.setColor(sf::Color(255, 255, 255, 200));
        text.setPosition(20.f, 40.f);
        window->draw(text);
    window->popGLStates();
}

void GameState::Reset(sf::Vector2i mpos)
{

}
