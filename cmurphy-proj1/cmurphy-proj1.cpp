//
//Name:Murphy, Chris
//Project:#1
//Due:5/1/14
//Course :cs-445-01-sp1
//
//Description:
/*Implement and compare the performance of the line equation 6-11, the 
Bresenham’s, DAA, and OpenGL line-drawing algorithms.(IMD 6-1, p193).
Print out and provide an analysis of the results.*/


#include <stdio.h> // For printf()
#include <stdlib.h> // For rand()
#include <time.h> // For time()
#include <math.h> // for round() and abs()
#include <fstream>

//Include OpenGL header files, so that we can use OpenGL
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


using namespace std;

struct Point{
public:
	int x;
	int y;
};

class TimeKeeper{
private:
	char *name;
	clock_t startClock;
	clock_t stopClock;
	int numLines;
	double elapsedClocks;
public:
	void startTimer(void);
	void stopTimer(void);
	double getLinesPerClock(void);
	TimeKeeper(char[10]);
} tGL = TimeKeeper("OpenGL")
, tDD = TimeKeeper("DDA")
, tBr = TimeKeeper("Breseham")
, tNa = TimeKeeper("Naive");
TimeKeeper::TimeKeeper(char* nname){
	name = nname;
	elapsedClocks = 0;
	numLines = 0;
}
void TimeKeeper::startTimer(void){
	startClock = clock();
}
void TimeKeeper::stopTimer(void){
	stopClock = clock();
	elapsedClocks += (stopClock - startClock);
	numLines += 1;
	printf("Lines Per CPU-CLock-%s: %4.2f\n", name, getLinesPerClock());
}
double TimeKeeper::getLinesPerClock(void){
	return ((float)numLines) / elapsedClocks;
}

ofstream myfile;

void init(void){
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // Set display-window color to white
	glMatrixMode(GL_PROJECTION); // Set projection parameters
	gluOrtho2D(0.0f, 601.0f, 0.0f, 601.0f);
	srand(time(NULL)); // Initializes a random number generator

	
	myfile.open("output.txt", ios::out);
	myfile << "File is used to store test data from the program\n";
	myfile << "The data is Lines Drawn Per CPU-CLock for: \n";
	myfile << "OpenGL\tDDA\tBresenham's\tNaive\n";
	
}
void drawAxis(void){
	glClear(GL_COLOR_BUFFER_BIT); // Clear display window
	glColor3f(0.0f, 0.0f, 0.0f); // Set line segment color to green
	glBegin(GL_LINES);

	glVertex2i(300, 0);
	glVertex2i(300, 601);

	glVertex2i(0, 300);
	glVertex2i(601, 300);

	glEnd();

	glFlush();
}
// Draw a line using OpenGL's libraries
void drlnGL(Point &p1, Point &p2){
	glBegin(GL_LINES);
	glVertex2i(p1.x, p1.y);
	glVertex2i(p2.x, p2.y);
	glEnd();
}
// Implement the digital differential analyzer (DDA)
int round(int v) { return (int)(v + .5); }
void drlnDD(Point &p1, Point &p2){
	int dx = p2.x - p1.x, dy = p2.y - p1.y, steps;
	float xIncrement, yIncrement, x = p1.x, y = p1.y;
	int abdx = abs(dx), abdy = abs(dy);
	if (abdx > abdy)
		steps = abdx;
	else
		steps = abdy;
	xIncrement = float(dx) / float(steps);
	yIncrement = float(dy) / float(steps);

	glBegin(GL_POINTS);
	glVertex2i(round(x), round(y));
	for (int k = 0; k < steps; k++){
		x += xIncrement;
		y += yIncrement;
		glVertex2i(round(x), round(y));
	}
	glEnd();
}
// Implement the Bresenham line-drawing algorithm
void drlnBr(Point &p1, Point &p2){
	int dx = p2.x - p1.x, dy = p2.y - p1.y;
	if (dx == 0){
		// Vertical Line
		int y0, ye;
		// Determine the ystart and yend values
		if (p1.y > p2.y){
			y0 = p2.y;
			ye = p1.y;
		}
		else{
			y0 = p1.y;
			ye = p2.y;
		}
		glBegin(GL_POINTS);
		for (int y = y0; y <= ye; y++){
			glVertex2i(p1.x, y);
		}
		glEnd();
	}
	else if (dy == 0){
		// Horizontal Line
		int x0, xe;
		// Determine the ystart and yend values
		if (p1.x > p2.x){
			x0 = p2.x;
			xe = p1.x;
		}
		else{
			x0 = p1.x;
			xe = p2.x;
		}
		glBegin(GL_POINTS);
		for (int x = x0; x <= xe; x++){
			glVertex2i(x, p1.y);
		}
		glEnd();
	}
	else{
		// Sloped line
		float m = (float)dy / dx;
		if (abs(m) == 1){
			// Positive slope = 1
			int x0, xe, y, k;
			// Determine the xstart and yend values
			if (p1.x > p2.x){
				x0 = p2.x;
				xe = p1.x;
				y = p2.y;
			}
			else{
				x0 = p1.x;
				xe = p2.x;
				y = p1.y;
			}
			// if negative slope decrement y by 1
			if (abs(m) < 0)	k = -1;
			// otherwise, increment y by 1
			else k = 1;

			glBegin(GL_POINTS);
			for (int x = x0; x <= xe; x++ , y+=k){
				glVertex2i(x, y);
			}
			glEnd();
		}
		else if (abs(m) < 1){
			// |slope| < 1 "stepping in dx"
			int step;
			if (m>0)
				step = 1;
			else
				step = -1;
			dx = abs(dx), dy = abs(dy);
			int p = 2 * dy - dx;
			int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);
			int x, y, xEnd;
			if (p1.x>p2.x){
				x = p2.x;
				y = p2.y;
				xEnd = p1.x;
			}
			else{
				x = p1.x;
				y = p1.y;
				xEnd = p2.x;
			}
			glBegin(GL_POINTS);
			glVertex2i(x, y);
			while (x < xEnd){
				x++;
				if (p < 0)
					p += twoDy;
				else{
					y += step;
					p += twoDyMinusDx;
				}
				glVertex2i(x, y);
			}
			glEnd();
		}
		else{
			// |slope| > 1 "stepping in dy"
			int step;
			if (m>0)
				step = 1;
			else
				step = -1;
			dx = abs(dx), dy = abs(dy);
			int p = 2 * dx - dy;
			int twoDx = 2 * dx, twoDxMinusDy = 2 * (dx - dy);
			int x, y, yEnd;
			if (p1.y>p2.y){
				x = p2.x;
				y = p2.y;
				yEnd = p1.y;
			}
			else{
				x = p1.x;
				y = p1.y;
				yEnd = p2.y;
			}
			glBegin(GL_POINTS);
			glVertex2i(x, y);
			while (y < yEnd){
				y++;
				if (p < 0)
					p += twoDx;
				else{
					x += step;
					p += twoDxMinusDy;
				}
				glVertex2i(x, y);
			}
			glEnd();
		}
	}
}
// Draw a line using a Naive method.
void drlnNa(Point &p1, Point &p2){
	int x0, xe, y0, ye;
	// Find the smallest x value
	if (p1.x < p2.x) { x0 = p1.x; xe = p2.x; }
	else { x0 = p2.x, xe = p1.x; }
	// Find the smallest y value
	if (p1.y < p2.y) { y0 = p1.y; ye = p2.y; }
	else { y0 = p2.y; ye = p1.y; }

	// Calculate m and b from y=mx+b
	double m = (float(p2.y - p1.y)) / (p2.x - p1.x);
	double b = p1.y - m*p1.x;

	glBegin(GL_POINTS);

	// Draw points based on dx=1
	for (int x = x0, y = y0; x <= xe; x++, y =round(m*x + b)){
		glVertex2i(x, y);
	}
	// Draw points based on dy=1
	for (int x = x0, y = y0; y <= ye; y++, x= round((y-b)/m)){
		glVertex2i(x, y);
	}

	glEnd();
}

