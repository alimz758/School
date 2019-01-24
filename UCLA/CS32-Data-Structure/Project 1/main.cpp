//
//  main.cpp
//  Zombies
//
//  Created by ali mirabzadeh on 1/9/19.
//  Copyright © 2019 AMZ Development. All rights reserved.
#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>

#include "Game.h"
#include "Arena.h"
#include "Player.h"
#include "Zombie.h"
#include "globals.h"

using namespace std;
///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);
    // Play the game
    g.play();
}


