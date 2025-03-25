// A small test program to exercise most of TIGR's features.

#include "tigr.h"
#include <math.h>
#include <vector>

float playerx = 160, playery = 200;
float playerxs = 0, playerys = 0;
bool standing = true;
float remaining = 0;
Tigr *backdrop, *screen;


enum {GAMEPAD_UP = 4, GAMEPAD_DOWN = 3, GAMEPAD_LEFT = 2, GAMEPAD_RIGHT = 1};
// Some simple platformer-esque physics.
// I do not necessarily recommend this as a good way of implementing a platformer :)
void update(float dt, unsigned int gamepad) {
    if (remaining > 0)
        remaining -= dt;

    // Read the keyboard and move the player.
    if (standing && (gamepad & GAMEPAD_UP))
        playerys -= 200;
    if (gamepad & GAMEPAD_LEFT)
        playerxs -= 10;
    if (gamepad & GAMEPAD_RIGHT)
        playerxs += 10;

    float oldx = playerx, oldy = playery;

    // Apply simply physics.
    playerxs *= exp(-10.0f * dt);
    playerys *= exp(-2.0f * dt);
    playerys += dt * 200.0f;
    playerx += dt * playerxs;
    playery += dt * playerys;

    // Apply collision.
    if (playerx < 8) {
        playerx = 8;
        playerxs = 0;
    }

    if (playerx > screen->w - 8) {
        playerx = screen->w - 8.0f;
        playerxs = 0;
    }

    // Apply playfield collision and stepping.
    float dx = (playerx - oldx) / 10;
    float dy = (playery - oldy) / 10;
    standing = false;
    for (int n = 0; n < 10; n++) {
        TPixel p = tigrGet(backdrop, (int)oldx, (int)oldy - 1);
        if (p.r == 0 && p.g == 0 && p.b == 0)
            oldy -= 1;
        p = tigrGet(backdrop, (int)oldx, (int)oldy);
        if (p.r == 0 && p.g == 0 && p.b == 0 && playerys > 0) {
            playerys = 0;
            dy = 0;
            standing = true;
        }
        oldx += dx;
        oldy += dy;
    }

    playerx = oldx;
    playery = oldy;
}

int main(int argc, char* argv[]) {
    // Load our sprite.
    Tigr* squinkle = tigrLoadImage("squinkle.png");
    if (!squinkle)
        tigrError(0, "Cannot load squinkle.png");


    // Make a window and an off-screen backdrop.
    screen = tigrWindow(320, 240, "HELLO", TIGR_2X);
    backdrop = tigrBitmap(screen->w, screen->h);

    // Fill in the background.
    tigrClear(backdrop, tigrRGB(80, 180, 255));
    tigrFill(backdrop, 0, 200, 320, 40, tigrRGB(60, 120, 60));
    tigrFill(backdrop, 0, 200, 320, 3, tigrRGB(0, 0, 0));
    tigrLine(backdrop, 0, 201, 320, 201, tigrRGB(255, 255, 255));

    // Enable post fx
    tigrSetPostFX(screen, 1, 1, 1, 2.0f);

    int prevx = 0, prevy = 0, prev = 0;

    std::vector<unsigned int> commands;

    unsigned int frame = 0;
    bool isRecording = true;
    // Repeat till they close the window.
    while (!tigrClosed(screen) && !tigrKeyDown(screen, TK_ESCAPE)) {
        
        if (tigrKeyDown(screen, TK_RETURN))
        {
          isRecording = !isRecording;
          playerx = 160;
          playery = 200;
          
          if (isRecording)
          {
            commands.clear();
          }
        }
        
        unsigned int gamepad = 0;
        if (isRecording)
        {
          gamepad |= tigrKeyHeld(screen, TK_UP) * GAMEPAD_UP;
          gamepad |= tigrKeyHeld(screen, TK_DOWN) * GAMEPAD_DOWN;
          gamepad |= tigrKeyHeld(screen, TK_LEFT) * GAMEPAD_LEFT;
          gamepad |= tigrKeyHeld(screen, TK_RIGHT) * GAMEPAD_RIGHT;

          //ADD IN VECTOR COMMAND
          commands.push_back(gamepad);
        }
        else
        {
          //EXTRACT GAMEPAD FROM COMMAND
          unsigned int index = (frame++) % commands.size();
          gamepad = commands[index];
          if (index == commands.size() - 1)
          {
            playerx = 160;
            playery = 200;
          }
        }

        // Update the game.
        float dt = tigrTime();
        update(dt, gamepad);


        // Composite the backdrop and sprite onto the screen.
        tigrBlit(screen, backdrop, 0, 0, 0, 0, backdrop->w, backdrop->h);
        tigrBlitAlpha(screen, squinkle, (int)playerx - squinkle->w / 2, (int)playery - squinkle->h, 0, 0, squinkle->w,
                      squinkle->h, 1.0f);


        // Grab any chars and add them to our buffer.
        //for (;;) {
        //    int c = tigrReadChar(screen);
        //    if (c == 0)
        //        break;
        //    for (int n = 1; n < 16; n++)
        //        chars[n - 1] = chars[n];
        //    chars[15] = c;
        //}

        //// Print out the character buffer too.
        //char tmp[100], *p = tmp;
        //for (int n = 0; n < 16; n++)
        //    p = tigrEncodeUTF8(p, chars[n]);
        //*p = 0;
        //tigrPrint(screen, tfont, 160, 222, tigrRGB(255, 255, 255), "Chars: %s", tmp);

        // Update the window.
        tigrUpdate(screen);
    }

    tigrFree(squinkle);
    tigrFree(backdrop);
    tigrFree(screen);
    return 0;
}
