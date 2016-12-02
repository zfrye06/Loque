#include "hud.h"

void HUD::update(double dt){
}

void HUD::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void HUD::draw(sf::RenderTarget& window){
    sf::View view = world->view;
    posX = view.getCenter().x - (view.getSize().x / 2);
    posY = view.getCenter().y - (view.getSize().y / 2);
    std::vector<Entity*> players = world->getEntitiesByType(Entity::Type::Player);
    if(players.size() > 0){
        int score = playerStats->getScore();
        sf::Text scoreText;
        scoreText.setFont( *Resources->getFont( "assets/fonts/arial.ttf" ) );
        std::ostringstream s;
        s << score;
        scoreText.setString(s.str());
        scoreText.setCharacterSize(40);
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setOutlineColor(sf::Color::Black);
        scoreText.setStyle(sf::Text::Bold);
        scoreText.setPosition(posX + 10, posY + 10);
        window.draw(scoreText);
    }
}

Entity::Type HUD::getType(){
    return Entity::Type::HUD;
}
