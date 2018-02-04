#include "SnookerApplication.h"

#include <iostream>
#include <sstream>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreFrameListener.h>
#include <OGRE/OgreCamera.h>

#include "SnookerFrameListener.h"
#include "SnookerScene.h"
#include "Physics.h"
#include "Callbacks.h"
#include "SnookerSettings.h"
#include "RestoreBall.h"
#include "acc_protocol.h"
#include "sleep.h"
#include "Converter.h"
#include "SnookerClient.h"
#include "SnookerMessage.h"
#include "GameStateFactory.h"
#include "GameStateTraining.h"
#include "GameStateInformation.h"
#include "Runtime.h"


using namespace Ogre;
using namespace network;

SnookerApplication::SnookerApplication()
{
    clientState = STATE_PVP_GAME;
    me = Player(0);
    lastViewInfo = 0.0;
    lastWhiteBallInfo = 0.0;
    wasWhiteBallMoved = false;
    lastGameList = 0.0;
    slowFactor = 1.0;
    
    cameraMode = CAMERA_MODE_DYNAMIC;
    cameraPosition = startPosition = goalPosition = BROWN_SPOT;
    cameraDirection = startDirection = goalDirection = BLUE_SPOT;

    screenshotId = 0;
    timeSinceStart = 0.0;
    buttonPressed = false;
    replayInProgress = false;
    
    
    physicsEngine.addListener(this);
    model.startNewGame();
}

SnookerApplication::~SnookerApplication()
{
    if (scene)
        delete scene;
}

SnookerApplication & SnookerApplication::getSingleton(void)
{
    static SnookerApplication application;
    return application;
}

void SnookerApplication::setServer(const std::string & addr, const std::string & p)
{
}

void SnookerApplication::createScene(void)
{
    scene = new SnookerScene(sceneManager, &model);
    gui.initialize();
    scene->setAccOvlVisible(true);
}

void SnookerApplication::createFrameListener(void)
{
    frameListener = new SnookerFrameListener(
            this, renderWindow, camera, eventListener, eventListener);
    root->addFrameListener(frameListener);
}

void SnookerApplication::showGameList(void)
{
    client.sendGamesListRequest();
}

void SnookerApplication::setClientState(ClientState state)
{
    clientState = state;
    gui.clientStateChanged();
}

void SnookerApplication::processMessage(SnookerMessage & message)
{
    switch (message.getType()) {
        case network::SnookerMessage::loginResponse:
            if (clientState != STATE_VERIFYING)
                std::cerr << "Login poniewczasie" << std::endl;
            if (message.getLoginConfirmation()) {
                setClientState(STATE_CONNECTED);
            } else {
                client.disconnect();
                setClientState(STATE_TRAINING);
                gui.showMessage("Error",
                        "Invalid login or password\n"
                        "Check this in Menu -> Options");
            }
            break;
        case network::SnookerMessage::gameListResponse:
            {
                std::vector<std::string> list = message.getGamesList();
                std::cout << "GAMES:" << std::endl;
                for (unsigned int i = 0; i < list.size(); i++)
                    std::cout << list[i] << std::endl;
                gui.setGameList(list);
            }
            break;
        case network::SnookerMessage::newGameResponse:
            client.sendParticipantsListRequest();
            me = Player(0);
            setClientState(STATE_WAITING_PLAYERS);
            break;
        case network::SnookerMessage::playerJoinedInfo:
            client.sendParticipantsListRequest();
            client.sendGameStartRequest();
            setClientState(STATE_GAME_STARTS);
            break;
        case network::SnookerMessage::playerLeftInfo:
        case network::SnookerMessage::playerQuitInfo:
            // TODO TODO czym sie roznia te 2 komunikaty
            client.sendParticipantsListRequest();
            if (networkGameInProgress() && clientState != STATE_END_OF_GAME)
                setClientState(STATE_END_OF_NETWORK_GAME);
            break;
        case network::SnookerMessage::spectatorLeftInfo:
            client.sendParticipantsListRequest();
            break;
        case network::SnookerMessage::joinGameResponse:
            // TODO a jak sie dolacza spectator?
            client.sendParticipantsListRequest();
            if (clientState == STATE_JOINING_AS_PLAYER)
                me = Player(1);
            else {
                std::cerr << "Joined as spectator!!!!" << std::endl;
                me = Player(2);
            }
            setClientState(STATE_GAME_STARTS);
            break;
        case network::SnookerMessage::participantsListResponse:
            gui.setPlayerList(message.getParticipiants());
            break;
        case network::SnookerMessage::gameStartInfo:
        case network::SnookerMessage::gameStartResponse:
            client.sendParticipantsListRequest();
            model.startNewGame();
            gui.gameStateChanged();
            break;
        case network::SnookerMessage::yourMoveInfo:
        case network::SnookerMessage::moveOfOtherPlayerInfo:
        case network::SnookerMessage::whiteBallMoveInfo:
        case network::SnookerMessage::shotDataInfo:
        case network::SnookerMessage::afterShotGameStateInfo:
        case network::SnookerMessage::afterNonShotGameStateInfo:
            messages.push(message);
            break;
        case network::SnookerMessage::chatMessage:
            gui.chatMessage(message.getChatMessage());
            break;
        case network::SnookerMessage::viewInfo:
            if (clientState == STATE_WAIT_FOR_MOVE && messages.empty()) {
                const double * viewInfo = (const double*) message.getBinaryDataPointer();
                model.vectorDirection = viewInfo[0];
                model.tilt = viewInfo[1];
                model.whiteBallPoint[0] = viewInfo[2];
                model.whiteBallPoint[1] = viewInfo[3];
            }
            break;
        case network::SnookerMessage::shutUp:
            std::cerr << "Shut up message" << std::endl;
            break;
        default:
            std::cerr << "Strange message:" << message.getType() << std::endl;
    }
}

