#include "UI/Data.hpp"
#include <iostream>

void Data::load()
{
    loadAsset(font, "assets/fonts/arial.ttf");
    loadAsset(tmenu, "assets/sprites/menu/login_screen.png");
    loadAsset(tloginbutton, "assets/sprites/menu/login_button.jpg");
    loadAsset(menuMusic, "assets/sounds/menu_music.ogg");
    loadAsset(gameMusic, "assets/sounds/game_music.ogg");
    loadAsset(tprofile, "assets/sprites/menu/profile.png");
    loadAsset(tready, "assets/sprites/menu/ready.png");
    loadAsset(treadybutton, "assets/sprites/menu/ready_button.png");
    loadAsset(tdisconnectbutton, "assets/sprites/menu/disconnect_button.png");
    loadAsset(tloading, "assets/sprites/menu/loading.png");
    loadAsset(tmap, "assets/sprites/game/background_map.png");
    loadAsset(tmapdead, "assets/sprites/game/background_map_dead.png");
    loadAsset(tplayer0, "assets/sprites/game/player0.png");
    loadAsset(tplayer1, "assets/sprites/game/player1.png");
    loadAsset(tplayer2, "assets/sprites/game/player2.png");
    loadAsset(tplayer3, "assets/sprites/game/player3.png");
    loadAsset(tenemy0, "assets/sprites/game/enemy0.png");
    loadAsset(tenemy1, "assets/sprites/game/enemy1.png");
    loadAsset(tenemy2, "assets/sprites/game/enemy2.png");
    loadAsset(tplayershot, "assets/sprites/game/playershot.png");
    loadAsset(tenemyshot, "assets/sprites/game/enemyshot.png");
    loadAsset(tblock, "assets/sprites/game/block.png");
    loadAsset(tlobbybutton, "assets/sprites/game/lobby_button.png");
    loadAsset(tdisconnectbuttongame, "assets/sprites/game/disconnect_button.png");
    loadAsset(bplayershot, "assets/sounds/player_shot.wav");
    splayershot.setBuffer(bplayershot);
}