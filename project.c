/* <AI Hockey- A classic Pong game>
    Copyright (C) <2016>  <Amritraj Singh>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
 	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
    	but WITHOUT ANY WARRANTY; without even the implied warranty of
    	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    	GNU General Public License for more details.

    	You should have received a copy of the GNU General Public License
    	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Author Contact Info-
		Mail - amritrajck@gmail.com
		
*/


//Using SDL and standard IO
#include <stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include"SDL2/SDL_ttf.h" 
#include"SDL2/SDL_mixer.h" 

typedef uint32_t u32;

//Unifying the destination to be be pasted on, and co-ordinate velocities of the sticks and the ball in a structure
typedef struct object{
	SDL_Rect destination;
	int x_velocity;
	int y_velocity;
}object;

#define BOOL u32
#define TRUE 1
#define FALSE 0

#define true 1
#define false 0

//Screen dimension definitions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

//Global Variables
SDL_Renderer *sdlRenderer = NULL;
int setoption;
int player_score = 0, comp_score = 0;
SDL_Texture *playerMessage = NULL, *computerMessage = NULL;
SDL_Surface *comp_score_text = NULL, *player_score_text = NULL;
TTF_Font *font;
SDL_Color Neon = { 0, 255, 0, 0.7 };
char Pstr[15], Cstr[15];

Mix_Music *intro = NULL;


//Function Prototype declaration for control 
void ballcontrol();
void aicontol();
void moveball();
int getrandomnumber(int, int);
void ballresetc(object *p);//When Computer Scores
void ballresetp(object *p);//When Player Score
void speedmodifier(object *p);
void initialisepos(SDL_Rect *p, SDL_Rect *q, SDL_Rect *r, SDL_Rect *s, object *t, object *u, object *v);
BOOL collisioncheck1(); //checking collisions between ball and the player
BOOL collisioncheck2(); //checking collisions between ball and the Computer
BOOL collisioncheck3();
int showmenu(); //Function to show Menu


int getrandomnumber(int Min, int Max)
{
	int diff = Max-Min;
	return (int) (((double)(diff+1)/RAND_MAX) * rand() + Min);
}


void ballresetc( object *p){
	int x, y;
	p->destination.x = SCREEN_WIDTH / 2;
   	p->destination.y = SCREEN_HEIGHT / 2;
	
	x = getrandomnumber(-3, 4);
	y = getrandomnumber(0, 4);
	if( x != 0 && y != 0 ){
		p->x_velocity = x;
		p->y_velocity = y;
	}		
	else
		ballresetc( p );
	

}

void ballresetp( object *p){
	int x, y;
	p->destination.x = SCREEN_WIDTH / 2;
   	p->destination.y = SCREEN_HEIGHT / 2;
	
	x = getrandomnumber(-3, 4);
	y = getrandomnumber(-3, 0);
	if( x != 0 && y != 0 ){
		p->x_velocity = x;
		p->y_velocity = y;
	}		
	else
		ballresetp( p );
	

}



BOOL collisioncheck1( object *p, object *q ) {
 
	int ball_x = p->destination.x;
        int ball_y = p->destination.y;
        int ball_width = p->destination.w;
        int ball_height = p->destination.h;
        int ball_speed = p->y_velocity;

        int stick_x = q->destination.x;
        int stick_y = q->destination.y; 
        int stick_width = q->destination.w;
        int stick_height = q->destination.h;

	if ( (ball_speed > 0) && (ball_y + ball_height >= stick_y) && (ball_y + ball_height <= stick_y + stick_height) ) {
		if ( (ball_x <= stick_x + stick_width) && (ball_x + ball_width >= stick_x) ) {
                	return true;
		}
        }
	return false;
}


BOOL collisioncheck2( object *p, object *q ) {
 
	int ball_x = p->destination.x;
        int ball_y = p->destination.y;
        int ball_width = p->destination.w;
        int ball_speed = p->y_velocity;

        int ai_x = q->destination.x;
        int ai_y = q->destination.y; 
        int ai_width = q->destination.w;
        int ai_height = q->destination.h;

	if ( (ball_speed < 0) && (ball_y >= ai_y) && (ball_y <= ai_y + ai_height) ) {
		if ( (ball_x <= ai_x + ai_width) && (ball_x + ball_width >= ai_x) ) {
                	return true;
		}
        }
	return false;
}


