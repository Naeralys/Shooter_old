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
    this->x = x;
    this->y = y;
    this->velX = 0;
    this->velY = 0;
    src.x = 0;
    src.y = 0;
    src.h = 32;
    src.w = 32;
    dest.h = src.h;
    dest.w = src.w;
    keylock = false;
    animationTimer = 0;
    animationChoice = 0;
    knockback = 1.0;
    vulnerable = true;
}

void Character::Update() {
    // Idle animation
    if( animationChoice == 1 )
        Animation( 0, 0 );
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
    if( !Vulnerable() ) {
        vulnerableTimer++;
        if( vulnerableTimer > 20 )
            vulnerable = true;
    }
    if( Block() ) {
        blockTimer++;
        if( blockTimer > 10)
            block = false;
    }
}
void Character::Animation( int animationChoice, int animationLenght ) {
    // Move the index
    switch( animationTimer ) {
        case 0: src.x = 0;
            break;
        case 5: src.x = 32;
            break;
        case 10: src.x = 64;
            break;
        case 15: src.x = 96;
            break;
        case 20: src.x = 128;
            break;
        case 25: src.x = 160;
            break;
    }
    // End the animation, reset back to idle
    if( animationTimer > animationLenght ) {
        animationTimer = 0;
        if( animationChoice != 1 )
            animationChoice = 1;
    }
    // Increment the timer
    animationTimer++;
}
void Character::Render() {
    SDL_RenderCopy( Game::renderer, texture, &src, &dest );
}
float Character::Knockback() {
    knockback = knockback * 1.1;
    return knockback;
}
void Character::KeyLock( int locktime ) {
    keylock = true;
    this->locktime = locktime;
}
bool Character::CheckKeyLock() {
    return keylock;
}
bool Character::Vulnerable() {
    return vulnerable;
}
void Character::Invulnerable() {
    vulnerable = false;
    vulnerableTimer = 0;
}
bool Character::Block() {
    return block;
}
void Character::Block( bool blockTrigger ) {
    if( blockTrigger ) {
        block = true;
        blockTimer = 0;
    }
}
