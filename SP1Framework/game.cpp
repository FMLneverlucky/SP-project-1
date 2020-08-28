// This is the main file for the game logic and function
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <irrKlang.h>
#include <fstream>
#include "game.h"
#include "Framework\console.h"
#include "time.h"
#include "Projectile.h"
#include "Wall.h"
#include "PowerUp.h"
#include "CCTV.h"
#include "Question.h"

#pragma comment(lib, "irrklang.lib")

using namespace irrklang;

//FOR TESTING
bool checkInputs = false;
bool checkTimeElapsed = false;
bool checkFramerate = true;
float splashScreenTime = 0.5;

//UI NAMES
std::string gameName = "A Very Fun Game";
std::string gameMode1 = "Normal";
std::string gameMode2 = "Endless Nightmare";
std::string gameMode3 = "Empty Button";
std::string gameMode4 = "Click This"; // for game test. not for final product
std::string winMessage = "HACKS REPORTED";
std::string loseMessage = "GGEZ Uninstall";
std::string deathByCCTV = "How'd they catch you in 4k?!";
std::string deathByCivilian = "You got beaten up lol";
std::string deathByPolice = "Get fined scrub";
std::string continueMessage = "Next Level";
std::string restartMessage = "Restart";
std::string mainMenuMessage = "Main Menu";
std::string quit = "Quit";
std::string resume = "Resume";
std::string back = "Back";
std::string gamemodes = "Play";

std::string objective = "";
std::string scoreboard = "";
std::string highscore = "";

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
std::string stage = "MAIN";


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

//Math horror th9ing
Question QNS;
Object correctAnswer(1,1);
Object wrongAnswer1(1, 1);
Object wrongAnswer2(1, 1);
Object wrongAnswer3(1, 1);
double showCooldown = 0;
double hideCooldown = 0;
std::string question;

//RENDERING
int BGcol;
int Wallcol;
int HNPCcol;
int HPolcol;

//GAME MODE
NormalMode NGameState = N_INIT;
EndlessMode EGameState = E_INIT;
Test TGameState = T_INIT;
int level = 0; //level no.
bool lose = false; //end game
bool clear = false;
int noC; //no. of civilian
int noP; //no. of Police
int noW; //no. of walls
int noCCTV; //no. of CCTVs
float spd; //spd of NPCs relative to player
int cdtime; //cooldown time of hostile NPCs after collision w player
int horrorChanceCount = 0; //counter for math horror jumpscare
bool invert = false;
bool horror = false;

//stored data
int highestLVL = 0;
double highestKPM = 0;
double bestTime = 0;
int highestKill = 0;
std::string highestLevelFile = "highestLevel.txt";
std::string highestKPMFile = "highestKPM.txt";
std::string bestTimeFile = "bestTime.txt";
std::string highestKillFile = "highestKill.txt";

int totalhostile = 0;

//for temp use in code
int tempcounter;
int flashcount = 0;

//map aesthetics
int col = 0x88;

double e_dElapsedTime; // time passed in endless mode
double kpm;

double  g_dElapsedTime;
double  g_dDeltaTime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

bool isMousePressed = false;
bool heldKey[7] = { false, false, false, false, false, false , false};
int playerVelocityX = 0;
int playerVelocityY = 0;
bool liftedW = true;
bool liftedA = true;
bool liftedS = true;
bool liftedD = true;


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
                        nullptr, nullptr, nullptr, nullptr, nullptr};                                             //CCTVs
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

Zone spawnPoint;
Zone endPoint;
Zone safeZone;

//Audio
ISoundEngine* engine = createIrrKlangDevice();

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

    /*std::ifstream file("highestLevel.txt");
    if (is_empty(file))
    {
        file.close();
        std::ofstream file("highestLevel.txt");
        if (file.is_open())
        {
            file << std::to_string(0);
            file.close();
        }
    }
    else
    {
        std::string temp;
        std::getline(file, temp);
        highestLVL = std::stoi(temp);
        file.close();
    }*/
    initStoredData(highestLevelFile, &highestLVL);
    initStoredData(highestKillFile, &highestKill);
    initStoredData(bestTimeFile, &bestTime);
    initStoredData(highestKPMFile, &highestKPM);
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
    case 0x4D: key = K_M; break;
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
        {
            buttonHoldPress(key);
            switch (key)
            {
            case 0:
                playerVelocityX += (playerVelocityX < 1 && liftedW) ? 1 : 0;
                liftedW = false;
                break;
            case 1:
                playerVelocityX += (playerVelocityX > -1 && liftedS) ? -1 : 0;
                liftedS = false;
                break;
            case 2:
                playerVelocityY += (playerVelocityY > -1 && liftedA) ? -1 : 0;
                liftedA = false;
                break;
            case 3:
                playerVelocityY += (playerVelocityY < 1 && liftedD) ? 1 : 0;
                liftedD = false;
                break;
            default:
                break;
            }
        }
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
        if (g_skKeyEvent[key].keyReleased)
        {
            buttonHoldRelease(key);
            switch (key)
            {
            case 0:
                playerVelocityX += playerVelocityX > -1 ? -1 : 0;
                liftedW = true;
                break;
            case 1:
                playerVelocityX += playerVelocityX < 1 ? 1 : 0;
                liftedS = true;
                break;
            case 2:
                playerVelocityY += playerVelocityY < 1 ? 1 : 0;
                liftedA = true;
                break;
            case 3:
                playerVelocityY += playerVelocityY > -1 ? -1 : 0;
                liftedD = true;
                break;
            default:
                break;
            }
        }
    }    
}

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
            if (playerVelocityX == 0 && (i == 0 || i == 1))
            { // incase they counterstrafe
                break;
            }
            if (playerVelocityY == 0 && (i == 2 || i == 3))
            { // incase they counterstrafe
                break;
            }
            return i;
        }
    }
    if (playerVelocityX > 0)
        return 0;
    if (playerVelocityX < 0)
        return 1;
    if (playerVelocityY < 0)
        return 2;
    if (playerVelocityY > 0)
        return 3;
    return 7;
}
//int playerDirection()
//{
//    switch (getButtonHold())
//    {
//    case 0:
//        playerVelocityX -= playerVelocityX < 1 ? 0 : 1;
//        break;
//    case 1:
//        playerVelocityX -= playerVelocityX > -1 ? 0 : 1;
//        break;
//    case 2:
//        playerVelocityY -= playerVelocityY > -1 ? 0 : 1;
//        break;
//    case 3:
//        playerVelocityY += playerVelocityY < 1 ? 0 : 1;
//        break;
//    default:
//        break;
//
//    }
//}

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
        case S_GAMEMODE1: playNormal(); // normal mode
            break;
        case S_GAMEMODE2: playEndless(); // endless mode
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
    player->set_pos(40, 12);
    g_sChar.m_cLocation.X = 40;
    g_sChar.m_cLocation.Y = 12;
    level_set();
    initHUD();
    
    NGameState = N_LEVEL;
    engine->play2D("media/NModeBGM.mp3", true);
}
 
