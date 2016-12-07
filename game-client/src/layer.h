/**
 * @file layer.h
 * @brief Simply renders a layer from the given map.
 * @author Zach Frye
 * @version 0.0.0
 * @date 2016-12-06
 */

#ifndef LQ_LAYER_H_
#define LQ_LAYER_H_

#include "entity.h"
#include "player.h"
#include "SFMLOrthogonalLayer.hpp"

/**
 * @brief Simply renders a layer from the given map.
 */
class Layer : public Entity {
private:
    MapLayer* mapLayer;
public:
    Layer(MapLayer* ml);
    ~Layer();
    void draw(sf::RenderTarget& window);
    Entity::Type getType();
};

#endif
