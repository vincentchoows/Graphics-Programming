
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "Assignment"

float tz = 0.0;
boolean isOrtho = true;
float oNear = -8.0, oFar = 8.0;
float pNear = 1.0, pFar = 20.0;
float tSpeed = 0.1;
float ptx = 0.0, pty = 0.0, ptSpeed = 0.1;
float tx = 0.0;
float pry = 0.0, prSpeed = 1.0;
float Ry = 180.0, Rx = 0.0;
float fovy = 10.0;
int qNo = 1;

float w = 0.0, walkBack = 0.0;
float u = 0.0, i = 0.0, upDown = 0.0;
float h = 0.0, handBack = 0.0, fingerRotate = 0.0;

int noOfTriangle = 30;
boolean walk = false;
boolean handmove = false, fingerMove = false;
boolean sword = false;
boolean defend = false;
boolean attack = false;
boolean rHandUp = false;
int swordUndo = 0, attackUndo = 0, walkUndo = 0, fingerUndo = 0;

boolean openGun = false, gunShot = false, gunChargeShot = false;
boolean handup = false;
int gunUndo = 0, armLeftUp = 0, rHandUndo = 0, defendUndo = 0;
float gunAngle = 0.0, gunShotSpeed = 0.0, gunChargeSize = 0.0, backgroundSpeed = 0.0, rotateIncrement = 2.0;

//lighting and material
float amb[] = { 0.6, 1.0, 0.6 }; //red ambient light source
float dif[] = { 0.6, 1.0, 0.6 }; //red diffuse light source
float posA[] = { 0.0, 0.4, 0.0 }; //ambient light from top 
float posD[] = { 0.0, 0.4, 0.0 }; //diffsue light from right 

float ambM[] = { 0.0, 1.0, 0.0 }; // red ambient material 
float difM[] = { 0.0, 1.0, 0.0 }; // red diffuse material
bool isLightOn = false;				//is light on?

//rotating head var
float headx = 0, heady = 0, headz = 0;
float xMax = -25.0, xMin = 25.0;
boolean projIdentifier = false;

GLuint texture = 0;  //texture name
BITMAP BMP;				//butmap structure
HBITMAP hBMP = NULL;	//bitmap handler
GLuint textureArray[10];

int bodyChange = 0.0;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else if (wParam == VK_RIGHT)
		{
			Ry += 2.0;
			glLoadIdentity();
			//glRotatef(Ry, 0.0, 1.0, 0.0);
		}
		else if (wParam == VK_LEFT)
		{
			Ry -= 2.0;
			glLoadIdentity();
			//glRotatef(Ry, 0.0, 1.0, 0.0);
		}
		else if (wParam == VK_UP)
		{
			Rx += 2.0;
			glLoadIdentity();
			//glRotatef(Ry, 1.0, 0.0, 0.0);
		}
		else if (wParam == VK_DOWN)
		{
			Rx -= 2.0;
			glLoadIdentity();
			//glRotatef(Ry, 1.0, 0.0, 0.0);
		}
		else if (wParam == VK_SPACE)
		{
			ptx = 0.0;
			pty = 0.0;
			pry = 0.0;
			fovy = 10.0;

			//head rotate var
			headx = 0;
			heady = 0;
			headz = 0;

			Rx = 0;
			Ry = 180;
			walk = false;
			handmove = false;
			sword = false;
			defend = false;
			attack = false;
			openGun = false;
			gunShot = false;
			gunChargeShot = false;
			handup = false;
			isLightOn = false;
			gunChargeSize = 0.0;
			gunUndo = 0;
			armLeftUp = 0;
			swordUndo = 0;
			attackUndo = 0;
			walkUndo = 0;
			bodyChange = 0;
			w = 0.0;
			rHandUp = false;
			rHandUndo = 0;
		}
		else if (wParam == 'O') {
			isOrtho = true;
			tz = 0.0;
			projIdentifier = false;
		}
		else if (wParam == 'P') {
			isOrtho = false;
			tz = 0.0;
			projIdentifier = true;
		}
		else if (wParam == 'Z') //Z = Walk
		{
			walk = true;
			walkUndo += 1;
			handup = false;
			gunShot = false;
			gunUndo = 0;
			armLeftUp = 0;
			rHandUp = false;
			defend = false;
			sword = false;
		}
		else if (wParam == 'E') //E = Sword
		{
			sword = true;
			swordUndo += 1;
			walk = false;
			w = 0.0;
			defend = false;
		}
		else if (wParam == 'V') //V = Defend(Shield) from left side
		{
			defend = true;
			defendUndo += 1;
		}
		else if (wParam == 'Q') //Q = Attack by move the hand
		{
			handmove = true;
			attack = true;
			attackUndo += 1;
			walk = false;
			w = 0.0;
			defend = false;

			rHandUp = false;
		}
		else if (wParam == 'G')//G = Show out the gun
		{
			openGun = true;
			gunUndo += 1;
			gunShot = false;
			gunChargeShot = false;
			gunChargeSize = 0.0;

			defend = false;
		}
		else if (wParam == 'N')// Up and down the left hand
		{
			armLeftUp += 1;
			gunShot = false;
			gunChargeShot = false;
			handup = true;
			gunChargeSize = 0.0;

			walk = false;
			w = 0.0;

			defend = false;
		}
		else if (wParam == 'T')
		{
			defend = false;
			if (gunUndo % 2 == 1 && armLeftUp % 2 == 1) {
				gunChargeShot = false;
				gunShot = true;
				gunShotSpeed = 0;
			}
		}
		else if (wParam == 'C')
		{
			defend = false;
			if (gunUndo % 2 == 1 && armLeftUp % 2 == 1) {
				gunShotSpeed = 0;

				gunChargeShot = true;
				gunChargeSize = 0.0;

			}
		}
		else if (wParam == 'M')		//M
		{
			defend = false;
			rHandUp = true;
			rHandUndo += 1;
			walk = false;
		}
		else if (wParam == 'J')		//J
		{
			defend = false;
			fingerMove = true;
			fingerUndo += 1;

		}
		else if (wParam == 'U')		//U
		{
			pry += prSpeed;

		}
		else if (wParam == 'I')		//I
		{
			pry -= prSpeed;

		}

		//==========
		else if (wParam == VK_F1) {  //F1

			if (heady == 0.0) {
				headx -= rotateIncrement;
				if (headx < xMax) {
					headx = xMax;
				}
			}
		}
		else if (wParam == VK_F2) {  //F2

			if (heady == 0.0) {
				headx += rotateIncrement;
				if (headx > xMin) {
					headx = xMin;
				}
			}
		}
		else if (wParam == VK_F3) {  //F3

			if (headx == 0.0)
				heady += rotateIncrement;
		}
		else if (wParam == VK_F4) {  //F4
			if (headx == 0.0)
				heady -= rotateIncrement;
		}



		else if (wParam == VK_F5)		//F5 --
		{
			if (isOrtho) {
				if (tz < oFar) {
					tz += tSpeed;
				}
				else {
					if (tz > pFar) {
						tz += tSpeed;
					}
				}
			}
			else {
				//zoom out
				//fovy -= tSpeed;
				//tz += tSpeed;


				if (fovy < pNear) {
					fovy = pNear;
				}
				else {
					fovy -= tSpeed;
				}

			}
		}
		else if (wParam == VK_F6)		//F6
		{
			if (isOrtho) {
				if (tz > oNear) {
					tz -= tSpeed;
				}
				else {
					if (tz < pNear) {
						tz -= tSpeed;
					}
				}
			}
			else {

				//zoom in
				//fovy += tSpeed;
				tz -= tSpeed;

				if (fovy > pFar) {
					fovy = pFar;
				}
				else {
					fovy += tSpeed;
				}


			}
		}





		else if (wParam == 'D') {  //D
			ptx -= ptSpeed;

		}
		else if (wParam == 'A') {  //A
			ptx += ptSpeed;
		}
		else if (wParam == 'S') {  //S
			pty += ptSpeed;


		}
		else if (wParam == 'W') {  //w
			pty -= ptSpeed;
		}
		else if (wParam == VK_LEFT) {  //left key
			tx -= ptSpeed;
		}
		else if (wParam == VK_RIGHT) {  //right key
			tx += ptSpeed;
		}

		else if (wParam == 'O')			// O
		{
			isOrtho = true;
			tz = 0.0;
		}
		else if (wParam == 'P')			//P
		{
			isOrtho = false;
			tz = 0.0;

		}
		else if (wParam == 'B')			//B 
		{
			bodyChange += 1;
		}
		else if (wParam == 'L')			//L 
		{
			isLightOn = !isLightOn;
		}

		//------------------------------------
		else if (wParam == 0x31)//1
		{
			glEnable(GL_LIGHT0);
			glDisable(GL_LIGHT1);
		}
		else if (wParam == 0x32)//2
		{
			glEnable(GL_LIGHT1);
			glDisable(GL_LIGHT0);
		}
		else if (wParam == 0x33)//3
		{
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void projection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  // reset projection

	glTranslatef(ptx, pty, 0.0);
	glRotatef(pry, 0.0, 1.0, 0.0);

	if (isOrtho)
	{
		//glOrtho(-8.0, 8.0, -8.0, 8.0, oNear, oFar);
		glOrtho(-8.0, 8.0, -8.0, 8.0, 1, -50);

		//float oNear = -8.0, oFar = 8.0;
		//float pNear = 1.0, pFar = 20.0;
	}
	else {
		//gluPerspective(fovy, 1.0, -1.0, 1.0);
		//glFrustum(-20.0, 20.0, -20.0, 20.0, pNear, pFar); /

		gluPerspective(fovy, 1.0, 1.0, -1.0);
		glFrustum(-20.0, 20.0, -20.0, 20.0, pNear, 50);



	}
}

void lighting() {

	if (isLightOn) {
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHTING);
	}


	//light 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_POSITION, posA);
	glEnable(GL_LIGHT0);

	//light 1
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT1, GL_POSITION, posD);
	glEnable(GL_LIGHT1);

}

void Cylinder(float br, float tr, float h) {
	GLUquadricObj* cylinder = NULL; //create quadric obj pointer
	cylinder = gluNewQuadric(); //create object
	gluQuadricDrawStyle(cylinder, GLU_FILL);//set the drawstyle
	gluCylinder(cylinder, br, tr, h, 30, 30);  //draw cylinder
	gluDeleteQuadric(cylinder);	//free used memory
}

