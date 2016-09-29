//Using SDL and standard IO
#include<SDL2/SDL.h>
#include <stdio.h>
#include<stdint.h>
#include<SDL/SDL_image.h>

#define BOOL u32
#define TRUE 1
#define FALSE 0

//Screen dimension definitions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

typedef uint32_t u32;

void drawSprite(SDL_Surface* imageSurface, 
                SDL_Surface* screenSurface,
                int srcX, int srcY, 
                int dstX, int dstY,
                int width, int height);



int main( int argc, char* args[] ) {

	//The window we'll be rendering to
	SDL_Window* win = NULL;
	
	//The surface contained by the window
	SDL_Surface* ScreenSurface = NULL;

	SDL_Surface* gHelloWorld = NULL;
		
		
	// Part of the bitmap that we want to draw
   	SDL_Rect source;
   	source.x = 24;
   	source.y = 63;
   	source.w = 65;
   	source.h = 44;

   	// Part of the screen we want to draw the sprite to
   	SDL_Rect destination;
   	destination.x = 100;
   	destination.y = 100;
   	destination.w = 65;
   	destination.h = 44;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else { 
		//Create window

		SDL_Window *win;
		SDL_Renderer *sdlRenderer;
		SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &win, &sdlRenderer);
		if( win == NULL ) {
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		}
		else {
			SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
			SDL_RenderClear(sdlRenderer);
			SDL_RenderPresent(sdlRenderer);
			

			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
			SDL_RenderSetLogicalSize(sdlRenderer, 640, 480);

			

			//Load Spash Image
			gHelloWorld = SDL_LoadBMP( "/home/amritraj/Desktop/SDL2/bat.bmp" );
			SDL_SetColorKey( gHelloWorld, SDL_TRUE, SDL_MapRGB(gHelloWorld->format, 255, 0, 255) );
			SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, gHelloWorld);
			SDL_FreeSurface(gHelloWorld);


			if( sdlTexture == NULL ) {
				printf( "Unable to load image %s! SDL Error: %s\n", "/home/amritraj/Desktop/SDL2/bat.bmp", SDL_GetError() );
		
			}
		
			
			//QuitGame
			BOOL done = FALSE;
		
			while(!done){
				SDL_Event event;
				while(SDL_PollEvent(&event)) {
					if(event.type == SDL_QUIT){
						done = TRUE;
						break;
					}
					
					if(event.type != SDL_KEYDOWN){
						break;
					}
					
					SDL_Keycode code = event.key.keysym.sym;
					
					switch(code) {
						case SDLK_ESCAPE:
							done = TRUE;
							break;
						default:
							break;
					}

					

				}

				SDL_RenderClear(sdlRenderer);
				SDL_RenderCopy(sdlRenderer, sdlTexture, &source, &destination);
				SDL_RenderPresent(sdlRenderer);
			}
			
			SDL_Quit();
		}

	}
}

