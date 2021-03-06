#include <allegro.h>

// Funções
void sair();
void menu();
void pause();
void control(); 
void mapa();
void blocos();
int colidir(int Ax, int Ay, int Bx, int By, int Aw, int Ah, int Bw, int Bh);
void end();

// Objectos
struct object{
	int wx, wy, x, y, w, h;
};

struct object 
	p = {450, 200, 50, 50, 50, 50},
	//p = {34, 128, 500, 50, 28, 34},
	bloco[13][15]
;

// Variáveis Globais
int sai = 0;
int start = 0;
int pausa = 0;
int lost = 0;
int dir = 0;
int vly = 0;
int vup = 15;
int grv = 4;
int out = -500;
int width = 900;
int height = 600;
int nTile=0;
int caindo = 1;
int pulando = 0;
int pLimit = 0;
char mp[13][15];

BITMAP *buffer, *fundoMenu, *jogador, *piso;
SAMPLE *soundIntro, *soundPause, *soundJump, *soundEat, *soundDeath;

int main() {
	
	// Inicialização
	allegro_init();
	install_keyboard();
	set_color_depth(32);
	set_window_title("AAAHHHRRR - HomeAway /  +2.5");
	set_close_button_callback(sair);
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
	
	int i, j;
	// Variáveis Locais
	buffer = create_bitmap(width, height);
	fundoMenu = load_bitmap("./assets/img/menu_back.bmp", NULL);
	jogador = load_bitmap("./assets/img/player/dragonball.bmp", NULL);
	piso = load_bitmap("./assets/img/grama.bmp", NULL);
	soundIntro = load_sample("./assets/sound/intro.wav");
	soundPause = load_sample("./assets/sound/pause.wav");
	soundJump = load_sample("./assets/sound/jump.wav");
	soundEat = load_sample("./assets/sound/eat.wav");
	soundDeath = load_sample("./assets/sound/death.wav");
	
	//Alimanhamento da matriz
	for(i=0; i<13; i++){
		for(j=0; j<15; j++){
			bloco[i][j].y = i*50;
			bloco[i][j].x = j*60;
			bloco[i][j].w = 60;
			bloco[i][j].h = 50;
			bloco[i][j].wy = 0;
		}
	}
	
	mapa();
	play_sample(soundIntro, 70, 128, 1000, 1);
	
	while (!(sai || key[KEY_ESC])) {
		
		// put your code here
		if(!start){
			menu();
		}
		
		// Perdeu
		end();
		if(lost){
			play_sample(soundDeath, 255, 128, 1000, 0);
		}
		
		// Pausa
		if(key[KEY_P] && !pausa){
			pausa = 1;
			//stop_sample(soundIntro);
			play_sample(soundPause, 255, 128, 1000, 0);
			textprintf_centre_ex(buffer, font, width/2, height/2, 0x000000, -1, "PAUSE");
		}
		
		control();
		blocos();
		/*for(i=0; i<13; i++){
			for(j=0; j<10; j++){
				masked_blit(piso, buffer, bloco[i][j].wx, bloco[i][j].wy, bloco[i][j].x, bloco[i][j].y, bloco[i][j].w, bloco[i][j].h);
				if (colidir(p.x+10, p.y+25, bloco[i][j].x, bloco[i][j].y, 34, 40, bloco[i][j].w, 10)){
					p.y = bloco[i][j].y - p.h;
					caindo = 0;
				}
			}
		}*/
		
		masked_blit(jogador, buffer, p.wx + nTile*50, p.wy + dir*50, p.x, p.y, p.w, p.h);
		//masked_blit(jogador, buffer, p.wx, p.wy, p.x, p.y, p.w, p.h);
		//rectfill(buffer, 0, 550, width, height, 0xfeebbf);
		draw_sprite(screen, buffer, 0, 0);
		pause();
		rest(50);
		clear(buffer);
		rectfill(buffer, 0, 0, width, height, 0x33ffff);
	}
	
	//Finalização
	destroy_bitmap(buffer);
	destroy_bitmap(fundoMenu);
	destroy_bitmap(jogador);
	destroy_bitmap(piso);
	destroy_sample(soundIntro);
	destroy_sample(soundPause);
	destroy_sample(soundDeath);

	return 0;
}
END_OF_MAIN();

