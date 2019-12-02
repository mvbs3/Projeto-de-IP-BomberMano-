#include "ACore.h"
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define BordaX 48
#define BordaY 34

#define TamQuadradoX 42.3
#define TamQuadradoY 32

#define numBlocos 99
#define numMaxBomba 4
#define tempDeExplo 1
#define tempoBomba 2

#define LARGURA 640
#define ALTURA 480
#define MAX_LOG_SIZE 17

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 13
#define HIST_MAX_SIZE 200
#define temporizador 2.0/FPS
ALLEGRO_TIMER *timer;
int flag=0;


int flagExplode[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int tempo=0;
int id,a = numBlocos, ba = numBlocos, bb = numBlocos, bc = numBlocos;
int perso; //VARIAVEL QUE VAI RECEBER O SEU ID
Terreno recebe; //VARIAVEL GLOBAL QUE RECEBE O MAPA INTEIRO
float seg[4]={0,0,0,0};
float tempExplode[4]={0,0,0,0};
int quebraCima[4]={99,99,99,99};
int quebraBaixo[4]={99,99,99,99};
int quebraEsquerda[4]={99,99,99,99};
int quebraDireita[4]={99,99,99,99};


botao botaoHelp, botaoPlay, botaoQuit,botaoBack2, botaoBack, botaoOk;
botao Sprite1, Sprite2, Sprite3, Sprite4, Sprite5;
int escolha = 0;

void desenhaPosicao(Terreno eu);//DESENHA MAPA INTEIRO
void vaiPraCima(Personagem *eu);// MANDA PERSONAGEM PARA CIMA
void vaiPraBaixo(Personagem *eu);//MANDA PERSONAGEM PARA BAIXO
void vaiPraDir(Personagem *eu);//MANDA PERSONAGEM PARA DIREITA
void vaiPraEsq(Personagem *eu);//MANDA PERSONAGEM PARA ESQUERDA
void colisao(Personagem *oi, int onde);//TESTA SE O PERSONAGEM COLIDIU
void printHello();//PRINTA MENSAGEM INICIALD E CONEXAO
void meuSprite();//ESCOLHE O SPRITE DOS PERSONAGENS
void spriteBomba();//DA O SPRITE PARA AS BOMBAS

void assertConnection();
enum conn_ret_t tryConnect(); // FUNCOES DE CONEXAO COM SERVIDOR

void iniciaMenu(ALLEGRO_BITMAP *BotaoHelp, ALLEGRO_BITMAP *BotaoPlay, ALLEGRO_BITMAP *BotaoQuit);
void encerraJogo();
int help(ALLEGRO_EVENT evento);
bool click(botao button, ALLEGRO_EVENT evento);
void setButtons();
void escolheSkin(ALLEGRO_EVENT evento);
void escolheu(int escolha);
void carregaSkin();
void carregaTelaSkin();
void escolhaFinal(ALLEGRO_BITMAP *spriteEscolha, int x, int y);
void perdeu();
void esperaPlayer();
void iniciaJogo(int escolha);
void telaVitoria();
int onde,i;
bool sair = false;
//******************************************************
int main(){

assertConnection();//CHAMA FUNCAO DE CONEXAO

//int tecla = 0,onde,i;
//Personagem oi,ola;
//Terreno completo;
//oi.pos.x=0;
//oi.pos.y=0;
//oi.sprite = &baixo;
//completo = recebe;

//printf("recebe.todos[id].pos.x=%d",completo.todos[id-1].pos.x);
//printf("recebe.todos[id].pos.x=%d",recebe.todos[id].pos.x);
//printf("recebe.todos[id].pos.y=%d",recebe.todos[id].pos.y);

//printf("completo.todos[1].pos.x=%d",completo.todos[0]);



if (!coreInit()) //INICIALIZA AS BIBLIOTECAS DA ALLEGRO
        return -1;

if (!windowInit(LARGURA, ALTURA, "Allegro Example - Graphical Chat"))//INICIA TELA DO JOGO
      return -1;

if (!inputInit()) //INICIA AS ENTRADAS
        return -1;



 

/*al_init();
al_init_image_addon();*/
al_install_keyboard();
al_install_mouse(); //INICIA MOUSE E TECLADO(ESTAVA DANDO ERRO NO INPUT INIT)


if(!loadGraphics())//CARREGA TODOS OS SPRITES E MAPA
        return -1;


al_register_event_source(fila_eventos, al_get_keyboard_event_source());
al_register_event_source(fila_eventos, al_get_mouse_event_source());
al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
al_set_mouse_cursor(janela, al_create_mouse_cursor(spriteCursor, 0, 0));
al_play_sample(sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
//al_rest(10.0);
iniciaMenu(BotaoHelp, BotaoPlay, BotaoQuit);  
  
//INICIA TODOS OS EVENTOS
    return 0;
}

enum conn_ret_t tryConnect() {
  char server_ip[30];
  printf("Please enter the server IP: ");
  scanf(" %s", server_ip);
  getchar();
  return connectToServer(server_ip);
}

void assertConnection() { //CONEXAO COM SERVIDOR
  printHello();
  enum conn_ret_t ans = tryConnect();
  while (ans != SERVER_UP) {
    if (ans == SERVER_DOWN) {
      puts("Server is down!");
    } else if (ans == SERVER_FULL) {
      puts("Server is full!");
    } else if (ans == SERVER_CLOSED) {
      puts("Server is closed for new connections!");
    } else {
      puts("Server didn't respond to connection!");
    }
    printf("Want to try again? [Y/n] ");
    int res;
    while (res = tolower(getchar()), res != 'n' && res != 'y' && res != '\n'){
      puts("anh???");
    }
    if (res == 'n') {
      exit(EXIT_SUCCESS);
    }
    ans = tryConnect();
  }
  char login[LOGIN_MAX_SIZE + 4];
  printf("Please enter your login (limit = %d): ", LOGIN_MAX_SIZE);
  scanf(" %[^\n]", login);
  getchar();
  int len = (int)strlen(login);
  sendMsgToServer(login, len + 1);
  recvMsgFromServer(&id, WAIT_FOR_IT);
  recvMsgFromServer(&recebe, WAIT_FOR_IT);
  
}
//*****************************************************************************************************
void desenhaPosicao(Terreno eu){// desenha mapa
    int i,j,x,y;
    al_draw_bitmap(mapa, 0, 0, 0);// desenha o mapa

//**********************************************************************************************************

    for(i=0;i<numPowerUps;i++){
        if(recebe.upgrades[i].qualUp==0&&recebe.upgrades[i].ativo==1)
            al_draw_bitmap(foguinho, (recebe.upgrades[i].pos.x * TamQuadradoX) +BordaX, (recebe.upgrades[i].pos.y * TamQuadradoY) +BordaY, 0);
    
        if(recebe.upgrades[i].qualUp==1&&recebe.upgrades[i].ativo==1)
            al_draw_bitmap(maisBomba, (recebe.upgrades[i].pos.x * TamQuadradoX) +BordaX, (recebe.upgrades[i].pos.y * TamQuadradoY) +BordaY, 0);
    }
//***************************************************
    for (i=0;i<numBlocos;i++){ // desenha blocos
        if(recebe.bloquinho[i].ativo==1)
        al_draw_bitmap(blocos, (recebe.bloquinho[i].pos.x * TamQuadradoX) +BordaX, (recebe.bloquinho[i].pos.y * TamQuadradoY) +BordaY, 0);
    }
//********************************************************************************************************
    for(i=0;i<4;i++){
//*********************************************************************************************************
        if(recebe.todos[i].conect == 1){//desenha boneco
            al_draw_bitmap(*(recebe.todos[i].sprite), (recebe.todos[i].pos.x * TamQuadradoX) +BordaX, (recebe.todos[i].pos.y * TamQuadradoY) +BordaY, 0);
//******************************************************************************************************
            for(j=0;j<4;j++){ //DESENHA BOMBA
                if(recebe.todos[i].minhaBomba[j].ativa == 1 && recebe.todos[i].minhaBomba[j].ex.explode == 0)//desenha bomba
                    al_draw_bitmap(*(recebe.todos[i].minhaBomba[j].spriteB), (recebe.todos[i].minhaBomba[j].pos.x * TamQuadradoX) +BordaX, (recebe.todos[i].minhaBomba[j].pos.y * TamQuadradoY) +BordaY, 0);
            }
//**************************************************************************************************************************************************
            for(j=0;j<4;j++){// DESENHA BOMBA
                if(recebe.todos[i].minhaBomba[j].ativa == 1 && recebe.todos[i].minhaBomba[j].ex.explode == 1){//desenha bomba
                    al_play_sample(sampleExplode, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                   
                   //EXPLODE CENTRO
                    al_draw_bitmap(explodeCenter, (recebe.todos[i].minhaBomba[j].pos.x * TamQuadradoX) +BordaX, (recebe.todos[i].minhaBomba[j].pos.y * TamQuadradoY) +BordaY, 0);
                    //DESENHA A EXPLOSAO CENTRAL DA BOMBA
                    if(recebe.todos[id].pos.x == recebe.todos[i].minhaBomba[j].pos.x && recebe.todos[id].pos.y == recebe.todos[i].minhaBomba[j].pos.y){
                        //TESTA SE O CENTRO DA BOMBA ATINGIU SEU PERSONAGEM
                        recebe.todos[id].conect = 0;//CASO ATINJA, O PERSONAGEM MORRE
                        sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                        perdeu();
                    }
//*********************************************************************************************************
    //EXPLODE PARA DIREITA
                for(x=recebe.todos[i].minhaBomba[j].pos.x + 1; x<=(recebe.todos[i].minhaBomba[j].pos.x + recebe.todos[i].potencia); x++){
                    if(x>=0&&x<=12){//testa se nao saiu do mapa
                        if(x%2==0 || recebe.todos[i].minhaBomba[j].pos.y %2==0){ // testa se nao chegou no bloco impar
                            for(a=0;a<numBlocos;a++){// testa se nao colidiu com um bloco
                                if(recebe.bloquinho[a].ativo==1 && recebe.bloquinho[a].pos.x == x && recebe.bloquinho[a].pos.y == recebe.todos[i].minhaBomba[j].pos.y){
                                    al_draw_bitmap(blocoExplod, (x * TamQuadradoX) +BordaX, (recebe.todos[i].minhaBomba[j].pos.y * TamQuadradoY) +BordaY, 0);
                                    if(i==id)
                                    quebraCima[j]=a;
                                    //recebe.todos[id].minhaBomba[j].quebraCima = a;
                                    x = (recebe.todos[i].minhaBomba[j].pos.x + recebe.todos[i].potencia) +1;//sair do For
                                    break;
                                }
                    else{
                        al_draw_bitmap(explodeLado, (x * TamQuadradoX) +BordaX, (recebe.todos[i].minhaBomba[j].pos.y * TamQuadradoY) +BordaY, 0);
                        //MORTO
                        if(recebe.todos[id].pos.x == x && recebe.todos[id].pos.y == recebe.todos[i].minhaBomba[j].pos.y){
                            recebe.todos[id].conect = 0;
                            sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                            perdeu();
                        }                   
                    }
                            }
                        }else break;//x = (recebe.todos[i].minhaBomba[j].pos.x + recebe.todos[i].potencia) +1; // SAI DO LOOP
                    }
                }
 //*******************************************************************************************************   
    //EXPLODE PARA ESQUERDA
                for(x=recebe.todos[i].minhaBomba[j].pos.x - 1; x>=(recebe.todos[i].minhaBomba[j].pos.x - recebe.todos[i].potencia); x--){
                    if(x>=0&&x<=12){
                        if(x%2==0 || recebe.todos[i].minhaBomba[j].pos.y %2==0){
                            for(ba=0;ba<numBlocos;ba++){// testa se nao colidiu com um bloco
                                if(recebe.bloquinho[ba].ativo==1 && recebe.bloquinho[ba].pos.x == x && recebe.bloquinho[ba].pos.y == recebe.todos[i].minhaBomba[j].pos.y){
                                // recebe.bloquinho[a].ativo=0;
                                    al_draw_bitmap(blocoExplod, (x * TamQuadradoX) +BordaX, (recebe.todos[i].minhaBomba[j].pos.y * TamQuadradoY) +BordaY, 0);
                                    if(i==id)
                                    quebraBaixo[j]=ba;
                                    //recebe.todos[id].minhaBomba[j].quebraCima = ba;
                                    x = (recebe.todos[i].minhaBomba[j].pos.x - recebe.todos[i].potencia) -1;
                                    break;
                                }else {
                                    al_draw_bitmap(explodeLado, (x * TamQuadradoX) +BordaX, (recebe.todos[i].minhaBomba[j].pos.y * TamQuadradoY) +BordaY, 0);
                                    if(recebe.todos[id].pos.x == x && recebe.todos[id].pos.y == recebe.todos[i].minhaBomba[j].pos.y){
                                        recebe.todos[id].conect = 0;
                                        sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                                        perdeu();
                                    }
                                }
                            }               
                        } else break;//x = (recebe.todos[i].minhaBomba[j].pos.x - recebe.todos[i].potencia) -1; // SAI DO LOOP 
                    }
                }
 //************************************************************************************************************   
    //EXPLODE PARA BAIXO
                for(y=recebe.todos[i].minhaBomba[j].pos.y + 1; y<=(recebe.todos[i].minhaBomba[j].pos.y + recebe.todos[i].potencia ); y++){   
                    if(y>=0&&y<=12){
                        if(recebe.todos[i].minhaBomba[j].pos.x % 2==0 || y % 2==0){
                            for(bb=0;bb<numBlocos;bb++){// testa se nao colidiu com um bloco
                                if(recebe.bloquinho[bb].ativo==1 && recebe.bloquinho[bb].pos.x == recebe.todos[i].minhaBomba[j].pos.x && recebe.bloquinho[bb].pos.y == y){
                                // recebe.bloquinho[a].ativo=0;
                                al_draw_bitmap(blocoExplod, (recebe.todos[i].minhaBomba[j].pos.x * TamQuadradoX) +BordaX, (y * TamQuadradoY) +BordaY, ALLEGRO_FLIP_HORIZONTAL);
                                if(i==id)
                                quebraDireita[j]=bb;    
                                    //recebe.todos[id].minhaBomba[j].quebraCima = bb;
                                y = (recebe.todos[i].minhaBomba[j].pos.y + recebe.todos[i].potencia ) +1;
                                //x = (recebe.todos[i].minhaBomba[j].pos.x + recebe.todos[i].potencia) +1;
                                break;
                                }
                                else{
                                    al_draw_bitmap(explodeCima, (recebe.todos[i].minhaBomba[j].pos.x * TamQuadradoX) +BordaX, (y * TamQuadradoY) +BordaY, ALLEGRO_FLIP_HORIZONTAL);
                                    if(recebe.todos[id].pos.x == recebe.todos[i].minhaBomba[j].pos.x  && recebe.todos[id].pos.y == y){
                                        recebe.todos[id].conect = 0;
                                        sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                                        perdeu();
                                    }
                                } 
                            }               

                        }else break;//y = (recebe.todos[i].minhaBomba[j].pos.y + recebe.todos[i].potencia ) +1;
                    }
                }
//*****************************************************************************************************    
    //EXPLODE PARA CIMA
                for(y=recebe.todos[i].minhaBomba[j].pos.y + -1; y>=(recebe.todos[i].minhaBomba[j].pos.y - recebe.todos[i].potencia); y--){
                    if(y>=0&&y<=12){   
                        if(recebe.todos[i].minhaBomba[j].pos.x % 2==0 || y % 2==0){
                            for(bc=0;bc<numBlocos;bc++){// testa se nao colidiu com um bloco
                                if(recebe.bloquinho[bc].ativo==1 && recebe.bloquinho[bc].pos.x == recebe.todos[i].minhaBomba[j].pos.x && recebe.bloquinho[bc].pos.y == y){
                                // recebe.bloquinho[a].ativo=0;
                                    al_draw_bitmap(blocoExplod, (recebe.todos[i].minhaBomba[j].pos.x * TamQuadradoX) +BordaX, (y * TamQuadradoY) +BordaY, ALLEGRO_FLIP_HORIZONTAL);
                                    if(i==id)
                                    quebraEsquerda[j]=bc;
                                    //recebe.todos[id].minhaBomba[j].quebraCima = bc;
                                    y = (recebe.todos[i].minhaBomba[j].pos.y - recebe.todos[i].potencia ) -1;
                                    //x = (recebe.todos[i].minhaBomba[j].pos.x + recebe.todos[i].potencia) +1;
                                    break;
                                }
                                else {
                                    al_draw_bitmap(explodeCima, (recebe.todos[i].minhaBomba[j].pos.x * TamQuadradoX) +BordaX, (y * TamQuadradoY) +BordaY, ALLEGRO_FLIP_HORIZONTAL);
                                    if(recebe.todos[id].pos.x == recebe.todos[i].minhaBomba[j].pos.x  && recebe.todos[id].pos.y == y){
                                        recebe.todos[id].conect = 0;
                                        sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                                        perdeu();
                                    } 
                                }
                            }
                        
                        }else break;//y = (recebe.todos[i].minhaBomba[j].pos.y - recebe.todos[i].potencia ) -1;
        
                    }
        
                }
//****************************************************************************************************    
                }
            }

        }
    }





    al_flip_display();
}
//**************************************************************************************************************
void  vaiPraCima(Personagem *eu){
  eu[0].pos.y--;
}
//**************************************************************************************************************

void  vaiPraBaixo(Personagem *eu){
  eu[0].pos.y++;
}

//**************************************************************************************************************
void  vaiPraDir(Personagem *eu){
  eu[0].pos.x++;
}

//**************************************************************************************************************
void  vaiPraEsq(Personagem *eu){
  eu[0].pos.x--;
}

//**************************************************************************************************************
void colisao(Personagem *oi, int onde){
    if(oi[0].pos.y < 0){//TESTA SE VAI SAIR PRA CIMA DO MAPA
        vaiPraBaixo(oi);
        return;
    }
    if(oi[0].pos.y > 12){//TESTA SE VAI SAIR PRA BAIXO DO MAPA
        vaiPraCima(oi);
        return;
    }
     if(oi[0].pos.x < 0){//TESTA SE VAI SAIRA PRA ESQUERDA DO MAPA
        vaiPraDir(oi);
        return;
    }
     if(oi[0].pos.x > 12){//TESTA SE VAI SAIR PRA DIREITA DO MAPA
        vaiPraEsq(oi);
        return;
    }
    if((oi[0].pos.x)%2!=0 && (oi[0].pos.y)%2!=0){//TESTA SE VAI COLIDIR NOS BLOCOS IMPARxIMPAR
        switch(onde){
            case 1://up
                vaiPraBaixo(oi);
                return;
                break;
            case 2://down
                vaiPraCima(oi);
                return;
                break;
            case 3://left
                vaiPraDir(oi);
                return;
                break;
            case 4://right
                vaiPraEsq(oi);
                return;
                break;
            // return;
        }
    }
    int i,j;
    for(i=0;i<numBlocos;i++){ //TESTA COLISAO COM BLOCOS ATIVOS
        if((oi[0].pos.x)==recebe.bloquinho[i].pos.x && (oi[0].pos.y)==recebe.bloquinho[i].pos.y && recebe.bloquinho[i].ativo ==1 ){
            switch(onde){
                case 1://up
                    vaiPraBaixo(oi);
                    return;
                    break;
                case 2://down
                    vaiPraCima(oi);
                    return;
                    break;
                case 3://left
                    vaiPraDir(oi);
                    return;
                    break;
                case 4://right
                    vaiPraEsq(oi);
                    return;
                    break;      
            }
        }
    }

    for(i=0;i<4;i++){ //TESTA COLISAO COM BOMBAS ATIVAS
        for(j=0;j<4;j++)
            if(recebe.todos[i].minhaBomba[j].ativa==1 && (oi[0].pos.x) == recebe.todos[i].minhaBomba[j].pos.x && (oi[0].pos.y)==recebe.todos[i].minhaBomba[j].pos.y && recebe.todos[i].minhaBomba[j].ex.explode == 0){
                switch(onde){
                    case 1://up
                        vaiPraBaixo(oi);
                        return;
                        break;
                    case 2://down
                        vaiPraCima(oi);
                        return;
                        break;
                    case 3://left
                        vaiPraDir(oi);
                        return;
                        break;
                    case 4://right
                        vaiPraEsq(oi);
                        return;
                        break;      
                }
            }
    }

}

//**************************************************************************************************************

void printHello() {
  puts("Hello! Welcome to bombermano");
  puts("We need some infos to start up!");
}

//**************************************************************************************************************
void spriteBomba(){
    int i,j;
    for(i=0;i<4;i++)
    for(j=0;j<4;j++)
    recebe.todos[i].minhaBomba[j].spriteB = &spriteBomb;
}


//**************************************************************************************************************
void meuSprite(){
    int i;
    for(i=0;i<4;i++){
        if(recebe.todos[i].conect==1){   
            switch(recebe.todos[i].skin){
//****************************************************************************************************                
                case 2:
                    switch(recebe.todos[i].numSprite){
                        case 1:
                            recebe.todos[i].sprite = &baixo;
                            break;
                        case 2:
                            recebe.todos[i].sprite = &cima;
                            break;
                        case 3:
                            recebe.todos[i].sprite = &esquerda;
                            break;
                        case 4:
                            recebe.todos[i].sprite = &direita;
                            break;
                        case 5:
                            break;
                    }
                break;
 //****************************************************************************               
                case 3:
                    switch(recebe.todos[i].numSprite){
                        case 1:
                            recebe.todos[i].sprite = &baixo2;
                            break;
                        case 2:
                            recebe.todos[i].sprite = &cima2;
                            break;
                        case 3:
                            recebe.todos[i].sprite = &esquerda2;
                            break;
                        case 4:
                            recebe.todos[i].sprite = &direita2;
                            break;
                        case 5:
                            break;
                    }
                break;
//***************************************************************************************************
                case 1:
                    switch(recebe.todos[i].numSprite){
                        case 1:
                            recebe.todos[i].sprite = &baixo3;
                            break;
                        case 2:
                            recebe.todos[i].sprite = &cima3;
                            break;
                        case 3:
                            recebe.todos[i].sprite = &esquerda3;
                            break;
                        case 4:
                            recebe.todos[i].sprite = &direita3;
                            break;
                        case 5:
                            break;
                    }
                break;
//**********************************************************************************************
                case 5:
                    switch(recebe.todos[i].numSprite){
                        case 1:
                            recebe.todos[i].sprite = &baixo4;
                            break;
                        case 2:
                            recebe.todos[i].sprite = &cima4;
                            break;
                        case 3:
                            recebe.todos[i].sprite = &esquerda4;
                            break;
                        case 4:
                            recebe.todos[i].sprite = &direita4;
                            break;
                        case 5:
                            break;
                    }
                break;

 //***************************************************************************************************
                case 4:
                    switch(recebe.todos[i].numSprite){
                        case 1:
                            recebe.todos[i].sprite = &baixo5;
                            break;
                        case 2:
                            recebe.todos[i].sprite = &cima5;
                            break;
                        case 3:
                            recebe.todos[i].sprite = &esquerda5;
                            break;
                        case 4:
                            recebe.todos[i].sprite = &direita5;
                            break;
                        case 5:
                            break;
                    }
                break;


                case 6:
                    switch(recebe.todos[i].numSprite){
                        case 1:
                            recebe.todos[i].sprite = &baixo6;
                            break;
                        case 2:
                            recebe.todos[i].sprite = &cima6;
                            break;
                        case 3:
                            recebe.todos[i].sprite = &esquerda6;
                            break;
                        case 4:
                            recebe.todos[i].sprite = &direita6;
                            break;
                        case 5:
                            break;
                    }
                break;
            }
        }
    }
}
//**************************************************************************************************************
void iniciaMenu(ALLEGRO_BITMAP *BotaoHelp, ALLEGRO_BITMAP *BotaoPlay, ALLEGRO_BITMAP *BotaoQuit){
    setButtons();
    al_draw_bitmap(menu, 0, 0, 0);    
    BotaoHelp = al_load_bitmap("Resources/Sprites/menu/botaoHelp.png");
    BotaoPlay = al_load_bitmap("Resources/Sprites/menu/Play.png");
    BotaoQuit = al_load_bitmap("Resources/Sprites/menu/quit.png");
    al_draw_bitmap(BotaoHelp, botaoHelp.x, botaoHelp.y, 0);
    al_draw_bitmap(BotaoPlay, botaoPlay.x, botaoPlay.y, 0);
    al_draw_bitmap(BotaoQuit, botaoQuit.x, botaoQuit.y, 0); 
    al_flip_display();
   
    while(1){
        while(!al_is_event_queue_empty(fila_eventos)){
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
            int onde;
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(evento.keyboard.keycode){
                    case ALLEGRO_KEY_H:
                        help(evento);
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        encerraJogo();  
                        exit(1);
                    case ALLEGRO_KEY_P:
                        escolheSkin(evento);      
                }             
            }             
            else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){             
                encerraJogo();
                exit(1);
            }
            else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                if(click(botaoPlay, evento)){
                    escolheSkin(evento);
                }
                else if(click(botaoHelp, evento)){
                    help(evento);
                }
                else if(click(botaoQuit, evento)){
                    encerraJogo();
                    exit(1);
                }
            }
        }   
    }
}

