// This is the main file for the game logic and function
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <irrKlang.h>
#include "game.h"
#include "Framework\console.h"
#include "time.h"
#include "Projectile.h"
#include "Wall.h"
#include "PowerUp.h"
#include "CCTV.h"

//FOR TESTING
bool checkInputs = false;
bool checkTimeElapsed = false;
bool checkFramerate = true;
float splashScreenTime = 0.5;

//UI NAMES
std::string gameName = "A Very Fun Game";
std::string gameMode1 = "Normal";
std::string gameMode2 = "Endless";
std::string gameMode3 = "Tutorial (under construction)";
std::string gameMode4 = "Click This"; // for game test. not for final product
std::string winMessage = "HACKS REPORTED";
std::string loseMessage = "GGEZ Uninstall";
std::string continueMessage = "Next Level";
std::string restartMessage = "Restart";
std::string mainMenuMessage = "Main Menu";
std::string quit = "Quit";
std::string resume = "Resume";
std::string back = "Back";
std::string gamemodes = "Play";

std::string objective = "";

//MAINMENU
Object title(71, 3);
Object MMButton(gameMode1.length() + 2, 3);
Object MMButton2(gameMode2.length() + 2, 3);
Object MMButton3(gameMode3.length() + 2, 3);
Object MMButton4(gameMode4.length() + 2, 3);
Object backButton(back.length() + 2, 3);
Object gamemodeButton(gamemodes.length() + 2, 3);
Object* MMButtons[2];
const int MMButtonCount = 2; 
Object* MGButtons[5];
const int MGButtonCount = 5;
std::string stage = "SELECT";

//QUICK FIX
bool isMousePressed = false;

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
Object mainMenuButton(mainMenuMessage.length() + 2, 3);
const int WLButtonCount = 3;

//HUD
Object healthBar(1, 1);
Object coughBar(1, 1);
Object NPCremaining(1, 1);
int currentHP;
int cooldownLength;
bool showHUD = true;

//NORMAL MODE
NormalMode NGameState = N_INIT;
EndlessMode EGameState = E_INIT;
Tutorial TutState = TUT_GAMEPLAY;
Test TGameState = T_INIT;
int level = 0; //level no.
bool lose = false; //end game
bool clear = false;
int noC; //no. of civilian
int noP; //no. of Police
float spd; //spd of NPCs relative to player
int cdtime; //cooldown time of hostile NPCs after collision w player
int noW; //no of walls
Position endPoint[9];
Position spawnPoint[9];
Position safezone[9];
int highestLVL;
int totalhostile = 0;


//for temp use in code
int tempcounter;
int flashcount = 0;

//TUTORIAL
std::string text = " ";
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
//MainMenu g_mainMenu = M_MAIN;

Player* player = new Player;

Entity* entities[66] = { player, 
                        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, //10 per row //NPCs
                        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, //NPCs
                        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, //Walls
                        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, //Walls
                        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, //Walls
                        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, //Walls
                        nullptr, nullptr, nullptr, nullptr, nullptr};                                             //6 rows //CCTVs
const int entityLimit = 66;

NPC* NPCs[20] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
const int NPCLimit = 20;

Wall* Walls[40] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
const int WallLimit = 10;

Projectile* projectile[3] = { nullptr, nullptr, nullptr };
const int particle_limit = 3;

PowerUp* powerup = nullptr;

CCTV* CCTVs[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
const int CCTVLimit = 5;

// Console object
Console g_Console(80, 25, "SP1 Framework");
COORD consoleSize = g_Console.getConsoleSize();

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
void shutdown(void)
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();

    deleteEntities();
    
    if (powerup != nullptr)
    {
        delete powerup;
    }
    
    //delete player;
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
    //switch (g_eGameState)
    //{
    //case S_MAINMENU: gameplayKBHandler(keyboardEvent); // handle thing for the splash screen
    //    break;
    //case S_TEST: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
    //    break;
    //case S_GAMEMODE1: gameplayKBHandler(keyboardEvent); 
    //    break;
    //case S_GAMEMODE2: gameplayKBHandler(keyboardEvent);
    //    break;
    //}
    gameplayKBHandler(keyboardEvent);
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
    //switch (g_eGameState)
    //{
    //case S_MAINMENU: gameplayMouseHandler(mouseEvent); // don't handle anything for the splash screen
    //    break;
    //case S_TEST: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
    //    break;
    //case S_GAMEMODE1: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
    //    break;
    //case S_GAMEMODE2: gameplayMouseHandler(mouseEvent);
    //    break;
    //case S_TUTORIAL: gameplayMouseHandler(mouseEvent);
    //    break;
    //}

    gameplayMouseHandler(mouseEvent);
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
        case S_TEST: testStates(); // gameplay logic when we are in the game
            break;
        case S_GAMEMODE1: playNormal();
            break;
        case S_GAMEMODE2:
            playEndless();
            break;
        case S_TUTORIAL: playTutorial();
            break;
    }
}

void playTutorial()
{
    switch (TutState)
    {
    case TUT_GAMEPLAY:
        initTutGP();
        playTutGP();
        break;
    case TUT_POLICE:
        break;
    case TUT_POWERUP:
        break;
    }
}

