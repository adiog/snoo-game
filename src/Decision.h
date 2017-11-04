#ifndef __Decision_h__
#define __Decision_h__

#include <string>
#include <vector>

enum DecisionId {
    DECISION_YELLOW,
    DECISION_GREEN,
    DECISION_BROWN,
    DECISION_BLUE,
    DECISION_PINK,
    DECISION_BLACK,
    DECISION_YELLOW_FREEBALL,
    DECISION_GREEN_FREEBALL,
    DECISION_BROWN_FREEBALL,
    DECISION_BLUE_FREEBALL,
    DECISION_PINK_FREEBALL,
    DECISION_BLACK_FREEBALL,
    DECISION_CONCEDE_FRAME,
    DECISION_CONTINUE,
    DECISION_FORCE_OPPONENT,
    DECISION_MISS,
};

class Decision {
    public:

        Decision(DecisionId);
        DecisionId getId(void) const;
        std::string getDescription(void) const;
    private:
        DecisionId id;
};

typedef std::vector<Decision> Decisions;

#endif
