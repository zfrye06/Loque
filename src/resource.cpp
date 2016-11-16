#include "resource.h"

ResourceManager* Resources = new ResourceManager();

Resource::Resource( std::string name ) {
    this->name = name;
}
Resource::Resource() {
}
Resource::~Resource() {
}

void* Resource::get() {
    return nullptr;
}

TextureResource::TextureResource( std::string name ) {
    this->name = name;
    texture = new sf::Texture();
    texture->loadFromFile(name.c_str());
}

TextureResource::~TextureResource() {
    delete texture;
}

void* TextureResource::get() {
    return texture;
}

sf::Texture* ResourceManager::getTexture( std::string name ) {
    for( Resource* r : resources ) {
        if ( r->name == name ) {
            return (sf::Texture*)((TextureResource*)r)->get();
        }
    }
    resources.push_back( new TextureResource( name ) );
    return (sf::Texture*)((TextureResource*)resources.back())->get();
}


MapResource::MapResource( std::string name ) {
    this->name = name;
    map = new tmx::Map();
    try{
        map->load(name);
    } catch(std::exception e ) {
        std::cout << "Failed to load map for reason: " << e.what() << "\n";
    }
}

MapResource::~MapResource() {
    delete map;
}

void* MapResource::get() {
    return map;
}

tmx::Map* ResourceManager::getMap( std::string name ) {
    for( Resource* r : resources ) {
        if ( r->name == name ) {
            return (tmx::Map*)((MapResource*)r)->get();
        }
    }
    resources.push_back( new MapResource( name ) );
    return (tmx::Map*)((MapResource*)resources.back())->get();
}

