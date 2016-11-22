#include "entity.h"
#include <vector>
#include <iostream>

Entity::Entity() {
}
Entity::~Entity() {
}
void Entity::update( double dt ) {
}
void Entity::onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal  ) {
}
void Entity::draw( sf::RenderWindow& window ) {
}
Entity::Type Entity::getType(){
    return Entity::Type::None;
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
    int x = 0;
    int y = 0;
    for (int i = 0; i < mapSize.x*mapSize.y; i++ ) {
        if ( tiles[i].ID != 0) {
            //std::cout<<tiles[i].ID<<std::endl;
            switch(tiles[i].ID){
                // Slope left
                case 311:
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
                // Half upper
                case 312:
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
                // Rounded box
                case 313:
                    {
                        b2PolygonShape shape;
                        b2Vec2 vertices[] = {
                            b2Vec2(-.5, .5),
                            b2Vec2(.5, .5),
                            b2Vec2(.5, 0),
                            b2Vec2(.3, -.3),
                            b2Vec2(0, -.5),
                            b2Vec2(-.3, -.3),
                            b2Vec2(-.5, 0)
                        };
                        shape.Set(vertices, 7);
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
                // Slope right
                case 314:
                    {
                        b2PolygonShape shape;
                        b2Vec2 vertices[] = {
                            b2Vec2(-.5, -.5),
                            b2Vec2(-.5, .5),
                            b2Vec2(.5, .5)
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
PhysicsDebug::PhysicsDebug(sf::RenderWindow& window) {
    uint32 flags = b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    //flags += b2Draw::e_aabbBit;
    //flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;

    drawer = new DebugDraw(window);
    drawer->SetFlags(flags);
    physicalWorld->get().SetDebugDraw(drawer);
}
PhysicsDebug::~PhysicsDebug(){}
void PhysicsDebug::update( double dt ){}
void PhysicsDebug::onHit( Entity* collider, b2Contact* c, b2Vec2 hitnormal  ){}
void PhysicsDebug::draw( sf::RenderWindow& window ) {
    window.pushGLStates();
    physicalWorld->get().DrawDebugData();
    window.popGLStates();
}

Entity::Type PhysicsDebug::getType(){
    return Entity::Type::None;
}

Background::Background(sf::View* view, std::string skyFile, std::string cloudsFile, std::string hillsFile){
    defaultView = view;
    sky = sf::Sprite(*Resources->getTexture(skyFile));
    clouds = sf::Sprite(*Resources->getTexture(cloudsFile));
    hills = sf::Sprite(*Resources->getTexture(hillsFile));
    Resources->getTexture(skyFile)->setRepeated(true);
    Resources->getTexture(cloudsFile)->setRepeated(true);
    Resources->getTexture(hillsFile)->setRepeated(true);
}

void Background::update( double dt){
    glm::vec2 viewPos = toGLM(defaultView->getCenter());
    sf::IntRect hillUV = hills.getTextureRect();
    hillUV.left = viewPos.x/4;
//    hillUV.top += viewPos.y/256;
    hills.setTextureRect(hillUV);
    sf::IntRect cloudsUV = clouds.getTextureRect();
    cloudsUV.left = viewPos.x/8;
    clouds.setTextureRect(cloudsUV);
}

void Background::onHit(Entity* collider, b2Contact* c, b2Vec2 hitnormal){
}

void Background::draw(sf::RenderWindow& window){
    
    sf::View view(sf::FloatRect(0,0,800,600));
    window.setView(view);
    window.draw(sky);
    window.draw(clouds);
    window.draw(hills);
    window.setView(*defaultView);
}

Entity::Type Background::getType(){
    return Entity::Type::None;
}