void SnookerApplication::checkGameServerMessages(void)
{
    if (replayInProgress)
        return;

    if (client.isConnected()) {
        while (client.messagesWaiting() > 0) {
            network::SnookerMessage message = client.getMessage();
            processMessage(message);
        }
    }
    if (messages.empty()) return;

    if (messages.front().getType() == network::SnookerMessage::yourMoveInfo)
    {
        setClientState(STATE_MY_MOVE);
        model.gameState->setPlayer(me);
        model.whiteBallPoint = R3::ZERO;
        gui.gameStateChanged();
        messages.pop();
    }
    if (messages.empty()) return;

    if (messages.front().getType() == network::SnookerMessage::moveOfOtherPlayerInfo) {
        setClientState(STATE_WAIT_FOR_MOVE);
        model.gameState->setPlayer(me.other());
        gui.gameStateChanged();
        messages.pop();
    }
    if (messages.empty()) return;

    if (clientState == STATE_WAIT_FOR_MOVE &&
            messages.front().getType() == network::SnookerMessage::whiteBallMoveInfo) {
        const double * whitePos = (const double *) messages.front().getBinaryDataPointer();
        model.positions[0].pos[0] = whitePos[0];
        model.positions[0].pos[1] = whitePos[1];
        messages.pop();
    }
    if (messages.empty()) return;

    if (clientState == STATE_WAIT_FOR_MOVE &&
            messages.front().getType() == network::SnookerMessage::shotDataInfo) {
        model.setShotDataFromString(std::string((const char*)messages.front().getBinaryDataPointer()));
        initNewShot();
        setClientState(STATE_WAIT_FOR_STOP);
        messages.pop();
    }
    if (messages.empty()) return;

    if (clientState == STATE_WAIT_FOR_MOVE &&
            messages.front().getType() == SnookerMessage::afterNonShotGameStateInfo) {
        network::GameStateInformation gi = messages.front().getGameStateInformation();
        model.result.points[0] = gi.pointsAfter[0];
        model.result.points[1] = gi.pointsAfter[1];
        model.result.currentBreak = gi.breakScore;
        model.setPositionsFromString(gi.ballsAfter);
        lastShotPositionsAfter = gi.ballsAfter;
        GameState * gs = GameStateFactory::deserialize(gi.stateAfter);
        delete model.gameState;
        model.gameState = gs;
        if (gs->isFinished())
            setClientState(STATE_END_OF_NETWORK_GAME);
        else
            setClientState(STATE_WAIT_FOR_MOVE);
        
        messages.pop();
    }
    if (messages.empty()) return;

    if (clientState == STATE_WAIT_FOR_POSITIONS &&
        messages.front().getType() == SnookerMessage::afterShotGameStateInfo) {
        
        network::GameStateInformation gi = messages.front().getGameStateInformation();
        model.result.points[0] = gi.pointsAfter[0];
        model.result.points[1] = gi.pointsAfter[1];
        model.result.currentBreak = gi.breakScore;
        model.setPositionsFromString(gi.ballsAfter);
        lastShotPositionsAfter = gi.ballsAfter;
        GameState * gs = GameStateFactory::deserialize(gi.stateAfter);
        delete model.gameState;
        model.gameState = gs;
        if (gs->isFinished())
            setClientState(STATE_END_OF_NETWORK_GAME);
        else
            setClientState(STATE_WAIT_FOR_MOVE);
        
        messages.pop();
    }
    if (messages.empty()) return;
}


