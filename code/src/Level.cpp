#include "Level.h"

    Level::Level(std::string levName, std::string spritePath)
    {
        levelName = levName;

        texture.setRepeated(true);
        texture.loadFromFile(spritePath);


        floorSprite.setTexture(texture);
        floorSprite.setTextureRect(sf::IntRect(0,0,16,16));
        wallSprite.setTexture(texture);
        wallSprite.setTextureRect(sf::IntRect(32,0,16,16));
        ceilingSprite.setTexture(texture);
        ceilingSprite.setTextureRect(sf::IntRect(16,0,16,16));

        floorTexture.create(128,128,true);
        ceilingTexture.create(128,128,true);
        wallTexture.create(128,128,true);

        //Render out skybox Textures
        sf::Texture skyboxTexture;
        skyboxTexture.setRepeated(true);
        skyboxTexture.setSmooth(false);
        skyboxTexture.loadFromFile("resources/skybox.png");
        skyboxSprite.setTexture(skyboxTexture);

        skyTop.create(128,128,true);
        skyBot.create(128,128,true);
        skyRig.create(128,128,true);
        skyLef.create(128,128,true);
        skyFro.create(128,128,true);
        skyBac.create(128,128,true);

       //activeCamera->activeWindow->setActive(false);

            skyboxSprite.setTextureRect(sf::IntRect(128,0,128,128));
            skyTop.setActive(true);
                glEnable (GL_BLEND);
                skyTop.clear(sf::Color::Transparent);
                skyTop.setSmooth(false);
                skyTop.draw(skyboxSprite);
                skyTop.display();
            skyTop.setActive(false);

            skyboxSprite.setTextureRect(sf::IntRect(128,256,128,128));
            skyBot.setActive(true);
                glEnable (GL_BLEND);
                skyBot.clear(sf::Color::Transparent);
                skyBot.setSmooth(false);
                skyBot.draw(skyboxSprite);
                skyBot.display();
            skyBot.setActive(false);

            skyboxSprite.setTextureRect(sf::IntRect(256,128,128,128));
            skyRig.setActive(true);
                glEnable (GL_BLEND);
                skyRig.clear(sf::Color::Transparent);
                skyRig.setSmooth(false);
                skyRig.draw(skyboxSprite);
                skyRig.display();
            skyRig.setActive(false);

            skyboxSprite.setTextureRect(sf::IntRect(0,128,128,128));
            skyLef.setActive(true);
                glEnable (GL_BLEND);
                skyLef.clear(sf::Color::Transparent);
                skyLef.setSmooth(false);
                skyLef.draw(skyboxSprite);
                skyLef.display();
            skyLef.setActive(false);

            skyboxSprite.setTextureRect(sf::IntRect(128,128,128,128));
            skyFro.setActive(true);
                glEnable (GL_BLEND);
                skyFro.clear(sf::Color::Transparent);
                skyFro.setSmooth(false);
                skyFro.draw(skyboxSprite);
                skyFro.display();
            skyFro.setActive(false);

            skyboxSprite.setTextureRect(sf::IntRect(384,128,128,128));
            skyBac.setActive(true);
                glEnable (GL_BLEND);
                skyBac.clear(sf::Color::Transparent);
                skyBac.setSmooth(false);
                skyBac.draw(skyboxSprite);
                skyBac.display();
            skyBac.setActive(false);
        //activeCamera->activeWindow->setActive(true);



        //File Operations
        bool hitCeiling = false;
        bool hitWall1 = false;
        bool hitWall2 = false;
        bool hitWall3 = false;

        bool hitNewSect = false;

        bool readX = false;
        bool readY = false;

        bool commitNode = false;
        bool changedTypes = false;
        bool firstRun = true;

        sf::Vector2f tempNodeXY;

        section tempSection;

        string line;
        ifstream file;
        file.open(levName.c_str());
        if (file.is_open())
        {
            getline(file, line);
            scale = atof(line.c_str());
            getline(file, line);
            bounds.x = atof(line.c_str());
            getline(file, line);
            bounds.y = atof(line.c_str());


            while ( file.good() )
            {
                getline(file, line);

                if(line.compare("ceiling")==0 && !hitCeiling){ hitCeiling = true; continue; }
                if(line.compare("wall1")==0 && hitCeiling){ hitWall1 = true; changedTypes = true; continue; }
                if(line.compare("wall2")==0 && hitWall1){ hitWall2 = true; changedTypes = true; continue; }
                if(line.compare("wall3")==0 && hitWall3){ hitWall3 = true; changedTypes = true; continue; }

                if(atoi(line.c_str())<0)
                {
                    if(firstRun)
                    {
                        firstRun = false;
                        tempSection.type = Level::ceiling;
                    }
                    else
                    {
                        if(hitCeiling && !hitWall1 && !hitWall2 && !hitWall3)
                        {
                            tempSection.type = Level::ceiling;
                            ceilingSections.push_back(tempSection);
                            tempSection.nodeList.clear();
                        }
                        if(hitCeiling && hitWall1 && !hitWall2 && !hitWall3)
                        {
                            if(changedTypes)
                            {
                                tempSection.type = Level::ceiling;
                                ceilingSections.push_back(tempSection);
                                tempSection.nodeList.clear();
                                changedTypes = false;
                            }
                            else
                            {
                                tempSection.type = Level::wallh1;
                                wall1Sections.push_back(tempSection);
                                tempSection.nodeList.clear();
                            }
                        }
                        if(hitCeiling && hitWall1 && hitWall2 && !hitWall3)
                        {
                            if(changedTypes)
                            {
                                changedTypes = false;
                                tempSection.type = Level::wallh1;
                                wall1Sections.push_back(tempSection);
                                tempSection.nodeList.clear();
                            }
                            else
                            {
                                tempSection.type = Level::wallh2;
                                wall2Sections.push_back(tempSection);
                                tempSection.nodeList.clear();
                            }
                        }
                        if(hitCeiling && hitWall1 && hitWall2 && hitWall3)
                        {
                            if(changedTypes)
                            {
                                changedTypes = false;
                                tempSection.type = Level::wallh2;
                                wall2Sections.push_back(tempSection);
                                tempSection.nodeList.clear();
                            }
                            else
                            {
                                tempSection.type = Level::wallh3;
                                wall3Sections.push_back(tempSection);
                                tempSection.nodeList.clear();
                            }
                        }
                    }

                    hitNewSect = true;
                    commitNode = false;
                    readY = false;
                    readX = true;
                }

                if(readX && atoi(line.c_str())>=0)
                {
                    hitNewSect = false;
                    tempNodeXY.x = atof(line.c_str());
                    readX = false;
                    readY = true;
                }
                else if(readY && atoi(line.c_str())>=0)
                {
                    tempNodeXY.y = atof(line.c_str());
                    readX = true;
                    readY = false;
                    commitNode = true;
                }

                if(commitNode)
                {

                    if(hitCeiling && !hitWall1 && !hitWall2 && !hitWall3)
                    {
                        tempSection.nodeList.push_back(tempNodeXY);
                        commitNode = false;
                    }
                    if(hitCeiling && hitWall1 && !hitWall2 && !hitWall3)
                    {
                        tempSection.nodeList.push_back(tempNodeXY);
                        commitNode = false;
                    }
                    if(hitCeiling && hitWall1 && hitWall2 && !hitWall3)
                    {
                        tempSection.nodeList.push_back(tempNodeXY);
                        commitNode = false;
                    }
                    if(hitCeiling && hitWall1 && hitWall2 && hitWall3)
                    {
                        tempSection.nodeList.push_back(tempNodeXY);
                        commitNode = false;
                    }
                }
            }
            file.close();
        }
        wall3Sections.push_back(tempSection);

        //Define Floor
        floorSection.type = Level::floor;
        //Just 4 corners
        floorSection.nodeList.push_back(sf::Vector2f(-bounds.x/2.0, -bounds.y/2.0));
        floorSection.nodeList.push_back(sf::Vector2f(bounds.x/2.0, -bounds.y/2.0));
        floorSection.nodeList.push_back(sf::Vector2f(bounds.x/2.0, bounds.y/2.0));
        floorSection.nodeList.push_back(sf::Vector2f(-bounds.x/2.0, bounds.y/2.0));

#ifdef DEBUG
        cout<<"Loaded map:"<<levelName<<endl;
        //Let's see what we have
        cout<<"Ceiling Sections "<<ceilingSections.size()<<endl;
        for(int i = 0; i < ceilingSections.size(); i++){
            cout<<"\t"<<"Section "<<i<<": "<<ceilingSections[i].nodeList.size()<<"nodes"<<endl;
        }
        cout<<"Wall1 Sections "<<wall1Sections.size()<<endl;
        for(int i = 0; i < wall1Sections.size(); i++){
            cout<<"\t"<<"Section "<<i<<": "<<wall1Sections[i].nodeList.size()<<"nodes"<<endl;
        }
        cout<<"Wall2 Sections "<<wall2Sections.size()<<endl;
        for(int i = 0; i < wall2Sections.size(); i++){
            cout<<"\t"<<"Section "<<i<<": "<<wall2Sections[i].nodeList.size()<<"nodes"<<endl;
        }
        cout<<"Wall3 Sections "<<wall3Sections.size()<<endl;
        for(int i = 0; i < wall3Sections.size(); i++){
            cout<<"\t"<<"Section "<<i<<": "<<wall3Sections[i].nodeList.size()<<"nodes"<<endl;
        }
#endif

    }

    void Level::Update(sf::Time deltaTime)
    {
        //dunno

    }

    void Level::Render(Camera* activeCamera)
    {
        //Render Textures
       activeCamera->activeWindow->setActive(false);
            floorTexture.setActive(true);
                glEnable (GL_BLEND);
                floorTexture.clear(sf::Color::Transparent);
                floorTexture.setSmooth(false);
                floorSprite.setScale(8,8);
                floorTexture.draw(floorSprite);
                floorTexture.display();
            floorTexture.setActive(false);
        activeCamera->activeWindow->setActive(true);

        activeCamera->activeWindow->setActive(false);
            ceilingTexture.setActive(true);
                glEnable (GL_BLEND);
                ceilingTexture.clear(sf::Color::Transparent);
                ceilingTexture.setSmooth(false);
                ceilingSprite.setScale(8,8);
                ceilingTexture.draw(ceilingSprite);
                ceilingTexture.display();
            ceilingTexture.setActive(false);
        activeCamera->activeWindow->setActive(true);

        activeCamera->activeWindow->setActive(false);
            wallTexture.setActive(true);
                glEnable (GL_BLEND);
                wallTexture.clear(sf::Color::Transparent);
                wallTexture.setSmooth(false);
                wallSprite.setScale(8,8);
                wallTexture.draw(wallSprite);
                wallTexture.display();
            wallTexture.setActive(false);
        activeCamera->activeWindow->setActive(true);


        glPushMatrix();
            glPushMatrix();
            glLoadIdentity();
            glColor4f(1.f, 1.f, 1.f, 1.f);
            glTranslatef(0,5,0);
            glScalef(100,100,100);
            glRotatef(activeCamera->angleDeg, 0, 1, 0);


            glEnable(GL_TEXTURE_2D);
        //RenderSkybox
            skyTop.getTexture().bind();
            glBegin(GL_QUADS);
                glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
                glTexCoord2f(1, 0); glVertex3f( -0.5f,  0.5f,  0.5f );
                glTexCoord2f(0, 0); glVertex3f(  0.5f,  0.5f,  0.5f );
                glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
            glEnd();

            skyBot.getTexture().bind();
            glBegin(GL_QUADS);
                glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
                glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f,  0.5f );
                glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f,  0.5f );
                glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
            glEnd();
            skyFro.getTexture().bind();
            glBegin(GL_QUADS);
                glTexCoord2f(0, 1); glVertex3f(  0.5f, -0.5f, -0.5f );
                glTexCoord2f(1, 1); glVertex3f( -0.5f, -0.5f, -0.5f );
                glTexCoord2f(1, 0); glVertex3f( -0.5f,  0.5f, -0.5f );
                glTexCoord2f(0, 0); glVertex3f(  0.5f,  0.5f, -0.5f );
            glEnd();

            skyBac.getTexture().bind();
            glBegin(GL_QUADS);
                glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f,  0.5f );
                glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f,  0.5f );
                glTexCoord2f(1, 0); glVertex3f(  0.5f,  0.5f,  0.5f );
                glTexCoord2f(0, 0); glVertex3f( -0.5f,  0.5f,  0.5f );

            glEnd();

            skyRig.getTexture().bind();
            glBegin(GL_QUADS);
                glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f, -0.5f );
                glTexCoord2f(1, 1); glVertex3f( -0.5f, -0.5f,  0.5f );
                glTexCoord2f(1, 0); glVertex3f( -0.5f,  0.5f,  0.5f );
                glTexCoord2f(0, 0); glVertex3f( -0.5f,  0.5f, -0.5f );
            glEnd();

            skyLef.getTexture().bind();
            glBegin(GL_QUADS);
                glTexCoord2f(0, 1); glVertex3f(  0.5f, -0.5f,  0.5f );
                glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f, -0.5f );
                glTexCoord2f(1, 0); glVertex3f(  0.5f,  0.5f, -0.5f );
                glTexCoord2f(0, 0); glVertex3f(  0.5f,  0.5f,  0.5f );
            glEnd();
            glPopMatrix();




        //Render Floor
            sf::Texture temp = floorTexture.getTexture();
            temp.setRepeated(true);
            temp.bind();
            //floorTexture.getTexture().bind();
            //floor loop
            glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f (scale*floorSection.nodeList[0].x, scale*floorSection.nodeList[0].y); glVertex3f(scale*floorSection.nodeList[0].x, -1, scale*floorSection.nodeList[0].y);
            glTexCoord2f (scale*floorSection.nodeList[1].x, scale*floorSection.nodeList[1].y); glVertex3f(scale*floorSection.nodeList[1].x, -1, scale*floorSection.nodeList[1].y);
            glTexCoord2f (scale*floorSection.nodeList[2].x, scale*floorSection.nodeList[2].y); glVertex3f(scale*floorSection.nodeList[2].x, -1, scale*floorSection.nodeList[2].y);
            glTexCoord2f (scale*floorSection.nodeList[3].x, scale*floorSection.nodeList[3].y); glVertex3f(scale*floorSection.nodeList[3].x, -1, scale*floorSection.nodeList[3].y);

            glEnd();

            //Render Ceiling
            temp = ceilingTexture.getTexture();
            temp.setRepeated(true);
            temp.bind();

            for(int i = 0; i < ceilingSections.size(); i++)
            {
                glBegin(GL_TRIANGLE_FAN);
                for(int j = 0; j < ceilingSections[i].nodeList.size(); j++)
                {
                    glTexCoord2f(scale*(ceilingSections[i].nodeList[j].x - bounds.x/2.0),       scale*(ceilingSections[i].nodeList[j].y - bounds.y/2.0));
                    glVertex3f  (scale*(ceilingSections[i].nodeList[j].x - bounds.x/2.0),   1,  scale*(ceilingSections[i].nodeList[j].y - bounds.y/2.0));
                }
                glEnd();
            }

            //Render Walls
            temp = wallTexture.getTexture();
            temp.setRepeated(true);
            temp.bind();

            for(int i = 0; i < wall1Sections.size(); i++)
            {
                glBegin(GL_TRIANGLE_STRIP);
                for(int j = 0; j < wall1Sections[i].nodeList.size(); j++)
                {
                    glTexCoord2f(scale*(wall1Sections[i].nodeList[j].x - bounds.x/2.0),       0);
                    glVertex3f  (scale*(wall1Sections[i].nodeList[j].x - bounds.x/2.0),   -1,  scale*(wall1Sections[i].nodeList[j].y - bounds.y/2.0));
                    glTexCoord2f(scale*(wall1Sections[i].nodeList[j].x - bounds.x/2.0),       1);
                    glVertex3f  (scale*(wall1Sections[i].nodeList[j].x - bounds.x/2.0),   1,  scale*(wall1Sections[i].nodeList[j].y - bounds.y/2.0));
                }
                glEnd();
            }

            for(int i = 0; i < wall2Sections.size(); i++)
            {
                glBegin(GL_TRIANGLE_STRIP);
                for(int j = 0; j < wall2Sections[i].nodeList.size(); j++)
                {
                    glTexCoord2f(scale*(wall2Sections[i].nodeList[j].x - bounds.x/2.0),       0);
                    glVertex3f  (scale*(wall2Sections[i].nodeList[j].x - bounds.x/2.0),   -1,  scale*(wall2Sections[i].nodeList[j].y - bounds.y/2.0));
                    glTexCoord2f(scale*(wall2Sections[i].nodeList[j].x - bounds.x/2.0),       3);
                    glVertex3f  (scale*(wall2Sections[i].nodeList[j].x - bounds.x/2.0),   5,  scale*(wall2Sections[i].nodeList[j].y - bounds.y/2.0));
                }
                glEnd();
            }
            glDisable(GL_TEXTURE_2D);
            glColor4f(0.f, 0.f, 0.f, 1.f);
            for(int i = 0; i < wall3Sections.size(); i++)
            {
                glBegin(GL_TRIANGLE_STRIP);
                for(int j = 0; j < wall3Sections[i].nodeList.size(); j++)
                {
                    glTexCoord2f(scale*(wall3Sections[i].nodeList[j].x - bounds.x/2.0),       0);
                    glVertex3f  (scale*(wall3Sections[i].nodeList[j].x - bounds.x/2.0),   -1,  scale*(wall3Sections[i].nodeList[j].y - bounds.y/2.0));
                    glTexCoord2f(scale*(wall3Sections[i].nodeList[j].x - bounds.x/2.0),       35);
                    glVertex3f  (scale*(wall3Sections[i].nodeList[j].x - bounds.x/2.0),   69,  scale*(wall3Sections[i].nodeList[j].y - bounds.y/2.0));
                }
                glEnd();
            }
        glPopMatrix();

    }

    Level::~Level()
    {

    }
