#pragma once

#include "State.h"
#include <SFML/OpenGL.hpp>
#include <cmath>
#include <iostream>

class GameState : public State
{
public:
    GameState()
    {
        m_bgTex.loadFromFile("resources/background.jpg");
        m_bgSpr = new sf::Sprite(m_bgTex);

        angle=0.0;
        lx = 0.0f;
        lz = -1.0f;
        sx = 1.0f;
        sz = 0.0f;
        x = 0.0f;
        z = 5.0f;
        fraction = 0.1f;

        deltaAngle = 0.0f;
        deltaMove = 0;
        deltaStrafe = 0;

        // Load an OpenGL texture.
        // We could directly use a sf::Texture as an OpenGL texture (with its Bind() member function),
        // but here we want more control on it (generate mipmaps, ...) so we create a new one from an image
        boxtexture = 0;
        sf::Image image;
        image.loadFromFile("resources/texture.jpg");
        glGenTextures(1, &boxtexture);
        glBindTexture(GL_TEXTURE_2D, boxtexture);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        billboardtexture = 0;
        image.loadFromFile("resources/billboard.jpg");
        glGenTextures(1, &billboardtexture);
        glBindTexture(GL_TEXTURE_2D, billboardtexture);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    ~GameState()
    {
        // Don't forget to destroy our texture
        glDeleteTextures(1, &boxtexture);
        glDeleteTextures(1, &billboardtexture);

        delete m_bgSpr;
    }

    void drawSnowMan() {

    // Bind our texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, boxtexture);
    glColor4f(1.f, 1.f, 1.f, 1.f);

	// Draw a cube
	float size = 2.f;

	glTranslatef(size, size, size);

	glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
            glTexCoord2f(0, 1); glVertex3f(-size,  size, -size);
            glTexCoord2f(1, 1); glVertex3f( size,  size, -size);
            glTexCoord2f(1, 0); glVertex3f( size, -size, -size);

            glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
            glTexCoord2f(0, 1); glVertex3f(-size,  size, size);
            glTexCoord2f(1, 1); glVertex3f( size,  size, size);
            glTexCoord2f(1, 0); glVertex3f( size, -size, size);

            glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
            glTexCoord2f(0, 1); glVertex3f(-size,  size, -size);
            glTexCoord2f(1, 1); glVertex3f(-size,  size,  size);
            glTexCoord2f(1, 0); glVertex3f(-size, -size,  size);

            glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
            glTexCoord2f(0, 1); glVertex3f(size,  size, -size);
            glTexCoord2f(1, 1); glVertex3f(size,  size,  size);
            glTexCoord2f(1, 0); glVertex3f(size, -size,  size);

            glTexCoord2f(0, 1); glVertex3f(-size, -size,  size);
            glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
            glTexCoord2f(1, 0); glVertex3f( size, -size, -size);
            glTexCoord2f(1, 1); glVertex3f( size, -size,  size);

            glTexCoord2f(0, 1); glVertex3f(-size, size,  size);
            glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
            glTexCoord2f(1, 0); glVertex3f( size, size, -size);
            glTexCoord2f(1, 1); glVertex3f( size, size,  size);
	glEnd();
    }


    float getMagnitude(float x, float y, float z)
    {
        return sqrt(x*x+y*y+z*z);
    }

    sf::Vector3f normalize(sf::Vector3f vec)
    {
        float length = getMagnitude(vec.x,vec.y,vec.z);
        sf::Vector3f temp;
        temp.x = vec.x/length;
        temp.y = vec.y/length;
        temp.z = vec.z/length;
        return temp;
    }

    sf::Vector3f cross(sf::Vector3f v1, sf::Vector3f v2)
    {
        sf::Vector3f temp;
        temp.x = v1.y*v2.z - v1.z*v2.y;
        temp.y = v1.z*v2.x - v1.x*v2.z;
        temp.z = v1.x*v2.y - v1.y*v2.x;
        return temp;
    }

    float dot(sf::Vector3f v1, sf::Vector3f v2)
    {
        return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
    }

    void drawBillboard(float objX, float objZ) {
        // Draw a cube
        float size = 2.f;

        sf::Vector3f objToCamProj(x - objX, 0.f, z-objZ);
        sf::Vector3f lookAt(0,0,1);

        sf::Vector3f objNormal = normalize(objToCamProj);
        sf::Vector3f upAux = cross(lookAt, objNormal);

        float angleCosine = dot(lookAt, objNormal);

        if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
          glRotatef(acos(angleCosine)*180/3.14159265f, upAux.x, upAux.y, upAux.z);

        // Bind our texture
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, billboardtexture);
        glColor4f(1.f, 1.f, 1.f, 1.f);



        glTranslatef(size, size, size);

        glBegin(GL_QUADS);
                glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
                glTexCoord2f(0, 1); glVertex3f(-size,  size, -size);
                glTexCoord2f(1, 1); glVertex3f( size,  size, -size);
                glTexCoord2f(1, 0); glVertex3f( size, -size, -size);
        glEnd();
    }

