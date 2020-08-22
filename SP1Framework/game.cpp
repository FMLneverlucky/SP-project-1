// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "time.h"
#include <stdlib.h>
#include "Projectile.h"
#include "Wall.h"
#include "PowerUp.h"

//FOR TESTING
bool checkInputs = false;
bool checkTimeElapsed = false;
bool checkFramerate = true;
float splashScreenTime = 0.5;

//UI NAMES
std::string gameName = "A Very Fun Game";
std::string gameMode1 = "GameMode1";
std::string gameMode2 = "GameMode2";
std::string gameMode3 = "GameMode3";
std::string gameMode4 = "Click This"; // for game test. not for final product
std::string winMessage = "HACKS REPORTED";
std::string loseMessage = "GGEZ Uninstall";
std::string continueMessage = "Next Level";
std::string restartMessage = "Redo Level";
std::string mainMenuMessage = "Main Menu";
std::string quit = "quit";
std::string resume = "resume";

//MAINMENU
Object* MMButtons[4];
Object MMButton(gameMode1.length() + 2, 3);
Object MMButton2(gameMode2.length() + 2, 3);
Object MMButton3(gameMode3.length() + 2, 3);
Object MMButton4(gameMode4.length() + 2, 3);
const int MMButtonCount = 4; 

//PAUSE MENU
bool paused = false;
Object* PMButtons[2];
Object resumeButton(resume.length() + 2, 3);
Object quitButton(quit.length() + 2, 3);
const int PMButtonCount = 2;

//WIN/LOSE SCREEN
Object* WLButtons[3];
Object continueButton(continueMessage.length() + 2, 3);
Object restartButton(restartMessage.length() + 2, 3);
Object mainMenuButton(restartMessage.length() + 2, 3);
const int WLButtonCount = 3;

//NORMAL MODE
NormalMode NGameState = N_INIT;
int level = 1; //level no.
bool lose = false; //end game
bool clear = false;
int noC; //no. of civilian
int noP; //no. of Police
float spd; //spd of NPCs relative to player
int cdtime; //cooldown time of hostile NPCs after collision w player
int noW; //no of walls
Position endPoint[9];
Position spawnPoint[9];

//TEST
//double timer = 0;

//map aesthetics
int prevcol = 0x88;

double  g_dElapsedTime;
double  g_dDeltaTime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_MAINMENU; // initial state

Player* player = new Player;

Entity* entities[31] = { player , nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
const int entityLimit = 31;

NPC* NPCs[20] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
const int NPCLimit = 20;

Wall* Walls[10] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
const int WallLimit = 10;

Projectile* projectile[3] = { nullptr, nullptr, nullptr };
int particle_limit = 3;

PowerUp* powerup = nullptr;

// Console object
Console g_Console(80, 25, "SP1 Framework");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;    

    // sets the initial state for the game
    g_eGameState = S_MAINMENU;

    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;
    player->set_pos(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y);
    g_sChar.m_bActive = true;
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler);

    //spawnWall(10);
    //spawnNPC(false, 3, 0.1, 3);
    //spawnNPC(true, 2, 0.1, 3);
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();

    for (int i = 0; i < entityLimit; i++)
    {
        if (entities[i] != nullptr)
        {
            delete entities[i];
        }
    }

    for (int p = 0; p < particle_limit; p++)
    {
        if (projectile[p] != nullptr)
        {
            if (projectile[p]->get_px() == 0 || projectile[p]->get_px() == 79)
                delete projectile[p];
        }
    }
    

    if (powerup != nullptr)
    {
        delete powerup;
    }
}

