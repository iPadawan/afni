/*******************************************************
 * 3dNotes                                             *
 * T. Ross 8/99                                        *
 * -- Modified by RWCox to use thd_notes.c functions   *
 *******************************************************/

#include "mrilib.h"

void Error_Exit(char *message) {
        fprintf (stderr, "\n\nError: %s\n", message);
        exit(1);
}


void Show_Help(void) {
   fprintf(stderr, 
   "3dNotes - a program to add, delete and show notes for AFNI datasets.\n"
   "(c)1999 Medical College of Wisconsin\nby - T. Ross\n\n"
   "Usage: 3dNotes [-a \"string\"] [-d num] [-help] dataset\n\n"
   "Where:\n"
   "dataset   Afni compatible dataset [required].\n"
   "-a   \"str\"   Add the string \"str\" to the list of notes.\n"
   "      Note that you can use the standard C escape codes,\n"
   "      \\n for newline \\t for tab, etc.\n"
   "-d   num   deletes note number num.\n"
   "-help      Displays this screen.\n\n"
   "The default action, with no options, is to display the notes for the\n"
   "dataset.  If there are options, all deletions occur first and esentially\n"
   "simutaneously.  Then, notes are added in the order listed on the command\n"
   "line.  If you do something like -d 10 -d 10, it will delete both notes 10\n"
   "and 11.  Don't do that.\n\n"
   );
   exit(1);
}


void Display_Notes(THD_3dim_dataset *dset) {
   ATR_int *notecount;
   ATR_string *note;
   int num_notes, i, j, num_char;
   char note_name[20];
   char * chn , * chd ;

   notecount = THD_find_int_atr(dset->dblk, "NOTES_COUNT");
   if (notecount == NULL) 
      Error_Exit("There are no notes in the dataset");
   num_notes = notecount->in[0];
   for (i=1; i<= num_notes; i++) {
      chn = tross_Get_Note( dset , i ) ;
      if( chn == NULL )
         Error_Exit("Could not get the next note;"
                    " is there a problem with the HEAD file?");

      chd = tross_Get_Notedate(dset,i) ;
      if( chd == NULL ){
        printf("\n----- NOTE %d [no date] -----\n%s\n",i,chn) ;
      } else {
        printf("\n----- NOTE %d [%s] -----\n%s\n",i,chd,chn) ;
        free(chd) ;
      }
      free(chn) ;
   }
}
   

int main (int argc, char * argv[]) {
        
   THD_3dim_dataset *dset=NULL;
   int narg = 1, i, curr_note=0, curr_del=0;
   char *notes[MAX_DSET_NOTES];
   int delnotes[MAX_DSET_NOTES], delindex, delnum;

   if (argc == 1)   /* no file listed */
      Show_Help();

   for (i=0; i<MAX_DSET_NOTES; i++) {
      notes[i] = NULL;
      delnotes[i] = 0;
   }

        /* Loop over arguements and pull out what we need */
        while( narg < argc && argv[narg][0] == '-' ){

                if( strncmp(argv[narg],"-a",2) == 0 ) {
                        narg++;
                        if (narg==argc)
                                Error_Exit("-a must be followed by a string");
                        notes[curr_note++] = argv[narg++];
                        continue;       
                }

                if( strncmp(argv[narg],"-d",2) == 0 ) {
                        narg++;
                        if (narg==argc)
                                Error_Exit("-d must be followed by a integer");
                        delnotes[curr_del] = (int)atol(argv[narg++]);
                        if (delnotes[curr_del++] < 1)
                           Error_Exit("Cannot delete a note numbered < 1");
                        continue;       
                }

                if( strncmp(argv[narg],"-help",5) == 0 ) {
                        Show_Help();
                }
   }

   if( narg >= argc )
      Error_Exit("No input dataset!?\n") ;

        dset = THD_open_one_dataset( argv[narg] ) ;
        if( dset == NULL )
           Error_Exit("Cannot open dataset") ; 


   /* First, delete notes */
   do {
      delnum = 0;
      /* find the largest note to delete, 
         since numbering for those > than deleted changes */
      for(i=0; i<curr_del; i++)
         if (delnotes[i]>delnum) {
            delnum=delnotes[i];
            delindex = i;
         }
      if (delnum) {
         delnotes[delindex]=0;
         tross_Delete_Note(dset, delnum);
      }
   } while (delnum);  /* loop ends when no more to delete */

   /* Next, add notes */
   for (i=0; i<curr_note; i++)
      tross_Add_Note(dset, notes[i]);

   /* Display, if required */
   if ((curr_note == 0) && (curr_del == 0))
      Display_Notes(dset);
   else {
           THD_write_3dim_dataset( NULL,NULL , dset , False ) ;
           THD_delete_3dim_dataset( dset , False ) ; 
   }

}