void set_spawn() //sets variables and npc stats based on level and spawns entities accordingly
{
    if (level == 1)
    {
        noC = 3;
        noP = 0;
        spd = 0.2;
        cdtime = 3;
        noW = 10;
        noCCTV = 0;
    }
    else if (level < 6)
    {
        noC++;
        spd += 0.05;
        cdtime = 2.5;
        if (level % 2 == 1)
        {
            noCCTV++;
        }
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
        if (level == 7 || level == 10)
        {
            noCCTV++;
        }
    }
    else
    {
        spd = 0.9;
        cdtime = 1;
        noP = 5;
        noC = 15;
        noCCTV = 5;
    }
    spawnAll();
}

void spawnAll()
{
    spawnWall(noW); //spawns walls
    spawnNPC(false, noC, spd, cdtime); //spawns civillians(NPC)
    spawnNPC(true, noP, spd, cdtime); //spawns police(NPC)
    spawnCCTV(noCCTV); //spawns CCTVs
}

void set_points()
{
    spawnPoint.setpos(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y); //sets spawnpoint to be the current position of the player

    bool overlap;
    Position temppos;
    do
    {
        overlap = false;
        temppos.set_x((rand() % 78) + 1);
        temppos.set_y((rand() % 21) + 2);

        endPoint.setpos(temppos.get_x(), temppos.get_y()); //sets endpoint to randomised position

        //checks if spawn and end points overlap
        for (int s = 0; s < 9; s++)
        {
            for (int e = 0; e < 9; e++)
            {
                if (spawnPoint.getpos(s)->get_x() == endPoint.getpos(e)->get_x() && spawnPoint.getpos(s)->get_x() == endPoint.getpos(e)->get_y())
                {
                    overlap = true;
                }
            }
        }
    } while (occupied(&temppos) != nullptr || inZone(&temppos, spawnPoint) || overlap); //conditions for end point's location to be invalid

    
 
}

void resetSpawns() //deletes any existing entities, sets all ptrs to nullptr
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

void level_set() //prepares for next level
{
    
    resetSpawns();
   
    player->resetHP();
    NPC::resetnoHostile();
    player->resetlethality();

    level++;

    set_points();
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

    if (player->get_lethalstatus() == 1) // if powerup picked up before
        player->update_ld(); //- 1 each time run tis code(runs by frame)

    //level clear condition
    if (NPC::getnoHostile() == noC + noP && static_cast<int>(player->getposx()) == endPoint.getpos(4)->get_x() && static_cast<int>(player->getposy()) == endPoint.getpos(4)->get_y())
    {
        clear = true;
        NGameState = N_NEXTLEVEL;
    }
    //end game condition
    if (player->get_HP() <= 0)
    {
        lose = true;
    }
    //tabulating of highest level cleared and deleting of remaining entities once player loses
    if (lose)
    {
        updateScore(highestLevelFile, level, &highestLVL);
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
        break;
    }
}

void InitEndless()
{
    //resetting variables etc to prepare for Endless
    player->resetKills();
    lose = false;
    totalhostile = 0;
    player->resetHP();
    NPC::resetnoHostile();
    tempcounter = 0;
    e_dElapsedTime = 0;

    player->set_pos(40, 12);
    g_sChar.m_cLocation.X = 40;
    g_sChar.m_cLocation.X = 12;
    safeZone.setpos(40, 12);
    horrorChanceCount = ((rand() % 20) + 20) / g_dDeltaTime;

    //spawning of entities
    spawnWall(10);
    spawnNPC(false, 5, 0.6, 1);
    spawnNPC(true, 3, 0.7, 1);
    spawnCCTV(5);
    

    initHUD();

    EGameState = E_PLAY;
}

