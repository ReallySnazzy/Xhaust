#include <iostream>

//to determine console width/height
#include <sys/ioctl.h>
#include <unistd.h>

#include <complex>

#include <ctime>

bool initialized = false;
char *buffer; //not a string buffer.
//it's a rendering buffer, that's a string.
int *colbuffer;

int cWidth;
int cHeight;

int width;
int height;

struct winsize w;

int curr_col_r;
int curr_col_g;
int curr_col_b;

int FORCE_SQUARE_ASPECT = true;

float pixelHeight = 10. / 4;

char defaultPixel = '#';

void resize()
{
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	cWidth = w.ws_col;
	cHeight = w.ws_row;

	width = cWidth;
	height = (int)(10 / 4. * cHeight);

	char *new_buffer = (char *)malloc((cWidth * cHeight + 1) * sizeof(char));
	int *new_colbuffer = (int *)malloc((cWidth * cHeight) * sizeof(int));

	for (int y = 0; y < cHeight; y++)
	{
		for (int x = 0; x < cWidth; x++)
		{
			new_buffer[y * cWidth + x] = ' ';
		}
	}
	new_buffer[cWidth * cHeight] = '\0';

	//this is a serious mem leak

	//todo properly handle prev contents of buffer.
	buffer = new_buffer;
	colbuffer = new_colbuffer;
}

void setCharwiseBrightness(int v)
{
	defaultPixel = "-~+|=*%&$#@"[v];
}

void initialize()
{
	initialized = true;
	setCharwiseBrightness(9);
	resize();
}

void clear()
{
	std::cout << "\033[H\033[J";
}

void resetColor()
{
	std::cout << "\x1b[0m";
}

void setColor(int r, int g, int b)
{
	curr_col_r = r;
	curr_col_g = g;
	curr_col_b = b;
	//std::cout << "\x1b[38;2;" << r << ";" << g << ";" << b << "m";
}

// concept: tween pixel for smoother animations.
void putFloatyPixel(float x, float y, char c)
{
	//throw exception if x or y out of bounds (or just don't render)

	if (x < 0 || width <= x)
		return;

	if (y < 0 || height <= y)
		return;

	if (FORCE_SQUARE_ASPECT)
		y = (int)(4 / 10. * (float)y);

	float xf = x - (int)x;
	float yf = y - (int)y;

	float cf;
	int col;

	//distribute color across four adjacent pixels

	cf = (1 - xf) * (1 - yf);
	col = ((int)(cf * curr_col_r) << 16) + ((int)(cf * curr_col_g) << 8) + (int)(cf * curr_col_b);
	colbuffer[cWidth * (int)y + (int)x] = col;
	buffer[cWidth * (int)y + (int)x] = c;

	cf = (1 - xf) * (yf);
	col = ((int)(cf * curr_col_r) << 16) + ((int)(cf * curr_col_g) << 8) + (int)(cf * curr_col_b);
	colbuffer[cWidth * (-(int)(-y)) + (int)x] = col;
	buffer[cWidth * (-(int)(-y)) + (int)x] = c;

	cf = (xf) * (1 - yf);
	col = ((int)(cf * curr_col_r) << 16) + ((int)(cf * curr_col_g) << 8) + (int)(cf * curr_col_b);
	colbuffer[cWidth * (int)y + (int)(-(int)(-x))] = col;
	buffer[cWidth * (int)y + (int)(-(int)(-x))] = c;

	cf = (xf) * (yf);
	col = ((int)(cf * curr_col_r) << 16) + ((int)(cf * curr_col_g) << 8) + (int)(cf * curr_col_b);
	colbuffer[cWidth * (-(int)(-y)) + (int)(-(int)(-x))] = col;
	buffer[cWidth * (-(int)(-y)) + (int)(-(int)(-x))] = c;

	// colbuffer[cWidth * y + x] = (curr_col_r << 16) +
	// 							(curr_col_g << 8) +
	// 							curr_col_b;

	//buffer[cWidth * y + x] = c;
}

void putPixel(int x, int y, char c)
{
	//throw exception if x or y out of bounds (or just don't render)

	if (x < 0 || width <= x)
		return;

	if (y < 0 || height <= y)
		return;

	if (FORCE_SQUARE_ASPECT)
		y = (int)(4 / 10. * (float)y);

	colbuffer[cWidth * y + x] = (curr_col_r << 16) + (curr_col_g << 8) + curr_col_b;
	buffer[cWidth * y + x] = c;
}

void clearPixel(int x, int y)
{
	putPixel(x, y, ' ');
}

void putPixel(int x, int y)
{
	putPixel(x, y, defaultPixel);
}

