
#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"
#include<string>

//Entities
#include "Player.h"
#include "NPC.h"
#include "Police.h"

#include "Object.h"

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;

// struct to store keyboard events
// a small subset of KEY_EVENT_RECORD
struct SKeyEvent
{
    bool keyDown;
    bool keyReleased;
};

// struct to store mouse events
// a small subset of MOUSE_EVENT_RECORD
struct SMouseEvent
{
    COORD mousePosition;
    DWORD buttonState;
    DWORD eventFlags;
};

// Enumeration to store the control keys that your game will have
enum EKEYS
{
    K_W,
    K_S,
    K_A,
    K_D,
    K_ESCAPE,
    K_SPACE,
    K_COUNT
};
// Enumeration for the different screen states
enum EGAMESTATES
{
    S_MAINMENU,
    S_GAMEMODE1,
    S_GAMEMODE2,
    S_GAMEMODE3,
    S_PAUSEMENU,
    S_TEST,
    S_COUNT
};

// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
    bool  m_bActive;
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory

void splashScreenWait();    // waits for time to pass in splash screen
void updateGame();          // gameplay logic
void moveCharacter();       // moves the character, collision detection, physics, etc
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void clearScreen();         // clears the current screen and draw from scratch 
void renderSplashScreen();  // renders the splash screen
void renderGame();          // renders the game stuff
void renderMap();           // renders the map to the buffer first
void renderCharacter();     // renders the character into the buffer
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game
void renderInputEvents();   // renders the status of input events

//Walls
void spawnWall(int no);
void renderWall();

//NPCs 
void spawnNPC(bool isPolice, int no, float spd, int cooldowntime); //spawns NPCs
void moveall(); //moves all NPCs
void renderNPC(); //draws NPCs on map
Entity* occupied(Position*);//if no entity occupy that position, return nullptr
void renderprojectile(); //set projectile colour and draw on map
void limitprojectile();
void check_collision();

//UI, Map Objects
void renderMainMenu(); //main menu.
void mainMenuWait(); //init UI for main menu
void renderBox(Object*, int, std::string); // draw box. can add text if you want
int checkButtonClicks(Object**, int);// check if player clicked a button

// keyboard and mouse input event managers
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent);  // define this function for the console to call when there are keyboard events
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);      // define this function for the console to call when there are mouse events
void buttonHoldPress(EKEYS key);
void buttonHoldRelease(EKEYS key); 
int getButtonHold();


void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent);   // handles keyboard events for gameplay 
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent); // handles mouse events for gameplay 


#endif // _GAME_H