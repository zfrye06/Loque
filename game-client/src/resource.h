#ifndef LQ_RESOURCE_H_
#define LQ_RESOURCE_H_

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <string>
#include <tmxlite/Map.hpp>
#include <vector>

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

class FontResource : public Resource {
private:
    sf::Font* font;
public:
    FontResource( std::string name );
    virtual ~FontResource();
    void load();
    void* get();
};

class SoundResource : public Resource {
private:
    sf::SoundBuffer* sound;
public:
    SoundResource( std::string name );
    virtual ~SoundResource();
    void load();
    void* get();
};

class ResourceManager {
private:
    std::vector<Resource*> resources;
public:
    ~ResourceManager();
    sf::SoundBuffer* getSound( std::string name );
    sf::Texture* getTexture( std::string name );
    sf::Font* getFont( std::string name );
    tmx::Map* getMap( std::string name );
};

extern ResourceManager* Resources;

#endif
