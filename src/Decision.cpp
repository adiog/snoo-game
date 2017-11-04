#include "Decision.h"

Decision::Decision(DecisionId i):
    id(i)
{
}

DecisionId Decision::getId(void) const
{
    return id;
}


std::string Decision::getDescription(void) const
{
    switch (id) {
        case DECISION_YELLOW: return "Choose the yellow ball";
        case DECISION_GREEN:  return "Choose the green ball";
        case DECISION_BROWN:  return "Choose the brown ball";
        case DECISION_BLUE:   return "Choose the blue ball";
        case DECISION_PINK:   return "Choose the pink ball";
        case DECISION_BLACK:  return "Choose the black ball";
        case DECISION_YELLOW_FREEBALL: return "Choose the yellow ball as a freeball";
        case DECISION_GREEN_FREEBALL:  return "Choose the green ball as a freeball";
        case DECISION_BROWN_FREEBALL:  return "Choose the brown ball as a freeball";
        case DECISION_PINK_FREEBALL:   return "Choose the pink ball as a freeball";
        case DECISION_BLACK_FREEBALL:  return "Choose the black ball as a freeball";
        case DECISION_CONTINUE: return "Play from this position";
        case DECISION_FORCE_OPPONENT:   return "Let your opponent continue";
        case DECISION_CONCEDE_FRAME:   return "Concede the frame";
        case DECISION_MISS: return "Let your opponent play again";
        default: return "Ugh, there must be a bug - unknown decision";
    }
}
