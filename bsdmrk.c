
//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
#include <stdio.h>
#define PATH_MAX 2500
#if defined(__linux__) //linux
#define MYOS 1
#elif defined(_WIN32)
#define MYOS 2
#elif defined(_WIN64)
#define MYOS 3
#elif defined(__unix__) 
#define MYOS 4  // freebsd
#define PATH_MAX 2500
#else
#define MYOS 0
#endif
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <time.h>


#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h> 
#include <time.h>

int tex_item_level = 0;



/////////////////////////////////
/////////////////////////////////
int fexist(char *a_option)
{
  char dir1[PATH_MAX]; 
  char *dir2;
  DIR *dip;
  strncpy( dir1 , "",  PATH_MAX  );
  strncpy( dir1 , a_option,  PATH_MAX  );

  struct stat st_buf; 
  int status; 
  int fileordir = 0 ; 

  status = stat ( dir1 , &st_buf);
  if (status != 0) {
    fileordir = 0;
  }

  // this is compatible to check if a file exists
  FILE *fp2check = fopen( dir1  ,"r");
  if( fp2check ) {
  // exists
  fileordir = 1; 
  fclose(fp2check);
  } 

  if (S_ISDIR (st_buf.st_mode)) {
    fileordir = 2; 
  }
return fileordir;
/////////////////////////////
}