void encerraJogo(){
    al_clear_to_color(al_map_rgb(0, 100, 255));
    al_draw_text(fonte, al_map_rgb(50, 50, 50),330, 230, ALLEGRO_ALIGN_CENTRE, "Saindo do bombermano");
    al_flip_display();
    al_rest(1.5);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
}

void perdeu(){
    al_clear_to_color(al_map_rgb(0, 100, 255));
    al_draw_text(fonte, al_map_rgb(50, 50, 50),330, 230, ALLEGRO_ALIGN_CENTRE, "Você Perdeu");
    al_flip_display();
    sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
    //sendMsgToServer();
    al_rest(2.0);
    al_play_sample(samplePerdeu, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);

    al_rest(4.0);
    //al_destroy_display(janela);
    //al_destroy_event_queue(fila_eventos);
    exit (1);
}
void esperaPlayer(){
    al_clear_to_color(al_map_rgb(244, 137, 66));
    al_draw_text(fonte, al_map_rgb(50, 50, 50),330, 230, ALLEGRO_ALIGN_CENTRE, "Esperando Outros Players...");
    al_flip_display();
    //al_rest(1.5);
    //al_destroy_display(janela);
    //al_destroy_event_queue(fila_eventos);
    //exit (1);
}

int help(ALLEGRO_EVENT evento){
    char text1[] = "Use as setas do teclado para mexer o bombermano.";
    char text2[] = "Use a tecla Q para soltar uma bomba!";
    char text3[] = "Colete os powerups e destrua seus inimigos!\n :)";
    arrowKeys = al_load_bitmap("Resources/Sprites/menu/ArrowKeys.png");
    qKey = al_load_bitmap("Resources/Sprites/menu/QKey.jpg");
    BotaoBack = al_load_bitmap("Resources/Sprites/menu/Return.png");
    al_draw_bitmap(helpScreen, 0, 0, 0);
    al_draw_bitmap(arrowKeys, 260, 130, 0);
    al_draw_bitmap(qKey, 280, 270, 0);
    al_draw_bitmap(BotaoBack, botaoBack.x, botaoBack.y, 0);
    al_draw_text(fonte, al_map_rgb(255, 0, 255),330, 30, ALLEGRO_ALIGN_CENTRE, "AJUDA");
    al_draw_multiline_text(texto, al_map_rgb(255, 255, 255), 320, 80, 600, 25, ALLEGRO_ALIGN_CENTRE, text1);
    al_draw_multiline_text(texto, al_map_rgb(255, 255, 255), 320, 230, 600, 25, ALLEGRO_ALIGN_CENTRE, text2);
    al_draw_multiline_text(texto, al_map_rgb(255, 255, 255), 320, 350, 600, 25, ALLEGRO_ALIGN_CENTRE, text3);
    al_flip_display();
        while(1){
            while(!al_is_event_queue_empty(fila_eventos)){
                al_wait_for_event(fila_eventos, &evento);
                if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                    switch(evento.keyboard.keycode){
                        case ALLEGRO_KEY_B:
                            iniciaMenu(BotaoHelp, BotaoPlay, BotaoQuit);
                            break;
                        case ALLEGRO_KEY_ESCAPE:
                            encerraJogo();  
                            exit (1);      
                    }             
                }             
                else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){             
                    encerraJogo();
                    exit (1);
                }
                else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if(click(botaoBack, evento)){
                        iniciaMenu(BotaoHelp, BotaoPlay, BotaoQuit);
                        al_flip_display();
                    }
                }
            }
        }
}