static double magicFunction(double arg)
{
    const double rotationThreshold = 0.1;
    
    if (fabs(arg) < rotationThreshold)
        return 0.0;

    double value = (fabs(arg) - rotationThreshold) * 2;
    double sign = sig(arg);
    if (arg > 1.5)
        return 0.0;
    return sign * value * value * value;
}

static bool ballTargeting = false;

void SnookerApplication::checkNetworkMessages(void)
{
    SnookerApplication & application = SnookerApplication::getSingleton();

    checkGameServerMessages();

    if (cueClient.isConnected()) {
        struct acc_msg msg, retmsg;
        if(cueFirstConnected) {
            retmsg.type = A_GESTURES;
            retmsg.arg = A_GESTURE_SHOT | A_GESTURE_MENUON | A_GESTURE_MENUOFF;
            if(!cueClient.send(sizeof(retmsg), &retmsg))
                scene->setAccOvlText(3, "Return message not sent");
            cueFirstConnected = false;
        }
        while (cueClient.messagesWaiting() > 0) {
            std::ostringstream napis;
            std::string message;
            if (!cueClient.getMessage(message))
                std::cerr << "Okropny bug 38742!!" << std::endl; 
            scene->setAccOvlText(0, "Cue: connected");
            memcpy(&msg, message.c_str(), sizeof(msg));
            // std::cout << "MSG " << msg.type << std::endl;
            switch(msg.type)
            {
                case A_POS:
                    napis << "Pozycja: " << msg.argf;
                    scene->setAccOvlText(1, napis.str());
                    break;
                case A_ROT:
                    napis << "Orientacja: " << msg.argf;
//                    scene->setAccOvlText(1, napis.str());
                    if (canLookAround()) {
                        setTiltWithInertion(msg.argf[1]);
                        rotateDirection(-magicFunction(msg.argf[2]) / 100.0);
                    }
                    break;
                case A_VEL:
                    napis << "Predkosc: " << msg.argf.length();
                    scene->setAccOvlText(2, napis.str());                    
                    break;
                case A_ACC:
                    napis << "Przyspieszenia: " << msg.argf.length();
                    scene->setAccOvlText(2, napis.str());                    
                    break;
                case A_KEYBCHG:
/*                    if(msg.arg == 1)
                    {
                        setButtonPressed();
                        retmsg.type = A_RESET;
                        retmsg.arg = A_SET_APOS | A_SET_AVEL;
                        if(!cueClient.send(sizeof(retmsg), &retmsg))
                            scene->setAccOvlText(3, "Return message not sent");
                    }
                    else
                        releaseButton(); */
                      if(msg.arg & A_KEY_UP) {
                        if(!gui.injectKey(OIS::KC_UP)) {
                          ballTargeting = true;
                          scene->setAccOvlText(2, "Shake the cue to move ball hit point");
                          retmsg.type = A_GESTURES;
                          retmsg.arg = A_GESTURE_MOVE_ANY;
                          if(!cueClient.send(sizeof(retmsg), &retmsg))
                              scene->setAccOvlText(3, "Return message not sent");
                        }
                      }
                      else if(ballTargeting){
                        ballTargeting = false;
                        scene->setAccOvlText(2, "");
                        retmsg.type = A_GESTURES;
                        retmsg.arg = A_GESTURE_SHOT | A_GESTURE_MENUON | A_GESTURE_MENUOFF;
                        if(!cueClient.send(sizeof(retmsg), &retmsg))
                            scene->setAccOvlText(3, "Return message not sent");
                      }
                      
                      if(msg.arg & A_KEY_DOWN) {
                        if(!gui.injectKey(OIS::KC_DOWN)) {
                        
                        }
                      }
                                   
                      if(msg.arg & A_KEY_FIRE) {
                        if(!gui.injectKey(OIS::KC_RETURN)) {
                        
                        }
                      }
                      if(msg.arg & A_KEY_LEFT) {
                        prevCameraMode();
                      }
                      if(msg.arg & A_KEY_RIGHT) {
                        nextCameraMode();
                      }
                    break;
                case A_GESTURE:
                    if (msg.arg == A_GESTURE_MOVE_ANY) {
                        moveWhiteBallPoint(msg.argf[1] * 0.4, msg.argf[0] * -0.4);
                    }
                    std::cerr << "FORCE: " << msg.argf[0] << std::endl;
                    if ((msg.arg == A_GESTURE_SHOT) && canShot() && msg.argf[0] > 0.8) {
                        setButtonPressed();
                        model.vectorSize = msg.argf[0];
                        releaseButton();  // syf, ale co zrobic?
                    }
                    if (msg.arg == A_GESTURE_MENUON) {
                        gui.openMainMenu();
                    }
                    if (msg.arg == A_GESTURE_MENUOFF) {
                        gui.closeMenu();
                    }
                    break;
                default:
                    std::cerr << "Cue: nieobslugiwany komunikat: " << msg.type << ".\n";
            }
        }
    }
}

