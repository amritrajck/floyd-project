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

	SDL_Surface* imageSurface = NULL;

	SDL_Renderer *sdlRenderer;

	const int FPS = 60;
	int frameTime = 0;
		
		
	/*Part of the bitmap that we want to draw
   	SDL_Rect source;
   	source.x = 24;
   	source.y = 63;
   	source.w = 65;
   	source.h = 44;

	*/

   	// Part of the screen we want to draw the sprite to
   	SDL_Rect destination1;
   	destination1.x = 270;
   	destination1.y = 30;
   	destination1.w = 90;
   	destination1.h = 30;

	SDL_Rect destination2;
   	destination2.x = 270;
   	destination2.y = 420;
   	destination2.w = 90;
   	destination2.h = 30;

	SDL_Rect destination3;
   	destination2.x = 550;
   	destination2.y = 420;
   	destination2.w = 90;
   	destination2.h = 30;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else { 
		//Create window

		/*SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &win, &sdlRenderer);*/

		SDL_Window *win = SDL_CreateWindow("AI Hockey",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             0, 0,
                             SDL_WINDOW_FULLSCREEN_DESKTOP);

		SDL_Renderer *sdlRenderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if( win == NULL ) {
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		}
		else {
			//Clearing the screen to black			
			SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);//Draw In Black
			SDL_RenderClear(sdlRenderer);//Clear the Whole window
			SDL_RenderPresent(sdlRenderer);//Put cleared window on screen
			
			//Making it think it's a 640*480 window, but rendering to system resolution without delay 
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
			SDL_RenderSetLogicalSize(sdlRenderer, 640, 480);

			

			//Load Spash Image 
			imageSurface = SDL_LoadBMP( "laser.bmp" );
			SDL_SetColorKey( imageSurface, SDL_TRUE, SDL_MapRGB(imageSurface->format, 255, 255, 255) );
			SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, imageSurface);
			SDL_FreeSurface(imageSurface);

			imageSurface = SDL_LoadBMP( "ball.bmp" );
			SDL_SetColorKey( imageSurface, SDL_TRUE, SDL_MapRGB(imageSurface->format, 255, 255, 255) );
			SDL_Texture *sdlTexture1 = SDL_CreateTextureFromSurface(sdlRenderer, imageSurface);
			SDL_FreeSurface(imageSurface);


			if( sdlTexture == NULL ) {
				printf( "Unable to load Paddle %s! SDL Error: %s\n", "laser.bmp", SDL_GetError() );
		
			}
		
			if( sdlTexture1 == NULL ) {
				printf( "Unable to load Ball %s! SDL Error: %s\n", "laser.bmp", SDL_GetError() );
		
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
						case SDLK_LEFT:
							if(destination2.x < 0){
								break;
							}
							else{
								destination2.x -= 4;
							}
							break;
						case SDLK_RIGHT:
							if(destination2.x > 552){
								break;
							}
							destination2.x += 4;
							break;
						default:
							break;
					}

					

					

				}

				frameTime++;
      				if(FPS / frameTime == 4){
					frameTime = 0;
				}

				SDL_RenderClear(sdlRenderer);
				SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &destination1);
				SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &destination2);
				
				SDL_RenderPresent(sdlRenderer);
			}
			
			SDL_Quit();
		}

	}
}

