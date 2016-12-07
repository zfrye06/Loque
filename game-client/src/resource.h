/**
 * @file resource.h
 * @brief Handles loading shared resources into memory. No worries about multi-loading images or whatever.
 *        Definitely could have smartened up the polymorphism here. Works fine regardless.
 * @author Dalton Nell
 * @version 0.0.0
 * @date 2016-12-06
 */
#ifndef LQ_RESOURCE_H_
#define LQ_RESOURCE_H_

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <iostream>
#include <string>
#include <tmxlite/Map.hpp>
#include <vector>

class Resource {
public:
    std::string name;
    Resource();
    Resource( std::string& name ) : name(name){};
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

class ShaderResource : public Resource {
private:
    sf::Shader* shader;
public:
    ShaderResource( std::string basename );
    virtual ~ShaderResource();
    void load();
    void* get();
};

class ResourceManager {
private:
    std::vector<Resource*> resources;
public:
    ~ResourceManager();
    sf::Shader* getShader( std::string name );
    sf::SoundBuffer* getSound( std::string name );
    sf::Texture* getTexture( std::string name );
    sf::Font* getFont( std::string name );
    tmx::Map* getMap( std::string name );
};

extern ResourceManager* Resources;

#endif
