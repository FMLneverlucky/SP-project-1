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
    K_UP,
    K_DOWN,
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


void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory
void deleteEntities(); // delete all entities in entities array

void splashScreenWait();    // waits for time to pass in splash screen
void updateGame();          // gameplay logic
void moveCharacter();       // moves the character and all NPCs
void checkAll();            //collision detection, physics etc
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void clearScreen();         // clears the current screen and draw from scratch 
void renderSplashScreen();  // renders the splash screen
void renderGame();          // renders the game stuff
void renderMap();           // renders the map to the buffer first
void renderCharacter();     // renders the character into the buffer
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game
void renderInputEvents();   // renders the status of input events

//memory storage stuff
bool is_empty(std::ifstream& pFile); // check if a file is empty
void updateScore(std::string fileName, int score, int* sessionBest); // store highscore to memory
void updateScore(std::string fileName, double score, double* sessionBest); // store highscore to memory
void initStoredData(std::string fileName, int*); // create file if it doesnt exist
void initStoredData(std::string fileName, double*); // create file if it doesnt exist

//For Game Modes
void resetSpawns(); //deletes any existing entities, sets all ptrs to nullptr
bool inZone(Position* pos, Zone& zone); //checks if passed in position lies within a zone(spawnendpoint/safezone)
//Normal mode
void playNormal();
void InitNormal();
void playLevel();
void set_spawn(); //sets variables and npc stats based on level and spawns entities accordingly
void spawnAll(); //spawns all entities according to variables
void level_set(); //prepares for next level
void renderPoints(); //renders spawn and end points
void set_points(); //sets position of spawn and end points
//Endless Mode
void playEndless();
void InitEndless();
void enterEndless();
void rendersafezone(); //renders safezone
void horrorFreeze(bool on);

//Testing area
void testStates();
void initTest();
void playTest();
void endTest();

//Walls
void spawnWall(int no);
void renderWall();

//CCTV
void renderCCTV(); //renders CCTVs and their radar onto map
void spawnCCTV(int no); //spawns CCTVs

//Others
void renderBG(int col); //renders background map and play area boundary/walll surrounding play area
void setallrpos(); //sets relative position of all entities
bool checkifinscreen(COORD c); //checks if a certain coordinate lies within the console's dimensions

//NPCs 
void spawnNPC(bool isPolice, int no, float spd, float cooldowntime); //spawns NPCs
void moveall(); //moves all NPCs
void renderNPC(); //draws NPCs on map
Entity* occupied(Position*);//if no entity occupy that position, return nullptr
void check_collision(); //checks for collision between Hostile NPC and Player and does following actions

//Projectile
void renderprojectile(); //set projectile colour and draw on map
void limitprojectile();


//PowerUp
void spawnPowerUp();
void renderPowerUp();
void deletePowerUp();

//UI, Map Objects
void renderMainMenu();// main menu.
void mainMenuWait();// checks for cliks in main menu
void renderPauseMenu();
void pauseMenuWait();
void renderWinLoseMenu(bool);
void winLoseMenuWait();
void renderHorror();
void initMathHorror();
void waitMathHorror();
void initHUD();//init ONCE
void renderHUD();// renders HUD
void renderBox(Object*, int, std::string); // draw box. can add text if you want
int checkButtonClicks(Object**, int);// check if player clicked a button

// keyboard and mouse input event managers
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent);  // define this function for the console to call when there are keyboard events
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);      // define this function for the console to call when there are mouse events
void buttonHoldPress(EKEYS key); // only WASD
void buttonHoldRelease(EKEYS key); // only WASD
int getButtonHold(); // the last button held only for WASD
//int playerDirection();

void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent);   // handles keyboard events for gameplay 
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent); // handles mouse events for gameplay 

//Audio
void playSound(std::string filename, std::string filetype, bool loop);
void muteBGM();
#endif // _GAME_H