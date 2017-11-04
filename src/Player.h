#ifndef __Player_h__
#define __Player_h__

/// Reprezentuje gracza lub jego brak
class Player {
    public:
        Player();
        Player(int);
        bool isKnown(void) const; /// czy gracz jest okreslony?
        int getId(void) const;    /// id gracza: 0 lub 1, tylko jeśli isKnown
        operator int () const;
        bool operator == (const Player &) const;
        Player other(void) const; /// id przeciwnika
    private:
        int id;
};

#endif
