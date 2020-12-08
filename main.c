#include <allegro.h>

// Funções
void sair();
void pause();
void control(); 
int colidir(int Ax, int Ay, int Bx, int By, int Aw, int Ah, int Bw, int Bh);


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
int pausa = 0;
int dir = 0;
int vly = 0;
int vup = 15;
int grv = 4;
int width = 900;
int height = 720;
int nTile=0;
int caindo = 1;
int pulando = 0;
int pLimit = 0;

BITMAP *buffer, *imagem, *piso;

int main() {
	
	// Inicialização
	allegro_init();
	install_keyboard();
	set_color_depth(32);
	set_window_title("Jogo Das Tropa");
	set_close_button_callback(sair);
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
	int i, j;
	// Variáveis Locais
	buffer = create_bitmap(width, height);
	imagem = load_bitmap("./assets/img/player/dragonball.bmp", NULL);
	piso = load_bitmap("./assets/img/grama.bmp", NULL);
	SAMPLE *sPause = load_sample("./assets/sound/pause.wav");
	
	//Alimanhamento da matriz
	for(i=0; i<13; i++){
		for(j=0; j<15; j++){
			bloco[i][j].y = 400;
			bloco[i][j].x = j*60;
			bloco[i][j].w = 60;
			bloco[i][j].h = 50;
			bloco[i][j].wy = 0;
		}
	}
	
	while (!(sai || key[KEY_ESC])) {
		// put your code here
		
		// Pausa
		if(key[KEY_P] && !pausa){
			pausa = 1;
			play_sample(sPause, 255, 128, 1000, 0);
			textprintf_centre_ex(buffer, font, width/2, height/2, 0x000000, -1, "PAUSE");
		}
		
		control();
		for(i=0; i<13; i++){
			for(j=0; j<10; j++){
				masked_blit(piso, buffer, bloco[i][j].wx, bloco[i][j].wy, bloco[i][j].x, bloco[i][j].y, bloco[i][j].w, bloco[i][j].h);
				if (colidir(p.x+10, p.y+50, bloco[i][j].x, bloco[i][j].y, 34, 40, bloco[i][j].w, 10)){
					p.y = bloco[i][j].y - p.h;
					caindo = 0;
				}
			}
		}
		
		masked_blit(imagem, buffer, p.wx + nTile*50, p.wy + dir*50, p.x, p.y, p.w, p.h);
		//masked_blit(imagem, buffer, p.wx, p.wy, p.x, p.y, p.w, p.h);
		rectfill(buffer, 0, 650, width, height, 0xfeebbf);
		draw_sprite(screen, buffer, 0, 0);
		pause();
		rest(50);
		clear(buffer);
		rectfill(buffer, 0, 0, width, height, 0xffffff);
	}
	
	//Finalização
	destroy_bitmap(buffer);
	destroy_bitmap(imagem);
	//destroy_sample(sPause);

	return 0;
}
END_OF_MAIN();

void sair(){
	sai = 1;
}

void pause(){
	while(key[KEY_P] && pausa);
	while(!key[KEY_P] && pausa && !(sai || key[KEY_ESC]));
	pausa = 0;
	while(key[KEY_P && !pausa]);
}

void control(){
	if(key[KEY_SPACE] && !pulando && !vly){
		pLimit = p.y;
		pulando = 1;
	}
	
	if(key[KEY_UP]){p.y-=10; dir = 3; nTile++;}
	else if(key[KEY_RIGHT]){p.x+=10; dir = 2; nTile++;}
	else if(key[KEY_LEFT]){p.x-=10; dir = 1; nTile++;}
	else if(key[KEY_DOWN]){p.y+=10; dir = 0; nTile++;}
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

int colidir(int Ax, int Ay, int Bx, int By, int Aw, int Ah, int Bw, int Bh){
	if(Ax + Aw > Bx && Ax < Bx + Bw && Ay + Ah > By && Ay < By + Bh)
	return 1;
	return 0;
}
