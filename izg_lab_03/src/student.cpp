/**
 * @file        student.cpp
 * @author      Ladislav Mosner, VUT FIT Brno, imosner@fit.vutbr.cz
 * @author      Petr Kleparnik, VUT FIT Brno, ikleparnik@fit.vutbr.cz
 * @author      Kamil Behun, VUT FIT Brno, ibehun@fit.vutbr.cz
 * @date        11.03.2018
 *
 * @brief       Deklarace funkci studentu. DOPLNUJI STUDENTI
 *
 */

#include "base.h"
#include "student.h"
#include "globals.h"
#include <math.h>

/**
 * @brief Vraci barvu pixelu z pozice [x, y]
 * @param[in] x X souradnice pixelu
 * @param[in] y Y souradnice pixelu
 * @return Barva pixelu na pozici [x, y] ve formatu RGBA
 */
RGBA getPixel(int x, int y)
{
    if (x >= width || y >= height || x < 0 || y < 0) {
        IZG_ERROR("Pristup do framebufferu mimo hranice okna\n");
    }
    return framebuffer[y * width + x];
}

/**
 * @brief Nastavi barvu pixelu na pozici [x, y]
 * @param[in] x X souradnice pixelu
 * @param[in] y Y souradnice pixelu
 * @param[in] color Barva pixelu ve formatu RGBA
 */
void putPixel(int x, int y, RGBA color)
{
    if (x >= width || y >= height || x < 0 || y < 0) {
        IZG_ERROR("Pristup do framebufferu mimo hranice okna\n");
    }
    framebuffer[y * width + x] = color;
}

/**
 * @brief Vykresli usecku se souradnicemi [x1, y1] a [x2, y2]
 * @param[in] x1 X souradnice 1. bodu usecky
 * @param[in] y1 Y souradnice 1. bodu usecky
 * @param[in] x2 X souradnice 2. bodu usecky
 * @param[in] y2 Y souradnice 2. bodu usecky
 * @param[in] color Barva pixelu usecky ve formatu RGBA
 * @param[in] arrow Priznak pro vykresleni sipky (orientace hrany)
 */
void drawLine(int x1, int y1, int x2, int y2, RGBA color, bool arrow = false)
{

    if (arrow) {
        // Sipka na konci hrany
        double vx1 = x2 - x1;
        double vy1 = y2 - y1;
        double length = sqrt(vx1 * vx1 + vy1 * vy1);
        double vx1N = vx1 / length;
        double vy1N = vy1 / length;
        double vx1NN = -vy1N;
        double vy1NN = vx1N;
        int w = 3;
        int h = 10;
        int xT = (int) (x2 + w * vx1NN - h * vx1N);
        int yT = (int) (y2 + w * vy1NN - h * vy1N);
        int xB = (int) (x2 - w * vx1NN - h * vx1N);
        int yB = (int) (y2 - w * vy1NN - h * vy1N);
        pinedaTriangle(Point(x2, y2), Point(xT, yT), Point(xB, yB), color, color, false);
    }

    bool steep = abs(y2 - y1) > abs(x2 - x1);

    if (steep) {
        SWAP(x1, y1);
        SWAP(x2, y2);
    }

    if (x1 > x2) {
        SWAP(x1, x2);
        SWAP(y1, y2);
    }

    const int dx = x2 - x1, dy = abs(y2 - y1);
    const int P1 = 2 * dy, P2 = P1 - 2 * dx;
    int P = 2 * dy - dx;
    int y = y1;
    int ystep = 1;
    if (y1 > y2) ystep = -1;

    for (int x = x1; x <= x2; x++) {
        if (steep) {
            if (y >= 0 && y < width && x >= 0 && x < height) {
                putPixel(y, x, color);
            }
        } else {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                putPixel(x, y, color);
            }
        }

        if (P >= 0) {
            P += P2;
            y += ystep;
        } else {
            P += P1;
        }
    }
}

/**
 * @brief Vyplni a vykresli trojuhelnik
 * @param[in] v1 Prvni bod trojuhelniku
 * @param[in] v2 Druhy bod trojuhelniku
 * @param[in] v3 Treti bod trojuhelniku
 * @param[in] color1 Barva vyplne trojuhelniku
 * @param[in] color2 Barva hranice trojuhelniku
 * @param[in] arrow Priznak pro vykresleni sipky (orientace hrany)
 *
 * SPOLECNY UKOL. Doplnuji studenti se cvicicim.
 */