void Cylinder3(float br, float tr, float h) {
	GLUquadricObj* cylinder = NULL; //create quadric obj pointer
	cylinder = gluNewQuadric(); //create object
	gluQuadricDrawStyle(cylinder, GLU_FILL);//set the drawstyle
	gluQuadricTexture(cylinder, true);
	gluCylinder(cylinder, br, tr, h, 30, 30);  //draw cylinder
	gluDeleteQuadric(cylinder);	//free used memory
}

void Sphere(float r, int sl, int st) {
	GLUquadricObj* sphere = NULL; //create quadric obj pointer
	sphere = gluNewQuadric(); //create object
	gluQuadricDrawStyle(sphere, GLU_FILL);//set the drawstyle
	gluSphere(sphere, r /* size(radius) */, sl /* slide */, st /* stack */); //draw sphere
	gluDeleteQuadric(sphere);	//free used memory

}

void Cube(float size) {
	//glBegin(GL_LINE_LOOP);
	glBegin(GL_QUADS);
	// Face 1 (Bottom (Y - Axis)
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	//glBegin(GL_LINE_LOOP);
	glBegin(GL_QUADS);
	// Face 2 LEFT (X - Axis)
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(0.0f, size, size);
	glVertex3f(0.0f, 0.0f, size);

	// Face 3 TOP (Z - Axis)
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(0.0f, size, size);
	glVertex3f(size, size, size);
	glVertex3f(size, 0.0f, size);

	// Face 4 RIGHT (X - Axis)
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, size, size);
	glVertex3f(size, size, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);

	// Face 5 RIGHT (X - Axis)
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(size, size, 0.0f);

	// Face 6 RIGHT (X - Axis)
	glVertex3f(size, size, 0.0f);
	glVertex3f(size, size, size);
	glVertex3f(0.0f, size, size);
	glVertex3f(0.0f, size, 0.0f);
	glEnd();
}

void Pyramid(float size) {

	glBegin(GL_TRIANGLE_STRIP);
	//glBegin(GL_LINE_LOOP);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, size);

	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, size);

	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, 0.0f);

	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

}

void drawPyramid(float size) {

	// Face 1 bottom (y axis)
	//glColor3f(1.0, 0.0, 0.0);  //red
	glBegin(GL_QUADS);
	//base
	//glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.0f, 0.0f, size); //pt 1
	glVertex3f(size, 0.0f, size); //pt 2
	glVertex3f(size, 0.0f, 0.0f); //pt 3
	glVertex3f(0.0f, 0.0f, 0.0f); //pt4
	glEnd();

	glBegin(GL_QUADS);
	//first triangle
	//glColor3f(0.0, 1.0, 0.0); //green
	glVertex3f(0.0f, 0.0f, 0.0f); //pt4
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	//second triangle
	//glColor3f(0.0, 0.0, 1.0); //blue
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);

	//third triangle
	//glColor3f(1.0, 1.0, 0.0); //yello2
	glVertex3f(size, 0.0f, size);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);

	//forth triangle
	//glColor3f(0.0, 1.0, 1.0); //cyan
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size / 2, size, size / 2);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, size);

	glEnd();

} //use

void drawPyramid2(float size, float height) {

	// Face 1 bottom (y axis)
	//glColor3f(1.0, 0.0, 0.0);  //red
	glTranslatef(-size / 2, 0.0, -size / 2);
	glBegin(GL_QUADS);
	//base
	//glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.0f, 0.0f, size); //pt 1
	glVertex3f(size, 0.0f, size); //pt 2
	glVertex3f(size, 0.0f, 0.0f); //pt 3
	glVertex3f(0.0f, 0.0f, 0.0f); //pt4
	glEnd();


	glBegin(GL_QUADS);
	//first triangle
	//glColor3f(0.0, 1.0, 0.0); //green
	glVertex3f(0.0f, 0.0f, 0.0f); //pt4
	glVertex3f(size / 2, height, size / 2);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	//second triangle
	//glColor3f(0.0, 0.0, 1.0); //blue
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(size / 2, height, size / 2);
	glVertex3f(size, 0.0f, size);
	glVertex3f(size, 0.0f, 0.0f);

	//third triangle
	//glColor3f(1.0, 1.0, 0.0); //yello2
	glVertex3f(size, 0.0f, size);
	glVertex3f(size / 2, height, size / 2);
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size, 0.0f, size);

	//forth triangle
	//glColor3f(0.0, 1.0, 1.0); //cyan
	glVertex3f(0.0f, 0.0f, size);
	glVertex3f(size / 2, height, size / 2);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, size);

	glEnd();
}

void drawTrianglePrism(float length, float height, float width) {

	//face front -tri
	//glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(length / 2, 0.0, 0.0);
	glVertex3f(length, height, 0.0);
	glVertex3f(0.0, height, 0.0);
	glEnd();

	//face up -quad
	//glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(length, height, 0.0);
	glVertex3f(0.0, height, 0.0);
	glVertex3f(0.0, height, width);
	glVertex3f(length, height, width);
	glEnd();

	//face back - tri
	//glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0, height, width);
	glVertex3f(length, height, width);
	glVertex3f(length / 2, 0.0, width);
	glEnd();

	//face left down -quad
	//glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.0, height, width);
	glVertex3f(length / 2, 0.0, width);
	glVertex3f(length / 2, 0.0, 0.0);
	glVertex3f(0.0, height, 0.0);
	glEnd();

	//face right down -quad
	//glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(length, height, width);
	glVertex3f(length / 2, 0.0, width);
	glVertex3f(length / 2, 0.0, 0.0);
	glVertex3f(length, height, 0.0);
	glEnd();


}

void drawCuboid3(float size, float length, float width, int mode) {
	// height, length, width
	//mode 1 = quad, 2 = line

	glPushMatrix();
	glTranslatef(-size / 2, -length / 2, -width / 2);

	switch (mode) {
	case 1:
		glBegin(GL_QUADS);
		break;
	case 2:
		glBegin(GL_LINE_STRIP);
		glLineWidth(1.0);
		break;
	}


	// Face 1 bottom (y axis)  face me
	//glColor3f(1.0, 0.0, 0.0);  //red
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0);
	glVertex3f(length, size, 0.0f);
	glVertex3f(length, 0.0f, 0.0f);

	// Face 2 left (x axis)
	//glColor3f(0.0, 1.0, 0.0);  //green face right
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(length, size, 0.0f);
	glVertex3f(length, size, width);
	glVertex3f(length, 0.0f, width);

	// Face 3 top (z axis)    // face down
	//glColor3f(0.0, 0.0, 1.0);  //blue
	glVertex3f(length, 0.0f, width);
	glVertex3f(length, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, width);

	// Face 4 right (x axis)
	//glColor3f(1.0, 0.5, 0.0);  //orange face left
	glVertex3f(0.0f, 0.0f, width);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(0.0f, size, width);

	// Face 5 back (z axis)
	//glColor3f(1.0, 1.0, 0.0);  //yellow face up
	glVertex3f(0.0f, size, width);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(length, size, 0.0f);
	glVertex3f(length, size, width);

	// Face 6 front (z axis)
	//glColor3f(1.0, 0.0, 1.0);  //magenta
	glVertex3f(0.0f, size, width);
	glVertex3f(length, size, width);
	glVertex3f(length, 0.0f, width);
	glVertex3f(0.0f, 0.0f, width);
	glVertex3f(0.0f, size, width);
	glEnd();

	glPopMatrix();

}

void drawCuboid4(float size, float length, float width, int mode) {
	// height, length, width
	//mode 1 = quad, 2 = line

	glPushMatrix();
	glTranslatef(-size / 2, -length / 2, -width / 2);

	switch (mode) {
	case 1:
		glBegin(GL_QUADS);
		break;
	case 2:
		glBegin(GL_LINE_STRIP);
		glLineWidth(1.0);
		break;
	}


	// Face 1 bottom (y axis)  face me
	//glColor3f(1.0, 0.0, 0.0);  //red
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, size, 0.0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(length, size, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(length, 0.0f, 0.0f);

	// Face 2 left (x axis)
	//glColor3f(0.0, 1.0, 0.0);  //green face right
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(length, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(length, size, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(length, size, width);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(length, 0.0f, width);

	// Face 3 top (z axis)    // face down
	//glColor3f(0.0, 0.0, 1.0);  //blue
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(length, 0.0f, width);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(length, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, width);

	// Face 4 right (x axis)
	//glColor3f(1.0, 0.5, 0.0);  //orange face left
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, width);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, size, width);

	// Face 5 back (z axis)
	//glColor3f(1.0, 1.0, 0.0);  //yellow face up
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, size, width);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(length, size, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(length, size, width);

	// Face 6 front (z axis)
	//glColor3f(1.0, 0.0, 1.0);  //magenta
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, size, width);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(length, size, width);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(length, 0.0f, width);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, width);
	glVertex3f(0.0f, size, width);
	glEnd();

	glPopMatrix();

}

void CubeBackGround(float size) {

	glBegin(GL_QUADS);

	// Face 1 (Bottom (Y - Axis)

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, size);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, 0.0f, size);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);
	// Face 2 LEFT (X - Axis)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, size);

	// Face 3 TOP (Z - Axis)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, size);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, size, size);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(size, 0.0f, size);

	// Face 4 RIGHT (X - Axis)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(size, 0.0f, size);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, size);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(size, size, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);

	// Face 5 RIGHT (X - Axis)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(size, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(size, size, 0.0f);

	// Face 6 RIGHT (X - Axis)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(size, size, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(size, size, size);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, size, size);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glEnd();
}

GLuint loadtexture(LPCSTR filename) {
	GLuint texture = 0;

	//step 3
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
		filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION |
		LR_LOADFROMFILE);

	GetObject(hBMP, sizeof(BMP), &BMP); //create object in memory to make handler refer to this object

	//step 4
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth,
		BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	DeleteObject(hBMP);

	return texture;
}

void drawTrianglePrism2(float length, float height, float width) {

	//face front -tri
	//glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(length / 2, 0.0, 0.0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(length, height, 0.0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0, height, 0.0);
	glEnd();

	//face up -quad
	//glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(length, height, 0.0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0, height, 0.0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0, height, width);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(length, height, width);
	glEnd();

	//face back - tri
	//glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0, height, width);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(length, height, width);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(length / 2, 0.0, width);
	glEnd();

	//face left down -quad
	//glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0.0, height, width);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(length / 2, 0.0, width);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(length / 2, 0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0, height, 0.0);
	glEnd();

	//face right down -quad
	//glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(length, height, width);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(length / 2, 0.0, width);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(length / 2, 0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(length, height, 0.0);
	glEnd();


}

