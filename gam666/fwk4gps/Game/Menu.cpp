#include "Menu.h"

#include "..\iSound.h"      // for the Sound Interface
#include "..\iText.h"       // for the Text Interface
#include "..\iHUD.h"        // for the HUD Interface
#include "..\iLight.h"      // for the Light Interface
#include "..\iObject.h"     // for the Object Interface
#include "..\iTexture.h"    // for the Texture Interface
#include "..\Camera.h"      // for the Camera class
#include "..\iGraphic.h"    // for the Graphic Interface
#include "..\MathDef.h"     // for Matrix operations
#include "..\Model.h"       // for ROLL_SPEED
#include "..\Translation.h" // for Action enumerations
#include "..\APIDisplay.h" // for Viewport
#include "..\APIUserInput.h" // for Viewport
#include "..\iAPIWindow.h"

#include "Game.h"
#include "GameObject.h"
#include "..\Utils.h"
#include "Map.h"
#include "btBulletDynamicsCommon.h"
#include "World.h"

#include "GameObjects\Player.h"
#include "GameObjects\Floor.h"

#include "Mesh.h"
#include "Utilities\ObjImporter.h"

Menu::Menu(Game* game) : Coordinator(game->apiObjects), 
  game(game), numberOfPlayers(1){    
}

void Menu::initialize() {    
  loadingScreen();
  numberOfPlayers = userInput->getDeviceCount(CONTROLLER);
  if (!numberOfPlayers) numberOfPlayers = 1;  
  farcp = 10000.0f;
  nearcp = 80.0f;
  initializeHUD();
  createProjection();  
  showCursor(true);
  currentItem = 0;
  currentLevel = 0;
  isMovingUp = isMovingDown = isSelected = false;
  levelList.push_back("DefaultMap");
  //levelList.push_back("NonexistantMap");  
}

void Menu::loadingScreen() {
  Object* bg = (Object*) ::CreateSprite(CreateGraphic(width, height));
  bg->attach(CreateTexture(L"blue_nebula.jpg"));
  Object* guy = (Object*) CreateSprite(L"gundam-style-main.png", Vector(10, 0), 0.5f);
  Object* title = (Object*) CreateSprite(L"gundam-style-text.png", Vector(-40, 40), 0.5f);
  title->alignRight();
  display->beginDrawFrame(&view);  
  bg->render();
  guy->render();  
  title->render();  
  display->endDrawFrame();
  present();
}

void Menu::initializeHUD() {	  	
  iHUD* hud = CreateHUD(CreateGraphic(), 0.0f, 0.6f, 0.99f, 0.4f, nullptr);  
  //hud->toggle();
  iText* testMe = CreateText(Rectf(0.0f, 0, 1.0f, 0.16f), hud, L"START GAME", 32, L"ARIAL", TEXT_TOP | TEXT_NORMAL | TEXT_CENTER);
  menuItem.push_back(testMe);
  //testMe = CreateText(Rectf(0.0f, 0.20f, 1.0f, 0.35f), hud, L"LEVEL: DefaultMap", 32, L"ARIAL", TEXT_TOP | TEXT_NORMAL | TEXT_CENTER);
  testMe = CreateText(Rectf(0.0f, 0.20f, 1.0f, 0.35f), hud, L"OPTIONS", 32, L"ARIAL", TEXT_TOP | TEXT_NORMAL | TEXT_CENTER);
  menuItem.push_back(testMe);
  testMe = CreateText(Rectf(0.0f, 0.40f, 1.0f, 0.55f), hud, L"QUIT", 32, L"ARIAL", TEXT_TOP | TEXT_NORMAL | TEXT_CENTER);
  menuItem.push_back(testMe);
  menuItem.at(0)->setColour(TEXT_YELLOW);
}

//handle menu inputs
void Menu::updateMenu() {
	int testKey = userInput->getDeviceCount(KEYBOARD);
	int testCon = userInput->getDeviceCount(CONTROLLER);
	int testPoint = userInput->getDeviceCount(POINTER);
	bool testInput = userInput->pressed(HUD_SELECT);
	char tmpLevel[80] = "LEVEL: ";
  int changeX = change(POSX);
  int changeY = change(POSY);
  bool buttonPressed = pressed(FIRE_WEAPON_1) || pressed(THRUSTER);

	if (buttonPressed && !isSelected) {		
		isSelected = true;
		switch (currentItem) {
			case 0:
				game->addState(new World(game, DefaultMap()));
				break;
			case 1:
				/*(currentLevel == levelList.size() - 1) ? currentLevel = 0 : currentLevel++;
				strcat (tmpLevel, levelList.at(currentLevel));
				menuItem.at(currentItem)->setLabel(tmpLevel);				*/
				suspend();
				reset();
				loadingScreen();
				restore();
				break;
			case 2:				
				terminate = true;
				break;
		}		
	}
	if (!buttonPressed && isSelected)
		isSelected = false;
  bool forward = (pressed(MOVE_FORWARD) || changeY < -50),
       backward = (pressed(MOVE_BACKWARD) || changeY > 50);
	if (forward && !isMovingUp) {		
		isMovingUp = true;
		//userInput->release(MOVE_FORWARD);
		menuItem.at(currentItem)->setColour(TEXT_COLOUR);
		(currentItem == 0) ? currentItem = menuItem.size() - 1 : currentItem--;
		menuItem.at(currentItem)->setColour(TEXT_YELLOW);
	}
	if (!forward && isMovingUp)
		isMovingUp = false;

	if (backward && !isMovingDown) {
		isMovingDown = true;
		menuItem.at(currentItem)->setColour(TEXT_COLOUR);
		(currentItem == menuItem.size() - 1) ? currentItem = 0 : currentItem++;
		menuItem.at(currentItem)->setColour(TEXT_YELLOW);
	}
	if (!backward && isMovingDown)
		isMovingDown = false;
}

void Menu::render() {
  updateMenu();  
  Coordinator::render();  
}

iObject* Menu::CreateSprite(const wchar_t* file, const Vector& position, float scale, unsigned char a) {
  iTexture* tex = CreateTexture(file);
  iObject* sprite = ::CreateSprite(CreateGraphic((int)(tex->getWidth() * scale), (int)(tex->getHeight() * scale)));
  sprite->attach(tex);
  sprite->translate(position.x, position.y, 0);
  sprites.push_back(sprite);
  return sprite;
}

Menu::~Menu() { 
}