void pinedaTriangle(const Point &v1, const Point &v2, const Point &v3, const RGBA &color1, const RGBA &color2, bool arrow)
{
    // Nalezeni obalky (minX, maxX), (minY, maxY) trojuhleniku.

    int minX = MIN(v1.x, MIN(v2.x, v3.x));
    int maxX = MAX(v1.x, MAX(v2.x, v3.x));
    int minY = MIN(v1.y, MIN(v2.y, v3.y));
    int maxY = MAX(v1.y, MAX(v2.y, v3.y));
    

    //////// DOPLNTE KOD /////////

    // Oriznuti obalky (minX, maxX, minY, maxY) trojuhleniku podle rozmeru okna.

    if (minX < 0) minX = 0;
    if (minY < 0) minY = 0;

    if (maxX > width) maxX = width;
    if (maxY > height) maxY = height;


    //////// DOPLNTE KOD /////////


    // Spocitani parametru hranove funkce (deltaX, deltaY) pro kazdou hranu.
	// Hodnoty deltaX, deltaY jsou souradnicemi vektoru, ktery ma pocatek
	// v prvnim vrcholu hrany, konec v druhem vrcholu.
	// Vypocet prvnotni hodnoty hranove funkce.

    std::vector<int> E1delta = { v1.x - v2.x, v1.y - v2.y };
    std::vector<int> E2delta = { v2.x - v3.x, v2.y - v3.y };
    std::vector<int> E3delta = { v3.x - v1.x, v3.y - v1.y };
   
    //////// DOPLNTE KOD /////////


    // Vyplnovani: Cyklus pres vsechny body (x, y) v obdelniku (minX, minY), (maxX, maxY).
    // Pro aktualizaci hodnot hranove funkce v bode P (x +/- 1, y) nebo P (x, y +/- 1)
    // vyuzijte hodnoty hranove funkce E (x, y) z bodu P (x, y).
    int e1;
    int e2;
    int e3;
    e1 = (minX - v2.x) * E1delta[1] - (minY - v2.y) * E1delta[0];
    e2 = (minX - v3.x) * E2delta[1] - (minY - v3.y) * E2delta[0];
    e3 = (minX - v1.x) * E3delta[1] - (minY - v1.y) * E3delta[0];


    for (int coordY = minY; coordY < maxY; coordY++) { 
        int counter = 0;
        for (int coordX = minX; coordX < maxX; coordX++) {
            counter += 1;
            if ( e1 >= 0 && e2 >= 0 && e3 >= 0 ) {
                putPixel(coordX, coordY, color1);
            }
            e1 += E1delta[1];
            e2 += E2delta[1];
            e3 += E3delta[1];
        }
        
        e1 -= (maxX - minX) * E1delta[1];
        e2 -= (maxX - minX) * E2delta[1];
        e3 -= (maxX - minX) * E3delta[1];

        e1 -= E1delta[0];
        e2 -= E2delta[0];
        e3 -= E3delta[0];
    }

    //////// DOPLNTE KOD /////////

    // Prekresleni hranic trojuhelniku barvou color2.
    drawLine(v1.x, v1.y, v2.x, v2.y, color2, arrow);
    drawLine(v2.x, v2.y, v3.x, v3.y, color2, arrow);
    drawLine(v3.x, v3.y, v1.x, v1.y, color2, arrow);
}

/**
 * @brief Vyplni a vykresli polygon
 * @param[in] points Pole bodu polygonu
 * @param[in] size Pocet bodu polygonu (velikost pole "points")
 * @param[in] color1 Barva vyplne polygonu
 * @param[in] color2 Barva hranice polygonu
 *
 * SAMOSTATNY BODOVANY UKOL. Doplnuji pouze studenti.
 */