bool click(botao button, ALLEGRO_EVENT evento){
        if((evento.mouse.x > button.x + button.width -1) ||
            (evento.mouse.x < button.x) ||
            (evento.mouse.y > button.y + button.height -1) ||
            (evento.mouse.y < button.y)){
        return false;
        }
    return true;
}

void carregaTelaSkin(){
    al_draw_bitmap(helpScreen, 0, 0, 0);
    al_draw_text(texto, al_map_rgb(255, 255, 255),330, 30, ALLEGRO_ALIGN_CENTRE, "ESCOLHA SEU BOMBERMANO");
    al_draw_bitmap(BotaoOk, 15, 400, 0);
    al_draw_bitmap(BotaoBack, 325, 400, 0);
    //al_flip_display();
}

void carregaSkin(){
    carregaTelaSkin();
    al_draw_bitmap(sprite1, Sprite1.x, Sprite1.y, 0);
    al_draw_bitmap(sprite2, Sprite2.x, Sprite2.y, 0);
    al_draw_bitmap(sprite3, Sprite3.x, Sprite3.y, 0);
    al_draw_bitmap(sprite4, Sprite4.x, Sprite4.y, 0);
    al_draw_bitmap(sprite5, Sprite5.x, Sprite5.y, 0);
    al_flip_display();
}

