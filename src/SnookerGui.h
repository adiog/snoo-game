#ifndef __SnookerGui_h__
#define __SnookerGui_h__

#include <string>
#include <stack>
#include <vector>
#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"

#include "Menu.h"
#include "Popup.h"
#include "OISString.h"
#include "Decision.h"
#include "GameData.h"

class SnookerGui {
    public:
        SnookerGui();
        virtual ~SnookerGui();

        /// Tworzy Gui - odpowiednie nakładki itp
        void initialize(void);

        /// Zwraca true, jeśli zrobiło jakiś pożytek z klawisza.
        /// Jeśli zwróciło false to wciśnięcie klawisza trzeba
        /// traktować jako ogólną instrukcję dla interfejsu gry
        bool injectKey(OIS::KeyCode);

        /// Otwiera główne menu gry
        void openMainMenu(void);

        /// Pozakuje okienko doalogowe
        void showMessage(std::string, std::string);

        /// Ustawia napis w lewej części paska stanu
        void setLeftStatus(std::string);

        /// Ustawia napis w prawej części paska stanu
        void setRightStatus(std::string);

        /// Pokazuje w oknie rozmowy nową wiadomość
        void chatMessage(std::string);

        /// Uaktulnia elementy GUI odpowiedzialne za wizualizację stanu gry
        void gameStateChanged(void);

        /// Uaktulnia elementy GUI odpowiedzialne za wizualizację trybu
        void clientStateChanged(void);

        /// Zamyka otwarte Menu
        void closeMenu(void);

        /// Wypisuje listę gier odbywających się na serwerze
        void setGameList(std::vector<std::string>);

        /// Pokazuje nicki obecnych w grze graczy
        void setPlayerList(std::vector<std::string>);

        /// Uaktualnia gui, operacja wykonywana co klatke
        void update(void);
    private:
        Menu * openedMenu; ///< Wskaźnik na otwarte menu, albo NULL
        Menu * mainMenu;   ///< Menu główne gry
        std::stack<Popup*> popups;     ///< Otwarte okna z komunikatami
        std::vector<std::string> chat;
        std::vector<GameData> games;
        Decisions decisions;
        int selectedDecision;
        bool gameListShown; /// Pokazana jest lista gier
        int selectedGame; ///< Wybrana gra, -1 <=> wybrano "create room"
        int selectedColumn; ///< 0 - kolumna Join, 1 - Spectate
        OISString message;
        std::string player1, player2;
        
        void createMainMenu(void); ///< Tworzy menu głóne gry (mainMenu)
        void updateDecisions(void); ///< Uaktualnia liste decycji
        void updateChatMessage(void); ///< Uaktualnia edytowaną wiadomość
        void refreshGameListPosition(void); ///< Uaktualnia podświetloną pozycję listy gier

};

#endif