//--------------------------------------------------------------
// Purpose  : Get all the console input events
//            This function sets up the keyboard and mouse input from the console.
//            We will need to reset all the keyboard status, because no events will be sent if no keys are pressed.
//
//            Remember to set the handlers for keyboard and mouse events.
//            The function prototype of the handler is a function that takes in a const reference to the event struct
//            and returns nothing. 
//            void pfKeyboardHandler(const KEY_EVENT_RECORD&);
//            void pfMouseHandlerconst MOUSE_EVENT_RECORD&);
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{
    // resets all the keyboard events
    memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    // then call the console to detect input from user
    g_Console.readConsoleInput();    
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the keyboard input. Whenever there is a keyboard event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            The KEY_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any keyboard event in the Splashscreen state
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent - reference to a key event struct
// Output   : void
//--------------------------------------------------------------
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent)
{    
    switch (g_eGameState)
    {
    case S_MAINMENU: gameplayKBHandler(keyboardEvent); // handle thing for the splash screen
        break;
    case S_TEST: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    case S_GAMEMODE1: gameplayKBHandler(keyboardEvent); 
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the mouse input. Whenever there is a mouse event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            For the mouse event, if the mouse is not moved, no event will be sent, hence you should not reset the mouse status.
//            However, if the mouse goes out of the window, you would not be able to know either. 
//
//            The MOUSE_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any mouse event in the Splashscreen state
//            
// Input    : const MOUSE_EVENT_RECORD& mouseEvent - reference to a mouse event struct
// Output   : void
//--------------------------------------------------------------
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{    
    switch (g_eGameState)
    {
    case S_MAINMENU: gameplayMouseHandler(mouseEvent); // don't handle anything for the splash screen
        break;
    case S_TEST: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    case S_GAMEMODE1: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the keyboard handler in the game state. Whenever there is a keyboard event in the game state, this function will be called.
//            
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case 0x57: key = K_W; break;
    case 0x53: key = K_S; break;
    case 0x41: key = K_A; break;
    case 0x44: key = K_D; break;
    case VK_SPACE: key = K_SPACE; break;
    case VK_ESCAPE: key = K_ESCAPE; break; 
    }
    // a key pressed event would be one with bKeyDown == true
    // a key released event would be one with bKeyDown == false
    // if no key is pressed, no event would be fired.
    // so we are tracking if a key is either pressed, or released
    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        if (g_skKeyEvent[key].keyDown)
            buttonHoldPress(key);
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
        if (g_skKeyEvent[key].keyReleased)
            buttonHoldRelease(key);
    }    
}

bool heldKey[6] = {false, false, false, false, false, false};
void buttonHoldPress(EKEYS key)
{
    for (int i = 0; i < 6; i++)
    {
        heldKey[i] = (i == key) ? true : false;
    }
}
void buttonHoldRelease(EKEYS key)
{
    heldKey[key] = false;
}
int getButtonHold()
{
    for (int i = 0; i < 6; i++)
    {
        if (heldKey[i] == true)
        {
            return i;
        }
    }
    return 7;
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the game state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_MAINMENU: mainMenuWait(); // game logic for the splash screen
            break;
        case S_TEST: updateGame(); // gameplay logic when we are in the game
            break;
        case S_GAMEMODE1: playNormal();
            break;
    }
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > splashScreenTime) // wait for set time to switch to game mode, else do nothing
        g_eGameState = S_TEST;
}

void updateGame()       // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    if (!paused)
        moveCharacter();    // moves the character, collision detection, physics, etc
                            // sound can be played here too.
    else
    {
        pauseMenuWait();
    }                
    
}

void playNormal()
{
    switch (NGameState)
    {
    case N_INIT:
        InitNormal();
        break;
    case N_LEVEL:
        playLevel();
        break;
    case N_NEXTLEVEL:
        level_end();
        level_start();
        break;

    }

}

void InitNormal()
{
    lose = false;
    player->resetHP();
    noC = 3;
    noP = 0;
    spd = 0.1;
    cdtime = 5;
    noW = 7;
    spawnNPC(false, noC, spd, cdtime);
    spawnNPC(true, noP, spd, cdtime);
    set_points();
    //spawnWall(noW);
    
    NGameState = N_LEVEL;
}
 
void set_spawn()
{
    
    if (level < 6)
    {
        noC++;
        spd += 0.0285;
        cdtime -= 0.2857;
    }
    else if (level < 15)
    {
        spd += 0.0285;
        cdtime -= 0.2857;
        if (level % 2)
        {
            noC++;
            noP = level - 5;
        }
    }
    else
    {
        spd = 0.5;
        cdtime = 1;
        noP = 5;
        noC = 15;
    }

    spawnNPC(false, noC, spd, cdtime);
    spawnNPC(true, noP, spd, cdtime);
    //spawnWall(noW);
    set_points();
}

void set_points()
{
    spawnPoint[4].set_x(g_sChar.m_cLocation.X);
    spawnPoint[4].set_y(g_sChar.m_cLocation.Y);

    Position tempp;
    do
    {
        tempp.set_x((rand() % 78) + 1);
        tempp.set_y((rand() % 22) + 1);

    } while (occupied(&tempp) != nullptr);
    endPoint[4].set_x(tempp.get_x());
    endPoint[4].set_y(tempp.get_y());

    for (int i = -1; i < 2; i++)
    {
        spawnPoint[1 + i].set_pos(spawnPoint[4].get_x() + i, spawnPoint[4].get_y() - 1);
        spawnPoint[7 + i].set_pos(spawnPoint[4].get_x() + i, spawnPoint[4].get_y() + 1);
    
        endPoint[1 + i].set_pos(endPoint[4].get_x() + i, endPoint[4].get_y() - 1);
        endPoint[7 + i].set_pos(endPoint[4].get_x() + i, endPoint[4].get_y() + 1);
      
        spawnPoint[3].set_pos(spawnPoint[4].get_x() - 1, spawnPoint[4].get_y());
        spawnPoint[5].set_pos(spawnPoint[4].get_x() + 1, spawnPoint[4].get_y());
        endPoint[3].set_pos(endPoint[4].get_x() - 1, endPoint[4].get_y());
        endPoint[5].set_pos(endPoint[4].get_x() + 1, endPoint[4].get_y());

    }
}

