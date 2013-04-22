#include "common.h"

char* my_itoa(int val, int base){	
  static char buf[32] = {0};
  int i = 30;
  for(; val && i ; --i, val /= base)
    buf[i] = "0123456789abcdef"[val % base];
  return &buf[i+1];
}
void poscpy(char *t,char *s,int a,int b){
   while ( a < b ) *(t++) = *(s + (a++));
   *t = 0x0;
}
 
char **split(char *str,char n,int *length){
   register int i, j;
   /* control */
   int len=strlen(str);
   int elements = 0, elpos = 0; 
   char **array;
   /* number of new itens in array */
   for(i=0;i<len;i++) if(str[i]==n) elements++;
   /* get the memory */
   array=(char **)calloc(elements,sizeof(char *));
   if(!array){
      printf("# Error in malloc(*).\n");
      return NULL;
   }
   /* the number of elements for the caller */
   *length=elements;
   /* lvl1
    *
    * @i = will be the start point for copy
    */
   for(i=0;i<len;i++)
     /* lvl2
      *
      * @j = will be end point for copy
      */
     for(j=i;j<=len;j++)
       /* found splitChar or EoL */
       if(str[j]==n){
	 /*
	  * @i has start point
	  * @j has end point
	  */  
	 array[elpos]=(char *)malloc((j-i+1)*sizeof(char));
	 if ( !array[ elpos ] ){
	   printf("# lvl2\n");
	   printf(" # Error in malloc().\n");
	   return NULL;
	 }
	 /* copy the string into the array */
	 poscpy(array[elpos],str,i,j);
	 /* increment array position */
	 elpos++;
	 /* after the copy is done,
	  * @i must be equal to @j
	  */
	 i=j;
	 /* end loop lvl2 */
	 break;
       }
   /* return array */
   return array;
}
