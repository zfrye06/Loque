#ifndef LQ_RESOURCE_H_
#define LQ_RESOURCE_H_

#include <iostream>

#include <tmxlite/Map.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include <string>

class Resource {
public:
    std::string name;
    Resource();
    Resource( std::string name );
    virtual ~Resource();
    virtual void* get();
};

class TextureResource : public Resource {
private:
    sf::Texture* texture;
public:
    TextureResource( std::string name );
    virtual ~TextureResource();
    void load();
    void* get();
};

class MapResource : public Resource {
private:
    tmx::Map* map;
public:
    MapResource( std::string name );
    virtual ~MapResource();
    void load();
    void* get();
};

class ResourceManager {
private:
    std::vector<Resource*> resources;
public:
    sf::Texture* getTexture( std::string name );
    tmx::Map* getMap( std::string name );
};

extern ResourceManager* Resources;

#endif
