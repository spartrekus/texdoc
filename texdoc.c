
//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
#include <stdio.h>
#if defined(__linux__)
#define MYOS 1
#elif defined(_WIN32)
#define MYOS 2
#elif defined(_WIN64)
#define MYOS 3
#elif defined(__unix__)
#define MYOS 4
#define PATH_MAX 2500
#else
#define MYOS 0
#endif


/////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h> 
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
/////////////////////////////////


/////////////////////////////////
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


//////////////////
void ncp( char *filetarget,  char *  filesource )
{
  // fread
  char            buffer[1];
  size_t          n;
  size_t          m;
  FILE *fp;
  FILE *fp1; 
  FILE *fp2;
  int counter = 0 ; 
  int freader = 1 ; 
  int i , j ,posy, posx ; 
  //if ( fexist( filesource ) == 1 )
  {
        fp = fopen( filesource, "rb");
        fp2 = fopen( filetarget, "wb");
        counter = 0; 
        while(  !feof(fp) && ( freader == 1)   ) {
           if (  feof(fp)   ) {
                freader = 0 ; 
            }
            n = fread(  buffer, sizeof(char), 1 , fp);
            m = fwrite( buffer, sizeof(char), 1,  fp2);
        }
        fclose(fp2);
        fclose(fp);
      }
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



//////////////////////////////
 void maketex( char *thefile )
 {
       char targetfile[PATH_MAX];
       char cmdi[PATH_MAX];

       strncpy( targetfile, fbasenoext( thefile ) , PATH_MAX );
       strncat( targetfile , ".tex" , PATH_MAX - strlen( targetfile ) -1 );

       strncpy( cmdi , " bsdmrk " , PATH_MAX );
       strncat( cmdi , "  \"" , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi ,  thefile   , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , "\"  " , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , "  \"" , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi ,  targetfile   , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , "\"  " , PATH_MAX - strlen( cmdi ) -1 );
       system( cmdi );
 }



//////////////////////////////
void makedoc( char *thefile )
{
      if ( fexist( thefile ) == 1 )
      {
                        system( " pkill mupdf " );
                        char mytargetfile[PATH_MAX];
                        strncpy( mytargetfile, getenv( "HOME" ) , PATH_MAX );
                        strncat( mytargetfile , "/" , PATH_MAX - strlen( mytargetfile ) - 1);
                        strncat( mytargetfile , "doc.mrk" , PATH_MAX - strlen( mytargetfile ) - 1);
                        ncp( mytargetfile , thefile );
    	                system( " cd ; bsdmrk  doc.mrk ; pdflatex   -shell-escape --interaction=nonstopmode    doc ; bibtex doc ; pdflatex   -shell-escape --interaction=nonstopmode     doc ; pdflatex    -shell-escape --interaction=nonstopmode     doc " ); 
    	                system( " cd ; mupdf doc.pdf " );
     }
}





//////////////////////////////
 void makepdf( char *thefile )
 {
       char targetfile[PATH_MAX];
       char cmdi[PATH_MAX];

       strncpy( targetfile, fbasenoext( thefile ) , PATH_MAX );
       strncat( targetfile , ".tex" , PATH_MAX - strlen( targetfile ) -1 );

       strncpy( cmdi , " pdflatex -shell-escape --interaction=nonstopmode " , PATH_MAX );
       // strncpy( cmdi , " pdflatex -shell-escape --interaction=nonstopmode " , PATH_MAX );
       strncat( cmdi , "  \"" , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , targetfile , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , "\"  " , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
       system( cmdi );
 }



//////////////////////////////
 void viewpdf( char *thefile )
 {
       char targetfile[PATH_MAX];
       char cmdi[PATH_MAX];

       strncpy( targetfile, fbasenoext( thefile ) , PATH_MAX );
       strncat( targetfile , ".pdf" , PATH_MAX - strlen( targetfile ) -1 );

       strncpy( cmdi , " mupdf " , PATH_MAX );
       strncat( cmdi , "  \"" , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , targetfile , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , "\"  " , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
       system( cmdi );
 }



/*
              strncpy( fileselection, nwin_file[ winsel ] , PATH_MAX );
              if ( fexist( fileselection ) == 1 )
              {
                char mytargetfile[PATH_MAX];
                ////////////////////////////////////// 
                strncpy( mytargetfile, getenv( "HOME" ) , PATH_MAX );
                strncat( mytargetfile , "/" , PATH_MAX - strlen( mytargetfile ) - 1);
                //strncat( mytargetfile , fbasename( fileselection ) , PATH_MAX - strlen( mytargetfile ) - 1);
                strncat( mytargetfile , "doc.mrk" , PATH_MAX - strlen( mytargetfile ) - 1);
                ncp( mytargetfile , fileselection );
                ////////////////////////////////////// 
                ////////////////////////////////////// 
              }
  */



/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////
int main( int argc, char *argv[])
{
    int i ; 
    if ( argc >= 2)
    {
          for( i = 1 ; i < argc ; i++) 
          {
	      printf( "---------\n" );
	      printf( "Argument %d/%d: %s\n", i, argc-1 ,  argv[ i ] );
	      if ( fexist( argv[ i ] ) == 1 )
              {
	        printf( "==> Processing (File Found): %d/%d: %s\n", i, argc-1 ,  argv[ i ] );
                //maketex( argv[ i ] );
                //makepdf( argv[ i ] );
                //viewpdf( argv[ i ] );
                  makedoc( argv[ i ] );
              }
          }
    }
    return 0;
}