void initTutGP()
{

}

void playTutGP()
{

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
    {
        moveCharacter();    // moves the character, collision detection, physics, etc
        checkAll();
        showHUD = true;                   // sound can be played here too.
    }
    else
    {
        pauseMenuWait();
        showHUD = false;
    }                
    
}

void testStates()
{
    switch (TGameState)
    {
    case T_INIT:
        initTest();
        break;
    case T_PLAY:
        playTest();
        break;
    case T_END:
        endTest();
        break;
    }
}
//Initialise variables and objects for test area here
void initTest()
{
    initHUD();
    TGameState = T_PLAY;
}
//The logic for test area
void playTest()
{
    updateGame();
}
//Extra state to test "end" game scenarios
void endTest()
{

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
        level_set();
    case N_LOSE:
        winLoseMenuWait();
        break;
    }
}

void InitNormal()
{
    lose = false;
    level = 0;
    level_set();
    initHUD();
    
    
    //spawnWall(noW);
    
    NGameState = N_LEVEL;
}
 
void set_spawn() //set stats based on level;; spawn NPCs, set spawn and end points
{
    if (level == 1)
    {
        noC = 3;
        noP = 0;
        spd = 0.2;
        cdtime = 3;
        noW = 10;
    }
    else if (level < 6)
    {
        noC++;
        spd += 0.05;
        cdtime = 2.5;
    }
    else if (level < 15)
    {
        spd += 0.05;
        cdtime = 2;
        if (level % 2)
        {
            noC++;
            noP = level - 5;
        }
    }
    else
    {
        spd = 0.9;
        cdtime = 1;
        noP = 5;
        noC = 15;
    }

    spawnWall(noW);
    set_points();
    spawnNPC(false, noC, spd, cdtime);
    spawnNPC(true, noP, spd, cdtime);
    spawnCCTV(2);
    
}

