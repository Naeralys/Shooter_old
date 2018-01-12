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
    
public:
    float x, y, velX, velY, cooldown, hp;
    Character( const char* fileName, SDL_Renderer *renderer, int x, int y );
    void Update();
    void Render();

};


#endif /* Character_hpp */