void Cone(float x, float y) {
	GLUquadricObj* cone = NULL; //create quadric obj pointer
	cone = gluNewQuadric(); //create object
	gluQuadricDrawStyle(cone, GLU_FILL);//set the drawstyle
	gluCylinder(cone, 0.0, x, y, 30, 30);  //draw cone
	gluDeleteQuadric(cone);	//free used memory
}

void RobotLeg() {
	glMatrixMode(GL_MODELVIEW);

	//===================
	//Waist
	//===================
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);//white
	glRotatef(90, 1.0, 0.0, 0.0);
	Cone(0.35, 0.3);//Cone
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);//white
	glTranslatef(0.0, -0.3, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	Cylinder(0.38, 0.38, 0.8); //Cylinder
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);//white
	glTranslatef(0.0, -1.18, 0.0);
	Sphere(0.38, 30, 30);//Sphere
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);//white
	glTranslatef(-1.55, 0.6, -0.15);
	glScalef(0.9, 0.9, 0.9);
	Pyramid(0.3);//Pyramid
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);//white
	glTranslatef(-1.55, 0.15, -0.15);
	glScalef(0.9, 1.5, 0.9);
	Cube(0.3);//Cube change to rectangle
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);//white
	glTranslatef(-0.24, -0.35, -0.15);
	glRotatef(70, 0.0, 0.0, 1.0);
	glScalef(0.9, 4.5, 0.9);
	Cube(0.3);//Cube
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);//white
	glTranslatef(1.25, 0.6, -0.15);
	glScalef(0.9, 0.9, 0.9);
	Pyramid(0.3);//Pyramid
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);//white
	glTranslatef(1.25, 0.15, -0.15);
	glScalef(0.9, 1.5, 0.9);
	Cube(0.3);//Cube change to rectangle
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);//white
	glTranslatef(0.13, -0.1, -0.15);
	glRotatef(-70, 0.0, 0.0, 1.0);
	glScalef(0.9, 4.5, 0.9);
	Cube(0.3);//Cube
	glPopMatrix();


	//============================================
	//Left Side Leg
	//============================================
	glPushMatrix();
	glColor3f(0, 0, 0);//black
	glTranslatef(-0.37, -0.9, 0.0);
	glScalef(1.2, 1.0, 1.0);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();
	if (walk == true && walkUndo % 2 == 1) {
		if (w >= 50) {
			w -= 0.2;
			walkBack = 1;
		}
		else if (walkBack == 1 && w > -50) {
			//Sleep(1);
			w -= 0.2;
		}
		else {
			//Sleep(1);
			w += 0.2;
			walkBack = 0;
		}
	}

	glPushMatrix();
	glTranslatef(-0.5, -0.9, 0.0);
	glRotatef(w, 1.0, 0.0, 0.0);
	glPushMatrix();

	if (bodyChange % 2 == 1) {

		textureArray[1] = loadtexture("body.bmp");

		glPushMatrix();
		glColor3f(1, 1, 1);//white
		glTranslatef(-0.7, 0.0, -0.39);
		glScalef(0.8, 1.5, 1.55);
		Pyramid(0.5);//Pyramid
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 1);//white
		glTranslatef(-0.75, -2.75, -0.4);
		CubeBackGround(0.8);//Cube
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.7, -4.0, -0.35);
		glScalef(1, 1.4, 1);
		CubeBackGround(0.7); //Cube set to rectangle
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0); //white
		glTranslatef(0.0, 0.0, -0.35);
		glRotatef(180, 0, 0, 1);
		glTranslatef(0.7, 1.6, 0);
		glScalef(-1.0, -2.3, 1.0);
		CubeBackGround(0.7); //Cube set to rectangle
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.55, -3.45, 0.3);
		glScalef(2.0, 1.0, 1.2);
		CubeBackGround(0.2);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.55, -3.25, 0.35);
		glScalef(2.0, 3.0, 1.2);
		Pyramid(0.2);
		glPopMatrix();


		glPushMatrix();
		glColor3f(1, 1, 1);//white
		glTranslatef(-0.35, -4.25, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		Cylinder3(0.4, 0.4, 1.6); //Cylinder
		glPopMatrix();

		glDeleteTextures(1, &textureArray[1]);
		glDisable(GL_TEXTURE_2D);

	}
	else {

		glPushMatrix();
		glColor3f(1, 1, 1);//white
		glTranslatef(-0.7, 0.0, -0.39);
		glScalef(0.8, 1.5, 1.55);
		Pyramid(0.5);//Pyramid
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 1);//white
		glTranslatef(-0.75, -2.75, -0.4);
		Cube(0.8);//Cube
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.7, -4.0, -0.35);
		glScalef(1, 1.4, 1);
		Cube(0.7); //Cube set to rectangle
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0); //white
		glTranslatef(0.0, 0.0, -0.35);
		glScalef(-1.0, -2.3, 1.0);
		Cube(0.7); //Cube set to rectangle
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.55, -3.45, 0.3);
		glScalef(2.0, 1.0, 1.2);
		Cube(0.2);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.55, -3.25, 0.35);
		glScalef(2.0, 3.0, 1.2);
		Pyramid(0.2);
		glPopMatrix();


		glPushMatrix();
		glColor3f(1, 1, 1);//white
		glTranslatef(-0.35, -4.25, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		Cylinder(0.4, 0.4, 1.6); //Cylinder
		glPopMatrix();
		glPushMatrix();
		glColor3f(1, 1, 1);//white
		glTranslatef(-0.35, -4.25, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		Cylinder(0.4, 0.4, 1.6); //Cylinder
		glPopMatrix();
	}



	//====================================

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0); //black
	glTranslatef(-0.35, -2.9, 0.0);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);//black
	glTranslatef(-0.35, -1.8, 0.0);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);//black
	glTranslatef(-0.35, -4.1, 0.0);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();

	//============================================
	//Left Side Foot
	//============================================
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);//white
	glTranslatef(-0.75, -6.03, -0.4);
	glScalef(2.65, 0.6, 4.5);
	Cube(0.3);//Cube
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow
	glTranslatef(-0.4, -5.85, 0.9);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.3, 1.2, 0.6);
	Pyramid(0.3);//Pyramid
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow
	glTranslatef(-0.65, -5.85, 0.9);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.3, 1.2, 0.6);
	Pyramid(0.3);//Pyramid
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow
	glTranslatef(-0.15, -5.85, 0.9);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.3, 1.2, 0.6);
	Pyramid(0.3);//Pyramid
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//============================================
	//Leg(Right Side)
	//============================================
	glPushMatrix();
	glColor3f(0.0, 0, 0);//black
	glTranslatef(0.37, -0.9, 0.0);
	glScalef(1.2, 1.0, 1.0);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, -0.9, 0.0);
	glRotatef(-w, 1.0, 0.0, 0.0);
	glPushMatrix();

	if (bodyChange % 2 == 1) {

		textureArray[1] = loadtexture("body.bmp");

		glPushMatrix();
		glColor3f(1, 1, 1);//white
		glTranslatef(0.3, 0.0, -0.39);
		glScalef(0.8, 1.5, 1.55);
		Pyramid(0.5);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 1);//white
		glTranslatef(-0.05, -2.75, -0.4);
		CubeBackGround(0.8);	//Cube
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 1);//white
		glTranslatef(0.35, -4.25, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		Cylinder3(0.4, 0.4, 1.6); //Cylinder
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.0, -4.0, -0.35);
		glScalef(1, 1.4, 1);
		CubeBackGround(0.7); // Cube set to rectangle
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.0, 0.0, -0.35);
		glScalef(1.0, -2.3, 1.0);
		CubeBackGround(0.7);//Cube set to rectangle
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.15, -3.45, 0.3);
		glScalef(2.0, 1.0, 1.2);
		CubeBackGround(0.2);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.15, -3.25, 0.35);
		glScalef(2.0, 3.0, 1.2);
		Pyramid(0.2);
		glPopMatrix();

		glDeleteTextures(1, &textureArray[1]);
		glDisable(GL_TEXTURE_2D);

	}
	else {

		glPushMatrix();
		glColor3f(1, 1, 1);//white
		glTranslatef(0.3, 0.0, -0.39);
		glScalef(0.8, 1.5, 1.55);
		Pyramid(0.5);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 1);//white
		glTranslatef(-0.05, -2.75, -0.4);
		Cube(0.8);	//Cube
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 1);//white
		glTranslatef(0.35, -4.25, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		Cylinder(0.4, 0.4, 1.6); //Cylinder
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.0, -4.0, -0.35);
		glScalef(1, 1.4, 1);
		Cube(0.7); // Cube set to rectangle
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.0, 0.0, -0.35);
		glScalef(1.0, -2.3, 1.0);
		Cube(0.7);//Cube set to rectangle
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.15, -3.45, 0.3);
		glScalef(2.0, 1.0, 1.2);
		Cube(0.2);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.15, -3.25, 0.35);
		glScalef(2.0, 3.0, 1.2);
		Pyramid(0.2);
		glPopMatrix();
	}


	//==============================
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0); //black
	//glLoadIdentity();
	glTranslatef(0.35, -2.9, 0.0);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);//black
	glTranslatef(0.35, -4.1, 0.0);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);//black
	glTranslatef(0.35, -1.8, 0.0);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();

	//============================================
	//Foot(Right Side)
	//============================================
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);//white
	glTranslatef(-0.05, -6.03, -0.4);
	glScalef(2.65, 0.6, 4.5);
	Cube(0.3);//cube
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow
	glTranslatef(0.3, -5.85, 0.9);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.3, 1.2, 0.6);
	Pyramid(0.3);//pyramid
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow
	glTranslatef(0.55, -5.85, 0.9);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.3, 1.2, 0.6);
	Pyramid(0.3);//pyramid
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow
	glTranslatef(0.05, -5.85, 0.9);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.3, 1.2, 0.6);
	Pyramid(0.3);//pyramid
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void SwordWord() {
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow  |
	glTranslatef(-0.157, -0.55, -0.08);
	glScalef(1.15, 1.6, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow  -
	glTranslatef(-0.157, -0.26, -0.20);
	glScalef(1.15, 0.2, 0.8);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow |
	glTranslatef(-0.157, -0.4, -0.20);
	glScalef(1.15, 0.7, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow -
	glTranslatef(-0.157, -0.42, -0.18);
	glScalef(1.15, 0.2, 1.4);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow |
	glTranslatef(-0.157, -0.4, 0.08);
	glScalef(1.15, 0.7, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow  -
	glTranslatef(-0.157, -0.25, 0.015);
	glScalef(1.15, 0.2, 0.5);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow |
	glTranslatef(-0.157, -0.32, 0.0);
	glScalef(1.15, 0.5, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow  -
	glTranslatef(-0.157, -0.33, 0.015);
	glScalef(1.15, 0.2, 0.9);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow  |
	glTranslatef(-0.157, -0.15, -0.025);
	glScalef(1.15, 1.2, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow  |
	glTranslatef(-0.157, 0.08, 0.02);
	glRotatef(-135, 1.0, 0.0, 0.0);
	glScalef(1.15, 0.8, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow  |
	glTranslatef(-0.157, 0.1, 0.015);
	glRotatef(135, 1.0, 0.0, 0.0);
	glScalef(1.15, 0.8, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow  -
	glTranslatef(-0.157, 0.25, 0.0);
	glScalef(1.15, 0.2, 0.6);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow |
	glTranslatef(-0.157, 0.25, 0.09);
	glScalef(1.15, 0.7, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow /
	glTranslatef(-0.157, 0.37, 0.09);
	glRotatef(-45, 1.0, 0.0, 0.0);
	glScalef(1.15, 0.7, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow |
	glTranslatef(-0.157, 0.25, 0.0);
	glScalef(1.15, 0.7, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow \ */
	glTranslatef(-0.157, 0.4, 0.0);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(1.15, 0.7, 0.2);
	Cube(0.2);
	glPopMatrix();

	//==
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow \ */
	glTranslatef(-0.157, 0.35, -0.16);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(1.15, 1.0, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow |
	glTranslatef(-0.157, 0.17, -0.06);
	glScalef(1.15, 0.9, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow  -
	glTranslatef(-0.157, 0.16, -0.15);
	glScalef(1.15, 0.2, 0.6);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow |
	glTranslatef(-0.157, 0.17, -0.16);
	glScalef(1.15, 0.9, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow /
	glTranslatef(-0.157, 0.33, -0.06);
	glRotatef(-45, 1.0, 0.0, 0.0);
	glScalef(1.15, 1.0, 0.2);
	Cube(0.2);
	glPopMatrix();
	//   / \  */
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow /
	glTranslatef(-0.157, 0.57, 0.08);
	glRotatef(-40, 1.0, 0.0, 0.0);
	glScalef(1.15, 0.5, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow \ */
	glTranslatef(-0.157, 0.60, -0.14);
	glRotatef(40, 1.0, 0.0, 0.0);
	glScalef(1.15, 0.5, 0.2);
	Cube(0.2);
	glPopMatrix();

	//-----
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow /
	glTranslatef(-0.157, 0.71, 0.1);
	glRotatef(-15, 1.0, 0.0, 0.0);
	glScalef(1.15, 1.0, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow \ */
	glTranslatef(-0.157, 0.75, -0.04);
	glRotatef(30, 1.0, 0.0, 0.0);
	glScalef(1.15, 1.0, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow /
	glTranslatef(-0.157, 0.72, -0.02);
	glRotatef(-30, 1.0, 0.0, 0.0);
	glScalef(1.15, 1.0, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow \ */
	glTranslatef(-0.157, 0.71, -0.18);
	glRotatef(15, 1.0, 0.0, 0.0);
	glScalef(1.15, 1.0, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow \ */
	glTranslatef(-0.157, 0.92, -0.04);
	glRotatef(30, 1.0, 0.0, 0.0);
	glScalef(1.15, 1.0, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow /
	glTranslatef(-0.157, 0.9, -0.02);
	glRotatef(-30, 1.0, 0.0, 0.0);
	glScalef(1.15, 1.0, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow  |
	glTranslatef(-0.157, 1.18, -0.025);
	glScalef(1.15, 1.2, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow  |
	glTranslatef(-0.157, 1.39, 0.02);
	glRotatef(-135, 1.0, 0.0, 0.0);
	glScalef(1.15, 0.8, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow  |
	glTranslatef(-0.157, 1.41, 0.015);
	glRotatef(135, 1.0, 0.0, 0.0);
	glScalef(1.15, 0.8, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow -
	glTranslatef(-0.157, 1.55, -0.15);
	glScalef(1.15, 0.2, 1.4);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow |
	glTranslatef(-0.157, 1.57, 0.085);
	glScalef(1.15, 1.1, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow -
	glTranslatef(-0.157, 1.74, -0.08);
	glScalef(1.15, 0.2, 0.9);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow |
	glTranslatef(-0.157, 1.65, -0.115);
	glScalef(1.15, 0.6, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow -
	glTranslatef(-0.157, 1.64, -0.075);
	glScalef(1.15, 0.2, 0.5);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow |
	glTranslatef(-0.159, 1.64, 0.01);
	glScalef(1.15, 1.15, 0.2);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow -
	glTranslatef(-0.157, 1.84, -0.15);
	glScalef(1.15, 0.2, 0.8);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow |
	glTranslatef(-0.157, 1.49, -0.05);
	glScalef(1.15, 2.3, 0.2);
	Cube(0.2);
	glPopMatrix();
}

void Sword() {
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glPushMatrix();
	glTranslatef(2.4455, -3.5, 0.0);

	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);//gray
	glTranslatef(-0.15, -0.6, -0.35);
	glScalef(0.4, 5.5, 1.4);
	Cube(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);//black
	glTranslatef(-0.155, -0.6, -0.255);
	glScalef(0.45, 5.5, 1.0);
	Cube(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);//gray
	glTranslatef(-0.15, -0.6, 0.35);
	glRotatef(-180, 1.0, 0.0, 0.0);
	glScalef(0.4, 1.2, 1.4);
	Pyramid(0.5);
	glPopMatrix();

	SwordWord();

	glPushMatrix();
	glColor3f(0.75, 0.75, 0.75);
	glTranslatef(-0.05, -0.62, 0.35);
	glScalef(0.15, 18.5, 0.15);
	Cube(0.15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.75, 0.75, 0.75);//0.62
	glTranslatef(-0.05, -0.62, -0.38);
	glScalef(0.15, 18.5, 0.15);
	Cube(0.15);
	glPopMatrix();

	glPopMatrix();
}

void WargreymonHead() {
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glTranslatef(3.3, -0.05, 2.0);
	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);//Gray
	glTranslatef(1.9, -0.42, 0.57);
	glRotatef(50, 1.0, 0.0, 0.0);
	glScalef(0.8, 1.7, 0.8);
	Pyramid(0.3);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);//Gray
	//glTranslatef(1.9, -0.6, 0.5);
	glTranslatef(1.9, -0.6, 20);
	glScalef(1.0, 1.2, 1.0);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glRotatef(45, 0.0, 1.0, 0.0);
	glTranslatef(0.8, 0.05, 1.05);
	glScalef(2.0, 0.9, 2.0);
	Cube(0.5);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	glPushMatrix();
	glColor3f(0.37, 0.37, 0.37);//brown
	glRotatef(45, 0.0, 1.0, 0.0);
	glTranslatef(0.8, -0.3, 1.05);
	glScalef(2.0, 0.7, 2.0);
	Cube(0.5);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.22, 0.72, 0.0);
	glRotatef(45, 0.0, 0.0, 1.0);
	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(0.7, -2.8, 0.06);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glScalef(0.8, 1.7, 0.8);
	Pyramid(0.3);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1, -0.08, 0.0);
	glRotatef(-45, 0.0, 0.0, 1.0);
	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(1.45, 0.7, 0.06);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glScalef(0.8, 1.7, 0.8);
	Pyramid(0.3);
	glPopMatrix();
	glPopMatrix();

	//============
	//eye
	//============

	glPushMatrix();
	glTranslatef(-0.81, 0.03, -0.05);
	glRotatef(45, 0.0, 0.0, 1.0);
	glPushMatrix();
	glRotatef(-90, 0.0, 1.0, 0.0);
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);//black
	glTranslatef(0.3, -2.1, -1.4);
	glRotatef(-45, 0.0, 0.0, 1.0);
	glScalef(0.8, 0.8, 0.4);
	Cube(0.2);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.8, 0.0, 0.65);
	glRotatef(-45, 0.0, 0.0, 1.0);
	glPushMatrix();
	glRotatef(90, 0.0, 1.0, 0.0);
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);//black
	glTranslatef(0.3, -2.1, -1.4);
	glRotatef(45, 0.0, 0.0, 1.0);
	glScalef(0.8, 0.8, 0.4);
	Cube(0.2);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//=========================
	//right side horn
	//=========================
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);//Gray
	glTranslatef(2.65, 0.04, 0.4);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.65, 1.4, 0.65);
	Pyramid(0.3);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);//Gray
	glTranslatef(2.51, -0.15, 0.2);
	glScalef(1.5, 1.0, 1.0);
	Cube(0.2);
	glPopMatrix();

	//=========================
	//left side horn
	//=========================
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);//Gray
	glTranslatef(1.2, 0.04, 0.4);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.65, 1.4, 0.65);
	Pyramid(0.3);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);//Gray
	glTranslatef(1.22, -0.15, 0.2);
	glScalef(1.5, 1.0, 1.0);
	Cube(0.2);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}

void RightShield() {

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glTranslatef(0.6, 2.0, -0.05);
	glPushMatrix();
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow
	glTranslatef(2.5, 0.5, -0.35);
	glScalef(0.2, 1.5, 1.5);
	Cube(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow
	glTranslatef(2.5, 1.25, -0.36);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(0.2, 1.1, 1.1);
	Cube(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow
	glTranslatef(2.5, 0.5, -0.36);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(0.2, 1.1, 1.1);
	Cube(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(2.55, 0.87, 0.0);
	Sphere(0.07, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(2.5, 1.25, -0.1);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(0.27, 0.35, 0.35);
	Cube(0.4);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(2.5, 0.5, -0.1);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(0.27, 0.35, 0.35);
	Cube(0.4);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(2.5, 0.87, 0.15);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(0.27, 0.35, 0.35);
	Cube(0.4);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(2.5, 0.87, -0.35);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(0.27, 0.35, 0.35);
	Cube(0.4);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(2.5, 0.87, 0.15);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(0.27, 0.35, 0.35);
	Cube(0.4);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

}

void LeftShield() {
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glTranslatef(-5.7, 2.0, 0.0);
	glPushMatrix();
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow
	glTranslatef(2.5, 0.5, -0.35);
	glScalef(0.2, 1.5, 1.5);
	Cube(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow
	glTranslatef(2.5, 1.25, -0.36);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(0.2, 1.1, 1.1);
	Cube(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);//yellow
	glTranslatef(2.5, 0.5, -0.36);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(0.2, 1.1, 1.1);
	Cube(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(2.54, 0.87, 0.0);
	Sphere(0.07, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(2.49, 1.25, -0.1);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(0.27, 0.35, 0.35);
	Cube(0.4);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(2.49, 0.5, -0.1);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(0.27, 0.35, 0.35);
	Cube(0.4);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(2.49, 0.87, 0.15);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(0.27, 0.35, 0.35);
	Cube(0.4);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(2.49, 0.87, -0.35);
	glRotatef(45, 1.0, 0.0, 0.0);
	glScalef(0.27, 0.35, 0.35);
	Cube(0.4);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}

void RightHand() {
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glTranslatef(-2.3, -2.65, -0.2);
	WargreymonHead();


	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(2.51, 1.3, 0.0);
	glScalef(1.1, 1.1, 1.1);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();



	//============
	//hand
	//============
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(-2.75, 2.6, 0.3);
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);//black
	glTranslatef(0.21, -3.2, -0.27);
	glScalef(0.9, 0.9, 0.9);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);//white
	glTranslatef(0.0, -3.4, -0.1);
	glRotatef(180, 1.0, 0.0, 0.0);
	glScalef(0.8, 1.5, 1.3);
	Cube(0.3);
	glPopMatrix();

	if (fingerUndo % 2 == 1 && fingerRotate < 125) {
		fingerRotate += 0.1;

	}
	else if (fingerUndo % 2 == 1 && fingerRotate >= 125) {
		fingerRotate = 125;
	}
	else if (fingerUndo % 2 == 0 && fingerRotate <= 125 && fingerRotate >= 0) {

		fingerRotate -= 0.1;

	}
	else {
		fingerRotate = 0;
	}


	if (fingerMove == true) {
		glPushMatrix();
		glTranslatef(0.15, -3.9, -0.15);

		glPushMatrix();
		glRotatef(fingerRotate, 0, 0, 1);
		glTranslatef(-0.15, 3.9, 0.15);

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.15, -3.9, -0.15);
		glScalef(0.2, 0.2, 0.2);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.10, -3.95, -0.10);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glScalef(0.4, 0.75, 0.25);
		Cube(0.3);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.24, -4.2, -0.13);
		glScalef(0.15, 0.15, 0.15);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.19, -4.2, -0.10);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-40, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.6, 0.2);
		Cube(0.3);
		glPopMatrix();

		glPopMatrix();
		glPopMatrix();
	}

	else {
		//finger 1
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.15, -3.9, -0.15);
		glScalef(0.2, 0.2, 0.2);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.10, -3.95, -0.10);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glScalef(0.4, 0.75, 0.25);
		Cube(0.3);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.24, -4.2, -0.13);
		glScalef(0.15, 0.15, 0.15);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.19, -4.2, -0.10);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-40, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.6, 0.2);
		Cube(0.3);
		glPopMatrix();

	}

	//finger 2
	if (fingerMove == true) {
		glPushMatrix();
		glTranslatef(0.15, -3.9, -0.30);

		glPushMatrix();
		glRotatef(fingerRotate, 0, 0, 1);
		glTranslatef(-0.15, 3.9, 0.30);

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.15, -3.9, -0.30);
		glScalef(0.2, 0.2, 0.2);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.10, -3.95, -0.25);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glScalef(0.4, 0.75, 0.25);
		Cube(0.3);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.24, -4.2, -0.28);
		glScalef(0.15, 0.15, 0.15);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.19, -4.2, -0.25);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-40, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.6, 0.2);
		Cube(0.3);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.15, -3.9, -0.30);
		glScalef(0.2, 0.2, 0.2);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.10, -3.95, -0.25);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glScalef(0.4, 0.75, 0.25);
		Cube(0.3);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.24, -4.2, -0.28);
		glScalef(0.15, 0.15, 0.15);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.19, -4.2, -0.25);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-40, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.6, 0.2);
		Cube(0.3);
		glPopMatrix();
	}
	//finger 3
	if (fingerMove == true) {
		glPushMatrix();
		glTranslatef(0.15, -3.9, -0.45);

		glPushMatrix();
		glRotatef(fingerRotate, 0, 0, 1);
		glTranslatef(-0.15, 3.9, 0.45);

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.15, -3.9, -0.45);
		glScalef(0.2, 0.2, 0.2);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.10, -3.95, -0.4);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glScalef(0.4, 0.75, 0.25);
		Cube(0.3);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.24, -4.2, -0.43);
		glScalef(0.15, 0.15, 0.15);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.19, -4.2, -0.4);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-40, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.6, 0.2);
		Cube(0.3);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

	}
	else {
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.15, -3.9, -0.45);
		glScalef(0.2, 0.2, 0.2);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.10, -3.95, -0.4);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glScalef(0.4, 0.75, 0.25);
		Cube(0.3);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.24, -4.2, -0.43);
		glScalef(0.15, 0.15, 0.15);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.19, -4.2, -0.4);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-40, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.6, 0.2);
		Cube(0.3);
		glPopMatrix();
	}


	//finger 4
	glPushMatrix();
	glTranslatef(0, 0, -0.55);
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);//black
	glTranslatef(0.15, -3.6, 0.0);
	glScalef(0.2, 0.2, 0.2);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);//white
	glTranslatef(0.1, -3.65, 0.05);
	glRotatef(180, 1.0, 0.0, 0.0);
	glRotatef(-30, 0.0, 0.0, 1.0);
	glScalef(0.4, 0.75, 0.25);
	Cube(0.3);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	//===============
	glPopMatrix();

	if (bodyChange % 2 == 1) {
		textureArray[1] = loadtexture("body.bmp");

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.05, 0.0, 0.07);
		glRotatef(180, 1.0, 0.0, 0.0);
		glScalef(1.8, 10.0, 1.8);
		CubeBackGround(0.3);
		glPopMatrix();
		glDeleteTextures(1, &textureArray[0]);
		glDisable(GL_TEXTURE_2D);
	}
	else {
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.05, 0.0, 0.07);
		glRotatef(180, 1.0, 0.0, 0.0);
		glScalef(1.8, 10.0, 1.8);
		Cube(0.3);
		glPopMatrix();
	}


	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);//white
	glTranslatef(0.45, -2.7, -0.4);
	glScalef(0.5, 0.8, 0.8);
	Cube(0.5);
	glPopMatrix();

}

