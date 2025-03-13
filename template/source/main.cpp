#include <stdio.h>
#include <nds.h>
#include <filesystem.h>
#include <nf_lib.h>

class Mario {
public:
    float world_xpos = 16, world_ypos = 144; // Mario's actual world position
    float xpos = 16, ypos = 144; // Screen position relative to camera
    float xspeed = 0, yspeed = 0;
    float xaccel = 0;
    float xmax = 2.5;
    float xmax_sprint = xmax * 1.4;
    float ymax = 4.3125;
    float gravity = 0;
    int direction = 1;
    bool onGround = true;

    float accel_right = 0.0546875f;
    float accel_left = -accel_right;
    float accel_right_move = 0.125f;
    float accel_left_move = -accel_right_move;

    void update(int camera_x, int camera_y) {
        xspeed += xaccel;
        yspeed += gravity;
        world_xpos += xspeed;
        world_ypos += yspeed;

        if (xspeed > xmax) xspeed = xmax;
        if (xspeed < -xmax) xspeed = -xmax;
        if (yspeed > ymax) yspeed = ymax;

        if ((direction == 1 && xspeed < 0.01) || (direction == -1 && xspeed > -0.01)) {
            xspeed = 0;
        }

        xpos = world_xpos - camera_x;
        ypos = world_ypos - camera_y;

        NF_MoveSprite(0, 0, xpos, ypos);

        if (direction == 1) {
            NF_HflipSprite(0, 0, false);
        } else {
            NF_HflipSprite(0, 0, true);
        }
    }

    void handleInput(uint16_t keys_held) {
        if (keys_held & KEY_LEFT) {
            if (xspeed == 0) direction = -1;
            xaccel = (xspeed != 0) ? accel_left_move : accel_left;
        } else if (keys_held & KEY_RIGHT) {
            if (xspeed == 0) direction = 1;
            xaccel = (xspeed != 0) ? accel_right_move : accel_right;
        } else {
            if (onGround) {
                if (xspeed > 0) xaccel = accel_left;
                else if (xspeed < 0) xaccel = accel_right;
                else xaccel = 0;
            }
        }
    }

    void printState() {
        printf("X: %.2f Y: %.2f XSpeed: %.2f YSpeed: %.2f Direction: %d\n",
             world_xpos, world_ypos, xspeed, yspeed, direction);
    }
};

int main(int argc, char **argv) {
    Mario mario;



    consoleDemoInit();
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");

    NF_Set2D(0, 0);
    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(0);
    NF_InitSpriteBuffers();
    NF_InitSpriteSys(0);

    NF_LoadTiledBg("bg/bg2", "layer_3", 256, 256);
    NF_CreateTiledBg(0, 3, "layer_3");

    NF_LoadSpriteGfx("sprite/mario", 1, 16, 16);
    NF_LoadSpritePal("sprite/mario", 1);
    NF_VramSpriteGfx(0, 1, 0, true);
    NF_VramSpritePal(0, 1, 0);
    NF_CreateSprite(0, 0, 0, 0, 31, 159);

    int camera_x = 0, camera_y = 0;
    int screen_width = 256, screen_height = 192;

    const int SCROLL_X = (screen_width / 2);
    const int SCROLL_Y = (screen_height / 2);

    while (1) {
        scanKeys();
        uint16_t keys_held = keysHeld();

        mario.handleInput(keys_held);
        mario.update(camera_x, camera_y);

        if (mario.world_xpos - camera_x > screen_width - SCROLL_X) {
            camera_x = mario.world_xpos - (screen_width - SCROLL_X);
        } else if (mario.world_xpos - camera_x < SCROLL_X) {
            camera_x = mario.world_xpos - SCROLL_X;
        }

        if (mario.world_ypos - camera_y > screen_height - SCROLL_Y) {
            camera_y = mario.world_ypos - (screen_height - SCROLL_Y);
        } else if (mario.world_ypos - camera_y < SCROLL_Y) {
            camera_y = mario.world_ypos - SCROLL_Y;
        }

        mario.printState();

        if (camera_x < 0) camera_x = 0;
        if (camera_y < 0) camera_y = 0;


        NF_SpriteOamSet(0);
        swiWaitForVBlank();

        NF_ScrollBg(0, 3, camera_x, camera_y);

        oamUpdate(&oamMain);
    }

    return 0;
}