void enterEndless()
{
    e_dElapsedTime += g_dDeltaTime;
    tempcounter++;
    kpm = player->getKills() / (e_dElapsedTime / 60);
    spawnPowerUp();
    deletePowerUp();
    updateGame();

    if (player->get_lethalstatus() == 1) // if powerup picked up before
        player->update_ld(); //- 1 each time run tis code(runs by frame)
    //chance of Math Horror Jumpscare
    if (horrorChanceCount <= 0)
    {
        horrorFreeze(true);
        initMathHorror();
        horrorChanceCount = ((rand() % 20) + 20) / g_dDeltaTime;
    }
    else
    {
        horrorChanceCount --;
    }

    if (horror)
    {
        if (showCooldown <= 0 && hideCooldown <= 0)
            hideCooldown = 1;
        if (hideCooldown > 0)
        {
            hideCooldown -= g_dDeltaTime;
            if (hideCooldown <= 0)
                showCooldown = rand() % 2 + 3;
        }
        else
            showCooldown -= g_dDeltaTime;
        if (paused)
        {
            hideCooldown = 1;
            showCooldown = 0;
        }

    }
    else
    {
        //spawns NPCs every 3 seconds if there are < 20 already on the Map
        if (tempcounter > (3 / g_dDeltaTime) && NPC::gettotal() != 20)
        {
            spawnNPC(false, 1, 0.6, 1);
            tempcounter = 0;
        }

        //despawning Hostile NPCs once their lifespan runs out (lifespan is set once they turn hostile)
        for (int i = 0; i < NPCLimit; i++)
        {
            if (NPCs[i] != nullptr)
            {
                if (NPCs[i]->isHostile() && NPCs[i]->type() == 'C')
                {
                    if (NPCs[i]->get_lifespan() <= 0)
                    {
                        delete NPCs[i];
                        NPCs[i] = nullptr;
                    }
                    else
                    {
                        NPCs[i]->set_lifespan(NPCs[i]->get_lifespan() - 1);
                    }
                }
            }
        }
    }

    //end game condition
    if (player->get_HP() <= 0)
    {
        lose = true;
    }

    //tabulating and resetting of variables once player loses
    if (lose)
    {
        updateScore(bestTimeFile, e_dElapsedTime, &bestTime);
        updateScore(highestKillFile, player->getKills(), &highestKill);
        updateScore(highestKPMFile, highestKPM, &highestKPM);
        totalhostile = NPC::getnoHostile();
        resetSpawns();
        EGameState = E_LOSE;
        
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
            //rendering of safezone's outer colour
            colour = 0xBB;
            c.X = safeZone.getpos(i)->get_x() - static_cast<int>(player->getposx()) + 40;
            c.Y = safeZone.getpos(i)->get_y() - static_cast<int>(player->getposy()) + 12;
            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, " ", colour);
            }

        }
    }

    //rendering of the middle of safezone
    colour = 0x7F;
    c.X = safeZone.getpos(4)->get_x() - static_cast<int>(player->getposx()) + 40;
    c.Y = safeZone.getpos(4)->get_y() - static_cast<int>(player->getposy()) + 12;
    if (checkifinscreen(c))
    {
        g_Console.writeToBuffer(c, (char)254, colour);
    }

}