void LeftHand() {


	if (bodyChange % 2 == 1) {
		textureArray[1] = loadtexture("body.bmp");

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.05, 0.0, 0.0);
		glRotatef(180, 1.0, 0.0, 0.0);
		glScalef(1.8, 10.0, 1.8);
		CubeBackGround(0.3);
		glPopMatrix();
		glDeleteTextures(1, &textureArray[0]);
		glDisable(GL_TEXTURE_2D);
	}
	else {
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.05, 0.0, 0.0);
		glRotatef(180, 1.0, 0.0, 0.0);
		glScalef(1.8, 10.0, 1.8);
		Cube(0.3);
		glPopMatrix();
	}

	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(0.21, -1.4, -0.26);
	glScalef(1.1, 1.1, 1.1);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();

	//============
	//hand
	//============
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);//black
	glTranslatef(0.21, -3.2, -0.27);
	glScalef(0.9, 0.9, 0.9);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);//white
	glTranslatef(0.0, -3.4, -0.1);
	glRotatef(180, 1.0, 0.0, 0.0);
	glScalef(0.8, 1.5, 1.3);
	Cube(0.3);
	glPopMatrix();



	if (fingerUndo % 2 == 1 && fingerRotate < 125) {
		fingerRotate += 0.1;

	}
	else if (fingerUndo % 2 == 1 && fingerRotate >= 125) {
		fingerRotate = 125;
	}
	else if (fingerUndo % 2 == 0 && fingerRotate <= 125 && fingerRotate >= 0) {

		fingerRotate -= 0.1;

	}
	else {
		fingerRotate = 0;
	}

	if (fingerMove == true) {
		glPushMatrix();
		glTranslatef(0.15, -3.9, -0.15);

		glPushMatrix();
		glRotatef(fingerRotate, 0, 0, 1);

		//finger 1
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glScalef(0.2, 0.2, 0.2);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.05, -0.05, 0.05);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glScalef(0.4, 0.75, 0.25);
		Cube(0.3);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.09, -0.3, 0.02);
		glScalef(0.15, 0.15, 0.15);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();


		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.04, -0.3, 0.05);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-40, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.6, 0.2);
		Cube(0.3);
		glPopMatrix();

		glPopMatrix();
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glTranslatef(0.15, -3.9, -0.15);
		//finger 1
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glScalef(0.2, 0.2, 0.2);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.05, -0.05, 0.05);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glScalef(0.4, 0.75, 0.25);
		Cube(0.3);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.09, -0.3, 0.02);
		glScalef(0.15, 0.15, 0.15);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();


		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.04, -0.3, 0.05);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-40, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.6, 0.2);
		Cube(0.3);
		glPopMatrix();

		glPopMatrix();
	}

	//finger 2
	if (fingerMove == true) {

		glPushMatrix();
		glTranslatef(0.15, -3.9, -0.30);

		glPushMatrix();
		glRotatef(fingerRotate, 0, 0, 1);

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glScalef(0.2, 0.2, 0.2);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.05, -0.05, 0.05);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glScalef(0.4, 0.75, 0.25);
		Cube(0.3);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.08, -0.3, 0.02);
		glScalef(0.15, 0.15, 0.15);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.04, -0.3, 0.05);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-40, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.6, 0.2);
		Cube(0.3);
		glPopMatrix();

		glPopMatrix();
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.15, -3.9, -0.30);
		glScalef(0.2, 0.2, 0.2);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.10, -3.95, -0.25);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glScalef(0.4, 0.75, 0.25);
		Cube(0.3);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.24, -4.2, -0.28);
		glScalef(0.15, 0.15, 0.15);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.19, -4.2, -0.25);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-40, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.6, 0.2);
		Cube(0.3);
		glPopMatrix();
	}

	if (fingerMove == true) {

		glPushMatrix();
		glTranslatef(0.15, -3.9, -0.45);

		glPushMatrix();
		glRotatef(fingerRotate, 0, 0, 1);

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glScalef(0.2, 0.2, 0.2);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-0.05, -0.05, 0.05);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glScalef(0.4, 0.75, 0.25);
		Cube(0.3);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.09, -0.3, 0.02);
		glScalef(0.15, 0.15, 0.15);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.04, -0.3, 0.05);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-40, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.6, 0.2);
		Cube(0.3);
		glPopMatrix();

		glPopMatrix();
		glPopMatrix();
	}
	else {
		//finger 3
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.15, -3.9, -0.45);
		glScalef(0.2, 0.2, 0.2);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.10, -3.95, -0.4);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glScalef(0.4, 0.75, 0.25);
		Cube(0.3);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);//black
		glTranslatef(0.24, -4.2, -0.43);
		glScalef(0.15, 0.15, 0.15);
		Sphere(0.35, 30, 30);//Sphere
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(0.19, -4.2, -0.4);
		glRotatef(180, 1.0, 0.0, 0.0);
		glRotatef(-40, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.6, 0.2);
		Cube(0.3);
		glPopMatrix();
	}

	//finger 4
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);//black
	glTranslatef(0.15, -3.6, 0.0);
	glScalef(0.2, 0.2, 0.2);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);//white
	glTranslatef(0.1, -3.65, 0.05);
	glRotatef(180, 1.0, 0.0, 0.0);
	glRotatef(-30, 0.0, 0.0, 1.0);
	glScalef(0.4, 0.75, 0.25);
	Cube(0.3);
	glPopMatrix();


	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);//white
	glTranslatef(-0.3, -2.75, -0.47);
	glScalef(0.5, 0.8, 0.8);
	Cube(0.5);
	glPopMatrix();




}