char *strcut( char *str , int myposstart, int myposend )
{  
      char ptr[strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if ( ( str[i] != '\0' ) && ( str[i] != '\0') )
         if ( ( i >=  myposstart-1 ) && (  i <= myposend-1 ) )
           ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}






char *fbasenoext(char* mystr)
{
    char *retstr;
    char *lastdot;
    if (mystr == NULL)
         return NULL;
    if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
        return NULL;
    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';
    return retstr;
}


///////////////////////////////////////////
void readfile( char *filesource )
{
   FILE *source; 
   int ch ; 
   source = fopen( filesource , "r");
   while( ( ch = fgetc(source) ) != EOF )
   {
         printf( "%c", ch );
   }
   fclose(source);
}




///////////////////////////////////////////
void readfileline( char *fileoutput , char *filesource )
{
   FILE *source; 
   FILE *target; 
   FILE *fp5; 
   int ch, c, co;
   char lline[PATH_MAX];
   char ncpfile[PATH_MAX];
   char charo[PATH_MAX];
   int pcc = 0;
   int linecount = 0;
   int posx    = 100;
   int posy    = 730;
   int posxmax = 588;
   int posymax = 835;

   source = fopen( filesource , "r");
   target = fopen( fileoutput , "wb+");

   int export_skip = 0;
   int area_archive = 0;
   int fileeof = 0;
   while( fileeof == 0 )
   {
       export_skip = 0;
       c = fgetc(source); 
       if ( c == EOF ) fileeof = 1;

       else
       {
           if ( c != '\n' )
               lline[pcc++]=c;


           if ( ( lline[ 0 ] == '!' )  && ( lline[ 1 ] == 'E' ) && ( lline[ 2 ] == 'O' )  && ( lline[ 3 ] == 'F' )  )
              fileeof = 1;


           if ( ( lline[ 0 ] == '/' )  && ( lline[ 1 ] == '*' ) ) 
               area_archive = 1;

           if ( ( lline[ 0 ] == '*' )  && ( lline[ 1 ] == '/' ) ) 
           {    area_archive = 0;  export_skip = 1;       }


           if ( fileeof == 0 )
           if ( c == '\n' ) 
           {
             lline[pcc++]='\0';

             printf( "%s", lline );
             printf( "\n" );

             if ( area_archive == 0 )
             {
              if ( export_skip == 0 )
              {
               // REM
               if (  ( lline[ 0 ] == '/' ) && ( lline[ 1 ] == '/' )  )
               {
                   printf( "Rem : do nothing.\n" );
               }

               ////////////////////////////
               // item header
               else if (  ( lline[ 0 ] == '!' ) && ( lline[ 1 ] == '>' ) &&  ( lline[ 2 ] == ' ' ) )
               {
                 if ( tex_item_level == 0  )
                 {
                    tex_item_level = 1;
                    fputs( "\\begin{itemize}\n" , target);
                 }
                 fputs( "\\item " , target);
                 fputs( strcut( lline, 2 +2 , strlen( lline )) , target);
                 fputs( "\n" , target);
               }


             ///////////
             else if ( ( lline[0] == '#' ) && ( strstr(lline, "#include{" ) != 0 ) )
             {
                 strncpy( ncpfile , strcut( lline , 8+2 , strlen( lline )-1 )  ,  PATH_MAX );
                 printf( "=> Found an include with string  |%s|%d|\n" , ncpfile , fexist( ncpfile ) );
                 strncpy( ncpfile , getenv( "HOME" ), PATH_MAX );
                 strncat( ncpfile , "/include/mrk/" , PATH_MAX - strlen( ncpfile ) -1 );
                 strncat( ncpfile , strcut( lline , 8+2 , strlen( lline )-1 )  , PATH_MAX - strlen( ncpfile ) -1 );
                 printf( "=> Found an include #include: |%s|%d|\n" , ncpfile , fexist( ncpfile ) );
                 if ( fexist( ncpfile ) == 1 ) 
                 {
                   fp5 = fopen( ncpfile  , "r");
                   while( ( ch = fgetc(fp5) ) != EOF )
                   {
                      printf( "%c", ch );
                      fputc( ch, target);
                   }
                   fclose(fp5);
                 }
             }



               ////////
               else if (  ( lline[ 0 ] == '!' ) && ( lline[ 1 ] == 't' ) &&  ( lline[ 2 ] == 'e' ) 
                  && ( strstr( lline, "!tex" ) != 0 ) )
               {
                 //fileeof = 0;
                 fputs( "\n" , target);
                 //fputs( "\\documentclass[a4paper,11pt,includefoot]{scrartcl}\n" , target);
                 fputs( "\\documentclass[a4paper,11pt]{article}\n" , target);
                 fputs( "\n" , target);
               }

               ////////
               else if (  ( lline[ 0 ] == '!' ) && ( lline[ 1 ] == 'b' ) &&  ( lline[ 2 ] == 'e' ) 
                  && ( strstr( lline, "!begin" ) != 0 ) )
               {
                 //fileeof = 1;
                 fputs( "\n" , target);
                 fputs( "\\begin{document}\n" , target);
                 fputs( "\n" , target);
               }



               ////////
               else if (  ( lline[ 0 ] == '!' ) && ( lline[ 1 ] == 's' ) &&  ( lline[ 2 ] == 't' ) 
               && ( strstr( lline, "!startdoc" ) != 0 ) )
               {
                   fputs( "\n" , target);
                   fputs( "\n" , target);
                   fputs( "\\documentclass[11pt]{article}\n" , target );
                   fputs( "\\usepackage{url}\n" , target );
                   fputs( "\\usepackage{hyperref}\n" , target );
                   fputs( "\\usepackage{grffile}\n" , target );
                   fputs( "\\usepackage{graphicx}\n" , target );
                   fputs( "\\usepackage{xcolor}\n" , target );
                   fputs( "\\begin{document}\n" , target );
                   fputs( "\n" , target);
               }


               ////////
               else if (  ( lline[ 0 ] == '!' ) && ( lline[ 1 ] == 'e' ) &&  ( lline[ 2 ] == 'n' ) 
                  && ( strstr( lline, "!enddoc" ) != 0 ) )
               {
                 fileeof = 1;
                 fputs( "\n" , target);
                 fputs( "\\end{document}\n" , target);
                 fputs( "\n" , target);
               }

               ////////
               else if (  ( lline[ 0 ] == '!' ) && ( lline[ 1 ] == 'e' ) &&  ( lline[ 2 ] == 'n' ) 
                  && ( strstr( lline, "!enddoc" ) != 0 ) )
               {
                 fileeof = 1;
                 fputs( "\n" , target);
                 fputs( "\\end{document}\n" , target);
                 fputs( "\n" , target);
               }


               ////////////////////////////
               // item content 
               else if (  ( lline[ 0 ] == '!' ) && ( lline[ 1 ] == '-' ) &&  ( lline[ 2 ] == ' ' ) )
               {
                 if ( tex_item_level == 1  )
                 {
                    tex_item_level = 2;
                    fputs( "\\begin{itemize}\n" , target);
                 }
                 fputs( "\\item " , target);
                 fputs( strcut( lline, 2 +2 , strlen( lline )) , target);
                 fputs( "\n" , target);
               }



               ////////////////////////////
               // !fig pic.png 
               else if ( ( lline[ 0 ] == '!' ) && ( lline[ 1 ] == 'f' ) &&  ( lline[ 2 ] == 'i' ) 
               &&  ( lline[ 3 ] == 'g' ) && ( lline[ 4 ] == ' ' ) )
               {
                 fputs( "\n" ,  target);
                 fputs( "\\begin{center}\n" , target);
                 fputs( "\\includegraphics[width=1.0\\textwidth]{" , target);
                 fputs( strcut( lline, 4 +2 , strlen( lline )) , target);
                 fputs( "}\n" , target);
                 fputs( "\\end{center}\n" , target);
               }


               ////////////////////////////
               // !fig{  to myfig 
               else if ( ( lline[ 0 ] == '!' ) && ( lline[ 1 ] == 'f' ) &&  ( lline[ 2 ] == 'i' ) 
               &&  ( lline[ 3 ] == 'g' ) && ( lline[ 4 ] == '{' ) )
               {
                 fputs( "\n" ,  target);
                 fputs( "\\myfig{" , target);
                 fputs( strcut( lline, 4 +2 , strlen( lline )) , target);
                 fputs( "\n" , target);
               }


               ////////////////////////////
               // !section 
               else if (  ( lline[ 0 ] == '!' ) && ( lline[ 1 ] == 's' ) &&  ( lline[ 2 ] == 'e' ) 
               &&  ( lline[ 3 ] == 'c' ) && ( lline[ 4 ] == 't' ) &&  ( lline[ 5 ] == 'i' ) 
               &&  ( lline[ 6 ] == 'o' ) && ( lline[ 7 ] == 'n' ) &&  ( lline[ 8 ] == ' ' ) )
               {
                 fputs( "\n" ,  target);
                 fputs( "\\section{" , target);
                 fputs( strcut( lline, 8 +2 , strlen( lline )) , target);
                 fputs( "}\n" , target);
               }


               else
               {
                 if ( ( strcmp( lline, "" ) == 0 ) &&  ( tex_item_level == 1  ) )
                 {
                    tex_item_level = 0;
                    fputs( "\\end{itemize}\n" , target);
                 }

                 else if ( ( strcmp( lline, "" ) == 0 ) &&  ( tex_item_level == 2  ) )
                 {
                    tex_item_level = 0;
                    fputs( "\\end{itemize}\n" , target);
                    fputs( "\\end{itemize}\n" , target);
                 }
                 fputs( lline , target);
                 fputs( "\n" , target);
               }
              }
             }

             // clear
             lline[0]='\0';
             pcc = 0;

           }

           co = c; 
       }
   }
   fclose(source);
   fclose( target );
}









///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
int main( int argc, char *argv[])
{
    char fichier[PATH_MAX];
    char targetfile[PATH_MAX];
    char string[PATH_MAX];

    if ( argc == 2)
    if ( strcmp( argv[1] , "time" ) ==  0 ) 
    {
       printf("%d\n", (int)time(NULL));
       return 0;
    }

    struct winsize w; // need ioctl and unistd 
    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
    printf("Env HOME:  %s\n", getenv( "HOME" ));
    printf("Env PATH:  %s\n", getcwd( string, PATH_MAX ) );
    printf("Env TERM ROW:  %d\n", w.ws_row );
    printf("Env TERM COL:  %d\n", w.ws_col );

    ///////////////
    if ( argc == 1)
    {
       printf("Usage: please enter a file to use this software." );
       printf("\n" );
       return 0;
    }

    ///////////////
    if ( argc == 2)
      strncpy( fichier, argv[ 1 ] , PATH_MAX );

    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    if ( argc == 2)
    {
          strncpy( targetfile, fbasenoext( argv[ 1 ] ) , PATH_MAX );
          strncat( targetfile , ".tex" , PATH_MAX - strlen( targetfile ) -1 );
          printf( "> Target: %s\n" , targetfile );
          printf( "> Source: %s => Target: %s \n", argv[1] , targetfile  );
          printf( "\n" );
          printf( "\n" );
          readfileline( targetfile, argv[ 1 ] );
          printf( "\n" );
          printf( "\n" );
    }
    return 0;
}


