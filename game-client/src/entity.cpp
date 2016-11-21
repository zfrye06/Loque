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

    background = new MapLayer( *map, 0 );
    ground = new MapLayer( *map, 1 );

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
            if ( l->getName() == "physical" ) {
                tileSet = (tmx::TileLayer*)l;
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
                case 1:
                    break;
                //Right slope
                case 8:
                    {
                        b2PolygonShape slopeRight;
                        b2Vec2 vertices[] = {
                            b2Vec2(-.5, .5),
                            b2Vec2(.5, -.5),
                            b2Vec2(.5, .5)
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
                case 9:
                case 10:
                case 11:
                case 12:
                case 23:
                case 24:
                case 25:
                case 26:
                case 37:
                case 38:
                case 39:
                case 40:
                case 51:
                case 52:
                case 53:
                case 54:
                    {
                        b2PolygonShape slopeRight;
                        b2Vec2 vertices[] = {
                            b2Vec2(-.5, -.5),
                            b2Vec2(-.5, 0),
                            b2Vec2(.5, 0),
                            b2Vec2(.5, -.5)
                        };
                        slopeRight.Set(vertices, 4);
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
    delete background;
    delete ground;
}
void Map::draw( sf::RenderWindow& window ) {
    window.draw(*background);
    window.draw(*ground);
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
