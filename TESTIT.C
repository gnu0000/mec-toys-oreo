/*
 *
 * testit.c
 * Wednesday, 12/1/1993.
 *
 */

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char *argv[])
   {
   FILE   *fp1, *fp2, *fp3, *fp4;
   USHORT i, c1, c2, c3, c4;
   BOOL   bChg;

   if (!(fp1 = fopen (argv[1], "rb")))
      return printf ("Cannot open file1");
   if (!(fp2 = fopen (argv[2], "rb")))
      return printf ("Cannot open file2");
   if (!(fp3 = fopen (argv[3], "rb")))
      return printf ("Cannot open file3");
   if (!(fp4 = fopen (argv[4], "rb")))
      return printf ("Cannot open file4");

   bChg = argc < 6;
  
   for (i=0; i<59036; i++)
      {
      c1 = getc (fp1);
      c2 = getc (fp2);
      c3 = getc (fp3);
      c4 = getc (fp4);

      if (bChg)
         {
         if (c1 == c2 && c3 == c4 && c1 != c3)
            printf ("Offset: %4.4X   1&2: %2.2X   3&4: %2.2X\n", i, c1, c3);
         }
      else
         {
         if (c1 == c2 && c3 != c1 && c4 != c1 && c3 != c4)
            printf ("Offset: %4.4X   1&2: %2.2X   3: %2.2X   4: %2.2X\n", i, c1, c3, c4);
         }
      }
   fclose (fp1);
   fclose (fp2);
   fclose (fp3);
   fclose (fp4);
   printf ("Done.");
   return 0;
   }

