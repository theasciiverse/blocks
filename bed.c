

#ifndef CURSES
#include <ncurses.h>
#else
#include <curses.h>
#endif

#include <string.h>
#include <ctype.h>

/*heck - i started writing the thing using curses - then 
         switched to ncurses, did not seem to make much
         difference
 */

#define NAMEL  40
#define FNAMEL 20
#define CHARL 126

/*#define DEBUG   1*/

struct game{
  char name[NAMEL];
  char fname[FNAMEL];
  char xdim;
  char ydim;
  char field[CHARL][CHARL];
  char diff;
};

int main(int argc,char **argv){
  struct game cw;
  int myx,myy;
  int in;
  char filename[40];
  FILE *fp;
  int unsaved=0;

  if(argc>1){
    strcpy(filename,argv[1]);
    filename[39]='\0';
  #ifdef DEBUG
    printf("going to check out file %s\n",filename);
    sleep(10);
  #endif
  }
  else{
    printf("\nEnter the name of the file containing the game : ");
    fgets(cw.fname,FNAMEL,stdin);
    #ifdef DEBUG
      printf("Last char is %d(%c)\n",cw.fname[strlen(cw.fname)-1]
                                    ,cw.fname[strlen(cw.fname)-1]);  
      sleep(2);
    #endif 
    if(iscntrl(cw.fname[strlen(cw.fname)-1]))cw.fname[strlen(cw.fname)-1]='\0';
    strcpy(filename,cw.fname);
  }

  if(NULL==(fp=fopen(filename,"r"))){

    printf("\nEnter the name of the game you wish to create  : ");
    fgets(cw.name,NAMEL,stdin);

    if(iscntrl(cw.name[strlen(cw.name)-1]))cw.name[strlen(cw.name)-1]='\0';
    strcpy(cw.fname,filename);
  
    printf("\nEnter the difficulty level : ");
    scanf("%d",&(cw.diff));

    for(myx=0;myx<CHARL;myx++){
      for(myy=0;myy<CHARL;myy++){
        cw.field[myx][myy]=' ';
      }
    }
  }

  else{
    fclose(fp);
    loadnext(&cw,filename);
  } 
 
  initscr();   /*start the curses*/

  cbreak();
  noecho();    /*for one char at a time without echo*/
 
  keypad(stdscr,TRUE);
 
  drawsolid(&cw);
 
  refresh();

    cw.xdim=COLS-2;
    cw.ydim=LINES-2;

    myx=COLS/2;
    myy=LINES*2/3;
  
  /*perform the sanity checks here*/
  /*ie screen too small etc...*/

    while(('q'!=(in=getch()))||unsaved){
  
      if(in!=ERR){
        switch(in){
          case KEY_LEFT :
          case '4' :
          case 'h' : if(--myx==0)myx=COLS-2;
                     break;
          case KEY_DOWN :
          case 'j' : if(++myy==LINES-1)myy=1;
                     break;
          case KEY_UP :
          case '8' :
          case 'k' : if(--myy==0)myy=LINES-2;
                     break;
          case KEY_RIGHT :
          case 'l' : if(++myx==COLS-1)myx=1;
                     break;
          case '3' :
          case '#' : mvaddch(myy,myx,'#');
                     cw.field[myy-1][myx-1]='#';
                     if(++myx==COLS-1)myx=1;
                     unsaved=1;
                     break;
          case '|' :
          case '\\': mvaddch(myy,myx,'|');
                     cw.field[myy-1][myx-1]='|';
                     if(++myx==COLS-1)myx=1;
                     unsaved=1;
                     break;
          case '1' :
          case '!' : mvaddch(myy,myx,'!');
                     cw.field[myy-1][myx-1]='!';
                     if(++myx==COLS-1)myx=1;
                     unsaved=1;
                     break; 
          case '2' :
          case '@' : mvaddch(myy,myx,'@');
                     cw.field[myy-1][myx-1]='@';
                     if(++myx==COLS-1)myx=1;
                     unsaved=1;
                     break; 
          case '+' :
          case '=' : mvaddch(myy,myx,'=');
                     cw.field[myy-1][myx-1]='=';
                     if(++myx==COLS-1)myx=1;
                     unsaved=1;
                     break; 
          case '-' :
          case '_' : mvaddch(myy,myx,'-');
                     cw.field[myy-1][myx-1]='-';
                     if(++myx==COLS-1)myx=1;
                     unsaved=1;
                     break; 
          case '6' : in='^';
          case ' ' :
          case '}' :
          case '{' :
          case '.' :
          case ':' :
          case '<' :
          case 'v' :
          case '^' :
          case '>' : mvaddch(myy,myx,in);
                     cw.field[myy-1][myx-1]=in;
                     if(++myx==COLS-1)myx=1;
                     unsaved=1;
                     break;
          case 's' :
          case 'w' :
          case 'S' :
          case 'W' : move(0,1);
                     printw("Saving Game %s as File %s (%d by %d)\n",cw.name,cw.fname,cw.xdim,cw.ydim);
                     if(COLS-2>CHARL||LINES-2>CHARL){
                       printw("WARNING - the game you are writing is too big"); 
                     }
                     savegame(cw);
                     unsaved=0;

                     break;
          case 'r' :
          case 'R' : drawsolid(&cw);
                     break;
          case '?' :
          case '/' : clear();
                     move(1,1);
                     printw("\tblock editor - create your own levels\t\tGPL\n\n");
                     printw("characters to use :\n");
                     printw("\t | ! . - = @ # v ^ > < { } and space bar\n");
                     printw("\n");
                     printw("The Space bar clears an induvidual cell\n");
                     printw("{ marks the starting position\n");
                     printw("} marks the end position\n");
                     printw("\n");
                     printw("each . counts 100 points\n");
                     printw("\n");
                     printw("w to write to file, q to quit\n");
                     refresh();
                     getch();
                     clear();
                     drawsolid(&cw);
                     refresh();
                     break;
          case 'm' : mvprintw(0,COLS-18," kludged by mwelz");
                     break;
          case 'Q' :
          case 'q' :
                     break;
          default  : /*if(isalpha(in)){
                       cw.field[myy-1][myx-1]=in;
                       mvaddch(myy,myx,in);
                       if(++myx==COLS-1)myx=1;
                       unsaved=1;
                     }
                     else{ */
                       mvprintw(0,COLS-18,"   Try ? for help");
                 /*  }*/
                     break;
        }
        move(myy,myx);
        refresh();
      }  
      if(unsaved&&in=='q'){
        unsaved=0;
                     mvprintw(0,COLS-18,"Game not saved...");
        refresh();
      }
    }

  
    clear();     /*clear up the mess we made*/
    refresh();
    endwin();

  
  return 0;
}