BOOL collisioncheck3( object *p, SDL_Rect *q ) {
 
	int ball_x = p->destination.x;
        int ball_y = p->destination.y;
        int ball_width = p->destination.w;
        int ball_speed = p->y_velocity;

        int ai_x = q->x;
        int ai_y = q->y; 
        int ai_width = q->w;
        int ai_height = q->h;

	if ( (ball_speed < 0) && (ball_y >= ai_y) && (ball_y <= ai_y + ai_height) ) {
		if ( (ball_x <= ai_x + ai_width) && (ball_x + ball_width >= ai_x) ) {
                	return true;
		}
        }
	return false;
}

//Menu Function
int showmenu(){
	
	int x, y;
	const int NUMMENU = 5;
	char* label[NUMMENU] ;
	label[0] = "RESUME / PLAY" ;
	label[1] = "SINGLE PLAYER" ;
	label[2] = "MULTIPLAYER" ;
	label[3] = "WALL MODE" ;
	label[4] = "QUIT" ;
	SDL_Surface* menu[NUMMENU];
	SDL_Texture* menuTexture[NUMMENU];
	BOOL selected[NUMMENU];
	selected[0] = 0;
	selected[1] = 0; 
	selected[2] = 0;
	selected[3] = 0; 
	selected[4] = 0; 
	SDL_Color one = { 255, 255, 255, 1 }; 
	SDL_Color two = { 255, 0, 0, 1 }; 

	intro = Mix_LoadMUS( "intro.mp3" );
	Mix_PlayMusic( intro, -0 ); 

	TTF_Font *fontP;
	fontP = TTF_OpenFont("Pacifico.ttf", 80);
	if( font == NULL ){ 
		printf("Failed to load font. SDL_TTF Error : %s\n", TTF_GetError());
	
	}
	
	SDL_Surface* menuback;
	SDL_Texture* menubackground;

	menuback = IMG_Load ("pongmenu.jpg");
	if(menuback == NULL)
		printf("Can't Load Surface from pongmenu.jpg . Error: %s\n", SDL_GetError());
		

  	menubackground = SDL_CreateTextureFromSurface(sdlRenderer, menuback);
	if(menubackground == NULL)
		printf("Can't Load Texture from pongmenu.jpg . Error: %s\n", SDL_GetError());
		
  	
	SDL_FreeSurface(menuback);

	menu[0] = TTF_RenderText_Blended(fontP, label[0], one);
	if( menu [0] == NULL){
		printf("Menu Option 1 Can't be Loaded. Error in Rendering Text : %s\n", TTF_GetError());
		
	}
	menuTexture[0] = SDL_CreateTextureFromSurface(sdlRenderer, menu[0]);
	if(menuTexture[0] == NULL){
		printf("Couldn't Create Texture from Menu Surface, Option 1. ERROR : %s\n", SDL_GetError());
		
	}
	
	
	menu[1] = TTF_RenderText_Blended(fontP, label[1], one);
	if( menu [1] == NULL){
		printf("Menu Option 2 Can't be Loaded. Error in Rendering Text : %s\n", TTF_GetError());
		
	}
	menuTexture[1] = SDL_CreateTextureFromSurface(sdlRenderer, menu[1]);
	if(menuTexture[1] == NULL){
		printf("Couldn't Create Texture from Menu Surface, Option 2. ERROR : %s\n", SDL_GetError());
		
	}

	menu[2] = TTF_RenderText_Blended(fontP, label[2], one);
	if( menu[2] == NULL){
		printf("Menu Option 3 Can't be Loaded. Error in Rendering Text : %s\n", TTF_GetError());
		
	}
	menuTexture[2] = SDL_CreateTextureFromSurface(sdlRenderer, menu[2]);
	if(menuTexture[2] == NULL){
		printf("Couldn't Create Texture from Menu Surface, Option 2. ERROR : %s\n", SDL_GetError());
		
	}
	
	menu[3] = TTF_RenderText_Blended(fontP, label[3], one);
	if( menu [3] == NULL){
		printf("Menu Option 1 Can't be Loaded. Error in Rendering Text : %s\n", TTF_GetError());
		
	}
	menuTexture[3] = SDL_CreateTextureFromSurface(sdlRenderer, menu[3]);
	if(menuTexture[3] == NULL){
		printf("Couldn't Create Texture from Menu Surface, Option 1. ERROR : %s\n", SDL_GetError());
		
	}

	menu[4] = TTF_RenderText_Blended(fontP, label[4], one);
	if( menu [4] == NULL){
		printf("Menu Option 1 Can't be Loaded. Error in Rendering Text : %s\n", TTF_GetError());
		
	}
	menuTexture[4] = SDL_CreateTextureFromSurface(sdlRenderer, menu[4]);
	if(menuTexture[4] == NULL){
		printf("Couldn't Create Texture from Menu Surface, Option 1. ERROR : %s\n", SDL_GetError());
		
	}


	SDL_Rect pos[NUMMENU];

	pos[0].x = 300;
	pos[0].y = 100;
	pos[0].w = 150;
	pos[0].h = 30;

	pos[1].x = 300;
	pos[1].y = 150;
	pos[1].w = 150;
	pos[1].h = 30;

	pos[2].x = 300;
	pos[2].y = 200;
	pos[2].w = 150;
	pos[2].h = 30;

	pos[3].x = 300;
	pos[3].y = 250;
	pos[3].w = 150;
	pos[3].h = 30;

	pos[4].x = 300;
	pos[4].y = 300;
	pos[4].w = 100;
	pos[4].h = 30;


	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); 
	SDL_RenderSetLogicalSize(sdlRenderer, 640, 480);

	SDL_Event event;
	while(1){

		
		while(SDL_PollEvent(&event)){
			switch(event.type){

				case SDL_QUIT:
					return 1;
					break;

				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;
					for(int  i = 0; i < NUMMENU; i++){
						if(x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h){
							if(!selected[i]){
								selected[i] = 1;
								menu[i] = TTF_RenderText_Solid(fontP, label[i], two);
								menuTexture[i] = SDL_CreateTextureFromSurface(sdlRenderer, menu[i]);
								SDL_FreeSurface(menu[i]);
							}
						}
						else{
							
							selected[i] = 0;
							menu[i] = TTF_RenderText_Solid(fontP, label[i], one);
							menuTexture[i] = SDL_CreateTextureFromSurface(sdlRenderer, menu[i]);
							SDL_FreeSurface(menu[i]);
							
 						}
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					x = event.button.x;
					y = event.button.y;
					for(int i = 0; i < NUMMENU; i++){
   						if(x >= pos[0].x && x <= pos[0].x + pos[0].w && y >= pos[0].y && y <= pos[0].y + pos[0].h){			
								return 0;
						}
						if(x >= pos[1].x && x <= pos[1].x + pos[1].w && y >= pos[1].y && y <= pos[1].y + pos[1].h){
								if(setoption != 0){
									comp_score = 0;
									player_score = 0;
									sprintf(Cstr, "%d", comp_score);
									//printf("%s\n", Cstr);
									comp_score_text = TTF_RenderText_Solid(fontP, Cstr, Neon);
									computerMessage = SDL_CreateTextureFromSurface(sdlRenderer, comp_score_text);
									SDL_FreeSurface(comp_score_text);
									sprintf(Pstr, "%d", player_score);
									//printf("%s\n", Pstr);
									player_score_text = TTF_RenderText_Solid(fontP, Cstr, Neon);
									playerMessage = SDL_CreateTextureFromSurface(sdlRenderer, player_score_text);
									SDL_FreeSurface(player_score_text);

									
								}
								setoption = 0;			
								return 0;
						}
						
						if(x >= pos[2].x && x <= pos[2].x + pos[2].w && y >= pos[2].y && y <= pos[2].y + pos[2].h){	
								if(setoption != 2){
									comp_score = 0;
									player_score = 0;
									sprintf(Cstr, "%d", comp_score);
									//printf("%s\n", Cstr);
									comp_score_text = TTF_RenderText_Solid(fontP, Cstr, Neon);
									computerMessage = SDL_CreateTextureFromSurface(sdlRenderer, comp_score_text);
									SDL_FreeSurface(comp_score_text);
									sprintf(Pstr, "%d", player_score);
									//printf("%s\n", Pstr);
									player_score_text = TTF_RenderText_Solid(fontP, Cstr, Neon);
									playerMessage = SDL_CreateTextureFromSurface(sdlRenderer, player_score_text);
									SDL_FreeSurface(player_score_text);

								}		
								setoption = 2;
								return 0;
								break;
						}
						if(x >= pos[3].x && x <= pos[3].x + pos[3].w && y >= pos[3].y && y <= pos[3].y + pos[3].h){			
								if(setoption != 3){
									comp_score = 0;
									player_score = 0;
									sprintf(Cstr, "%d", comp_score);
									//printf("%s\n", Cstr);
									comp_score_text = TTF_RenderText_Solid(font, Cstr, Neon);
									computerMessage = SDL_CreateTextureFromSurface(sdlRenderer, comp_score_text);
									SDL_FreeSurface(comp_score_text);
									sprintf(Pstr, "%d", player_score);
									//printf("%s\n", Pstr);
									player_score_text = TTF_RenderText_Solid(font, Cstr, Neon);
									playerMessage = SDL_CreateTextureFromSurface(sdlRenderer, player_score_text);
									SDL_FreeSurface(player_score_text);

								}
								setoption = 3;
								return 0;
								break;
						}
						if(x >= pos[4].x && x <= pos[4].x + pos[4].w && y >= pos[4].y && y <= pos[4].y + pos[4].h){			
								return 1;
						}

					}					
					break;

				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE){
						return 1;
					}
					break;

			}

		}
		
		SDL_RenderClear(sdlRenderer);
		SDL_RenderCopy(sdlRenderer, menubackground, NULL, NULL);
		SDL_RenderCopy(sdlRenderer, menuTexture[0], NULL, &pos[0]);
		SDL_RenderCopy(sdlRenderer, menuTexture[1], NULL, &pos[1]);
		SDL_RenderCopy(sdlRenderer, menuTexture[2], NULL, &pos[2]);
		SDL_RenderCopy(sdlRenderer, menuTexture[3], NULL, &pos[3]);
		SDL_RenderCopy(sdlRenderer, menuTexture[4], NULL, &pos[4]);
				
		SDL_RenderPresent(sdlRenderer);

	
	}


}