void escolheSkin(ALLEGRO_EVENT evento){
    escolha=0;
    carregaSkin();
    while(1){
        while(!al_is_event_queue_empty(fila_eventos)){
            al_wait_for_event(fila_eventos, &evento);   
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(evento.keyboard.keycode){
                    case ALLEGRO_KEY_B:
                        iniciaMenu(BotaoHelp, BotaoPlay, BotaoQuit);
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        encerraJogo();  
                        exit (1);
                    case ALLEGRO_KEY_1: 
                        escolha = 1; 
                        escolheu(escolha);
                        break;
                    case ALLEGRO_KEY_2: 
                        escolha = 2; 
                        escolheu(escolha);
                        break;      
                    case ALLEGRO_KEY_3: 
                        escolha = 3; 
                        escolheu(escolha);
                        break;
                    case ALLEGRO_KEY_4: 
                        escolha = 4;
                        escolheu(escolha);
                        break;
                    case ALLEGRO_KEY_5: 
                        escolha = 5;
                        escolheu(escolha);
                        break;
                    case ALLEGRO_KEY_ENTER: 
                        al_unregister_event_source(fila_eventos, al_get_mouse_event_source());
                        al_flush_event_queue(fila_eventos);
                        recebe.todos[id].ok=1;
                        iniciaJogo(escolha);
                        break;
                    }             
                }           
                else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){             
                    encerraJogo();
                    exit (1);
                }
                else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                    if(click(Sprite1, evento)){
                        escolha = 1;
                        escolheu(escolha);
                    }
                    else if(click(Sprite2, evento)){
                        escolha = 2;
                        escolheu(escolha);
                    }
                    else if(click(Sprite3, evento)){
                        escolha = 3;
                        escolheu(escolha);
                    }
                    else if(click(Sprite4, evento)){
                        escolha = 4;
                        escolheu(escolha);
                    }
                    else if(click(Sprite5, evento)){
                        escolha = 5;
                        escolheu(escolha);
                    }
                    else if(click(botaoOk, evento) && escolha != 0){
                        //exit (1);
                        al_unregister_event_source(fila_eventos, al_get_mouse_event_source());
                        al_flush_event_queue(fila_eventos);
                        recebe.todos[id].ok=1;
                        //recvMsgFromServer(&recebe, WAIT_FOR_IT);
                        iniciaJogo(escolha);
                        
                    }
                    else if(click(botaoBack2, evento)){
                        iniciaMenu(BotaoHelp, BotaoPlay, BotaoQuit); 
                    }
                }
            }
        }
}