void set_points()
{
    spawnPoint[4].set_pos(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y);

    Position tempp;
    do
    {
        tempp.set_x((rand() % 78) + 1);
        tempp.set_y((rand() % 21) + 2);

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

void resetSpawns()
{
    for (int i = 0; i < NPCLimit; i++)
    {
        if (NPCs[i] != nullptr)
        {
            delete NPCs[i];
            NPCs[i] = nullptr;
        }
    }

    for (int w = 0; w < 40; w++)
    {
        if (Walls[w] != nullptr)
        {
            delete Walls[w];
            Walls[w] = nullptr;
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

    for (int c = 0; c < CCTVLimit; c++)
    {
        if (CCTVs[c] != nullptr)
        {
            delete CCTVs[c];
            CCTVs[c] = nullptr;
        }
    }

    if (powerup != nullptr)
    {
        delete powerup;
        powerup = nullptr;
    }
}

void level_set() //deletes everyth
{
    
    resetSpawns();
   
    player->resetHP();
    NPC::resetnoHostile();
    level++;

    set_spawn();
    clear = false;
    NGameState = N_LEVEL;
    
    
}

void playLevel()
{
    spawnPowerUp();
    deletePowerUp();
    updateGame();
    renderHUD();

    if (player->get_lethalstatus() == 0)
        player->update_ld();

    if (NPC::getnoHostile() == noC + noP && static_cast<int>(player->getposx()) == endPoint[4].get_x() && static_cast<int>(player->getposy()) == endPoint[4].get_y())
    {
        clear = true;
        NGameState = N_NEXTLEVEL;
    }

    if (player->get_HP() <= 0)
    {
        lose = true;
    }

    if (lose)
    {
        highestLVL = level;
        resetSpawns();
        NGameState = N_LOSE;

    }
}
    
void playEndless()
{
    switch (EGameState)
    {
    case E_INIT:
        InitEndless();
        break;
    case E_PLAY:
        enterEndless();
        break;
    case E_LOSE:
        winLoseMenuWait();
    }
}

void InitEndless()
{
    lose = false;
    totalhostile = 0;
    player->resetHP();
    NPC::resetnoHostile();
    tempcounter = 0;
    
    setsafezone();

    spawnWall(10);
    spawnNPC(false, 5, 0.6, 1);
    //spawnNPC(true, 1, 0.5, 1);

    initHUD();

    EGameState = E_PLAY;
}

void enterEndless()
{
    tempcounter++;
    if (tempcounter > (3 / g_dDeltaTime) && NPC::gettotal() != 20)
    {
        spawnNPC(false, 1, 0.6, 1);
        tempcounter = 0;
    }

    updateGame();

    if (player->get_HP() == 0)
    {
        lose = true;
        
    }

    for (int i = 0; i < NPCLimit; i++)
    {
        if (NPCs[i] != nullptr)
        {
            COORD npcpos;
            npcpos.X = NPCs[i]->getposx() - static_cast<int>(player->getposx()) + 40;
            npcpos.Y = NPCs[i]->getposy() - static_cast<int>(player->getposy()) + 12;
            if (checkifinscreen(npcpos) == false)
            {
                delete NPCs[i];
                NPCs[i] = nullptr;
            }
        }
    }
    if (lose)
    {
        totalhostile = NPC::getnoHostile();
        resetSpawns();
        EGameState = E_LOSE;
        
    }
}

void setsafezone()
{
    safezone[4].set_pos(40, 12);

    for (int i = -1; i < 2; i++)
    {
        safezone[1 + i].set_pos(safezone[4].get_x() + i, safezone[4].get_y() - 1);
        safezone[7 + i].set_pos(safezone[4].get_x() + i, safezone[4].get_y() + 1);

        safezone[3].set_pos(safezone[4].get_x() - 1, safezone[4].get_y());
        safezone[5].set_pos(safezone[4].get_x() + 1, safezone[4].get_y());
    }
}

void rendersafezone()
{
    COORD c;
    int colour;

    for (int i = 0; i < 9; i++)
    {
       
        if (i != 4)
        {

            colour = 0xBB;
            c.X = safezone[i].get_x() - static_cast<int>(player->getposx()) + 40;
            c.Y = safezone[i].get_y() - static_cast<int>(player->getposy()) + 12;
            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, " ", colour);
            }

        }
    }

    colour = 0x7F;
    c.X = safezone[4].get_x() - static_cast<int>(player->getposx()) + 40;
    c.Y = safezone[4].get_y() - static_cast<int>(player->getposy()) + 12;
    if (checkifinscreen(c))
    {
        g_Console.writeToBuffer(c, (char)254, colour);
    }

}

void moveCharacter()
{   
    // Updating the location of the character based on the key release
    // providing a beep sound whenver we shift the character
    if (getButtonHold() == K_W && g_sChar.m_cLocation.Y > 1)
    {
        //Beep(1440, 30);
        player->set_direction(1);
        
    }
    else if (getButtonHold() == K_A && g_sChar.m_cLocation.X > 0)
    {
        //Beep(1440, 30);
        player->set_direction(3);
    }
    else if (getButtonHold() == K_S && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);     
        player->set_direction(2);
    }
    else if (getButtonHold() == K_D && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
        //Beep(1440, 30);    
        player->set_direction(4);
    }
    else
    {
        player->set_direction(0);
    }


    //conditions such that player cannot move:: got wall etc
    if (occupied(entities[0]->new_pos(g_dDeltaTime)) != nullptr && occupied(entities[0]->new_pos(g_dDeltaTime)) != entities[0])
    {
        player->set_direction(0);
    }
    if (!insafezone(player->getpos()) && g_eGameState == S_GAMEMODE2 && insafezone(player->new_pos(g_dDeltaTime)))
    {
        player->set_direction(0);
    }
    
    //necessary/related updates
    
    player->update_pos(g_dDeltaTime); //sets pos of player
    g_sChar.m_cLocation.Y = player->getposy(); //moves player
    g_sChar.m_cLocation.X = player->getposx(); //moves player
    
    moveall(); //moves NPCs
    

    
}

void checkAll()
{
    //cough
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

        //for turning Police hostile
        for (int n = 0; n < NPCLimit; n++)
        {
            if (NPCs[n] != nullptr)
            {
                if (NPCs[n]->type() == 'B' && NPCs[n]->isHostile() == false)
                {
                    NPCs[n]->anger();
                }
            }
        }

        //checking if player is within cctv radar when coughing
        for (int c = 0; c < CCTVLimit; c++)
        {
            if (CCTVs[c] != nullptr)
            {
                for (int r = 0; r < 20; r++)
                {
                    if (occupied(CCTVs[c]->getRadarPos(r)) != nullptr)
                    {
                        if (occupied(CCTVs[c]->getRadarPos(r))->type() == 'P')
                        {
                            lose = true;
                            break;
                        }

                    }
                }
            }
        }
    }

    check_collision(); //checks for HostileNPC-Player Collision
    limitprojectile(); //moves/updates projectiles

    //rotates CCTVs radar every second
    for (int c = 0; c < CCTVLimit; c++)
    {
        if (CCTVs[c] != nullptr)
        {
            if (CCTVs[c]->getCD() == 0)
            {
                CCTVs[c]->update_cctv();
                CCTVs[c]->setCD(1 / g_dDeltaTime);
            }
            else
            {
                CCTVs[c]->setCD(CCTVs[c]->getCD() - 1);
            }
        }
    }

    //checks if cough projectile is on the same block as an NPC, and turns them hostile if so
    for (int p = 0; p < particle_limit; p++)
    {
        if ((projectile[p] != nullptr) && (occupied(projectile[p]->getpos()) != nullptr))
        {
            if (occupied(projectile[p]->getpos())->type() == 'C')
            {
                for (int i = 0; i < NPCLimit; i++)
                {
                    if (NPCs[i] == occupied(projectile[p]->getpos()) && player->get_lethalstatus() == 1)
                    {
                        delete NPCs[i];
                        NPCs[i] = nullptr;
                        noC--;
                    }

                    else if (NPCs[i] == occupied(projectile[p]->getpos()) && NPCs[i]->isHostile() == false)
                    {
                        NPCs[i]->anger();
                        NPCs[i]->cooldownstart();
                        NPCs[i]->set_count(NPCs[i]->get_ftime() / g_dDeltaTime);

                    }
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
    case S_MAINMENU: 
        //renderBG(0x00);
        renderMainMenu();
        break;
    case S_TEST: 
        renderGame();
        break;
    case S_GAMEMODE1: 
        renderBG(prevcol);
        renderPoints();
        renderGame();
        if (lose)
        {
            renderWinLoseMenu(false);
        }
        break;
    case S_GAMEMODE2:
        renderBG(0x88); //dk what colour for now
        rendersafezone();
        renderGame();
        if (lose)
        {
            renderWinLoseMenu(false);
        }
        break;
    case S_TUTORIAL:
        renderBG(prevcol);
        renderGame();
        renderText();
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
    setallrpos();
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    if (paused)
        renderPauseMenu();
    else
        renderHUD();
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

    
    //renderPoints();
    renderCCTV();
    renderNPC();
    renderprojectile();
    renderWall();
    
    renderPowerUp();
}

void renderCharacter()
{
    // Draw the location of the character

    if (player->get_flash())
    {
        if (flashcount % 10)
        {
            g_sChar.m_bActive = false;
        }
        else
        {
            g_sChar.m_bActive = true;
        }
        flashcount--;
        if (flashcount == 0)
        {
            player->set_flash(false);
        }
    }
    else
    {
        g_sChar.m_bActive = true;
    }

    WORD charColor = 0x44;
    if (g_sChar.m_bActive)
    {
        charColor = 0x09;
    }
    COORD c;
    c.X = player->getrposx();
    c.Y = player->getrposy();

    
    //g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, charColor);
    g_Console.writeToBuffer(c, (char)1, charColor);
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
    for (int i = 0; i < WallLimit*4; i++)
    {
        if (Walls[i] != nullptr)
        {
            c.X = Walls[i]->getposx() - static_cast<int>(player->getposx()) + 40;
            c.Y = Walls[i]->getposy() - static_cast<int>(player->getposy()) + 12;

            colour = 0x00;

            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, "W", colour);
            }
        }
    }
}

void spawnWall(int no)                                                                                                              //function to spawn wall
{
    for (int j = 0; j < no; j++)                                                                                                    //for loop to cycle the spawning of each wall
    {   //find random x and y on unused spaces
        bool isSpaceNearPlayer;                                                                                             //used as a second conditon in while loop to ensure no space chosen intersects with the spawn zone
        bool isSpaceOccupied;

        for (int w = 0; w < WallLimit; w++)                                                                                         // for loop to set positions on map for each wall entity
        {
            if (Walls[w] == nullptr)                                                                                                //checks if wall entity is unassigned on map
            {
                Walls[w] = new Wall;
                entities[w + 20] = Walls[w];

                do
                {
                    isSpaceNearPlayer = false;                                                                                             //used as a second conditon in while loop to ensure no space chosen intersects with the spawn zone
                    isSpaceOccupied = false;

                    int Pivotx = (rand() % 76) + 4;                                                                                     //set x coordinate of variable, wallPos[0], as a number from 0 to 80
                    int Pivoty = (rand() % 23);                                                                                     //set y coordinate of variable, wallPos[0], as a number from 0 to 24
                    Walls[w]->setPos(Pivotx, Pivoty);
                    
                    Walls[w]->setPosForAll();
                    for (int i = 0; i < 4; i++)
                    {
                        if (Walls[w]->getPos(i)->get_x() >= 39 && Walls[w]->getPos(i)->get_x() <= 41)                                //check for if random x coordinate is not within 1 block of spawn zone on x axis 
                        {
                            if (Walls[w]->getPos(i)->get_y() >= 12 && Walls[w]->getPos(i)->get_y() <= 14)                            //check for if random y coordinate is not within 1 block of spawn zone on x axis
                            {
                                isSpaceNearPlayer = true;
                                break;//if chosen coords are not near spawn zone, second condition is true for loop to stop
                            }
                        }

                        if (occupied(Walls[w]->getPos(i)) != nullptr && occupied(Walls[w]->getPos(i)) != Walls[w])
                        {
                            isSpaceOccupied = true;
                            break;
                        }
                    }
                } while (isSpaceOccupied == true || isSpaceNearPlayer == true);                                                      //while position on map is unavailable

                for (int i = 1; i < 4; i++)
                {
                    Walls[w + WallLimit * i] = new Wall;                                                                              //set element of array as new object under wall class
                    entities[w + 20 + WallLimit * i] = Walls[w + WallLimit * i];                                                        //set element from wall array to corresponding element on entity array
                    entities[w + 20 + WallLimit * i]->set_pos(Walls[w]->getPos(i)->get_x(), Walls[w]->getPos(i)->get_y());            //set position of the temp wall entity to an element in the entity array
                }
                break;
            }
        }
    }
}

void renderPowerUp()
{
    COORD pu;
    int colour;

    if (powerup != nullptr)
    {

            pu.X = static_cast<int>(powerup->get_xcoord()) - static_cast<int>(player->getposx()) + 40;
            pu.Y = static_cast<int>(powerup->get_ycoord()) - static_cast<int>(player->getposy()) + 12;

            colour = 0xB6;
            if (checkifinscreen(pu))
            {
                g_Console.writeToBuffer(pu, (char)232, colour);
            }
    }
    
}

void spawnPowerUp()
{
    if (powerup == nullptr)
    {
        int r = rand() % 1000;
        int a = rand() % 1000;

        if (r == a)
        {
            powerup = new PowerUp;

            do
            {
                powerup->set_xcoord(rand() % 80);
                powerup->set_ycoord((rand() % 24) + 1); 
            } while (occupied(powerup->get_pos()) != nullptr);

            powerup->set_detime(2250);
        }
    }
}

void deletePowerUp()
{
    if (powerup != nullptr)
    {
        if ((g_sChar.m_cLocation.X == powerup->get_xcoord()) && (g_sChar.m_cLocation.Y == powerup->get_ycoord()))
        {
            player->set_lethal();
            delete powerup;
            powerup = nullptr;
        }

        else if (powerup->get_detime() != 0)
            powerup->set_detime(powerup->get_detime() - 1);

        else
        {
            delete powerup;
            powerup = nullptr;
        }
    }

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
            c.X = NPCs[i]->getrposx();
            c.Y = NPCs[i]->getrposy();

            if (NPCs[i]->type() == 'B')
            {
                if (NPCs[i]->isHostile())
                {
                    colour = 0xC3;
                }
                else
                {
                    colour = 0x11;
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
            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, " ", colour);
            }

            
        }
    }
    
}