void bullet() {

	if (gunUndo % 2 == 1 && armLeftUp % 2 == 1 && gunShot == true && gunChargeShot == false) {
		glPushMatrix();
		glTranslatef(-4, 0, 4 + gunShotSpeed);
		glPushMatrix();
		glColor3f(0.2, 0.2, 1);
		Sphere(0.35, 30, 30);
		glPopMatrix();
		glPopMatrix();
	}
	else {
		//gunShotSpeed = 0;
		gunChargeSize += 0.005;
		glPushMatrix();

		glTranslatef(-4, 0, 7.5);


		glColor3f(0.2, 0.2, 1);
		//gunShotSpeed += 0.01;

		if (gunChargeSize >= 4.0) {
			glTranslatef(0, 0, -37.5 + gunShotSpeed);
			Sphere(4.0, 30, 30);
		}
		else {

			Sphere(gunChargeSize, 30, 30);
		}

		glPopMatrix();
	}

}

void ungun() {

	glPushMatrix();
	glTranslatef(-1.5, 0.5, 0.0);
	//gray
	glPushMatrix();
	glColor3f(0.7, 0.7, 0.7);
	glTranslatef(-3.3, 0, -0.5);
	glRotatef(-180, 0, 0, 1);
	glScalef(1.5, 3, 2);
	Cube(0.5);
	glPopMatrix();

	//blue
	glPushMatrix();
	glColor3f(0, 0.4, 0.8);
	glTranslatef(-4, 0, -0.5);
	glRotatef(-180, 0, 0, 1);
	glScalef(1.5, 3, 2);
	Cube(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.25, 0.25, 0.25);
	glTranslatef(-4, 0, 0);
	glScalef(1.5, 1.5, 3);
	Sphere(0.2, 30, 30);
	glPopMatrix();

	//ear
	glPushMatrix();
	glColor3f(0, 0, 0.8);
	glTranslatef(-4.65, -0.5, -0.02);
	glRotatef(90, 0, 0, 1);
	Pyramid(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0.8);
	glTranslatef(-4.65, -0.5, -0.52);
	glRotatef(90, 0, 0, 1);
	Pyramid(0.5);
	glPopMatrix();

	//gun eyes
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-4.4, -1.2, 0.31);
	glRotatef(45, 0, 0, 1);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-4.4, -1.2, -0.51);
	glRotatef(45, 0, 0, 1);
	Cube(0.2);
	glPopMatrix();

	glPopMatrix();
}