void SnookerApplication::updateScene(Real dtime)
{
    dtime /= slowFactor;

    if (dtime == 0.0)
        dtime = 0.0001;

    timeSinceStart += dtime;

    checkNetworkMessages();
    
    if (clientState != STATE_TRAINING &&
        clientState != STATE_PVP_GAME &&
        clientState != STATE_AI_GAME &&
        clientState != STATE_END_OF_GAME &&
        !(client.isConnected() || client.isConnecting()))
    {
        gui.showMessage("Error", "You have been disconnected");
        setClientState(STATE_TRAINING);
    }

    if (buttonPressed) {
        model.vectorSize += dtime * 10.0;
        if (model.vectorSize > 10)
            model.vectorSize = 10;
    }

    physicsEngine.updateModel(model, dtime);

    // Sprawdzamy, czy wlasnie nie zatrzymala sie ostatnia bila
    if (!model.anyBallMoving() && model.shotInProgress) {
        model.shotInProgress = false;
        
        if (replayInProgress) {
            slowFactor = 1.0;
            replayInProgress = false;
            model.setPositionsFromString(lastShotPositionsAfter);
        } else {
            if (clientState == STATE_WAIT_FOR_STOP) {
                setClientState(STATE_WAIT_FOR_POSITIONS);
            } else {
                GameStateInformation gi;
                gi.pointsBefore[0] = model.result.points[0];
                gi.pointsBefore[1] = model.result.points[1];
                int breakScoreBefore = model.result.currentBreak;
                std::string descr;

                GameState * newState =
                    model.gameState->applyShot(model.shotEvents, model, descr);
                lastShotPositionsAfter = model.positions.toString();
                model.setPositionsFromString(lastShotPositionsAfter);
                
                if (clientState == STATE_MY_MOVE) {
                    std::string newStateS = newState->toString();
                    int action = network::GameStateInformation::none;
                    if (model.gameState->getPlayer() != newState->getPlayer())
                        action = network::GameStateInformation::breakFinished;
                    if (newState->isFinished())
                        action = network::GameStateInformation::gameFinished;

                    gi.pointsAfter[0] = model.result.points[0];
                    gi.pointsAfter[1] = model.result.points[1];
                    gi.breakScore = model.result.currentBreak;
                    if (model.result.currentBreak > 0)
                        gi.points = model.result.currentBreak - breakScoreBefore;
                    else
                        gi.points = 0;
                    gi.ballsBefore = model.begin.toString();
                    gi.shotParams = lastShotData;
                    gi.ballsAfter = lastShotPositionsAfter;
                    gi.description = descr;
                    gi.stateAfter = newState->toString();
                    gi.extraAction = action;
                    client.sendAfterShotGameStateInfo(gi);
                }
                delete model.gameState;
                model.gameState = newState;
                if (newState->isFinished()) {
                    if (networkGameInProgress())
                        setClientState(STATE_END_OF_NETWORK_GAME);
                    else
                        setClientState(STATE_END_OF_GAME);
                } else
                    gui.gameStateChanged();
            }
        }
        
        model.vectorSize = 0;
        model.whiteBallPoint = R3::ZERO;
        model.shotEvents.removeAll();
        scene->updateHint();
    }
   
    if (clientState == STATE_CONNECTING && client.isConnected()) {
        client.sendLoginRequest(
                settings.login.c_str(),
                settings.password.c_str());
        setClientState(STATE_VERIFYING);
    }

    if (clientState == STATE_CONNECTED && getTime() - lastGameList > 1.0) {
        client.sendGamesListRequest();
        lastGameList = getTime();
    }

    if (clientState == STATE_MY_MOVE &&
            model.gameState->getPlayer() == me &&
            getTime() - lastViewInfo > 0.1) {
        double viewInfo[4];
        viewInfo[0] = model.vectorDirection;
        viewInfo[1] = model.tilt;
        viewInfo[2] = model.whiteBallPoint[0];
        viewInfo[3] = model.whiteBallPoint[1];
        client.sendViewInfo(viewInfo, sizeof(viewInfo));
        lastViewInfo = getTime();
    }
    
    if (clientState == STATE_MY_MOVE && wasWhiteBallMoved &&
            getTime() - lastWhiteBallInfo > 0.3) {
        double whitePos[2];
        whitePos[0] = model.positions[0].pos[0];
        whitePos[1] = model.positions[0].pos[1];
        client.sendWhiteBallMoveInfo(whitePos, sizeof(whitePos));
        lastWhiteBallInfo = getTime();
        wasWhiteBallMoved = false;
    }

    updateCamera(dtime * slowFactor);
    scene->update();
    gui.update();

#ifdef DEBUG
    // wyswietlanie parametrow bialej bili
    std::ostringstream napis, napis2;
    napis << "Predkosc liniowa: " << model.positions[0].v;
    scene->setAccOvlText(4, napis.str());
    napis2 << "Predkosc katowa: " << model.positions[0].w;
    scene->setAccOvlText(5, napis2.str());
#endif

    // Opozniamy czas aby bylo tylko LIMIT fps
    const int LIMIT = 40;
    if (1.00 / dtime > LIMIT) {
        microSleep((1.0 / LIMIT - dtime) * 1000000);
    }
}

