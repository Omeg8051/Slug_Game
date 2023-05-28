/*
This is a stupid project about a stupid slug
That follows where you click. and will go
idle when reaching the target or run out of 
patientce.

left click to set target fot the slug.
right click to select a slug.
middle click to spawn a new slug.
click mouse 4 and drag to create new plaform

*/

#include <SDL2/SDL.h>
//#include <SDL2/SDL_vulkan.h>
#include <time.h>
#include <list>
#include <math.h>
#include <iostream>

#define CHK_TH 5
#define IDL_SPD_DIV 3
#define SEL_TH 10

using namespace std;

SDL_Window* win;
SDL_Renderer* rd;

unsigned long evecount = 0;

class Thingy{
	public:

	void set_pos(int x,int y){
		lx = x;
		ly = y;
	}

	void runFrame(){
		//cout  <<"here\n";
		ly+=jcnt?-5:fall;
		jcnt -= jcnt!=0;

		if(state == 0){
			pace = 0;
			if(!nxt_mov){
				if((rand()%7) < 3){
					nxt_mov = 300 + (rand() % 200);
					state = 2;
					return;
				} 
				tx += (50 + (rand() % 100)) * ((rand()%2)?1:-1);
				tx += tx>640?-640:tx<0?640:0;
				ty = ly - (rand() % 300) + 150;
				ty += ty>474?-150:tx<0?150:0;
				nxt_mov = 200 + (rand() % 400);
				state = 1;
				patience = 5;
			} else {
				nxt_mov--;
			}
		}else if(state == 1){
			fo_x = tx<lx?-2:2;
			fo_y = -1;
			lx += (abs(lx - tx) > CHK_TH)*(tx<lx?-1:1)*(pace || !(++stsub%IDL_SPD_DIV));
			face = (tx>lx);

			if(abs(lx - tx) < abs(ly - ty) && (ly - ty) >= 4 && !fall && patience && !jcnt){
				jcnt = abs(ly - ty) / 5 + 5;
				jcnt = jcnt > 15?15:jcnt;
				patience--;
				if (!patience)state = 0;
			}

			if(ly < ty && abs(lx - tx) < abs(ly - ty) && !fall){
				ly++;
			}

			if(abs(lx - tx) <= CHK_TH && abs(ly - ty) <= CHK_TH){
				state = 0;
			} 
			
		} else if(state = 2){
			if(!(--nxt_mov)){
				state = 0;
			}
			
		}
		
		if(state != 1) {
			fo_x = 0;
			fo_y = 0;
			
		}

	}

	bool check_fall(list<SDL_Rect*> *lp){
		SDL_Rect slf;
		slf.x = lx-8;
		slf.y = ly-4;
		slf.w = 17;
		slf.h = 9;

		SDL_Rect it;

		if(ly > 474){
			fall = 0;
			return 0;
			}

		for(SDL_Rect* l : *lp){
			if(l->y < (ly+4))continue;
			
			if(SDL_IntersectRect(l,&slf,&it)){
				fall = 0;
				return 0;
			}
			
		}
			
		fall = 1;
		return 1;
	}

	void setState(int s){
		state = s;
		nxt_mov = 300;
	}

	void setTarget(int x, int y){
		tx = x;
		ty = y;
		state = 1;
		pace = 1;
		patience = 10;
	}

