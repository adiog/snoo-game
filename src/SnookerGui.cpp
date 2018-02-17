#include "SnookerGui.h"

#include <sstream>
#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>

#include "SnookerApplication.h"
#include "Callbacks.h"
#include "Menu.h"
#include "MenuItemLabel.h"
#include "MenuItemBool.h"
#include "MenuItemEdit.h"
#include "MenuItemPassword.h"
#include "MenuItemCommand.h"
#include "MenuItemSubmenu.h"
#include "OverlayCommons.h"
#include "Id.h"
#include "SnooOverlayManager.h"

using namespace Ogre;

SnookerGui::SnookerGui():
    openedMenu(NULL),
    mainMenu(NULL),
    selectedDecision(0)
{
}

SnookerGui::~SnookerGui()
{
}

bool SnookerGui::injectKey(OIS::KeyCode code)
{
    SnookerApplication & application = SnookerApplication::getSingleton();
    
    if (!popups.empty() && (code == OIS::KC_RETURN || code == OIS::KC_ESCAPE)) {
        delete popups.top();
        popups.pop();
        return true;
    }

    // jesli mamy popupa, to ignorujemy reszte klawiszy
    if (!popups.empty())
        return true;

    if (openedMenu != NULL) {
        openedMenu->injectKey(code);
        return true;
    }

    // Gora - dol w decycjach
    if (code == OIS::KC_DOWN && application.canDecide() && decisions.size() > 0) {
        selectedDecision++;
        if (selectedDecision >= (int)decisions.size())
            selectedDecision = -1;
        return true;
    }
    
    if (code == OIS::KC_UP && application.canDecide() && decisions.size() > 0) {
        selectedDecision--;
        if (selectedDecision < -1)
            selectedDecision = decisions.size() - 1;
        return true;
    }

    // Gora - dol w liscie gier
    if (code == OIS::KC_UP &&
            application.clientState == SnookerApplication::STATE_CONNECTED &&
            gameListShown) {
#ifdef CLIENT_WITH_SPECTATE
        if (selectedGame >= 0) {
            if (selectedColumn == 0 || (selectedColumn == 1 && (!games[selectedGame].joinable))) {
                selectedGame = selectedGame - 1;
                selectedColumn = 1;
            } else
                selectedColumn = 0;
        } else {
            selectedGame = games.size() - 1;
            selectedColumn = 1;
        }
#else
        do {
            if (selectedGame >= 0)
                selectedGame--;
            else
                selectedGame = games.size() - 1;
        } while (selectedGame != -1 && !games[selectedGame].joinable);
        selectedColumn = 0;
#endif
        refreshGameListPosition();
        return true;
    }
    
    if (code == OIS::KC_DOWN &&
            application.clientState == SnookerApplication::STATE_CONNECTED &&
            gameListShown) {
#ifdef CLIENT_WITH_SPECTATE
        if (selectedGame == -1) {
            selectedGame = 0;
            selectedColumn = 0;
        } else {
            if (selectedColumn == 0)
                selectedColumn = 1;
            else {
                selectedGame = selectedGame + 1;
                selectedColumn = 0;
            }
        }
        if (selectedGame >= (int)games.size())
            selectedGame = -1; 
#else
        bool anyJoinableGame = false;
        for (unsigned i = 0; i < games.size(); i++)
            if (games[i].joinable)
                anyJoinableGame = true;

        do {
            selectedGame++;
            if (selectedGame >= (int)games.size())
                selectedGame = -1; 
        } while (selectedGame != -1 && !games[selectedGame].joinable);
        selectedColumn = 0;
#endif
        refreshGameListPosition();
        return true;
    }

    // Enter w liscie decyzji
    if (code == OIS::KC_RETURN && application.canDecide() &&
            decisions.size() > 0 && selectedDecision >= 0) {
        application.applyDecision(decisions[selectedDecision]);
        return true;
    }

    // Enter w liscie gier
    if (code == OIS::KC_RETURN &&
            application.clientState == SnookerApplication::STATE_CONNECTED &&
            gameListShown) {
        if (selectedGame == -1)
            application.createRoom();
        else if (selectedColumn == 0)
            application.joinGame(games[selectedGame].id);
        else
            application.spectateGame(games[selectedGame].id);
        return true;
    }


    // Pisanie wiadomości
    bool shift = application.getKeyboard()->isKeyDown(OIS::KC_LSHIFT) ||
        application.getKeyboard()->isKeyDown(OIS::KC_RSHIFT);
    if (message.injectKey(code, shift)) {
        updateChatMessage();
        return true;
    }

    // Wysyłanie wiadomości
    if (code == OIS::KC_RETURN && message.getString().size() > 0) {
        if (application.client.isConnected()) {
            std::string s = "<" + application.settings.login + "> " + message.getString();
            chatMessage(s);
            application.client.sendChatMessage(s);
            message.setString("");
            updateChatMessage();
        }
        return true;
    }

    return false;
}

