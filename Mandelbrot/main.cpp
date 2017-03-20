#include "SDL.h"

struct color
{
	int b;
	int g;
	int r;
};
struct area
{
	double lgx;
	double lgy;
	double pdx;
	double pdy;
};

color ekran[500][500];
SDL_Surface *screen = NULL;
SDL_Surface *cursor = NULL;
Uint8 * keystate = SDL_GetKeyState( NULL );
int curX=250;
int curY=250;
double LGx=-2.5;
double LGy=2.5;
double PDx=2.5;
double PDy=-2.5;
double echoCurX=(LGx+PDx)/(double)2;
double echoCurY=(LGy+PDy)/(double)2;

int L=0;
area *reg = new area[L];

void Draw(SDL_Surface *screen, SDL_Surface *cursor)
{
	SDL_LockSurface(screen);
	for(int i=0;i<500;i++)
	{
		for(int j=0;j<500;j++)
		{
			((char *)screen->pixels)[screen->pitch * j + i * 4 + 0] = ekran[i][j].b;
            ((char *)screen->pixels)[screen->pitch * j + i * 4 + 1] = ekran[i][j].g;
            ((char *)screen->pixels)[screen->pitch * j + i * 4 + 2] = ekran[i][j].r;
	    }
	}
	SDL_UnlockSurface(screen);

	SDL_Rect dest;
	dest.x = curX-3;
	dest.y = curY-3;
	dest.w = 7;
	dest.h = 7;
	SDL_BlitSurface(cursor, NULL, screen, &dest);

    SDL_Flip( screen );
}

int Check (double x, double y)
{
	double X=0;
	double Y=0;
	double zx=0;
	double zy=0;
	int m=0;
	while( (((zx*zx)+(zy*zy))<4)  && (m<100) )
	{
		X=( (zx*zx)-(zy*zy) )+x;
		Y=( 2*zx*zy )+y;
		zx=X;
		zy=Y;
		m++;
	}
	return m;
}

void Set()
{
	double x=0;
	double y=0;
	for(int i=0;i<500;i++)
	{
		for(int j=0;j<500;j++)
		{
			x=LGx + i*( (PDx-LGx)/(double)500 );
			y=PDy + (500-j)*( (LGy-PDy)/(double)500 );
			int w=0;
			w=Check(x,y);
			ekran[i][j].r=w*2;

	    }
	}
}
void ZoomIn()
{
	double lgx, lgy, pdx, pdy;
	lgx=echoCurX - ( (PDx-LGx)/(double)4 );
	lgy=echoCurY + ( (LGy-PDy)/(double)4 );
    pdx=echoCurX + ( (PDx-LGx)/(double)4 );
	pdy=echoCurY - ( (LGy-PDy)/(double)4 );

	LGx=lgx;
	LGy=lgy;
	PDx=pdx;
	PDy=pdy;
	echoCurX=(LGx+PDx)/(double)2;
	echoCurY=(LGy+PDy)/(double)2;
	curX=250;
	curY=250;

	L++;
	reg[L].lgx=LGx;
	reg[L].lgy=LGy;
	reg[L].pdx=PDx;
	reg[L].pdy=PDy;
	
}
void ZoomOut()
{
	L--;
	LGx=reg[L].lgx;
	LGy=reg[L].lgy;
	PDx=reg[L].pdx;
	PDy=reg[L].pdy;

	echoCurX=(LGx+PDx)/(double)2;
	echoCurY=(LGy+PDy)/(double)2;
	curX=250;
	curY=250;
}

int main( int argc, char* args[] )
{
	SDL_Event action;
    bool quit = false;
	
	SDL_Init( SDL_INIT_EVERYTHING );
	screen = SDL_SetVideoMode( 500, 500, 32, SDL_SWSURFACE );
	SDL_WM_SetCaption( "Mandelbrot @Robert Knop", NULL );
	
	cursor = SDL_LoadBMP("kursor.bmp" );
	SDL_SetColorKey(cursor, SDL_SRCCOLORKEY, 0xFFFFFF);

	reg[0].lgx=-2.5;
	reg[0].lgy=2.5;
	reg[0].pdx=2.5;
	reg[0].pdy=-2.5;
	Set();
	while(!quit)
	{
	    //tu zaczynaja sie opcje pojedynczych klawiszy
		while( SDL_PollEvent(&action))
        {
            if( action.type == SDL_QUIT )
            {
                quit = true;
            }
			if( action.type == SDL_KEYDOWN )
			{
				switch ( action.key.keysym.sym )
				{
				case SDLK_ESCAPE:
					quit=true;
					break;
				case SDLK_z:
					ZoomIn();
					Set();
					break;
				case SDLK_x:
					ZoomOut();
					Set();
				}
			}
        }
		//tu koncza sie opcje pojedynczych klawiszy
		//tu zaczynaja sie opcje ciaglych klawiszy
		if( keystate[ SDLK_RIGHT ] )
		{
			curX++;
			echoCurX = echoCurX + ( (PDx-LGx)/(double)500 );
		}
		if( keystate[ SDLK_LEFT ] )
		{
			curX--;
			echoCurX = echoCurX - ( (PDx-LGx)/(double)500 );
			
		}
		if( keystate[ SDLK_UP ] )
		{
			curY--;
			echoCurY = echoCurY + ( (LGy-PDy)/(double)500 );
			
		}
		if( keystate[ SDLK_DOWN ] )
		{
			curY++;
			echoCurY = echoCurY - ( (LGy-PDy)/(double)500 );
			
		}
		//tu koncza sie opcje ciaglych klawiszy
		Draw(screen, cursor);
	}
	
	SDL_FreeSurface(screen);
	SDL_FreeSurface(cursor);
    SDL_Quit();

	delete [] reg;
	reg = 0;
	return 0;
}