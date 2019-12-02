#include "ACore.h"
#include "server.h"
#include <stdio.h>
#include <string.h>

#define MSG_MAX_SIZE 350
#define BUFFER_SIZE (MSG_MAX_SIZE + 100)
#define LOGIN_MAX_SIZE 13
#define MAX_CHAT_CLIENTS 4
#define numBlocos 99
#define nBombs 4

Personagem recebe;
Terreno envio, salva;
int players=0;
int a;
int desativa;
int inicialX (int id);
int inicialY (int id);
int desconectado[4] = {0,0,0,0};
int inicia =0;
int malu = 0;

void preencheBlocos();

int main() {
  char client_names[MAX_CHAT_CLIENTS][LOGIN_MAX_SIZE];
  char str_buffer[BUFFER_SIZE], aux_buffer[BUFFER_SIZE];
  int i,j,ok;
 envio.start =0;
  preencheBlocos();
//envio.start=1;
  for(i=0;i<numBlocos;i++){
    envio.bloquinho[i].ativo=1;
  }
  //preenche powerups aleatoriamente
  for(i=0;i<numPowerUps;i++){
    envio.upgrades[i].ativo = 1;
  }
  srand(time(NULL));
  for(i=0;i<numPowerUps;i++){
    envio.upgrades[i].qualUp=rand()%2;
    envio.upgrades[i].pos.x=rand()%13;
    envio.upgrades[i].pos.y=rand()%13;
    if(envio.upgrades[i].pos.x%2 !=0 && envio.upgrades[i].pos.y%2!=0){
      i--;
    }
    else{
      ok=0;
      for(j=0;j<numBlocos;j++){
        if(envio.upgrades[i].pos.x == envio.bloquinho[j].pos.x && envio.upgrades[i].pos.y == envio.bloquinho[j].pos.y)
          ok=1;
      }
      if(ok==0){i--;}
    }
  }
  ///envio.bloquinho[0].pos.x = 2;
  //envio.bloquinho[0].pos.x = 2;
  //sendMsgToServer
  for(i=0;i<4;i++){
    envio.todos[i].conect=0;
    envio.todos[i].ok=0;
  }
  for(a=0;a<4;a++){
    for(i=0;i<4;i++){
      envio.todos[a].minhaBomba[i].ativa = 0;
      envio.todos[a].minhaBomba[i].ex.explode = 0;
      envio.todos[a].minhaBomba[i].quebra =0;
    }
  }
  serverInit(MAX_CHAT_CLIENTS);
  puts("Server is running!!");
  
  while(1){
    int id = acceptConnection();
    if(id != NO_CONNECTION){
      recvMsgFromClient(client_names[id], id, WAIT_FOR_IT);
      strcpy(str_buffer, client_names[id]);
      strcat(str_buffer, " connected to chat");
      //broadcast(str_buffer, (int)strlen(str_buffer) + 1);
      printf("%s connected id = %d\n", client_names[id], id);
      //for(i=0;i<id;i++)
      //envio.todos[i] = salva.todos[i];
      //envio = salva;
      desconectado[id]=1;
      envio.todos[id].conect=1;
      envio.todos[id].pos.x = inicialX(id);
      envio.todos[id].pos.y = inicialY(id);
      envio.todos[id].skin = 1;
      envio.todos[id].numSprite = 1;
      envio.todos[id].numBombas = 1;
      envio.todos[id].potencia = 2;
      envio.todos[id].win=0;
      players++;
      for(a=0;a<nBombs;a++){ //ZERA TODAS AS BOMBAS INICIALMENTE
        envio.todos[id].minhaBomba[a].quebraCima = numBlocos;
        envio.todos[id].minhaBomba[a].quebraBaixo = numBlocos;
        envio.todos[id].minhaBomba[a].quebraEsquerda = numBlocos;
        envio.todos[id].minhaBomba[a].quebraDireita = numBlocos;
      }    
      //salva.todos[id]=envio.todos[id];
      sendMsgToClient(&id, sizeof(int), id);
      //sendMsgToClient(&envio, sizeof(Terreno), id);
      //if(id==1)
      broadcast(&envio,sizeof(Terreno));
    }
  
  struct msg_ret_t msg_ret = recvMsg(&recebe);//RECEBE MENSAGEM
  
    if(msg_ret.status == MESSAGE_OK && inicia==0){ 
      envio.todos[msg_ret.client_id] = recebe;

      switch(players){
        case 1:
        case 2:
        if(envio.todos[0].ok==1 && envio.todos[1].ok==1){
        inicia=1;
        }
        break;
        
        case 3:
        if(envio.todos[0].ok==1 && envio.todos[1].ok==1 && envio.todos[2].ok==1){
        inicia=1;
        }
        break;
        case 4:
        if(envio.todos[0].ok==1 && envio.todos[1].ok==1 && envio.todos[2].ok==1 && envio.todos[3].ok==1 ){
        inicia=1;
        }
        break;
      }


      if(inicia == 1){
        envio.start=1;
        //broadcast(&envio,sizeof(Terreno));
      } 
       broadcast(&envio,sizeof(Terreno)); 
    }  
    //inicia=1; 
    //struct msg_ret_t msg_ret = recvMsg(aux_buffer);
    if(msg_ret.status == MESSAGE_OK){
      envio.todos[msg_ret.client_id] = recebe;//POE A MENSAGEM DO CLIENTE NA ESTRUTURA;
      if(envio.todos[msg_ret.client_id].conect==0){ // SE CLIENT MORREU, DISCONECTA ELE;(NAO TA SERVINDO PRA NADA)
        //envio.todos[msg_ret.client_id].conect=0;
        envio.todos[msg_ret.client_id].conect=0;
        envio.todos[msg_ret.client_id].pos.x = inicialX(id);
        envio.todos[msg_ret.client_id].pos.y = inicialY(id);
        envio.todos[msg_ret.client_id].skin = 1;
        envio.todos[msg_ret.client_id].numSprite = 1;
        envio.todos[msg_ret.client_id].numBombas = 1;
        envio.todos[msg_ret.client_id].potencia = 2;
        envio.todos[msg_ret.client_id].win=0;
      
      for(i=0;i<4;i++){
        envio.todos[msg_ret.client_id].minhaBomba[i].ativa = 0;
        envio.todos[msg_ret.client_id].minhaBomba[i].ex.explode = 0;
        envio.todos[msg_ret.client_id].minhaBomba[i].quebra =0;
        envio.todos[msg_ret.client_id].minhaBomba[i].ex.explode = 0;
      }
        
        //broadcast(&envio, sizeof(Terreno));
      }
   //*******************************************************  
     //testa se explodiu alguma coisa
      for(a=0;a<nBombs;a++){
        if(envio.todos[msg_ret.client_id].minhaBomba[a].quebraCima!=numBlocos && envio.todos[msg_ret.client_id].minhaBomba[a].quebra==1){ //TESTA SE A BOMBA QUEBROU ALGUMA COISA
          envio.bloquinho[envio.todos[msg_ret.client_id].minhaBomba[a].quebraCima].ativo=0;
          envio.todos[msg_ret.client_id].minhaBomba[a].quebraCima = numBlocos;
        } 
        
        if(envio.todos[msg_ret.client_id].minhaBomba[a].quebraBaixo!=numBlocos && envio.todos[msg_ret.client_id].minhaBomba[a].quebra==1){
          envio.bloquinho[envio.todos[msg_ret.client_id].minhaBomba[a].quebraBaixo].ativo=0;
          envio.todos[msg_ret.client_id].minhaBomba[a].quebraBaixo = numBlocos;
        } 

        if(envio.todos[msg_ret.client_id].minhaBomba[a].quebraEsquerda!=numBlocos && envio.todos[msg_ret.client_id].minhaBomba[a].quebra==1){
          envio.bloquinho[envio.todos[msg_ret.client_id].minhaBomba[a].quebraEsquerda].ativo=0;
          envio.todos[msg_ret.client_id].minhaBomba[a].quebraEsquerda = numBlocos;
        } 

        if(envio.todos[msg_ret.client_id].minhaBomba[a].quebraDireita!=numBlocos && envio.todos[msg_ret.client_id].minhaBomba[a].quebra==1){
          envio.bloquinho[envio.todos[msg_ret.client_id].minhaBomba[a].quebraDireita].ativo=0;
          envio.todos[msg_ret.client_id].minhaBomba[a].quebraDireita = numBlocos;
        } 
     }
//*****************************************************************************************
     for(i=0;i<numPowerUps;i++){
        if(envio.todos[msg_ret.client_id].pos.x ==  envio.upgrades[i].pos.x &&envio.todos[msg_ret.client_id].pos.y==  envio.upgrades[i].pos.y){
          if(envio.upgrades[i].ativo==1){
            envio.upgrades[i].ativo = 0;
            if(envio.upgrades[i].qualUp == 0){
              envio.todos[msg_ret.client_id].potencia++;
            }
            else{
              envio.todos[msg_ret.client_id].numBombas++;
              if(envio.todos[msg_ret.client_id].numBombas>4){
                envio.todos[msg_ret.client_id].numBombas=4;
              }
            }
          }
        }
      }

      broadcast(&envio, sizeof(Terreno));
    } 
    else if (msg_ret.status == DISCONNECT_MSG) {
        sprintf(str_buffer, "%s disconnected", client_names[msg_ret.client_id]);
        printf("%s disconnected, id = %d is free\n",
        client_names[msg_ret.client_id], msg_ret.client_id);
        players--;
        if(players == 1){
          for(i=0;i<4;i++)
          {
            envio.todos[i].win = 1;
          }
        }

        
        broadcast(&envio, sizeof(Terreno));
        //broadcast(str_buffer, (int)strlen(str_buffer) + 1);
    }
  }
}

