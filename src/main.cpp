//include SDL2 libraries
#include <SDL.h>
#include "ppm.h"
#include "scene.h"

//C++ includes
#include <iostream>
#include <cfloat>
#include <cmath>
#include <vector>
#include <algorithm>

///
/// Log an SDL error with some error message to the output stream of our
/// choice
///
/// \param os The output stream to write the message to
/// \param msg The error message to write, SDL_GetError() appended to it
///
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

///
/// Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
/// the texture's width and height
///
/// \param tex The source texture we want to draw
/// \param ren The renderer we want to draw to
/// \param x The x coordinate to draw to
/// \param y The y coordinate to draw to
///
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;

	//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

int main(int argc, char** argv){

	//create vars for main
	ppm* image = new ppm();
	scene* sc = new scene();
	bool up = false; //update image?
	int type = 0; //type of shading

	//populate ppm
	if(argc < 3){
		std::cout << "usage: prog05 input output" << std::endl;
		exit(EXIT_FAILURE);
	}

	//load in data and store it
	sc->acquireData(argv[1]);
	sc->setup();
	sc->draw();
	image->setData(sc->returnData(type), sc->returnHeight(), 3 * sc->returnWidth());
	image->setHeight(sc->returnHeight());
	image->setWidth(sc->returnWidth());

	//Start up SDL and make sure it went ok
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

  //create window for the image, then check to make sure it loaded properly
  SDL_Window *windowImage = SDL_CreateWindow("Loaded Image", 100, 100, image->returnWidth(), image->returnHeight(), SDL_WINDOW_SHOWN);
  if (windowImage == NULL){
	  logSDLError(std::cout, "CreateWindowImage");
	  SDL_Quit();
	  return 1;
  }

  //ditto, but renderer instead of window
  SDL_Renderer *rendererImage = SDL_CreateRenderer(windowImage, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (rendererImage == NULL){
 	 logSDLError(std::cout, "CreateRendererImage");
 	 SDL_DestroyWindow(windowImage);
 	 SDL_Quit();
 	 return 1;
  }

  //The texture we'll be using
  SDL_Texture *imageTexture;

  //Initialize the texture.  SDL_PIXELFORMAT_RGB24 specifies 3 bytes per
  //pixel, one per color channel
  imageTexture = SDL_CreateTexture(rendererImage,SDL_PIXELFORMAT_RGB24,SDL_TEXTUREACCESS_STATIC,image->returnWidth(),image->returnHeight());

  //Copy the raw data array into the texture.
  SDL_UpdateTexture(imageTexture, NULL, image->returnData(), 3*image->returnWidth());
  if (imageTexture == NULL){
    logSDLError(std::cout, "CreateImageTextureFromSurface");
  }

  //Make sure it loaded ok
  if (imageTexture == NULL){
    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(rendererImage);
    SDL_DestroyWindow(windowImage);
    SDL_Quit();
    return 1;
  }
  //render loaded texture here
	renderTexture(imageTexture, rendererImage, 0, 0);

	//Update the screen
	SDL_RenderPresent(rendererImage);

    //Variables used in the rendering loop
    SDL_Event event;
	bool quit = false;

	while (!quit){
    //Grab the time for frame rate computation
    const Uint64 start = SDL_GetPerformanceCounter();

    //Clear the screen
	SDL_RenderClear(rendererImage);

		//Event Polling
    //This while loop responds to mouse and keyboard commands.
    while (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT){
				quit = true;
			}
      //Use number input to select which clip should be drawn
      if (event.type == SDL_KEYDOWN){
        switch (event.key.keysym.sym){
			case SDLK_ESCAPE: //if escape, quit
				quit = true;
			break;
			case SDLK_f:
				up = true;
				type = 0;
			break;
			case SDLK_g:
				up = true;
				type = 1;
			break;
			case SDLK_p:
				up = true;
				type = 2;
			break;
			default:
			break;
        }
      }
    }

	if(up){ //if the image was updated
		image->setData(sc->returnData(type), sc->returnHeight(), 3 * sc->returnWidth());
		SDL_UpdateTexture(imageTexture, NULL, image->returnData(), 3*image->returnWidth());
		renderTexture(imageTexture, rendererImage, 0, 0);
		SDL_RenderPresent(rendererImage);
		up = false;
	}

    //Display the frame rate to stdout, as well as current gamma value
    const Uint64 end = SDL_GetPerformanceCounter();
    const static Uint64 freq = SDL_GetPerformanceFrequency();
    const double seconds = ( end - start ) / static_cast< double >( freq );

    //You may want to comment this line out for debugging purposes
    std::cout << "Frame time: " << seconds * 1000.0 << "ms" << std::endl;
  }

    //After the loop finishes (when the window is closed, or escape is
    //pressed, clean up the data that we allocated.
	SDL_DestroyTexture(imageTexture);
	SDL_DestroyRenderer(rendererImage);
	SDL_DestroyWindow(windowImage);
	SDL_Quit();

    //write data to a SDR ppm
	image->writeData(argv[2]);

	//clear memory
	delete image;
	delete sc;

  return 0;
}
