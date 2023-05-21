// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: NightFox & Co., 2009-2011
//
// Alpha blending example (real alpha blending and fake blending by blinking).
// http://www.nightfoxandco.com

#include <stdio.h>
#include <time.h>

#include <nds.h>
#include <filesystem.h>

#include <nf_lib.h>

int main(int argc, char **argv)
{
    // Set random seed based on the current time
    srand(time(NULL));

    // Prepare a NitroFS initialization screen
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);
    consoleDemoInit();
    printf("\n NitroFS init. Please wait.\n\n");
    printf(" Iniciando NitroFS,\n por favor, espere.\n\n");
    swiWaitForVBlank();

    // Initialize NitroFS and set it as the root folder of the filesystem
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");

    // Initialize 2D engine in both screens and use mode 0
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);

    // Initialize tiled backgrounds system
    NF_InitTiledBgBuffers();    // Initialize storage buffers
    NF_InitTiledBgSys(0);       // Top screen
    NF_InitTiledBgSys(1);       // Bottom screen

    // Initialize sprite system
    NF_InitSpriteBuffers();     // Initialize storage buffers
    NF_InitSpriteSys(0);        // Top screen
    NF_InitSpriteSys(1);        // Bottom screen

    // Load background files from NitroFS
    NF_LoadTiledBg("bg/nfl", "nfl", 256, 256);
    NF_LoadTiledBg("bg/bg3", "capa_3", 256, 256);
    NF_LoadTiledBg("bg/bg2", "capa_2", 1024, 256);

    // Load sprite files from NitroFS
    NF_LoadSpriteGfx("sprite/personaje", 0, 64, 64);
    NF_LoadSpritePal("sprite/personaje", 0);

    NF_LoadSpriteGfx("sprite/bola", 1, 32, 32);
    NF_LoadSpritePal("sprite/bola", 1);

    // Create top screen background
    NF_CreateTiledBg(0, 3, "nfl");

    // Create bottom screen backgrounds
    NF_CreateTiledBg(1, 3, "capa_3");
    NF_CreateTiledBg(1, 2, "capa_2");

    // Transfer the required sprites to VRAM
    NF_VramSpriteGfx(1, 0, 0, true); // Ball: Keep all frames in VRAM
    NF_VramSpritePal(1, 0, 0);

    NF_VramSpriteGfx(0, 1, 0, false); // Character: Keep unused frames in RAM
    NF_VramSpritePal(0, 1, 0);

    // Enable alpha
    //REG_BLDCNT = BLEND_ALPHA // Layer 2 over layer 3 and sprites (top screen)
    //           | BLEND_SRC_BG2
    //           | BLEND_DST_BG3 | BLEND_DST_SPRITE;
    REG_BLDCNT_SUB = BLEND_ALPHA // Layer 2 over layer 3 (bottom screen)
                   | BLEND_SRC_BG2
                   | BLEND_DST_BG3;

    // Alpha value
    //REG_BLDALPHA = 0x05 | (0x0F << 8); // Top screen (5 / 15)
    REG_BLDALPHA_SUB = 0x00 | (0x0F << 8); // Bottom screen (0 / 15)

    // Variables
    s8 blink = 1;
    u8 alpha = 0;
    u8 alpha_timer = 0;
    s8 alpha_inc = 1;

    // Setup character sprite
    s16 pj_x = 0;
    s16 pj_y = 127;
    u8 pj_frame = 0;
    u8 pj_anim = 0;
    s8 pj_speed = 1;
    NF_CreateSprite(1, 0, 0, 0, pj_x, pj_y);

    // Setup ball sprites
    s16 bola_x[32];
    s16 bola_y[32];
    s8 bola_spx[32];
    s8 bola_spy[32];

    for (int n = 0; n < 32; n++)
    {
        bola_x[n] = rand() % 223;
        bola_y[n] = rand() % 159;
        bola_spx[n] = (rand() % 3) + 1;
        bola_spy[n] = (rand() % 3) + 1;
        NF_CreateSprite(0, n, 0, 0, bola_x[n], bola_y[n]);
    }

    while (1)
    {
        // Move character
        pj_x += pj_speed;
        if ((pj_x < 0) || (pj_x > 191))
        {
            pj_speed *= -1;
            if (pj_speed > 0)
                NF_HflipSprite(1, 0, false);
            else
                NF_HflipSprite(1, 0, true);
        }
        NF_MoveSprite(1, 0, pj_x, pj_y);

        // Animate character
        pj_anim++;
        if (pj_anim > 5)
        {
            pj_anim = 0;
            pj_frame++;
            if (pj_frame > 11)
                pj_frame = 0;
            NF_SpriteFrame(1, 0, pj_frame);
        }

        // Move balls
        for (int n = 0; n < 32; n++)
        {
            bola_x[n] += bola_spx[n];
            if ((bola_x[n] < 0) || (bola_x[n] > 223))
                bola_spx[n] *= -1;

            bola_y[n] += bola_spy[n];
            if ((bola_y[n] < 0) || (bola_y[n] > 159))
                bola_spy[n] *= -1;

            NF_MoveSprite(0, n, bola_x[n], bola_y[n]);

            // Fake alpha effect in the last 15 balls. This is done by showing
            // the balls every other frame. This only looks fine in old LCD
            // screens with long refresh times. In old DS models it looks better
            // than 2DS/3DS and emulators for PC.
            if (n < 15)
            {
                if (blink)
                    NF_ShowSprite(0, n, true);
                else
                    NF_ShowSprite(0, n, false);
            }
        }
        blink ^= 1;

        // "Dissolve" background effect
        alpha_timer++;
        if (alpha_timer > 5)
        {
            alpha_timer = 0;
            alpha += alpha_inc;
            if ((alpha == 0) || (alpha == 14))
                alpha_inc *= -1;

            // Alpha level in bottom screen (the max is 15)
            REG_BLDALPHA_SUB = alpha | (0x0F << 8);
        }

        // Update OAM array
        NF_SpriteOamSet(0);
        NF_SpriteOamSet(1);

        // Wait for the screen refresh
        swiWaitForVBlank();

        // Update OAM
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }

    return 0;
}
