/*
 *  CSCI 441, Computer Graphics, Fall 2018
 *
 *  Project: SMK
 *  File: main.cpp
 *
 *	Authors: Alexander DeGroat, Mila Evans, Joshua Nachtigal, Savannah Paul
 *
 *  Description:
 *      Watch some heroes race around a track
 *
 */

// include the OpenGL library header
#ifdef __APPLE__					// if compiling on Mac OS
	#include <OpenGL/gl.h>
#else										// if compiling on Linux or Windows OS
	#include <GL/gl.h>
#endif

#include <GLFW/glfw3.h>	// include GLFW framework header

#include <CSCI441/objects.hpp> // for our 3D objects

// include GLM libraries and matrix functions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>

#include <math.h>				// for cos(), sin() functionality
#include <stdio.h>			// for printf functionality
#include <stdlib.h>			// for exit functionality
#include <time.h>			  // for time() functionality
#include <fstream>			// for file I/O
#include <vector>				// for vectors
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <math.h>

#include "HeroBase.h"
#include "Alex.h"
#include "Mila.h"
#include "Josh.h"
#include "Sav.h"

using namespace std;

//*************************************************************************************
//
// Global Parameters

// global variables to keep track of window width and height.
// set to initial values for convenience, but we need variables
// for later on in case the window gets resized.
int windowWidth = 640, windowHeight = 480;

const int ARCBALL_CAM = 0;
const int FREE_CAM = 1;


int leftMouseButton;    	 						// status of the mouse button
glm::vec2 mousePos;			              		  	// last known X and Y of the mouse

glm::vec3 camPos;            						// camera position in cartesian coordinates
float cameraTheta, cameraPhi;               		// camera DIRECTION in spherical coordinates
float camDistance = 5.0f;							// camera distance from the hero (lookAt point)
float cameraSpeed = 0.5f;							// speed the camera moves as a factor of a unit vector
bool cameraIn = false;								// values that determine whether or not the camera is moving and in what direction
bool cameraOut = false;
bool cameraLeft = false;
bool cameraRight = false;

glm::vec3 heroPos;									// position of the hero
float heroAngle;									// direction the hero is facing in radians (0 is along the positive Z axis)
glm::vec3 heroDir;									// vector for the direction the hero is facing
float walkSpeed = 0.1f;							// speed the hero is walking as a factor of a unit vector
float turnSpeed = 0.05f;							// speed the hero is turning

float racerPos = 0;									// t value for racing heroes

bool walking = false;								// values that determine whether or not the hero is turning and/or walking
bool turning = false;
float direction = 1.0f;								// direction the hero is walking (forwards = 1.0, backwards = -1.0)
float turnDirection = 1.0f;							// direction the hero is turning (clockwise = -1.0, counterclockwise = 1.0)

float animationFrame = 0.0f;						// value for the current animation cycle (used for legs moving and body bobbing)
int animateIndex = 5;								// index the value of the animation cycle is in the following vector
vector<float> animateVals;							// vector to store a few different animation cycle values

vector<vector<glm::vec3> > controlPoints; 			// control points for Bezier surface
vector<glm::vec3> curveControlPoints;				// control points for Bezier curve
vector<glm::vec3> cactusPoints;
vector<glm::vec3> lampPoints;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
int surfaceRes = 3;
=======
=======
>>>>>>> d49867c (Small edit)
map<float, float> lookupTable;	
<<<<<<< HEAD
int tableResolution = 1000;							// for smooth vehicle movement
=======
int tableResolution = 101;								// for smooth vehicle movement
>>>>>>> 0d05240 (Got parameterized stuff working)
=======
map<float, float> lookupTable;
map<pair<float, float>, float> totalPoints;
pair<float, float> mew(1.2, 2.4);
>>>>>>> 32113c0 (height and stuff)
=======
map<float, float> lookupTable;
>>>>>>> aaf8ff6 (Added wanderer along the surface)

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
bool cageOn = true;									// Determines if the cage/curve should be visible or not
bool curveOn = true;
<<<<<<< HEAD
>>>>>>> ecc2a23 (Added getParameterizedt function which takes in a float t and returns the respective parameterized value. Probably needs testing after everything is put together)
=======
=======
>>>>>>> b5cf244 (edited merge related stuff)
int surfaceRes = 3;
>>>>>>> d49867c (Small edit)
=======
int surfaceRes = 5;
=======
int tableResolution = 100;								// for smooth vehicle movement

<<<<<<< HEAD
int surfaceRes = 50;
>>>>>>> 528532e (added colors)
=======
int surfaceRes = 100;
>>>>>>> aaf8ff6 (Added wanderer along the surface)
float height = 0;
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> fa8d1a8 (Refactored for two separate loadControlPoint functions)
=======
int numObjects;
>>>>>>> b513a88 (still working)
=======
int scaleConstant = 10;
int heightScaleConstant = 4;
>>>>>>> 0c06735 (shrug)

glm::mat4 transMtx; 								// global variables used for transformations
glm::mat4 rotateMtx;
glm::mat4 scaleMtx;

map<pair<float, float>, float> surfacePoints;

GLuint cloudTexHandle;

GLuint environmentDL;
GLuint terrainDL;
GLuint trackDL;

// For free cam
glm::vec3 freeCamPos, freeCamDir;
float yaw = 0.0f;
float pitch = 0.0f;

int currCam = ARCBALL_CAM;
bool viewOverlay = false;
int overlaySize = 150;

void drawCactus();

HeroBase* currHero;
///////////////////////////////////---------------------------------------------------------------------------------------------------------------
Alex alex(glm::vec3(5.0f, 20.0f, 10.0f));
Mila mila(glm::vec3(5.0f, 20.3f, 5.0f));
Josh josh(glm::vec3(-5.0f, 20.0f, -5.0f));
Sav sav(glm::vec3(5.0f, 20.0f, 15.0f));

//*************************************************************************************
//
// Helper Functions

// getRand() ///////////////////////////////////////////////////////////////////
//
//  Simple helper function to return a random number between 0.0f and 1.0f.
//
////////////////////////////////////////////////////////////////////////////////
float getRand() { return rand() / (float)RAND_MAX; }