void gun() {

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glColor3f(0.7, 0.7, 0.7);
	glTranslatef(-4, 0, -0.5);
	glRotatef(-70, 0, 0, 1);
	glScalef(3, 1.5, 2);
	Cube(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0.8);
	glTranslatef(-4.7, 0.22, -0.5);
	glRotatef(250, 0, 0, 1);
	glScalef(3, 1.5, 2);
	Cube(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.25, 0.25, 0.25);
	glTranslatef(-4, 0, 0);
	glScalef(1.5, 1.5, 3);
	Sphere(0.2, 30, 30);
	glPopMatrix();

	//===========
	//Cannon
	//===========
	glPushMatrix();
	glTranslatef(-0.45, 0.0, 0.0);
	glScalef(0.9, 0.8, 0.9);
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glTranslatef(-4.0, -0.1, 0);
	glRotatef(90, 1, 0, 0);
	Cylinder(0.3, 0.6, 1.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0, 0.8);
	glTranslatef(-4.0, -1.1, 0);
	glRotatef(90, 1, 0, 0);
	Cylinder(0.5, 0.5, 1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0.4, 0.4);
	glTranslatef(-4.0, -1.75, 0);
	glRotatef(90, 1, 0, 0);
	Cylinder(0.6, 0.3, 1.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.25, 0.25, 0.25);
	glTranslatef(-4, -3.0, 0);
	Sphere(0.33, 30, 30);
	glPopMatrix();
	glPopMatrix();
	//==============
	//gun eyes
	//==============
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-4.7, -1, 0.31);
	glRotatef(20, 0, 0, 1);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-4.7, -1, -0.51);
	glRotatef(20, 0, 0, 1);
	Cube(0.2);
	glPopMatrix();

	//teeth
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-4.2, -1, 0.28);
	glRotatef(110, 0, 0, 1);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-4.05, -0.6, 0.28);
	glRotatef(110, 0, 0, 1);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-3.55, -1.08, 0.28);
	glRotatef(230, 0, 0, 1);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-3.7, -0.7, 0.28);
	glRotatef(230, 0, 0, 1);
	Cube(0.2);
	glPopMatrix();

	//teeth
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-4.2, -1, -0.48);
	glRotatef(110, 0, 0, 1);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-4.05, -0.6, -0.48);
	glRotatef(110, 0, 0, 1);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-3.55, -1.08, -0.48);
	glRotatef(230, 0, 0, 1);
	Cube(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-3.7, -0.7, -0.48);
	glRotatef(230, 0, 0, 1);
	Cube(0.2);
	glPopMatrix();

	//ear
	glPushMatrix();
	glColor3f(0, 0, 0.8);
	glTranslatef(-4.8, -0.3, -0.02);
	glRotatef(70, 0, 0, 1);
	Pyramid(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0.8);
	glTranslatef(-4.8, -0.3, -0.52);
	glRotatef(70, 0, 0, 1);
	Pyramid(0.5);
	glPopMatrix();


}

void zero() {
	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-2.0, -6.5 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-2.075, -6.575 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-2.15, -6.65 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-2.15, -6.725 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-2.075, -6.8 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-2.0, -6.875 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	//===============================================
	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-1.925, -6.575 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-1.85, -6.65 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-1.85, -6.725 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-1.925, -6.8 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();
}

void one() {
	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-2.0, -6.5 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-2.1, -6.575 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-1.95, -6.75 + backgroundSpeed, 3);
	glScalef(1, 3, 1);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-2.1, -6.75 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-2.05, -6.8 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-1.9, -6.8 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.4, 0);
	glTranslatef(-1.81, -6.75 + backgroundSpeed, 3);
	Cube(0.1);
	glPopMatrix();
}

void backGround() {

	backgroundSpeed += 0.01;

	if (backgroundSpeed >= 15) {
		backgroundSpeed = 0;
	}

	if ((backgroundSpeed >= 2 && backgroundSpeed <= 4) || (backgroundSpeed >= 6 && backgroundSpeed <= 8) || (backgroundSpeed >= 10 && backgroundSpeed <= 12)) {
		//0
		zero();

		glPushMatrix();
		glTranslatef(7, 3, -2);
		one();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(5, 5, -4);
		zero();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(9, -3, -5);
		one();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-4, -1, -3.5);
		one();
		glPopMatrix();


	}
	else {//1

		one();

		glPushMatrix();
		glTranslatef(7, 3, -2);
		zero();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(5, 5, -4);
		one();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(9, -3, -5);
		zero();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-4, -1, -3.5);
		zero();
		glPopMatrix();
	}


}

void head() {

	//head sphere	
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	Sphere(0.8, 30, 30);
	glPopMatrix();

	//eye right
	glPushMatrix();
	glTranslatef(0.4, 0.2, 0.7);
	glColor3f(0.0, 0.0, 0.0);
	Sphere(0.2, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.4, 0.2, 0.85);
	glColor3f(1.0, 1.0, 0.0);
	Sphere(0.1, 30, 30);
	glPopMatrix();

	//eye left
	glPushMatrix();
	glTranslatef(-0.4, 0.2, 0.7);
	glColor3f(0.0, 0.0, 0.0);
	Sphere(0.2, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.4, 0.2, 0.85);
	glColor3f(1.0, 1.0, 0.0);
	Sphere(0.1, 30, 30);
	glPopMatrix();

	//head pyramids RIGHT
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(0.26, -0.7, -0.5);
	drawPyramid(0.95);
	glPopMatrix();

	//head pyramids left
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(-1.24, -0.7, -0.5);
	drawPyramid(1.0);
	glPopMatrix();

	//middlle head spike
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 0.2);
	glRotatef(20, 1.0, 0.2, 0.0);
	drawPyramid2(0.3, 2.5);
	glPopMatrix();

	//left head spike
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 0.2);
	glRotatef(20, 1.0, 0.2, 0.0);
	drawPyramid2(0.28, 2.0);
	glPopMatrix();
	glPopMatrix();

	//right head spike
	glPushMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 0.2);
	glRotatef(20, 1.0, 0.2, 0.0);
	drawPyramid2(0.28, 2.0);
	glPopMatrix();
	glPopMatrix();

	////neck
	//glPushMatrix();
	//glTranslatef(0.1, -1.0, 0.0);
	//glColor3f(0.0, 0.0, 0.0);
	//drawCuboid3(0.8, 0.6, 0.6, 1);
	//glPopMatrix();

	//mouth
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-0.45, -0.5, 0.6);
	glScalef(3.0, 0.7, 0.5);
	Cube(0.3);
	glPopMatrix();

}