void level_end()
{
    for (int i = 0; i < NPCLimit; i++)
    {
        if (NPCs[i] != nullptr)
        {
            delete NPCs[i];
            NPCs[i] = nullptr;
        }
    }

    for (int p = 0; p < particle_limit; p++)
    {
        if (projectile[p] != nullptr)
        {
            delete projectile[p];
            projectile[p] = nullptr;
        }
    }


    if (powerup != nullptr)
    {
        delete powerup;
        powerup = nullptr;
    }

}

void level_start()
{
    level++;
    player->resetHP();
    NPC::resetnoHostile();
    set_spawn();
    clear = false;
    NGameState = N_LEVEL;
    setBG();
  
}

void playLevel()
{
    
    updateGame();

    if (NPC::getnoHostile() == noC + noP && static_cast<int>(player->getposx()) == endPoint[4].get_x() && static_cast<int>(player->getposy()) == endPoint[4].get_y())
    {
        clear = true;
        //print level clear screen here
        NGameState = N_NEXTLEVEL;
    }

    if (player->get_HP() <= 0)
    {
        lose = true;
        level_end();
        NGameState = N_INIT;
        g_eGameState = S_MAINMENU;
        //print lose screen here  
    }
}
    

void moveCharacter()
{   
    // Updating the location of the character based on the key release
    // providing a beep sound whenver we shift the character
    if (getButtonHold() == K_W && g_sChar.m_cLocation.Y > 0)
    {
        //Beep(1440, 30);
        //g_sChar.m_cLocation.Y--;
        entities[0]->set_direction(1);
        
    }
    else if (getButtonHold() == K_A && g_sChar.m_cLocation.X > 0)
    {
        //Beep(1440, 30);
        //g_sChar.m_cLocation.X--;     
        entities[0]->set_direction(3);
    }
    else if (getButtonHold() == K_S && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);
        //g_sChar.m_cLocation.Y++;       
        entities[0]->set_direction(2);
    }
    else if (getButtonHold() == K_D && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
        //Beep(1440, 30);
        //g_sChar.m_cLocation.X++;        
        entities[0]->set_direction(4);
    }
    else
    {
        entities[0]->set_direction(0);
    }

    if (occupied(entities[0]->new_pos(g_dDeltaTime)) != nullptr && occupied(entities[0]->new_pos(g_dDeltaTime)) != entities[0])
    {
        entities[0]->set_direction(0);
    }
    
    if (g_skKeyEvent[K_SPACE].keyReleased)
    {
        for (int p = 0; p < particle_limit; p++)
        {
            if (projectile[p] == nullptr)
            {

                projectile[p] = new Projectile;
                projectile[p]->set_ppos(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y);
                projectile[p]->direction(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y);
                projectile[p]->set_newpos(); 
                projectile[p]->set_pcooldown(100);
                break;
            }
        }

        //g_sChar.m_bActive = !g_sChar.m_bActive;
        for (int n = 0; n < NPCLimit; n++)
        {
            if (NPCs[n] != nullptr)
            {
                if (NPCs[n]->type() == 'B')
                {
                    NPCs[n]->anger();
                }
            }
        }
    }

    player->set_cooldown(player->get_cooldown() - 1);
    player->update_pos(g_dDeltaTime); //sets pos of player
    g_sChar.m_cLocation.Y = player->getposy(); //moves player
    g_sChar.m_cLocation.X = player->getposx(); //moves player
    
    moveall(); //moves NPCs
    check_collision();
    limitprojectile(); //moves/updates projectiles
    

    for (int p = 0; p < particle_limit; p++)
    {
        if ((projectile[p] != nullptr) && (occupied(projectile[p]->getpos()) != nullptr) && occupied(projectile[p]->getpos())->type() == 'C')
        {
            for (int i = 0; i < NPCLimit; i++)
            {
                if (NPCs[i] == occupied(projectile[p]->getpos()) && NPCs[i]->isHostile() == false)
                {
                    NPCs[i]->anger();
                    NPCs[i]->cooldownstart();
                    NPCs[i]->set_count(NPCs[i]->get_ftime() / g_dDeltaTime);
                    
                }
            }
        }
    }
}

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
        paused = paused ? false : true;
        //g_bQuitGame = true;    
}