void escolheu(int escolha){
    int x, y;
    switch(escolha){
        case 1: spriteEscolha = al_load_bitmap("Resources/Sprites/menu/escolheu acm.png");
                x = Sprite1.x; y = Sprite1.y;
            break;
        case 2: spriteEscolha = al_load_bitmap("Resources/Sprites/menu/escolheu bombermano.png");
                x = Sprite2.x; y = Sprite2.y;
            break;
        case 3: spriteEscolha = al_load_bitmap("Resources/Sprites/menu/escolheu arnalo.png");
                x = Sprite3.x; y = Sprite3.y;
            break;
        case 4: spriteEscolha = al_load_bitmap("Resources/Sprites/menu/escolheu heitor.png");
                x = Sprite4.x; y = Sprite4.y;
            break;
        case 5: spriteEscolha = al_load_bitmap("Resources/Sprites/menu/escolheu bubu.png");
                x = Sprite5.x; y = Sprite5.y;
            break;
    }
    carregaTelaSkin();
    escolhaFinal(spriteEscolha, x, y);
}

void escolhaFinal(ALLEGRO_BITMAP *spriteEscolha, int x, int y){
    switch(escolha){
        case 0: carregaTelaSkin();
                carregaSkin(); break;
        case 1: carregaTelaSkin();
                al_draw_bitmap(sprite3, Sprite3.x, Sprite3.y, 0);
                al_draw_bitmap(sprite2, Sprite2.x, Sprite2.y, 0);
                al_draw_bitmap(sprite4, Sprite4.x, Sprite4.y, 0);
                al_draw_bitmap(sprite5, Sprite5.x, Sprite5.y, 0);
                al_draw_bitmap(spriteEscolha, (x-5), (y-5), 0);
                al_flip_display();
            break;
        case 2: carregaTelaSkin();
                al_draw_bitmap(sprite1, Sprite1.x, Sprite1.y, 0);
                al_draw_bitmap(sprite3, Sprite3.x, Sprite3.y, 0);
                al_draw_bitmap(sprite4, Sprite4.x, Sprite4.y, 0);
                al_draw_bitmap(sprite5, Sprite5.x, Sprite5.y, 0);
                al_draw_bitmap(spriteEscolha, (x-5), (y-5), 0);
                al_flip_display();
            break;
        case 3: carregaTelaSkin();
                al_draw_bitmap(sprite1, Sprite1.x, Sprite1.y, 0);
                al_draw_bitmap(sprite2, Sprite2.x, Sprite2.y, 0);
                al_draw_bitmap(sprite4, Sprite4.x, Sprite4.y, 0);
                al_draw_bitmap(sprite5, Sprite5.x, Sprite5.y, 0);
                al_draw_bitmap(spriteEscolha, (x-5), (y-5), 0);
                al_flip_display();
            break;
        case 4: carregaTelaSkin();
                al_draw_bitmap(sprite1, Sprite1.x, Sprite1.y, 0);
                al_draw_bitmap(sprite3, Sprite3.x, Sprite3.y, 0);
                al_draw_bitmap(sprite2, Sprite2.x, Sprite2.y, 0);
                al_draw_bitmap(sprite5, Sprite5.x, Sprite5.y, 0);
                al_draw_bitmap(spriteEscolha, (x-5), (y-5), 0);
                al_flip_display();
            break;
        case 5: carregaTelaSkin();
                al_draw_bitmap(sprite1, Sprite1.x, Sprite1.y, 0);
                al_draw_bitmap(sprite2, Sprite2.x, Sprite2.y, 0);
                al_draw_bitmap(sprite3, Sprite3.x, Sprite3.y, 0);
                al_draw_bitmap(sprite4, Sprite4.x, Sprite4.y, 0);
                al_draw_bitmap(spriteEscolha, (x-5), (y-5), 0);
                al_flip_display();
            break;
    }
}

