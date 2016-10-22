//Using SDL and standard IO
#include <stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL/SDL_image.h>
#include<time.h>
#include"SDL2/SDL_ttf.h"

#define BOOL u32
#define TRUE 1
#define FALSE 0

#define true 1
#define false 0

//Screen dimension definitions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

//Function Prototypes for control 
void ballcontrol();
void aicontol();
void moveball();
int getrandomnumber(int, int);
void ballreset();

//Function Prototypes for Score Management
void func_playerscore();
void func_aiscore();

typedef uint32_t u32;

void drawSprite(SDL_Surface* imageSurface, 
                SDL_Surface* screenSurface,
                int srcX, int srcY, 
                int dstX, int dstY,
                int width, int height);

//Unifying the destination to be be pasted on, and co-ordinate velocities of the sticks and the ball in a structure
typedef struct object{
	SDL_Rect destination;
	int x_velocity;
	int y_velocity;
}object;

object ai_stick, player_stick, ball;

//Taking Score as Global Variable
int player_score, ai_score;

TTF_Font *times;

SDL_Rect p_score, aig_score; 


int getrandomnumber(int Min, int Max)
{
    int diff = Max-Min;
    return (int) (((double)(diff+1)/RAND_MAX) * rand() + Min);
}


void ballreset(){
	ball.destination.x = SCREEN_WIDTH / 2;
   	ball.destination.y = SCREEN_HEIGHT / 2;
	ball.x_velocity = getrandomnumber(-3, 4);
	ball.y_velocity = getrandomnumber(-3, 4);

}



//checking collisions between ball and the palyer
BOOL collisioncheck1() {
 
	int ball_x = ball.destination.x;
        int ball_y = ball.destination.y;
        int ball_width = ball.destination.w;
        int ball_height = ball.destination.h;
        int ball_speed = ball.y_velocity;

        int stick_x = player_stick.destination.x;
        int stick_y = player_stick.destination.y; 
        int stick_width = player_stick.destination.w;
        int stick_height = player_stick.destination.h;

	if ( (ball_speed > 0) && (ball_y + ball_height >= stick_y) && (ball_y + ball_height <= stick_y + stick_height) ) {
		if ( (ball_x <= stick_x + stick_width) && (ball_x + ball_width >= stick_x) ) {
                	return true;
		}
		return false;
        }
}

BOOL collisioncheck2() {
 
	int ball_x = ball.destination.x;
        int ball_y = ball.destination.y;
        int ball_width = ball.destination.w;
        int ball_height = ball.destination.h;
        int ball_speed = ball.y_velocity;

        int ai_x = ai_stick.destination.x;
        int ai_y = ai_stick.destination.y; 
        int ai_width = ai_stick.destination.w;
        int ai_height = ai_stick.destination.h;

	if ( (ball_speed < 0) && (ball_y >= ai_y) && (ball_y <= ai_y + ai_height) ) {
		if ( (ball_x <= ai_x + ai_width) && (ball_x + ball_width >= ai_x) ) {
                	return true;
		}
		return false;
        }
}
   
   
int main( int argc, char* args[] ) {

	//The window we'll be rendering to
	SDL_Window* win = NULL;
	
	//The surface contained by the window
	SDL_Surface* ScreenSurface = NULL;

	SDL_Surface* imageSurface = NULL;

	SDL_Renderer *sdlRenderer;

   	// Part of the screen we want to draw the sprite to

   	//ai stick
   	ai_stick.destination.x = 270;
   	ai_stick.destination.y = 30;
   	ai_stick.destination.w = 90;
   	ai_stick.destination.h = 30;
	
	//player stick
   	player_stick.destination.x = 270;
   	player_stick.destination.y = 420;
   	player_stick.destination.w = 90;
   	player_stick.destination.h = 30;

	//ball
   	ball.destination.x = 270;
   	ball.destination.y = 200;
   	ball.destination.w = 20;
   	ball.destination.h = 20;

	//Playe Score zero initially
	int player_score = 0;
 	ai_score = 0;

	srand(time(NULL));

	ballreset();

	//times = TTF_OPENFONT("times.ttf", 14);


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

				ball.destination.x += ball.x_velocity;	
				ball.destination.y += ball.y_velocity;
				
				//Collision of ball with Left Wall
				if(ball.destination.x < 0 + 2){
					ball.x_velocity = -ball.x_velocity;
				}
				
				//Collision of ball with Right Wall
				if(ball.destination.x + ball.destination.w > SCREEN_WIDTH - 2){
					ball.x_velocity = -ball.x_velocity;
				}

				//Resetting ball when AI misses the ball
				if(ball.destination.y < 0 ){
					ballreset();
				}

				//Resetting ball when player misses the ball
				if(ball.destination.y + ball.destination.h > SCREEN_HEIGHT){
					ballreset();
				}
			
				//Collisoncheck Player and Ball
				if(collisioncheck1(/*player_stick.destination, ball.destination*/) == true){
					ball.y_velocity = -ball.y_velocity;
				}
	
				//Collisoncheck AI and Ball	
				if(collisioncheck2() == true){
					ball.y_velocity = -ball.y_velocity;
				}


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
							if(player_stick.destination.x < 0 + 2/*(ForPrecision)*/){
								break;
							}
							else{
								player_stick.destination.x -= 10;
							}
							break;
						case SDLK_r:
							ballreset();
							break;
						case SDLK_RIGHT:
							if(player_stick.destination.x + player_stick.destination.w  > SCREEN_WIDTH - 2/*(ForPrecision)*/){
								break;
							}
							player_stick.destination.x += 10;
							break;
						default:
							break;
					}

						

				}

				SDL_RenderClear(sdlRenderer);
				SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &ai_stick.destination);
				SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &player_stick.destination);
				SDL_RenderCopy(sdlRenderer, sdlTexture1, NULL, &ball.destination);
				
				SDL_RenderPresent(sdlRenderer);
			}
			
			SDL_Quit();
		}

	}
}