//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
    case S_MAINMENU: renderMainMenu();
        break;
    case S_TEST: renderGame();
        break;
    case S_GAMEMODE1: 
        renderBG(prevcol);
        renderPoints();
        renderGame();
        
        break;
    }
    renderFramerate();      // renders debug information, frame rate, elapsed time, etc
    if (checkInputs)
        renderInputEvents();    // renders status of input events
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x1F);
}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void renderSplashScreen()  // renders the splash screen
{
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 9;
    g_Console.writeToBuffer(c, "A game in 3 seconds", 0x03);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(c, "Press <Space> to change character colour", 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x09);
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    if (paused)
        renderPauseMenu();
    /*if (lose)
    {
        renderWinLoseMenu(false);
    }
    if (clear)
    {
        renderWinLoseMenu(true);
    }*/

    
}

void renderMap()
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

   /* COORD c;
    for (int i = 0; i < 12; ++i)
    {
        c.X = 5 * i;
        c.Y = i + 1;
        colour(colors[i]);
        g_Console.writeToBuffer(c, " °±²Û", colors[i]);
        
    }*/

    //renderWall();
    //renderPoints();
    renderNPC();
    renderprojectile();
    
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0C;
    if (g_sChar.m_bActive)
    {
        charColor = 0x09;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, charColor);
}

void renderFramerate()
{
    COORD c;
    std::ostringstream ss;
    // displays the framerate
    if (checkFramerate)
    {
        ss << std::fixed << std::setprecision(3);
        ss << 1.0 / g_dDeltaTime << "fps";
        c.X = g_Console.getConsoleSize().X - 9;
        c.Y = 0;
        g_Console.writeToBuffer(c, ss.str());
    }
    // displays the elapsed time
    if (checkTimeElapsed)
    {
        ss.str("");
        ss << g_dElapsedTime << "secs";
        c.X = 0;
        c.Y = 0;
        g_Console.writeToBuffer(c, ss.str(), 0x59);
    }
}

// this is an example of how you would use the input events
void renderInputEvents()
{
    // keyboard events
    COORD startPos = {50, 2};
    std::ostringstream ss;
    std::string key;
    for (int i = 0; i < K_COUNT; ++i)
    {
        ss.str("");
        switch (i)
        {
        case K_W: key = "W";
            break;
        case K_S: key = "S";
            break;
        case K_A: key = "A";
            break;
        case K_D: key = "D";
            break;
        case K_SPACE: key = "SPACE";
            break;
        default: continue;
        }
        if (g_skKeyEvent[i].keyDown)
            ss << key << " pressed";
        else if (g_skKeyEvent[i].keyReleased)
            ss << key << " released";
        else
            ss << key << " not pressed";

        COORD c = { startPos.X, startPos.Y + i };
        g_Console.writeToBuffer(c, ss.str(), 0x17);
    }

    // mouse events    
    ss.str("");
    ss << "Mouse position (" << g_mouseEvent.mousePosition.X << ", " << g_mouseEvent.mousePosition.Y << ")";
    g_Console.writeToBuffer(g_mouseEvent.mousePosition, ss.str(), 0x59);
    ss.str("");
    switch (g_mouseEvent.eventFlags)
    {
    case 0:
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            ss.str("Left Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, ss.str(), 0x59);
        }
        else if (g_mouseEvent.buttonState == RIGHTMOST_BUTTON_PRESSED)
        {
            ss.str("Right Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, ss.str(), 0x59);
        }
        else
        {
            ss.str("Some Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 3, ss.str(), 0x59);
        }
        break;
    case DOUBLE_CLICK:
        ss.str("Double Clicked");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 4, ss.str(), 0x59);
        break;        
    case MOUSE_WHEELED:
        if (g_mouseEvent.buttonState & 0xFF000000)
            ss.str("Mouse wheeled down");
        else
            ss.str("Mouse wheeled up");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 5, ss.str(), 0x59);
        break;
    default:        
        break;
    }   
}

void renderWall()
{//maybe?
    COORD c;
    int colour;
    for (int i = 0; i < WallLimit; i++)
    {
        if (Walls[i] != nullptr)
        {
            c.X = Walls[i]->getposx();
            c.Y = Walls[i]->getposy();

            colour = 0x00;

            g_Console.writeToBuffer(c, "W", colour);
        }
    }
}

