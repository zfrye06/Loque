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
    sf::IntRect hillUV = hills.getTextureRect();
    hillUV.left = viewPos.x/4;
//    hillUV.top += viewPos.y/280;
    hills.setTextureRect(hillUV);
    sf::IntRect cloudsUV = clouds.getTextureRect();
    cloudsUV.left = viewPos.x/8;
    clouds.setTextureRect(cloudsUV);
}

void Background::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void Background::draw(sf::RenderTarget& window){
    sf::View temp = window.getView();
    window.setView( window.getDefaultView() );
    window.draw(sky);
    window.draw(clouds);
    window.draw(hills);
    window.setView( temp );
}

Entity::Type Background::getType(){
    return Entity::Type::None;
}