void SnookerApplication::handleShotEvent(const ShotEvent & event)
{
    model.shotEvents.addEvent(event);
}

void SnookerApplication::moveWhiteBallPoint(const double dx, const double dy)
{
    model.whiteBallPoint += R3(dx, dy, 0);
    if (model.whiteBallPoint.length() > 0.7) {
        model.whiteBallPoint /= model.whiteBallPoint.length();
        model.whiteBallPoint *= 0.7;
    }
}

void SnookerApplication::moveWhiteBall(const double dx, const double dy)
{
    if (model.positions[0].potted ||
            (!model.gameState->canMoveWhiteBall()) ||
            (!canShot()))
        return;

    R3 forward = model.getDirectionAsVector();
    R3 left = forward.crossProd(R3(0,0,-1));
    if (cameraMode == CAMERA_MODE_TOP) {
        forward = R3(-1, 0, 0);
        left = R3(0, -1, 0);
    } else if (cameraMode == CAMERA_MODE_TV) {
        forward = R3(0, -1, 0);
        left = R3(1, 0, 0);
    }

    R3 new_pos = model.positions[0].pos - forward * dy - left * dx;

    model.positions[0].potted = true;
    bool position_free = RestoreBall::canPlaceBall(model, new_pos);
    bool inside_D =
        new_pos.distanceTo(BROWN_SPOT) < D_RADIUS &&
        new_pos[1] < BROWN_SPOT[1];
        
    if (position_free && (clientState == STATE_TRAINING || inside_D)) {
        model.positions[0].pos = new_pos;
    }
    model.positions[0].potted = false;

    scene->updateHint();

    if (clientState == STATE_MY_MOVE)
        wasWhiteBallMoved = true;
}

