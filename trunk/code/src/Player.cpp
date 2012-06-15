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


    animation temp;
    vector< sf::IntRect > tempVector;
    temp.betweenFrames = sf::seconds(0.25f);
    temp.currentFrame = 0;

    //Face Front
    tempVector.push_back(sf::IntRect(0,0,16,16));
    temp.frames = tempVector;
    addAnimation("front",temp);
    tempVector.clear();

    //Face Right
    tempVector.push_back(sf::IntRect(16,0,16,16));
    temp.frames = tempVector;
    addAnimation("right",temp);
    tempVector.clear();

    //Face Back
    tempVector.push_back(sf::IntRect(32,0,16,16));
    temp.frames = tempVector;
    addAnimation("back",temp);
    tempVector.clear();

    //Face Left
    tempVector.push_back(sf::IntRect(48,0,16,16));
    temp.frames = tempVector;
    addAnimation("left",temp);
    tempVector.clear();

    //Ball
    tempVector.push_back(sf::IntRect(64,0,16,16));
    temp.frames = tempVector;
    addAnimation("ball",temp);
    tempVector.clear();

    currentAnim = "front";
    animationTimer.restart();


    //Player Values (load these from a config in the future)
    frictionCoef = 8.0f;
}

void Player::Jump()
{
    if(position.y == 0){
        jumpVelocity = 0.16;
        velocity *= 1.5f;
    }
}

void Player::Update(sf::Time elapsed)
{
    Entity::Update(elapsed);

    if(isClient)
    {
        if(velocity.x != 0 || velocity.y != 0) DIRTY = true;

        position.x += velocity.x * elapsed.asSeconds();
        position.z += velocity.y * elapsed.asSeconds();

        if(position.y == 0) velocity -= (frictionCoef*velocity) * elapsed.asSeconds();
        if(sqrt(velocity.x*velocity.x + velocity.y * velocity.y)<0.2f) velocity = sf::Vector2f(0,0);
        if(Utilities::getMagnitude(velocity.x, velocity.y, 0)> 15.0f) velocity = Utilities::normalize(velocity, 15.0f);

        position.y -= jumpVelocity;
        if(position.y >0) position.y = 0;
        if(position.y < 0)
        {
            jumpVelocity -= 0.75 * elapsed.asSeconds();
        }
        //add latest to networker;
    }
    else
    {

        //Increment Animation Frame
        if(animationTimer.getElapsedTime() > animations[currentAnim].betweenFrames)
        {
            animationTimer.restart();
            if(animations[currentAnim].currentFrame + 1 < animations[currentAnim].frames.size())
            {
               animations[currentAnim].currentFrame++;
            }
            else
            {
               animations[currentAnim].currentFrame = 0; //restart if non-existent
            }
        }
        sprite.setTextureRect(animations[currentAnim].frames[animations[currentAnim].currentFrame]); //lol -RL
    }

    while(angleDeg<0) angleDeg+=360;
    while(angleDeg>360) angleDeg-=360;
    camera->position = position;
    camera->angleDeg = angleDeg;

}

void Player::addAnimation(std::string animName, animation anim)
{
    animations[animName] = anim;
}

void Player::removeAnimation(std::string animName)
{
    animations.erase(animName);
}

void Player::Render(Camera* activeCamera)
{
    //Entity::Render(activeCamera);

    if(!isClient)
    {

    //Change Animation Based on Angle (this really shouldn't be in Render but fuck it; I need the activeCamera -RL )

        float angDiff = Utilities::billboardAngle(position,activeCamera->position)-90+angleDeg;
        while(angDiff<-180) angDiff+=360;
        while(angDiff>180) angDiff-=360;

        //cout<<angDiff<<endl;

        if((angDiff > -135 && angDiff < -45)    ||  (angDiff > 225 && angDiff < 315))   currentAnim = "front";
        if((angDiff > -225 && angDiff < -135)   ||  (angDiff > 135 && angDiff < 225))   currentAnim = "right";
        if((angDiff > -315 && angDiff < -225)   ||  (angDiff > 45  && angDiff < 135))   currentAnim = "back";
        if((angDiff > -45 &&  angDiff < 45)     ||  (angDiff > 315 || angDiff < -315))   currentAnim = "left";


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
                    glTexCoord2f(0, 1); glVertex3f(-size, -size-0.5, 0);
                    glTexCoord2f(0, 0); glVertex3f(-size,  size-0.5, 0);
                    glTexCoord2f(1, 0); glVertex3f( size,  size-0.5, 0);
                    glTexCoord2f(1, 1); glVertex3f( size, -size-0.5, 0);
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
