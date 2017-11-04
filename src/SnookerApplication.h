#ifndef __SnookerApplication_h__
#define __SnookerApplication_h__

#include <queue>

#include "OgreVector3.h"
#include "ApplicationFramework.h"
#include "SnookerEventListener.h"
#include "SnookerScene.h"
#include "SnookerModel.h"
#include "SnookerClient.h"
#include "SnookerMessage.h"
#include "SnookerSettings.h"
#include "SnookerGui.h"
#include "NetworkClient.h"
#include "SnookerClient.h"
#include "ShotEventListener.h"
#include "Player.h"
#include "Physics.h"

class SnookerApplication: public ApplicationFramework, public ShotEventListener
{
    public:

        enum ClientState {
            STATE_PVP_GAME = 0,        ///  0 2 graczy przy 1 komputerze
            STATE_AI_GAME,             ///  1 gracz vs AI
            STATE_TRAINING,            ///  2 trening
            STATE_CONNECTING,          ///  3 klient sie laczy
            STATE_VERIFYING,           ///  4 klient potwierdza login
            STATE_CONNECTED,           ///  5 ekran wyboru gry
            STATE_CREATING_GAME,       ///  6 wyslano żądanie stworzenia gry
            STATE_WAITING_PLAYERS,     ///  7 stworzono pokój, czekamy na graczy
            STATE_GAME_STARTS,         ///  8 czekamy na info czyj jest ruch
            STATE_JOINING_AS_PLAYER,   ///  9 kliknelismy 'join as player'
            STATE_MY_MOVE,             /// 10 moj ruch w grze sieciowej
            STATE_WAIT_FOR_MOVE,       /// 11 czekam na shot (ew non-shot)
            STATE_WAIT_FOR_STOP,       /// 12 czekam na zatrzymanie bil przeciwnika
            STATE_WAIT_FOR_POSITIONS,  /// 13 czekam na pozycje bil od przeciwnika
            STATE_END_OF_GAME,         /// 14 gra sie skonczyla (PvP, z AI)
            STATE_END_OF_NETWORK_GAME, /// 15 gra sie skonczyla (sieciowa)
            STATE_JOINING_AS_SPECTATOR /// 16 kliknelismy 'join as spectator'
        };

        enum CameraMode {
            CAMERA_MODE_DYNAMIC = 0,
            CAMERA_MODE_TV,
            CAMERA_MODE_TOP
        };

        SnookerApplication();
        ~SnookerApplication();
        static SnookerApplication & getSingleton(void); ///< zwraca referencję do singletonu
        void setServer(const std::string &, const std::string &); ///< ustawia dane serwera
        void createFrameListener(void); ///< Tworzy obiekt wykonujący polecenia co klatkę
        void createScene(void); ///< Tworzy obiekty na scenie (stół, bile...)
        void updateScene(Ogre::Real dtime); ///< Uaktualnia obraz między klatkami
        void handleShotEvent(const ShotEvent &); ///< Zapisuje informacje o zdarzeniu w czasie uderzenia
        virtual void connect(void); ///< Łączy z serwerem
        virtual void createRoom(void); ///< Wysyła żądanie utworzenia pokoju
        virtual void joinGame(int id); ///< Wysyła żądanie dołączenia się do gry
        virtual void spectateGame(int id);  ///< Wysyła żądanie obseracji gry
        virtual void startMatch(void); ///< Rozpoczyna grę PVP
        virtual void startTraining(void); ///< Rozpoczyna trening
        virtual void showGameList(void); ///< Wysyła żądanie listy gier
        virtual void setClientState(ClientState); ///< Ustawia stan klienta (ClientState)
        virtual void rotateDirection(float angle); ///< Obrana kij o dany kąt
        virtual void rotateTilt(float angle); ///< Podnosi/opuszcza kij o dany kąt
        virtual void setTilt(float angle); ///< Ustawia dany kąt podniesienia kija
        virtual void setTiltWithInertion(float angle); ///< Ustawia dany kąt z bezwładnością
        virtual void moveWhiteBallPoint(const double, const double); ///< Przesuwa punk na białej bili
        virtual void moveWhiteBall(const double, const double); ///< Przesuwa białą bilę (jeśli można)
        virtual void setButtonPressed(void); ///< Obsługa wciśnięcia klawisza myszy
        virtual void initNewShot(void); ///< Rozpoczyna strzał
        virtual void initReplay(double howSlow); ///< Rozpoczyna replay z daną prędkością
        virtual void releaseButton(void); ///< Obsługa puszczenia klawisza myszy
        virtual void applyDecision(Decision); ///< Obsługa podjęcia przez gracza decyzji
        virtual void setCameraMode(CameraMode); ///< Zmiania tryb pracy kamery
        virtual void nextCameraMode(void); ///< Zmienia tryb pracy kamery na kolejny z listy
        virtual void prevCameraMode(void); ///< j.w., na poprzedni
        virtual void screenshot(void); ///< Zapisuje screenshot (w katalogu tym, co plik snoo.conf)
        virtual void cueConnect(void); ///< Łączy się z serwerem kija
        virtual bool networkGameInProgress(void); ///< Czy trwa gra sieciowa?
        virtual bool canShot(void);   ///< Czy gracz używający aplikacji może uderzać
        virtual bool canLookAround(void); ///< Czy gracz może obracać kijem
        virtual bool anyoneCanShot(void); ///< czy ktokolwiek moze uderzac
        virtual bool anyonePlays(void);   ///< czy ktokolwiek gra
        virtual bool canDecide(void); ///< Czy można podejmować teraz decyzje?
        virtual double getTime(void); ///< Czas od startu aplikacji (w sekundach)
        
        SnookerModel model;
        SnookerSettings settings;
        SnookerGui gui;
        network::SnookerClient client;
        ClientState clientState;
        CameraMode cameraMode;
        bool replayInProgress;
        SnookerScene * scene;
    private:
        class SnookerEventListener eventListener;
        NetworkClient cueClient;
        bool cueFirstConnected;
        Physics physicsEngine;

        std::string lastShotData;
        std::string lastShotPositionsBefore;
        std::string lastShotPositionsAfter;

        // Bufory na komunikaty sieciowe
        std::queue<network::SnookerMessage> messages;
        // Numer gracza w grze sieciwej
        Player me;
        // Kiedy ostatnio wyslalismy view info?
        double lastViewInfo;
        // Kiedy ostatnio wyslalismy pozycje bialej bili?
        double lastWhiteBallInfo;
        bool wasWhiteBallMoved;
        // Kiedy ostatnio pobralismy liste gier?
        double lastGameList;

        // Tyle razy jest spowalniany czas, np. podczas powtorki
        double slowFactor;

        // Obsluga kamery
        R3 cameraPosition;
        R3 cameraDirection;
        R3 startPosition;
        R3 startDirection;
        R3 goalPosition;
        R3 goalDirection;

        int screenshotId;
        double timeSinceStart;

        // Obsluga strzalow
        bool buttonPressed;
        
        // Przydatne funkcje
        void updateCamera(double dtime);
        void checkNetworkMessages(void);
        void checkGameServerMessages(void);
        void processMessage(network::SnookerMessage &);
};

#endif
