#include "layer.h"

Layer::Layer(MapLayer* ml){
    mapLayer = ml;
}

Layer::~Layer() {
    delete mapLayer;
}

void Layer::draw(sf::RenderTarget& window){
    window.draw(*mapLayer);
}

Entity::Type Layer::getType(){
    return Entity::Type::Layer;
}
