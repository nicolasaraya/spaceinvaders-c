#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#define ALIEN '@'
#define BULLETS '.'

typedef struct{
  int x;
  int y;
  int violencia;
  char figura; 
}ALIENS;
 
typedef struct{
  int x;
  int y;
  char figura;    
}BALAS;

typedef struct{
  int x;
  int y;
  char figura;    
}ESCUDOS;

typedef struct {
  int ptos;
  char name[3];
}PUNTAJES;

//funcion para que no cruce limts cursor
int limitesCursor(int y, int x){
  int ymax, xmax;
  getmaxyx(stdscr, ymax, xmax);
  
  if(y >= 0 && y <= ymax - 1 && x >= 0 && x <= xmax - 1){
    return (1);
  }
  return (0);
}
BALAS* UbicarBalas(int n, int m, ALIENS matriz[n][m], BALAS* bullets){
  int z = 0;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      if(matriz[i][j].figura != ' '){
        matriz[i][j].violencia = rand() % 2;
      }
      if(matriz[i][j].figura == ' '){
        matriz[i][j].violencia = 0;
      }
    }     
  }
  for(int i = 0; i < n ; i++){
    for(int j = 0; j < m; j++){
      bullets[z].x = matriz[i][j].x;  
      bullets[z].y = matriz[i][j].y;
      if(matriz[i][j].violencia == 1){
        bullets[z].figura = BULLETS;
        z++;
      }
      bullets[z].figura = ' ';
      z++;  
    }
  }
  return(bullets);
}
BALAS* UbicarBalasJEFE(BALAS* bullet, int n, ALIENS jefe[n]) {
  if(jefe[1].figura != ' '){
    bullet[0].x = jefe[1].x;
    bullet[0].y = jefe[1].y;
    bullet[0].figura = '*';
  }
  return(bullet);
}
void MovBalas(BALAS* bullets, int numBalas){
  for (int i = 0; i < numBalas; i++) {
    if(bullets[i].y != -1){
      bullets[i].y += 1;        
    } 
  }             
}
void UbicaJefe(int n, ALIENS boss[n], int aparicion){
  int ymax, xmax, cte = 0;
  getmaxyx(stdscr, ymax, xmax);
  
  if(aparicion == 0){
    for(int i = 0; i < 9; i++){
      boss[i].figura = ' ';
    }
  } 
  else {
    boss[0].figura = '(';
    boss[1].figura = '<';
    boss[2].figura = '*';
    boss[3].figura = '>';
    boss[4].figura = '.';
    boss[5].figura = '<';
    boss[6].figura = '*';
    boss[7].figura = '>';
    boss[8].figura = ')';
  }

  for(int i = 0; i < 9; i++){
    if(i == 4){
      boss[i].violencia = 1;
    }
    boss[i].violencia = 0;
    boss[i].x = xmax/2 + cte;
    cte += 1;
    boss[i].y = ymax/6;
  }
}
void UbicarEscudos(int n, ESCUDOS shield[n], int nivel){
  int ymax, xmax, cte = 0;
  getmaxyx(stdscr, ymax, xmax);

  for(int i = 0; i < 49; i++){
    shield[i].x = xmax/4 + cte;
    cte += 1;
    shield[i].y = ymax - 5; 
    shield[i].figura = '_';
    if(nivel == 2){
      if((i >= 9 && i <= 16) || (i >= 25 && i <= 32) || (i>= 41 && i<= 48)){
        shield[i].figura = ' ';
      }
    }
    if(nivel == 3){
      shield[i].figura = ' ';
    }
  }
}
void UbicarAliensGrande(int n, int m, ALIENS nave[n][m]){
  int incrementoy=0, ymax, xmax;
  getmaxyx(stdscr, ymax, xmax);
  
  for (int i = 0; i < n; ++i){
    int cte=xmax/4-5;
    for (int j = 0; j < m; ++j){
      nave[i][j].x=cte;
      cte=cte+4;
      nave[i][j].y=ymax/4+incrementoy;
      nave[i][j].figura=ALIEN;
    }
    incrementoy++;
  } 
}
int main() {
  int posx, posy, ymax, xmax, bx = -1, by = -1, contador = 0, cambio = 0, menu = 0, intro= 1;
  int vidas = 3, pts = 0, interruptor=1, frame_counter, vidasBoss = 100, comojugar = 0, lvl=1;
  int tecla, creditos = 0, jugar =0, maxptje = 0, velocidadNave = 50, velocidadBull = 40, change = 1, ingresoName=0, x=0 ,y=0 ,z=0 , n=0;
  initscr();
  start_color(); //inicializa color
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);    
  cbreak();
  noecho();
  curs_set(0); //visibilidad del cursor 0 invisible, 2 muy visible
  //screen size
  getmaxyx(stdscr, ymax, xmax); 
  //crea cursor
  char cursor[3][6] = {" _^_ ","<_|_>"," <|> "};
  posy = ymax-1;
  posx = xmax/2 - 6;
  refresh();

  keypad(stdscr, TRUE);
  timeout(25);
  refresh();
  //crea ventana pts
  WINDOW * puntaje = newwin(ymax/6, xmax, 0, 0);// ( int $filas , int $columnas , int $y , int $x )
  refresh();
  erase();

  //crea jefe   
  ALIENS boss[9];
  UbicaJefe(9, boss, 0);

  //crea escudos
  ESCUDOS shield[49];
  UbicarEscudos(49, shield, 1);
  
  //generar naves y posiciones
  ALIENS nave[3][12]; 
  UbicarAliensGrande(3, 12, nave);

  char abc[3][26];
  for (int i = 0; i < 3; ++i){
  	for (int j = 0; j < 26; ++j){
  		abc[i][j]=65+j;
  	}
  }

  PUNTAJES puntajesMax[4];
  for (int i = 0; i < 3; ++i){
    puntajesMax[i].ptos=0;
    puntajesMax[i].name[0]='A';
    puntajesMax[i].name[1]='A';
    puntajesMax[i].name[2]='A';
  }

  //crea balas
  BALAS* bullet;
  bullet = (BALAS*)malloc((3*12)*sizeof(BALAS));
  BALAS* BalaBoss;
  BalaBoss = (BALAS*)malloc((1)*sizeof(BALAS));

  while(1) { 
    while (intro == 1) {
      erase();
      char space[6][43] ={" _____   _____   ______   ______   ______ ","|  ___| |  __ | |  __  | |  ____| |  ____|","| |___  | |   | | |__  | | |      | |____ ","|___  | |  ___| |  __  | | |      |  ____|"," ___| | | |     | |  | | | |____  | |____","|_____| |_|     |_|  |_| |______| |______|"};
      char invaders[6][72] ={" ____   ___   _                  ____   _______  _____   _      _____ ","|_  _| | _ | ||       |_|       | __ | | ___  / | ____| | |___ |  ___|","  ||   || || ||     | |.| |     ||__ | ||  / /  ||____  |  ___|| |___ ","  ||   || || || |  _|_|.|_|_  | | __ | || / /   | ____| | |    |___  |"," _||_  || ||_|| |_|___ . ___|_| ||  || ||/ /    ||____  | |     ___| |","|____| || |___|       |_|       ||  || |__/     |_____| |_|    |_____|"};
    
      for (int i = 0; i < 6; i++) {
        mvwprintw (stdscr, ymax/7+i, xmax/2-25 , space[i]);   
      }
      for (int i = 0; i < 6; i++) {
        mvwprintw (stdscr, ymax/2+i, xmax/2-35, invaders[i]);   
      }
      refresh();
    
      attron(A_BLINK |  A_BOLD);  
      mvwprintw(stdscr, ymax-4, xmax/2-22,"PRESIONA 'ENTER' PARA EMPEZAR LA TRAVESIA\n");
      attroff( A_BLINK |  A_BOLD);
    
      tecla = wgetch (stdscr);
      if(tecla == 10){
        intro = 0;
        menu = 1;  
      }
      refresh();
    }

    while(menu == 1){
      erase();
      char menu1[60] = {' ','_','_','_','_',' ',' ',' ','_','_','_','_',' ',' ','_','_','_','_','_','_','_','_','_',' ',' ',' ','_','_','_','_',' ',' ',' ',' ','_',' ',' ',' ',' ','_',' ',' ',' ',' ',' ',' ','_',' '};
      char menu2[60] = {'|',' ','|',' ','|','_','_','|',' ','|',' ','|',' ','|',' ',' ','_','_','_','_','_','_','_','|',' ','|',' ',' ','_',' ','|',' ',' ','|',' ','|',' ',' ','|',' ','|',' ',' ',' ',' ','|',' ','|'};
      char menu3[60] = {'|',' ','|','-','.',' ',' ','.','-','|',' ','|',' ','|',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ','|',' ','|','|',' ',' ','|',' ','|',' ',' ','|',' ','|',' ',' ',' ',' ','|',' ','|'};
      char menu4[60] = {'|',' ','|',' ','|','_','_','|',' ','|',' ','|',' ','|',' ','|','_','_','_','_','_','_','_',' ',' ','|',' ','|',' ','|','|',' ',' ','|',' ','|',' ',' ','|',' ','|',' ',' ',' ',' ','|',' ','|'};
      char menu5[60] = {'|',' ','|',' ',' ',' ',' ',' ',' ','|',' ','|',' ','|',' ',' ','_','_','_','_','_','_','_','|',' ','|',' ','|',' ','|','|',' ',' ','|',' ','|',' ',' ','|',' ','|',' ',' ',' ',' ','|',' ','|'};
      char menu6[60] = {'|',' ','|',' ',' ',' ',' ',' ',' ','|',' ','|',' ','|',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ','|',' ','|','|',' ',' ','|',' ','|',' ',' ','|',' ','|',' ',' ',' ',' ','|',' ','|'};
      char menu7[60] = {'|',' ','|',' ',' ',' ',' ',' ',' ','|',' ','|',' ','|',' ','|','_','_','_','_','_','_','_',' ',' ','|',' ','|',' ','|','|','_','_','|',' ','|',' ',' ','|',' ','|','_','_','_','_','|',' ','|'};
      char menu8[60] = {'|','_','|',' ',' ',' ',' ',' ',' ','|','_','|',' ','|','_','_','_','_','_','_','_','_','_','|',' ','|','_','|',' ','|','_','_','_','_','_','|',' ',' ','|','_','_','_','_','_','_','_','_','|'};
      char choices[4][22] = {"J u g a r", "¿C o m o  j u g a r?", "C r e d i t o s", "S a l i r"};  
      int elegida = 0;
      //char dibujomenu[8][64] = {" ____   ___   _______   ____    _   _      _ ","| | |__| | | |  _____| |  _ |  | | | |    | |","| |-.  .-| | | |       | | ||  | | | |    | |","| | |__| | | | |_____  | | ||  | | | |    | |","| |      | | |  _____| | | ||  | | | |    | |","| |      | | | |       | | ||  | | | |    | |","| |      | | | |_____  | | ||__| | | |____| |","|_|      |_| |_______| |_| |_____| |________|"};
    
   
      attron(COLOR_PAIR(5));
      mvwprintw(stdscr, ymax/7+1, xmax/2-27,"%s\n",menu1); 
      mvwprintw(stdscr, ymax/7+2, xmax/2-27,"%s\n",menu2);
      mvwprintw(stdscr, ymax/7+3, xmax/2-27,"%s\n",menu3);
      mvwprintw(stdscr, ymax/7+4, xmax/2-27,"%s\n",menu4);
      mvwprintw(stdscr, ymax/7+5, xmax/2-27,"%s\n",menu5);
      mvwprintw(stdscr, ymax/7+6, xmax/2-27,"%s\n",menu6);
      mvwprintw(stdscr, ymax/7+7, xmax/2-27,"%s\n",menu7);
      mvwprintw(stdscr, ymax/7+8, xmax/2-27,"%s\n",menu8);
      attroff(COLOR_PAIR(5));

      for (int i = 0; i < 4; i++) {
        mvwprintw (stdscr, ymax/2+i+2, xmax/2 -11 , choices[i]);   
      }

      while (1) {
        for (int i = 0; i < 4; i++) {
          if (i == elegida) {
            attron (A_UNDERLINE | A_BOLD | A_BLINK);   
            mvwprintw (stdscr, ymax/2+i+2, xmax/2 -11, choices[i]); 
            attroff (A_UNDERLINE | A_BOLD | A_BLINK);
            refresh();
          }
          else {
            attron (A_NORMAL);   
            mvwprintw (stdscr, ymax/2+i+2, xmax/2 -11, choices[i]); 
            attroff (A_NORMAL);
            refresh();
          }
        }
        tecla = wgetch (stdscr);
        refresh();
        switch (tecla){
          case KEY_UP:
            elegida--;
            if (elegida == -1){
              elegida = 0;
            }
            break;
          case KEY_DOWN:
            elegida++;
            if (elegida == 4){
              elegida = 3; 
            }
            break;  
        }
      
        if(tecla == 10){
          if(elegida == 0){
            jugar = 1;
            break;
          }
          if(elegida == 1){
            comojugar = 1;
            break;
          }
          if(elegida == 2){
            creditos = 1;
            break;
          }
          if(elegida == 3){
            endwin();
            return 0;
          }
        }  
      }
    //sub while del menu:
      while(comojugar == 1){
        erase();
        char casos[2][16] = {"SALVAR AL MUNDO", "VOLVER AL  MENU"};
        char intrucciones[11][54]={"Los aliens arroba han decidido atacar nuestro mundo y","tu, como todo buen alumno de programacion, tienes una","nave espacial en tu bodega. El profesor X te contacta","y te pide que desempolves tu nave y salves al mundo.","Tu mision es matar a todos los aliens, disparas con","la flecha hacia arriba y te desplazaras por el campo","de batalla con las flechas hacia los lados. Cuidado,","hay un lider que los manda, es posible matarlo si le","disparas 5 veces, sin embargo sus subditos son menos","fuertes y al primer disparo mueren.","Tienes 3 vidas, aprovechalas, el mundo depende de ti."};
        elegida = 0;
      
        for(int i = 0; i < 11; i++){
          mvwprintw(stdscr, ymax/7+1+i,xmax/2-27,"%s\n",intrucciones[i]);
        }
        refresh();

        while (1) {
          for (int i = 0, j = 0; i < 2; i++,j+=20) {
            if (i == elegida) {
              attron (A_UNDERLINE | A_BOLD | A_BLINK | COLOR_PAIR(5));   
              mvwprintw (stdscr, ymax/2+5, xmax/2-20+j, casos[i]); 
              attroff (A_UNDERLINE | A_BOLD | A_BLINK | COLOR_PAIR(5));
              refresh();
            }
            else{
              attron (A_NORMAL);   
              mvwprintw (stdscr, ymax/2+5, xmax/2-20+j, casos[i]); 
              attroff (A_NORMAL);
              refresh();
            }
          }
  
          tecla = wgetch (stdscr);
          refresh();
          switch (tecla) {
            case KEY_LEFT:
              elegida--;
              if (elegida == -1){
                elegida = 0;
              }
              break;
            case KEY_RIGHT:
              elegida++;
              if (elegida == 2){
                elegida = 1; 
              }
              break;
          }

          if(tecla == 10){
            if(elegida == 0){//ir al juego
              vidas = 3;
              jugar = 1;
              comojugar = 0;
              break;
            }
            if(elegida == 1){//volver al menu
              comojugar = 0;
              break;
            }
          }
        }
      }
      //cierre while de como jugar
      while(creditos == 1){
        erase();
        char credito[5][50] = {"Este juego fue hecho por:", "Nicolás Araya,","Maximiliano Vergara","Martina Cádiz.","                                                "};
      
        for(int i = 0; i < 5; i++){
          mvwprintw(stdscr, ymax/4+1+i,xmax/5,"%s\n",credito[i]);
        }
        mvwprintw(stdscr, ymax/4+10, xmax/5,"El primer puntaje mas alto es de %s con %d puntos\n", puntajesMax[0].name, puntajesMax[0].ptos);
        mvwprintw(stdscr, ymax/4+11, xmax/5,"El segundo puntaje mas alto es de %s con %d puntos\n", puntajesMax[1].name, puntajesMax[1].ptos);
        mvwprintw(stdscr, ymax/4+12, xmax/5,"El tercer puntaje mas alto es de %s con %d puntos\n", puntajesMax[2].name, puntajesMax[2].ptos);


        refresh();
      
        attron(A_UNDERLINE | A_BOLD | A_BLINK | COLOR_PAIR(5));  
        mvwprintw(stdscr, ymax-10, xmax/2-10,"VOLVER AL MENU\n");
        attroff( A_UNDERLINE | A_BOLD | A_BLINK | COLOR_PAIR(5));
        refresh();
    
        tecla = wgetch (stdscr);
        if(tecla == 10){
          menu = 1;
          creditos = 0;
          break;  
        }
      }
          
      while (jugar == 1){
        erase();
        while (ingresoName==0){
          erase();
          while(1){
            attron ( A_BOLD | COLOR_PAIR(5));   
            mvwprintw (stdscr, ymax/4, xmax/2-16, "I N G R E S E   U N   U S U A R I O\n"); 
            attroff (A_BOLD | COLOR_PAIR(5));
            if (n==0){
              attron (A_UNDERLINE | A_BLINK| COLOR_PAIR(5));   
              mvaddch(ymax/2, xmax/2-2, abc[0][x]);
              attroff (A_UNDERLINE | A_BLINK| COLOR_PAIR(5)); 
              attron (A_NORMAL | A_DIM);
              mvaddch(ymax/2, xmax/2, abc[1][y]);
              mvaddch(ymax/2, xmax/2+2, abc[2][z]);
              attroff (A_NORMAL | A_DIM);
            }

            if (n==1){
              mvaddch(ymax/2, xmax/2-2, abc[0][x]);
              attron (A_UNDERLINE | A_BLINK| COLOR_PAIR(5));   
              mvaddch(ymax/2, xmax/2, abc[1][y]);
              attroff (A_UNDERLINE | A_BLINK| COLOR_PAIR(5)); 
              attron (A_NORMAL| A_DIM);
              mvaddch(ymax/2, xmax/2+2, abc[2][z]);
              attroff (A_NORMAL| A_DIM);
            }
            if (n==2){
              mvaddch(ymax/2, xmax/2-2, abc[0][x]);
              mvaddch(ymax/2, xmax/2, abc[1][y]);
              attron (A_UNDERLINE | A_BLINK| COLOR_PAIR(5));   
              mvaddch(ymax/2, xmax/2+2, abc[2][z]);
              attroff (A_UNDERLINE | A_BLINK| COLOR_PAIR(5)); 
            }
            tecla = wgetch (stdscr);
            switch (tecla) {
              case KEY_UP:
                if(n==0){
                  x--;
                  if(x<0){
                    x=25;
                  }
                  if(x>25){
                    x=0;
                  }
                  break;
                }
                if(n==1){
                  y--;
                  if(y<0){
                    y=25;
                  }
                  if(y>25){
                    y=0;
                  }
                  break;
                }
                if(n==2){
                  z--;
                  if(z<0){
                    z=25;
                  }
                  if(z>25){
                    z=0;
                  }
                  break;
                }
              case KEY_DOWN:
                if(n==0){
                  x++;
                  if(x<0){
                    x=25;
                  }
                  if(x>25){
                    x=0;
                  }
                  break;
                }
                if(n==1){
                  y++;
                  if(y<0){
                    y=25;
                  }
                  if(y>25){
                    y=0;
                  }
                  break;
                }
                if(n==2){
                  z++;
                  if(z<0){
                    z=25;
                  }
                  if(z>25){
                    z=0;
                  }
                  break;
                }
                break;
              }
              if (tecla==10){
                if(n==2){
                  ingresoName=1;
                }
                n++;
                break;
              }
          }
          refresh();

        }

        frame_counter++;
        //si pierde
        while(vidas <= 0){
          erase();
          char cases[3][16] = {"CONTINUAR", "VOLVER AL  MENU","SALIR"};
          elegida = 0;
          while (1) {
            for (int i = 0, j = 0; i < 3; i++,j+=2) {
              if (i == elegida) {
                attron (A_UNDERLINE | A_BOLD | A_BLINK | COLOR_PAIR(5));   
                mvwprintw (stdscr, ymax/4+j, xmax/2-16, cases[i]); 
                attroff (A_UNDERLINE | A_BOLD | A_BLINK | COLOR_PAIR(5));
                refresh();
              }
              else{
                attron (A_NORMAL);   
                mvwprintw (stdscr, ymax/4+j, xmax/2-16, cases[i]); 
                attroff (A_NORMAL);
                refresh();
              }
            } 
            tecla = wgetch (stdscr);
            refresh();
            switch (tecla) {
              case KEY_UP:
                elegida--;
                if (elegida == -1){
                  elegida = 0;
                }
                break;
              case KEY_DOWN:
                elegida++;
                if (elegida == 3){
                  elegida = 2; 
                }
                break;
            }
            if(tecla == 10){
              if(elegida == 0){ //continuar
              vidas = 3;
              contador = 0;
              UbicarBalas(3, 12, nave, bullet);
              UbicarBalasJEFE(BalaBoss, 5, boss);
              break;
              }
              if(elegida == 1){ //reinicio
                erase();
                posy = ymax-1;
                posx = xmax/2 - 6;
                UbicarAliensGrande(3, 12, nave);
                UbicarEscudos(49, shield, 1);
                UbicaJefe(9, boss, 0);
                puntajesMax[3].name[0]=abc[0][x];
                puntajesMax[3].name[1]=abc[0][y];
                puntajesMax[3].name[2]=abc[0][z];
                puntajesMax[3].ptos=pts;
                ingresoName=0;
                n=0;
                x=0;  
                y=0;
                z=0;
                for (int i = 0; i < 4; ++i){                       //ordenamiento
                  for (int j = i; j < 4; ++j){
                    if (puntajesMax[i].ptos<puntajesMax[j].ptos){

                      int aux=puntajesMax[i].ptos;
                      puntajesMax[i].ptos=puntajesMax[j].ptos;
                      puntajesMax[j].ptos=aux;
                      char c=puntajesMax[i].name[0];
                      puntajesMax[i].name[0]=puntajesMax[j].name[0];
                      puntajesMax[j].name[0]=c;

                      c=puntajesMax[i].name[1];
                      puntajesMax[i].name[1]=puntajesMax[j].name[1];
                      puntajesMax[j].name[1]=c;

                      c=puntajesMax[i].name[2];
                      puntajesMax[i].name[2]=puntajesMax[j].name[2];
                      puntajesMax[j].name[2]=c;
                    }
                  }
                }
                contador = 0;
                UbicarBalas(3, 12, nave, bullet);
                UbicarBalasJEFE(BalaBoss, 5, boss);
                vidasBoss = 100;
                pts = 0;
                velocidadNave = 50;
                velocidadBull = 40;
                vidas = 3;
                jugar = 0;
                break;  
              }
              if(elegida == 2){
                endwin();
                return 0;
                break;
              }
            }
          }
        }
        //entra al nivel 2:
        if(pts == 300 && change == 1){
          lvl=2;
          velocidadNave = 20;
          velocidadBull = 10;
          posy = ymax-1;
          posx = xmax/2 - 6;
          contador = 0;
          UbicarBalas(3, 12, nave, bullet);
          UbicarBalasJEFE(BalaBoss, 5, boss);
          UbicarAliensGrande(3, 12, nave);
          UbicarEscudos(49, shield, 2);
          UbicaJefe(9, boss, 0);
          change = 0;
          int temp=0;
          while (temp == 0) {
            erase();
            attron(A_BLINK |  A_BOLD);  
            mvwprintw(stdscr, ymax/2-4, xmax/2-4,"NIVEL 2\n");
            mvwprintw(stdscr, ymax/2-2 , xmax/2-15,"PRESIONA 'ENTER' PARA CONTINUAR\n");
            attroff( A_BLINK |  A_BOLD);
            tecla = wgetch (stdscr);
            if(tecla == 10){
              temp=1;
            }
            refresh();
          }
        }
        if(pts == 600 && change == 0){
          lvl=3;
          velocidadNave = 5;
          velocidadBull = 10;
          posy = ymax-1;
          posx = xmax/2 - 6;
          contador = 0;
          UbicarBalas(3, 12, nave, bullet);
          UbicarBalasJEFE(BalaBoss, 5, boss);
          UbicaJefe(9, boss, 1);
          UbicarAliensGrande(3, 12, nave);
          UbicarEscudos(49, shield, 3);
          change = 1;
          int temp=0;
          while (temp == 0) {
            erase();
            attron(A_BLINK |  A_BOLD);  
            mvwprintw(stdscr, ymax/2-4, xmax/2-4,"NIVEL 3\n");
            mvwprintw(stdscr, ymax/2-2, xmax/2-15,"PRESIONA 'ENTER' PARA CONTINUAR\n");
            attroff( A_BLINK |  A_BOLD);
            tecla = wgetch (stdscr);
            if(tecla == 10){
              temp=1;
            }
            refresh();
          }
        }
        if (pts == 900){
          int temp=0;
          puntajesMax[3].name[0]=abc[0][x];
          puntajesMax[3].name[1]=abc[0][y];
          puntajesMax[3].name[2]=abc[0][z];
          puntajesMax[3].ptos=pts;
          ingresoName=0;
          n=0;
          x=0;
          y=0;
          z=0;
          for (int i = 0; i < 4; ++i){                       //ordenamiento
            for (int j = i; j < 4; ++j){
              if (puntajesMax[i].ptos<puntajesMax[j].ptos){

                int aux=puntajesMax[i].ptos;
                puntajesMax[i].ptos=puntajesMax[j].ptos;
                puntajesMax[j].ptos=aux;
                char c=puntajesMax[i].name[0];
                puntajesMax[i].name[0]=puntajesMax[j].name[0];
                puntajesMax[j].name[0]=c;

                c=puntajesMax[i].name[1];
                puntajesMax[i].name[1]=puntajesMax[j].name[1];
                puntajesMax[j].name[1]=c;

                c=puntajesMax[i].name[2];
                puntajesMax[i].name[2]=puntajesMax[j].name[2];
                puntajesMax[j].name[2]=c;
              }
            }
          }
          while (temp == 0) {
            erase();
            attron(A_BLINK |  A_BOLD);  
            mvwprintw(stdscr, ymax/2-4, xmax/2-5,"FELICIDADES\n");
            mvwprintw(stdscr, ymax/2-2 , xmax/2-15,"PRESIONA 'ENTER' PARA CONTINUAR\n");
            attroff( A_BLINK |  A_BOLD);
            tecla = wgetch (stdscr);
            if(tecla == 10){
              erase();
              posy = ymax-1;
              posx = xmax/2 - 6;
              UbicarAliensGrande(3, 12, nave);
              UbicarEscudos(49, shield, 1);
              UbicaJefe(9, boss, 0);
              if(pts > maxptje){
                maxptje = pts;
              }
              contador = 0;
              UbicarBalas(3, 12, nave, bullet);
              UbicarBalasJEFE(BalaBoss, 5, boss);
              vidasBoss = 100;
              pts = 0;
              velocidadNave = 50;
              velocidadBull = 40;
              vidas = 3;
              jugar = 0;
              menu=0;
              intro=1;
              lvl=1;
              refresh();
              break;
            }
            refresh();
          }
        }
         
        //actualizar escudos 
        for (int i = 0; i < 48; ++i) {
          attron(COLOR_PAIR(4));
          mvaddch(shield[i].y, shield[i].x, shield[i].figura);
          attroff(COLOR_PAIR(4));
        }
        //posicion bullets nave
        for (int i = 0; i < 36; ++i) {
          mvaddch(bullet[i].y, bullet[i].x, bullet[i].figura);
        }
        //posicion nave
        for (int i = 0; i < 3; ++i){
          for (int j = 0; j < 12; ++j){
            mvaddch(nave[i][j].y, nave[i][j].x, nave[i][j].figura);
          }
        }
        //color bala boss
        if(pts >= 600){
          attron(COLOR_PAIR(1));
          mvaddch(BalaBoss->y, BalaBoss->x, BalaBoss->figura);
          attroff(COLOR_PAIR(1));
        }
        //color y posicion boss
        for (int i = 0; i < 9; ++i) {
          attron(COLOR_PAIR(1));
          mvaddch(boss[i].y, boss[i].x, boss[i].figura);
          attroff(COLOR_PAIR(1));
        }
        refresh();

        //cambio color cursor
        if(vidas == 3){ 
          attron(COLOR_PAIR(3));
          for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 5; j++) {
              mvaddch(posy - 2 + i, posx -2 + j, cursor[i][j]);
            }
          } 
          attroff(COLOR_PAIR(3));
        }
        if(vidas == 2){
          attron(COLOR_PAIR(2));
          for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 5; j++) {
              mvaddch(posy - 2 + i, posx - 2 + j, cursor[i][j]);
            }
          } 
          attroff(COLOR_PAIR(2));
        }
        if(vidas == 1){
          attron(COLOR_PAIR(1));
          for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 5; j++) {
              mvaddch(posy - 2 + i, posx- 2 + j, cursor[i][j]); //FIGURAS ASC PREDET
            }
          } 
          attroff(COLOR_PAIR(1));
        }
   
        //BULLET cursor
        if (bx != -1){
          by--;
          if (by <= -1){
            bx = -1;
          }
          else { 
            mvaddch(by, bx, ACS_BULLET);
          }
        }
        refresh();

        for (int i = 0; i < 3; i++) {       //morir, desaparecer, puntaje
          for (int j = 0; j < 12; ++j){
            if(nave[i][j].figura == ALIEN){
              if(nave[i][j].x == bx && nave[i][j].y==by+1){
                bx=-1;
                by=-1;
                nave[i][j].figura=' ';
                pts=pts+100;
                beep();
              }
              if(nave[i][j].y == ymax-1){ //muere si los aliens llegan al final
                vidas = 0;
                beep();
                beep();
              }
            }    
          }
        }

        //disparo muerte boss 
        if(boss[0].figura != ' ' && boss[1].figura != ' ' && boss[2].figura != ' ' && boss[3].figura != ' ' && boss[4].figura != ' ' && boss[5].figura != ' ' && boss[6].figura != ' ' && boss[7].figura != ' ' && boss[8].figura != ' '){
          if((boss[0].x == bx && boss[0].y == by) || (boss[1].x == bx && boss[1].y == by) || (boss[2].x == bx && boss[2].y == by) || (boss[3].x == bx && boss[3].y == by) || (boss[4].x == bx && boss[4].y == by) || (boss[5].x == bx && boss[5].y == by) || (boss[6].x == bx && boss[6].y == by) || (boss[7].x == bx && boss[7].y == by) || (boss[8].x == bx && boss[8].y == by) ){
            by = -1;
            bx = -1;
            vidasBoss=vidasBoss-20;
            if(vidasBoss == 0){
              pts += 300;
              for (int i = 0; i < 9; i++) {
                boss[i].figura = ' ';
              }
              beep();
            }
          }
        }
        refresh();

        //movimiento y reubicacion balas
        if (frame_counter % velocidadBull == 0){
          if(contador % ymax == 0){
            UbicarBalas(3, 12, nave, bullet);
            UbicarBalasJEFE(BalaBoss, 5, boss);
            refresh();
          }
          MovBalas(bullet, 36);
          MovBalas(BalaBoss, 1);
          contador++;
          refresh();
        }
    
        //descuenta vidas X BALAS
        for (int i = 0; i < 36; i++){
          for (int j = 0; j < 5; j++) { 
            if((bullet[i].figura == BULLETS && posy - 2 == bullet[i].y && posx -2+j == bullet[i].x) || (bullet[i].figura == BULLETS && posy - 3 == bullet[i].y && posx == bullet[i].x)){ //CAMBIO:para que cursor pierda vidas en cada parte que lo tocan
              vidas -= 1;
              beep();
              beep();
              bullet[i].figura = ' '; 
            }
          }
        }
        //si el jefe achunta
        for (int j = 0; j < 5; j++) { 
          if((BalaBoss[0].figura == '*' && posy - 2 == BalaBoss[0].y && posx - 2 + j == BalaBoss[0].x) || (BalaBoss[0].figura == '*' && posy - 3 == BalaBoss[0].y && posx == BalaBoss[0].x)){ //CAMBIO: lo mismo para balas boss
            vidas -= 2;
            beep();
            beep();
            BalaBoss[0].figura = ' ';
          }
        }
    
        //desaparecen escudos si le caen balas
        for (int i = 0; i < 46; i++){ //bala aliens
          if(shield[i].figura == '_'){
            for (int j = 0; j < 36; j++){
              if(bullet[j].figura == BULLETS && bullet[j].x == shield[i].x && bullet[j].y - 1 == shield[i].y){              
                shield[i].figura = ' ';
                bullet[j].figura = ' ';  
              }
            }
          }
        }
        //si caen la balas del jefe
        for (int i = 0; i < 46; i++){
          if(shield[i].figura == '_'){
            if(BalaBoss[0].figura == '*' && BalaBoss[0].x == shield[i].x && BalaBoss[0].y == shield[i].y){              
              shield[i].figura = ' ';
              BalaBoss[0].figura = ' ';  
            }
          }
        }

        //movimiento jefe 
        if (frame_counter % 2 == 0 && cambio % 2 == 0){
          for (int i = 0; i < 9; i++){
            if(boss[8].x == 9){
              cambio++;
              break;
            }
            boss[i].x -= 1;      
          }
        }
        if (frame_counter % 2 == 0 && cambio% 2 != 0){
          for (int i = 0; i < 9; i++){
            if(boss[8].x == xmax-1){
              cambio++;
              break;
            }
            boss[i].x += 1;      
          }
        }

        //mov nave
        if (frame_counter % velocidadNave == 0 && interruptor%2!=0){
          for (int i = 0; i < 3; ++i){
            for (int j = 0; j < 12; ++j){
              nave[i][j].x=nave[i][j].x+1;
            }
          }
          if (nave[0][11].x==xmax-2){
            for (int i = 0; i < 3; ++i){
              for (int j = 0; j < 12; ++j){
                nave[i][j].y=nave[i][j].y+1;
              }
            }
            interruptor++;
          }
          refresh();
        }
        if (frame_counter % velocidadNave == 0 && interruptor%2==0){
          for (int i = 0; i < 3; ++i){
            for (int j = 0; j < 12; ++j){
              nave[i][j].x=nave[i][j].x-1;
            }
          }
          if (nave[0][0].x==2){
            for (int i = 0; i < 3; ++i){
              for (int j = 0; j < 12; ++j){
                nave[i][j].y=nave[i][j].y+1;
                nave[i][j].x=nave[i][j].x+1;
              }
            }
            interruptor++;
          }
          refresh();
        }

        //dibujo caja score
        werase(puntaje);
        box(puntaje, 0, 0);
        if(lvl!=3){
        	mvwprintw(puntaje, 1, 1, "Puntaje:  %d        Vidas Restantes: %d      Nivel %d     ", pts, vidas, lvl);
        }
        else{
        	mvwprintw(puntaje, 1, 1, "Puntaje:  %d        Vidas Restantes: %d      Nivel %d     Salud Jefe %d ", pts, vidas, lvl, vidasBoss);
        }

        wrefresh(puntaje);

        //moviemnto arrows
        tecla = getch();
        switch(tecla) {
          case KEY_RIGHT:
            if (limitesCursor(posy, posx + 3)){
              posx++;    
            }
            break;
          case KEY_LEFT:
            if (limitesCursor(posy, posx - 3)){  
              posx--;   
            }
            break;
          case KEY_UP:        
            if(bx == -1){
              bx = posx;
              by = posy;
            }    
        }
      }//cierre while jugar
    }//cierre while menu 
  }//cierre while 1
  endwin();
  return 0;
}