float random(){
	return ((float)rand()) / RAND_MAX * 300;
}
float randomf(){
	return ((float)rand()) / RAND_MAX;
}
void update(int value){
	Point p1, p2;
	p1.x = random();
	p1.y = random();
	//p2.x = p1.x;
	p2.x = random();
	p2.y = random();
	system("cls");
	// Print out Points to console
	printf("(r1,r2): %i , %i\n", p1.x, p1.y);
	printf("(r1,r2): %i , %i\n", p2.x, p2.y);
	float m = (float(p2.y - p1.y)) / (p2.x - p1.x);
	
		glColor3f(randomf(), randomf(), randomf()); // Set line segment color to green

		tGL.startTimer();
		drlnGL(p1, p2); // Draw line with OpenGL
		tGL.stopTimer();

		p1.x += 301; // Move points 1 quadrant right
		p2.x += 301;

		tDD.startTimer();
		drlnDD(p1, p2); // Draw line with DDA
		tDD.stopTimer();

		p1.y += 301; // Move points again, 1 quadrant up
		p2.y += 301;
		
		tBr.startTimer();
		drlnBr(p1, p2); // Draw line with Bresenham
		tBr.stopTimer();

		p1.x -= 301; // Move points 1 quadrant left
		p2.x -= 301;

		tNa.startTimer();
		drlnNa(p1, p2); // Draw line with Naive
		tNa.stopTimer();

		glFlush();
		


		glutPostRedisplay(); //Tell GLUT that the display has changed
		if (!myfile.is_open())
			myfile.open("output.txt", ios::app);
		myfile << tGL.getLinesPerClock() << "\t" << tDD.getLinesPerClock()
			<< "\t" << tBr.getLinesPerClock() << "\t" << tNa.getLinesPerClock()
			<< endl;
		myfile.close();
	//Tell GLUT to call update again in 25 milliseconds
	glutTimerFunc(0, update, 0);
}

void drawStart(void){}

int main(int argc, char** argv){
	glutInit(&argc, argv); // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //set display mode
	glutInitWindowPosition(50, 100); //set top left window display position
	glutInitWindowSize(601, 601); // display window width and height
	glutCreateWindow("Compare the performance of the line equations");
	init(); //Execute initialization procedure
	
	glutDisplayFunc(drawStart); //Send Graphics to the display window
	drawAxis();
	glutTimerFunc(25, update, 0); //Add a timer

	glutMainLoop(); //Display everything and wait

	return 0;
}

