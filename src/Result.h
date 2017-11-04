#ifndef __Result_h__
#define __Result_h__

class Result {
    public:
        int points[2];    ///< Punkty graczy
        int frames[2];    ///< Liczba wygranych parti
        int currentBreak; ///< Punkty w aktualnym podejściu do stołu
};

#endif