void sair(){
	sai = 1;
}

void menu(){
	int time = 0, flash = 0;
	
	while(!(sai || key[KEY_ESC]) && !start){
		
		if(key[KEY_ENTER] && !start){
			p.x = 50;
			p.y = 50;
			start = 1;
			lost = 0;
			stop_sample(soundIntro);
		}else{	
			// Contador
			if(time > 20) 
				time = 0;
			
			draw_sprite(buffer, fundoMenu, 0, 0);
			
			if(time < 10)
				textprintf_centre_ex(buffer, font, width/2, height/1.3, 0xffffff, -1, "Prima ENTER para continuar...");
			
			draw_sprite(screen, buffer, 0, 0);
			rest(50);
			clear(buffer);
			time++;
		}
		
	}
}

void pause(){
	while(key[KEY_P] && pausa);
	while(!key[KEY_P] && pausa && !(sai || key[KEY_ESC]));
	pausa = 0;
	while(key[KEY_P] && !pausa);
}

void control(){
	
	if(key[KEY_SPACE] && !pulando && !vly){
		pLimit = p.y;
		pulando = 1;
	}
		
	if(key[KEY_UP]){
		if(key[KEY_UP] && key[KEY_RIGHT]){
			p.x+=10; dir = 2; nTile++;
		}else if(key[KEY_UP] && key[KEY_LEFT]){
			p.x-=10; dir = 1; nTile++;
		}else{
			dir = 3; nTile++;
		}
		p.y-=10;
		//p.y-=10; dir = 3; nTile++;
	}else if(key[KEY_RIGHT]){p.x+=10; dir = 2; nTile++;}
	else if(key[KEY_LEFT]){p.x-=10; dir = 1; nTile++;}
	else if(key[KEY_DOWN]){
		dir = 0; nTile++;
		//p.y+=10; dir = 0; nTile++;
	}
	else nTile = 1;
	if(nTile < 0) nTile = 2;
	if(nTile > 2) nTile =0;
	
	if(pulando && p.y > pLimit - 30){
		p.y += vly;
		vly =-vup;
		caindo = 1;
	}else if(caindo){
		pulando = 0;
		vly += grv;
		p.y += vly;
	}else {
		pulando = 0;
		vly = 0;
	}
	caindo = 1;
}

void mapa(){
	int i, j;											//15
	char map[13][15] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,3,3,3,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,1,1,1,2,1,0,0,0},
						{0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
						{0,0,0,0,0,0,0,0,0,1,1,1,1,0,0},
						{0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,1,2,3,1,0,0,0,0},
						{0,0,1,0,0,1,0,0,0,0,0,0,0,0,0},
						{1,3,3,0,3,3,0,0,1,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}; //13
						
	for(i=0; i<13; i++){
		for(j=0; j<15; j++){
			if(map[i][j]) 
				bloco[i][j].wx = (map[i][j]-1)*bloco[i][j].w;
			else 
				bloco[i][j].x = out;
			mp[i][j] = map[i][j];
		}
	}
}

void blocos(){
	int i, j;
	
	for(i=0; i<13; i++){
		for(j=0; j<15; j++){
			if (colidir(p.x+10, p.y+25, bloco[i][j].x, bloco[i][j].y, 34, 40, bloco[i][j].w, 10)){
				if(mp[i][j] != 2){
					caindo = 0;
					p.y = bloco[i][j].y - p.h;
				}
			}
			masked_blit(piso, buffer, bloco[i][j].wx, bloco[i][j].wy, bloco[i][j].x, bloco[i][j].y, bloco[i][j].w, bloco[i][j].h);
		}
	}
}

int colidir(int Ax, int Ay, int Bx, int By, int Aw, int Ah, int Bw, int Bh){
	if(Ax + Aw > Bx && Ax < Bx + Bw && Ay + Ah > By && Ay < By + Bh)
		return 1;
	return 0;
}

void end(){
	if(p.y > height){
		lost = 1;
		start = 0;
	}
}