void moveCharacter()
{
    if (!horror)
    {
        // Updating the location of the character based on the key being held
        if (getButtonHold() == K_W && g_sChar.m_cLocation.Y > 1)
        {
            player->set_direction(1);
        }
        else if (getButtonHold() == K_A && g_sChar.m_cLocation.X > 0)
        {
            player->set_direction(3);
        }
        else if (getButtonHold() == K_S && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
        {
            player->set_direction(2);
        }
        else if (getButtonHold() == K_D && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
        {
            player->set_direction(4);
        }
        else
        {
            player->set_direction(0);
        }


        //conditions such that player cannot move; got wall etc
        if (occupied(entities[0]->new_pos(g_dDeltaTime)) != nullptr && occupied(entities[0]->new_pos(g_dDeltaTime)) != entities[0])
        {
            player->set_direction(0);
        }
        if (!inZone(player->getpos(), safeZone) && g_eGameState == S_GAMEMODE2 && inZone(player->new_pos(g_dDeltaTime), safeZone))
        {
            player->set_direction(0);
        }

        player->update_pos(g_dDeltaTime); //sets pos of player
        g_sChar.m_cLocation.Y = player->getposy(); //moves player
        g_sChar.m_cLocation.X = player->getposx(); //moves player
    }
    
    moveall(); //moves all NPCs
}

void checkAll()
{
    //Cough related checks and updates
    if (g_skKeyEvent[K_SPACE].keyReleased)
    {
        for (int p = 0; p < particle_limit; p++)
        {
            if (projectile[p] == nullptr)
            {
                //Coughs if there are projectiles available
                projectile[p] = new Projectile;
                projectile[p]->set_ppos(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y);
                projectile[p]->direction(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y);
                projectile[p]->set_newpos();
                projectile[p]->set_pcooldown(100);
                engine->play2D("media/CoughSFX.mp3", false);

                //checking if player is within cctv radar when coughing - lose game condition
                for (int c = 0; c < CCTVLimit; c++)
                {
                    if (CCTVs[c] != nullptr)
                    {
                        for (int r = 0; r < 25; r++)
                        {
                            if (occupied(CCTVs[c]->getRadarPos(r)) != nullptr)
                            {
                                if (occupied(CCTVs[c]->getRadarPos(r))->type() == 'P')
                                {
                                    player->prevDamaged(CCTVs[c]->type());
                                    lose = true;
                                    break;
                                }

                            }
                        }
                    }
                }

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

        
    }

    check_collision(); //checks for HostileNPC-Player Collision
    limitprojectile(); //moves/updates projectiles

    //CCTV related checks/updates
    for (int c = 0; c < CCTVLimit; c++)
    {
        if (CCTVs[c] != nullptr)
        {   
            //rotates CCTVs radar every 2 seconds
            if (CCTVs[c]->getCD() == 0)
            {
                CCTVs[c]->update_cctv();
                CCTVs[c]->setCD(2 / g_dDeltaTime);
            }
            else
            {
                CCTVs[c]->setCD(CCTVs[c]->getCD() - 1);
            }
            
            //checks for walls, and blocks CCTV's line of sight accordingly
            for (int i = 0; i < 25; i++)
            {
                if (occupied(CCTVs[c]->getRadarPos(i)) != nullptr)
                {
                    if (occupied(CCTVs[c]->getRadarPos(i))->type() == 'W')
                    {
                        CCTVs[c]->disable(i);
                    }
                }
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
                    if (NPCs[i] != nullptr)
                    {
                        if (NPCs[i] == occupied(projectile[p]->getpos()) && NPCs[i]->isHostile() == false)
                        {
                            NPCs[i]->anger();
                            NPCs[i]->cooldownstart();
                            NPCs[i]->set_count(NPCs[i]->get_ftime() / g_dDeltaTime);
                            NPCs[i]->set_lifespan(20 / g_dDeltaTime);

                        }

                        if (NPCs[i] == occupied(projectile[p]->getpos()) && player->get_lethalstatus() == 1) // if player is buffed, projectile will delete any npc
                        {
                            player->addKills(1);
                            delete NPCs[i];
                            NPCs[i] = nullptr;
                        }
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

    if (invert)
    {
        BGcol = 0x00;
        Wallcol = 0x88;
        HNPCcol = 0x55;
        HPolcol = 0xDD;
    }
    else
    {
        BGcol = 0x88;
        Wallcol = 0x00;
        HNPCcol = 0x44;
        HPolcol = 0xCC;
    }

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
        renderBG(col);
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
    setallrpos();       //sets relative position for all entities for camera lock
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    if (horror)
    {
        renderHorror();
    }
    if (paused)
        renderPauseMenu();
    else
        renderHUD();
}

void renderMap()
{
    renderCCTV();
    renderNPC();
    renderWall();
    
    if (invert == false)
    {
        
        renderprojectile();
        renderPowerUp();
    }
    
    
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
    if (invert)
    {
        charColor = 0xF4;
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
{
    COORD c;
    //int colour;
    for (int i = 0; i < WallLimit*4; i++)
    {
        if (Walls[i] != nullptr)
        {
            c.X = Walls[i]->getposx() - static_cast<int>(player->getposx()) + 40;
            c.Y = Walls[i]->getposy() - static_cast<int>(player->getposy()) + 12;

            //colour = 0x00;

            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, "W", Wallcol);
            }
        }
    }
}

void spawnWall(int no)                                                                                                              //function to spawn wall
{
    for (int j = 0; j < no; j++)                                                                                                    //for loop to cycle the spawning of each wall
    {   //find random x and y on unused spaces
        bool isSpaceinZone;                                                                                             //used as a second conditon in while loop to ensure no space chosen intersects with the spawn zone
        bool isSpaceOccupied;

        for (int w = 0; w < WallLimit; w++)                                                                                         // for loop to set positions on map for each wall entity
        {
            if (Walls[w] == nullptr)                                                                                                //checks if wall entity is unassigned on map
            {
                Walls[w] = new Wall;
                entities[w + 21] = Walls[w];

                do
                {
                    isSpaceinZone = false;                                                                                             //used as a second conditon in while loop to ensure no space chosen intersects with the spawn zone
                    isSpaceOccupied = false;

                    int Pivotx = (rand() % 77) + 2;                                                                                     //set x coordinate of variable, wallPos[0], as a number from 0 to 80
                    int Pivoty = (rand() % 20) + 3;                                                                                     //set y coordinate of variable, wallPos[0], as a number from 0 to 24
                    Walls[w]->setPos(Pivotx, Pivoty);
                    
                    Walls[w]->setPosForAll();
                    for (int i = 0; i < 4; i++)
                    {
                        switch (g_eGameState)
                        {
                        case S_GAMEMODE1:
                            if (inZone(Walls[w]->getPos(i), spawnPoint) || inZone(Walls[w]->getPos(i), endPoint))
                            {
                                isSpaceinZone = true;
                            }
                            break;
                        case S_GAMEMODE2:
                            if (inZone(Walls[w]->getPos(i), safeZone))
                            {
                                isSpaceinZone = true;
                            }
                            break;
                        }

                        if (occupied(Walls[w]->getPos(i)) != nullptr && occupied(Walls[w]->getPos(i)) != Walls[w])
                        {
                            isSpaceOccupied = true;
                            break;
                        }
                        
                    }
                } while (isSpaceOccupied == true || isSpaceinZone == true);                                                      //while position on map is unavailable

                for (int i = 1; i < 4; i++)
                {
                    Walls[w + WallLimit * i] = new Wall;                                                                              //set element of array as new object under wall class
                    entities[w + 21 + WallLimit * i] = Walls[w + WallLimit * i];                                                        //set element from wall array to corresponding element on entity array
                    entities[w + 21 + WallLimit * i]->set_pos(Walls[w]->getPos(i)->get_x(), Walls[w]->getPos(i)->get_y());            //set position of the temp wall entity to an element in the entity array
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
                g_Console.writeToBuffer(pu, (char)232, colour); //powerup aqua with yellow ascii tile
            }
    }
    
}

void spawnPowerUp()
{
    if (powerup == nullptr)
    {
        int r = rand() % 1350;


        if (r == 45)//chance of spawning completely random
        {
            powerup = new PowerUp;

            do
            {
                powerup->set_xcoord(rand() % 80);
                powerup->set_ycoord((rand() % 24) + 1); 
            } while (occupied(powerup->get_pos()) != nullptr);

            powerup->set_detime(2250);//abt 25 sec to despawn
        }
    }
}

void deletePowerUp()
{
    if (powerup != nullptr)
    {
        if ((g_sChar.m_cLocation.X == powerup->get_xcoord()) && (g_sChar.m_cLocation.Y == powerup->get_ycoord())) //when player walks over powerup
        {
            player->set_lethal(); //set buff duration and buff is true
            delete powerup;
            powerup = nullptr;
        }

        else if (powerup->get_detime() != 0)
            powerup->set_detime(powerup->get_detime() - 1);//when despawn time hasnt run out, minus 1

        else
        {
            delete powerup; //deletes powerup when despawn time runs out
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

            if (NPCs[i]->type() == 'B') //Police
            {
                if (NPCs[i]->isHostile())
                {
                    colour = HPolcol;
                }
                else
                {
                    colour = 0x11;
                }
            }
            else //Civillian
            {
                if (NPCs[i]->isHostile())
                {
                    colour = HNPCcol;
                }
                else
                {
                    colour = 0xF6;
                }
            }

            if (invert)
            { 
                if (NPCs[i]->isHostile() && checkifinscreen(c))
                {
                    g_Console.writeToBuffer(c, " ", colour);
                }
            }
            else if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, " ", colour);
            }

            
        }
    }
    
}

void spawnNPC(bool isPolice, int no, float spd, float cooldowntime) 
{
    for (int i = 0; i < no; i++)
    {
        Position temppos;
        bool valid; 

        do
        {
            valid = true;
            //generates random pos
            temppos.set_x(rand() % 80);
            temppos.set_y((rand() % 23) + 1);
          
            //check if pos is valid
            switch (g_eGameState)
            {
            case S_GAMEMODE1:

                if (inZone(&temppos, spawnPoint) || inZone(&temppos, endPoint))
                {
                    valid = false;
                }
                break;

            case S_GAMEMODE2:

                COORD t;
                t.X = temppos.get_x() - player->getposx() + 40;
                t.Y = temppos.get_y() - player->getposy() + 12;

                if (checkifinscreen(t) == false) //NPCs spawn only within screen in Endless
                {
                    valid = false;
                }

                if (inZone(&temppos, safeZone))
                {
                    valid = false;
                }
                break;
            }

        } while (occupied(&temppos) != nullptr || valid == false); //while pos is not available

        for (int n = 0; n < NPCLimit; n++)
        {
            if (NPCs[n] == nullptr)
            {
                //creates NPC Object and sets Position and other variables
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
                entities[n + 1]->set_pos(temppos.get_x(), temppos.get_y());
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
            if (NPCs[i]->isHostile() == false) //movement for NEUTRAL NPCs
            {
                int tempcount = ((rand() % 3) + 5) / g_dDeltaTime; //counter which acts as cooldown before NPCs change direction, seconds before changing is randomised so all NPCs change at different timings

                if (NPCs[i]->get_count() < tempcount)
                {
                    NPCs[i]->set_count(NPCs[i]->get_count() + 1);

                    if (NPCs[i]->get_count() > (0.7 * tempcount)) //stops NPC from moving just before it is about to change directon for smoother movement and change in direction
                    {
                        NPCs[i]->set_direction(0);
                    }
                }
                else 
                {

                    NPCs[i]->set_count(0); //resets NPC's count

                    int aaa = (rand() % 7) + 1; //randomises direction 
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

                //condition for NPCs to be unable to move in their chosen direction
                //walls, other entities etc
                if (occupied(NPCs[i]->new_pos(g_dDeltaTime)) != nullptr && occupied(NPCs[i]->new_pos(g_dDeltaTime)) != NPCs[i])
                {

                    NPCs[i]->set_direction(0);
                }

                
            }
            else if (NPCs[i]->isonCD()) //npc is hostile but on cooldown
            {
                NPCs[i]->set_direction(0); //NPC does not move
                NPCs[i]->set_count(NPCs[i]->get_count() - 1); //when hostile, npc's count is used for cooldown instead of direction change since direction is dependant on player and not randomised
                if (NPCs[i]->get_count() == 0)
                {
                    NPCs[i]->cooldownend(); //stops npcs from their cooldown
                }
                
            }
            else //npc is hostile and not on cooldown
            {
                int diffinx = g_sChar.m_cLocation.X - static_cast<int>(NPCs[i]->getposx());
                int diffiny = g_sChar.m_cLocation.Y - static_cast<int>(NPCs[i]->getposy());

                if (abs(diffinx) > abs(diffiny)) //checks if npc is further from the player by x or y
                {
                    //distance larger by x
                    if (diffinx > 0) //checks if player is on the left or right
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
                    //distance by x == distance by y
                    int a = (rand() % 2) + 1; //randomise left/right or up/down so that npcs have a chance to split up instead of moving tgt as one block
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
                else 
                {
                    //distance by y is larger
                    if (diffiny > 0)
                    {
                        NPCs[i]->set_direction(2);
                    }
                    else
                    {
                        NPCs[i]->set_direction(1);
                    }
                }

                //conditions where hostile npcs cannot move forward; walls/cctvs
                if (occupied(NPCs[i]->new_pos(g_dDeltaTime)) != nullptr)
                {
                    if (occupied(NPCs[i]->new_pos(g_dDeltaTime))->type() == 'W' || occupied(NPCs[i]->new_pos(g_dDeltaTime))->type() == 'R')
                    {

                        NPCs[i]->set_direction(0);
                    }
                }
                            
            }

            //conditions where ALL NPCs cannot move forward
            switch (g_eGameState)
            {
            case S_GAMEMODE1:
                if (inZone(NPCs[i]->new_pos(g_dDeltaTime), spawnPoint) || inZone(NPCs[i]->new_pos(g_dDeltaTime), endPoint))
                {
                    NPCs[i]->set_direction(0);
                }
                break;
            case S_GAMEMODE2:
                if (inZone(NPCs[i]->new_pos(g_dDeltaTime), safeZone))
                {
                    NPCs[i]->set_direction(0);
                }
                break;
            }

            NPCs[i]->update_pos(g_dDeltaTime); //sets pos of and moves all NPCs
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

            if (player->get_lethalstatus() == 1)// changes projectile colour to purple when in buffed state
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
            g_Console.writeToBuffer(c, temp = (x >= ((box->length() + 1) / 2) - (text.length() + 1) / 2 && x < ((box->length() + 1) / 2) + ((text.length() + 1) / 2)  && y == box->height() / 2) ? text[i++] : ' ', colour);
        }
        c.Y++;
    }
}

void renderMainMenu()
{// put this in render loop
    if (stage == "MAIN")
    {// create the object classes outside (scroll to top)
        title.move(consoleSize.X / 2, consoleSize.Y / 4);// use move function to move it to where u want it to be
        gamemodeButton.move(consoleSize.X / 2, consoleSize.Y * 2 / 4);
        quitButton.move(consoleSize.X / 2, consoleSize.Y * 3 / 4);

        MMButtons[0] = &gamemodeButton;// create an array for the clickable functions
        MMButtons[1] = &quitButton;// add the object objects that are used as buttons into the array

        renderBox(&gamemodeButton, 0x78, gamemodes);// use this renderbox function to render the object
        renderBox(&quitButton, 0x78, quit); // 3rd parameter(text) is optional. text length must be smaller than or equal to length of box
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
{// put this in update loop
    if (stage == "MAIN")
    {
        switch (checkButtonClicks(MMButtons, MMButtonCount))
        { // add what u want the buttons to do here
        case 0: // the order is according to the position you placed the button in the array
            stage = "SELECT";
            break;
        case 1:
            g_bQuitGame = true;
            break;
        default:// if no button is clicked, it will give a larger number and will come here
            break;
        }
    }
    if (stage == "SELECT")
    {
        switch (checkButtonClicks(MGButtons, MGButtonCount))
        {
        case 0:
            g_eGameState = S_GAMEMODE1;
            NGameState = N_INIT;
            break;
        case 1:
            g_eGameState = S_GAMEMODE2;
            EGameState = E_INIT;
            break;
        case 2:
            //nothing here
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
        horrorFreeze(false);
        break;
    case 1:
        g_eGameState = S_MAINMENU;
        paused = false;
        horrorFreeze(false);
        break;
    default:
        break;
    }
}

//set true for win screen, false for lose screen
void renderWinLoseMenu(bool win)
{
    Object title(71, 3, Position(consoleSize.X / 2, consoleSize.Y / 3));

    continueButton.move(consoleSize.X * 3 / 4, consoleSize.Y * 2 / 3);
    restartButton.move(consoleSize.X * 3 / 4, consoleSize.Y * 2 / 3);
    mainMenuButton.move(consoleSize.X * 2 / 4, consoleSize.Y * 2 / 3);
    quitButton.move(consoleSize.X / 4, consoleSize.Y * 2 / 3);

    WLButtons[0] = &quitButton;
    WLButtons[1] = &mainMenuButton;
    WLButtons[2] = win ? &continueButton : &restartButton;

    char killedBy = player->getPrevDamaged();
    std::string* message = &winMessage;
    if (!win)
    {
        if (killedBy == 'R') // killed by cctv
        {
            message = &deathByCCTV;
        }
        else if (killedBy == 'C') // killed by civilian
        {
            message = &deathByCivilian;
        }
        else if (killedBy == 'B') // killed by police
        {
            message = &deathByPolice;
        }
        else
        {
            message = &loseMessage;
        }
    }    
    renderBox(&title, 0x0F, *message);
    renderBox(WLButtons[0], 0x04, quit);
    renderBox(WLButtons[1], 0x0A, mainMenuMessage);
    renderBox(WLButtons[2], 0x0F, win ? continueMessage : restartMessage);
    
}

void winLoseMenuWait()
{
    showHUD = false;
    switch (checkButtonClicks(WLButtons, WLButtonCount))
    {
    case 0:
        g_bQuitGame = true;
        break;
    case 1:
        g_eGameState = S_MAINMENU;
        NGameState = N_INIT;
        EGameState = E_INIT;
        showHUD = true;
        break;
    case 2:
        NGameState = N_INIT;
        EGameState = E_INIT;
        showHUD = true;
        break;
    default:
        break;
    }
}

void initMathHorror()
{
    //FUNCTIONS YOULL NEED BELOW AAAAAAAA >--|-O
    QNS.SetNewQns(); //generates new question
    question = QNS.get_question(); //returns the question in string
    QNS.get_answer(); //returns answer to the question in int 
    //returns the 3 wrong other choices
    QNS.get_choice(0);
    QNS.get_choice(1);
    QNS.get_choice(2);

    //once player chooses an option set EGameState back to E_PLAY and call horrorFreeze(false);
    //EGameState = E_PLAY;
    //horrorFreeze(false);

    //dont forget to make sure u dont block the player we must make the player watch the npcs slowly approach them
    //must make them suffer 
}

void renderHorror()
{
    if (showCooldown > 0)
    {
        //render stuff
        g_Console.clearBuffer(0x00);
        int lines = 0;
        lines = (question.length() + 70) / 70;
        std::size_t startPos = 0;
        std::size_t endPos;

        for (int i = 0; i < lines; i++)
        {
            endPos = startPos + 60;
            std::string temp;
            if (endPos < question.length())
            {
                temp = question.substr(endPos);
                endPos += temp.find(" ");
                temp = question.substr(startPos, endPos - startPos);
                startPos = endPos;
            }
            else
                temp = question.substr(startPos);
            Object qns(78, 1, Position(consoleSize.X / 2, (consoleSize.Y / 5) + i));
            renderBox(&qns, 0x0F, temp);
        }
    }
    else
    {
        //dont render stuff
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
    Object Scoreboard(16, 1, Position(consoleSize.X / 2, 1));
    Object highScore(25, 1, Position(Scoreboard.position()->get_x(), Scoreboard.position()->get_y() + 1));
    if (player->get_HP() != currentHP)
    {
        currentHP = player->get_HP();
        healthBar.resize(20, 1);
        healthBar.move((healthBar.length() - 1) / 2 + 1, 2);
        healthBar.setPivot(healthBar.referencePosition()->get_x(), healthBar.referencePosition()->get_y());
        healthBar.scale((float)currentHP / player->get_maxHP (), 1);
    }
    int counter = 0;// count projectiles on board
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
            counter = 0; // this time to count npcs alive
            for (int i = 0; i < NPCLimit; i++)
            {
                counter += (NPCs[i] != nullptr && NPCs[i]->type() == 'C' && !NPCs[i]->isHostile()) ? 1 : 0;
            }
            objective = "Objective:";
            if (counter > 0)
            {
                objective.append("cough at ");
                objective.append(std::to_string(counter));
                objective.append(" civilian");
            }
            else
                objective.append("escape");
            scoreboard = "Level ";
            scoreboard.append(std::to_string(level));
            highscore = "Level Record: Level ";
            highscore.append(std::to_string(highestLVL));
        }
        if (g_eGameState == S_GAMEMODE2)
        {
            objective = "Kills: ";
            objective.append(std::to_string(player->getKills()));
            objective.append(" Time: ");
            objective.append(std::to_string(e_dElapsedTime));
            scoreboard = "KPM: ";
            scoreboard.append(std::to_string(kpm));
            highscore = "Best KPM: ";
            highscore.append(std::to_string(highestKPM));
        }
        renderBox(&HealthText, 0x04, "Health");
        renderBox(&HealthBorder, 0x00);
        renderBox(&healthBar, 0x40);
        renderBox(&coughBar, player->get_lethalstatus() == 1 ? 0x55 : 0xAA);
        renderBox(&Objective, 0x70, objective);
        renderBox(&Scoreboard, 0x07, scoreboard);
        renderBox(&highScore, 0x07, highscore);
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
                if (mouseX >= buttons[i]->referencePosition()->get_x() + 1 &&
                    mouseX <= buttons[i]->referencePosition()->get_x() + buttons[i]->length() &&
                    mouseY >= buttons[i]->referencePosition()->get_y() - 1 &&
                    mouseY <= buttons[i]->referencePosition()->get_y() + buttons[i]->height() - 1)
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
                player->loseHP(NPCs[i]->get_damage()); //player takes damage
                player->prevDamaged(NPCs[i]->type()); //records last NPC type that damaged player
                flashcount = 1 / g_dDeltaTime; 
                player->set_flash(true); //makes player flash in red for 1 second to show player has taken damage
                NPCs[i]->cooldownstart(); //sets NPCs on cooldown
                NPCs[i]->set_count(NPCs[i]->get_ftime() / g_dDeltaTime);
                switch (g_eGameState)
                {
                case S_GAMEMODE1:
                    player->set_pos(spawnPoint.getpos(4)->get_x(), spawnPoint.getpos(4)->get_y()); //moves player back to spawnPoint
                    resetSpawns(); //resets objectives
                    spawnAll(); 
                    NPC::resetnoHostile();
                    player->resetlethality();
                    break;
                case S_GAMEMODE2:
                    player->set_pos(safeZone.getpos(4)->get_x(), safeZone.getpos(4)->get_y()); //moves player back to safezone
                    break;
                }
                
                g_sChar.m_cLocation.Y = player->getposy(); //moves player char to player's pos
                g_sChar.m_cLocation.X = player->getposx(); 
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
            //rendering of top middle of spawn/end point, with labrl 'S' or 'E'
            colour = 0x90;
            c.X = endPoint.getpos(i)->get_x() - static_cast<int>(player->getposx()) + 40;
            c.Y = endPoint.getpos(i)->get_y() - static_cast<int>(player->getposy()) + 12;
            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, "E", colour);
            }
            colour = 0x30;
            c.X = spawnPoint.getpos(i)->get_x() - static_cast<int>(player->getposx()) + 40;
            c.Y = spawnPoint.getpos(i)->get_y() - static_cast<int>(player->getposy()) + 12;
            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, "S", colour);
            }
        }
        else if (i != 4)
        {
            //rendering of the rest of outer 8 blocks of spawn and end points
            colour = 0x99;
            c.X = endPoint.getpos(i)->get_x() - static_cast<int>(player->getposx()) + 40;
            c.Y = endPoint.getpos(i)->get_y() - static_cast<int>(player->getposy()) + 12;
            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, " ", colour);
            }
            colour = 0x33;
            c.X = spawnPoint.getpos(i)->get_x() - static_cast<int>(player->getposx()) + 40;
            c.Y = spawnPoint.getpos(i)->get_y() - static_cast<int>(player->getposy()) + 12;
            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, " ", colour);
            }

        }
    }

    //rendering of middle of spawn and end points
    colour = 0x7F;
    c.X = endPoint.getpos(4)->get_x() - static_cast<int>(player->getposx()) + 40;
    c.Y = endPoint.getpos(4)->get_y() - static_cast<int>(player->getposy()) + 12;
    if (checkifinscreen(c))
    {
        g_Console.writeToBuffer(c, (char)254, colour);
    }
    colour = 0x7F;
    c.X = spawnPoint.getpos(4)->get_x() - static_cast<int>(player->getposx()) + 40;
    c.Y = spawnPoint.getpos(4)->get_y() - static_cast<int>(player->getposy()) + 12;
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
            for (int y = 0; y < 26; y++) //rendering of playarea boundary walls vertically
            {
                c.X = x - static_cast<int>(player->getposx()) + 40;
                c.Y = y - static_cast<int>(player->getposy()) + 12;

                if (checkifinscreen(c))
                {
                    g_Console.writeToBuffer(c, " ", Wallcol);
                }
            }
        }
        else
        {
   
       
            for (int y = 1; y < 25; y++) //rendering of playarea background/map
            {
                c.X = x - static_cast<int>(player->getposx()) + 40;
                c.Y = y - static_cast<int>(player->getposy()) + 12;
                if (checkifinscreen(c))
                {
                    g_Console.writeToBuffer(c, " ", BGcol);
                }
            }

            //rendering of playarea boundary walls horizontally
            c.X = x - static_cast<int>(player->getposx()) + 40;
            c.Y = 0 - static_cast<int>(player->getposy()) + 12;

            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, " ", Wallcol);
            }

            c.Y = 25 - static_cast<int>(player->getposy()) + 12;

            if (checkifinscreen(c))
            {
                g_Console.writeToBuffer(c, " ", Wallcol);
            }

        }

    }

}