	void render_Creature(SDL_Renderer* rd){
		SDL_SetRenderDrawColor(rd,R,G,B,255);
//draw body
	if(face){
		for(int j = -15;j<=6;j++){
			SDL_RenderDrawLine(rd,
			(lx+j),(ly-((j<-10)?-1:(j<-8)?1:(j<-5)?2:(j<-4)?3:(j<-3)?4:(j<1)?5:(j<5)?4:2)),
			(lx+j),(ly+((j<-13)?3:(j<-3)?4:(j<3)?4:(j<5)?4:2)));
		}
	} else {
		for(int j = 15;j>=-6;j--){
			SDL_RenderDrawLine(rd,
			(lx+j),(ly-((j>10)?-1:(j>8)?1:(j>5)?2:(j>4)?3:(j>3)?4:(j>-1)?5:(j>-5)?4:2)),
			(lx+j),(ly+((j>13)?3:(j>3)?4:(j>-3)?4:(j>-5)?4:2)));
		}
	}
		
//draw antina
		
	SDL_RenderDrawLine(rd,lx,ly,lx-3,ly-9);
	SDL_RenderDrawLine(rd,lx,ly,lx+3,ly-9);
				
//draw this if selected
	if(selected){
		SDL_RenderDrawLine(rd,lx,ly-10,lx,ly-20);
		SDL_RenderDrawLine(rd,lx,ly-10,lx-5,ly-13);
		SDL_RenderDrawLine(rd,lx,ly-10,lx+5,ly-13);
	}
		


//draw that dumb little face
		SDL_SetRenderDrawColor(rd,0,0,0,255);
		if(state == 2){
			SDL_RenderDrawLine(rd,(lx-4),(ly-1),(lx-2),(ly-1));
			SDL_RenderDrawLine(rd,(lx+4),(ly-1),(lx+2),(ly-1));
		} else {
			for(int j = -1;j<=1;j++){
				SDL_RenderDrawLine(rd,(lx-3+fo_x+j),(ly-2),(lx-3+fo_x+j),(ly+1));
				SDL_RenderDrawLine(rd,(lx+3+fo_x+j),(ly-2),(lx+3+fo_x+j),(ly+1));
			}
		}
		
		SDL_RenderDrawLine(rd,(lx-2+fo_x),(ly+3),(lx+2+fo_x),(ly+3));

//draw sweat when running	
		if(pace){
			SDL_SetRenderDrawColor(rd,150, 200, 255,255);
			SDL_RenderDrawLine(rd,lx+(face?-4:4),ly-8,lx+(face?-5:5),ly-9);
			SDL_RenderDrawLine(rd,lx+(face?-7:7),ly-8,lx+(face?-9:9),ly-9);
		
		}


	}

	void init(){
		fo_x = 0;fo_y = 0;
		tx=0; ty=0;
		pace = 0;
		R = 100+(rand()%126);
		G = 100+(rand()%126);
		B = 100+(rand()%126);
		jcnt = 0 ;
		patience = 0;
		stsub = 0;
		nxt_mov = 0;
		state = 0;
		selected = 0;
	}
	void select(bool s){
		selected = s;
	}

	bool near(int x, int y){
		return (abs(x-lx)< SEL_TH && abs(y-ly) < SEL_TH);
	}
	private:
	int fo_x = 0,fo_y = 0;
	int lx = 100,ly = 100;
	int tx,ty;
	unsigned int state;
	unsigned char R = 255,G = 255,B = 255;
	unsigned char jcnt = 0 ;
	bool face = 0;
	char patience = 0;
	unsigned int nxt_mov = 0;
	bool pace = 0;
	unsigned char stsub = 0;
	bool selected = 0, fall= 0;
	//unsigned int state;
	
	/*
	0 idle
	1 follow
	2 sleep
	*/

};

Thingy* slug;

std::list<Thingy*> jar;

std::list<SDL_Rect*> plt;

void run_all_Thingy(list<Thingy*>* chain){
	
	for (Thingy* ct : *chain)ct->runFrame();
}

void render_all_Thingy(list<Thingy*>* chain,SDL_Renderer* rd){
	for (Thingy* ct : *chain)ct->render_Creature(rd);
}

void chk_fall_all_Thingy(list<Thingy*>* chain,list<SDL_Rect*>* plat){
	for (Thingy* ct : *chain)ct->check_fall(plat);
}


Thingy* add_Thingy(list<Thingy*>* chain,int x,int y){
	Thingy* newSlug = (Thingy*)malloc(sizeof(Thingy));
	newSlug->init();
	newSlug->set_pos(x,y);
	chain->push_front(newSlug);
	return newSlug;
}

Thingy* select_new_thingy(Thingy** ct,int x,int y,list<Thingy*> *chain){
	Thingy* tt = NULL;
	for (Thingy* cs : *chain){
		if(cs->near(x,y)){
			//for (Thingy* cl : *chain)cl->select(0);
			(*ct)->select(0);
			(*ct) = cs;
			(*ct)->select(1);
			break;
		}
	}
	
	return tt;
}

void add_plat(list<SDL_Rect*> *plt,int x,int y,int w, int h){

	SDL_Rect* newPlt = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	newPlt->x = x;
	newPlt->y = y;
	newPlt->w = w;
	newPlt->h = h;
	
	plt->push_front(newPlt);
	return;
}

