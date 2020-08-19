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

//FOR TESTING
bool checkInputs = false;
bool checkTimeElapsed = false;
bool checkFramerate = true;

std::string gameName = "zsdfghjk";

double  g_dElapsedTime;
double  g_dDeltaTime;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state

//Player* player = new Player;
//Entity* ePlayer = player;
Entity* entities[11] = { new Player , nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
const int entityLimit = 11;
NPC* NPCs[10] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
const int NPCLimit = 10;

Projectile* projectile[3] = { nullptr, nullptr, nullptr };
int particle_limit = 3;

//Object box(5, 5, Position(3, 8));

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
    g_eGameState = S_SPLASHSCREEN;

    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;
    entities[0]->set_pos(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y);
    g_sChar.m_bActive = true;
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler);

    spawnNPC(false, 5, 0.1);

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
            delete projectile[p];
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
    case S_SPLASHSCREEN: // don't handle anything for the splash screen
        break;
    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
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
    case S_SPLASHSCREEN: // don't handle anything for the splash screen
        break;
    case S_GAME: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
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
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
        case S_GAME: updateGame(); // gameplay logic when we are in the game
            break;
    }
}


void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 10) // wait for 0.5 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
}

void updateGame()       // gameplay logic
{
    
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
    
}

void moveCharacter()
{   
    // Updating the location of the character based on the key release
    // providing a beep sound whenver we shift the character
    if (getButtonHold() == K_W && g_sChar.m_cLocation.Y > 0)
    {
        //Beep(1440, 30);
        entities[0]->set_direction(1);
        //g_sChar.m_cLocation.Y = 13;
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
    
    if (g_skKeyEvent[K_SPACE].keyReleased)
    {
        for (int p = 0; p < particle_limit; p++)
        {
            if (projectile[p] == nullptr)
            {
                projectile[p] = new Projectile;
            }

            projectile[p]->set_ppos(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y);
        }

        g_sChar.m_bActive = !g_sChar.m_bActive;
    }

    entities[0]->update_pos();
    g_sChar.m_cLocation.Y = entities[0]->getposy();
    g_sChar.m_cLocation.X = entities[0]->getposx();

    moveall();
    

}

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
        g_bQuitGame = true;    
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
    case S_SPLASHSCREEN: renderMainMenu();
        break;
    case S_GAME: renderGame();
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
}

void renderMap()
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    /*COORD c;
    for (int i = 0; i < 12; ++i)
    {
        c.X = 5 * i;
        c.Y = i + 1;
        colour(colors[i]);
        g_Console.writeToBuffer(c, " ����", colors[i]);
        
    }*/

    renderNPC();
    renderprojectile();
    
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0C;
    if (g_sChar.m_bActive)
    {
        charColor = 0x0A;
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

            if (NPCs[i]->isHostile())
            {
                colour = 0x4D;
            }
            else
            {
                colour = 0xF6;
            }

            g_Console.writeToBuffer(c, " ", colour);
        }
    }
    
}

void spawnNPC(bool isPolice, int no, float spd)
{
    for (int i = 0; i < no; i++)
    {
        Position temp;
        do
        {
            temp.set_x(rand() % 80);
            temp.set_y(rand() % 24);
        } while (occupied(&temp) != nullptr); //while pos is not available

        
              
        for (int n = 0; n < NPCLimit; n++)
        { 
            if (NPCs[n] == nullptr)
            {
                if (isPolice)
                {
                    NPCs[n] = new Police;
                    entities[n + 1] = NPCs[n];
                }
                else
                {
                    NPCs[n] = new NPC;
                    entities[n + 1] = NPCs[n];
                }
                entities[n + 1]->set_pos(temp.get_x(), temp.get_y());
                NPCs[n]->set_speed(spd);
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
            
            if (NPCs[i]->get_count() < 300 && NPCs[i]->isHostile() == false)
            {
                NPCs[i]->set_count(NPCs[i]->get_count() + 1);

                if (NPCs[i]->get_count() > 200)
                {
                    NPCs[i]->set_direction(0);
                }
            }
            else //count = 300
            {

                NPCs[i]->set_count(0);

                if (NPCs[i]->isHostile() == false)
                {
                    

                    int aaa = (rand() % 4) + 1;
                    switch (aaa)
                    {
                    case 1:
                        NPCs[i]->set_direction(1);
                        break;
                    case 2:
                        NPCs[i]->set_direction(2);
                        break;
                    case 3:
                        NPCs[i]->set_direction(3);
                        break;
                    case 4:
                        NPCs[i]->set_direction(4);
                        break;
                    default:
                        break;
                    }

                    
                }
                else //is hostile
                {
                    int diffinx = g_sChar.m_cLocation.X - NPCs[i]->getposx();
                    int diffiny = g_sChar.m_cLocation.Y - NPCs[i]->getposy();

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
                }
            }
            NPCs[i]->update_pos();

           
    
               
        }
        
    }
}

Entity* occupied(Position* pos)
{
    for (int i = 0; i < entityLimit; i++)
    {
        if (entities[i] != nullptr)
        {
            if (entities[i]->getpos()->get_x() == pos->get_x() && entities[i]->getpos()->get_y() == pos->get_y())
            {
                return entities[i];
            }
        }
    }
    return nullptr;
}

void renderprojectile()
{
    COORD c;
    int colour;
    for (int p = 0; p < particle_limit; p++)
    {
        if (projectile[p] != nullptr)
        {
            projectile[p]->direction(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y);
            projectile[p]->update_particle();

            c.X = projectile[p]->get_px();
            c.Y = projectile[p]->get_py();

            colour = 0x6F;

            g_Console.writeToBuffer(c, " ", colour);
        }
    }
}

void renderBox(Object* box, int colour, std::string text = " ")
{
    COORD c;
    c.Y = box->referencePosition()->get_y();
    for (int y = 0; y < box->height(); y++)
    {
        c.X = box->referencePosition()->get_x();
        for (int x = 0; x < box->length(); x++)
        {   
            c.X++;
            g_Console.writeToBuffer(c, text, colour);
        }
        c.Y++;
    }
}

void renderMainMenu()
{
    COORD c = g_Console.getConsoleSize();
    Object text(gameName.length(), 1, Position(c.X / 2, c.Y / 5));
    renderBox(&text, 0x0F, gameName);
}
