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

Character* playerOne, *playerTwo;
GameObject* bulletOne, *bulletTwo, *tile[5], *map;
Combat* combat;
Collision* collision;
SDL_GameController* controller[2];

float movementSpeed = 0.0002;
const Uint8 *keyState;

SDL_Renderer* Game::renderer = nullptr;

void Game::Init( const char *title, int xPos, int yPos, int width, int height, bool fullscreen ) {
    if( SDL_Init( SDL_INIT_EVERYTHING ) == 0 ) {
        window = SDL_CreateWindow( title, xPos, yPos, width, height, fullscreen );
        if( window ) {
            std::cout << "Window created!" << std::endl;
        }
        else {
            std::cout << "Error creating window..." << std::endl;
        }
        
        renderer = SDL_CreateRenderer( window, -1, 0 );
        if( renderer ) {
            std::cout << "Renderer created!" << std::endl;
        }
        else {
            std::cout << "Error creating renderer..." << std::endl;
        }
        isRunning = true;
        std::cout << "SDL Initialized successfully!" << std::endl;
    }
    else {
        isRunning = false;
        std::cout << "Error initializing SDL..." << std::endl;
    }
    
    // INITIALIZE GAME CONTROLLER
    for( int i = 0; i < 2; i++ ) {
        if( SDL_IsGameController( i ) ) {
            controller[i] = SDL_GameControllerOpen( i );
            if( controller[i] ) {
                std::cout << "Game controller found" << std::endl;
            }
            else {
                std::cout << "Could not open game controller" << std::endl;
            }
        }
    }
    
    map = new GameObject( "assets/background.bmp", Game::renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    playerOne = new Character( "assets/player.bmp", Game::renderer,  100, 100 );
    playerTwo = new Character( "assets/player.bmp", Game::renderer,  100, 100 );
    bulletOne = new GameObject( "assets/bullet.bmp", Game::renderer, 0, 0, 5, 5 );
    bulletTwo = new GameObject( "assets/bullet.bmp", Game::renderer, 0, 0, 5, 5 );
    tile[0] = new GameObject( "assets/bullet.bmp", Game::renderer, 100, SCREEN_HEIGHT - 250, 100, 10 );
    tile[1] = new GameObject( "assets/bullet.bmp", Game::renderer, (SCREEN_WIDTH/2) - 50, SCREEN_HEIGHT - 300, 100, 10 );
    tile[2] = new GameObject( "assets/bullet.bmp", Game::renderer, SCREEN_WIDTH - 200, SCREEN_HEIGHT - 250, 100, 10 );
}
void Game::HandleEvents() {
    SDL_Event event;
    while( SDL_PollEvent( &event ) != 0 ) {
        if( event.type == SDL_QUIT )
            isRunning = false;
    }
    keyState = SDL_GetKeyboardState( NULL );
    
    /*                        *
     * PLAYER ONE CONTROLLERS *
     *                        */
    
    // Keyboard - Movement
    if( !playerOne->CheckKeyLock() ) {
        // Jump
        if( keyState[ SDL_SCANCODE_W ] && playerOne->velY == 0 )
            playerOne->velY = -20;
        // Movement
        if( keyState[ SDL_SCANCODE_A ] )
            playerOne->velX = -5;
        if( keyState[ SDL_SCANCODE_D ] )
            playerOne->velX = 5;
        // Dash
        if( keyState[ SDL_SCANCODE_E ] && playerOne->velX > -2 && playerOne->velX <= 5 ) {
            playerOne->velX = 15;
            playerOne->KeyLock( 25 );
        }
        if( keyState[ SDL_SCANCODE_Q ] && playerOne->velX < 2 && playerOne->velX >= -5 ) {
            playerOne->velX = -15;
            playerOne->KeyLock( 25 );
        }
    }
    // Keyboard - Combat
    playerOne->cooldown++;
    if( !playerOne->CheckKeyLock() && playerOne->cooldown >= 30 ) {
        // Shoot
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
        // Melee
        if( keyState[ SDL_SCANCODE_R ] ) {
            combat->Melee( playerOne, playerTwo,  1 );
            playerOne->cooldown = 0;
        }
        if( keyState[ SDL_SCANCODE_T ] ) {
            combat->Melee( playerOne, playerTwo, 2 );
            playerOne->cooldown = 0;
        }
    }
    // Controller - Movement
    if( !playerOne->CheckKeyLock() ) {
        // Jump
        if( SDL_GameControllerGetButton( controller[0], SDL_CONTROLLER_BUTTON_A ) && playerOne->velY == 0 )
            playerOne->velY = -20;
        // Movement
        if( SDL_GameControllerGetAxis( controller[0], SDL_CONTROLLER_AXIS_LEFTX ) < -15000 )
            playerOne->velX = SDL_GameControllerGetAxis( controller[0], SDL_CONTROLLER_AXIS_LEFTX ) * movementSpeed;
        if( SDL_GameControllerGetAxis( controller[0], SDL_CONTROLLER_AXIS_LEFTX ) > 15000)
            playerOne->velX = SDL_GameControllerGetAxis( controller[0], SDL_CONTROLLER_AXIS_LEFTX ) * movementSpeed;
        // Dash
        if( SDL_GameControllerGetAxis( controller[0], SDL_CONTROLLER_AXIS_RIGHTX ) < -15000 && playerOne->velX > -2 && playerOne->velX <= 5 ) {
            playerOne->velX = 15;
            playerOne->KeyLock( 25 );
        }
        if( SDL_GameControllerGetAxis( controller[0], SDL_CONTROLLER_AXIS_RIGHTX ) > 15000 && playerOne->velX < 2 && playerOne->velX >= -5 ) {
            playerOne->velX = -15;
            playerOne->KeyLock( 25 );
        }
    }
    // Controller - Combat
    playerOne->cooldown++;
    if( !playerOne->CheckKeyLock() && playerOne->cooldown >= 30 ) {
        // Shoot
        if( SDL_GameControllerGetButton( controller[0], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER ) ) {
            bulletOne->x= playerOne->x - 16;
            bulletOne->y= playerOne->y + 16;
            combat->Shoot( bulletOne, 1 );
            playerOne->cooldown = 0;
        }
        if( SDL_GameControllerGetButton( controller[0], SDL_CONTROLLER_BUTTON_LEFTSHOULDER ) ) {
            bulletOne->x = playerOne->x + 16;
            bulletOne->y = playerOne->y + 16;
            combat->Shoot( bulletOne, 2 );
            playerOne->cooldown = 0;
        }
        // Melee
        if( SDL_GameControllerGetAxis( controller[0], SDL_CONTROLLER_AXIS_TRIGGERLEFT ) > 10000 ) {
            combat->Melee( playerOne, playerTwo,  1 );
            playerOne->cooldown = 0;
        }
        if( SDL_GameControllerGetAxis( controller[0], SDL_CONTROLLER_AXIS_TRIGGERRIGHT ) > 10000 ) {
            combat->Melee( playerOne, playerTwo, 2 );
            playerOne->cooldown = 0;
        }
    }
    
    /*                        *
     * PLAYER TWO CONTROLLERS *
     *                        */
    
    // Keyboard - Movement
    if( !playerTwo->CheckKeyLock() ) {
        // Jump
        if( keyState[ SDL_SCANCODE_UP ] && playerTwo->velY == 0 )
            playerTwo->velY = -20;
        // Movement
        if( keyState[ SDL_SCANCODE_LEFT ] )
            playerTwo->velX = -5;
        if( keyState[ SDL_SCANCODE_RIGHT ] )
            playerTwo->velX = 5;
        // Dash
        if( keyState[ SDL_SCANCODE_9 ] && playerTwo->velX > -2 && playerTwo->velX <= 5 ) {
            playerTwo->velX = 15;
            playerTwo->KeyLock( 25 );
        }
        if( keyState[ SDL_SCANCODE_8 ] && playerTwo->velX < 2 && playerTwo->velX >= -5 ) {
            playerTwo->velX = -15;
            playerTwo->KeyLock( 25 );
        }
    }
    // Keyboard - Combat
    playerTwo->cooldown++;
    if( !playerTwo->CheckKeyLock() && playerTwo->cooldown >= 30 ) {
        // Shoot
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
        // Melee
        if( keyState[ SDL_SCANCODE_I ] ) {
            combat->Melee( playerTwo, playerOne, 1 );
            playerTwo->cooldown = 0;
        }
        if( keyState[ SDL_SCANCODE_O ] ) {
            combat->Melee( playerTwo, playerOne, 2 );
            playerTwo->cooldown = 0;
        }
    }
    // Controller - Movement
    if( !playerTwo->CheckKeyLock() ) {
        // Jump
        if( SDL_GameControllerGetButton( controller[1], SDL_CONTROLLER_BUTTON_A ) && playerTwo->velY == 0 )
            playerTwo->velY = -20;
        // Movement
        if( SDL_GameControllerGetAxis( controller[1], SDL_CONTROLLER_AXIS_LEFTX ) < -15000 )
            playerTwo->velX = SDL_GameControllerGetAxis( controller[1], SDL_CONTROLLER_AXIS_LEFTX ) * movementSpeed;
        if( SDL_GameControllerGetAxis( controller[1], SDL_CONTROLLER_AXIS_LEFTX ) > 15000 )
            playerTwo->velX = SDL_GameControllerGetAxis( controller[1], SDL_CONTROLLER_AXIS_LEFTX ) * movementSpeed;
        // Dash
        if( SDL_GameControllerGetAxis( controller[1], SDL_CONTROLLER_AXIS_RIGHTX ) < -15000 && playerTwo->velX > -2 && playerTwo->velX <= 5 ) {
            playerTwo->velX = -15;
            playerTwo->KeyLock( 25 );
        }
        if( SDL_GameControllerGetAxis( controller[1], SDL_CONTROLLER_AXIS_RIGHTX ) > 15000 && playerTwo->velX < 2 && playerTwo->velX >= -5 ) {
            playerTwo->velX = 15;
            playerTwo->KeyLock( 25 );
        }
    }
    // Controller - Combat
    playerTwo->cooldown++;
    if( !playerTwo->CheckKeyLock() && playerTwo->cooldown >= 30 ) {
        // Shoot
        if( SDL_GameControllerGetButton( controller[1], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER ) ) {
            bulletTwo->x = playerTwo->x - 16;
            bulletTwo->y = playerTwo->y + 16;
            combat->Shoot( bulletTwo, 1 );
            playerTwo->cooldown = 0;
        }
        if( SDL_GameControllerGetButton( controller[1], SDL_CONTROLLER_BUTTON_LEFTSHOULDER ) ) {
            bulletTwo->x = playerTwo->x + 16;
            bulletTwo->y = playerTwo->y + 16;
            combat->Shoot( bulletTwo, 2 );
            playerTwo->cooldown = 0;
        }
        // Melee
        if( SDL_GameControllerGetAxis( controller[1], SDL_CONTROLLER_AXIS_TRIGGERLEFT ) > 10000 ) {
            combat->Melee( playerTwo, playerOne, 1 );
            playerTwo->cooldown = 0;
        }
        if( SDL_GameControllerGetAxis( controller[1], SDL_CONTROLLER_AXIS_TRIGGERRIGHT ) > 10000 ) {
            combat->Melee( playerTwo, playerOne, 2 );
            playerTwo->cooldown = 0;
        }
    }
}
void Game::Update() {
    collision->Map( playerOne, tile );
    collision->Map( playerTwo, tile );
    collision->Bullet( playerTwo, bulletOne );
    collision->Bullet( playerOne, bulletTwo );
    playerOne->Update();
    playerTwo->Update();
    bulletOne->Update();
    bulletTwo->Update();
    if( playerOne->x < 0 || playerOne->x > SCREEN_WIDTH ) {
        std::cout << "Player Two Wins!" << std::endl;
        isRunning = false;
    }
    if( playerTwo->x < 0 || playerTwo->x > SCREEN_WIDTH ) {
        std::cout << "Player One Wins!" << std::endl;
        isRunning = false;
    }
}
void Game::Render() {
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0 );
    SDL_RenderClear( renderer );
    map->Render();
    tile[0]->Render();
    tile[1]->Render();
    tile[2]->Render();
    playerOne->Render();
    playerTwo->Render();
    bulletOne->Render();
    bulletTwo->Render();
    SDL_RenderPresent( renderer );
}
void Game::Clean() {
    SDL_DestroyWindow( window );
    SDL_DestroyRenderer( renderer );
    SDL_Quit();
    std::cout << "SDL Terminated..." << std::endl;
}
