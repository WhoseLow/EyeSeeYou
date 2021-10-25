//OpenCV Include Files
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/types_c.h"

//SDL Include Files
#include <SDL2/SDL.h>

//Base Include Files
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <thread>
#include <chrono>

struct eyePoint {
	int x, y;
};

using namespace std;
using namespace cv;

string cascadeName, nestedCascadeName;

SDL_Rect rectAt(int x, int y);

int distBetween(eyePoint a, eyePoint b);

int IMG_W = 640;
int IMG_H = 480;
int PUPIL_S = 180;
int FPS = 60;


int main(int argc, char ** argv) {
	srand(time(NULL));
   // VideoCapture class for playing video for which faces to be detected
	VideoCapture capture;
	Mat frame, image;

	// PreDefined trained XML classifiers with facial features
	CascadeClassifier cascade;
	double scale=1;

	// Change path before execution
	cascade.load( "/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml" ) ;

   SDL_Event event;

   SDL_Init(SDL_INIT_VIDEO);
 
   SDL_Window * window = SDL_CreateWindow("EyeSeeYou",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, IMG_W, IMG_H, SDL_WINDOW_SHOWN);

   SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

   //create background texture
   SDL_Surface * load_image = SDL_LoadBMP ("resources/background.bmp");
   SDL_Texture * background = SDL_CreateTextureFromSurface(renderer, load_image);

   //create pupil texture
   load_image = SDL_LoadBMP ("resources/pupil.bmp");
   SDL_Texture * pupil = SDL_CreateTextureFromSurface(renderer, load_image);

   //create outer face layer
   load_image = SDL_LoadBMP ("resources/face.bmp");
   SDL_Texture * face = SDL_CreateTextureFromSurface(renderer, load_image);


	capture.open(0);
	if( capture.isOpened() ) {
		// Capture frames from video and detect faces
		cout << "Face Detection Started...." << endl;
		
		while (1) {
			capture >> frame;
				if( frame.empty() )
					break;
			SDL_WaitEvent(&event);

			vector<Rect> faces;
			Mat gray, smallImg;

			Mat frame1 = frame.clone();
			cvtColor(frame1, gray, COLOR_BGR2GRAY);
			resize(gray,smallImg, Size(), 1, 1, INTER_LINEAR);
			equalizeHist(smallImg, smallImg);
			cascade.detectMultiScale(smallImg, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(15,15));

			struct eyePoint origin;
				origin.x = 0;
				origin.y = 0;
			struct eyePoint currPos;
			struct eyePoint tarPos;
			int failCount = 0;
			bool tracking;
			
			//logic to determine target face
			if(faces.size() < 1) {
				failCount++;
				if(failCount > 120) {
					tracking = false;
					tarPos = origin;
				}
			}
			else if(tracking) {
				for( size_t i = 0; i < faces.size(); i++) {
					Rect r = faces[i];
					struct eyePoint temp;
					temp.x = (r.x + r.width*0.5) - IMG_W/2;
					temp.y = -(r.y + r.height*0.5) + IMG_H/3;
					if(distBetween(temp, tarPos) <= 5) {
						tarPos.x = temp.x;
						tarPos.y = temp.y;
					}
				}
			}
			else {
				Rect r = faces[rand() % faces.size()];
				tarPos.x = (r.x + r.width*0.5) - IMG_W/2;
				tarPos.y = (r.y + r.height*0.5) - IMG_H/2;
				tracking = true;
			}

			//move eye towards target
			double step = 2; //px
			double angle = atan2((tarPos.y - currPos.y), (tarPos.x - currPos.x));	//radians

			currPos.x += step*cos(angle);
			currPos.y += step*sin(angle);

			this_thread::sleep_for(chrono::milliseconds(1000/FPS));


			switch (event.type) {
			case SDL_QUIT:
				break;
			}

			//add background layer
			SDL_RenderCopy(renderer, background, NULL, NULL);

			SDL_Rect dstrect = rectAt(-currPos.x, currPos.y);
			SDL_RenderCopy(renderer, pupil, NULL, &dstrect);

			//add outer face layer
			SDL_RenderCopy(renderer, face, NULL, NULL);
			SDL_RenderPresent(renderer);


			char c = (char)waitKey(10);
          
            // Press q to exit from window
			if( c == 27 || c == 'q' || c == 'Q' ) 
                break;
			}

			SDL_DestroyTexture(face);
			SDL_FreeSurface(load_image);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();

		return 0;
	}
	else
		cout<<"Could not Open Camera";
	return 0;
}



//assumes middle of image is 0,0
//TODO: once i get final art, add a limit region 
SDL_Rect rectAt(int x, int y) {
   SDL_Rect ret = {x + IMG_W/2 - PUPIL_S/2, -y + IMG_H/2 - PUPIL_S/2, PUPIL_S, PUPIL_S};
   return ret;
}

int distBetween(eyePoint a, eyePoint b) {
	int x = b.x - a.x;
	int y = b.y - a.y;
	return sqrt(pow(x,2) + pow(y,2));
}