void spawnWall(int no)                                                                                  //function to spawn wall
{
    for (int i = 0; i < no; i++)                                                                        //for loop to cycle the spawning of each wall
    {
        /*if (setType() == I)
        {
            typeI();
        }
        
        else if (setType() == J)
        {
            typeJ();
        }

        else if (setType() == L)
        {
            typeL();
        }

        else if (setType() == O)
        {
            typeO();
        }

        else if (setType() == S)
        {
            typeS();
        }

        else if (setType() == T)
        {
            typeT();
        }

        else if (setType() == Z)
        {
            typeZ();
        }

        else if (setType() == d)
        {
            typed();
        }*/

        Position temp;                                                                                  //find random x and y coords of unused spaces on map
        bool isSpaceNearPlayer = false; 																//bool to check if anything wants to spawn in around player

        while ((occupied(&temp) != nullptr) && isSpaceNearPlayer == true) 					            //while pos is not available
        {
            temp.set_x(rand() % 80); 															        //set x coordinate of temp variable as a number from 0 to 80
            temp.set_y(rand() % 24); 														            //set y coordinate of temp variable as a number from 0 to 25

            if (temp.get_x() > 39 && temp.get_x() <= 41) 							                    //check if randomiser chose outside range of the 1 block diameter in x axis around player
            {
                if (temp.get_y() > 12 && temp.get_y() <= 14)					                        //check if randomiser chose outside range of the 1 block diameter in y axis around player
                {
                    isSpaceNearPlayer = true;															//if not in range, set bool to true to end loop
                }
            }
        }

        for (int w = 0; w < WallLimit; w++)                                                             // for loop to set positions on map for each wall entity
        {
            if (Walls[w] == nullptr)                                                                    //check for wall entity not assigned on map
            {
                Walls[w] = new Wall;                                                                    //set element of array as new object under wall class
                entities[w + 11] = Walls[w];                                                            //set element from wall array to corresponding element on entity array
                entities[w + 11]->set_pos(temp.get_x(), temp.get_y());                                  //set position of the temp wall entity to an element in the entity array
                break;                                                                                  //break from current loop
            }
        }
    }
}

void renderPowerUp()
{

}

void spawnPowerUp()
{
    //powerup = new PowerUp(15 / g_dDeltaTime);
}

void renderNPC()
{//can probably change this function to show all the entites rather than just NPCs. If yall want then can use the entity pointer array and type() function to differentiate the derieved classes
    
    //only separated them cos didnt see the need for player to be in entities pointer array since we using ascii but if we change it ye thatll be good
    //initially i changed Entities pointer array to NPCs bcos player was taken out and i could use NPC class functions only on NPC pointers unless virtual thingy
    COORD c;
    int colour;
    for (int i = 0; i < NPCLimit; i++)
    {
        if (NPCs[i] != nullptr)
        {
            c.X = NPCs[i]->getposx();
            c.Y = NPCs[i]->getposy();

            if (NPCs[i]->type() == 'B')
            {
                if (NPCs[i]->isHostile())
                {
                    colour = 0xC3;
                }
                else
                {
                    colour = 0x66;
                }
            }
            else
            {
                if (NPCs[i]->isHostile())
                {
                    colour = 0x4D;
                }
                else
                {
                    colour = 0xF6;
                }
            }

           /* if (NPCs[i]->get_ftime() != 0)
            {
                colour = 0x3C;
            }*/

            g_Console.writeToBuffer(c, " ", colour);

            
        }
    }
    
}

void spawnNPC(bool isPolice, int no, float spd, int cooldowntime) //spd shud be btw 0.1 and 0.9; spd of 1 = spd of player
{
    for (int i = 0; i < no; i++)
    {
        Position temp;
        bool valid = true;

        do
        {
            temp.set_x(rand() % 80);
            temp.set_y(rand() % 24);
   
            for (int i = 0; i < 9; i++)
            {
                if (temp.get_x() == spawnPoint[i].get_x() && temp.get_y() == spawnPoint[i].get_y())
                {
                    valid = false;
                }
                else if (temp.get_x() == endPoint[i].get_x() && temp.get_y() == endPoint[i].get_y())
                {
                    valid = false;
                }
            }
        } while (occupied(&temp) != nullptr && valid == false); //while pos is not available

        for (int n = 0; n < NPCLimit; n++)
        {
            if (NPCs[n] == nullptr)
            {
                if (isPolice)
                {
                    NPCs[n] = new Police(cooldowntime);
                    entities[n + 1] = NPCs[n];
                }
                else
                {
                    NPCs[n] = new NPC(cooldowntime);
                    entities[n + 1] = NPCs[n];
                }
                entities[n + 1]->set_pos(temp.get_x(), temp.get_y());
                NPCs[n]->set_speed(spd);
                NPCs[n]->set_count(2 / g_dDeltaTime);
                break;
            }
        }


    }
}

