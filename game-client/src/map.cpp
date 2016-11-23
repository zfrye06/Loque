#include "map.h"

bool Map::isPowerOfTwo( int x ) {
    int y = 2;
    while( y < x && y > 0 ) {
        y*=2;
    }
    if ( y == x ) {
        return true;
    }
    return false;
}

Map::Map( std::string resource ) {
    map = Resources->getMap(resource);

    // Parse map properties
    for ( tmx::Property p : map->getProperties() ) {
        if ( p.getType() == tmx::Property::Type::String && p.getName() == "ambient" ) {
            ambient.openFromFile(p.getStringValue());
            ambient.play();
        }
    }

    b2BodyDef boxDef;
    boxDef.type = b2_staticBody;
    boxDef.position.Set(0, 0);
    boxDef.angle = 0;

    // First grab physical layer
    tmx::TileLayer* tileSet;
    //for( auto l : map->getLayers() ) {
    for ( int i=0;i<map->getLayers().size();i++ ) {
        auto l = map->getLayers()[i].get();
        if ( l->getType() == tmx::Layer::Type::Tile ) {
            if ( l->getName() == "physical" || l->getName() == "Physical" ) {
                tileSet = (tmx::TileLayer*)l;
            } else {
                layers.push_back( new MapLayer( *map, i ) );
            }
        }
    }
    // Then loop through the tiles, placing 1x1 boxes where there's tiles.
    auto tiles = tileSet->getTiles();
    auto mapSize = map->getTileCount();
    if ( !isPowerOfTwo(mapSize.x) || !isPowerOfTwo(mapSize.y) ) {
        std::cerr << "Map size isn't powers of two, may not load properly!\n";
    }
    int x = 0;
    int y = 0;
    for (int i = 0; i < mapSize.x*mapSize.y; i++ ) {
        if ( tiles[i].ID != 0) {
            switch(tiles[i].ID){
                //Right slope
                case 309+2:
                    {
                        b2PolygonShape shape;
                        b2Vec2 vertices[] = {
                            b2Vec2(-.5, .5),
                            b2Vec2(.5, .5),
                            b2Vec2(.5, -.5)
                        };
                        shape.Set(vertices, 3);
                        b2FixtureDef shapeFixDef;
                        shapeFixDef.shape = &shape;
                        shapeFixDef.density = 500;
                        shapeFixDef.restitution = 0;
                        boxDef.position.Set(x+0.5, y+0.5);
                        b2Body* slopeBody = physicalWorld->get().CreateBody( &boxDef );
                        slopeBody->CreateFixture(&shapeFixDef);
                        slopeBody->SetUserData(this);
                        break;
                    }
                //Left slope
                case 309+5:
                    {
                        b2PolygonShape slopeRight;
                        b2Vec2 vertices[] = {
                            b2Vec2(.5, .5),
                            b2Vec2(-.5, -.5),
                            b2Vec2(-.5, .5)
                        };
                        slopeRight.Set(vertices, 3);
                        b2FixtureDef slopeRightFixureDef;
                        slopeRightFixureDef.shape = &slopeRight;
                        slopeRightFixureDef.density = 500;
                        slopeRightFixureDef.restitution = 0;
                        boxDef.position.Set(x+0.5, y+0.5);
                        b2Body* slopeBody = physicalWorld->get().CreateBody( &boxDef );
                        slopeBody->CreateFixture(&slopeRightFixureDef);
                        slopeBody->SetUserData(this);
                        break;
                    }
                //HalfBox
                case 309+3:
                    {
                        b2PolygonShape shape;
                        b2Vec2 vertices[] = {
                            b2Vec2(-.5, -.5),
                            b2Vec2(-.5, 0),
                            b2Vec2(.5, 0),
                            b2Vec2(.5, -.5)
                        };
                        shape.Set(vertices, 4);
                        b2FixtureDef shapeFixDef;
                        shapeFixDef.shape = &shape;
                        shapeFixDef.density = 500;
                        shapeFixDef.restitution = 0;
                        boxDef.position.Set(x+0.5, y+0.5);
                        b2Body* slopeBody = physicalWorld->get().CreateBody( &boxDef );
                        slopeBody->CreateFixture(&shapeFixDef);
                        slopeBody->SetUserData(this);

                        break;
                    }
                // Circle cap
                case 309+4:
                    {
                        b2PolygonShape slopeRight;
                        b2Vec2 vertices[] = {
                            b2Vec2(-.5, 0),
                            b2Vec2(.5, 0),
                            b2Vec2(.5, .5),
                            b2Vec2(-.5, .5)
                        };
                        slopeRight.Set(vertices, 4);
                        b2FixtureDef slopeRightFixureDef;
                        slopeRightFixureDef.shape = &slopeRight;
                        slopeRightFixureDef.density = 500;
                        slopeRightFixureDef.restitution = 0;
                        boxDef.position.Set(x+0.5, y+0.5);
                        b2Body* slopeBody = physicalWorld->get().CreateBody( &boxDef );
                        slopeBody->CreateFixture(&slopeRightFixureDef);
                        b2CircleShape circleShape;
                        circleShape.m_radius = .5;
                        slopeRightFixureDef.shape = &circleShape;
                        slopeBody->CreateFixture(&slopeRightFixureDef);
                        slopeBody->SetUserData(this);
                        break;
                    }
                // Ledge left
                case 315:
                    {
                        b2PolygonShape shape;
                        b2Vec2 vertices[] = {
                            b2Vec2(-.5, -.5),
                            b2Vec2(-.5, 0),
                            b2Vec2(-.25, 0),
                            b2Vec2(.25, .5),
                            b2Vec2(.5, .5),
                            b2Vec2(.5, -.5)
                        };
                        shape.Set(vertices, 6);
                        b2FixtureDef shapeFixDef;
                        shapeFixDef.shape = &shape;
                        shapeFixDef.density = 500;
                        shapeFixDef.restitution = 0;
                        boxDef.position.Set(x+0.5, y+0.5);
                        b2Body* slopeBody = physicalWorld->get().CreateBody( &boxDef );
                        slopeBody->CreateFixture(&shapeFixDef);
                        slopeBody->SetUserData(this);
                        break;
                    }
                // Ledge right
                case 316:
                    {
                        b2PolygonShape shape;
                        b2Vec2 vertices[] = {
                            b2Vec2(-.5, -.5),
                            b2Vec2(-.5, .5),
                            b2Vec2(-.25, .5),
                            b2Vec2(.25, 0),
                            b2Vec2(.5, 0),
                            b2Vec2(.5, -.5)
                        };
                        shape.Set(vertices, 6);
                        b2FixtureDef shapeFixDef;
                        shapeFixDef.shape = &shape;
                        shapeFixDef.density = 500;
                        shapeFixDef.restitution = 0;
                        boxDef.position.Set(x+0.5, y+0.5);
                        b2Body* slopeBody = physicalWorld->get().CreateBody( &boxDef );
                        slopeBody->CreateFixture(&shapeFixDef);
                        slopeBody->SetUserData(this);
                        break;
                    }
                // Half lower
                case 317:
                    {
                        b2PolygonShape shape;
                        b2Vec2 vertices[] = {
                            b2Vec2(-.5, 0),
                            b2Vec2(-.5, .5),
                            b2Vec2(.5, .5),
                            b2Vec2(.5, 0)
                        };
                        shape.Set(vertices, 4);
                        b2FixtureDef shapeFixDef;
                        shapeFixDef.shape = &shape;
                        shapeFixDef.density = 500;
                        shapeFixDef.restitution = 0;
                        boxDef.position.Set(x+0.5, y+0.5);
                        b2Body* slopeBody = physicalWorld->get().CreateBody( &boxDef );
                        slopeBody->CreateFixture(&shapeFixDef);
                        slopeBody->SetUserData(this);
                        break;
                    }
                default:
                    {
                        b2PolygonShape boxShape;
                        boxShape.SetAsBox(.5,.5);
                        b2FixtureDef boxFixtureDef;
                        boxFixtureDef.shape = &boxShape;
                        boxFixtureDef.density = 500;
                        boxFixtureDef.restitution = 0;
                        boxDef.position.Set(x+0.5, y+0.5);
                        b2Body* b = physicalWorld->get().CreateBody( &boxDef );
                        b->CreateFixture(&boxFixtureDef);
                        b->SetUserData(this);
                    }
            }
        }
        x++;
        if ( x >= mapSize.x ) {
            y++;
            x=0;
        }
    }
}
Map::~Map() {
    ambient.stop();
    for ( auto i : layers ) {
        delete i;
    }
}
void Map::draw( sf::RenderWindow& window ) {
    for ( auto i : layers ) {
        window.draw(*i);
    }
}
void Map::update( double dt ) {
}
void Map::onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal ) {
}

Entity::Type Map::getType(){
    return Entity::Type::Map;
}
