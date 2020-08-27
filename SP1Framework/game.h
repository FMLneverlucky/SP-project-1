#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"
#include<string>

//Entities
#include "Player.h"
#include "NPC.h"
#include "Police.h"
#include "Object.h"
#include "Zone.h"
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
    //K_M,
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
    S_TUTORIAL,
    S_TEST,
    S_COUNT
};

// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
    bool  m_bActive;
};

//enum MainMenu
//{
//    M_MAIN,
//    M_GAMEMODES
//};

enum NormalMode
{
    N_INIT,
    N_LEVEL,
    N_NEXTLEVEL,
    N_LOSE
};

enum EndlessMode
{
    E_INIT,
    E_PLAY,
    E_LOSE
};

enum Test
{
    T_INIT,
    T_PLAY,
    T_END
};

enum Tutorial
{
    TUT_GAMEPLAY,
    TUT_POLICE,
    TUT_POWERUP
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory
void deleteEntities(); // delete all entities in entities array

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

bool is_empty(std::ifstream& pFile); // check if a file is empty

void updateScore(std::string fileName, int score); // store highscore to memory

void checkAll();

//Game Modes
void InitNormal();
void playNormal();
void playLevel();
void set_spawn();
void spawnAll();
void resetSpawns();
void level_set();
void playEndless();
void InitEndless();
void enterEndless();
void renderPoints();
void set_points();
void rendersafezone();
bool inZone(Position* pos, Zone& zone);


//Tutorials
void playTutorial();
void renderText();
void initTutGP();
void playTutGP();

//Testing area
void testStates();
void initTest();
void playTest();
void endTest();

//Walls
void spawnWall(int no);
void renderWall();

//CCTV
void renderCCTV();
void spawnCCTV(int no);

//aesthetics
void renderBG(int col);
void setallrpos();
bool checkifinscreen(COORD c);


//NPCs 
void spawnNPC(bool isPolice, int no, float spd, float cooldowntime); //spawns NPCs
void moveall(); //moves all NPCs
void renderNPC(); //draws NPCs on map
Entity* occupied(Position*);//if no entity occupy that position, return nullptr
void renderprojectile(); //set projectile colour and draw on map
void limitprojectile();
void check_collision();

//PowerUp
void spawnPowerUp();
void renderPowerUp();
void deletePowerUp();

//UI, Map Objects
void renderMainMenu();// main menu.
void mainMenuWait();// checks for cliks in main menu
void renderPauseMenu();// d
void pauseMenuWait();
void renderWinLoseMenu(bool);
void winLoseMenuWait();
void initHUD();//init ONCE
void renderHUD();// renders HUD
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

//Audio
void playSound(std::string filename, std::string filetype, bool loop);
void muteBGM();
#endif // _GAME_H