void setButtons(){
    botaoPlay.x=170;
    botaoPlay.y=150;
    botaoPlay.height=50;
    botaoPlay.width=300;
    botaoHelp.x=170;
    botaoHelp.y=215;
    botaoHelp.height=50;
    botaoHelp.width=300;
    botaoQuit.x=170;
    botaoQuit.y=280;
    botaoQuit.height=50;
    botaoQuit.width=300;
    botaoBack.x=170;
    botaoBack.y=420;
    botaoBack.height=50;
    botaoBack.width=300;
    botaoBack2.x=325;//botao back na pag de escolha de skin
    botaoBack2.y=400;
    botaoBack2.height=50;
    botaoBack2.width=300;
    botaoOk.x=15;//botao ok
    botaoOk.y=400;
    botaoOk.height=50;
    botaoOk.width=300;
    Sprite1.x=70;//ACM
    Sprite1.y=200;
    Sprite1.height=60;
    Sprite1.width=80;
    Sprite2.x=170;//BOMBERMANO
    Sprite2.y=200;
    Sprite2.height=60;
    Sprite2.width=80;
    Sprite3.x=270;//ARNALDO
    Sprite3.y=200;
    Sprite3.height=60;
    Sprite3.width=80;
    Sprite4.x=370;//HEITOR
    Sprite4.y=200;
    Sprite4.height=60;
    Sprite4.width=80;
    Sprite5.x=470;//BUBU
    Sprite5.y=200;
    Sprite5.height=60;
    Sprite5.width=80;
}