void render_all_Plat(list<SDL_Rect*>* plt,SDL_Renderer* rd){
	SDL_SetRenderDrawColor(rd,150,150,150,255);

	for (SDL_Rect* pl : *plt)SDL_RenderDrawRect(rd,pl);
}


int main(int argc, char* argv[]){
	srand(time(NULL));
	
	slug = add_Thingy(&jar,100,100);

	add_plat(&plt,0,0,100,25);

	cout << jar.size() << endl;
	slug->select(1);

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
		cout << "oof\n";
	} else {
		cout << "=============================\n|                           |\n|         Slug Game         |\n|                           |\n=============================\nleft click to set target fot the slug.\nright click to select a slug.\nmiddle click to add another slug\nclick mouse 4 and drag to create new plaform\n";
	}
	
	//cout << "vulkan status: " <<SDL_Vulkan_LoadLibrary(NULL) << endl;

	win = SDL_CreateWindow(
		"Slug game",
		860,
		300,
		640,
		480,
		SDL_WINDOW_SHOWN
		);

	rd = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	if(rd == NULL){

		cout << "RD som tin wong" << endl;
		
		return -20;
	}
	//SDL_SetRenderTarget(rd, NULL);

	
	bool g_run = 1;
	bool bd = false;

	int x = 0,y = 0,st = 0;

	int psx,psy,pex,pey;
	bool mkplt = 0;
	SDL_Rect nplt;
	while (g_run){
		SDL_Event eve;
	
		while(SDL_PollEvent(&eve)){
			if(eve.type == SDL_QUIT){
				g_run = 0;
			} else if(eve.type == SDL_MOUSEBUTTONDOWN){
				if((int)eve.button.button == 1){
					x = eve.button.x;
					y = eve.button.y;
					slug->setTarget(x ,y);
				}else if((int)eve.button.button == 3){
					//slug->select(0);
					select_new_thingy(&slug,eve.button.x,eve.button.y,&jar);
				}else if((int)eve.button.button == 2){
					//cout << "add Thingy\n";
					add_Thingy(&jar,eve.button.x,eve.button.y);
				}else if((int)eve.button.button == 5){
					mkplt = 1;
					psx = eve.button.x;
					psy = eve.button.y;
					
				}
				
				//cout <set_pos< "mouse butten " << (int)eve.button.button << " pressed\n";
			} else if(eve.type == SDL_MOUSEBUTTONUP){
				if((int)eve.button.button == 5){
					if(mkplt){
						mkplt = 0;
						add_plat(&plt,nplt.x,nplt.y,nplt.w,nplt.h);
					}
				}
				//cout << "mouse butten " << (int)eve.button.button << " released\n";
			}
			
			if(eve.type == SDL_MOUSEMOTION && mkplt){
				pex = eve.button.x;
				pey = eve.button.y;
				
				nplt.x = psx<pex?psx:pex;
				nplt.y = psy<pey?psy:pey;

				nplt.w = abs(psx - pex);
				nplt.h = abs(psy - pey);
							
			}


		}
		/*
		runframe
		*/
		chk_fall_all_Thingy(&jar,&plt);
		run_all_Thingy(&jar);
			
		
		/*
		render begin

		*/


		SDL_RenderClear(rd);
		SDL_SetRenderDrawColor(rd,0, 0, 0,255);

		SDL_RenderFillRect(rd,NULL);
		
		if(mkplt){
			SDL_SetRenderDrawColor(rd,100, 255, 50,255);
			SDL_RenderFillRect(rd,&nplt);
		

		}
		
		
		SDL_SetRenderDrawColor(rd,255, 255, 255,255);

		SDL_RenderDrawLine(rd,0,0,eve.button.x,eve.button.y);
		//slug->runFrame();
		SDL_SetRenderDrawColor(rd,255, 0, 255,255);

		SDL_RenderDrawPoint(rd,x,y);
				
		render_all_Plat(&plt,rd);
				
		//slug.render_Creature(rd);
		render_all_Thingy(&jar,rd);

		SDL_RenderPresent(rd);
	//render end
		SDL_Delay(10);
	
	}
	SDL_DestroyWindow(win);
	for(Thingy* ct: jar){free(ct);}
SDL_Quit();
return 0;
}