void moveall()
{
    for (int i = 0; i < NPCLimit; i++)
    {
        if (NPCs[i] != nullptr)
        {
            if (NPCs[i]->isHostile() == false)
            {
                int tempcount = ((rand() % 3) + 5) / g_dDeltaTime;

                if (NPCs[i]->get_count() < tempcount && NPCs[i]->isHostile() == false)
                {
                    NPCs[i]->set_count(NPCs[i]->get_count() + 1);

                    if (NPCs[i]->get_count() > (0.7 * tempcount))
                    {
                        NPCs[i]->set_direction(0);
                    }
                }
                else 
                {

                    NPCs[i]->set_count(0);

                    
                    int aaa = (rand() % 7) + 1;
                    switch (aaa)
                    {
                    case 1: //Up
                        NPCs[i]->set_direction(1);
                        break;
                    case 2: //Down
                        NPCs[i]->set_direction(2);
                        break;
                    case 3: //Left
                        NPCs[i]->set_direction(3);
                        break;
                    case 4: //Right
                        NPCs[i]->set_direction(4);
                        break;
                    case 5: //Don't move
                        NPCs[i]->set_direction(0);
                        break;
                    default: //(6 or 7) continue in same direction
                        break;
                    }

                    
                }

                if (occupied(NPCs[i]->new_pos(g_dDeltaTime)) != nullptr && occupied(NPCs[i]->new_pos(g_dDeltaTime)) != NPCs[i])
                {

                    NPCs[i]->set_direction(0);
                }

                
            }
            else if (NPCs[i]->get_ftime() != 0) //npc is hostile but on cooldown
            {
                NPCs[i]->set_direction(0);
                NPCs[i]->set_count(NPCs[i]->get_count() - 1);
                if (NPCs[i]->get_count() == 0)
                {
                    NPCs[i]->cooldownend();
                }
                
            }
            else //npc is hostile and not on cooldown
            {
                int diffinx = g_sChar.m_cLocation.X - static_cast<int>(NPCs[i]->getposx());
                int diffiny = g_sChar.m_cLocation.Y - static_cast<int>(NPCs[i]->getposy());

                if (abs(diffinx) > abs(diffiny))
                {
                    if (diffinx > 0)
                    {
                        NPCs[i]->set_direction(4);
                    }
                    else
                    {
                        NPCs[i]->set_direction(3);
                    }
                 
                }
                else if (abs(diffinx) == abs(diffiny))
                {
                    int a = (rand() % 2) + 1;
                    switch (a)
                    {
                    case 1:
                        if (diffinx > 0)
                        {
                            NPCs[i]->set_direction(4);
                        }
                        else
                        {
                            NPCs[i]->set_direction(3);
                        }
                        break;
                    default:

                        if (diffiny > 0)
                        {
                            NPCs[i]->set_direction(2);
                        }
                        else
                        {
                            NPCs[i]->set_direction(1);
                        }
                    }
                }
                else //up or down
                {
                    if (diffiny > 0)
                    {
                        NPCs[i]->set_direction(2);
                    }
                    else
                    {
                        NPCs[i]->set_direction(1);
                    }
                }
                if (occupied(NPCs[i]->new_pos(g_dDeltaTime)) != nullptr)
                {
                    if (occupied(NPCs[i]->new_pos(g_dDeltaTime))->type() == 'W')
                    {

                        NPCs[i]->set_direction(0);
                    }
                }
                            
            }

            if (NPCs[i]->new_pos(g_dDeltaTime)->get_x() <= spawnPoint[5].get_x() + 1 && NPCs[i]->new_pos(g_dDeltaTime)->get_y() <= spawnPoint[7].get_y() + 1 && (NPCs[i]->new_pos(g_dDeltaTime)->get_x() >= spawnPoint[3].get_x() && NPCs[i]->new_pos(g_dDeltaTime)->get_y() >= spawnPoint[1].get_y()))
            {
                NPCs[i]->set_direction(0);
            }
            if (NPCs[i]->new_pos(g_dDeltaTime)->get_x() <= endPoint[5].get_x() + 1 && NPCs[i]->new_pos(g_dDeltaTime)->get_y() <= endPoint[7].get_y() + 1 && NPCs[i]->new_pos(g_dDeltaTime)->get_x() >= endPoint[3].get_x() && NPCs[i]->new_pos(g_dDeltaTime)->get_y() >= endPoint[1].get_y())
            {
                NPCs[i]->set_direction(0);
            }

            NPCs[i]->update_pos(g_dDeltaTime);
        }
      
    }
}

