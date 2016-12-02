#include "layer.h"

Layer::Layer(MapLayer* ml){
    mapLayer = ml;
}

void Layer::update(double dt){
}

void Layer::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void Layer::draw(sf::RenderTarget& window){
    window.draw(*mapLayer);
}

Entity::Type Layer::getType(){
    return Entity::Type::Layer;
}