void spawnNPC(bool isPolice, int no, float spd, float cooldowntime) //spd shud be btw 0.1 and 0.9; spd of 1 = spd of player
{
    for (int i = 0; i < no; i++)
    {
        Position temp;
        bool valid; 

        do
        {
            valid = true;

            temp.set_x(rand() % 80);
            temp.set_y((rand() % 23) + 1);
          
            if (g_eGameState == S_GAMEMODE2)
            {
                COORD t;
                t.X = temp.get_x() - player->getposx() + 40;
                t.Y = temp.get_y() - player->getposy() + 12;
                if (checkifinscreen(t) == false)
                {
                    valid = false;
                }
            }
            
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
        } while (occupied(&temp) != nullptr || valid == false); //while pos is not available

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
            else if (NPCs[i]->isonCD()) //npc is hostile but on cooldown
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
                    if (occupied(NPCs[i]->new_pos(g_dDeltaTime))->type() == 'W' || occupied(NPCs[i]->new_pos(g_dDeltaTime))->type() == 'R')
                    {

                        NPCs[i]->set_direction(0);
                    }
                }
                            
            }

            switch (g_eGameState)
            {
            case S_GAMEMODE1:
                if (NPCs[i]->new_pos(g_dDeltaTime)->get_x() <= spawnPoint[5].get_x() + 1 && NPCs[i]->new_pos(g_dDeltaTime)->get_y() <= spawnPoint[7].get_y() + 1 && (NPCs[i]->new_pos(g_dDeltaTime)->get_x() >= spawnPoint[3].get_x() && NPCs[i]->new_pos(g_dDeltaTime)->get_y() >= spawnPoint[1].get_y()))
                {
                    NPCs[i]->set_direction(0);
                }
                if (NPCs[i]->new_pos(g_dDeltaTime)->get_x() <= endPoint[5].get_x() + 1 && NPCs[i]->new_pos(g_dDeltaTime)->get_y() <= endPoint[7].get_y() + 1 && NPCs[i]->new_pos(g_dDeltaTime)->get_x() >= endPoint[3].get_x() && NPCs[i]->new_pos(g_dDeltaTime)->get_y() >= endPoint[1].get_y())
                {
                    NPCs[i]->set_direction(0);
                }
                break;
            case S_GAMEMODE2:
                if (insafezone(NPCs[i]->new_pos(g_dDeltaTime)))
                {
                    NPCs[i]->set_direction(0);
                }
                break;
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

            pr.X = static_cast<int>(projectile[p]->get_px()) - static_cast<int>(player->getposx()) + 40;
            pr.Y = static_cast<int>(projectile[p]->get_py()) - static_cast<int>(player->getposy()) + 12;

            if (player->get_lethalstatus() == 1)
                colour = 0x55;
            else
                colour = 0xA1;

            if (checkifinscreen(pr))
            {
                g_Console.writeToBuffer(pr, " ", colour);
            }
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
    if (stage == "MAIN")
    {
        title.move(consoleSize.X / 2, consoleSize.Y / 4);
        gamemodeButton.move(consoleSize.X / 2, consoleSize.Y * 2 / 4);
        quitButton.move(consoleSize.X / 2, consoleSize.Y * 3 / 4);

        MMButtons[0] = &gamemodeButton;
        MMButtons[1] = &quitButton;

        renderBox(&gamemodeButton, 0x78, gamemodes);
        renderBox(&quitButton, 0x78, quit);
    }
    if (stage == "SELECT")
    {
        title.move(consoleSize.X / 2, consoleSize.Y / 7);
        MMButton.move(consoleSize.X / 2, consoleSize.Y * 2 / 7);
        MMButton2.move(consoleSize.X / 2, consoleSize.Y * 3 / 7);
        MMButton3.move(consoleSize.X / 2, consoleSize.Y * 4 / 7);
        MMButton4.move(consoleSize.X / 2, consoleSize.Y * 5 / 7);
        backButton.move(consoleSize.X / 2, consoleSize.Y * 6 / 7);

        MGButtons[0] = &MMButton;
        MGButtons[1] = &MMButton2;
        MGButtons[2] = &MMButton3;
        MGButtons[3] = &MMButton4;
        MGButtons[4] = &backButton;

        renderBox(&MMButton, 0x78, gameMode1);
        renderBox(&MMButton2, 0x78, gameMode2);
        renderBox(&MMButton3, 0x78, gameMode3);
        renderBox(&MMButton4, 0x78, gameMode4);
        renderBox(&backButton, 0x78, back);
    }
    renderBox(&title, 0x0F, gameName);
}