Entity* occupied(Position* pos)
{
    for (int i = 0; i < entityLimit; i++)
    {
        if (entities[i] != nullptr)
        {
            if (static_cast<int>(entities[i]->getpos()->get_x()) == static_cast<int>(pos->get_x()) && static_cast<int>(entities[i]->getpos()->get_y()) == static_cast<int>(pos->get_y()))
            {
                return entities[i];
            }
        }
    }
    return nullptr;
}

void renderprojectile()
{
    COORD pr;
    int colour;
    for (int p = 0; p < particle_limit; p++)
    {
        if (projectile[p] != nullptr)
        {

            pr.X = projectile[p]->get_px();
            pr.Y = projectile[p]->get_py();

            colour = 0xA1;

            g_Console.writeToBuffer(pr, " ", colour);
        }
    }
}

void renderBox(Object* box, int colour, std::string text = " ")
{
    COORD c;
    char temp;
    int i = 0;// for displaying text
    c.Y = box->referencePosition()->get_y();
    for (int y = 0; y < box->height(); y++)
    {
        c.X = box->referencePosition()->get_x();
        for (int x = 0; x < box->length(); x++)
        {
            c.X++;
            g_Console.writeToBuffer(c, temp = (x >= ((box->length() + 1) / 2) - 1 - text.length() / 2 && x < ((box->length() + 1) / 2) + text.length() / 2  && y == box->height() / 2) ? text[i++] : ' ', colour);
        }
        c.Y++;
    }
}

void renderMainMenu()
{
    COORD c = g_Console.getConsoleSize();
    Object title(71, 3, Position(c.X / 2, c.Y / 5));
    renderBox(&title, 0x0F, gameName);

    MMButton.move(c.X / 2, c.Y * 2 / 5);
    MMButton2.move(c.X / 2, c.Y * 3 / 5);
    MMButton3.move(c.X / 2, c.Y * 4 / 5);
    MMButton4.move(c.X / 2, c.Y);

    MMButtons[0] = &MMButton;
    MMButtons[1] = &MMButton2;
    MMButtons[2] = &MMButton3;
    MMButtons[3] = &MMButton4;

    renderBox(&MMButton, 0x04, gameMode1);
    renderBox(&MMButton2, 0x0A, gameMode2);
    renderBox(&MMButton3, 0x0B, gameMode3);
    renderBox(&MMButton4, 0x06, gameMode4);
}

void mainMenuWait()
{
    switch (checkButtonClicks(MMButtons, MMButtonCount))
    {
    case 0:
        g_eGameState = S_GAMEMODE1;
        break;
    case 1:
        g_eGameState = S_GAMEMODE2;
        break;
    case 2:
        g_eGameState = S_GAMEMODE3;
        break;
    case 3:
        g_eGameState = S_TEST;
        break;
    default:
        break;
    }
}

void renderPauseMenu()
{
    COORD c = g_Console.getConsoleSize();
    Object title(71, 3, Position(c.X / 2, c.Y / 6));
    renderBox(&title, 0x0F, "Paused");

    resumeButton.move(c.X / 2, c.Y * 2 / 4);
    quitButton.move(c.X / 2, c.Y * 3 / 4);

    PMButtons[0] = &resumeButton;
    PMButtons[1] = &quitButton;

    renderBox(&resumeButton, 0x0A, resume);
    renderBox(&quitButton, 0x04, quit);
}

void pauseMenuWait()
{
    switch (checkButtonClicks(PMButtons, PMButtonCount))
    {
    case 0:
        paused = false;
        break;
    case 1:
        g_bQuitGame = true;
        break;
    default:
        break;
    }
}

//set true for win screen, false for lose screen
void renderWinLoseMenu(bool win)
{
    COORD c = g_Console.getConsoleSize();
    std::string* message = win ? &winMessage : &loseMessage;
    Object title(71, 3, Position(c.X / 2, c.Y / 3));
    renderBox(&title, 0x0F, *message);

    continueButton.move(c.X * 3 / 4, c.Y * 2 / 3);
    restartButton.move(c.X * 3 / 4, c.Y * 2 / 3);
    mainMenuButton.move(c.X * 2 / 4, c.Y * 2 / 3);
    quitButton.move(c.X / 4, c.Y * 2 / 3);

    WLButtons[0] = &quitButton;
    WLButtons[1] = &mainMenuButton;
    WLButtons[2] = win ? &continueButton : &restartButton;

    renderBox(WLButtons[0], 0x04, quit);
    renderBox(WLButtons[1], 0x0A, mainMenuMessage);
    renderBox(WLButtons[2], 0x0F, win ? continueMessage : restartMessage);
    
}