float getDist(float x1, float y1, float x2, float y2){
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// recomputeOrientation() //////////////////////////////////////////////////////
//
// This function updates the camera's direction in cartesian coordinates based
//  on its position in spherical coordinates. Should be called every time
//  cameraTheta or cameraPhi is updated.
//
////////////////////////////////////////////////////////////////////////////////
void recomputeOrientation(HeroBase* hero = NULL) {
	if(hero == NULL) hero = currHero;
	float camDistance = hero->camDistance;
	float cameraTheta = hero->cameraTheta;
	float cameraPhi = hero->cameraPhi;

	hero->direction = glm::vec3(sin(hero->yaw), 0, cos(hero->yaw));
	hero->camPos = glm::vec3(camDistance * sin(cameraTheta) * sin(cameraPhi), -camDistance * cos(cameraPhi), -camDistance * cos(cameraTheta) * sin(cameraPhi)) + hero->pos;
}

// checkBounds() ///////////////////////////////////////////////////////////////
//
// This function determines if, when the hero walks, their position is
//  out of bounds of the map, and if it is, places the hero back on the map
//
////////////////////////////////////////////////////////////////////////////////
void checkBounds() {
	if(heroPos.x < -50){
		heroPos.x = -50;
	}
	else if(heroPos.x > 50){
		heroPos.x = 50;
	}
	else if(heroPos.z < -50){
		heroPos.z = -50;
	}
	else if(heroPos.z > 50){
		heroPos.z = 50;
	}
}

void setupTextures() {

	cloudTexHandle = SOIL_load_OGL_texture("textures/lessclouds.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cloudTexHandle);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S,
		GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		GL_REPEAT);

}

// loadControlPoints() /////////////////////////////////////////////////////////
//
//  Load our control points from file and store them in
//	the global variable controlPoints
//
////////////////////////////////////////////////////////////////////////////////
bool loadPoints( char* filename ) {
	ifstream inputFile(filename);

	string numPointsString;
	int numPoints;
	getline(inputFile, numPointsString);
	sscanf(numPointsString.c_str(), "%d", &numPoints);

	for(int j = 0; j < numPoints / 16; j++){
		vector<glm::vec3> newSurface;
		for (int i = 0; i < 16; i++) {
			string x, y, z;
			getline(inputFile, x, ',');
			getline(inputFile, y, ',');
			getline(inputFile, z);
			glm::vec3 controlPoint = glm::vec3(atof(x.c_str()) * scaleConstant, atof(y.c_str()) * heightScaleConstant, atof(z.c_str()) * scaleConstant);
			newSurface.push_back(controlPoint);
		}
		controlPoints.push_back(newSurface);
	}

	string numPointsString2;
	int numPoints2;
	getline(inputFile, numPointsString2);
	sscanf(numPointsString2.c_str(), "%d", &numPoints2);

	for (int j = 0; j < numPoints2; j++) {
		string x, y, z;
		getline(inputFile, x, ',');
		getline(inputFile, y, ',');
		getline(inputFile, z);
		glm::vec3 controlPoint = glm::vec3(atof(x.c_str()) * scaleConstant, atof(y.c_str()) * heightScaleConstant, atof(z.c_str()) * scaleConstant);
		curveControlPoints.push_back(controlPoint);
	}

	string numObjectsStr;
	string objType;
	string objx, objy;
	getline(inputFile, numObjectsStr);
	sscanf(numObjectsStr.c_str(), "%d", &numObjects);
	cout << numObjects << endl;
	for(int i = 0; i < numObjects; i++) {
		getline(inputFile, objType, ',');
		getline(inputFile, objx, ',');
		getline(inputFile, objy);
		if (objType == "cactus") {
			glm::vec3 pnt(atof(objx.c_str()), atof(objy.c_str()), 0);
			cactusPoints.push_back(pnt);
		}
		if (objType == "lamp") {
			glm::vec3 pnt2(atof(objx.c_str()), atof(objy.c_str()), 0);
			lampPoints.push_back(pnt2);
		}
	}


	return true;
}

float round(float var){
	float value = (int)(var * 100 + .5);
	return (float)value / 100;
}

float calcHeight(float x, float y) {

	float shortestDist = 100000.0;
	float newY = 0.0;
	map<pair<float, float>, float>::iterator it = surfacePoints.begin();

	while (it != surfacePoints.end()) {
		if (getDist(it->first.first, it->first.second, x , y) < shortestDist) {
			shortestDist = getDist(it->first.first, it->first.second, x, y);
			newY = it->second;
		}
		it++;
	}
	return newY;


	//cout << "x: " << x << "y: " << y << endl;
	
	//while (x > 100) {
	//	x = x - 100;
	//}
	//while (y > 100) {
	//	y =y - 100;
	//}
	//while (x < 0) {
	//	x = x + 100;
//	}
//	while (y < 0) {
//		y = y + 100;
//	}
//	pair<float, float> temp(round(x), round(y));
	//stringstream iss;
	//iss << x;
	//string x_t = iss.str();
	//stringstream iss2;
	//iss2 << y;
	//string y_t = iss2.str();
	//string both = x_t + " " + y_t;


	//if (totalPoints.find(temp) == totalPoints.end()) {
		//cout << "key not found" << endl;
		//map<pair<float, float>, float>::iterator lower = totalPoints.lower_bound(temp);
	//	map<pair<float, float>, float>::iterator upper = totalPoints.upper_bound(temp);
	//	return upper->second;
		//return (upper->second + lower->second) / 2;
		//return ( (upper->second * (temp.first / upper->first.first) * (temp.second / upper->first.second) / upper->first.first) + (lower->second * (upper->first.first - (temp.first / upper->first.first)) * (upper->first.second - upper->first.second / temp.second)) / upper->first.second);
	//}

	//cout << both << endl;
	//cout << totalPoints[both] << endl;
	//cout << totalPoints[both] << endl;
	//return totalPoints[temp];
}


// evaluateBezierCurve() ////////////////////////////////////////////////////////
//
// Computes a location along a Bezier Curve.
//
////////////////////////////////////////////////////////////////////////////////

glm::vec3 evaluateBezierCurve( glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t ) {
	glm::vec3 point = (float(pow((1.0 - t), 3)) * p0) + (3.0f * float(pow((1.0 - t), 2)) * t * p1) + (3.0f * (1.0f - t) * float(pow(t, 2)) * p2) + (float(pow(t, 3)) * p3);
<<<<<<< HEAD
<<<<<<< HEAD
	
	
	/*stringstream iss;
	iss << round(point.x);
	string x = iss.str();
	stringstream iss2;
	iss2 << round(abs(point.z));
	string y = iss2.str();
	string both = x + " " + y;
	glm::vec3 temp(point.x, point.z, 0);
	*/
	
	//cout << both << endl;
	//float z = point.y;
	//cout <<"x: " << x << " y: " << y << " z: " << z << endl;
	//totalPoints[both] = z;

	int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	//if(find(std::begin(a), std::end(a), x) != std::end(a))

=======

	pair<float, float> coords(point.x, point.z);
	surfacePoints[coords] = point.y;
>>>>>>> 3a3c990 (something)

	return point;
}
glm::vec3 evaluateBezierCurve2(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t) {
	glm::vec3 point = (float(pow((1.0 - t), 3)) * p0) + (3.0f * float(pow((1.0 - t), 2)) * t * p1) + (3.0f * (1.0f - t) * float(pow(t, 2)) * p2) + (float(pow(t, 3)) * p3);

<<<<<<< HEAD
<<<<<<< HEAD

	/*stringstream iss;
	iss << round(point.x);
	string x = iss.str();
	stringstream iss2;
	iss2 << round(abs(point.z));
	string y = iss2.str();
	string both = x + " " + y;
	glm::vec3 temp(point.x, point.z, 0);
	*/
	//pair<float, float> temp(round(point.x), abs(round(point.z)));
	//cout << temp.first << " " << temp.second << " " << point.y << endl;
	//totalPoints[temp] = point.y;

	//cout << both << endl;
	//float z = point.y;
	//cout <<"x: " << x << " y: " << y << " z: " << z << endl;
	//totalPoints[both] = z;

	int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	//if(find(std::begin(a), std::end(a), x) != std::end(a))

=======

	pair<float, float> coords(point.x - 50, point.z + 50);
	surfacePoints.emplace(coords, point.y);
>>>>>>> aaf8ff6 (Added wanderer along the surface)
=======
	pair<float, float> coords(point.x, point.z);
	surfacePoints[coords] = point.y;
>>>>>>> 3a3c990 (something)

=======
>>>>>>> 427c140 (merging)
	return point;
}

// renderBezierCurve() //////////////////////////////////////////////////////////
//
// Responsible for drawing a Bezier Curve as defined by four control points.
//  Breaks the curve into n segments as specified by the resolution.
//
////////////////////////////////////////////////////////////////////////////////
void renderBezierCurve( glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 p5, glm::vec3 p6, glm::vec3 p7, int resolution ) {
	float t = 0.0;
	while(t <= 1.0 - (1.0 / resolution) + 0.0001) {
		glm::vec3 nextPoint1 = evaluateBezierCurve(p0, p1, p2, p3, t);
		glm::vec3 nextPoint2 = evaluateBezierCurve(p0, p1, p2, p3, t + (1.0 / resolution));
		glm::vec3 nextPoint3 = evaluateBezierCurve(p4, p5, p6, p7, t);
		glm::vec3 nextPoint4 = evaluateBezierCurve(p4, p5, p6, p7, t + (1.0 / resolution));

		glm::vec3 crossPoint1 = nextPoint2 - nextPoint1;
		glm::vec3 crossPoint2 = nextPoint3 - nextPoint1;

		glm::vec3 normalVec = glm::cross(crossPoint2, crossPoint1);
		glNormal3f(normalVec.x, normalVec.y, normalVec.z);

		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(nextPoint1.x, nextPoint1.y, nextPoint1.z);
			glVertex3f(nextPoint2.x, nextPoint2.y, nextPoint2.z);
			glVertex3f(nextPoint3.x, nextPoint3.y, nextPoint3.z);
			glVertex3f(nextPoint4.x, nextPoint4.y, nextPoint4.z);
		glEnd();

		t += (1.0/resolution);
	}
}
<<<<<<< HEAD
=======
void renderBezierCurve1(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int resolution) {
	glDisable(GL_LIGHTING);
	glBegin(GL_LINE_STRIP);
	for (float i = 0; i < resolution; i += 1) {
		glm::vec3 point = evaluateBezierCurve2(p0, p1, p2, p3, i / resolution);
		glVertex3f(point.x, point.y, point.z);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}
>>>>>>> 427c140 (merging)

<<<<<<< HEAD
<<<<<<< HEAD
=======
void generateLookupTable() {
	lookupTable.clear();
	float distance = 0;
	glm::vec3 lastPoint = curveControlPoints[0];
	for (unsigned int i = 0; i + 1 < curveControlPoints.size(); i += 3) {
		for (float j = 0; j < tableResolution; j += 1) {
			glm::vec3 point = evaluateBezierCurve2(curveControlPoints[i], curveControlPoints[i + 1], curveControlPoints[i + 2], curveControlPoints[i + 3], j / tableResolution);
			distance += sqrt(pow((point.x - lastPoint.x), 2) + pow(point.y - lastPoint.y, 2) + pow(point.z - lastPoint.z, 2));
			float t = i / 3 + j / tableResolution;
			cout << distance << " " << t << endl;
			lookupTable.insert(pair<float, float>(distance, t));
		}
	}
}

float getParameterizedt(float pos) {
	float tAvg = pos / ((floor(curveControlPoints.size()) / 3));
	map<float, float>::iterator low;
	map<float, float>::iterator high;

	tAvg = tAvg * lookupTable.rbegin()->first;
	float t = pos - floor(pos);
	low = lookupTable.lower_bound(tAvg);
	high = lookupTable.upper_bound(tAvg);

	float value = low->second * (1 - t) + high->second * t;
	return low->second * (1 - t) + high->second * t;
}
>>>>>>> d49867c (Small edit)
// renderBezierSurface() //////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
void renderBezierSurface(vector<glm::vec3> p, int u_res) {
	float u = 0.0;
	while (u <= 1.0 - (1.0 / u_res) + 0.0001) {
		renderBezierCurve(evaluateBezierCurve2(p[0], p[1], p[2], p[3], u),
			evaluateBezierCurve2(p[4], p[5], p[6], p[7], u),
			evaluateBezierCurve2(p[8], p[9], p[10], p[11], u),
			evaluateBezierCurve2(p[12], p[13], p[14], p[15], u),
			evaluateBezierCurve2(p[0], p[1], p[2], p[3], u + (1.0 / u_res)),
			evaluateBezierCurve2(p[4], p[5], p[6], p[7], u + (1.0 / u_res)),
			evaluateBezierCurve2(p[8], p[9], p[10], p[11], u + (1.0 / u_res)),
			evaluateBezierCurve2(p[12], p[13], p[14], p[15], u + (1.0 / u_res)), u_res);
		u += (1.0 / u_res);
	}
}

<<<<<<< HEAD

=======
void generateLookupTable() {
	lookupTable.clear();
	float distance = 0;
	glm::vec3 lastPoint = controlPoints[0];
	for (unsigned int i = 0; i + 1 < controlPoints.size(); i += 3) {
		for (float j = 0; j < tableResolution; j += 1) {
			glm::vec3 point = evaluateBezierCurve(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3], j / tableResolution);
			distance += sqrt(pow((point.x - lastPoint.x), 2) + pow(point.y - lastPoint.y, 2) + pow(point.z - lastPoint.z, 2));
			float t = i + j / tableResolution;
			lookupTable[t] = distance;
		}
	}
}

float getParameterizedt(float pos) {
	float bot = lookupTable.at(floor(pos * tableResolution) / tableResolution);
	float top = lookupTable.at(ceil(pos * tableResolution) / tableResolution);
	return (bot * (1 - (racerPos - floor(racerPos))) + top * (racerPos - floor(racerPos)));
}
>>>>>>> ecc2a23 (Added getParameterizedt function which takes in a float t and returns the respective parameterized value. Probably needs testing after everything is put together)

=======
>>>>>>> d49867c (Small edit)
//*************************************************************************************
//
// Event Callbacks

//
//	void error_callback( int error, const char* description )
//
//		We will register this function as GLFW's error callback.
//	When an error within GLFW occurs, GLFW will tell us by calling
//	this function.  We can then print this info to the terminal to
//	alert the user.
//
static void error_callback( int error, const char* description ) {
	fprintf( stderr, "[ERROR]: %s\n", description );
}

static void keyboard_callback( GLFWwindow *window, int key, int scancode, int action, int mods ) {
	if( action == GLFW_PRESS || action == GLFW_REPEAT ) {
		switch( key ) {
			case GLFW_KEY_ESCAPE: // Escape quits the program
				exit(EXIT_SUCCESS);
				break;
			case GLFW_KEY_UP:	// Up arrow key moves camera inward
				cameraIn = true;
				break;
			case GLFW_KEY_DOWN:	// Down arrow key moves camera outward
				cameraOut = true;
				break;
			case GLFW_KEY_LEFT: // Left arrow key moves camera clockwise around hero
				cameraLeft = true;
				break;
			case GLFW_KEY_RIGHT: // Right arrow key moves counterclockwise around hero
				cameraRight = true;
				break;
			case GLFW_KEY_W: // W key moves hero forward
				walking = true;
				direction = 1.0f;
				break;
			case GLFW_KEY_S: // S key moves hero backward
				walking = true;
				direction = -1.0f;
				break;
			case GLFW_KEY_A: // A key turns hero counterclockwise
				turning = true;
				turnDirection = 1.0f;
				break;
			case GLFW_KEY_D: // D key turns hero clockwise
				turning = true;
				turnDirection = -1.0f;
				break;
			case GLFW_KEY_Q:
				surfaceRes++;
				break;
			case GLFW_KEY_E:
				if (surfaceRes != 1) {
					surfaceRes--;
				}
				break;
			case GLFW_KEY_1:
				currCam = ARCBALL_CAM;
				break;
			case GLFW_KEY_2:
				currCam = FREE_CAM;
				break;
			case GLFW_KEY_3:
				viewOverlay = !viewOverlay;
				break;
		}
	}
	else{
		walking = false; // Resets all boolean values because nothing is happening
		turning = false;
		direction = 1.0f;
		cameraIn = false;
		cameraOut = false;
		cameraLeft = false;
		cameraRight = false;
	}

	if(mods == GLFW_MOD_CONTROL && action == GLFW_PRESS) {
		if(key == GLFW_KEY_A) {
			currHero = &alex;
		}
		else if(key == GLFW_KEY_M) {
			currHero = &mila;
		}
		else if(key == GLFW_KEY_S) {
			currHero = &sav;
		}
		else if(key == GLFW_KEY_J) {
			currHero = &josh;
		}
	}
}

// cursor_callback() ///////////////////////////////////////////////////////////
//
//  GLFW callback for mouse movement. We update cameraPhi and/or cameraTheta
//      based on how much the user has moved the mouse in the
//      X or Y directions (in screen space) and whether they have held down
//      the left or right mouse buttons. If the user hasn't held down any
//      buttons, the function just updates the last seen mouse X and Y coords.
//
////////////////////////////////////////////////////////////////////////////////
static void cursor_callback( GLFWwindow *window, double x, double y ) {
	if( leftMouseButton == GLFW_PRESS ) {
		currHero->cameraTheta -= (0.005 * (x - mousePos.x));
		currHero->cameraPhi += (0.005 * (mousePos.y - y));

		if(currHero->cameraPhi > M_PI - 0.01){
			currHero->cameraPhi = M_PI - 0.01;
		}
		else if(currHero->cameraPhi < 0.01){
			currHero->cameraPhi = 0.01;
		}
		recomputeOrientation();     // update camera (x,y,z) based on (theta,phi)

		if(currCam == FREE_CAM) {
			float mdx = x - mousePos.x;
			float mdy = y - mousePos.y;

			yaw += mdx * 0.01f;
			pitch += mdy * 0.01f;

			float maxAngle = M_PI / 2.0f - 0.01f;
			if(pitch >= maxAngle) {
				pitch = maxAngle;
			}
			if(pitch <= -maxAngle) {
				pitch = -maxAngle;
			}
		}
	}

	mousePos.x = x;
	mousePos.y = y;
}

// mouse_button_callback() /////////////////////////////////////////////////////
//
//  GLFW callback for mouse clicks. We save the state of the mouse button
//      when this is called so that we can check the status of the mouse
//      buttons inside the motion callback (whether they are up or down).
//
////////////////////////////////////////////////////////////////////////////////
static void mouse_button_callback( GLFWwindow *window, int button, int action, int mods ) {
	if( button == GLFW_MOUSE_BUTTON_LEFT ) {
		leftMouseButton = action;
	}
}

// scroll_callback() /////////////////////////////////////////////////////
//
//  GLFW callback for mouse scrolling. Used to zoom the camera in or out
//
////////////////////////////////////////////////////////////////////////////////
static void scroll_callback( GLFWwindow *window, double xoffset, double yoffset){
	currHero->camDistance -= yoffset;
	if(currHero->camDistance < 1){
		currHero->camDistance = 1;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	else if(camDistance > 150){
		camDistance = 150;
=======
	else if(currHero->camDistance > 15){
		currHero->camDistance = 15;
>>>>>>> f444aad (Get multiple heros working with cameras)
=======
	else if(currHero->camDistance > 150){
		currHero->camDistance = 150;

>>>>>>> aaf8ff6 (Added wanderer along the surface)
	}
	recomputeOrientation();
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

// drawGrid() //////////////////////////////////////////////////////////////////
//
//  Function to draw a grid in the XZ-Plane using OpenGL 2D Primitives (GL_LINES)
//
////////////////////////////////////////////////////////////////////////////////
void drawGrid() {
	/*
     *	We will get to why we need to do this when we talk about lighting,
     *	but for now whenever we want to draw something with an OpenGL
     *	Primitive - like a line, quad, point - we need to disable lighting
     *	and then reenable it for use with the CSCI441 3D Objects.
     */
	glDisable( GL_LIGHTING );

	glColor3ub(255, 255, 255);
	glBegin(GL_LINES);
		for(int i = -50; i < 50; i++){
			glVertex3f(i, 0, -50);
			glVertex3f(i, 0, 50);
			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);
		}
	glEnd();

	/*
     *	As noted above, we are done drawing with OpenGL Primitives, so we
     *	must turn lighting back on.
     */
	glEnable( GL_LIGHTING );
}

<<<<<<< HEAD
void drawCactus() {

	GLfloat matColorC[4] = { 0.0, 0.05, 0.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matColorC);

	GLfloat matColorC1[4] = { 0.2, 0.3, 0.2 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matColorC1);

	GLfloat matColorC2[4] = { 0.04, 0.6, 0.04, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matColorC2);



	glColor3f(0, 1.0, 0);

	glPushMatrix();
	glTranslatef(0, -10.0, 0);
	CSCI441::drawSolidCylinder(1.0, 1.0, 20.0, 20, 20);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 10.0, 0);
	CSCI441::drawSolidSphere(1.0, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	glTranslatef(0, 0, -6);
	CSCI441::drawSolidCylinder(.5, .5, 4.0, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 6, 4);
	CSCI441::drawSolidSphere(.5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 6, 4);
	CSCI441::drawSolidCylinder(.5, .5, 1.5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 7.5, 4);
	CSCI441::drawSolidSphere(.5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 1.0, 0, 0);
	glTranslatef(0, -4, -6);
	CSCI441::drawSolidCylinder(.5, .5, 4.0, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 6, -4);
	CSCI441::drawSolidSphere(.5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 6, -4);
	CSCI441::drawSolidCylinder(.5, .5, 2.5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 8.5, -4);
	CSCI441::drawSolidSphere(.5, 20, 20);
	glPopMatrix();

}

void drawTopHalf(){ // Draws body, head (with eyes), and horns of character
	// Draws body (torso)
	glColor3ub(82, 84, 105);
	CSCI441::drawSolidCone(.35, 1, 5, 20);

	// Draws bottom of cloak
	rotateMtx = glm::rotate(glm::mat4(), static_cast<float>(M_PI)/2.0f, glm::vec3(1, 0, 0));
	glMultMatrixf(&rotateMtx[0][0]);
	CSCI441::drawSolidDisk(0, 0.35, 20, 20);
	rotateMtx = glm::rotate(glm::mat4(), static_cast<float>(-M_PI)/2.0f, glm::vec3(1, 0, 0));
	glMultMatrixf(&rotateMtx[0][0]);

	// Draws head
	transMtx = glm::translate(glm::mat4(), glm::vec3(0, 0.95, 0));
	glMultMatrixf(&transMtx[0][0]);
	glColor3ub(255, 255, 255);
	CSCI441::drawSolidSphere(.3, 20, 20);
	transMtx = glm::translate(glm::mat4(), glm::vec3(0, -0.95, 0));
	glMultMatrixf(&transMtx[0][0]);

	// Draws horns
	scaleMtx = glm::scale(glm::mat4(), glm::vec3(1, 5, 1));
	glMultMatrixf(&scaleMtx[0][0]);
	transMtx = glm::translate(glm::mat4(), glm::vec3(-0.2, 0.225, 0));
	glMultMatrixf(&transMtx[0][0]);
	rotateMtx = glm::rotate(glm::mat4(), -0.5f, glm::vec3(0, 0, 1));
	glMultMatrixf(&rotateMtx[0][0]);
	CSCI441::drawSolidCube(0.1);
	rotateMtx = glm::rotate(glm::mat4(), 0.5f, glm::vec3(0, 0, 1));
	glMultMatrixf(&rotateMtx[0][0]);
	transMtx = glm::translate(glm::mat4(), glm::vec3(0.2, -0.225, 0));
	glMultMatrixf(&transMtx[0][0]);
	scaleMtx = glm::scale(glm::mat4(), glm::vec3(1, 0.2, 1));
	glMultMatrixf(&scaleMtx[0][0]);
	scaleMtx = glm::scale(glm::mat4(), glm::vec3(1, 5, 1));
	glMultMatrixf(&scaleMtx[0][0]);
	transMtx = glm::translate(glm::mat4(), glm::vec3(0.2, 0.225, 0));
	glMultMatrixf(&transMtx[0][0]);
	rotateMtx = glm::rotate(glm::mat4(), 0.5f, glm::vec3(0, 0, 1));
	glMultMatrixf(&rotateMtx[0][0]);
	CSCI441::drawSolidCube(0.1);
	rotateMtx = glm::rotate(glm::mat4(), -0.5f, glm::vec3(0, 0, 1));
	glMultMatrixf(&rotateMtx[0][0]);
	transMtx = glm::translate(glm::mat4(), glm::vec3(-0.2, -0.225, 0));
	glMultMatrixf(&transMtx[0][0]);
	scaleMtx = glm::scale(glm::mat4(), glm::vec3(1, 0.2, 1));
	glMultMatrixf(&scaleMtx[0][0]);

	// Draws eyes
	glColor3ub(0, 0, 0);
	transMtx = glm::translate(glm::mat4(), glm::vec3(-0.1, 0.9, 0.25));
	glMultMatrixf(&transMtx[0][0]);
	CSCI441::drawSolidSphere(0.05, 20, 20);
	transMtx = glm::translate(glm::mat4(), glm::vec3(0.1, 0.2, -0.25));
	glMultMatrixf(&transMtx[0][0]);
	transMtx = glm::translate(glm::mat4(), glm::vec3(0.1, -0.2, 0.25));
	glMultMatrixf(&transMtx[0][0]);
	CSCI441::drawSolidSphere(0.05, 20, 20);
	transMtx = glm::translate(glm::mat4(), glm::vec3(-0.1, -0.9, -0.25));
	glMultMatrixf(&transMtx[0][0]);
}

void drawLeg(){ // Draws a single leg
	glColor3ub(30, 30, 30);
	CSCI441::drawSolidCylinder(0.08, 0.08, 0.3, 20, 20);
	CSCI441::drawSolidSphere(0.08, 20, 20);
}

void drawCharacter() { // Draws character from upper half and two legs
	transMtx = glm::translate(glm::mat4(), heroPos); // Checks the orientation of the hero and draws the upper half accordingly
	glMultMatrixf(&transMtx[0][0]);
	rotateMtx = glm::rotate(glm::mat4(), heroAngle, glm::vec3(0, 1, 0));
	glMultMatrixf(&rotateMtx[0][0]);
	transMtx = glm::translate(glm::mat4(), glm::vec3(0, animationFrame / 20, 0));
	glMultMatrixf(&transMtx[0][0]);
	drawTopHalf();
	transMtx = glm::translate(glm::mat4(), glm::vec3(0, -animationFrame / 20, 0));
	glMultMatrixf(&transMtx[0][0]);

	transMtx = glm::translate(glm::mat4(), glm::vec3(-0.1, -0.22, 0)); // Draws both legs according to current animation frame
	glMultMatrixf(&transMtx[0][0]);
	rotateMtx = glm::rotate(glm::mat4(), 1.2f*animationFrame, glm::vec3(1, 0, 0));
	glMultMatrixf(&rotateMtx[0][0]);
	drawLeg();

	rotateMtx = glm::rotate(glm::mat4(), -1.2f*animationFrame, glm::vec3(1, 0, 0));
	glMultMatrixf(&rotateMtx[0][0]);
	transMtx = glm::translate(glm::mat4(), glm::vec3(0.2, 0, 0));
	glMultMatrixf(&transMtx[0][0]);
	rotateMtx = glm::rotate(glm::mat4(), -1.2f*animationFrame, glm::vec3(1, 0, 0));
	glMultMatrixf(&rotateMtx[0][0]);
	drawLeg();

	rotateMtx = glm::rotate(glm::mat4(), 1.2f*animationFrame, glm::vec3(1, 0, 0)); // Undoes transformations
	glMultMatrixf(&rotateMtx[0][0]);
	transMtx = glm::translate(glm::mat4(), glm::vec3(-0.1, 0.22, 0));
	glMultMatrixf(&transMtx[0][0]);
	rotateMtx = glm::rotate(glm::mat4(), -heroAngle, glm::vec3(0, 1, 0));
	glMultMatrixf(&rotateMtx[0][0]);
	transMtx = glm::translate(glm::mat4(), -heroPos);
	glMultMatrixf(&transMtx[0][0]);
}

=======
>>>>>>> f444aad (Get multiple heros working with cameras)
void drawLamppost(){ // Draws a single lamppost
	GLfloat matColorL[4] = { 0.0, 0.0, 0.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matColorL);

	GLfloat matColorL1[4] = { 0.01, 0.01, 0.01 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matColorL1);

	GLfloat matColorL2[4] = { 0.5, 0.5, 0.5, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matColorL2);

	transMtx = glm::translate(glm::mat4(), glm::vec3(10, 0, 10)); // Moves lamppost to (10, 0, 10);
	glMultMatrixf(&transMtx[0][0]);

	transMtx = glm::translate(glm::mat4(), glm::vec3(0, 2.37, 0)); // Draws post
	glMultMatrixf(&transMtx[0][0]);
	scaleMtx = glm::scale(glm::mat4(), glm::vec3(1, 16, 1));
	glMultMatrixf(&scaleMtx[0][0]);
	glColor3ub(160, 160, 160);
	CSCI441::drawSolidCube(0.3);

	scaleMtx = glm::scale(glm::mat4(), glm::vec3(1, 1.0/16, 1)); // Draws the top cover for the light
	glMultMatrixf(&scaleMtx[0][0]);
	transMtx = glm::translate(glm::mat4(), glm::vec3(0, -2.37, 0));
	glMultMatrixf(&transMtx[0][0]);
	transMtx = glm::translate(glm::mat4(), glm::vec3(0.35, 4.9, 0));
	glMultMatrixf(&transMtx[0][0]);
	scaleMtx = glm::scale(glm::mat4(), glm::vec3(3, 1, 2));
	glMultMatrixf(&scaleMtx[0][0]);
	CSCI441::drawSolidCube(0.3);

	glDisable(GL_LIGHTING);
	transMtx = glm::translate(glm::mat4(), glm::vec3(0, -0.2, 0)); // Draws the light
	glMultMatrixf(&transMtx[0][0]);
	glColor3ub(255, 255, 0);
	CSCI441::drawSolidCube(0.2);
	glEnable(GL_LIGHTING);

	transMtx = glm::translate(glm::mat4(), glm::vec3(0, 0.2, 0)); // Undoes transformations
	glMultMatrixf(&transMtx[0][0]);
	scaleMtx = glm::scale(glm::mat4(), glm::vec3(1.0/3, 1, 0.5));
	glMultMatrixf(&scaleMtx[0][0]);
	transMtx = glm::translate(glm::mat4(), glm::vec3(-0.35, -4.9, 0));
	glMultMatrixf(&transMtx[0][0]);
	transMtx = glm::translate(glm::mat4(), glm::vec3(-10, 0, -10));
	glMultMatrixf(&transMtx[0][0]);
}

<<<<<<< HEAD
<<<<<<< HEAD
void drawVehicleNotParameterized() {
<<<<<<< HEAD
	if (racerPos > ceil((curveControlPoints.size()) / 3))
		racerPos = 0;
=======
void drawVehicleNotParameterized(HeroBase* racer) {
<<<<<<< HEAD
>>>>>>> 3a3c990 (something)


=======
>>>>>>> 0d05240 (Got parameterized stuff working)
=======
>>>>>>> d26ca5a (yay)
=======
void drawVehicleNotParameterized(HeroBase* racer) {	
>>>>>>> 6b566da (Fix hero following)
	//move to location on bezier curve
	int p0 = floor(racerPos) * 3;
	float t = racerPos - floor(racerPos);
	//cout << t << endl;

	glm::vec3 loc = evaluateBezierCurve2(curveControlPoints.at(p0), curveControlPoints.at(p0 + 1), curveControlPoints.at(p0 + 2), curveControlPoints.at(p0 + 3), t);
	
	racer->pos = glm::vec3(loc.x, calcHeight(loc.x, loc.z), loc.z);
	//recomputeOrientation(racer);

	glm::mat4 transMtx = glm::translate(glm::mat4(), glm::vec3(loc.x, calcHeight(loc.x, loc.z), loc.z));
	glMultMatrixf(&transMtx[0][0]);
	//draw vehicle
	float t2 = racerPos + .01;
	t2 = t2 - floor(t2);

	//float yaw = glm::cross((evaluateBezierCurve2(curveControlPoints.at(p0), curveControlPoints.at(p0 + 1), curveControlPoints.at(p0 + 2), curveControlPoints.at(p0 + 3), t2) - loc), racer->yaw) ;
	racer->yaw = yaw;

	//rotateMtx = glm::rotate(glm::mat4(), yaw, glm::vec3(0, 1, 0));
	//glMultMatrixf(&rotateMtx[0][0]);

	racer->draw(true);
	recomputeOrientation(racer);

	//rotateMtx = glm::rotate(glm::mat4(), -yaw, glm::vec3(0, 1, 0));
	//glMultMatrixf(&rotateMtx[0][0]);

	glMultMatrixf(&(glm::inverse(transMtx))[0][0]);


}

void drawVehicleParameterized(HeroBase* racer) {
	//lerp
	float t = getParameterizedt(racerPos);
	//move to location on bezier curve
	int p0 = floor(t) * 3;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	t = t - floor(t);
<<<<<<< HEAD
	cout << t << endl;
>>>>>>> 0b785c0 (changed points some)
=======
	//cout << t << endl;
<<<<<<< HEAD
>>>>>>> 6af7604 (parameterization)
	glm::vec3 loc = evaluateBezierCurve(curveControlPoints.at(p0), curveControlPoints.at(p0 + 1), curveControlPoints.at(p0 + 2), curveControlPoints.at(p0 + 3), t);
<<<<<<< HEAD
=======
	t = t - floor(t);
	glm::vec3 loc = evaluateBezierCurve(controlPoints.at(p0), controlPoints.at(p0 + 1), controlPoints.at(p0 + 2), controlPoints.at(p0 + 3), t);
>>>>>>> 0d05240 (Got parameterized stuff working)
	glm::mat4 transMtx = glm::translate(glm::mat4(), glm::vec3(loc.x, loc.y, loc.z));
=======
=======
	glm::vec3 loc = evaluateBezierCurve2(curveControlPoints.at(p0), curveControlPoints.at(p0 + 1), curveControlPoints.at(p0 + 2), curveControlPoints.at(p0 + 3), t);
<<<<<<< HEAD
>>>>>>> 427c140 (merging)
	glm::mat4 transMtx = glm::translate(glm::mat4(), glm::vec3(loc.x, calcHeight(loc.x, loc.z), loc.z));
>>>>>>> 3a3c990 (something)
	glMultMatrixf(&transMtx[0][0]);
=======
	
	racer->pos = glm::vec3(loc.x, calcHeight(loc.x, loc.z), loc.z);
	//recomputeOrientation(racer);

	
	glm::mat4 transMtx = glm::translate(glm::mat4(), glm::vec3(loc.x, calcHeight(loc.x, loc.z), loc.z));
	//glMultMatrixf(&transMtx[0][0]);
>>>>>>> 6b566da (Fix hero following)
	//draw vehicle
	float t2 = racerPos + .01;
	t2 = t2 - floor(t2);
	//glm::vec3 yaw = evaluateBezierCurve2(curveControlPoints.at(p0), curveControlPoints.at(p0 + 1), curveControlPoints.at(p0 + 2), curveControlPoints.at(p0 + 3), t2) - loc;
	racer->yaw = yaw;

	//rotateMtx = glm::rotate(glm::mat4(), yaw, glm::vec3(0, 1, 0));
	//glMultMatrixf(&rotateMtx[0][0]);

	racer->draw(true);
	recomputeOrientation(racer);

	//rotateMtx = glm::rotate(glm::mat4(), -yaw, glm::vec3(0, 1, 0));
	//glMultMatrixf(&rotateMtx[0][0]);

	//glMultMatrixf(&(glm::inverse(transMtx))[0][0]);



}

void drawLandscape() {

<<<<<<< HEAD
}
=======
void drawTrack() {

	for (size_t i = 0; i < curveControlPoints.size() - 3; i += 3) {
		for (float j = 0; j < 10; j += 1) {
			glm::vec3 point = evaluateBezierCurve2(curveControlPoints.at(i), curveControlPoints.at(i + 1), curveControlPoints.at(i + 2), curveControlPoints.at(i + 3), j / 10);
			glPushMatrix();
			glTranslatef(point.x, calcHeight(point.x, point.z) -3.0, point.z);
			CSCI441::drawSolidCube(5.5);
			//glRotatef(90, 1, 0, 0);
			//CSCI441::drawSolidCylinder(1.0, 1.0, 2.0, 20, 20);
			glPopMatrix();
		}
	}
}


>>>>>>> d26ca5a (yay)

// generateEnvironmentDL() /////////////////////////////////////////////////////
//
//  This function creates a display list with the code to draw a simple
//      environment for the user to navigate through.
//
//  And yes, it uses a global variable for the display list.
//  I know, I know! Kids: don't try this at home. There's something to be said
//      for object-oriented programming after all.
//
////////////////////////////////////////////////////////////////////////////////
void generateEnvironmentDL() {
	environmentDL = glGenLists(1);
	glNewList(environmentDL, GL_COMPILE);
	//drawGrid();
	//drawTrack();
	glEndList();


	terrainDL = glGenLists(1);

<<<<<<< HEAD


=======
>>>>>>> d26ca5a (yay)
	glNewList(terrainDL, GL_COMPILE);
	GLfloat matColorD[4] = { 0.0215,0.1745 ,0.0215,1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matColorD);

	GLfloat matColorD1[4] = { 0.07568,0.61424 ,0.07568,1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matColorD1);

	GLfloat matColorD2[4] = { 0.633 ,0.727811,0.633,1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matColorD2);

	glMaterialf(GL_FRONT, GL_SHININESS, .0 * 128);
	for (unsigned int i = 0; i < controlPoints.size(); i++) {
		renderBezierSurface(controlPoints[i], surfaceRes);
	}
	glEndList();

}

//
//	void renderScene()
//
//		This method will contain all of the objects to be drawn.
//
void renderScene(void)  {
	// update vehicle position
	racerPos += .01;
	if (racerPos > floor((curveControlPoints.size()) / 3))
		racerPos = 0;

	glCallList(environmentDL);
<<<<<<< HEAD
<<<<<<< HEAD
	//glCallList(terrainDL);

	drawLamppost();
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> b513a88 (still working)
	glPushMatrix();
	glScalef(.5, .5, .5);
=======
=======
	
>>>>>>> d26ca5a (yay)
	//drawCharacter();
	//drawLamppost();

>>>>>>> 4905365 (am)
	GLfloat matColor[4] = { 0.135,0.2225,0.1575 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matColor);

	GLfloat matColor1[4] = { 0.54,0.89,0.63 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matColor1);

	GLfloat matColor2[4] = { 0.316228,0.316228,0.316228 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matColor2);
	for (int i = 0; i < cactusPoints.size(); i++) {
		glPushMatrix();
		glTranslatef(cactusPoints[i].x, calcHeight(cactusPoints[i].x, cactusPoints[i].y), cactusPoints[i].y);
		glScalef(.5, .5, .5);
		drawCactus();
		glPopMatrix();
	}

	for (int i = 0; i < lampPoints.size(); i++) {
		glPushMatrix();
		glTranslatef(lampPoints[i].x, calcHeight(lampPoints[i].x, lampPoints[i].y) -5.0, lampPoints[i].y);
		drawLamppost();
		glPopMatrix();
	}


	// Draw all the heros
	//glPushMatrix();
	//glTranslatef(0, 10, 0);
<<<<<<< HEAD

<<<<<<< HEAD
=======
	drawBezierCurve();

<<<<<<< HEAD
	drawVehicleParameterized(&alex);
	drawVehicleNotParameterized(&josh);
	drawVehicleParameterized(&sav);
	david.draw(false);
	/*
>>>>>>> 3a3c990 (something)
=======
=======
	//drawBezierCurve();
	drawTrack();
>>>>>>> d26ca5a (yay)
	GLfloat matColorA[4] = { 0.19225, 0.19225, 0.19225 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matColorA);

	GLfloat matColorA1[4] = { 0.40754, 0.40754, 0.40754 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matColorA1);

	GLfloat matColorA2[4] = { 0.408273, 0.408273, 0.408273, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matColorA2);

<<<<<<< HEAD
>>>>>>> 0d3e800 (Changed material colors for everything)
=======
	drawVehicleNotParameterized(&alex);

	GLfloat matColorJ[4] = { 0.05, 0.05, 0.05 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matColorJ);

	GLfloat matColorJ1[4] = { 0.5, 0.5, 0.5 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matColorJ1);

	GLfloat matColorJ2[4] = { 0.7, 0.7, 0.7, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matColorJ2);

	drawVehicleNotParameterized(&josh);

	GLfloat matColorS[4] = { 0.1745, 0.01175, 0.01175 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matColorS);

	GLfloat matColorS1[4] = { 0.41424, 0.04136, 0.04136 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matColorS1);

	GLfloat matColorS2[4] = { 0.427811, 0.326959, 0.326959, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matColorS2);

	drawVehicleParameterized(&sav);

	//mila.draw(false);
	/*
>>>>>>> d26ca5a (yay)
	alex.draw(false);
<<<<<<< HEAD
=======
	mila.draw(false);
	josh.draw(false);
	sav.draw(false);
	//glPopMatrix();make
	*/
	
	//GLfloat matColorD[4] = { 0.0215,0.1745 ,0.0215,1.0 };
	//glMaterialfv(GL_FRONT, GL_AMBIENT, matColorD);
>>>>>>> a267889 (Updated hero name)

<<<<<<< HEAD
=======

	//drawBezierCurve();






	

	drawCactus();
	glPopMatrix();
	// Draw all the heros
>>>>>>> d26ca5a (yay)
	GLfloat matColorD[4] = { 0.1, 0.18725, 0.1745 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matColorD);

	GLfloat matColorD1[4] = { 0.39, 0.74151, 0.69102 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matColorD1);

	GLfloat matColorD2[4] = { 0.297254, 0.30829, 0.306678, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matColorD2);

	mila.draw(false);

	GLfloat matColorJ[4] = { 0.05, 0.05, 0.05 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matColorJ);

	GLfloat matColorJ1[4] = { 0.5, 0.5, 0.5 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matColorJ1);

	GLfloat matColorJ2[4] = { 0.7, 0.7, 0.7, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matColorJ2);

	josh.draw(false);

	GLfloat matColorS[4] = { 0.1745, 0.01175, 0.01175 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matColorS);

	GLfloat matColorS1[4] = { 0.41424, 0.04136, 0.04136 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matColorS1);

	GLfloat matColorS2[4] = { 0.427811, 0.326959, 0.326959, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matColorS2);

	sav.draw(false);
	//glPopMatrix();make
	*/
	/*
	GLfloat matColorD[4] = { 0.0215,0.1745 ,0.0215,1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matColorD);

	GLfloat matColorD1[4] = { 0.07568,0.61424 ,0.07568,1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matColorD1);

	GLfloat matColorD2[4] = { 0.633 ,0.727811,0.633,1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matColorD2);

	glMaterialf(GL_FRONT, GL_SHININESS, .001 * 128);
<<<<<<< HEAD

	drawCactus();
	glPopMatrix();
<<<<<<< HEAD
	
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> d49867c (Small edit)
=======
=======
	
=======
	// Draw all the heros
	alex.draw(false);
	david.draw(false);


>>>>>>> aaf8ff6 (Added wanderer along the surface)
	glColor3ub(255, 255, 0);
	for(unsigned int i = 0; i + 1 < controlPoints.size(); i+=3){
		//renderBezierCurve(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3], 20);
	}
	*/
>>>>>>> b513a88 (still working)

<<<<<<< HEAD
<<<<<<< HEAD
	//drawVehicleParameterized();
	//drawVehicleNotParameterized();
=======
	// Draw all the heros
	alex.draw(false);
	david.draw(false);
>>>>>>> f444aad (Get multiple heros working with cameras)
=======
	drawVehicleParameterized();
	drawVehicleNotParameterized();
>>>>>>> 0b785c0 (changed points some)
=======
	//drawVehicleParameterized();
	//drawVehicleNotParameterized();
>>>>>>> 3a3c990 (something)

	glColor3ub(0, 255, 0);
	for(unsigned int i = 0; i < controlPoints.size(); i++){
		transMtx = glm::translate(glm::mat4(), glm::vec3(controlPoints[i].x, controlPoints[i].y, controlPoints[i].z));
		glMultMatrixf(&transMtx[0][0]);
		glLoadName(i);
		CSCI441::drawSolidSphere(0.07, 20, 20);
		glMultMatrixf(&(glm::inverse(transMtx))[0][0]);
	}
	
>>>>>>> 0d05240 (Got parameterized stuff working)
	//draws curve
	glDisable(GL_LIGHTING);
	
	glLineWidth(3);
	glColor3ub(0, 0, 255);
	glBegin(GL_LINE_LOOP);
	for(unsigned int i = 0; i < controlPoints.size(); i++){
		glVertex3f(controlPoints[i].x, controlPoints[i].y, controlPoints[i].z);
	}
	glEnd();
	glLineWidth(1);
	
	glColor3ub(255, 255, 0);
	for(unsigned int i = 0; i + 1 < controlPoints.size(); i+=3){
<<<<<<< HEAD
	//	renderBezierCurve(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3], 20);
=======
		//renderBezierCurve(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3], 20);
>>>>>>> ba6ea04 (Work on cameras)
	}
<<<<<<< HEAD
>>>>>>> ecc2a23 (Added getParameterizedt function which takes in a float t and returns the respective parameterized value. Probably needs testing after everything is put together)
=======

<<<<<<< HEAD
>>>>>>> d49867c (Small edit)
=======
>>>>>>> b5cf244 (edited merge related stuff)



	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cloudTexHandle);

	glColor3ub(255, 255, 255);
	CSCI441::drawSolidCube(1000.0);

	glDisable(GL_TEXTURE_2D);

	glColor3ub(255, 255, 0);

	glEnable(GL_LIGHTING);

	glColor3ub(45, 163, 59);

	glCallList(terrainDL);
<<<<<<< HEAD
=======
	// draws surface
	renderBezierSurface(controlPoints, surfaceRes);
>>>>>>> a229d73 (Add overlay for first person)
=======

	//cout << mew.first << " " << mew.second << endl;
>>>>>>> 32113c0 (height and stuff)
}






//*************************************************************************************
//
// Setup Functions

//
//  void setupGLFW()
//
//      Used to setup everything GLFW related.  This includes the OpenGL context
//	and our window.
//
GLFWwindow* setupGLFW() {
	// set what function to use when registering errors
	// this is the ONLY GLFW function that can be called BEFORE GLFW is initialized
	// all other GLFW calls must be performed after GLFW has been initialized
	glfwSetErrorCallback( error_callback );

	// initialize GLFW
	if( !glfwInit() ) {
		fprintf( stderr, "[ERROR]: Could not initialize GLFW\n" );
		exit( EXIT_FAILURE );
	} else {
		fprintf( stdout, "[INFO]: GLFW initialized\n" );
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );	// request OpenGL v2.X
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );	// request OpenGL v2.1
	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );		// do not allow our window to be able to be resized

	// create a window for a given size, with a given title
	GLFWwindow *window = glfwCreateWindow( windowWidth, windowHeight, "Super Mario Kart", NULL, NULL );
	if( !window ) {						// if the window could not be created, NULL is returned
		fprintf( stderr, "[ERROR]: GLFW Window could not be created\n" );
		glfwTerminate();
		exit( EXIT_FAILURE );
	} else {
		fprintf( stdout, "[INFO]: GLFW Window created\n" );
	}

	glfwMakeContextCurrent(window);		// make the created window the current window
	glfwSwapInterval(1);				     	// update our screen after at least 1 screen refresh

	glfwSetKeyCallback( window, keyboard_callback );							// set our keyboard callback function
	glfwSetCursorPosCallback( window, cursor_callback );					// set our cursor position callback function
	glfwSetMouseButtonCallback( window, mouse_button_callback );	// set our mouse button callback function
	glfwSetScrollCallback(window, scroll_callback);					// set a scholl wheel callback function

	return window;						       // return the window that was created
}

//
//  void setupOpenGL()
//
//      Used to setup everything OpenGL related.  For now, the only setting
//	we need is what color to make the background of our window when we clear
//	the window.  In the future we will be adding many more settings to this
//	function.
//
void setupOpenGL() {
	// tell OpenGL to perform depth testing with the Z-Buffer to perform hidden
	//		surface removal.  We will discuss this more very soon.
	glEnable(GL_DEPTH_TEST);

	glFrontFace(GL_CCW);              // denote front faces specified by counter-clockwise winding order

	glShadeModel(GL_FLAT);            // use flat shading

	GLfloat lightAmbCol[4] = { 0, 0, 0, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbCol);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// set the clear color to black

	//******************************************************************
	// this is some code to enable a default light for the scene;
	// feel free to play around with this, but we won't talk about
	// lighting in OpenGL for another couple of weeks yet.
<<<<<<< HEAD
	float lightCol[4] = { 1, 1, 1, 1 };
	float ambientCol[4] = { 0.0, 0.0, 0.0, .5 };
	float lPosition[4] = { 10, 10, 10, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
=======
	glEnable(GL_LIGHTING);            // we are now using lighting
	glEnable(GL_LIGHT0);              // and turn on Light 0 please (and thank you)
	float diffuseCol[4] = { 0.1, 0.2, 0.2, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseCol);
	float ambientCol[4] = { 0.0, 0.0, 0.0, 1.0 };
>>>>>>> cd6cd86 (Did a couple things with lighting)
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientCol);

	glEnable(GL_NORMALIZE);           // make sure our normals stay normal after transformations

	float diffuseCol2[4] = { 1.0, 0.8, 0.8, 1.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseCol2);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 12);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100);
	glEnable(GL_LIGHT1);

	// tell OpenGL not to use the material system; just use whatever we
	// pass with glColor*()

	//******************************************************************

	// tells OpenGL to blend colors across triangles. Once lighting is
	// enabled, this means that objects will appear smoother - if your object
	// is rounded or has a smooth surface, this is probably a good idea;
	// if your object has a blocky surface, you probably want to disable this.
	glShadeModel(GL_SMOOTH);

}

//
//  void setupScene()
//
//      Used to setup everything scene related.  Give our camera an
//	al starting point and generate the display list for our city
//
void setupScene() {
	// Initialize animateVals vector to store different animation settings
	animateVals.push_back(-0.5);
	animateVals.push_back(-0.4);
	animateVals.push_back(-0.3);
	animateVals.push_back(-0.2);
	animateVals.push_back(-0.1);
	animateVals.push_back(0.0);
	animateVals.push_back(0.1);
	animateVals.push_back(0.2);
	animateVals.push_back(0.3);
	animateVals.push_back(0.4);
	animateVals.push_back(0.5);
	animateVals.push_back(0.4);
	animateVals.push_back(0.3);
	animateVals.push_back(0.2);
	animateVals.push_back(0.1);
	animateVals.push_back(0.0);
	animateVals.push_back(-0.1);
	animateVals.push_back(-0.2);
	animateVals.push_back(-0.3);
	animateVals.push_back(-0.4);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======

>>>>>>> 4905365 (am)
	// give the camera a scenic starting point.
	camPos.x = 5;
	camPos.y = 5;
	camPos.z = 5;
	cameraTheta = -M_PI / 3.0f;
	cameraPhi = M_PI / 2.8f;

	// place the hero in a default position
	heroPos = glm::vec3(0, 0.3, 0);
	heroPos.y = calcHeight(heroPos.x, heroPos.z);
	heroAngle = 0.0f;
	heroDir = glm::vec3(0, 0, 1);
>>>>>>> 32113c0 (height and stuff)
	recomputeOrientation();

	freeCamDir.x = 0.0f;
	freeCamDir.y = 0.0f;
	freeCamDir.z = 1.0f;

	freeCamPos.x = 1.0f;
	freeCamPos.y = 1.0f;
	freeCamPos.z = 1.0f;

	srand(time(NULL));	// seed our random number generator
	generateEnvironmentDL();
}

///*************************************************************************************
//
// Our main function

//
//	int main( int argc, char *argv[] )
//
//		Really you should know what this is by now.  We will make use of the parameters later
//
int main(int argc, char *argv[]) {
	/*
	if (argc != 3) {
		fprintf(stderr, "[ERROR]: Control point CSVs not passed into command line\nPass surface, then curve");
		exit(EXIT_FAILURE);
	}
	*/

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	loadSurfaceControlPoints(argv[1]);
	loadCurveControlPoints(argv[2]);
=======
	currHero = &david;

	loadControlPoints(argv[1]);
>>>>>>> f444aad (Get multiple heros working with cameras)

=======
	loadControlPoints(argv[1]);
	generateLookupTable();
>>>>>>> 0d05240 (Got parameterized stuff working)
=======
	loadSurfaceControlPoints(argv[1]);
	loadCurveControlPoints(argv[2]);
=======
	loadPoints(argv[1]);

	//loadSurfaceControlPoints(argv[1]);
	//loadCurveControlPoints(argv[2]);
<<<<<<< HEAD
>>>>>>> b513a88 (still working)
=======

	currHero = &mila;

>>>>>>> a267889 (Updated hero name)

	generateLookupTable();
>>>>>>> f90c036 (Stuff works i guess)
	// GLFW sets up our OpenGL context so must be done first
	GLFWwindow *window = setupGLFW();	// initialize all of the GLFW specific information releated to OpenGL and our window
	setupOpenGL();										// initialize all of the OpenGL specific information
	setupTextures();
	setupScene();											// initialize objects in our scene

	//  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
	//	until the user decides to close the window and quit the program.  Without a loop, the
	//	window will display once and then the program exits.
	while( !glfwWindowShouldClose(window) ) {	// check if the window was instructed to be closed
		glDrawBuffer( GL_BACK );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLint framebufferWidth, framebufferHeight;
		glfwGetFramebufferSize( window, &framebufferWidth, &framebufferHeight );
		glViewport(0, 0, framebufferWidth, framebufferHeight);

		glm::mat4 projMtx, viewMtx;

		projMtx = glm::perspective( 45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.001f, 1000.0f );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glMultMatrixf( &projMtx[0][0] );

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		if(currCam == ARCBALL_CAM) {
			viewMtx = glm::lookAt( currHero->camPos, currHero->pos + glm::vec3(0, 1, 0), glm::vec3(  0,  1,  0 ) );
		} else if (currCam == FREE_CAM) {
			freeCamDir.x = cos(pitch) * cos(yaw);
			freeCamDir.y = sin(pitch);
			freeCamDir.z = cos(pitch) * sin(yaw);

			freeCamDir = glm::normalize(freeCamDir);

			if(walking) {
				freeCamPos += freeCamDir * cameraSpeed * direction;
			}

			viewMtx = glm::lookAt(freeCamPos, freeCamPos + freeCamDir, glm::vec3(0, 1, 0));
		}
		glMultMatrixf( &viewMtx[0][0] );
		renderScene();

		if(viewOverlay) {
			int overlayX = windowWidth - overlaySize;
			int overlayY = windowHeight - overlaySize;

			glEnable(GL_SCISSOR_TEST);
			glScissor(overlayX, overlayY, overlaySize, overlaySize);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			glDisable(GL_SCISSOR_TEST);

			glViewport(overlayX, overlayY, overlaySize, overlaySize);

			projMtx = glm::perspective( 45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.001f, 1000.0f );
			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();
			glMultMatrixf(&projMtx[0][0]);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			// First person camera view matrix
			glm::vec3 normalDir = glm::normalize(currHero->direction);
			glm::vec3 pos = glm::vec3(currHero->pos.x, currHero->pos.y + 2.0f, currHero->pos.z);
			viewMtx = glm::lookAt(pos + 0.5f*normalDir, pos + normalDir, glm::vec3(  0,  1,  0 ) );
			glMultMatrixf(&viewMtx[0][0]);
			renderScene();
		}

		if(currCam == ARCBALL_CAM) {
			// Checks what direction the camera is moving (if any) and recomputes camera orientation
			if(cameraIn) {
				currHero->camDistance -= 0.2;
				recomputeOrientation();
				if(currHero->camDistance < 5){
					currHero->camDistance = 5;
					recomputeOrientation();
				}
			}
			else if(cameraOut){
				currHero->camDistance += 0.2;
				recomputeOrientation();
				if(currHero->camDistance > 150){
					currHero->camDistance = 150;
					recomputeOrientation();
				}
			}
			else if(cameraLeft){
				currHero->cameraTheta += 0.05;
				recomputeOrientation();
			}
			else if(cameraRight){
				currHero->cameraTheta -= 0.05;
				recomputeOrientation();
			}
		}

		// Whatever hero we want to freely walk around
		if(currCam == ARCBALL_CAM) {
			// Checks what the hero is doing, and moves/animates the hero accordingly
			if(walking && turning){
				currHero->pos = currHero->pos + (direction * walkSpeed * currHero->direction);
				currHero->pos.y = calcHeight(currHero->pos.x, currHero->pos.z);
				currHero->yaw += turnDirection * turnSpeed;

<<<<<<< HEAD
<<<<<<< HEAD
=======
				currHero->pos.y = calcHeight(currHero->pos.x, currHero->pos.z);
				//cout << currHero->pos.x / scaleConstant << " " << currHero->pos.y / heightScaleConstant << " " << currHero->pos.z / scaleConstant << endl;

				recomputeOrientation();
				checkBounds();

				animateIndex = (animateIndex + 1) % animateVals.size();
				// animationFrame = animateVals[animateIndex];
				mila.setAnimationFrame(animateVals[animateIndex]);
			}
			else if(walking){
				currHero->pos = currHero->pos + (direction * walkSpeed * currHero->direction);
//

				/*
>>>>>>> a267889 (Updated hero name)
				float shortestDist = 100000.0;
				float newY = 0.0;
				map<pair<float, float>, float>::iterator it = surfacePoints.begin();

				while(it != surfacePoints.end()){
					if(getDist(it->first.first, it->first.second, currHero->pos.x, currHero->pos.z) < shortestDist){
						shortestDist = getDist(it->first.first, it->first.second, currHero->pos.x, currHero->pos.z);
						newY = it->second;
					}
					it++;
				}
<<<<<<< HEAD
				currHero->pos.y = newY + 2.5;
=======
				currHero->pos.y = newY;
=======
				currHero->pos.y = calcHeight(currHero->pos.x, currHero->pos.z);
>>>>>>> 427c140 (merging)
				//cout << currHero->pos.x / scaleConstant << " " << currHero->pos.y / heightScaleConstant << " " << currHero->pos.z / scaleConstant << endl;
>>>>>>> 6af7604 (parameterization)

				recomputeOrientation();
				checkBounds();

				animateIndex = (animateIndex + 1) % animateVals.size();
				// animationFrame = animateVals[animateIndex];
				david.setAnimationFrame(animateVals[animateIndex]);
			}
			else if(walking){
				currHero->pos = currHero->pos + (direction * walkSpeed * currHero->direction);
<<<<<<< HEAD
<<<<<<< HEAD
				currHero->pos.y = calcHeight(currHero->pos.x, currHero->pos.z);
=======

=======
//

				/*
>>>>>>> 427c140 (merging)
				float shortestDist = 100000.0;
				float newY = 0.0;
				map<pair<float, float>, float>::iterator it = surfacePoints.begin();

				while(it != surfacePoints.end()){
					if(getDist(it->first.first, it->first.second, currHero->pos.x, currHero->pos.z) < shortestDist){
						shortestDist = getDist(it->first.first, it->first.second, currHero->pos.x, currHero->pos.z);
						newY = it->second;
					}
					it++;
<<<<<<< HEAD
				}
<<<<<<< HEAD
				currHero->pos.y = newY;

>>>>>>> aaf8ff6 (Added wanderer along the surface)
=======
				currHero->pos.y = newY ;
=======
				} */
				currHero->pos.y = calcHeight(currHero->pos.x, currHero->pos.z);
>>>>>>> 427c140 (merging)
				//cout << currHero->pos.x / scaleConstant << " " << currHero->pos.y / heightScaleConstant << " " << currHero->pos.z / scaleConstant<< endl;
>>>>>>> 6af7604 (parameterization)
				recomputeOrientation();
				checkBounds();

				animateIndex = (animateIndex + 1) % animateVals.size();
				// animationFrame = animateVals[animateIndex];
				mila.setAnimationFrame(animateVals[animateIndex]);
			}
			else if(turning){
				currHero->yaw += turnDirection * turnSpeed;
				recomputeOrientation();

				animateIndex = (animateIndex + 1) % animateVals.size();
				// animationFrame = animateVals[animateIndex];
				mila.setAnimationFrame(animateVals[animateIndex]);
			}
			else{
				// animationFrame = 0; // Default state for the hero
				mila.setAnimationFrame(0);
				animateIndex = 5;
			}
		}
<<<<<<< HEAD
=======
		else if(cameraLeft){
			cameraTheta += 0.05;
			recomputeOrientation();
		}
		else if(cameraRight){
			cameraTheta -= 0.05;
			recomputeOrientation();
		}

		// Checks what the hero is doing, and moves/animates the hero accordingly
		if(walking && turning){
			heroPos = heroPos + (direction * walkSpeed * heroDir);
			heroPos.y = calcHeight(heroPos.x, heroPos.z);
			heroAngle += turnDirection * turnSpeed;
			recomputeOrientation();
			checkBounds();

			animateIndex = (animateIndex + 1) % animateVals.size();
			animationFrame = animateVals[animateIndex];
		}
		else if(walking){
			heroPos = heroPos + (direction * walkSpeed * heroDir);
			heroPos.y = calcHeight(heroPos.x, heroPos.z);
			recomputeOrientation();
			checkBounds();

			animateIndex = (animateIndex + 1) % animateVals.size();
			animationFrame = animateVals[animateIndex];
		}
		else if(turning){
			heroAngle += turnDirection * turnSpeed;
			recomputeOrientation();

			animateIndex = (animateIndex + 1) % animateVals.size();
			animationFrame = animateVals[animateIndex];
		}
		else{
			animationFrame = 0; // Default state for the hero
			animateIndex = 5;
		}


>>>>>>> 32113c0 (height and stuff)

		glfwSwapBuffers(window);// flush the OpenGL commands and make sure they get rendered!
		glfwPollEvents();				// check for any events and signal to redraw screen
	}

	glfwDestroyWindow( window );// clean up and close our window
	glfwTerminate();						// shut down GLFW to clean up our context

	return EXIT_SUCCESS;				// exit our program successfully!
}