void SnookerGui::initialize(void)
{
    OverlayManager * overlayManager = SnooOverlayManager::getSingletonPtr();
    SnookerApplication & application = SnookerApplication::getSingleton();
    
    createMainMenu();
    
    overlayManager->getByName("Snoo/GUI")->show();
    OverlayElement * guiPanel = overlayManager->getOverlayElement("GUI");
    guiPanel->setLeft((application.getWidth() - guiPanel->getWidth()) / 2);
    guiPanel->setTop(application.getHeight() - guiPanel->getHeight() - 20);
    getPanel("GUI/Decisions")->hide();

    updateChatMessage();
    gameStateChanged();
    clientStateChanged();
}

void SnookerGui::createMainMenu(void)
{
    SnookerApplication & application = SnookerApplication::getSingleton();

    Menu * optMenu = new Menu("OptMenu");
    optMenu->addMenuItem(new MenuItemCommand(
                "omWireframe", "Toggle wireframe mode", cb_wireframe));
    optMenu->addMenuItem(new MenuItemLabel(
                "omGameServer", "Game Server:"));
    optMenu->addMenuItem(new MenuItemEdit(
                "omGameServerE", &application.settings.gameServer));
    
    optMenu->addMenuItem(new MenuItemLabel(
                "omGamePort", "Port:"));
    optMenu->addMenuItem(new MenuItemEdit(
                "omGamePortE", &application.settings.gamePort));
    
    optMenu->addMenuItem(new MenuItemLabel(
                "omGameLogin", "Login:"));
    optMenu->addMenuItem(new MenuItemEdit(
                "omGameLoginE", &application.settings.login));
   
    optMenu->addMenuItem(new MenuItemLabel(
                "omGamePassword", "Password:"));
    optMenu->addMenuItem(new MenuItemPassword(
                "omGamePasswordE", &application.settings.password));
#ifdef CLIENT_WITH_CUE
    optMenu->addMenuItem(new MenuItemLabel(
                "omSeparator", "------------------------"));
    
    optMenu->addMenuItem(new MenuItemLabel(
                "omCueServer", "Cue Server:"));
    optMenu->addMenuItem(new MenuItemEdit(
                "omCueServerE", &application.settings.cueServer));
    
    optMenu->addMenuItem(new MenuItemLabel(
                "omCuePort", "Port:"));
    optMenu->addMenuItem(new MenuItemEdit(
                "omCuePortE", &application.settings.cuePort));
    optMenu->addMenuItem(new MenuItemCommand(
                "omConnect", "Connect with cue...", cb_cueConnect));
#endif

    mainMenu = new Menu("MainMenu");
    mainMenu->addMenuItem(new MenuItemLabel(
                "mmSnoo", "Snoo"));
    mainMenu->addMenuItem(new MenuItemCommand(
                "mmNewGame", "Start match", cb_newGame));
    mainMenu->addMenuItem(new MenuItemCommand(
                "mmTraining", "Start training", cb_training));
    mainMenu->addMenuItem(new MenuItemCommand(
                "mmPlayOnline", "Play online", cb_connect));
    mainMenu->addMenuItem(new MenuItemCommand(
                "mmHelp", "Help", cb_help));
    mainMenu->addMenuItem(new MenuItemSubmenu(
                "mmOptions", "Options...", optMenu));
    mainMenu->addMenuItem(new MenuItemCommand(
                "mmQuit", "Quit", cb_closeApplication));
}

void SnookerGui::openMainMenu(void)
{
    if (openedMenu == NULL)
        mainMenu->open(&openedMenu); 
}

void SnookerGui::showMessage(std::string title, std::string text)
{
    popups.push(new Popup(title, text));
}

void SnookerGui::setLeftStatus(std::string text)
{
    OverlayManager * manager = SnooOverlayManager::getSingletonPtr();
    TextAreaOverlayElement * el = dynamic_cast<TextAreaOverlayElement*>(
            manager->getOverlayElement("GUI/StatusTextLeft"));
    el->setCaption(text);
}