void iniciaJogo(int escolha){
    recebe.todos[id].skin = escolha; 
    sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
     
    //al_start_timer(timer);
    while(recebe.start!=1)
    {  // al_start_timer(timer);
        esperaPlayer();
        recvMsgFromServer(&recebe, DONT_WAIT);
    }
    
    al_destroy_sample(sample);//TIRA A MUSICA DO MENU;
    timer=al_create_timer(temporizador);
    al_register_event_source(fila_eventos,al_get_timer_event_source(timer));
        
    while(!sair) // COMECA O JOGO
        //startTimer();
        //while(!al_is_event_queue_empty(fila_eventos)){
        while(1){
            if(recebe.todos[id].conect == 0){
                //menuz
                //al_register_event_source(fila_eventos, al_get_mouse_event_source());
                //iniciaMenu(BotaoHelp, BotaoPlay, BotaoQuit);
                exit(1);
                sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                
                perdeu();
                
            }
            if(recebe.todos[id].win == 1){
                telaVitoria();
            }
            al_start_timer(timer);//COMECA TIMER DE 1 FPS (1 DE 60)
            //startTimer();//TIMER DA BIBLIO DE COMUNICACAO
            //al_flush_event_queue(fila_eventos);
            al_wait_for_event(fila_eventos, &evento);//ESPERA UM EVENTO ACONTECER, SE NAO ACONTECER, DEPOIS DE UM FRAME ELE ATUALIZA A TELA
            recvMsgFromServer(&recebe,DONT_WAIT);//RECEBE MENSAGEM DO SERVER, SE NAOR ECEBER NADA, NAO FAZ NADA
            spriteBomba();//DA O SPRITE DA BOMBA
            meuSprite();//DA SPRITE AO PERSONAGEM
            desenhaPosicao(recebe);// DESENHA MAPA
            //FPSLimit();
            if(evento.type == ALLEGRO_EVENT_TIMER){//tempo de bomba   
                for(i=0;i<numMaxBomba;i++){
                  seg[i]+=temporizador;// 0.083 = 5/fps
                  tempExplode[i]+=temporizador;
                }
                for(i=0;i<numMaxBomba;i++){
                    if(tempExplode[i]>=tempDeExplo && recebe.todos[id].minhaBomba[i].ex.explode ==1){   
                        recebe.todos[id].minhaBomba[i].quebra =1;
                        //if(a != numBlocos) recebe.bloquinho[a].ativo=0;// testa se algum bloco estorou
                        recebe.todos[id].minhaBomba[i].quebraCima = quebraCima[i];
                        recebe.todos[id].minhaBomba[i].quebraBaixo = quebraBaixo[i];
                        recebe.todos[id].minhaBomba[i].quebraEsquerda = quebraEsquerda[i];
                        recebe.todos[id].minhaBomba[i].quebraDireita = quebraDireita[i];
                        //if(ba != numBlocos) recebe.bloquinho[ba].ativo=0;// testa se algum bloco estorou
                        //if(bb != numBlocos) recebe.bloquinho[bb].ativo=0;// testa se algum bloco estorou
                        //if(bc != numBlocos) recebe.bloquinho[bc].ativo=0;// testa se algum bloco estorou
                        recebe.todos[id].minhaBomba[i].ativa=0;
                        recebe.todos[id].numBombas++;
                        recebe.todos[id].minhaBomba[i].ex.explode = 0;
                        sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                        recvMsgFromServer(&recebe,DONT_WAIT);//RECEBE MENSAGEM DO SERVER, SE NAOR ECEBER NADA, NAO FAZ NADA
                    }
                    if(seg[i]>=tempoBomba && recebe.todos[id].minhaBomba[i].ativa ==1){
                        //recebe.todos[id].numBombas++;
                        //recebe.todos[id].minhaBomba[i].ativa--;
                        recebe.todos[id].minhaBomba[i].ex.explode = 1;
                        seg[i]=0;
                        tempExplode[i] = 0;
                        sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                        //recebe.todos[id].pos.x = 12;
                    }       
                }
            }
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN){ //FAZ FUNCAO DEPENDENDOD E EVENTO QUE FOI ACIONADO
                switch(evento.keyboard.keycode){
                    case ALLEGRO_KEY_UP://CLICOU CIMA
                        recvMsgFromServer(&recebe,DONT_WAIT);
                        vaiPraCima(&recebe.todos[id]);
                        onde = 1;
                        colisao(&recebe.todos[id], onde);
                        recebe.todos[id].numSprite = 2;
                        sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                        break;  
                    case ALLEGRO_KEY_DOWN:   //CLICOU BAIXO
                        recvMsgFromServer(&recebe,DONT_WAIT);
                        vaiPraBaixo(&recebe.todos[id]);
                        onde = 2;
                        colisao(&recebe.todos[id], onde);
                        recebe.todos[id].numSprite = 1;
                        sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                        break;     
                    case ALLEGRO_KEY_LEFT:  //CLICOU ESQUERDA
                        recvMsgFromServer(&recebe,DONT_WAIT);
                        vaiPraEsq(&recebe.todos[id]);
                        onde = 3;
                        colisao(&recebe.todos[id], onde);
                        //colisaoBloco(&recebe.todos[id], onde);
                        recebe.todos[id].numSprite = 3;
                        sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                        break; 
                    case ALLEGRO_KEY_RIGHT:  //CLICOU DIREITA
                        recvMsgFromServer(&recebe,DONT_WAIT);
                        vaiPraDir(&recebe.todos[id]);
                        onde = 4;
                        colisao(&recebe.todos[id], onde);
                        recebe.todos[id].numSprite = 4;
                        sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                        break;  
                    case ALLEGRO_KEY_Q: //CLICOU Q
                        recvMsgFromServer(&recebe,DONT_WAIT);
                        if(recebe.todos[id].numBombas>0){
                            for(i=0;i<numMaxBomba;i++){
                                if(recebe.todos[id].minhaBomba[i].ativa==0){
                                    seg[i]=0;
                                    recebe.todos[id].numBombas--;
                                    recebe.todos[id].minhaBomba[i].ativa=1;
                                    recebe.todos[id].minhaBomba[i].pos.x = recebe.todos[id].pos.x;
                                    recebe.todos[id].minhaBomba[i].pos.y = recebe.todos[id].pos.y;
                                    recebe.todos[id].minhaBomba[i].quebra =0;
                                    i=numMaxBomba;   // procura bomba nao ativa  
                                }
                            }
                        }
                        sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                        break;

                    case ALLEGRO_KEY_Z:  //CLICOU DIREITA
                        if(id==3){
                            recebe.todos[id].skin = 6;
                        }
                        sendMsgToServer(&recebe.todos[id],sizeof(Personagem));
                        break;

                    case ALLEGRO_KEY_ESCAPE:  
                        encerraJogo();
                        exit (1);      
                }            
            }            
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){ //clica em fechar           
              al_clear_to_color(al_map_rgb(255, 255, 255));
              al_flip_display();
              al_destroy_display(janela);
              exit;        
            }               
        }

        al_clear_to_color(al_map_rgb(255, 255, 255));
        al_flip_display();
        al_rest(5.0);
        al_destroy_display(janela);
}

void telaVitoria(){
    al_clear_to_color(al_map_rgb(0, 100, 255));
    al_draw_text(fonte, al_map_rgb(50, 50, 50),330, 230, ALLEGRO_ALIGN_CENTRE, "VOCÊ GANHOU!!!! :D");
    al_flip_display();
    al_rest(1.0);
    al_play_sample(sampleGanhou, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    al_rest(3.5);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    exit (1);
}