void winLoseMenuWait()
{
    switch (checkButtonClicks(WLButtons, WLButtonCount))
    {
    case 0:
        g_bQuitGame = true;
        break;
    case 1:
        g_eGameState = S_MAINMENU;
        break;
    case 2:
        //do win lose stuff;
        break;
    default:
        break;
    }
}

int checkButtonClicks(Object** buttons, int arrayLength)
{
    int mouseX, mouseY;
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {// check when player left click 
        mouseX = g_mouseEvent.mousePosition.X;
        mouseY = g_mouseEvent.mousePosition.Y;
        for (int i = 0; i < arrayLength; i++)
        {//check all the objects in the given array
            if (mouseX >= buttons[i]->referencePosition()->get_x() &&
                mouseX <= buttons[i]->referencePosition()->get_x() + buttons[i]->length() &&
                mouseY >= buttons[i]->referencePosition()->get_y() &&
                mouseY <= buttons[i]->referencePosition()->get_y() + buttons[i]->height())
            {// check if mouse is within this Object
                return i;
            }
        }
    }
    return arrayLength;
}

void limitprojectile()
{
    for (int p = 0; p < particle_limit; p++)
    {
        if (projectile[p] != nullptr)
        {
            if ((projectile[p]->get_pcooldown() != 0))
            {
                projectile[p]->set_pcooldown(projectile[p]->get_pcooldown() - 1);
                if (projectile[p]->get_spacecount() != 0)
                {
                    projectile[p]->update_particle(g_dDeltaTime);
                    projectile[p]->set_spacecount(projectile[p]->get_spacecount() - 1);
                }
            }
            else
            {   
                delete projectile[p];
                projectile[p] = nullptr;
            }
        }
    }
}

void check_collision()
{
    for (int i = 0; i < NPCLimit; i++)
    {
        if (NPCs[i] != nullptr)
        {
            if (NPCs[i]->isHostile() && occupied(NPCs[i]->getpos())->type() == 'P' && NPCs[i]->get_ftime() == 0)
            {
                //timer += g_dDeltaTime;
                //timer >3, run else dont;
                NPCs[i]->cooldownstart();
                NPCs[i]->set_count(NPCs[i]->get_ftime() / g_dDeltaTime);
                player->loseHP(NPCs[i]->get_damage());
                player->set_pos(spawnPoint[4].get_x(), spawnPoint[4].get_y());
                g_sChar.m_cLocation.Y = player->getposy(); 
                g_sChar.m_cLocation.X = player->getposx(); 
                //timer = 0;
            }
 
        }
    }
}

void renderPoints()
{
    COORD c;
    int colour;

    for (int i = 0; i < 9; i++)
    {
        if (i == 1)
        {
            colour = 0x30;
            c.X = spawnPoint[i].get_x();
            c.Y = spawnPoint[i].get_y();
            g_Console.writeToBuffer(c, "S", colour);

            colour = 0x90;
            c.X = endPoint[i].get_x();
            c.Y = endPoint[i].get_y();
            g_Console.writeToBuffer(c, "E", colour);
        }
        else if (i != 4)
        {
            colour = 0x33;
            c.X = spawnPoint[i].get_x();
            c.Y = spawnPoint[i].get_y();
            g_Console.writeToBuffer(c, " ", colour);

            colour = 0x99;
            c.X = endPoint[i].get_x();
            c.Y = endPoint[i].get_y();
            g_Console.writeToBuffer(c, " ", colour);
        }
    }
    
    colour = 0x7F;
    c.X = endPoint[4].get_x();
    c.Y = endPoint[4].get_y();
    g_Console.writeToBuffer(c, (char)254 , colour);

    colour = 0x7F;
    c.X = spawnPoint[4].get_x();
    c.Y = spawnPoint[4].get_y();
    g_Console.writeToBuffer(c, (char)254 , colour);

}

void setBG()
{
    int colour;
    do
    {
        int a = rand() % 3;
        switch (a)
        {
        case 0:
            colour = 0x88;
            break;
        case 1:
            colour = 0x55;
            break;
        case 2:
            colour = 0xDD;
            break;
            

        }
    } while (prevcol == colour);

    prevcol = colour;
}

void renderBG(int col)
{
    COORD c;
    for (int x = 0; x < 80; x++)
    {
        for (int y = 0; y < 25; y++)
        {
            c.X = x;
            c.Y = y;
            g_Console.writeToBuffer(c, " ", col);
        }
    }
}