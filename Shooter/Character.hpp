//
//  Character.hpp
//  Shooter
//
//  Created by Adam Lind on 5/1/18.
//  Copyright © 2018 Adam Lind. All rights reserved.
//

#ifndef Character_hpp
#define Character_hpp

#include "Game.hpp"

class Character {
private:
    SDL_Texture *texture;
    SDL_Rect src, dest;
    SDL_Renderer *renderer;
    float knockback;
    bool keylock, vulnerable, block;
    int locktimer, locktime, vulnerableTimer, blockTimer;
    int animationTimer, animationChoice;
public:
    float x, y, velX, velY, cooldown;
    Character( const char* fileName, SDL_Renderer *renderer, int x, int y );
    void Update();
    void Render();
    void Animation( int animationChoice, int animationLenght );
    float Knockback();
    void KeyLock( int locktime );
    bool CheckKeyLock();
    bool Vulnerable();
    bool Block();
    void Block( bool blockTrigger );
    void Invulnerable();
};


#endif /* Character_hpp */