void SnookerApplication::setCameraMode(CameraMode mode)
{
    startPosition = cameraPosition;
    startDirection = cameraDirection;
    cameraMode = mode;
    
    switch (mode) {
        case CAMERA_MODE_TOP:
            goalPosition = BLUE_SPOT + R3(40, 0, 350);
            goalDirection = BLUE_SPOT + R3(39.9, 0, 0);
            model.tilt = 0.0;
            break;
        case CAMERA_MODE_TV:
            goalPosition = R3(0, TABLE_LENGTH / 2 + 100, 130);
            goalDirection = (PINK_SPOT * 2 + BLACK_SPOT) / 3;
            break;
        default:
            break;
    }
    scene->updateHint();
}

void SnookerApplication::nextCameraMode()
{
    switch (cameraMode) {
        case CAMERA_MODE_TOP:
            setCameraMode(CAMERA_MODE_DYNAMIC);
            break;
        case CAMERA_MODE_TV:
            setCameraMode(CAMERA_MODE_TOP);
            break;
        case CAMERA_MODE_DYNAMIC:
            setCameraMode(CAMERA_MODE_TV);
            break;
        default:
            break;
    }
}

void SnookerApplication::prevCameraMode()
{
    switch (cameraMode) {
        case CAMERA_MODE_TOP:
            setCameraMode(CAMERA_MODE_TV);
            break;
        case CAMERA_MODE_TV:
            setCameraMode(CAMERA_MODE_DYNAMIC);
            break;
        case CAMERA_MODE_DYNAMIC:
            setCameraMode(CAMERA_MODE_TOP);
            break;
        default:
            break;
    }
}

void SnookerApplication::updateCamera(double dtime)
{

    if (cameraMode == CAMERA_MODE_DYNAMIC) {
        // odległość kamery od białej bili
        double distance = 0.0;
        for (int i = 0; i < MAX_BALLS; i++)
            if (model.positions[i].v.length() > distance)
                distance = model.positions[i].v.length();
        distance = 60 + distance / 2;
        distance = 40 + (distance - 40) / slowFactor;
        if (distance > 230)
            distance = 230;

        // to jest wektor równy mniej więcej wektorkowi przylepionemu do białej bili:
        R3 vect = model.getDirectionAsVector();
        vect *= distance * cos(model.tilt);

        // wektor pionowy, o tyle mamy podnieść kamerę ponad srodek bili
        R3 tilt(0, 0, sin(model.tilt) * distance);

        // znajdujemy pozycję bialej bili
        R3 white = model.positions[0].pos;
        
        // wysokość głowy :)
        R3 head(0, 0, 5 + 10 / slowFactor);

        // docelową pozycję kamery:
        goalPosition = white - vect + tilt + head;
        goalDirection = white + head;

        if (!model.shotInProgress && cameraPosition.distanceTo(goalPosition) < 0.05)
            cameraPosition = goalPosition;
        else if (dtime < 1.0 / 8.0)
            cameraPosition += (goalPosition - cameraPosition) * 8 * dtime;
        else
            cameraPosition = goalPosition;

        if (!model.shotInProgress && cameraDirection.distanceTo(goalDirection) < 0.05)
            cameraDirection = goalDirection;
        else if (dtime < 1.0 / 15.0)
            cameraDirection += (goalDirection - cameraDirection) * 15 * dtime;
        else
            cameraDirection = goalDirection;

    } else {

        // Prędkość ruchu kamery w cm / s
        const double speed = 400.0;

        R3 dx = (goalPosition - cameraPosition).unit() * speed * dtime;
        if (dx.length() > goalPosition.distanceTo(cameraPosition))
            cameraPosition = goalPosition;
        else
            cameraPosition += dx;

        dx = (goalDirection - cameraDirection).unit() * speed * dtime;
        if (dx.length() > goalDirection.distanceTo(cameraDirection))
            cameraDirection = goalDirection;
        else
            cameraDirection += dx;
    }

    camera->setPosition(Converter::toOgre(cameraPosition));
    camera->lookAt(Converter::toOgre(cameraDirection));
}

void SnookerApplication::connect()
{
    gui.closeMenu();
    
    if (networkGameInProgress())
        client.sendLeaveGameRequest();

    if (client.isConnecting()) {
        setClientState(STATE_CONNECTING);
        return;
    }

    if (client.isConnected()) {
        setClientState(STATE_CONNECTED);
        return;
    }

    client.establishConnection(
            settings.gameServer.c_str(), settings.gamePort.c_str());
    setClientState(STATE_CONNECTING);
}
        
