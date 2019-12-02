#include "ACore.h"


/*
INITIALIZATION OF BASIC CORE MODULES:
--ALLEGRO
--ALLEGRO IMAGE
--ALLEGRO FONTS
--ALLEGRO TTF FONT SUPPORT
--ALLEGRO PRIMITIVES
--EVENT QUEUE
*/
bool coreInit()
{
    //modules and add-ons initialization
	if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return false;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return false;
    }

    if (!al_init_font_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_font.\n");
        return false;
    }

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return false;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_primitives.\n");
        return false;
    }
    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        return false;
    }
    eventsQueue = al_create_event_queue();
    if (!eventsQueue)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        return false;
    }

    fila_bomba = al_create_event_queue();
    if(!fila_bomba)
    {
        fprintf(stderr, "Falha ao criar fila de bomba");
        return false;
    }

    if(!al_install_audio()){
      fprintf(stderr, "failed to initialize audio!\n");
      return -1;
   }

    if(!al_init_acodec_addon()){
      fprintf(stderr, "failed to initialize audio codecs!\n");
      return -1;
   }

    if (!al_reserve_samples(1)){
      fprintf(stderr, "failed to reserve samples!\n");
      return -1;
   }
 	

    return true;
}


//FOR INITIALIZING A WINDOW OF WxH SIZE WITH TEXT "title[]"
bool windowInit(int W, int H, char title[])
{   
    janela = al_create_display(W, H);
    
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }
    al_set_window_title(main_window, title);

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

   /* //window and event queue creation
    main_window = al_create_display(W, H);
    if (!main_window)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }
    al_set_window_title(main_window, title);

    //registra janela na fila de eventos
    al_register_event_source(eventsQueue, al_get_display_event_source(main_window));
*/
    return true;
}


//FOR INITIALIZING MAIN EXTERNAL INPUTS (KEYBOARD, MOUSE AND CURSOR)
bool inputInit()
{
	/*------------------------------MOUSE--------------------------------*/
	//Inicializa Mouse
	if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        //al_destroy_display(main_window);
        al_destroy_display(janela);
        return false;
    }

    // Atribui o cursor padrão do sistema para ser usado
    
    /*if (!al_set_system_mouse_cursor(main_window, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(main_window);
        return false;
    }*/
    /*------------------------------MOUSE--------------------------------*/

    /*------------------------------KEYBOARD------------------------------*/
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    /*------------------------------KEYBOARD------------------------------*/


    //Registra mouse e teclado na fila de eventos
    al_register_event_source(eventsQueue, al_get_mouse_event_source());
    al_register_event_source(eventsQueue, al_get_keyboard_event_source());

    return true;
}


//FOR READING KEYBOARD INPUT WITH MAX SIZE = LIMIT AND SAVING AT STR[]
void readInput(ALLEGRO_EVENT event, char str[], int limit)
{
    if (event.type == ALLEGRO_EVENT_KEY_CHAR)
    {
        if (strlen(str) <= limit)
        {
            char temp[] = {event.keyboard.unichar, '\0'};
            if (event.keyboard.unichar == ' ')
            {
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= '!' &&
                     event.keyboard.unichar <= '?')
            {
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= 'A' &&
                     event.keyboard.unichar <= 'Z')
            {
                strcat(str, temp);
            }
            else if (event.keyboard.unichar >= 'a' &&
                     event.keyboard.unichar <= 'z')
            {
                strcat(str, temp);
            }
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0)
        {
            str[strlen(str) - 1] = '\0';
        }
    }
}


//FPS CONTROL (FPS IS DEFINED IN ACORE.H)
void startTimer()
{
    startingTime = al_get_time();
}

double getTimer()
{
    return al_get_time() - startingTime;
}

void FPSLimit()
{
    if (getTimer() < 1.0/FPS)
    {
        al_rest((1.0 / FPS) - getTimer());
    }
}


//FOR DEALLOCATING ALL ALLEGRO STUFF
void allegroEnd()
{
    al_destroy_display(main_window);
    al_destroy_event_queue(eventsQueue);
}


//MODIFY THIS TO LOAD YOUR OWN FONTS (FONT POINTERS ARE DEFINED AT ACORE.H)
bool fontInit()
{
    /*------------------------------FONTE--------------------------------*/
    start = al_load_font("examples/graphicChat/Resources/Fonts/pressStart.ttf", 16, 0);
    if (!start)
    {
        fprintf(stderr, "Falha ao carregar \"examples/graphicChat/Resources/Fonts/pressStart.ttf\".\n");
        return false;
    }

    ubuntu = al_load_font("examples/graphicChat/Resources/Fonts/Ubuntu-R.ttf", 32, 0);
    if (!ubuntu)
    {
        fprintf(stderr, "Falha ao carregar \"Ubuntu-R.ttf\".\n");
        return false;
    }

    return true;
}