void mainMenuWait()
{
    if (stage == "MAIN")
    {
        switch (checkButtonClicks(MMButtons, MMButtonCount))
        {
        case 0:
            stage = "SELECT";
            break;
        case 1:
            g_bQuitGame = true;
            break;
        default:
            break;
        }
    }
    if (stage == "SELECT")
    {
        switch (checkButtonClicks(MGButtons, MGButtonCount))
        {
        case 0:
            g_eGameState = S_GAMEMODE1;
            break;
        case 1:
            g_eGameState = S_GAMEMODE2;
            break;
        case 2:
            g_eGameState = S_TUTORIAL;
            break;
        case 3:
            g_eGameState = S_TEST;
            break;
        case 4:
            stage = "MAIN";
            break;
        default:
            break;
        }
    }
    
}

void renderPauseMenu()
{
    Object title(71, 3, Position(consoleSize.X / 2, consoleSize.Y / 6));
    renderBox(&title, 0x0F, "Paused");

    resumeButton.move(consoleSize.X / 2, consoleSize.Y * 2 / 4);
    mainMenuButton.move(consoleSize.X / 2, consoleSize.Y * 3 / 4);

    PMButtons[0] = &resumeButton;
    PMButtons[1] = &mainMenuButton;

    renderBox(&resumeButton, 0x0A, resume);
    renderBox(&mainMenuButton, 0x04, mainMenuMessage);
}