void SnookerGui::setRightStatus(std::string text)
{
    OverlayManager * manager = SnooOverlayManager::getSingletonPtr();
    TextAreaOverlayElement * el = dynamic_cast<TextAreaOverlayElement*>(
            manager->getOverlayElement("GUI/StatusTextRight"));
    el->setCaption(text);
}

void SnookerGui::chatMessage(std::string chatMessage)
{
    chat.push_back(chatMessage);
    while (chat.size() > 6)
        chat.erase(chat.begin());

    std::ostringstream ss;
    for (unsigned int i = 0; i < chat.size(); i++)
        ss << chat[i] << "\n";
    getTextArea("GUI/Chat")->setCaption(ss.str());

}

void SnookerGui::updateChatMessage(void)
{
    getTextArea("GUI/Message")->setCaption("> " + message.getString());
}

void SnookerGui::updateDecisions(void)
{
    if (decisions.empty() || !SnookerApplication::getSingleton().canDecide()) {
        getPanel("GUI/Decisions")->hide();
        selectedDecision = -1;
        return;
    }

    std::string dec, dec_h;
    for (unsigned int i = 0; i < decisions.size(); i++)
        if (i == (unsigned)selectedDecision) {
            dec_h += (decisions[i].getDescription() + "\n");
            dec += "\n";
        } else {
            dec += (decisions[i].getDescription() + "\n");
            dec_h += "\n";
        }
    getPanel("GUI/Decisions")->show();
    getPanel("GUI/Decisions")->setHeight((decisions.size() + 1)*16 + 5);
    getTextArea("GUI/DecisionList")->setCaption(dec);
    getTextArea("GUI/DecisionListHi")->setCaption(dec_h);
}

void SnookerGui::gameStateChanged(void)
{
    SnookerApplication & application = SnookerApplication::getSingleton();
    SnookerModel & model = application.model;
    GameState * state = application.model.gameState;
    
    getTextArea("GUI/Frames0")->setCaption(intToString(
                model.result.frames[0]));
    getTextArea("GUI/Frames1")->setCaption(intToString(
                model.result.frames[1]));
    getTextArea("GUI/Points0")->setCaption(
            intToString(model.result.points[0]) + "  " + player1);
    getTextArea("GUI/Points1")->setCaption(
            player2 + "  " + intToString(model.result.points[1]));


    getPanel("GUI/LeftTriangle")->hide();
    getPanel("GUI/RightTriangle")->hide();

    if (application.anyonePlays())
    {
        setLeftStatus(state->description());

        if (state->getPlayer().getId() == 0)
            getPanel("GUI/LeftTriangle")->show();
       
        if (state->getPlayer().getId() == 1)
            getPanel("GUI/RightTriangle")->show();
       
        if (application.model.gameState->canMoveWhiteBall())
            setRightStatus("Ball in hand");
        else if (application.model.result.currentBreak > 0)
            setRightStatus("Break: " + intToString(model.result.currentBreak));
        else
            setRightStatus("");
    } else {
            setRightStatus("");
            setLeftStatus("");
    }

    decisions = state->getAvailableDecisions(application.model);
    selectedDecision = -1;
}