void SnookerApplication::createRoom(void)
{
    client.sendCreateAndJoinNewGameRequest();
    setClientState(STATE_CREATING_GAME);
}
        
void SnookerApplication::joinGame(int id)
{
    client.sendJoinGameAsPlayerRequest(id);
    setClientState(STATE_JOINING_AS_PLAYER);
}

void SnookerApplication::spectateGame(int id)
{
    client.sendJoinGameAsSpectatorRequest(id);
    setClientState(STATE_JOINING_AS_SPECTATOR);
}

void SnookerApplication::startMatch(void)
{ 
    if (networkGameInProgress())
        client.sendLeaveGameRequest();

    model.startNewGame();
    setClientState(STATE_PVP_GAME);
}

void SnookerApplication::startTraining(void)
{
    if (networkGameInProgress())
        client.sendLeaveGameRequest();

    model.startNewGame();
    delete model.gameState;
    model.gameState = new GameStateTraining(Player(0), model);
    gui.gameStateChanged();
    setClientState(STATE_TRAINING);
}

void SnookerApplication::rotateDirection(float angle)
{
    if (buttonPressed)
        return;
    model.vectorDirection += angle;
    while (model.vectorDirection < 0.0)
        model.vectorDirection += 2*PI;
    while (model.vectorDirection > 2*PI)
        model.vectorDirection -= 2*PI;
    if (canShot())
        scene->updateHint();
}

void SnookerApplication::rotateTilt(float angle)
{
    if (buttonPressed)
        return;
    model.tilt += angle;
    if (model.tilt < 0)
        model.tilt = 0;
    if (model.tilt > PI / 2 - 0.01)
        model.tilt = PI / 2 - 0.01;
    if (cameraMode == CAMERA_MODE_TOP)
        model.tilt = 0.0;
}

void SnookerApplication::setTilt(float angle)
{
    if (buttonPressed)
        return;
    model.tilt = angle;
    if (model.tilt < 0)
        model.tilt = 0;
    if (model.tilt > PI / 2 - 0.01)
        model.tilt = PI / 2 - 0.01;
}

void SnookerApplication::setTiltWithInertion(float angle)
{
    if (buttonPressed)
        return;
    model.tilt = ((angle - 1.0 / 6.0) + 9.0 * model.tilt) / 10.0;
    if (model.tilt < 0)
        model.tilt = 0;
    if (model.tilt > PI / 2 - 0.01)
        model.tilt = PI / 2 - 0.01;
}

void SnookerApplication::setButtonPressed(void)
{
    if (canShot() && !model.shotInProgress) { 
        buttonPressed = true;
    }
}

void SnookerApplication::initReplay(double howSlow)
{
    if (replayInProgress)
        slowFactor = howSlow;

    if (model.shotInProgress || lastShotData.size() == 0)
        return;

    replayInProgress = true;
    slowFactor = howSlow;
    model.setShotDataFromString(lastShotData);
    model.setPositionsFromString(lastShotPositionsBefore);
    physicsEngine.initShot(model);
}

void SnookerApplication::initNewShot(void)
{
    lastShotData = model.shotDataToString();
    model.setShotDataFromString(lastShotData);
    lastShotPositionsBefore = model.positions.toString();
    model.beginGameState = model.gameState->toString();
    physicsEngine.initShot(model);
}

void SnookerApplication::releaseButton(void)
{
    if (buttonPressed && model.gameState->canShot())
    {
        if (clientState == STATE_MY_MOVE) {
            std::string data = model.shotDataToString();
            client.sendShotDataInfo(data.c_str(), data.size() + 1);
        }
        initNewShot();
    } else
        gui.injectKey(OIS::KC_RETURN);
    buttonPressed = false;
}

