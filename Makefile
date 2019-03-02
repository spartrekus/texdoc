

all:
	   gcc  bsdmrk.c -o bsdmrk 
	   gcc  texdoc.c -o texdoc 
	   gcc  medit.c -o medit -lncurses 
	    echo copy the binaries file to /usr/local/bin/ 
 

run:
	    echo press f6 to bring the pdf
	    medit example.mrk 

