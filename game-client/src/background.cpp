#include "background.h"

Background::Background(std::string skyFile, std::string cloudsFile, std::string hillsFile){
    sky = sf::Sprite(*Resources->getTexture(skyFile));
    clouds = sf::Sprite(*Resources->getTexture(cloudsFile));
    hills = sf::Sprite(*Resources->getTexture(hillsFile));
    Resources->getTexture(skyFile)->setRepeated(true);
    Resources->getTexture(cloudsFile)->setRepeated(true);
    Resources->getTexture(hillsFile)->setRepeated(true);
}

void Background::update( double dt ){
    glm::vec2 viewPos = toGLM(world->view.getCenter());
    glm::vec2 viewSize = toGLM(world->windowView.getSize());
    sf::IntRect hillUV = hills.getTextureRect();
    hillUV.left = viewPos.x/4;
//    hillUV.top += viewPos.y/280;
    hills.setTextureRect(hillUV);
    sf::IntRect cloudsUV = clouds.getTextureRect();
    cloudsUV.left = viewPos.x/8;
    clouds.setTextureRect(cloudsUV);
    sky.setScale( viewSize.x/sky.getTexture()->getSize().x, viewSize.y/sky.getTexture()->getSize().y );
    hills.setScale( viewSize.x/hills.getTexture()->getSize().x, viewSize.y/hills.getTexture()->getSize().y );
    clouds.setScale( viewSize.x/clouds.getTexture()->getSize().x, viewSize.y/clouds.getTexture()->getSize().y );
}

void Background::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void Background::draw(sf::RenderTarget& window){
    window.setView( world->windowView );
    window.draw(sky);
    window.draw(clouds);
    window.draw(hills);
    window.setView( world->view );
}

Entity::Type Background::getType(){
    return Entity::Type::None;
}
