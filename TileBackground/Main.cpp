#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

static int imageWidth;
static int imageHeight;

/**
 * Position function for a static tile background
 */
int* calculatePosition(int x, int y) {
	int pos[2] = { x*imageWidth, y*imageHeight };
	return pos;
}

/**
 * Position function for a moveable tile background
 */
int* calculatePosition(int x, int y, int incrX, int incrY) {
	int pos[2] = { x*imageWidth + incrX, y*imageHeight + incrY };
	return pos;
}

int main() {

	IrrlichtDevice *device =
		createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, true, false, nullptr);

	if (!device)
		return 1;
	device->setWindowCaption(L"blog.offbeat-pioneer.net - Tile-based Background");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	// Credit for the image of retro pixel penguin goes to 'NicoleMarieProductions' (http://opengameart.org/content/cute-retro-pixel-penguin-16x16)
	ITexture* penguin = driver->getTexture("./penguin.png");

	smgr->addLightSceneNode(nullptr, vector3df(0, 20, -5));

	ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	camera->setPosition(vector3df(0, 13, -40));
	camera->setTarget(vector3df(0));

	//add a cube
	smgr->addCubeSceneNode();

	int n; int m;
	int screenWidth; int screenHeight;

	//Retrieve screen, and tile size 
	screenWidth = driver->getScreenSize().Width;
	screenHeight = driver->getScreenSize().Height;

	imageWidth = penguin->getSize().Width;
	imageHeight = penguin->getSize().Height;

	//Calculate the number of tiles per row/column
	n = ceil(screenHeight / imageHeight);
	m = ceil(screenWidth / imageWidth);

	float incX = 0;
	float incY = 0;
	while (device->run()) {

		driver->beginScene(true, true, SColor(128, 128, 128, 100));


		//Build up the tile map for the background
		for (int i = -1; i <= n; i++) {
			for (int j = -1; j <= m; j++) {
				//int* pos = calculatePosition(j, i);
				int* pos = calculatePosition(j, i, incX, incY);
				driver->draw2DImage(penguin, position2d<s32>(pos[0], pos[1]));
			}
		}

		//change position of the tiles to get things moving
		if (incY >= imageHeight)
			incY = 0;
		if (incX >= imageWidth)
			incX = 0;
		incX += 0.1;
		incY += 0.1;

		smgr->drawAll();

		//Another approach instead of the two for loops
		//for (int k = 0; k < n*m; k++) {
		//	int i = k % m;
		//	int j = floor(k / m);
		//	int* pos = calculatePosition(i, j);
		//	driver->draw2DImage(penguin, position2d<s32>(pos[0], pos[1]));
		//}

		driver->endScene();
	}
	return 0;
}