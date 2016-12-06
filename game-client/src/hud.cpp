#include "hud.h"

HUD::HUD() {
    scoreText.setFont( *Resources->getFont( "assets/fonts/arial.ttf" ) );
    scoreText.setCharacterSize(40);
}

void HUD::update(double dt){
    int score = playerStats->getScore();
    if ( score >= 0 ) {
        scoreText.setFillColor(sf::Color::White);
    } else {
        scoreText.setFillColor(sf::Color::Red);
    }
    scoreText.setOutlineThickness(2);
    scoreText.setOutlineColor(sf::Color(152,152,152,255));
    scoreText.setStyle(sf::Text::Bold);
    std::ostringstream s;
    s << "Score: " << score;
    scoreText.setString(s.str());
    sf::View view = world->view;
    posX = view.getCenter().x - (view.getSize().x / 2);
    posY = view.getCenter().y - (view.getSize().y / 2);
    scoreText.setPosition(posX + 10, posY + 10);
}

void HUD::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void HUD::draw(sf::RenderTarget& window){
    window.draw(scoreText);
}

Entity::Type HUD::getType(){
    return Entity::Type::HUD;
}