void ballspeedmodifier( object *p){
		p->x_velocity = 1.5 * p->x_velocity;
		p->y_velocity = 2 * p->y_velocity;
		
}

void initialisepos(SDL_Rect *p, SDL_Rect *q, SDL_Rect *r, SDL_Rect *s, object *t, object *u, object *v){

	p->x = 350;
	p->y = 0;
	p->w = 30; 
	p->h = 20; 

	q->x = 350;
	q->y = 460; 
	q->w = 30; 
	q->h = 20; 

	r->x = 290;
	r->y = 0; 
	r->w = 50;
	r->h = 20;
	
	s->x = 290;
	s->y = 460; 
	s->w = 50; 
	s->h = 20; 

   	t->destination.x = 270;
   	t->destination.y = 30;
   	t->destination.w = 90;
   	t->destination.h = 30;

   	u->destination.x = 270;
   	u->destination.y = 420;
   	u->destination.w = 90;
   	u->destination.h = 30;

   	v->destination.x = 270;
   	v->destination.y = 200;
   	v->destination.w = 20;
   	v->destination.h = 20;

}

int main( int argc, char* args[] ) {

	SDL_Window *win;
	object ai_stick, player_stick, ball;	
	int count = 0, count1 = 0;
	
	SDL_Texture *sdlTexture = NULL, *sdlTexture1 = NULL;
	SDL_Rect CMessage_rect, PMessage_rect, ComputerMessage_rect, PlayerMessage_rect, display_rect, destination1;
	SDL_Surface *imageSurface = NULL;
	SDL_Texture *display = NULL;

	display_rect.x = 280;
	display_rect.y = SCREEN_HEIGHT / 2;
	display_rect.w = 100; 
	display_rect.h = 100; 
	
	destination1.x = -2;
	destination1.y = 30;
	destination1.w = SCREEN_WIDTH + 2;
	destination1.h = 30;

	SDL_Texture *background = NULL;
	//The window we'll be rendering to
	win = NULL;

	ballresetp( &ball );
	
	srand(time(NULL));//Seeding The Geneartor
	
	initialisepos( &CMessage_rect, &PMessage_rect, &ComputerMessage_rect, &PlayerMessage_rect, &ai_stick, &player_stick, &ball );	

	//Initialize TTF
	if( TTF_Init() < 0){
		printf( "Error Initialising Text:\n ");
	}
	
	//Font Loading
	font = TTF_OpenFont("times.ttf", 80);
	if( font == NULL ){ 
		printf("Failed to load font. SDL_TTF Error : %s\n", TTF_GetError());
	}

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO );
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 || SDL_INIT_AUDIO < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
		printf( "Error Initialising Audio: %s\n ", SDL_GetError() );
	}
	
	else { 
		//Create window
		win = SDL_CreateWindow("AI Hockey",
                             		SDL_WINDOWPOS_UNDEFINED,
                             		SDL_WINDOWPOS_UNDEFINED,
                             		640, 480,
                             		SDL_WINDOW_RESIZABLE);
		//Create Renderer
		sdlRenderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		//Load Music
		Mix_Chunk *soundEffect = Mix_LoadWAV("coll.wav");
		Mix_Chunk *wallhit = Mix_LoadWAV("wallhit.wav");
		Mix_Chunk *ballmiss = Mix_LoadWAV("ballmiss.wav");
		
			
		//Display Menu
		int i = showmenu();
		
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
			
			imageSurface = IMG_Load ("pong.jpg");
			if( imageSurface == NULL){
				printf( "Unable to load Background Image Into Surface. %s! SDL Error: %s\n", "pong.jpg", SDL_GetError() );
				
			}
  			background = SDL_CreateTextureFromSurface(sdlRenderer, imageSurface);
  			SDL_FreeSurface(imageSurface);
		
			if( background == NULL ) {
				printf( "Unable to load Background Surface Into Texture. %s! SDL Error: %s\n", "pong.jpg", SDL_GetError() );
			
			}

			//Load Spash Image 
			imageSurface = SDL_LoadBMP( "laser.bmp" );
			SDL_SetColorKey( imageSurface, SDL_TRUE, SDL_MapRGB(imageSurface->format, 255, 255, 255) );
			sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, imageSurface);
			SDL_FreeSurface(imageSurface);

			imageSurface = SDL_LoadBMP( "ball.bmp" );
			SDL_SetColorKey( imageSurface, SDL_TRUE, SDL_MapRGB(imageSurface->format, 255, 255, 255) );
			sdlTexture1 = SDL_CreateTextureFromSurface(sdlRenderer, imageSurface);
			SDL_FreeSurface(imageSurface);

			if( sdlTexture == NULL || sdlTexture1 == NULL ) {
				printf( "Unable to load Paddle %s! SDL Error: %s\n", "laser.bmp", SDL_GetError() );
					
			}
		
			TTF_WasInit();						
			 	
			SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, "Computer", Neon);
			SDL_Surface* surfaceMessage1 = TTF_RenderText_Blended(font, "Player", Neon);
			if( surfaceMessage == NULL || surfaceMessage1 == NULL ){
				printf("Couldn't Load Font In the Texture. TTF Error : %s\n", TTF_GetError());
				
			}
			SDL_Texture *Message = SDL_CreateTextureFromSurface(sdlRenderer, surfaceMessage);
			SDL_Texture *Message1 = SDL_CreateTextureFromSurface(sdlRenderer, surfaceMessage1);
			if (Message == NULL || Message1 == NULL){
				printf("Couldn't create Texture From Font Surface\n");
				
			}
			SDL_FreeSurface(surfaceMessage);
			SDL_FreeSurface(surfaceMessage1);

						
			sprintf(Cstr, "%d", comp_score);
			if( Cstr == NULL ){
				
			}
			else{
				comp_score_text = TTF_RenderText_Solid(font, Cstr, Neon);
				if(comp_score_text == NULL){
					printf("Couldn't Load Font In the Texture. TTF Error : %s\n", TTF_GetError());
					
				}
				else{					
					computerMessage = SDL_CreateTextureFromSurface(sdlRenderer, comp_score_text);
					if (computerMessage == NULL){
						printf("Couldn't create Texture From Font Surface\n");
						
					}
					else{					
						SDL_FreeSurface(comp_score_text);
								
					}
				}
					
			}

			sprintf(Pstr, "%d", player_score);
			if( Pstr == NULL ){
						
			}
			else{					
				player_score_text = TTF_RenderText_Solid(font, Pstr, Neon);
				if(player_score_text == NULL){
					printf("Couldn't Load Font In the Texture. TTF Error : %s\n", TTF_GetError());
					
				}
				else{					
					playerMessage = SDL_CreateTextureFromSurface(sdlRenderer, player_score_text);
					if (playerMessage == NULL){
						printf("Couldn't create Texture From Font Surface\n");
						
					}
					else{
						SDL_FreeSurface(player_score_text);
								
					}
				}
			}

				
			//Game Running Loop
			BOOL done = FALSE;
		
			if( i == 1){
				done = TRUE;
			}
		
			while(!done){
				SDL_Event event;
				if( player_score != 10 && comp_score != 10 ){
					ball.destination.x += ball.x_velocity;	
					ball.destination.y += ball.y_velocity;
				}
				if( setoption == 3){
					ball.destination.x += ball.x_velocity;	
					ball.destination.y += ball.y_velocity;
				}
					
				if( setoption != 3){
					if( player_score == 10 && comp_score == 10){

					}
					else{
						if( player_score == 10 ){
							//SDL_DestroyTexture(sdlTexture1);
							ball.destination.x += 0;	
							ball.destination.y += 0;
							imageSurface = TTF_RenderText_Solid(font, "YOU WIN !", Neon);
							if(imageSurface == NULL){
								printf("Couldn't Load Font In the Texture. TTF Error : %s\n", TTF_GetError());
						
							}
							else{					
								display = SDL_CreateTextureFromSurface(sdlRenderer, imageSurface);
								if (display == NULL){
									printf("Couldn't create Texture From Font Surface\n");
							
								}
								else{					
									SDL_FreeSurface( imageSurface );
									
		
								}
							}
						}
						else if ( comp_score == 10 ){
							//SDL_DestroyTexture(sdlTexture1);
							ball.destination.x += 0;	
							ball.destination.y += 0;		
							imageSurface = TTF_RenderText_Solid(font, "YOU LOSE !", Neon);
							if(imageSurface == NULL){
								printf("Couldn't Load Font In the Texture. TTF Error : %s\n", TTF_GetError());
						
							}
							else{					
								display = SDL_CreateTextureFromSurface(sdlRenderer, imageSurface);
								if (display == NULL){
									printf("Couldn't create Texture From Font Surface\n");
							
								}
								else{					
									SDL_FreeSurface( imageSurface );
										
								}
							}
	
						}
					}

				}
				
				
				
				//Collision of ball with Left Wall
				if(ball.destination.x < 0 + 2){
					ball.x_velocity = -ball.x_velocity;
					Mix_PlayChannel(-1, wallhit, 0);
					
				}
				
				//Collision of ball with Right Wall
				if(ball.destination.x + ball.destination.w > SCREEN_WIDTH - 2){
					ball.x_velocity = -ball.x_velocity;
					Mix_PlayChannel(-1, wallhit, 0);
					
				}

				//Resetting ball when AI misses the ball
				if(ball.destination.y < 15 ){
					ballresetp( &ball );
					player_score += 1;
					sprintf(Pstr, "%d", player_score);
					player_score_text = TTF_RenderText_Solid(font, Pstr, Neon);
					playerMessage = SDL_CreateTextureFromSurface(sdlRenderer, player_score_text);
					SDL_FreeSurface(player_score_text);
					
				}

				//Resetting ball when player misses the ball
				if(ball.destination.y + ball.destination.h > SCREEN_HEIGHT){
					ballresetc( &ball );
					comp_score += 1;
					sprintf(Cstr, "%d", comp_score);
					comp_score_text = TTF_RenderText_Solid(font, Cstr, Neon);
					computerMessage = SDL_CreateTextureFromSurface(sdlRenderer, comp_score_text);
					SDL_FreeSurface(comp_score_text);
					Mix_PlayChannel(-1, ballmiss, 0);
					
				}

				//Collisoncheck Player and Ball
				if(collisioncheck1( &ball, &player_stick ) == true){
					ball.y_velocity = -ball.y_velocity;
					count += 1;
					Mix_PlayChannel(-1, soundEffect, 0);
					
				}
	
				//Collisoncheck AI and Ball
				if(setoption == 3){	
					if(collisioncheck3( &ball, &destination1 ) == true){
						ball.y_velocity = -ball.y_velocity;
						count1 += 1;
						Mix_PlayChannel(-1, soundEffect, 0);
						player_score += 1;
						sprintf(Pstr, "%d", player_score);
						player_score_text = TTF_RenderText_Solid(font, Pstr, Neon);
						playerMessage = SDL_CreateTextureFromSurface(sdlRenderer, player_score_text);
						SDL_FreeSurface(player_score_text);

					}
				}
				else 
					if(collisioncheck2( &ball, &ai_stick) == true){
						ball.y_velocity = -ball.y_velocity;
						count1 += 1;
						Mix_PlayChannel(-1, soundEffect, 0);
					
					}

				if( count == 5 && count1 == 5){
					ballspeedmodifier( &ball );
					count = count1 = 0;
				}
				
				// AI Code, to move the ai pedal left
				if(setoption!=2 && setoption!=3){			
					if(ai_stick.destination.x + ai_stick.destination.w / 2 < ball.destination.x + ball.destination.w / 2 ){
						ai_stick.destination.x += 10;
					
					}
				}
			
				// AI Code, to move the ai pedal right
				if(setoption!=2 && setoption!=3){
					if(ai_stick.destination.x + ai_stick.destination.w / 2 > ball.destination.x + ball.destination.w / 2 ){
						ai_stick.destination.x -= 10;
					
					}
				}				
					
				//To limit the ai pedal within the screen, left bound
				if(ai_stick.destination.x < 2 ){
					ai_stick.destination.x = 2;

				}
				
				//To limit the ai pedal within the screen, right bound
				if(ai_stick.destination.x + ai_stick.destination.w > SCREEN_WIDTH - 2){
					ai_stick.destination.x = SCREEN_WIDTH - ai_stick.destination.w - 2 ;

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
							//done = TRUE;
							display = NULL;
							i = showmenu();
							if( i == 1){
								done = TRUE;
							}
							break;
						case SDLK_LEFT:
							if(player_stick.destination.x < 0 + 2/*(ForPrecision)*/){
								break;
							}
							else{
								player_stick.destination.x -= 10;
							}
							break;
						
						case SDLK_RIGHT:
							if(player_stick.destination.x + player_stick.destination.w  > SCREEN_WIDTH - 2 ) {
								break;
							}
							player_stick.destination.x += 10;
							break;

						case SDLK_a:
							if( setoption == 2 ){
							if(player_stick.destination.x < 0 + 2/*(ForPrecision)*/){
								break;
							}
							ai_stick.destination.x -= 10;
							}
							break;

						case SDLK_d:	
							if(setoption == 2){
							if(ai_stick.destination.x + ai_stick.destination.w > SCREEN_WIDTH - 2){
								break;
							}
							ai_stick.destination.x += 10;
							}
							break;

						default:
							break;
					}

						

				}

				SDL_RenderClear(sdlRenderer);
				SDL_RenderCopy(sdlRenderer, background, NULL, NULL);
				if(setoption == 3){
					SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &destination1);
				}
				else{
					SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &ai_stick.destination);
				}
				SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &player_stick.destination);
				SDL_RenderCopy(sdlRenderer, sdlTexture1, NULL, &ball.destination);
				SDL_RenderCopy(sdlRenderer, playerMessage, NULL, &PMessage_rect);//Actual Scores
				if ( setoption != 2 && setoption != 3 ){
					SDL_RenderCopy(sdlRenderer, computerMessage, NULL, &CMessage_rect);//Actual Scores
					SDL_RenderCopy(sdlRenderer, Message, NULL, &ComputerMessage_rect);//Territory
					SDL_RenderCopy(sdlRenderer, Message1, NULL, &PlayerMessage_rect);//Territory
				}
				if( setoption == 2 ){
					SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, "Player 2", Neon);
					SDL_Texture *Message = SDL_CreateTextureFromSurface(sdlRenderer, surfaceMessage);
					SDL_RenderCopy(sdlRenderer, computerMessage, NULL, &CMessage_rect);//Actual Scores
					SDL_RenderCopy(sdlRenderer, Message, NULL, &ComputerMessage_rect);//Territory
					SDL_FreeSurface(surfaceMessage);
					SDL_Surface* surfaceMessage1 = TTF_RenderText_Blended(font, "Player 1", Neon);
					SDL_Texture *Message1 = SDL_CreateTextureFromSurface(sdlRenderer, surfaceMessage);
					SDL_RenderCopy(sdlRenderer, playerMessage, NULL, &PMessage_rect);//Actual Scores
					SDL_RenderCopy(sdlRenderer, Message1, NULL, &PlayerMessage_rect);//Territory
					SDL_FreeSurface(surfaceMessage1);

				}
				
				SDL_RenderCopy(sdlRenderer, display, NULL, &display_rect);
				
				SDL_RenderPresent(sdlRenderer);
			}
			
			SDL_Quit();
		}

	}

}