void pauseMenuWait()
{
    switch (checkButtonClicks(PMButtons, PMButtonCount))
    {
    case 0:
        paused = false;
        break;
    case 1:
        g_eGameState = S_MAINMENU;
        break;
    default:
        break;
    }
}

//set true for win screen, false for lose screen
void renderWinLoseMenu(bool win)
{
    std::string* message = win ? &winMessage : &loseMessage;
    Object title(71, 3, Position(consoleSize.X / 2, consoleSize.Y / 3));
    renderBox(&title, 0x0F, *message);

    continueButton.move(consoleSize.X * 3 / 4, consoleSize.Y * 2 / 3);
    restartButton.move(consoleSize.X * 3 / 4, consoleSize.Y * 2 / 3);
    mainMenuButton.move(consoleSize.X * 2 / 4, consoleSize.Y * 2 / 3);
    quitButton.move(consoleSize.X / 4, consoleSize.Y * 2 / 3);

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
        NGameState = N_INIT;
        EGameState = E_INIT;
        break;
    case 2:
        NGameState = N_INIT;
        EGameState = E_INIT;
        break;
    default:
        break;
    }
}

void initHUD()
{
    currentHP = player->get_maxHP();
    cooldownLength = 0;
    healthBar.resize(20, 1);
    healthBar.move((healthBar.length() - 1) / 2 + 1, 2);
    healthBar.setPivot(healthBar.referencePosition()->get_x(), healthBar.referencePosition()->get_y());
    coughBar.resize(30, 1);
    coughBar.move(consoleSize.X / 2, consoleSize.Y * 9 / 10);
    coughBar.setPivot(coughBar.referencePosition()->get_x(), coughBar.referencePosition()->get_y());
}

void renderHUD()
{
    Object HealthText(22, 1, Position(19 / 2 + 1, 1));
    Object HealthBorder(22, 1, Position(19 / 2 + 1, 2));
    Object Objective(30, 1 ,Position(consoleSize.X / 2, consoleSize.Y * 9 / 10 + 1));
    if (player->get_HP() != currentHP)
    {
        currentHP = player->get_HP();
        healthBar.resize(20, 1);
        healthBar.move((healthBar.length() - 1) / 2 + 1, 2);
        healthBar.setPivot(healthBar.referencePosition()->get_x(), healthBar.referencePosition()->get_y());
        healthBar.scale((float)currentHP / player->get_maxHP (), 1);
    }
    int counter = 0;
    for (int i = 0; i < particle_limit; i++)
        counter += projectile[i] != nullptr ? 1 : 0;
    coughBar.resize(30, 1);
    coughBar.move(consoleSize.X / 2, consoleSize.Y * 9 / 10);
    coughBar.setPivot(coughBar.referencePosition()->get_x(), coughBar.referencePosition()->get_y());
    coughBar.scale((particle_limit - counter) / (float)particle_limit, 1);
    if (showHUD)
    {
        if (g_eGameState == S_GAMEMODE1)
        {
            counter = 0;
            for (int i = 0; i < NPCLimit; i++)
            {
                counter += (NPCs[i] != nullptr && NPCs[i]->type() == 'C') ? 1 : 0;
            }
            counter -= NPC::getnoHostile();
            objective = "Objective:";
            if (counter > 0)
            {
                objective.append("cough at ");
                objective.append(std::to_string(counter));
                objective.append(" civilian");
            }
            else
                objective.append("escape");
                
        }
        renderBox(&HealthText, 0x04, "Health");
        renderBox(&HealthBorder, 0x00);
        renderBox(&healthBar, 0x40);
        renderBox(&coughBar, 0x20);
        renderBox(&Objective, 0x70, objective);
    }
}

