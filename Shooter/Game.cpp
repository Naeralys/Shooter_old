//
//  Game.cpp
//  Shooter
//
//  Created by Adam Lind on 5/1/18.
//  Copyright © 2018 Adam Lind. All rights reserved.
//

#include "Game.hpp"
#include "TextureManager.hpp"
#include "Character.hpp"
#include "GameObject.hpp"
#include "Combat.hpp"
#include "Collision.hpp"

Character* playerOne;
Character* playerTwo;
GameObject* bulletOne;
GameObject* bulletTwo;
GameObject* tile[5];
Combat* combat;
Collision* collision;

const Uint8 *keyState;

SDL_Renderer* Game::renderer = nullptr;

void Game::Init( const char *title, int xPos, int yPos, int width, int height, bool fullscreen )
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == 0 )
    {
        window = SDL_CreateWindow( title, xPos, yPos, width, height, fullscreen );
        if( window )
        {
            std::cout << "Window created!" << std::endl;
        }
        else
        {
            std::cout << "Error creating window..." << std::endl;
        }
        
        renderer = SDL_CreateRenderer( window, -1, 0 );
        if( renderer )
        {
            std::cout << "Renderer created!" << std::endl;
        }
        else
        {
            std::cout << "Error creating renderer..." << std::endl;
        }
        isRunning = true;
        std::cout << "SDL Initialized successfully!" << std::endl;
    }
    else
    {
        isRunning = false;
        std::cout << "Error initializing SDL..." << std::endl;
    }
    playerOne = new Character( "assets/player.bmp", Game::renderer,  100, 100 );
    playerTwo = new Character( "assets/player.bmp", Game::renderer,  100, 100 );
    bulletOne = new GameObject( "assets/bullet.bmp", Game::renderer, 0, 0, 5, 5 );
    bulletTwo = new GameObject( "assets/bullet.bmp", Game::renderer, 0, 0, 5, 5 );
    tile[0] = new GameObject( "assets/bullet.bmp", Game::renderer, 200, 400, 100, 10 );
    for( int i = 1; i < 5; i++ ) {
        tile[i] = NULL;
    }
    
}
void Game::HandleEvents() {
    SDL_Event event;
    
    while( SDL_PollEvent( &event ) != 0 ) {
        if( event.type == SDL_QUIT )
            isRunning = false;
        else {
            switch( event.type )
            {
                case SDL_KEYUP:
                    switch( event.key.keysym.sym ){
                        case SDLK_a:
                            playerOne->velX = 0;
                            break;
                        case SDLK_d:
                            playerOne->velX = 0;
                            break;
                        case SDLK_LEFT:
                            playerTwo->velX = 0;
                            break;
                        case SDLK_RIGHT:
                            playerTwo->velX = 0;
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    keyState = SDL_GetKeyboardState( NULL );
    // Player one
    if( keyState[ SDL_SCANCODE_W ] && playerOne->velY == 0 )
        playerOne->velY = -20;
    if( keyState[ SDL_SCANCODE_A ] )
        playerOne->velX = -5;
    if( keyState[ SDL_SCANCODE_D ] )
        playerOne->velX = 5;
    // Combat keys
    playerOne->cooldown++;
    if( playerOne->cooldown >= 30 ) {
        if( keyState[ SDL_SCANCODE_G] ) {
            bulletOne->x= playerOne->x - 16;
            bulletOne->y= playerOne->y + 16;
            combat->Shoot( bulletOne, 1 );
            playerOne->cooldown = 0;
        }
        if( keyState[ SDL_SCANCODE_F ] ) {
            bulletOne->x = playerOne->x + 16;
            bulletOne->y = playerOne->y + 16;
            combat->Shoot( bulletOne, 2 );
            playerOne->cooldown = 0;
        }
        if( keyState[ SDL_SCANCODE_R ] ) {
            combat->Melee( playerOne, playerTwo,  1 );
            playerOne->cooldown = 0;
        }
        if( keyState[ SDL_SCANCODE_T ] ) {
            combat->Melee( playerOne, playerTwo, 2 );
            playerOne->cooldown = 0;
        }
    }
    
    // Player two
    if( keyState[ SDL_SCANCODE_UP ] && playerTwo->velY == 0 )
        playerTwo->velY = -20;
    if( keyState[ SDL_SCANCODE_LEFT ] )
        playerTwo->velX = -5;
    if( keyState[ SDL_SCANCODE_RIGHT ] )
        playerTwo->velX = 5;
    // Combat keys
    playerTwo->cooldown++;
    if( playerTwo->cooldown >= 30 ) {
        if( keyState[ SDL_SCANCODE_L ] ) {
            bulletTwo->x = playerTwo->x - 16;
            bulletTwo->y = playerTwo->y + 16;
            combat->Shoot( bulletTwo, 1 );
            playerTwo->cooldown = 0;
        }
        if( keyState[ SDL_SCANCODE_K ] ) {
            bulletTwo->x = playerTwo->x + 16;
            bulletTwo->y = playerTwo->y + 16;
            combat->Shoot( bulletTwo, 2 );
            playerTwo->cooldown = 0;
        }
        if( keyState[ SDL_SCANCODE_I ] ) {
            combat->Melee( playerTwo, playerOne, 1 );
            playerTwo->cooldown = 0;
        }
        if( keyState[ SDL_SCANCODE_O ] ) {
            combat->Melee( playerTwo, playerOne, 2 );
            playerTwo->cooldown = 0;
        }
    }
        
        
}
void Game::Update()
{
    collision->Map( playerOne, tile );
    collision->Map( playerTwo, tile );
    collision->Bullet( playerTwo, bulletOne );
    collision->Bullet( playerOne, bulletTwo );
    playerOne->Update();
    playerTwo->Update();
    bulletOne->Update();
    bulletTwo->Update();
}
void Game::Render()
{
    SDL_RenderClear( renderer );
    tile[0]->Render();
    playerOne->Render();
    playerTwo->Render();
    bulletOne->Render();
    bulletTwo->Render();
    SDL_RenderPresent( renderer );
}
void Game::Clean()
{
    SDL_DestroyWindow( window );
    SDL_DestroyRenderer( renderer );
    SDL_Quit();
    std::cout << "SDL Terminated..." << std::endl;
}