void SnookerApplication::applyDecision(Decision decision)
{
    GameStateInformation gi;
    gi.pointsBefore[0] = model.result.points[0];
    gi.pointsBefore[1] = model.result.points[1];
    int breakScoreBefore = model.result.currentBreak;
    std::string descr;

    GameState * newState = model.gameState->applyDecision(decision, model, descr);

    if (clientState == STATE_MY_MOVE) {
        std::string newPosS = model.positions.toString();
        std::string newStateS = newState->toString();
        int action = network::GameStateInformation::none;
        if (model.gameState->getPlayer() != newState->getPlayer())
            action = network::GameStateInformation::breakFinished;
        if (newState->isFinished())
            action = network::GameStateInformation::gameFinished;

        gi.pointsAfter[0] = model.result.points[0];
        gi.pointsAfter[1] = model.result.points[1];
        gi.breakScore = model.result.currentBreak;
        if (model.result.currentBreak > 0)
            gi.points = model.result.currentBreak - breakScoreBefore;
        else
            gi.points = 0;
        gi.ballsBefore = model.begin.toString();
        gi.shotParams = "";
        gi.ballsAfter = model.positions.toString();
        gi.description = descr;
        gi.stateAfter = newState->toString();
        gi.extraAction = action;
        client.sendAfterNonShotGameStateInfo(gi); 
    }

    delete model.gameState;
    model.gameState = newState;
    gui.gameStateChanged();
    model.shotEvents.removeAll();
    model.whiteBallPoint = R3::ZERO;
    scene->updateHint();
}

void SnookerApplication::screenshot(void)
{
    std::ostringstream ss;
    ss << "screenshot_" << ++screenshotId << ".png";
    renderWindow->writeContentsToFile(Runtime::workDir() + ss.str());
}

void SnookerApplication::cueConnect(void)
{
    // scene->setAccOvlVisible(true);
    scene->setAccOvlText(0, "Cue: connecting...");
    cueClient.establishConnection(
            settings.cueServer.c_str(),
            settings.cuePort.c_str());
    cueFirstConnected = true;
}

bool SnookerApplication::networkGameInProgress(void)
{
    return (clientState == STATE_MY_MOVE ||
            clientState == STATE_WAITING_PLAYERS ||
            clientState == STATE_GAME_STARTS ||
            clientState == STATE_WAIT_FOR_MOVE ||
            clientState == STATE_WAIT_FOR_POSITIONS ||
            clientState == STATE_WAIT_FOR_STOP ||
            clientState == STATE_END_OF_NETWORK_GAME);
}

bool SnookerApplication::canShot(void)
{
    if (model.shotInProgress)
        return false;
    if (clientState == STATE_TRAINING && model.gameState->canShot())
        return true;
    if (clientState == STATE_PVP_GAME && model.gameState->canShot())
        return true;
    if (clientState == STATE_AI_GAME && model.gameState->getPlayer() == me &&
            model.gameState->canShot())
        return true;
    if (clientState == STATE_MY_MOVE &&
            model.gameState->getPlayer() == me && model.gameState->canShot())
        return true;
    return false;
}

bool SnookerApplication::canLookAround(void)
{
    if (model.shotInProgress)
        return true;
    if (clientState == STATE_TRAINING ||
            clientState == STATE_PVP_GAME ||
            clientState == STATE_AI_GAME ||
            clientState == STATE_MY_MOVE)
        return true;
    return false;
}

bool SnookerApplication::anyoneCanShot(void)
{
    if (model.shotInProgress)
        return false;
    if (clientState == STATE_TRAINING ||
            clientState == STATE_PVP_GAME ||
            clientState == STATE_MY_MOVE ||
            clientState == STATE_WAIT_FOR_MOVE ||
            clientState == STATE_WAIT_FOR_POSITIONS ||
            clientState == STATE_AI_GAME)
        return true;
    return false;
}

bool SnookerApplication::canDecide(void)
{
    if (model.shotInProgress)
        return false;
    if (clientState == STATE_TRAINING || clientState == STATE_PVP_GAME)
        return true;
    if (clientState == STATE_AI_GAME && model.gameState->getPlayer() == me)
        return true;
    if (clientState == STATE_MY_MOVE && model.gameState->getPlayer() == me)
        return true;
    return false;
}

bool SnookerApplication::anyonePlays(void)
{
    if (clientState == STATE_TRAINING ||
            clientState == STATE_PVP_GAME ||
            clientState == STATE_AI_GAME ||
            clientState == STATE_MY_MOVE ||
            clientState == STATE_TRAINING ||
            clientState == STATE_WAIT_FOR_MOVE ||
            clientState == STATE_WAIT_FOR_STOP ||
            clientState == STATE_WAIT_FOR_POSITIONS)
        return true;
    else
        return false;
}

double SnookerApplication::getTime(void)
{
    return timeSinceStart;
}
