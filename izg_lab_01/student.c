/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id: $
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - ibobak@fit.vutbr.cz, orderedDithering
 */

#include "student.h"
#include "globals.h"

#include <time.h>

const int M[] = {
    0, 204, 51, 255,
    68, 136, 187, 119,
    34, 238, 17, 221,
    170, 102, 153, 85
};

const int M_SIDE = 4;

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).
 Ukol za 0.25 bodu */
S_RGBA getPixel(int x, int y)
{
	S_RGBA current;
	
	current.red = frame_buffer[y * width + x].red;
	current.blue = frame_buffer[y * width + x].blue;
	current.green = frame_buffer[y * width + x].green;
	current.alpha = 0.587 * current.green + 0.299 * current.red + 0.114 * current.blue;

    return current; //vraci barvu (0, 0, 0)
}
/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
 Ukol za 0.25 bodu */
void putPixel(int x, int y, S_RGBA color)
{
	if (x >= 0 && x < width && y >= 0 && y < height) {
		frame_buffer[y * width + x] = color;
	}
}
/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na odstiny sedi. Vyuziva funkce GetPixel a PutPixel.
 Ukol za 0.5 bodu */
void grayScale()
{
	for (int y = 9; y < height; y++) {
		for (int x = 0; x < width; x++) {
			S_RGBA current_pixel = frame_buffer[y * width + x];
			double intensity_of_new = 0.587 * current_pixel.green + 0.299 * current_pixel.red + 0.114 * current_pixel.blue;
			frame_buffer[y * width + x].blue = intensity_of_new;
			frame_buffer[y * width + x].green = intensity_of_new;
			frame_buffer[y * width + x].red = intensity_of_new;
		}
	}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu maticoveho rozptyleni.
 Ukol za 1 bod */

void orderedDithering()
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int i = x % M_SIDE;
			int j = y % M_SIDE;
			if (frame_buffer[y * width + x].red > M[M_SIDE * j + i]){
				frame_buffer[y * width + x].red = 255;
				frame_buffer[y * width + x].blue = 255;
				frame_buffer[y * width + x].green = 255;
			}
			else {
				frame_buffer[y * width + x].red = 0;
				frame_buffer[y * width + x].blue = 0;
				frame_buffer[y * width + x].green = 0;
			}
		}
	}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu distribuce chyby.
 Ukol za 1 bod */
void errorDistribution() {
	grayScale();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			signed int error = 0;
			if (frame_buffer[y * width + x].red < 126) {
				error = frame_buffer[y * width + x].red;
				frame_buffer[y * width + x] = COLOR_BLACK;
			}
			else {
				error = frame_buffer[y * width + x].red - 255;
				frame_buffer[y * width + x] = COLOR_WHITE;
			}


			if (x < width - 2 && y < height - 2) {

				int right = getPixel(x+1,y).red + ROUND(0.375 * (double)error);

				if (right < 0) {
					frame_buffer[y * width + x + 1] = COLOR_BLACK;
				}
				else if (right > 255) {
					frame_buffer[y * width + x + 1] = COLOR_WHITE;
				}
				else {
					frame_buffer[y * width + x + 1].red = right;
					frame_buffer[y * width + x + 1].green = right;
					frame_buffer[y * width + x + 1].blue = right;
				}


				int bottom_right_pixel = ROUND(0.25 * (double)error) + getPixel(x+1, y+1).red;
				if (bottom_right_pixel < 0) {
					frame_buffer[(y + 1) * width + x + 1] = COLOR_BLACK;
				}
				else if (bottom_right_pixel > 255) {
					frame_buffer[(y + 1) * width + x + 1] = COLOR_WHITE;
				}
				else {
					frame_buffer[(y + 1) * width + x + 1].red = bottom_right_pixel;
					frame_buffer[(y + 1) * width + x + 1].green = bottom_right_pixel;
					frame_buffer[(y + 1) * width + x + 1].blue = bottom_right_pixel;
				}


				int bottom_pixel = ROUND(0.375 * (double)error) + getPixel(x, y+1).red;
				if (bottom_pixel < 0) {
					frame_buffer[(y + 1) * width + x] = COLOR_BLACK;
				}
				else if (bottom_pixel > 255) {
					frame_buffer[(y + 1) * width + x] = COLOR_WHITE;
				}
				else {
					frame_buffer[(y + 1) * width + x].red = bottom_pixel;
					frame_buffer[(y + 1) * width + x].green = bottom_pixel;
					frame_buffer[(y + 1) * width + x].blue = bottom_pixel;
				}
			}
		}
	}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci metody prahovani.
 Demonstracni funkce */
void thresholding(int Threshold)
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > Threshold)
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci nahodneho rozptyleni. 
 Vyuziva funkce GetPixel, PutPixel a GrayScale.
 Demonstracni funkce. */
void randomDithering()
{
	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Inicializace generatoru pseudonahodnych cisel */
	srand((unsigned int)time(NULL));

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);
			
			/* Porovname hodnotu cervene barevne slozky s nahodnym prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > rand()%255)
			{
				putPixel(x, y, COLOR_WHITE);
			}
			else
				putPixel(x, y, COLOR_BLACK);
		}
}
/*****************************************************************************/
/*****************************************************************************/