void SnookerGui::clientStateChanged(void)
{
    SnookerApplication & application = SnookerApplication::getSingleton();

    switch (application.clientState) {
        case SnookerApplication::STATE_PVP_GAME:
        case SnookerApplication::STATE_AI_GAME:
            getPanel("GUI/GameList")->hide();
            getPanel("GUI/Communication")->hide();
            getPanel("GUI/BallPointer")->show();
            getPanel("GUI/BlueStripe")->show();
            player1 = "Adam";
            player2 = "Eve";
            break;

        case SnookerApplication::STATE_TRAINING:
            getPanel("GUI/GameList")->hide();
            getPanel("GUI/Communication")->hide();
            getPanel("GUI/BallPointer")->show();
            getPanel("GUI/BlueStripe")->hide();
            break;
        
        case SnookerApplication::STATE_CONNECTING:
            getPanel("GUI/GameList")->show();
            getPanel("GUI/GameListTable")->hide();
            getPanel("GUI/Communication")->hide();
            getPanel("GUI/BallPointer")->hide();
            getPanel("GUI/BlueStripe")->hide();
            getTextArea("GUI/GameListStatus")->show();
            getTextArea("GUI/GameListStatus")->setCaption("Connecting...");
            break;
        
        case SnookerApplication::STATE_VERIFYING:
            getPanel("GUI/GameList")->show();
            getPanel("GUI/GameListTable")->hide();
            getPanel("GUI/Communication")->hide();
            getPanel("GUI/BallPointer")->hide();
            getPanel("GUI/BlueStripe")->hide();
            getTextArea("GUI/GameListStatus")->show();
            getTextArea("GUI/GameListStatus")->setCaption("Logging in...");
            break;
        
        case SnookerApplication::STATE_CONNECTED:
            getPanel("GUI/GameList")->show();
            getPanel("GUI/GameListTable")->hide();
            getPanel("GUI/Communication")->hide();
            getPanel("GUI/BallPointer")->hide();
            getPanel("GUI/BlueStripe")->hide();
            gameListShown = false; // Jeszcze nie ma listy, sciaga sie
            getTextArea("GUI/GameListStatus")->show();
            getTextArea("GUI/GameListStatus")->setCaption("Loading...");
            break;
        
        case SnookerApplication::STATE_CREATING_GAME:
            getPanel("GUI/GameList")->show();
            getPanel("GUI/GameListTable")->hide();
            getPanel("GUI/Communication")->hide();
            getPanel("GUI/BallPointer")->hide();
            getPanel("GUI/BlueStripe")->hide();
            getTextArea("GUI/GameListStatus")->show();
            getTextArea("GUI/GameListStatus")->setCaption("Creating game...");
            break;
        
        case SnookerApplication::STATE_WAITING_PLAYERS:
            getPanel("GUI/GameList")->show();
            getPanel("GUI/GameListTable")->hide();
            getPanel("GUI/Communication")->hide();
            getPanel("GUI/BallPointer")->hide();
            getPanel("GUI/BlueStripe")->hide();
            getTextArea("GUI/GameListStatus")->show();
            getTextArea("GUI/GameListStatus")->setCaption("Waiting for opponent");
            break;
        
        case SnookerApplication::STATE_GAME_STARTS:
            getPanel("GUI/GameList")->hide();
            getPanel("GUI/Communication")->show();
            getPanel("GUI/BallPointer")->show();
            getPanel("GUI/BlueStripe")->show();
            break;
        
        case SnookerApplication::STATE_JOINING_AS_PLAYER:
        case SnookerApplication::STATE_JOINING_AS_SPECTATOR:
            getPanel("GUI/GameList")->show();
            getPanel("GUI/GameListTable")->hide();
            getPanel("GUI/Communication")->hide();
            getPanel("GUI/BallPointer")->hide();
            getPanel("GUI/BlueStripe")->hide();
            getTextArea("GUI/GameListStatus")->show();
            getTextArea("GUI/GameListStatus")->setCaption("Joining game...");
            break;
        
        case SnookerApplication::STATE_MY_MOVE:
            getPanel("GUI/GameList")->hide();
            getPanel("GUI/Communication")->show();
            getPanel("GUI/BallPointer")->show();
            getPanel("GUI/BlueStripe")->show();
            break;
        
        case SnookerApplication::STATE_WAIT_FOR_MOVE:
            getPanel("GUI/GameList")->hide();
            getPanel("GUI/Communication")->show();
            getPanel("GUI/BallPointer")->show();
            getPanel("GUI/BlueStripe")->show();
            break;
        
        case SnookerApplication::STATE_WAIT_FOR_STOP:
            getPanel("GUI/GameList")->hide();
            getPanel("GUI/Communication")->show();
            getPanel("GUI/BallPointer")->hide();
            getPanel("GUI/BlueStripe")->show();
            break;
        
        case SnookerApplication::STATE_WAIT_FOR_POSITIONS:
            getPanel("GUI/GameList")->hide();
            getPanel("GUI/Communication")->show();
            getPanel("GUI/BallPointer")->hide();
            getPanel("GUI/BlueStripe")->show();
            break;
        
        case SnookerApplication::STATE_END_OF_GAME:
            getPanel("GUI/GameList")->hide();
            getPanel("GUI/Communication")->hide();
            getPanel("GUI/BallPointer")->hide();
            getPanel("GUI/BlueStripe")->show();
            showMessage("Snoo", "End of the game");
            break;

        case SnookerApplication::STATE_END_OF_NETWORK_GAME:
            getPanel("GUI/GameList")->hide();
            getPanel("GUI/Communication")->show();
            getPanel("GUI/BallPointer")->hide();
            getPanel("GUI/BlueStripe")->show();
            showMessage("Snoo", "End of the game");
            break;
 
        default:
            break;
    }
    
    gameStateChanged();
}
        