//MODIFY THIS TO LOAD YOUR OWN GRAPHICS (BITMAP POINTERS ARE DEFINED AT ACORE.H)
bool loadGraphics()
{
    /*menuScreen = al_load_bitmap("examples/graphicChat/Resources/Etc/titleScreen2.png");
    if (!menuScreen)
    {
        fprintf(stderr, "Falha carregando menuScreen\n");
        return false;
    }*/

   /* objects = al_load_bitmap("examples/graphicChat/Resources/Tilesets/objects.png");
    if (!objects){
        fprintf(stderr, "Falha carregando objects.png\n");
        return false;
    }
    */
    mapa = al_load_bitmap("Resources/Sprites/mapinha1.jpg");
    if (!mapa){
        fprintf(stderr, "Falha carregando mapinha1.jpg\n");
        return false;
    }

//***********************************************************************
    cima = al_load_bitmap("Resources/Sprites/bombermano/cima.png");
     if (!cima){
        fprintf(stderr, "Falha carregando cima.png\n");
        return false;
    }

    baixo = al_load_bitmap("Resources/Sprites/bombermano/baixo.png");
     if (!baixo){
        fprintf(stderr, "Falha carregando baixo.png\n");
        return false;
    }

    esquerda = al_load_bitmap("Resources/Sprites/bombermano/esquerda.png");
    if (!esquerda){
        fprintf(stderr, "Falha carregando esquerda.png\n");
        return false;
    }
    direita = al_load_bitmap("Resources/Sprites/bombermano/direita.png");
    if (!direita){
        fprintf(stderr, "Falha carregando direita.png\n");
        return false;
    }

//***********************************************************************
     cima2 = al_load_bitmap("Resources/Sprites/arnaldo/B - cima 2.png");
    if(!cima2){
        fprintf(stderr, "Falha carregando B - cima 2.png");
    }

    baixo2 = al_load_bitmap("Resources/Sprites/arnaldo/B - baixo 2.png");
    if(!baixo2){
        fprintf(stderr, "Falha carregando b - baixo 2.png");
    }
    
     esquerda2 = al_load_bitmap("Resources/Sprites/arnaldo/B - left 4.png");
    if(!esquerda2){
        fprintf(stderr, "Falha carregando B - left 4.png");
    }

     direita2 = al_load_bitmap("Resources/Sprites/arnaldo/B - right 4.png");
    if(!direita2){
        fprintf(stderr, "Falha carregando B - right 4.png");
    }

 
//***********************************************************************  
     cima3 = al_load_bitmap("Resources/Sprites/ACM/A - cima 1.png");
    if(!cima3){
        fprintf(stderr, "Falha carregando A - cima 1.png");
    }

    baixo3 = al_load_bitmap("Resources/Sprites/ACM/A - baixo 2.png");
    if(!baixo3){
        fprintf(stderr, "Falha carregando A - baixo 2.png");
    }
    
     esquerda3 = al_load_bitmap("Resources/Sprites/ACM/A - left 4.png");
    if(!esquerda3){
        fprintf(stderr, "Falha carregando A - left 4.png");
    }

     direita3 = al_load_bitmap("Resources/Sprites/ACM/A - direita 2.png");
    if(!direita3){
        fprintf(stderr, "Falha carregando A - direita 2.png");
    }   

 
//***********************************************************************        
    cima4 = al_load_bitmap("Resources/Sprites/bubu/D - cima.png");
    if(!cima4){
        fprintf(stderr, "Falha carregando D - cima.png");
    }

    baixo4 = al_load_bitmap("Resources/Sprites/bubu/D - baixo.png");
    if(!baixo4){
        fprintf(stderr, "Falha carregando D - baixo.png");
    }
    
     esquerda4 = al_load_bitmap("Resources/Sprites/bubu/D - left.png");
    if(!esquerda4){
        fprintf(stderr, "Falha carregando D - left.png");
    }

     direita4 = al_load_bitmap("Resources/Sprites/bubu/D - right.png");
    if(!direita4){
        fprintf(stderr, "Falha carregando D - right.png");
    } 


//***********************************************************************
    cima5 = al_load_bitmap("Resources/Sprites/heitor/C - cima.png");
    if(!cima5){
        fprintf(stderr, "Falha carregando C - cima.png");
    }

    baixo5 = al_load_bitmap("Resources/Sprites/heitor/C - baixo.png");
    if(!baixo5){
        fprintf(stderr, "Falha carregando C - baixo.png");
    }
    
     esquerda5 = al_load_bitmap("Resources/Sprites/heitor/C - left.png");
    if(!esquerda5){
        fprintf(stderr, "Falha carregando C - left.png");
    }

     direita5 = al_load_bitmap("Resources/Sprites/heitor/C - right.png");
    if(!direita5){
        fprintf(stderr, "Falha carregando C - right.png");
    } 
  //***********************************************************************
  
    cima6 = al_load_bitmap("Resources/Sprites/segredo/cimaCavalo.png");
    if(!cima6){
        fprintf(stderr, "Falha carregando segredoCima.png");
    }

    baixo6 = al_load_bitmap("Resources/Sprites/segredo/baixoCavalo.png");
    if(!baixo6){
        fprintf(stderr, "Falha carregando segredobaixo.png");
    }
    
     esquerda6 = al_load_bitmap("Resources/Sprites/segredo/esquerdaCavalo.png");
    if(!esquerda6){
        fprintf(stderr, "Falha carregando segredo esquerda.png");
    }

     direita6 = al_load_bitmap("Resources/Sprites/segredo/direitaCavalo.png");
    if(!direita6){
        fprintf(stderr, "Falha carregando segredoDireita.png");
    } 

//***********************************************************************

    foguinho = al_load_bitmap("Resources/Sprites/powerUps/increase blast.png");
    if (!foguinho){
        fprintf(stderr, "Falha carregando foguinho fogoso.png\n");
        return false;
    }   

    maisBomba = al_load_bitmap("Resources/Sprites/powerUps/increase bombs.png");
    if (!maisBomba){
        fprintf(stderr, "Falha carregando spriteBomb.png\n");
        return false;
    }

    spriteBomb = al_load_bitmap("Resources/Sprites/bomb.png");
     if (!spriteBomb){
        fprintf(stderr, "Falha carregando spriteBomb.png\n");
        return false;
    }

    blocos = al_load_bitmap("Resources/Sprites/bloco.png");
    if(!blocos) {
        fprintf(stderr, "Falha carregando sprite bloco.png\n");
    }

    explodeCenter = al_load_bitmap("Resources/Sprites/explodeCentro.png");
    if(!explodeCenter){
        fprintf(stderr, "Falha carregando sprite explodeCentro.png\n");
    }

    explodeLado = al_load_bitmap("Resources/Sprites/estouraLado.png");
    if(!explodeLado){
        fprintf(stderr, "Falha carregando sprite estouraLado.png\n");
    }

    explodeCima = al_load_bitmap("Resources/Sprites/estouraCima.png");
    if(!explodeCima){
        fprintf(stderr, "Falha carregando sprite estouraCima.png\n");
    }

    blocoExplod = al_load_bitmap("Resources/Sprites/blocoEstouradoA.png");
    if(!blocoExplod){
        fprintf(stderr, "Falha carregando sprite blocoEstouradoA.png");
    }

    menu = al_load_bitmap("Resources/Sprites/menu/menu.jpeg");
    helpScreen = al_load_bitmap("Resources/Sprites/menu/help.png");
	fonte = al_load_font("Resources/Sprites/menu/pressStart.ttf", 20, 0);
    texto = al_load_font("Resources/Sprites/menu/pressStart.ttf", 15, 0);
    sprite1 = al_load_bitmap("Resources/Sprites/menu/A - baixo.png");
    sprite2 = al_load_bitmap("Resources/Sprites/menu/baixo.png");
    sprite3 = al_load_bitmap("Resources/Sprites/menu/B - right 4.png");
    sprite4 = al_load_bitmap("Resources/Sprites/menu/C - baixo.png");
    sprite5 = al_load_bitmap("Resources/Sprites/menu/D - baixo.png");
    BotaoBack = al_load_bitmap("Resources/Sprites/menu/Return.png");
    BotaoOk = al_load_bitmap("Resources/Sprites/menu/botaoOk.png");
    //fila_eventos = al_create_event_queue();
    spriteCursor = al_load_bitmap("Resources/Sprites/menu/pointer.png");
    
    sample = al_load_sample("Resources/audio/aaa.wav");
    if (!sample){
      printf("Audio clip sample not loaded!\n"); 
      return -1;
    }
    
    sampleExplode = al_load_sample("Resources/audio/explosivo.wav");
    if (!sampleExplode){
      printf("Audio clip sample not loaded!\n"); 
      return -1;
    }
    
      
    sampleGanhou = al_load_sample("Resources/audio/ganhou.wav");
    if (!sampleGanhou){
      printf("Audio clip sample not loaded!\n"); 
      return -1;
    }
    
    samplePerdeu = al_load_sample("Resources/audio/perdeu.wav");
    if (!samplePerdeu){
      printf("Audio clip sample not loaded!\n"); 
      return -1;
    }
    return true;
}