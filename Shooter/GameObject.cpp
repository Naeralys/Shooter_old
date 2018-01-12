//
//  GameObject.cpp
//  Shooter
//
//  Created by Adam Lind on 5/1/18.
//  Copyright © 2018 Adam Lind. All rights reserved.
//

#include "GameObject.hpp"
#include "TextureManager.hpp"

GameObject::GameObject( const char* fileName, SDL_Renderer* renderer, int xPos, int yPos, int width, int height ) {
    this->renderer = renderer;
    texture = TextureManager::LoadTexture( fileName );
    x = xPos;
    y = yPos;
    w = width;
    h = height;
    src.x = 0;
    src.y = 0;
    src.h = h;
    src.w = w;
    dest.h = src.h;
    dest.w = src.w;
    dest.x = x;
    dest.y = y;
}

void GameObject::Update() {
    x += velX;
    y += velY;
    
    dest.x = x;
    dest.y = y;
}
void GameObject::Render() {
    SDL_RenderCopy( renderer, texture, &src, &dest );
}
