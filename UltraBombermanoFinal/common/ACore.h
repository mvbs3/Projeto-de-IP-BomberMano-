#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define FPS 60
#define WIDTH  640
#define HEIGHT 480
#define IP_MAX_SIZE 100
#define numPowerUps 25

double startingTime;
ALLEGRO_DISPLAY *janela;
ALLEGRO_BITMAP *menu;
ALLEGRO_BITMAP *spriteCursor;
ALLEGRO_BITMAP *sprite1;
ALLEGRO_BITMAP *sprite2;
ALLEGRO_BITMAP *sprite3;
ALLEGRO_BITMAP *sprite4;
ALLEGRO_BITMAP *sprite5;
ALLEGRO_BITMAP *spriteEscolha;
ALLEGRO_BITMAP *BotaoHelp;
ALLEGRO_BITMAP *BotaoPlay;
ALLEGRO_BITMAP *BotaoQuit;
ALLEGRO_BITMAP *BotaoBack;
ALLEGRO_BITMAP *BotaoOk;
ALLEGRO_BITMAP *helpScreen;
ALLEGRO_BITMAP *arrowKeys;
ALLEGRO_BITMAP *qKey;
ALLEGRO_FONT *fonte;
ALLEGRO_FONT *texto;
ALLEGRO_DISPLAY *main_window;
ALLEGRO_EVENT_QUEUE *eventsQueue;
ALLEGRO_EVENT_QUEUE *fila_eventos;
ALLEGRO_EVENT_QUEUE *fila_bomba;
ALLEGRO_EVENT evento;
//========================
//FONT AND BITMAP POINTERS
ALLEGRO_FONT *ubuntu;
ALLEGRO_FONT *start;

ALLEGRO_BITMAP *objects;
ALLEGRO_BITMAP *menuScreen;
ALLEGRO_BITMAP *mapa;

ALLEGRO_BITMAP *cima;
ALLEGRO_BITMAP *baixo;
ALLEGRO_BITMAP *esquerda;
ALLEGRO_BITMAP *direita;

ALLEGRO_BITMAP *cima2;
ALLEGRO_BITMAP *baixo2;
ALLEGRO_BITMAP *esquerda2;
ALLEGRO_BITMAP *direita2;

ALLEGRO_BITMAP *cima3;
ALLEGRO_BITMAP *baixo3;
ALLEGRO_BITMAP *esquerda3;
ALLEGRO_BITMAP *direita3;

ALLEGRO_BITMAP *cima4;
ALLEGRO_BITMAP *baixo4;
ALLEGRO_BITMAP *esquerda4;
ALLEGRO_BITMAP *direita4;

ALLEGRO_BITMAP *cima5;
ALLEGRO_BITMAP *baixo5;
ALLEGRO_BITMAP *esquerda5;
ALLEGRO_BITMAP *direita5;

ALLEGRO_BITMAP *cima6;
ALLEGRO_BITMAP *baixo6;
ALLEGRO_BITMAP *esquerda6;
ALLEGRO_BITMAP *direita6;

ALLEGRO_BITMAP *foguinho;
ALLEGRO_BITMAP *maisBomba;
ALLEGRO_BITMAP *spriteBomb;
ALLEGRO_BITMAP *blocos;
ALLEGRO_BITMAP *explodeCenter;
ALLEGRO_BITMAP *explodeLado;
ALLEGRO_BITMAP *explodeCima;
ALLEGRO_BITMAP *blocoExplod;
//========================
//========================
ALLEGRO_SAMPLE *sample;
ALLEGRO_SAMPLE *sampleExplode;
ALLEGRO_SAMPLE *sampleGanhou;
ALLEGRO_SAMPLE *samplePerdeu;


//EXAMPLE STRUCT
typedef struct{
    int x, y, width, height;
} botao;

typedef struct DADOS
{
    char mensagem[100];
    int valor;
}DADOS;

typedef struct{
  int x,y;
} Posicao;

typedef struct{

 Posicao pos;
 int explode;
 
} Explosao;

typedef struct {
  Posicao pos;
  int ativa;
  ALLEGRO_BITMAP **spriteB;
  int numSpriteB;
  Explosao ex;
  
  int quebraCima;
  int quebraBaixo;
  int quebraEsquerda;
  int quebraDireita;
  int quebra;
} Bomba;

typedef struct {
Posicao pos;
int ativo;
} Bloco;

typedef struct{
  Posicao pos;
  ALLEGRO_BITMAP **sprite;
  int numSprite;
  int skin;
  int conect;
  int numBombas;
  int potencia;
  int win;
  int ok;
  Bomba minhaBomba[4];


} Personagem;

typedef struct {
Posicao pos;
int ativo;
int qualUp;
} powerUp;

typedef struct{

  int start;
  Personagem todos[4];
  Bloco bloquinho[99];
  powerUp upgrades[numPowerUps];
} Terreno;


//MAIN ALLEGRO FUNCTIONS
bool coreInit();
bool windowInit(int W, int H, char title[]);
bool inputInit();
void allegroEnd();

//FPS CONTROL FUNCTIONS
void startTimer();
double getTimer();
void FPSLimit();

//RESOURCE LOADING FUNCTIONS
bool loadGraphics();
bool fontInit();

//INPUT READING FUNCTION
void readInput(ALLEGRO_EVENT event, char str[], int limit);


#endif
