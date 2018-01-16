//
//  Character.cpp
//  Shooter
//
//  Created by Adam Lind on 5/1/18.
//  Copyright © 2018 Adam Lind. All rights reserved.
//

#include "Character.hpp"
#include "TextureManager.hpp"

Character::Character( const char* fileName, SDL_Renderer* renderer, int x, int y ) {
    this->renderer = renderer;
    texture = TextureManager::LoadTexture( fileName );
    this->x = 0;
    this->y = 600;
    this->velX = 0;
    this->velY = 0;
    src.x = 0;
    src.y = 0;
    src.h = 32;
    src.w = 32;
    dest.h = src.h;
    dest.w = src.w;
    hp = 20;
}

void Character::Update() {
    x += velX;
    y += velY;
    
    dest.x = x;
    dest.y = y;
    
    if( velX > 0 )
        velX -= 0.5;
    if( velX < 0 )
        velX += 0.5;
}

void Character::Render() {
    SDL_RenderCopy( Game::renderer, texture, &src, &dest );
}
