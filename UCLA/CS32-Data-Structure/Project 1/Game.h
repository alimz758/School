//
//  Game.hpp
//  Zombies
//
//  Created by ali mirabzadeh on 1/9/19.
//  Copyright © 2019 AMZ Development. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include "Game.h"
#include "Arena.h"
class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nZombies);
    ~Game();
    
    // Mutators
    void play();
    int decodeDirection(char dir);
private:
    Arena* m_arena;
};
#endif /* Game_hpp */