void setallrpos()
{
    for (int i = 1; i < entityLimit; i++)
    {
        //sets relative pos of all entities
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

bool inZone(Position* pos, Zone& zone)
{
    if ((int)pos->get_x() <= zone.getpos(5)->get_x() && (int)pos->get_y() <= zone.getpos(7)->get_y() && (int)pos->get_x() >= zone.getpos(3)->get_x() && (int)pos->get_y() >= zone.getpos(1)->get_y())
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
            
        }
    }
    delete entities[0];
    player = nullptr;
    entities[0] = nullptr;
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
            //rendering of CCTV's line of sight
            for (int r = 0; r < 25; r++)
            {
                colour = 0x8F;
                radarpos.X = CCTVs[c]->getRadarPos(r)->get_x() - static_cast<int>(player->getposx()) + 40;
                radarpos.Y = CCTVs[c]->getRadarPos(r)->get_y() - static_cast<int>(player->getposy()) + 12;
                if (checkifinscreen(radarpos) && CCTVs[c]->getRadarPos(r)->get_x() < 80 && CCTVs[c]->getRadarPos(r)->get_x() > -1 && CCTVs[c]->getRadarPos(r)->get_y() > 0 && CCTVs[c]->getRadarPos(r)->get_y() < 25)
                {
                    g_Console.writeToBuffer(radarpos, (char)177, colour);
                }
            }

            //rendering of CCTV
            colour = 0x7A;
            cctvpos.X = CCTVs[c]->getrposx();
            cctvpos.Y = CCTVs[c]->getrposy();

            if (checkifinscreen(cctvpos))
            {
                g_Console.writeToBuffer(cctvpos, (char)233 , colour);
            }

        }
    }
}