int inicialX(int id){ 
  int retorna;
  switch(id){
    case 0:
      retorna = 0;
      break;
    case 1:
      retorna = 12;
      break;
    case 2:
      retorna = 12;
      break;
    case 3:
      retorna = 0;
      break;
    }   
  return retorna;
}

int inicialY(int id){ 
  int retorna;
  switch(id){
    case 0:
      retorna = 0;
      break;
    case 1:
      retorna = 12;
      break;
    case 2:
      retorna = 0;
      break;
    case 3:
      retorna = 12;
      break;
    }   
  return retorna;
}

void preencheBlocos(){
  //PREENCHE LINHA 0 DA MATRIZ
  envio.bloquinho[0].pos.x = 2;
  envio.bloquinho[0].pos.y = 0;

  envio.bloquinho[1].pos.x = 3;
  envio.bloquinho[1].pos.y = 0;


  envio.bloquinho[2].pos.x = 4;
  envio.bloquinho[2].pos.y = 0;


  envio.bloquinho[3].pos.x = 7;
  envio.bloquinho[3].pos.y = 0;


  envio.bloquinho[4].pos.x = 9;
  envio.bloquinho[4].pos.y = 0;


  envio.bloquinho[5].pos.x = 10;
  envio.bloquinho[5].pos.y = 0;
// **************************************
//PREENCHE LINHA 1 DA MATRIZ
  envio.bloquinho[6].pos.x = 4;
  envio.bloquinho[6].pos.y = 1;
  
  envio.bloquinho[7].pos.x = 6;
  envio.bloquinho[7].pos.y = 1;

  envio.bloquinho[8].pos.x = 8;
  envio.bloquinho[8].pos.y = 1;

  envio.bloquinho[9].pos.x = 10;
  envio.bloquinho[9].pos.y = 1;
  
//************************************
//PREENCHE LINHA 2 DA MATRIZ

  envio.bloquinho[10].pos.x = 0;
  envio.bloquinho[10].pos.y = 2;

  envio.bloquinho[11].pos.x = 1;
  envio.bloquinho[11].pos.y = 2;

  envio.bloquinho[12].pos.x = 3;
  envio.bloquinho[12].pos.y = 2;

  envio.bloquinho[13].pos.x = 4;
  envio.bloquinho[13].pos.y = 2;

  envio.bloquinho[14].pos.x = 5;
  envio.bloquinho[14].pos.y = 2;

  envio.bloquinho[15].pos.x = 6;
  envio.bloquinho[15].pos.y = 2;

  envio.bloquinho[16].pos.x = 7;
  envio.bloquinho[16].pos.y = 2;

  envio.bloquinho[17].pos.x = 10;
  envio.bloquinho[17].pos.y = 2;

//***********************************
//PREENCHE LINHA 3 DA MATRIZ
  envio.bloquinho[18].pos.x = 4;
  envio.bloquinho[18].pos.y = 3;
  
  envio.bloquinho[19].pos.x = 10;
  envio.bloquinho[19].pos.y = 3;

  envio.bloquinho[20].pos.x = 12;
  envio.bloquinho[20].pos.y = 3;

//************************************

//PREENCHE LINHA 4 DA MATRIZ
  envio.bloquinho[21].pos.x = 0;
  envio.bloquinho[21].pos.y = 4;

  envio.bloquinho[22].pos.x = 1;
  envio.bloquinho[22].pos.y = 4;

  envio.bloquinho[23].pos.x = 2;
  envio.bloquinho[23].pos.y = 4;

  envio.bloquinho[24].pos.x = 3;
  envio.bloquinho[24].pos.y = 4;

  envio.bloquinho[25].pos.x = 4;
  envio.bloquinho[25].pos.y = 4;

  envio.bloquinho[26].pos.x = 5;
  envio.bloquinho[26].pos.y = 4;

  envio.bloquinho[27].pos.x = 7;
  envio.bloquinho[27].pos.y = 4;

  envio.bloquinho[28].pos.x = 8;
  envio.bloquinho[28].pos.y = 4;

  envio.bloquinho[29].pos.x = 9;
  envio.bloquinho[29].pos.y = 4;

  envio.bloquinho[30].pos.x = 10;
  envio.bloquinho[30].pos.y = 4;

  envio.bloquinho[31].pos.x = 11;
  envio.bloquinho[31].pos.y = 4;

  envio.bloquinho[32].pos.x = 12;
  envio.bloquinho[32].pos.y = 4;
//*************************************
//PREENCHE LINHA 5 DA MATRIZ
  envio.bloquinho[33].pos.x = 0;
  envio.bloquinho[33].pos.y = 5;

  envio.bloquinho[34].pos.x = 2;
  envio.bloquinho[34].pos.y = 5;

  envio.bloquinho[35].pos.x = 4;
  envio.bloquinho[35].pos.y = 5;

  envio.bloquinho[36].pos.x = 6;
  envio.bloquinho[36].pos.y = 5;

  envio.bloquinho[37].pos.x = 8;
  envio.bloquinho[37].pos.y = 5;
  
  envio.bloquinho[38].pos.x = 10;
  envio.bloquinho[38].pos.y = 5;
  
  envio.bloquinho[39].pos.x = 12;
  envio.bloquinho[39].pos.y = 5;

//**********************************
//PREENCHE LINHA 6
  envio.bloquinho[40].pos.x = 0;
  envio.bloquinho[40].pos.y = 6;

  envio.bloquinho[41].pos.x = 1;
  envio.bloquinho[41].pos.y = 6;

  envio.bloquinho[42].pos.x = 2;
  envio.bloquinho[42].pos.y = 6;

  envio.bloquinho[43].pos.x = 3;
  envio.bloquinho[43].pos.y = 6;

  envio.bloquinho[44].pos.x = 4;
  envio.bloquinho[45].pos.y = 6;

  envio.bloquinho[46].pos.x = 5;
  envio.bloquinho[46].pos.y = 6;

  envio.bloquinho[47].pos.x = 6;
  envio.bloquinho[47].pos.y = 6;

  envio.bloquinho[48].pos.x = 7;
  envio.bloquinho[48].pos.y = 6;

  envio.bloquinho[49].pos.x = 8;
  envio.bloquinho[49].pos.y = 6;

  envio.bloquinho[50].pos.x = 10;
  envio.bloquinho[50].pos.y = 6;

  envio.bloquinho[51].pos.x = 12;
  envio.bloquinho[51].pos.y = 6;
//**********************************
//PREENCHE A LINHA 7 DA MATRIZ

  envio.bloquinho[52].pos.x = 0;
  envio.bloquinho[52].pos.y = 7;

  envio.bloquinho[53].pos.x = 2;
  envio.bloquinho[53].pos.y = 7;

  envio.bloquinho[54].pos.x = 4;
  envio.bloquinho[54].pos.y = 7;

  envio.bloquinho[55].pos.x = 6;
  envio.bloquinho[55].pos.y = 7;

  envio.bloquinho[56].pos.x = 8;
  envio.bloquinho[56].pos.y = 7;

  envio.bloquinho[57].pos.x = 12;
  envio.bloquinho[57].pos.y = 7;

//**********************************
//PREENCHE LINHA 8 DA MATRIZ
  envio.bloquinho[58].pos.x = 0;
  envio.bloquinho[58].pos.y = 8;

  envio.bloquinho[59].pos.x = 1;
  envio.bloquinho[59].pos.y = 8;

  envio.bloquinho[60].pos.x = 3;
  envio.bloquinho[60].pos.y = 8;

  envio.bloquinho[61].pos.x = 4;
  envio.bloquinho[61].pos.y = 8;

  envio.bloquinho[62].pos.x = 5;
  envio.bloquinho[62].pos.y = 8;

  envio.bloquinho[63].pos.x = 6;
  envio.bloquinho[63].pos.y = 8;

  envio.bloquinho[64].pos.x = 7;
  envio.bloquinho[64].pos.y = 8;

  envio.bloquinho[65].pos.x = 8;
  envio.bloquinho[65].pos.y = 8;

  envio.bloquinho[66].pos.x = 10;
  envio.bloquinho[66].pos.y = 8;

  envio.bloquinho[67].pos.x = 11;
  envio.bloquinho[67].pos.y = 8;

  envio.bloquinho[68].pos.x = 12;
  envio.bloquinho[68].pos.y = 8;

//*************************************
//PREENCHE LINHA 9 MATRIZ
  envio.bloquinho[69].pos.x = 2;
  envio.bloquinho[69].pos.y = 9;

  envio.bloquinho[70].pos.x = 4;
  envio.bloquinho[70].pos.y = 9;

  envio.bloquinho[71].pos.x = 6;
  envio.bloquinho[72].pos.y = 9;

  envio.bloquinho[73].pos.x = 8;
  envio.bloquinho[73].pos.y = 9;

  envio.bloquinho[74].pos.x = 10;
  envio.bloquinho[74].pos.y = 9;

  envio.bloquinho[75].pos.x = 12;
  envio.bloquinho[75].pos.y = 9;

//*************************************
//PREENCHE LINHA 10
  
  envio.bloquinho[76].pos.x = 0;
  envio.bloquinho[76].pos.y = 10;

  envio.bloquinho[77].pos.x = 1;
  envio.bloquinho[77].pos.y = 10;

  envio.bloquinho[78].pos.x = 2;
  envio.bloquinho[78].pos.y = 10;

  envio.bloquinho[79].pos.x = 3;
  envio.bloquinho[79].pos.y = 10;

  envio.bloquinho[80].pos.x = 4;
  envio.bloquinho[80].pos.y = 10;

  envio.bloquinho[81].pos.x = 5;
  envio.bloquinho[81].pos.y = 10;

  envio.bloquinho[82].pos.x = 6;
  envio.bloquinho[82].pos.y = 10;

  envio.bloquinho[83].pos.x = 7;
  envio.bloquinho[83].pos.y = 10;

  envio.bloquinho[84].pos.x = 8;
  envio.bloquinho[84].pos.y = 10;

  envio.bloquinho[85].pos.x = 9;
  envio.bloquinho[85].pos.y = 10;

  envio.bloquinho[86].pos.x = 12;
  envio.bloquinho[86].pos.y = 10;


//**************************************
//PREENCHE LINHA 11 DA MATRIZ
  envio.bloquinho[87].pos.x = 2;
  envio.bloquinho[87].pos.y = 11;

  envio.bloquinho[88].pos.x = 4;
  envio.bloquinho[88].pos.y = 11;

  envio.bloquinho[89].pos.x = 6;
  envio.bloquinho[89].pos.y = 11;

  envio.bloquinho[90].pos.x = 10;
  envio.bloquinho[90].pos.y = 11;


//***************************************
//PREENCHE LINHA 12 DA MATRIZ
  envio.bloquinho[91].pos.x = 2;
  envio.bloquinho[91].pos.y = 12;

  envio.bloquinho[92].pos.x = 4;
  envio.bloquinho[92].pos.y = 12;

  envio.bloquinho[93].pos.x = 5;
  envio.bloquinho[93].pos.y = 12;

  envio.bloquinho[94].pos.x = 6;
  envio.bloquinho[94].pos.y = 12;

  envio.bloquinho[95].pos.x = 7;
  envio.bloquinho[95].pos.y = 12;

  envio.bloquinho[96].pos.x = 8;
  envio.bloquinho[96].pos.y = 12;

  envio.bloquinho[97].pos.x = 9;
  envio.bloquinho[97].pos.y = 12;

  envio.bloquinho[98].pos.x = 10;
  envio.bloquinho[98].pos.y = 12;

//***************************************
}