//void updateHUD()
//{
//    
//    if (showHUD)
//    {
//        renderBox(&healthBar, 0x04);
//        renderBox(&coughBar, 0x02);
//    }
//}

int checkButtonClicks(Object** buttons, int arrayLength)
{
    int mouseX, mouseY;
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {// check when player left click 
        if (isMousePressed == false)
        {
            isMousePressed = true;
            mouseX = g_mouseEvent.mousePosition.X;
            mouseY = g_mouseEvent.mousePosition.Y;
            for (int i = 0; i < arrayLength; i++)
            {//check all the objects in the given array
                if (mouseX >= buttons[i]->referencePosition()->get_x() - 1 &&
                    mouseX <= buttons[i]->referencePosition()->get_x() + buttons[i]->length() &&
                    mouseY >= buttons[i]->referencePosition()->get_y() - 1 &&
                    mouseY <= buttons[i]->referencePosition()->get_y() + buttons[i]->height())
                {// check if mouse is within this Object
                    return i;
                }
            }
        }
    } 
    else
    {
        isMousePressed = false;
    }
    return arrayLength;
}



void limitprojectile()
{
    for (int p = 0; p < particle_limit; p++)
    {
        if (projectile[p] != nullptr)
        {
            if (projectile[p]->get_pcooldown() > 0)
            {
                if (projectile[p]->get_pcooldown() > 100)
                    projectile[p]->set_pcooldown(100); // make sure it cap at 100
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
            if (NPCs[i]->isHostile() && occupied(NPCs[i]->getpos())->type() == 'P' && NPCs[i]->isonCD() == false)
            {
                //timer += g_dDeltaTime;
                //timer >3, run else dont;
                
                player->loseHP(NPCs[i]->get_damage());
                flashcount = 1 / g_dDeltaTime;
                player->set_flash(true);
                switch (g_eGameState)
                {
                case S_GAMEMODE1:
                    NPCs[i]->cooldownstart();
                    NPCs[i]->set_count(NPCs[i]->get_ftime() / g_dDeltaTime);
                    player->set_pos(spawnPoint[4].get_x(), spawnPoint[4].get_y());
                    resetallNPCs();
                    NPC::resetnoHostile();
                    break;
                case S_GAMEMODE2:
                    player->set_pos(safezone[4].get_x(), safezone[4].get_y());
                    break;
                }
                
                
                g_sChar.m_cLocation.Y = player->getposy(); 
                g_sChar.m_cLocation.X = player->getposx(); 
                //timer = 0;
            }
 
        }
    }
}

void resetallNPCs()
{
    for (int i = 0; i < NPCLimit; i++)
    {
        if (NPCs[i] != nullptr)
        {
            if (NPCs[i]->isHostile())
            {
                NPCs[i]->calmdown();
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
  
            colour = 0x90;
            c.X = endPoint[i].get_x() - static_cast<int>(player->getposx()) + 40;
            c.Y = endPoint[i].get_y() - static_cast<int>(player->getposy()) + 12;
            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, "E", colour);
            }

            colour = 0x30;
            c.X = spawnPoint[i].get_x() - static_cast<int>(player->getposx()) + 40;
            c.Y = spawnPoint[i].get_y() - static_cast<int>(player->getposy()) + 12;
            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, "S", colour);
            }
        }
        else if (i != 4)
        {
  
            colour = 0x99;
            c.X = endPoint[i].get_x() - static_cast<int>(player->getposx()) + 40;
            c.Y = endPoint[i].get_y() - static_cast<int>(player->getposy()) + 12;
            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, " ", colour);
            }

            colour = 0x33;
            c.X = spawnPoint[i].get_x() - static_cast<int>(player->getposx()) + 40;
            c.Y = spawnPoint[i].get_y() - static_cast<int>(player->getposy()) + 12;
            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, " ", colour);
            }

        }
    }
    
    colour = 0x7F;
    c.X = endPoint[4].get_x() - static_cast<int>(player->getposx()) + 40;
    c.Y = endPoint[4].get_y() - static_cast<int>(player->getposy()) + 12;
    if (checkifinscreen(c))
    {
        g_Console.writeToBuffer(c, (char)254, colour);
    }

    colour = 0x7F;
    c.X = spawnPoint[4].get_x() - static_cast<int>(player->getposx()) + 40;
    c.Y = spawnPoint[4].get_y() - static_cast<int>(player->getposy()) + 12;
    if (checkifinscreen(c))
    {
        g_Console.writeToBuffer(c, (char)254, colour);
    }

}