int savegame(struct game s){
  FILE *fp;
  int  i,j;
  int  retval=0;
  
  fp=fopen(s.fname,"w");
  if(fp!=NULL){
    for(i=0;i<NAMEL;i++)fputc(s.name[i],fp);
    for(i=0;i<FNAMEL;i++)fputc(s.fname[i],fp);
    fputc(s.xdim,fp);
    fputc(s.ydim,fp);
    fputc(s.diff,fp);
    for(i=0;i<CHARL;i++){
      for(j=0;j<CHARL;j++){
        fputc(s.field[i][j],fp);
      }
    }
    fclose(fp);
  }
  else{
    printf("Save operation failed\n");
    retval=1;
  }
  return retval;
}

int loadnext(struct game *s,char *fname){
  static count=0;
  FILE *fp;
  int i,j;
  int retval=0;

#ifdef DEBUG
  printf("\nAttempting to open file %s \n",fname);
#endif

  fp=fopen(fname,"r");
  if(fp!=NULL){
    fseek(fp,(long)(count*(CHARL*CHARL+FNAMEL+NAMEL+3)),SEEK_SET);
    for(i=0;i<NAMEL;i++){
      if(EOF==(s->name[i]=fgetc(fp))){
        retval=1; 
        i=NAMEL;
      }
    } 
    if(!retval){
      for(i=0;i<FNAMEL;i++){
        if(EOF==(s->fname[i]=fgetc(fp))){
          retval=2; 
        }
      }
      if(EOF==(s->xdim=fgetc(fp))){
        retval=2; 
      }
      if(EOF==(s->ydim=fgetc(fp))){
        retval=2; 
      }
      if(EOF==(s->diff=fgetc(fp))){
        retval=2; 
      }
      for(i=0;i<CHARL;i++){
        for(j=0;j<CHARL;j++){
          if(EOF==((s->field[i][j])=fgetc(fp))){
            retval=2; 
          }
#ifdef DEBUG
          if((s->field[i][j])=='#')printf("#");
          else                     printf("-");
#endif
        }
      }
    } 
    count++;
    fclose(fp);
  }
  else{
    retval=2;  
  }
  return retval;
}    

int drawsolid(struct game *s){
  int retval=0;
  int i,j,x,y,xo,yo;

  if(COLS<(s->xdim)||LINES<(s->ydim)){
    retval=1;
  }
  else{
    xo=(COLS-(s->xdim))/2;
    yo=(LINES-(s->ydim))/2;
    
    for(i=0;i<s->ydim;i++){
      for(j=0;j<s->xdim;j++){
        switch((s->field[i][j])){
          case '!' :
          case '=' :
          case '-' :
          case ':' :
          case '@' :
          case '{' :
          case '}' : 
          case  0  :
          case ' ' :
          case '|' :
          case '#' :
          case '>' :
          case '<' :
          case 'v' :
          case '^' :
          case '.' : mvaddch(yo+i,xo+j,(s->field[i][j]));
                     break;
          default  : if(isalpha(s->field[i][j]))mvaddch(yo+i,xo+j,(s->field[i][j])); 
                     break;
        }
      }
    }    
  }
  

  refresh();
  return retval;
}
  

