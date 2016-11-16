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