void SnookerGui::closeMenu(void)
{
    if (openedMenu != NULL)
        openedMenu->close();
}

void SnookerGui::refreshGameListPosition()
{
    if (selectedGame >= (int)games.size()) {
        selectedDecision = games.size() - 1;
        selectedColumn = 0; // join
    }

    if (selectedGame >= 0 &&
            (!games[selectedGame].joinable) &&
            selectedColumn == 0)
        selectedColumn = 1;

    std::string j_on, j_off, s_on, s_off;
    for (unsigned int i = 0; i < games.size(); i++)
        if ((int)i == selectedGame) {
            if (selectedColumn == 0) {
                // wybrano join
                j_off += "\n";
                j_on += "Join\n";
                s_on += "\n";
                s_off += "Spectate\n";
            } else {
                // wybrano spectate
                s_on += "Spectate\n";
                s_off += "\n";
                j_on += "\n";
                if (games[i].joinable)
                    j_off += "Join\n";
                else
                    j_on += "\n";
            }
        } else {
            s_on  += "\n";
            s_off += "Spectate\n";
            j_on += "\n";
            if (games[i].joinable)
                j_off += "Join\n";
            else
                j_off += "\n";
        }
#ifndef CLIENT_WITH_SPECTATE
    s_on = "";
    s_off = "";
#endif

    getTextArea("GUI/GameListJoinOn")->setCaption(j_on);
    getTextArea("GUI/GameListJoinOff")->setCaption(j_off);
    getTextArea("GUI/GameListSpectateOn")->setCaption(s_on);
    getTextArea("GUI/GameListSpectateOff")->setCaption(s_off);

    getTextArea("GUI/GameListCreateOn")->hide();
    getTextArea("GUI/GameListCreateOff")->hide();
    if (selectedGame == -1)
        getTextArea("GUI/GameListCreateOn")->show();
    else
        getTextArea("GUI/GameListCreateOff")->show();
    
    if (games.size() == 0) {
        getTextArea("GUI/GameListStatus")->show();
        getTextArea("GUI/GameListStatus")->setCaption("No rooms. Press Enter to create your own.");
    } else
        getTextArea("GUI/GameListStatus")->hide();
}

void SnookerGui::setGameList(std::vector<std::string> gameList)
{
    games.erase(games.begin(), games.end());
    for (unsigned int i = 0; i < gameList.size(); i++) {
        std::istringstream ss(gameList[i]);
        GameData data;
        ss >> data.id >> data.p1 >> data.p2;
        data.joinable = false;
        if (data.p1 == "???") {
            data.p1 = "-----";
            data.joinable = true;
        }
        
        if (data.p2 == "???") {
            data.p2 = "-----";
            data.joinable = true;
        }
        games.push_back(data);
    }
   
    if (!gameListShown) {
        // Po raz pierwszy pokazujemy
        selectedGame = -1;
        gameListShown = true;
    }
    
    std::string id, p1, p2;
    for (unsigned int i = 0; i < games.size(); i++) {
        id += intToString(games[i].id) + "\n";
        p1 += games[i].p1 + "\n";
        p2 += games[i].p2 + "\n";
    }

    getTextArea("GUI/GameListStatus")->hide();
    getPanel("GUI/GameListTable")->show();
    getTextArea("GUI/GameListID")->setCaption(id);
    getTextArea("GUI/GameListP1")->setCaption(p1);
    getTextArea("GUI/GameListP2")->setCaption(p2);
    refreshGameListPosition();
}

void SnookerGui::setPlayerList(std::vector<std::string> players)
{
    player1 = (players[0] != "???" ? players[0] : "-----");
    player2 = (players[1] != "???" ? players[1] : "-----");

    std::string allPlayers;
    for (unsigned int i = 0; i < players.size(); i++)
        allPlayers += players[i] + "\n";

    getTextArea("GUI/PlayerList")->setCaption(allPlayers);
    gameStateChanged();
}

void SnookerGui::update(void)
{
    SnookerApplication & application = SnookerApplication::getSingleton();

    getPanel("GUI/RedDot")->setPosition(
        52 + application.model.whiteBallPoint[0] * 40,
        52 + application.model.whiteBallPoint[1] * 40);

    if (application.replayInProgress)
        getTextArea("GUI/Replay")->show();
    else
        getTextArea("GUI/Replay")->hide();

    updateDecisions();
}