void spawnCCTV(int no)
{
    for (int i = 0; i < no; i++)
    {
        Position temppos;
        bool valid;
        do
        {
            valid = true;
            temppos.set_pos(rand() % 80, (rand() % 24) + 1); //generates random position

            //checks if position is available
            if (inZone(&temppos, spawnPoint) || inZone(&temppos, endPoint) || inZone(&temppos, safeZone))
            {
                valid = false;
            }

        } while (occupied(&temppos) != nullptr || valid == false); //conditions for position to be invalid

        //creates cctv objects and assigns its position, rotation(CW/ACW) and initial direction(radar/lineofsight)
        for (int c = 0; c < CCTVLimit; c++)
        {
            if (CCTVs[c] == nullptr)
            {
                CCTVs[c] = new CCTV((rand() % 4) + 1, (rand() % 2));
                entities[c + 61] = CCTVs[c];
                CCTVs[c]->set_pos(temppos.get_x(), temppos.get_y());
                CCTVs[c]->setCD(1 / g_dDeltaTime); //cooldown before rotation
                CCTVs[c]->update_cctv(); //rotate and set all radar pos
                break;
            }
        }

    }
}

void horrorFreeze(bool on)
{
    horror = on ? true : false;
    float changeinspd;
    if (on)
    {
        changeinspd = 0.1;
    }
    else
    {
        changeinspd = 10;
    }

    for (int i = 0; i < NPCLimit; i++)
    {
        if (NPCs[i] != nullptr)
        {
            NPCs[i]->set_speed(NPCs[i]->get_speed() * changeinspd);
        }
    }

    invert = on;
}