    void computePos() {

	    x += deltaMove * lx * 0.1f;
	    z += deltaMove * lz * 0.1f;
    }

    void computeDir() {

	    angle += deltaAngle;
	    lx = sin(angle);
        lz = -cos(angle);
    }

    // All corresponding states can use these
    void Input(uint press, uint held, uint mpress, uint mheld, sf::Vector2i mpos)
    {
        if ( press & KEY_L || held & KEY_L)
        {
            deltaStrafe = 0.5f;
        }
		else if ( press & KEY_R || held & KEY_R)
        {
			deltaStrafe = -0.5f;
        }
        else
        {
            deltaStrafe = 0.0f;
        }
		if ( press & KEY_U || held & KEY_U)
        {
			 deltaMove = 0.5f;
        }
		else if ( press & KEY_D || held & KEY_D)
        {
			 deltaMove = -0.5f;
        }
        else
        {
            deltaMove = 0.0f;
        }

        m_mouseVel.x = mpos.x - m_mousePos.x;
        m_mouseVel.y = mpos.y - m_mousePos.y;

        m_mousePos = mpos;

        // Need to recenter the mouse
    }

    uint Update(sf::Time)
    {
		// update deltaAngle
		deltaAngle = m_mouseVel.x * 0.005f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);

		sx = sin(angle + deltaStrafe);
		sz = -cos(angle + deltaStrafe);

        if (deltaMove || deltaStrafe)
            computePos();
        if (deltaAngle)
            computeDir();

        return STATE_IDLE;
    } // do some updating

    void Render(sf::RenderWindow * window)
    {
        // Draw a quick background
        window->pushGLStates();
        window->draw(*m_bgSpr);
        window->popGLStates();

        // Reset transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //glTranslatef(x, y, -100.f);

        // Set the camera
        gluLookAt(	x, 1.0f, z,
                x+lx, 1.0f,  z+lz,
                0.0f, 1.0f,  0.0f);

        // Draw ground
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_QUADS);
            glVertex3f(-100.0f, 0.0f, -100.0f);
            glVertex3f(-100.0f, 0.0f,  100.0f);
            glVertex3f( 100.0f, 0.0f,  100.0f);
            glVertex3f( 100.0f, 0.0f, -100.0f);
        glEnd();

        // Draw 36 SnowMen
        for(int i = -3; i < 3; i++)
            for(int j=-3; j < 3; j++) {
                glPushMatrix();
                glTranslatef(i*10.0,0,j * 10.0);
                drawSnowMan();
                glPopMatrix();
            }

        // Draw 36 billboards
        for(int i = -3; i < 3; i++)
            for(int j=-3; j < 3; j++) {
                glPushMatrix();
                glTranslatef(i*10.0 + 5.0, 0, j * 10.0 + 5.0);
                drawBillboard(i*10.0 + 5.0,j * 10.0 + 5.0);
                glPopMatrix();
            }

        // Draw some text on top of our OpenGL object
        window->pushGLStates();
        sf::Text text("in game state");
        text.setColor(sf::Color(255, 255, 255, 200));
        text.setPosition(250.f, 450.f);
        window->draw(text);
        window->popGLStates();
    } // render things onto the screen

    void Reset(sf::Vector2i mpos)
    {
        m_mousePos = mpos;
    } // this should happen at the beginning of a state change

private:
    sf::Texture billboard;
    sf::Sprite testSprite;

    sf::Texture m_bgTex;
    sf::Sprite * m_bgSpr;

    // angle of rotation for the camera direction
    float angle;
    // actual vector representing the camera's direction
    float lx,lz,sx,sz;
    // XZ position of the camera
    float x,z;

    float deltaAngle;
    float deltaMove;
    float deltaStrafe;

    float fraction;

    sf::Vector2i m_mousePos;
    sf::Vector2i m_mouseVel;

    GLuint boxtexture;
    GLuint billboardtexture;
};
