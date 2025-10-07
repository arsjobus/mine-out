#include "Explosion.h"
#include <SFML/Audio.hpp>


Explosion::Explosion(void)
{
    this->loadDefaultSettings();
}

Explosion::Explosion(float radius, float positionX, float positionY)
{
    this->loadDefaultSettings();
    this->setRadius(radius);
    this->setSize(sf::Vector2f(this->getRadius() * 2, this->getRadius() * 2));
    this->setRadius(radius);
    this->setOrigin(sf::Vector2f(this->getGlobalBounds().size.x / 2, this->getGlobalBounds().size.y / 2));
    this->setPosition(sf::Vector2f(positionX, positionY));
}

Explosion::~Explosion(void)
{
}

bool Explosion::detectCollisionWithBlocks(Block &block)
{
    return this->getGlobalBounds().findIntersection(block.getGlobalBounds()).has_value();
}

bool Explosion::nextFrame()
{
    dt += deltaClock.restart();

    sf::Vector2i frameSize(256, 256);

    if (dt.asMilliseconds() >= sf::milliseconds(20).asMilliseconds())
    {
        if (++currentFrame.x < totalFrames.x)
            this->setTextureRect(sf::Rect<int>(
                sf::Vector2i(currentFrame.x * frameSize.x, currentFrame.y * frameSize.y),
                sf::Vector2i(256, 256)));
        else
        {
            currentFrame.x = 0;

            if (++currentFrame.y <= totalFrames.y)
                this->setTextureRect(sf::Rect<int>(
					sf::Vector2i(this->currentFrame.x * frameSize.x, this->currentFrame.y * frameSize.y),
					sf::Vector2i(256, 256)));
            else
            {
                this->currentFrame.x = 0;

                this->currentFrame.y = 0;

                this->setActive(false);
            }
        }

        dt = sf::seconds(0);

        return true;
    }

    return false;
}

void Explosion::loadDefaultSettings()
{
    this->deltaClock.restart();
    this->dt = sf::seconds(0);
    sf::Sound sndExplosion = sf::Sound(resource.getBufferedSound(1));
	this->getRefToSound()->push_back(sndExplosion);
    this->setActive(true);
    this->setDamage(5);
    this->setTexture(&resource.getAnimationTexture(0));
    this->currentFrame.x = 0;
    this->currentFrame.y = 0;
    this->totalFrames.x  = 8;
    this->totalFrames.y  = 6;
    this->setTextureRect(
        sf::Rect<int>(
            sf::Vector2i(this->currentFrame.x, this->currentFrame.y),
            sf::Vector2i(256, 256)
        )
    );
    this->setOrigin(sf::Vector2f(this->getGlobalBounds().size.x / 2, this->getGlobalBounds().size.y / 2));
}

float Explosion::getRadius()
{
    return this->radius;
}

void Explosion::setRadius(float radius)
{
    this->radius = radius;
}

bool Explosion::getActive()
{
    return this->active;
}

void Explosion::setActive(bool active)
{
    this->active = active;
}
