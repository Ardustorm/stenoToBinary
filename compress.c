#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Colors */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"



char* stenoOrder = "#STKPWHRAO*-EUFRPBLGTSDZ";
/* int getStrokes(char*stroke, unsigned char **bytes); */
unsigned char * getStrokes(char*stroke, int *size);
void getBytes(char* stroke, unsigned char *bytes);

int main() {

   FILE * fin = fopen("./main.json", "r");
   FILE * fout = fopen("./out.bin", "wb");
   char* line = NULL;
   char* stroke = NULL;
   char* trans = NULL;
   size_t len = 0;
   int i = 0;
   int size = 0;
   unsigned char *bytes;

      
   if( fin == NULL || fout == NULL) {
      perror("file not found");
      exit(EXIT_FAILURE);
   }
   
   while( i < 6 && getline(&line, &len, fin) != -1 ) {
      /* printf("%s\n",line); */
      stroke = strtok(line, ":");
      trans = strtok(NULL, ",");

      if(trans) {			/* makes sure not null */
	 /* printf("~~%s~~ [][]" BLU "~~%s~~\n" RESET, stroke, trans); */

	 bytes = getStrokes(stroke, &size);

	 
	 fwrite(bytes, 1, size, fout);
	 fwrite(trans+3, 1, strlen(trans)-4, fout);
	 free(bytes);
	 /* getBytes(stroke, bytes); */
	 i++;
      }

   }

   if(line)
      free(line);

   fclose(fin);
   fclose(fout);

   return 0;
}


/* 
   Max series of strokes is 10 in plover default dictionary
   this function takes a string representing steno
   and a pointer to a byte array
   It fills the array with a byte representation of the stroke
and returns the size of the array
 */

unsigned char * getStrokes(char*stroke, int *size) {
   /* int getStrokes(char*stroke, unsigned char **bytes) { */
   
   /* int i = 0; */
   int numOfStrokes=0;
   char* ptr = NULL;
   unsigned char *bytes = NULL;
   *size = 0;
   
   /* printf("Stroke:" YEL " %-20s" RESET " Bytes: " RED, stroke); */
   ptr = strtok(stroke, "/");
   while( ptr != NULL) {
      *size = *size + 4;
      bytes = realloc(bytes, (*size) * sizeof(char));

      getBytes(ptr, &bytes[4*(numOfStrokes++)]);

      ptr = strtok(NULL, "/");
   }

   /* for (i = 0; i < numOfStrokes*3; i++){ */
   /*    printf(" %02X", bytes[i]); */
   /*    if(i%3 == 2) */
   /* 	 printf(" |"); */
   /* } */
   /* printf(RESET "\n"); */

   return bytes;
}


/* 
   This function takes a steno stroke and fills the
   given buffer with the TX Bolt protocol representation
   of the stroke (4 bytes total) There is some extra
   space in the upper bits to do fancy stuff (such as
   set all msb so that it is easy to find when seeking
   TX Bolt protocol:
   _Byte 0     Byte 1     Byte 2     Byte 3
   --HWPKTS   --UE*OAR   --GLBPRF   ---#ZDST

 */
void getBytes(char* stroke, unsigned char *bytes) {
   char** ptr = &stroke;
   bytes[0] = bytes[1] = bytes[2] = bytes[3] = 0x0;
   
   printf("stroke: %-8s ", stroke);

   /* remove leading quote or spaces */
   while(**ptr == ' ' || **ptr == '\"')
      (*ptr)++;

   /********** BYTE 0 **********/
   if(**ptr == 'S') { bytes[0] |= 0x1 << 0; (*ptr)++;}
   if(**ptr == 'T') { bytes[0] |= 0x1 << 1; (*ptr)++;}
   if(**ptr == 'K') { bytes[0] |= 0x1 << 2; (*ptr)++;}
   if(**ptr == 'P') { bytes[0] |= 0x1 << 3; (*ptr)++;}
   if(**ptr == 'W') { bytes[0] |= 0x1 << 4; (*ptr)++;}
   if(**ptr == 'H') { bytes[0] |= 0x1 << 5; (*ptr)++;}

   /********** BYTE 1 **********/
   if(**ptr == 'R') { bytes[1] |= 0x1 << 0; (*ptr)++;}
   if(**ptr == 'A') { bytes[1] |= 0x1 << 1; (*ptr)++;}
   if(**ptr == 'O') { bytes[1] |= 0x1 << 2; (*ptr)++;}
   if(**ptr == '*') { bytes[1] |= 0x1 << 3; (*ptr)++;}
   if(**ptr == 'E') { bytes[1] |= 0x1 << 4; (*ptr)++;}
   if(**ptr == 'U') { bytes[1] |= 0x1 << 5; (*ptr)++;}

   if(**ptr == '-') { (*ptr)++;} 	/* Increament pointer for hypen */

   /********** BYTE 2 **********/
   if(**ptr == 'F') { bytes[2] |= 0x1 << 0; (*ptr)++;}
   if(**ptr == 'R') { bytes[2] |= 0x1 << 1; (*ptr)++;}
   if(**ptr == 'P') { bytes[2] |= 0x1 << 2; (*ptr)++;}
   if(**ptr == 'B') { bytes[2] |= 0x1 << 3; (*ptr)++;}
   if(**ptr == 'L') { bytes[2] |= 0x1 << 4; (*ptr)++;}
   if(**ptr == 'G') { bytes[2] |= 0x1 << 5; (*ptr)++;}
      

   /********** BYTE 3**********/
   if(**ptr == 'T') { bytes[3] |= 0x1 << 0; (*ptr)++;}
   if(**ptr == 'S') { bytes[3] |= 0x1 << 1; (*ptr)++;}
   if(**ptr == 'D') { bytes[3] |= 0x1 << 2; (*ptr)++;}
   if(**ptr == 'Z') { bytes[3] |= 0x1 << 3; (*ptr)++;}
   if(**ptr == '#') { bytes[3] |= 0x1 << 4; (*ptr)++;}


   printf("bytes: %02x %02x %02x %02x\n", bytes[0], bytes[1], bytes[2], bytes[3]);
}

   
