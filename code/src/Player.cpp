#include "Player.h"

Player::Player()
{
    //Why...

}

Player::Player(string name, sf::Vector2f initialPosition, string spritePath, bool isPlayer)
{
    Entity();

    playerName = name;
    alive = false;

    //Graphics Garbage
    texture.loadFromFile(spritePath);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0,0,16,16));
    renderTexture.create(128,128,true);
    renderTexture.setSmooth(false);

    camera = new Camera();



    entityID = 1;
    for (size_t i=0; i < name.length(); i++){
        entityID +=name.at(i)*i;
        }
    entityID = sqrt(entityID*entityID)*-1;//entityID's for players are negative

    position.x = initialPosition.x;
    position.y = 0;
    position.z = initialPosition.y;
    angleDeg = 0;

    camera->position = position;
    camera->angleDeg = angleDeg;
    isClient = isPlayer;
    updateID = 0;

    //Player Values (load these from a config in the future)
    frictionCoef = 8.0f;
}

void Player::Update(sf::Time elapsed)
{
    Entity::Update(elapsed);

    if(isClient)
    {
        position.x += velocity.x * elapsed.asSeconds();
        position.z += velocity.y * elapsed.asSeconds();

        velocity -= (frictionCoef*velocity) * elapsed.asSeconds();
        if(sqrt(velocity.x*velocity.x + velocity.y * velocity.y)<0.1f) velocity = sf::Vector2f(0,0);

        //add latest to networker;
    }
    else
    {
        //Temporarily jump to random frame on sprite sheet to make sure the RTT system works -RL
        //TODO Animastion system
        if(rand()%100 < 10){
           int random = rand()%5;
           sprite.setTextureRect(sf::IntRect(random*16,0,16,16));
        }

        //get Latest position from networker
    }

    camera->position = position;
    camera->angleDeg = angleDeg;
}

void Player::Render(Camera* activeCamera)
{
    //Entity::Render(activeCamera);

    if(!isClient)
    {


        //Texture Render Target
        // This stuff breaks rendering in weird ways. Really buggy. Have to manually activate targets for some reason
        activeCamera->activeWindow->setActive(false);
            renderTexture.setActive(true);
                glEnable (GL_BLEND);
                renderTexture.clear(sf::Color::Transparent);
                renderTexture.setSmooth(false);
                sprite.setScale(8,8);
                renderTexture.draw(sprite);
#ifdef DEBUG
                sf::Text debugTxt;
                char debugTXT[128];
                sprintf(debugTXT,"x:%.1f z:%.1f",position.x, position.z);
                debugTxt.setString(playerName + debugTXT);
                debugTxt.setColor(sf::Color(255, 0, 0, 255));
                debugTxt.setPosition(0.f, 0.f);
                debugTxt.setCharacterSize(18);
                //debugTxt.setScale(0.1f,0.1f);
                renderTexture.draw(debugTxt);
#endif
                renderTexture.display();
            renderTexture.setActive(false);
        activeCamera->activeWindow->setActive(true);



        //Render Billboard
        glPushMatrix();
            //glLoadIdentity();

            glTranslatef(-position.x, 0, -position.z);
            //glRotatef(-activeCamera->angleDeg,0,1,0);   //holy fake billboarding -RL
            glRotatef(Utilities::billboardAngle(position,activeCamera->position),0,1,0);

            float size = 0.5f;
            glEnable(GL_TEXTURE_2D);
            renderTexture.getTexture().bind(); //Binding RenderTarget
            glColor4f(1.f, 1.f, 1.f, 1.f);
            glBegin(GL_QUADS);
                    /*
                    glTexCoord2f(0, 1); glVertex3f(-size, -size, -size);
                    glTexCoord2f(0, 0); glVertex3f(-size,  size, -size);
                    glTexCoord2f(1, 0); glVertex3f( size,  size, -size);
                    glTexCoord2f(1, 1); glVertex3f( size, -size, -size);
                    */
                    glTexCoord2f(0, 1); glVertex3f(-size, -size, 0);
                    glTexCoord2f(0, 0); glVertex3f(-size,  size, 0);
                    glTexCoord2f(1, 0); glVertex3f( size,  size, 0);
                    glTexCoord2f(1, 1); glVertex3f( size, -size, 0);
            glEnd();

        glPopMatrix();
        //cout<<glGetError();
    }
    else
    {
        //Dunno what we're rendering if we're the player
    }

}

Player::~Player(){
    //deal with it later
}