void putText(int x, int y, char const *msg, bool renderSpace)
{
	for (int i = 0; msg[i] != '\0'; i++)

		if (msg[i] != ' ' || renderSpace)
			putPixel(x + i, y, msg[i]);
}

//TODO: use recursion to support newlines
void putText(int x, int y, char const *msg)
{
	putText(x, y, msg, false);
}

void blit()
{
	clear();

	int r, g, b = 0;
	//std::cout << "\x1b[38;2;" << r << ";" << g << ";" << b << "m";
	for (int i = 0; buffer[i] != '\0'; i++)
	{
		r = (colbuffer[i] >> 16);
		g = (colbuffer[i] >> 8) & 0xFF;
		b = colbuffer[i] & 0xFF;

		if (i == 0 || colbuffer[i] != colbuffer[i - 1])
		{
			std::cout << "\x1b[38;2;" << r << ";" << g << ";" << b << "m";
			// << colbuffer[i] << ";;" << r << ";" << g << ";" << b;
		}

		std::cout << buffer[i];

		//newline at end of each line
		// if (i % width == width - 1)
		// 	std::cout << '\n';
	}
}

void line(int x1, int y1, int x2, int y2, char c)
{
	int xi = std::min(x1, x2);
	int xf = std::max(x1, x2);

	int yi = std::min(y1, y2);
	int yf = std::max(y1, y2);

	float y = yi;
	float x = xi;

	float progress = 0;

	while (yf - y > 0 || xf - x > 0)
	{
		putPixel(x, (int)y, c);

		if (xf - xi > yf - yi)
		{
			x += 1;
			progress = (x - xi) / (float)(xf - xi);
			y = yi + progress * (yf - yi);
		}
		else if (yf - yi > xf - xi)
		{
			y += 1;
			progress = (y - yi) / (float)(yf - yi);
			x = xi + progress * (xf - xi);
		}
		else
		{
			x++;
			y++;
		}
	}
}

void line(int x1, int y1, int x2, int y2)
{
	line(x1, y1, x2, y2, defaultPixel);
}

void ellipse(int x, int y, int w, int h, int c)
{
	int ch;
	for (int px = x; px < x + w; px++)
	{
		float upx = (px - x) / (.5 * w) - 1;
		ch = h * std::sqrt(1 - upx * upx);
		for (int py = y + h / 2 - ch / 2; py < y + h / 2 + ch / 2; py++)
			putPixel(px, py, c);
	}
}

void ellipse(int x, int y, int w, int h)
{
	ellipse(x, y, w, h, defaultPixel);
}

//TODO: make this method a little faster?
void rect(int x, int y, int w, int h, int c)
{
	for (int py = y; py < y + h; py++)
		for (int px = x; px < x + w; px++)
			putPixel(px, py, c);
}

void clearRect(int x, int y, int w, int h)
{
	rect(x, y, w, h, ' ');
}

void rect(int x, int y, int w, int h)
{
	rect(x, y, w, h, defaultPixel);
}

int main(int argc, char **argv)
{
	clear();

	initialize();

	for (int i = -10; i < width; i++)
	{
		int r = (int)(255 * (i + 10) / (float)(width + 10)) % 255; //(0 + i) % 255;
		setColor(r, 46, 113);

		int v = (9 * r / 255.);
		line(i, 0, 10 + i, height, (char)(48 + v));

		//setCharwiseBrightness(v);
		line(i, 0, 10 + i, height);
	}

	//setCharwiseBrightness(9);

	setColor(0, 100, 0);
	line(0, 0, 0, height - 1);
	line(1, 0, 1, height - 1);
	line(0, height - 1, width - 1, height - 1);
	line(width - 1, height - 1, width - 1, 0);
	line(width - 2, height - 1, width - 2, 0);
	line(width - 1, 0, 0, 0);

	rect(4, 3 * pixelHeight, 10, 10);

	setColor(255, 255, 255);
	putText(6, 12, " I can also render text here!", true);

	setColor(234, 221, 43);
	int ellipse_size = 20;
	ellipse(width / 2 - ellipse_size / 2, height / 2 - ellipse_size / 2, ellipse_size, ellipse_size);

	setColor(255, 255, 255);

	blit();
	int px = 0;
	setColor(255, 255, 255);

	double duration;
	std::clock_t start;

	while (true) //timed interval.. add keyboard interval (and default it).. they dont
	//really play nicely together. they can but it's not cross platform
	{
		duration = 0;

		// start = std::clock();
		// while (duration < .1)
		// {
		// 	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		// }

		std::getc(std::cin); //proper keyboard input is system/console dependant

		putPixel(px, height - 10);
		blit();

		px++;
		if (px > 20)
			break;
	}

	resetColor();
}