void neck() {
	//neck
	glPushMatrix();
	glTranslatef(0.1, -1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	drawCuboid3(0.8, 0.6, 0.6, 1);
	glPopMatrix();
}

void mainBody() {

	//main body
	glPushMatrix();
	glColor3f(0.5, 0.2, 0.2);
	glTranslatef(-1.5, -0.5, 0.0); //offset
	drawCuboid3(2.0, 5.0, 2.29, 1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0.2, 0.2);
	glTranslatef(-2.5, -4.5, -1.145);//offset
	drawTrianglePrism(5.0, 1.5, 2.29); // length, height, width
	glPopMatrix();

	//main body design
	///top triangle front plate // drawCuboid3(2.0, 5.0, 2.29, 1);
	if (bodyChange % 2 == 1) {
		textureArray[1] = loadtexture("body.bmp");


		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-1.45, -0.5, -0.40); //offset
		glRotatef(180, 0, 0, 1);
		glTranslatef(-2.92, 3.0, 0); //offset
		drawCuboid4(2.0, 4.9, 2.29, 1);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-2.45, -4.45, -1.60);//offset
		drawTrianglePrism2(4.9, 1.5, 2.29); // length, height, width
		glPopMatrix();

		glColor3f(1.0, 1.0, 1.0); //white
		glPushMatrix();
		glTranslatef(-2.0, -1.755, 0.0);
		glPushMatrix();
		glTranslatef(0.0, 0.0, 0.3);
		drawTrianglePrism2(4.0, 0.75, 1.0);
		glPopMatrix();
		glPopMatrix();

		////left triangle front plate
		glPushMatrix();
		glTranslatef(1.01, -1.75, 0.0);
		glPushMatrix();
		glTranslatef(-1.0, -1.25, 0.3);
		glRotatef(90, 0.0, 0.0, 1.0);
		drawTrianglePrism2(2.0, 2.5, 1.0);////length, height, width
		glPopMatrix();
		glPopMatrix();

		////right triangle front plate
		glPushMatrix();
		glRotatef(180, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.0, -1.6);
		glPushMatrix();
		glTranslatef(1.01, -1.75, 0.0);
		glPushMatrix();
		glTranslatef(-1.0, -1.25, 0.3);
		glRotatef(-270, 0.0, 0.0, 1.0);
		drawTrianglePrism2(2.0, 2.5, 1.0);////length, height, width
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		////left bottom corner
		glPushMatrix();
		glScalef(0.9, 0.9, 0.9);
		glTranslatef(1.0, -3.26, 0.3);
		glRotatef(148.5, 0.0, 0.0, 1.0);
		glPushMatrix();
		drawTrianglePrism2(2.8, 2.0, 1.0);////length, height, width
		glPopMatrix();
		glPopMatrix();

		////right bottom corner
		glPushMatrix();
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.0, -1.5);
		glScalef(0.9, 0.9, 0.9);
		glTranslatef(1.0, -3.26, 0.3);
		glRotatef(148.5, 0.0, 0.0, 1.0);
		glPushMatrix();
		drawTrianglePrism2(2.8, 2.0, 1.0);////length, height, width
		glPopMatrix();
		glPopMatrix();

		glDeleteTextures(1, &textureArray[0]);
		glDisable(GL_TEXTURE_2D);


	}
	else { //editted here

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-1.45, -0.5, -0.40); //offset
		drawCuboid3(2.0, 4.9, 2.29, 1);
		glPopMatrix();

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);//white
		glTranslatef(-2.45, -4.45, -1.60);//offset
		drawTrianglePrism(4.9, 1.5, 2.29); // length, height, width
		glPopMatrix();

		////top trangle front plate
		glColor3f(1.0, 1.0, 1.0); //white
		glPushMatrix();
		glTranslatef(-2.0, -1.755, 0.0);
		glPushMatrix();
		glTranslatef(0.0, 0.0, 0.3);
		drawTrianglePrism(4.0, 0.75, 1.0);
		glPopMatrix();
		glPopMatrix();

		////left triangle front plate
		glPushMatrix();
		glTranslatef(1.01, -1.75, 0.0);
		glPushMatrix();
		glTranslatef(-1.0, -1.25, 0.3);
		glRotatef(90, 0.0, 0.0, 1.0);
		drawTrianglePrism(2.0, 2.5, 1.0);////length, height, width
		glPopMatrix();
		glPopMatrix();

		////right triangle front plate
		glPushMatrix();
		glRotatef(180, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.0, -1.6);
		glPushMatrix();
		glTranslatef(1.01, -1.75, 0.0);
		glPushMatrix();
		glTranslatef(-1.0, -1.25, 0.3);
		glRotatef(90, 0.0, 0.0, 1.0);
		drawTrianglePrism(2.0, 2.5, 1.0);////length, height, width
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		////left bottom corner
		glPushMatrix();
		glScalef(0.9, 0.9, 0.9);
		glTranslatef(1.0, -3.26, 0.3);
		glRotatef(148.5, 0.0, 0.0, 1.0);
		glPushMatrix();
		drawTrianglePrism(2.8, 2.0, 1.0);////length, height, width
		glPopMatrix();
		glPopMatrix();

		////right bottom corner
		glPushMatrix();
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.0, -1.5);
		glScalef(0.9, 0.9, 0.9);
		glTranslatef(1.0, -3.26, 0.3);
		glRotatef(148.5, 0.0, 0.0, 1.0);
		glPushMatrix();
		drawTrianglePrism(2.8, 2.0, 1.0);////length, height, width
		glPopMatrix();
		glPopMatrix();
	}
	//arc center
	////arc 
	glPushMatrix();
	glTranslatef(0.0, -2.0, 0.3);  //move to pos
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0); //blue
	glTranslatef(0.0, 0.0, -1.140); //offset
	Cylinder(0.4, 0.4, 2.29);
	glPopMatrix();

	////4 pyramid around arc 
	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.19);
	glColor3f(0.5, 0.0, 0.0);
	drawPyramid2(0.1, 0.42);

	glPushMatrix();
	glRotatef(90, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -0.05, 0.0);
	drawPyramid2(0.1, 0.40);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.05, 0.0, 0.0);
	glRotatef(180, 0.0, 0.0, 1.0);
	drawPyramid2(0.1, 0.40);
	glPopMatrix();

	glPushMatrix();
	glRotatef(270, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.05, 0.0);
	drawPyramid2(0.1, 0.40);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-3.5, -1.75, -0.5);
	glScalef(2.0, 1.0, 2.0);
	Cube(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(2.5, -1.75, -0.5);
	glScalef(2.0, 1.0, 2.0);
	Cube(0.5);
	glPopMatrix();
}

void lowerBody() {

	//lower body
	glPushMatrix();
	glColor3f(0.6, 0.3, 0.3);
	glTranslatef(0.0, -2.0, 0.0);
	glScalef(0.8, 0.8, 0.8);
	////lower body quad
	glPushMatrix();
	glTranslatef(-1.5, -0.5, 0.0); //offset
	drawCuboid3(2.0, 5.0, 2.29, 1);
	glPopMatrix();
	////lower body prism
	glPushMatrix();
	glTranslatef(-2.5, -4.5, -1.145);//offset
	drawTrianglePrism(5.0, 1.5, 2.29); // length, height, width
	glPopMatrix();
	glPopMatrix();



	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.0, -3.0, 0.0);
	glScalef(0.7, 0.7, 0.7);
	////lower body quad
	glPushMatrix();
	glTranslatef(-1.5, -0.5, 0.0); //offset
	drawCuboid3(2.0, 5.0, 2.29, 1);
	glPopMatrix();
	////lower body prism
	glPushMatrix();
	glTranslatef(-2.5, -4.5, -1.145);//offset
	drawTrianglePrism(5.0, 1.5, 2.29); // length, height, width
	glPopMatrix();
	glPopMatrix();

}

void frontShield(float RgbX, float RgbY, float RgbZ) {
	glPushMatrix();
	glColor3f(RgbX, RgbY, RgbZ);
	glTranslatef(0.0, 0.0, 1.5);
	glRotatef(90, 0.0, 0.0, 1.0);
	drawTrianglePrism(1.0, 1.7, 0.5); // length, height, width
	glPopMatrix();

	glPushMatrix();
	glColor3f(RgbX, RgbY, RgbZ);
	glTranslatef(0.0, 1.0, 1.5);
	glRotatef(-90, 0.0, 0.0, 1.0);
	drawTrianglePrism(1.0, 1.7, 0.5); // length, height, width
	glPopMatrix();

	glPushMatrix();
	glColor3f(RgbX, RgbY, RgbZ);
	glTranslatef(-0.35, 0.12, 1.5);
	glRotatef(45, 0.0, 0.0, 1.0);
	drawTrianglePrism(1.0, 1.5, 0.5); // length, height, width
	glPopMatrix();

	glPushMatrix();
	glColor3f(RgbX, RgbY, RgbZ);
	glTranslatef(-0.35, 0.85, 1.5);
	glRotatef(-45, 0.0, 0.0, 1.0);
	drawTrianglePrism(1.0, 1.5, 0.5); // length, height, width
	glPopMatrix();

	glPushMatrix();
	glColor3f(RgbX, RgbY, RgbZ);
	glTranslatef(0.35, 0.15, 1.5);
	glRotatef(135, 0.0, 0.0, 1.0);
	drawTrianglePrism(1.0, 1.5, 0.5); // length, height, width
	glPopMatrix();

	glPushMatrix();
	glColor3f(RgbX, RgbY, RgbZ);
	glTranslatef(0.3, 0.9, 1.5);
	glRotatef(-135, 0.0, 0.0, 1.0);
	drawTrianglePrism(1.0, 1.5, 0.5); // length, height, width
	glPopMatrix();

	glPushMatrix();
	glColor3f(RgbX, RgbY, RgbZ);
	glTranslatef(0.48, 0.55, 1.5);
	glRotatef(-180, 0.0, 0.0, 1.0);
	drawTrianglePrism(1.0, 1.7, 0.5); // length, height, width
	glPopMatrix();

	glPushMatrix();
	glColor3f(RgbX, RgbY, RgbZ);
	glTranslatef(-0.5, 0.4, 1.5);
	drawTrianglePrism(1.0, 1.7, 0.5); // length, height, width
	glPopMatrix();
}