bool is_empty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

void updateScore(std::string fileName, int score, int* sessionBest)
{
    std::string prevScore;
    std::ifstream file(fileName);
    if (file.is_open()) //check if file is successfully opened
    {
        std::getline(file, prevScore);//get the previous highscore and store in this temp string
        file.close();
        if (level > std::stoi(prevScore))
        {
            std::ofstream file(fileName);
            if (file.is_open())
            {
                file << std::to_string(score);
                file.close();
            }
        }
    }
    if (score > *sessionBest)//in case theres problem opening that file
    {
        *sessionBest = score;
    }
}

void updateScore(std::string fileName, double score, double* sessionBest)
{
    std::string prevScore;
    std::ifstream file(fileName);
    if (file.is_open()) //check if file is successfully opened
    {
        std::getline(file, prevScore);//get the previous highscore and store in this temp string
        file.close();
        if (level > std::stod(prevScore))
        {
            std::ofstream file(fileName);
            if (file.is_open())
            {
                file << std::to_string(score);
                file.close();
            }
        }
    }
    if (score > * sessionBest)//in case theres problem opening that file
    {
        *sessionBest = score;
    }
}

void initStoredData(std::string fileName, double* data)
{
    std::ifstream file(fileName);
    if (is_empty(file))
    {
        file.close();
        std::ofstream file(fileName);
        if (file.is_open())
        {
            file << std::to_string(0);
            file.close();
        }
    }
    else
    {
        std::string temp;
        std::getline(file, temp);
        *data = std::stod(temp);
        file.close();
    }
}

void initStoredData(std::string fileName, int* data)
{
    std::ifstream file(fileName);
    if (is_empty(file))
    {
        file.close();
        std::ofstream file(fileName);
        if (file.is_open())
        {
            file << std::to_string(0);
            file.close();
        }
    }
    else
    {
        std::string temp;
        std::getline(file, temp);
        *data = std::stoi(temp);
        file.close();
    }
}

//void playSound(std::string filename, std::string filetype, bool loop)
//{
//    
//    
//    std::string songFile = "media/" + filename + '.' + filetype;
//
//    engine->play2D("songFile", loop);
//}

void muteBGM()
{
    if (g_skKeyEvent[K_M].keyReleased)
    {
        //for all sound, use bool array to store state of M key, if true, drop engine, kill sound, if false, create engine, sound functions should all work
        engine->drop();
    }
}