void pinedaPolygon(const Point *points, const int size, const RGBA &color1, const RGBA &color2)
{
    // Pri praci muzete vyuzit pro vas predpripravene datove typy z base.h., napriklad:
    //
    //      Pro ukladani parametru hranovych funkci muzete vyuzit prichystany vektor parametru hranovych funkci "EdgeParams":
    //
    //          EdgeParams edgeParams(size)                         // Vytvorite vektor (pole) "edgeParams" parametru hranovych funkci o velikosti "size".
    //          edgeParams[i].deltaX, edgeParams[i].deltaY          // Pristup k parametrum (deltaX, deltaY) hranove funkce v poli "edgeParams" na indexu "i".
    //
    //      Pro ukladani hodnot hranovych funkci muzete vyuzit prichystany vektor hodnot hranovych funkci "EdgeFncValues":
    //
    //          EdgeFncValues edgeFncValues(size)                   // Vytvorite vektor (pole) "edgeFncValues" hodnot hranovych funkci o velikosti "size".
    //          edgeFncValues[i]                                    // Pristup k hodnote hranove funkce v poli "edgeFncValues" na indexu "i".
    //

    if (size == 0) return;

    // Nalezeni obalky (minX, maxX), (minY, maxY) polygonu.

    int minX;
    int maxX;
    int minY;
    int maxY;

    // najdenie min a max hodnot
    if (size != 0) {
        for (int i = 0; i < size; i++) {
            if (i == 0) {
                minX = points[i].x;
                maxX = points[i].x;
                minY = points[i].y;
                maxY = points[i].y;
            }
            else {
                if (points[i].x < minX) {
                    minX = points[i].x;
                }
                if (points[i].x > maxX) {
                    maxX = points[i].x;
                }
                if (points[i].y < minY) {
                    minY = points[i].y;
                }
                if (points[i].y > maxY) {
                    maxY = points[i].y;
                }
            }
        }
    }
    // Oriznuti obalky (minX, maxX), (minY, maxY) polygonu podle rozmeru okna

    if (minX < 0) minX = 0;
    if (maxX > width) maxX = width;
    if (minY < 0) minY = 0;
    if (maxY > height) maxY = height;


    // Spocitani parametru (deltaX, deltaY) hranove funkce pro kazdou hranu.
	// Hodnoty deltaX, deltaY jsou souradnicemi vektoru, ktery ma pocatek
	// v prvnim vrcholu hrany, konec v druhem vrcholu.
	// Vypocet prvnotnich hodnot hranovych funkci pro jednotlive hrany.


    EdgeParams edgeParams(size);

    for (int i = 0; i < size; i++) {
        edgeParams[i].deltaX = points[(i + 1)%size].x - points[i].x;
        edgeParams[i].deltaY = points[(i + 1)%size].y - points[i].y;
    }

    // Test konvexnosti polygonu
    
    bool first_sign;
    bool sign;
    for (int i = 0; i < size; i++) {
        if (edgeParams[i].deltaX * edgeParams[(i + 1) % size].deltaY - edgeParams[i].deltaY * edgeParams[(i + 1) % size].deltaX >= 0) {
            sign = 1;
        }
        else {
            sign = 0;
        }

        // ulozenie prveho znamienka
        if (i == 0) {
            first_sign = sign;
        }
        // porovnanie ostatnych znamienok
        else {
            if (sign != first_sign) {
                printf("Nekonvexny polygon! Vyplnene na vlastne riziko.\n");
                break;
            }
        }
    }
    

    // Vyplnovani: Cyklus pres vsechny body (x, y) v obdelniku (minX, minY), (maxX, maxY).
    // Pro aktualizaci hodnot hranove funkce v bode P (x +/- 1, y) nebo P (x, y +/- 1)
    // vyuzijte hodnoty hranove funkce E (x, y) z bodu P (x, y) */

    EdgeFncValues edgeFncValues(size);
    for (int y = minY; y < maxY; y++) {
        for (int x = minX; x < maxX; x++) {
            
            // vypocet hranovej funkcie pre prvy pixel
            if (x == minX && y == minY) {
                for (int i = 0; i < size; i++) {
                    edgeFncValues[i] = edgeParams[i].deltaX * (y - points[i].y) - edgeParams[i].deltaY * (x - points[i].x);
                }
            }
            else {
                // posun hranovej funkcie na kazdej hrane
                for (int w = 0; w < size; w++) {
                    edgeFncValues[w] -= edgeParams[w].deltaY;
                }
            }

            // zistenie ci pixel lezi vnutri polygonu
            int valid_pixel = 1;
            for (int w = 0; w < size; w++) {
                if (edgeFncValues[w] < 0) {
                    valid_pixel = 0;
                }
            }
            if (valid_pixel == 1) {
                putPixel(x, y, color1);

            }
        }

        // posun na x=0
        for (int w = 0; w < size; w++) {
            edgeFncValues[w] += (maxX - minX) * edgeParams[w].deltaY;
        }

        // posun na y += 1
        for (int w = 0; w < size; w++) {
            edgeFncValues[w] += edgeParams[w].deltaX;
        }
                
    }

    // Prekresleni hranic polygonu barvou color2.
    for (int i = 0; i < size; i++) {
        drawLine(points[i].x, points[i].y, points[(i + 1) % size].x, points[(i + 1) % size].y, color2/*, true*/);
    }
}