void all() {

	if (u >= 90 && armLeftUp % 2 == 1) {
		u == 90;
	}
	else if (armLeftUp % 2 == 0 && u >= 0) {
		u -= 0.2;
	}
	else {
		u += 0.2;
	}

	if (handup == true && (armLeftUp % 2 == 1 || armLeftUp % 2 == 0)) {

		glPushMatrix();
		glTranslatef(-2.62, 2.87, -0.1);
		glRotatef(-u, 1.0, 0.0, 0.0);
		glPushMatrix();
		glTranslatef(0.0, 0.0, 0.33);
		LeftHand();
		glPopMatrix();
		glPushMatrix();
		glScalef(0.6, 0.9, 0.6);

		if (openGun == true && gunUndo % 2 == 1) {
			glPushMatrix();
			glTranslatef(2.65, -2.7, 0.1);
			gun();
			glPopMatrix();
		}
		else {
			glPushMatrix();
			glTranslatef(4.4, -2.95, 0.1);
			ungun();
			glPopMatrix();
		}
		glPopMatrix();


		glPopMatrix();
	}
	else {
		if (walk == true && walkUndo % 2 == 1) {
			if (w >= 50) {
				w -= 0.2;
				walkBack = 1;
			}
			else if (walkBack == 1 && w > -50) {
				//Sleep(1);
				w -= 0.2;
			}
			else {
				//Sleep(1);
				w += 0.2;
				walkBack = 0;
			}
		}

		glPushMatrix();
		glTranslatef(-2.62, 3.0, 0.0);//2.8
		glRotatef(-w, 1.0, 0.0, 0.0);
		glPushMatrix();
		glTranslatef(0.0, 0.0, 0.24);
		LeftHand();
		glPopMatrix();
		if (walk == true) {
			glPushMatrix();
			glTranslatef(2.6, -2.68, 0.0);
			glPushMatrix();
			glScalef(0.6, 0.9, 0.6);
			ungun();
			glPopMatrix();
			glPopMatrix();
		}
		glPopMatrix();

		if (walk == false) {
			if (openGun == true && gunUndo % 2 == 1) {
				glPushMatrix();
				glTranslatef(-0.9, 0.45, 0.0);
				glScalef(0.6, 0.9, 0.6);
				gun();
				glPopMatrix();
			}
			else {

				glPushMatrix();
				glTranslatef(0.0, 0.3, 0.0);
				glScalef(0.6, 0.9, 0.6);
				ungun();
				glPopMatrix();
			}
		}

	}

	if (gunUndo % 2 == 1 && armLeftUp % 2 == 1 && (gunShot == true || gunChargeShot == true)) {

		glPushMatrix();
		gunShotSpeed += 0.05;
		glTranslatef(-1.05, 2.95, 2.95);
		glScalef(0.6, 0.6, 0.6);
		bullet();
		glPopMatrix();

	}

	RobotLeg();
	RightShield();
	LeftShield();
}

void display()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearColor(0.5, 0.5, 0.5, 0.0);

	//backdrop
	glPushMatrix();
	if (isOrtho) {

		textureArray[0] = loadtexture("1.bmp");

		glPushMatrix();

		glTranslatef(-8, -8, -10.0);

		glColor3f(1.0, 1.0, 1.0);
		CubeBackGround(20);
		glPopMatrix();

		glDeleteTextures(1, &textureArray[0]);
		glDisable(GL_TEXTURE_2D);


	}
	else {
		glPushMatrix();
		textureArray[0] = loadtexture("1.bmp");
		//glTranslatef(-8, -8, -15);
		//glTranslatef(-8, -8, 30);
		glTranslatef(-65.0, -65.0, 30);
		glColor3f(1.0, 1.0, 1.0);
		CubeBackGround(130); //edit here

		glPopMatrix();
		glDeleteTextures(1, &textureArray[0]);
		glDisable(GL_TEXTURE_2D);

	}
	glPopMatrix();

	projection();
	glMatrixMode(GL_MODELVIEW);

	

	//glPushMatrix();
	//	textureArray[0] = loadtexture("1.bmp");
	//	//glTranslatef(-8, -8, -15);
	//	//glTranslatef(-8, -8, 30);
	//	glTranslatef(-15.0, -15.0, 30);
	//	glColor3f(1.0, 1.0, 1.0);
	//	CubeBackGround(60); //edit here
	//	
	//glPopMatrix();
	//glDeleteTextures(1, &textureArray[0]);
	//glDisable(GL_TEXTURE_2D);
	

	lighting();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, difM);	//diffuse material

	glPushMatrix();
	glTranslatef(0.0, 0.0, 5.0);
	glRotatef(Rx, 1.0, 0.0, 0.0);
	glRotatef(Ry, 0.0, 1.0, 0.0);
	//glRotatef(Rz, 0.0, 1.0, 0.0);
	glPushMatrix();

	//???????????????????????????????????????????????????????????????????????????????????????????????????
	glPushMatrix();
	
	//glTranslatef(0.0, 0.0, -5.0);

	//HEAD ANIMATE MANUAL [F1,F2-rotate x axis, F3, F4-rotate y axis]
	glPushMatrix();
	glPushMatrix();
	glTranslatef(0.0, 4.0, -0.05);
	
	glScalef(0.65, 0.8, 0.4);
	glRotatef(headx, 1.0, 0.0, 0.0);
	glRotatef(heady, 0.0, 1.0, 0.0);
	glRotatef(headz, 0.0, 0.0, 1.0);
	head();
	neck();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 4.1, 0.0);
	glScalef(0.65, 0.8, 0.4);
	mainBody();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 2.9, 0.0);
	glScalef(0.65, 0.5, 0.4);
	lowerBody();
	glPopMatrix();

	backGround();

	//======================
	//Left Side Shoulder
	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(-2.4, 2.9, -0.02);
	glScalef(1.2, 1.0, 1.0);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);//white
	glTranslatef(-3.12, 2.7, -0.25);
	glScalef(1.4, 1.4, 1.4);
	Cube(0.3);
	glPopMatrix();
	//======================


	//======================
	//Right Side Shoulder
	glPushMatrix();
	glColor3f(1.0, 0.6, 0.2);//orange
	glTranslatef(2.4, 2.9, -0.02);
	glScalef(1.2, 1.0, 1.0);
	Sphere(0.35, 30, 30);//Sphere
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);//white
	glTranslatef(2.7, 2.7, -0.25);
	glScalef(1.4, 1.4, 1.4);
	Cube(0.3);
	glPopMatrix();
	//======================

	if (sword == true && attack == false && rHandUp == false && swordUndo % 2 == 1) {
		glPushMatrix();
		glTranslatef(0.80, 0.9, 0.0);
		Sword();
		glPopMatrix();
	}

	if (defend == true && defendUndo % 2 == 1) {
		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		glScalef(1.5, 1.5, 1.5);
		frontShield(0.6, 0.8, 1.0);//set the shield color
		glPopMatrix();

		glPushMatrix();
		glRotatef(-20, 0.0, 0.0, 1.0);
		glTranslatef(-0.2, -0.2, -0.8);
		glScalef(1.75, 1.75, 1.75);
		frontShield(0.0, 0.0, 1.0);//set the shield color
		glPopMatrix();
	}


	if (attack == true && attackUndo % 2 == 1) {
		all();

		if (handmove == true) {
			if (h >= 70) {
				h -= 0.3;
				handBack = 1;
			}
			else if (handBack == 1 && h > 0) {
				//Sleep(1.0);
				h -= 0.3;

			}
			else {
				//Sleep(1.0);
				h += 0.3;
				handBack = 0;
			}
		}

		glPushMatrix();
		glTranslatef(2.6, 3.2, 0.0);//3.0
		glRotatef(h, 1.0, 1.0, 1.0);
		glPushMatrix();
		glPushMatrix();
		glRotatef(-50, 0.0, 1.0, 0.0);
		glPushMatrix();
		glTranslatef(-0.4, 0.0, 0.4);
		glRotatef(-90, 1.0, 0.0, 0.0);
		RightHand();
		glPopMatrix();
		glPopMatrix();

		if (sword == true && attack == true && swordUndo % 2 == 1) {
			glPushMatrix();
			glRotatef(45, 0.0, 1.0, 0.0);
			glPushMatrix();
			glRotatef(-90, 1.0, 0.0, 0.0);
			glPushMatrix();
			glTranslatef(-2.5, 2.1, -0.23);
			glRotatef(-90, 0.0, 0.0, 1.0);

			Sword();


			glPopMatrix();
			glPopMatrix();
			glPopMatrix();
		}

		glPopMatrix();
		glPopMatrix();
	}
	else {
		if (walk == true && walkUndo % 2 == 1) {
			if (w >= 50) {
				w -= 0.2;
				walkBack = 1;
			}
			else if (walkBack == 1 && w > -50) {
				//Sleep(1);
				w -= 0.2;
			}
			else {
				//Sleep(1);
				w += 0.2;
				walkBack = 0;
			}
		}

		if (i >= 90 && rHandUndo % 2 == 1) {
			i == 90;
		}
		else if (rHandUndo % 2 == 0 && i >= 0) {
			i -= 0.2;
		}
		else {
			i += 0.2;
		}

		if (rHandUp == true && (rHandUndo % 2 == 1 || rHandUndo % 2 == 0)) {


			glPushMatrix();
			glTranslatef(2.2, 3.1, 0.2);
			glRotatef(-i, 1.0, 0.0, 0.0);
			RightHand();

			if (rHandUp == true && sword == true && swordUndo % 2 == 1) {
				glPushMatrix();
				glTranslatef(-1.35, -2.0, -0.2);
				Sword();
				glPopMatrix();
			}
			glPopMatrix();
		}
		else {

			glPushMatrix();
			glTranslatef(2.18, 3.0, 0.18);
			glRotatef(w, 1.0, 0.0, 0.0);
			RightHand();
			glPopMatrix();

		}
		all();
	}
	glPopMatrix();
	glPopMatrix();


	//???????????????????????????????????????????????????????????????????????????????????????????????????
	glPopMatrix();

}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1000, 900,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------