void renderBG(int col)
{
    COORD c;
    for (int x = -1; x < 81; x++)
    {
    
        if (x == -1 || x == 80)
        {
            for (int y = 0; y < 26; y++) //for surrounding playarea walls vertically
            {
                c.X = x - static_cast<int>(player->getposx()) + 40;
                c.Y = y - static_cast<int>(player->getposy()) + 12;

                if (checkifinscreen(c))
                {
                    g_Console.writeToBuffer(c, " ", 0x00);
                }
            }
        }
        else
        {

            for (int y = 1; y < 25; y++) //for playarea bg
            {
                c.X = x - static_cast<int>(player->getposx()) + 40;
                c.Y = y - static_cast<int>(player->getposy()) + 12;
                if (checkifinscreen(c))
                {
                    g_Console.writeToBuffer(c, " ", col);
                }
            }

            //for surrounding playarea walls horizontally
            c.X = x - static_cast<int>(player->getposx()) + 40;
            c.Y = 0 - static_cast<int>(player->getposy()) + 12;

            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, " ", 0x00);
            }

            c.Y = 25 - static_cast<int>(player->getposy()) + 12;

            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, " ", 0x00);
            }

        }

    }

}

void setallrpos()
{
    for (int i = 1; i < entityLimit; i++)
    {
        player->set_rpos(40, 12);
        if (entities[i] != nullptr)
        {
            entities[i]->set_rpos((int)entities[i]->getposx() - (int)player->getposx() + 40, (int)entities[i]->getposy() - (int)player->getposy() + 12);

        }
    }
    
}

bool checkifinscreen(COORD c)
{
    if (c.X <= 79 && c.Y <= 24 && c.X >= 0 && c.Y >= 0)
    {
        return true;
    }

    return false;
}

bool insafezone(Position* pos)
{
    if (static_cast<int>(pos->get_x()) <= 41 && static_cast<int>(pos->get_x()) >= 39 && static_cast<int>(pos->get_y()) <= 13 && static_cast<int>(pos->get_y() >= 11))
    {
        return true;
    }
    return false;
}

void deleteEntities()
{
    for (int i = 1; i < entityLimit; i++)
    {
        if (entities[i] != nullptr)
        {
            for (int n = 0; n < NPCLimit; n++)
            {
                if (NPCs[n] == entities[i])
                {
                    delete entities[i];
                    NPCs[n] = nullptr;
                    entities[i] = nullptr;
                    return;
                }
            }
            for (int n = 0; n < WallLimit; n++)
            {
                if (Walls[n] == entities[i])
                {
                    delete entities[i];
                    Walls[n] = nullptr;
                    entities[i] = nullptr;
                    return;
                }
            }
            for (int c = 0; c < CCTVLimit; c++)
            {
                if (CCTVs[c] == entities[i])
                {
                    delete entities[i];
                    CCTVs[c] = nullptr;
                    entities[i] = nullptr;
                }
            }
            /*delete entities[i];
            entities[i] = nullptr;*/
            
        }
    }
    delete entities[0];
    player = nullptr;
    entities[0] = nullptr;
}

void renderText()
{
    COORD textpos;
    textpos.X = 30;
    textpos.Y = 20;
    g_Console.writeToBuffer(textpos, text , 0x0F);
}

void renderCCTV()
{
    COORD cctvpos;
    COORD radarpos;
    int colour;
    for (int c = 0; c < CCTVLimit; c++)
    {
        if (CCTVs[c] != nullptr)
        {
            //rendering of CCTV
            colour = 0x7A;
            cctvpos.X = CCTVs[c]->getrposx();
            cctvpos.Y = CCTVs[c]->getrposy();

            if (checkifinscreen(cctvpos))
            {
                g_Console.writeToBuffer(cctvpos, (char)233 , colour);
            }

            //rendering of CCTV's line of sight
            for (int r = 0; r < 20; r++)
            {
                colour = 0x8F;
                radarpos.X = CCTVs[c]->getRadarPos(r)->get_x() - static_cast<int>(player->getposx()) + 40;
                radarpos.Y = CCTVs[c]->getRadarPos(r)->get_y() - static_cast<int>(player->getposy()) + 12;
                if (checkifinscreen(radarpos))
                {
                    g_Console.writeToBuffer(radarpos, (char)177, colour);
                }
            }
        }
    }
}

void spawnCCTV(int no)
{
    for (int i = 0; i < no; i++)
    {
        Position temp;
        bool valid;
        do
        {
            valid = true;
            temp.set_pos(rand() % 80, (rand() % 24) + 1);

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

        } while (occupied(&temp) != nullptr || valid == false);

        for (int c = 0; c < CCTVLimit; c++)
        {
            if (CCTVs[c] == nullptr)
            {
                CCTVs[c] = new CCTV((rand() % 8) + 1, (rand() % 2));
                entities[c + 61] = CCTVs[c];
                CCTVs[c]->set_pos(temp.get_x(), temp.get_y());
                CCTVs[c]->setCD(1 / g_dDeltaTime);
                CCTVs[c]->update_cctv();
                break;
            }
        }

    }
}