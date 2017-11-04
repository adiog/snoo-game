#include "Table.h"

#include "R3.h"
#include "SnooCommons.h"
#include "ComplexCushion.h"
#include "CushionFace.h"
#include "CushionDisk.h"

Table::Table()
{
    const double W = TABLE_WIDTH;
    const double L = TABLE_LENGTH;
    const double CW = CUSHION_WIDTH;
    const R3 & C = TABLE_CENTER;

    // Promienie krzywizn band wokół kieszeni narożnych i środkowych
    const double CORNER_R = 12.4;
    const double MIDDLE_R = CW;

    // Odległość środka krzywizny bandy przy kieszeni środkowej
    // od środka długiej bandy
    const double MIDDLE_L = 10.75;

    // Większa odległość środka krzywizny bandy przy
    // kieszeni narożnej od krawędzi stołu
    const double CORNER_L = 17.47;

    // Mniejsza odległość środka krzywizny bandy przy
    // kieszeni narożnej od krawędzi stołu
    const double CORNER_W = 6.1;

    // zielona kieszen
    cushions.add(new CushionDisk(
            C + R3(-W/2 - CORNER_W, -L/2 + CORNER_L), CORNER_R));
    cushions.add(new CushionFace(
            C + R3(-W/2 + CW, -L/2 + CORNER_L),
            C + R3(-W/2 + CW, -MIDDLE_L)));
    cushions.add(new CushionDisk(C + R3(-W/2, -MIDDLE_L), MIDDLE_R));
    
    // zielono-niebieska

    cushions.add(new CushionDisk(C + R3(-W/2, MIDDLE_L), MIDDLE_R));
    cushions.add(new CushionFace(
                C + R3(-W/2 + CW, MIDDLE_L),
                C + R3(-W/2 + CW, L/2 - CORNER_L)));
    cushions.add(new CushionDisk(
            C + R3(-W/2 - CORNER_W, L/2 - CORNER_L), CORNER_R));
    
    // zielono-czarna

    cushions.add(new CushionDisk(
            C + R3(-W/2 + CORNER_L, L/2 + CORNER_W), CORNER_R));
    cushions.add(new CushionFace(
                C + R3(-W/2 + CORNER_L, L/2 - CW),
                C + R3(W/2 - CORNER_L, L/2 - CW)));
    cushions.add(new CushionDisk(
            C + R3(W/2 - CORNER_L, L/2 + CORNER_W), CORNER_R));
    
    // żółto-czarna
    
    cushions.add(new CushionDisk(
            C + R3(W/2 + CORNER_W, L/2 - CORNER_L), CORNER_R));
    cushions.add(new CushionFace(
                C + R3(W/2 - CW, L/2 - CORNER_L),
                C + R3(W/2 - CW, MIDDLE_L)));
    cushions.add(new CushionDisk(C + R3(W/2, MIDDLE_L), MIDDLE_R));
    
    // żółto-niebieska
    
    cushions.add(new CushionDisk(C + R3(W/2, -MIDDLE_L), MIDDLE_R));
    cushions.add(new CushionFace(
                C + R3(W/2 - CW, -MIDDLE_L),
                C + R3(W/2 - CW, -L/2 + CORNER_L)));
    cushions.add(new CushionDisk(
            C + R3(W/2 + CORNER_W, -L/2 + CORNER_L), CORNER_R));
    
    // żółta
    
    cushions.add(new CushionDisk(
            C + R3(W/2 - CORNER_L, -L/2 - CORNER_W), CORNER_R));
    cushions.add(new CushionFace(
                C + R3(W/2 - CORNER_L, -L/2 + CW),
                C + R3(-W/2 + CORNER_L, -L/2 + CW)));
    cushions.add(new CushionDisk(
            C + R3(-W/2 + CORNER_L, -L/2 - CORNER_W), CORNER_R));
    
    // zielona
}

Table::~Table()
{
}

Table & Table::getSingleton(void)
{
    static Table table;
    return table;
}

ComplexCushion & Table::getCushions(void)
{
    return cushions;
}
