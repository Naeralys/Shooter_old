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
    this->x = SCREEN_WIDTH / 2;
    this->y = SCREEN_HEIGHT - 40;
    this->velX = 0;
    this->velY = 0;
    src.x = 0;
    src.y = 0;
    src.h = 32;
    src.w = 32;
    dest.h = src.h;
    dest.w = src.w;
    keylock = false;
}

void Character::Update() {
    x += velX;
    y += velY;
    
    dest.x = x;
    dest.y = y;
        
    if( velX > 0.5 )
        velX -= 0.5;
    if( velX < -0.5 )
        velX += 0.5;
    if( velX > -0.6 && velX < 0.6)
        velX = 0;
    if( keylock )
        locktimer++;
    if( locktimer > locktime ) {
        keylock = false;
        locktimer = 0;
    }
}

void Character::Render() {
    SDL_RenderCopy( Game::renderer, texture, &src, &dest );
}
void Character::KeyLock( int locktime ) {
    keylock = true;
    this->locktime = locktime;
}
bool Character::CheckKeyLock() {
    return keylock;
}
