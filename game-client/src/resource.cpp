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
    texture->setSmooth( false );
    //texture->generateMipmap();
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

FontResource::FontResource( std::string name ) {
    this->name = name;
    font = new sf::Font();
    font->loadFromFile(name.c_str());
}

FontResource::~FontResource() {
    delete font;
}

void* FontResource::get() {
    return font;
}

sf::Font* ResourceManager::getFont( std::string name ) {
    for( Resource* r : resources ) {
        if ( r->name == name ) {
            return (sf::Font*)((FontResource*)r)->get();
        }
    }
    resources.push_back( new FontResource( name ) );
    return (sf::Font*)((FontResource*)resources.back())->get();
}

SoundResource::SoundResource( std::string name ) {
    this->name = name;
    sound = new sf::SoundBuffer();
    sound->loadFromFile(name.c_str());
}

SoundResource::~SoundResource() {
    delete sound;
}

void* SoundResource::get() {
    return sound;
}

sf::SoundBuffer* ResourceManager::getSound( std::string name ) {
    for( Resource* r : resources ) {
        if ( r->name == name ) {
            return (sf::SoundBuffer*)((SoundResource*)r)->get();
        }
    }
    resources.push_back( new SoundResource( name ) );
    return (sf::SoundBuffer*)((SoundResource*)resources.back())->get();
}
