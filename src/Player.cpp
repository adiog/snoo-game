#include "Player.h"

Player::Player(): id(-1)
{
}

Player::Player(int i): id(i)
{
}

bool Player::isKnown(void) const
{
    return (id == 0 || id == 1);
}

int Player::getId(void) const
{
    return id;
}

Player::operator int () const
{
    return getId();
}

bool Player::operator == (const Player & p) const
{
    return p.getId() == this->getId();
}

Player Player::other(void) const
{
    return Player(1 - id);
}
