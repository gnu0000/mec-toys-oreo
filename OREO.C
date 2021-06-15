/*
 *
 * oreo.c
 * Monday, 11/22/1993.
 *
 */

#define INCL_VIO
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GnuScr.h>
#include <GnuMisc.h>
#include <GnuKbd.h>
#include <GnuArg.h>
#include "Oreo.h"


USHORT PaintColumn (PGW pgw, USHORT uItemCol);

typedef void (*pfnShow) (struct _fns *, USHORT, USHORT, USHORT, USHORT);
typedef void (*pfnEdit) (struct _fns *, USHORT, USHORT, USHORT, USHORT, int);

typedef struct _fns
   {
   USHORT  uType;      // column id
   PSZ     pszLabel1;  // top label for this column
   PSZ     pszLabel2;  // bottom label for this column
   pfnShow pfnShow;    // function to display col
   pfnEdit pfnChange;  // function to edit col
   USHORT  uRace;      //
   USHORT  uExtra;     //
   } FNS;
typedef FNS *PFNS;


/***********************************************************************/

void ShowText  (PFNS, USHORT, USHORT, USHORT, USHORT);
void ShowNum   (PFNS, USHORT, USHORT, USHORT, USHORT);
void EditNum   (PFNS, USHORT, USHORT, USHORT, USHORT, int);
void EditChoice(PFNS, USHORT, USHORT, USHORT, USHORT, int);
void EditName  (PFNS, USHORT, USHORT, USHORT, USHORT, int);


PFNS fns = NULL;

PGW    pgw  = NULL;  // the list window
PNFO   pnfo = NULL;  // the planet data

USHORT uPLANETS = 48;

USHORT uCOLS     = 0;  // count of cols
USHORT uSHOWCOLS = 8;  // displayed cols
USHORT uLEFTCOL  = 0;  // leftmost col
USHORT uSELCOL   = 0;  // selected col
USHORT uCOLSIZE  = 9;  // size of col
BOOL   bREVERSE = FALSE;


PSZ pszFertility[] = {"Hostile", "Normal",  "Fertile", "Gaia",
                      NULL,      "INVALID", "INVALID", "INVALID",
                      "INVALID", "INVALID", "INVALID", "INVALID",
                      "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"};

PSZ pszMineral[]   = {"U-Poor",  "Poor",    "Normal",  "Artifact",
                      "Rich",  "U-Rich",    "4xTech",  NULL,
                      "INVALID", "INVALID", "INVALID", "INVALID",
                      "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"};

PSZ pszTerrain[]   = {"NONE",    "Radiated","Toxic",   "Inferno",
                      "Toxic",   "Tundra",  "Barren",  "Minimal",
                      "Desert",  "Steppe",  "Arrid",   "Ocean",
                      "Jungle",  "Terran",   NULL,     "INVALID", "INVALID"};

PSZ pszColor[]     = {"Yellow",  "Red",     "Green",   "White",
                      "Blue",    "Neutron", NULL,      "INVALID",
                      "INVALID", "INVALID", "INVALID", "INVALID",
                      "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"};

PSZ pszOwner[]     = {"Player",  "Race#1",  "Race#2",  "Race#3",  
                      "Race#4",  "Race#5",  "Race#6",  "Nobody",
                      NULL,      "INVALID", "INVALID", "INVALID",
                      "INVALID", "INVALID", "INVALID", "INVALID", "Nobody"};

PSZ pszShield[]    = {"None",    "Class V", "Class X", "ClassXV",
                      "ClassXX", NULL,      "INVALID", "INVALID",
                      "INVALID", "INVALID", "INVALID", "INVALID",
                      "INVALID", "INVALID", "INVALID", "INVALID", "INVALID"};

char     sz [512];
PSZ      pszBuff;
BYTE     bc[2];
BOOL     bSorted = FALSE; 
USHORT   uOfst, uFlt;

BOOL     bDEBUG = FALSE;
USHORT   uVER   = 0;
USHORT   uLastKey = 0;

FLEET    flts[6];

/******************************************************************/


PNFO FreeNFO (PNFO pnfo)
   {
   if (!pnfo)
      return pnfo;
   if (pnfo->ppl)
      free (pnfo->ppl);
   if (pnfo->pszFile)
      free (pnfo->pszFile);
   free (pnfo);
   return NULL;
   }


void add (PFNS pf, USHORT u1, PSZ p1, PSZ p2, pfnShow pfn1, pfnEdit pfn2, USHORT u2, USHORT u3)
   {
   pf->uType     = u1;
   pf->pszLabel1 = p1;
   pf->pszLabel2 = p2;
   pf->pfnShow   = pfn1;
   pf->pfnChange = pfn2;
   pf->uRace     = u2;
   pf->uExtra    = u3;
   }


/*
 * given a physical column, this
 * returns the logical column (array index)
 */
USHORT DisplayCol (USHORT uPhysCol)
   {
   if (!uPhysCol)
      return 0;

   return uPhysCol + uLEFTCOL;
   }

USHORT PhysCol (USHORT uLogicalCol)
   {
   return uLogicalCol - uLEFTCOL;
   }



void SetupScreen (PNFO pnfo)
   {
   PFNS   pfns;
   USHORT uRace, uType;

   if (fns)
      free (fns);

   uCOLS = 18;
   if (pnfo)
      for (uRace=0; uRace<pnfo->uRaces; uRace++)
         uCOLS +=flts[uRace].uTypes;

   pfns = fns = malloc (uCOLS * sizeof (FNS));

   add (pfns++ , NAME,      "Planet  ", "Name    ", ShowText, EditName  , 0, 0);
   add (pfns++ , OWNER,     "        ", "Owner   ", ShowText, EditChoice, 0, 0);
   add (pfns++ , FACTORIES, "        ", " Factory", ShowNum , EditNum   , 0, 0);
   add (pfns++ , BASES,     " Missile", "   Bases", ShowNum , EditNum   , 0, 0);
   add (pfns++ , SHIELD,    "        ", "Shield  ", ShowText, EditChoice, 0, 0);
   add (pfns++ , FERTILITY, "        ", "Ecology ", ShowText, EditChoice, 0, 0);
   add (pfns++ , MINERAL,   "        ", "Wealth  ", ShowText, EditChoice, 0, 0);
   add (pfns++ , SHIPCRED,  "    Ship", " Credits", ShowNum , EditNum   , 0, 0);
   add (pfns++ , DEFCRED,   " Defense", " Credits", ShowNum , EditNum   , 0, 0);
   add (pfns++ , POP,       "  Popul-", "   ation", ShowNum , EditNum   , 0, 0);
   add (pfns++ , BASEPOP,   "Base Max", "     Pop", ShowNum , EditNum   , 0, 0);
   add (pfns++ , MODPOP,    " Mod Max", "     Pop", ShowNum , EditNum   , 0, 0);
   add (pfns++ , MAXPOP,    "     Max", "     Pop", ShowNum , EditNum   , 0, 0);
   add (pfns++ , XLOC,      "   X    ", "Location", ShowNum , EditNum   , 0, 0);
   add (pfns++ , YLOC,      "   Y    ", "Location", ShowNum , EditNum   , 0, 0);
   add (pfns++ , TERRAIN,   "        ", "Terrain ", ShowText, EditChoice, 0, 0);
   add (pfns++ , COLOR,     "Star    ", "Color   ", ShowText, EditChoice, 0, 0);

   if (pnfo)
      for (uRace=0; uRace<pnfo->uRaces; uRace++)
         for (uType=0; uType<flts[uRace].uTypes; uType++)
            add (pfns++, SHIP, pszOwner[uRace], flts[uRace].Ship[uType].szName, ShowNum , EditNum, uRace, uType);

   add (pfns++ , INDEX,     "    Star", "   Index", ShowNum , EditNum   , 0, 0);
   }


USHORT GetIdx (USHORT uItem, USHORT uRow,  USHORT  uCol,
               USHORT uAtt,  int iAutoInc, PSZ    *ppsz,
               PUSHORT puResult)
   {
   PGW    pgw2;
   PMET   pmet;
   USHORT uYPos, uXPos, uItems, uMaxSize = 0, c;


   for (uItems=0; ppsz[uItems]; uItems++)
      uMaxSize = max (uMaxSize, strlen (ppsz[uItems]));

   if (iAutoInc)
      {
      if (!*puResult && iAutoInc<0)
         return *puResult = uItems -1;
      return *puResult = (*puResult + iAutoInc) % uItems;
      }

   pmet = ScrGetMetrics ();

   uYPos = min (uRow - 1, pmet->uYSize - uItems   - 3);
   uXPos = min (uCol,     pmet->uXSize - uMaxSize - 5);

   pgw2 = GnuCreateWin2 (uYPos, uXPos, uItems+2, uMaxSize+4, NULL);
   pgw2->uClientXPos  += 1;
   pgw2->uClientXSize -= 2;
   pgw2->pUser1 = ppsz;
   pgw2->uItemCount = uItems;
   pgw2->uSelection = *puResult % uItems;
   GnuPaintWin (pgw2, 0xFFFF);

   while (TRUE)
      {
      if ((c = KeyGet (FALSE)) == '\x1B')
         break;
      if (c == '\x0D')
         {
         *puResult = pgw2->uSelection;
         break;
         }
      if (!GnuDoListKeys (pgw2, c))
          Beep (0);
      }
   GnuDestroyWin (pgw2);
   return *puResult;
   }



void ShowText (PFNS pfns, USHORT uItem, USHORT uRow, USHORT uCol, USHORT uAtt)
   {
   PSZ *pp;
   USHORT u;

   switch (pfns->uType)
      {
      case FERTILITY: pp = pszFertility; u = pnfo->ppl[uItem].uFertility; break;
      case MINERAL  : pp = pszMineral;   u = pnfo->ppl[uItem].uMineral;   break;
      case TERRAIN  : pp = pszTerrain;   u = pnfo->ppl[uItem].uTerrain;   break;
      case COLOR    : pp = pszColor;     u = pnfo->ppl[uItem].uColor;     break;
      case OWNER    : pp = pszOwner;     u = pnfo->ppl[uItem].uOwner;     break;
      case SHIELD   : pp = pszShield;    u = pnfo->ppl[uItem].uShield/5;  break;
      case NAME     : GnuPaint2 (pgw, uRow, uCol, 0, uAtt, pnfo->ppl[uItem].szName, uCOLSIZE); return;
      default       : return;
      }
   GnuPaint2 (pgw, uRow, uCol, 0, uAtt, pp[min (u, 16)], uCOLSIZE);
   }


void ShowNum (PFNS pfns, USHORT uItem, USHORT uRow, USHORT uCol, USHORT uAtt)
   {
   switch (pfns->uType)
      {
      case POP      : sprintf (sz, "%u", pnfo->ppl[uItem].uPop);      break;
      case LASTPOP  : sprintf (sz, "%u", pnfo->ppl[uItem].uLastPop);  break;
      case BASEPOP  : sprintf (sz, "%u", pnfo->ppl[uItem].uBasePop);   break;
      case MODPOP   : sprintf (sz, "%u", pnfo->ppl[uItem].uModPop);   break;
      case MAXPOP   : sprintf (sz, "%u", pnfo->ppl[uItem].uMaxPop);   break;
      case FACTORIES: sprintf (sz, "%u", pnfo->ppl[uItem].uFactories);break;
      case BASES    : sprintf (sz, "%u", pnfo->ppl[uItem].uBases);    break;
      case XLOC     : sprintf (sz, "%u", pnfo->ppl[uItem].uX);        break;
      case YLOC     : sprintf (sz, "%u", pnfo->ppl[uItem].uY);        break;
      case SHIPCRED : sprintf (sz, "%u", pnfo->ppl[uItem].uShipCredits);break;
      case DEFCRED  : sprintf (sz, "%u", pnfo->ppl[uItem].uDefCredits); break;
      case INDEX    : sprintf (sz, "%u", pnfo->ppl[uItem].uIndex);     break;

      case SHIP      : sprintf (sz, "%u", flts[pfns->uRace].uShips[pnfo->ppl[uItem].uIndex][pfns->uExtra]);    break;
      }
   GnuPaint (pgw, uRow, uCol+uCOLSIZE-1, 1, uAtt, sz);
   }



void EditNum (PFNS pfns, USHORT uItem, USHORT uRow, USHORT uCol, USHORT uAtt, int iAutoInc)
   {
   PUSHORT pu;

   switch (pfns->uType)
      {
      case POP      : pu = &pnfo->ppl[uItem].uPop;         break;
      case LASTPOP  : pu = &pnfo->ppl[uItem].uLastPop;     break;
      case BASEPOP  : pu = &pnfo->ppl[uItem].uBasePop;     break;
      case MODPOP   : pu = &pnfo->ppl[uItem].uModPop;      break;
      case MAXPOP   : pu = &pnfo->ppl[uItem].uMaxPop;      break;
      case FACTORIES: pu = &pnfo->ppl[uItem].uFactories;   break;
      case BASES    : pu = &pnfo->ppl[uItem].uBases;       break;
      case XLOC     : pu = &pnfo->ppl[uItem].uX;           break;
      case YLOC     : pu = &pnfo->ppl[uItem].uY;           break;
      case SHIPCRED : pu = &pnfo->ppl[uItem].uShipCredits; break;
      case DEFCRED  : pu = &pnfo->ppl[uItem].uDefCredits;  break;
      case INDEX    : pu = &pnfo->ppl[uItem].uIndex;       break;

      case SHIP     : pu = &flts[pfns->uRace].uShips[pnfo->ppl[uItem].uIndex][pfns->uExtra]; break;
      }

   if (iAutoInc)
      *pu += iAutoInc;
   else
      {
      sprintf (sz, "%u", *pu);
      KeyEditCellMode (NULL, "\x48\x50", 2);
      KeyEditCell (sz, uRow, uCol, uCOLSIZE, uLastKey);
      KeyEditCellMode (NULL, NULL, 2);
      *pu = atoi (sz);
      }
   }




void EditChoice (PFNS pfns, USHORT uItem, USHORT uRow, USHORT uCol, USHORT uAtt, int iAutoInc)
   {
   switch (pfns->uType)
      {
      case FERTILITY: GetIdx (uItem, uRow, uCol, uAtt, iAutoInc, pszFertility, &(pnfo->ppl[uItem].uFertility)); break;
      case MINERAL  : GetIdx (uItem, uRow, uCol, uAtt, iAutoInc, pszMineral,   &(pnfo->ppl[uItem].uMineral));   break;
      case TERRAIN  : GetIdx (uItem, uRow, uCol, uAtt, iAutoInc, pszTerrain,   &(pnfo->ppl[uItem].uTerrain));   break;
      case COLOR    : GetIdx (uItem, uRow, uCol, uAtt, iAutoInc, pszColor,     &(pnfo->ppl[uItem].uColor));     break;
      case OWNER    : GetIdx (uItem, uRow, uCol, uAtt, iAutoInc, pszOwner,     &(pnfo->ppl[uItem].uOwner));
                      if (pnfo->ppl[uItem].uOwner > 6) pnfo->ppl[uItem].uOwner = 0xFFFF;              break;
      case SHIELD   : pnfo->ppl[uItem].uShield /= 5;
                      GetIdx (uItem, uRow, uCol, uAtt, iAutoInc, pszShield,    &(pnfo->ppl[uItem].uShield));
                      pnfo->ppl[uItem].uShield *= 5;                 break;
      }
   }

void EditName (PFNS pfns, USHORT uItem, USHORT uRow, USHORT uCol, USHORT uAtt, int iAutoInc)
   {
   if (iAutoInc)
      return;
   strcpy (sz, pnfo->ppl[uItem].szName);
   KeyEditCellMode (NULL, "\x48\x50", 2);
   KeyEditCell (sz, uRow, uCol, uCOLSIZE, 1);
   KeyEditCellMode (NULL, NULL, 2);
   memset (pnfo->ppl[uItem].szName, '\0', 14);
   strcpy (pnfo->ppl[uItem].szName, sz);
   }



USHORT GetAtt (USHORT uVal, BOOL bHilite)
   {
   USHORT uAtt;

   uAtt = (bHilite ? HILITE : NORMAL);
   
   uAtt |= (uVal > 8 ? GRAYFG : (15 - uVal) << 8);
   return uAtt;
   }


USHORT _cdecl PaintItem (PGW pgw, USHORT uItem, USHORT uRow)
   {
   USHORT i, uAtt;

   if (!pnfo || !pnfo->ppl || !pgw->uItemCount || uItem < pgw->uScrollPos ||
       uItem >= pgw->uScrollPos+DisplayItems(pgw))
      return GnuPaintNChar (pgw, uRow, 0, 0, 0,  ' ', uCOLSIZE*uSHOWCOLS);

   for (i=0; i<uSHOWCOLS; i++)
      {
      uAtt = GetAtt (pnfo->ppl[uItem].uOwner, uItem == pgw->uSelection && DisplayCol (i) == uSELCOL);

      GnuPaintNChar (pgw, uRow, i*uCOLSIZE, 0, uAtt,  ' ', 9);
//      fns[DisplayCol(i)].pfnShow (fns[DisplayCol(i)].uType, uItem, uRow, i*uCOLSIZE, uAtt);
      fns[DisplayCol(i)].pfnShow (fns+DisplayCol(i), uItem, uRow, i*uCOLSIZE, uAtt);
      }
   }


USHORT PaintColumn (PGW pgw, USHORT uItemCol)
   {
   USHORT i, uItem, uEnd, uAtt, uCol;

   if (!pnfo || !pnfo->ppl || !pgw->uItemCount)
      return 0;

   uEnd = min ((DisplayItems(pgw)) , pgw->uItemCount - pgw->uScrollPos);

   uCol = PhysCol (uItemCol);

   for (i=0; i<uEnd; i++)
      {
      uItem = i + pgw->uScrollPos;

      uAtt = GetAtt (pnfo->ppl[uItem].uOwner, uItem == pgw->uSelection && uItemCol == uSELCOL);

      GnuPaintNChar (pgw, i, uCol*uCOLSIZE, 0, uAtt,  ' ', 9);
//      fns[uItemCol].pfnShow (fns[uItemCol].uType, uItem, i, uCol*uCOLSIZE, uAtt);
      fns[uItemCol].pfnShow (fns+uItemCol, uItem, i, uCol*uCOLSIZE, uAtt);
      }
   }


void RefreshStatic (PNFO pnfo)
   {
   PMET   pmet;

   pmet = ScrGetMetrics ();

   /*--- cls all but list window area---*/
   VioScrollDn (0, 0, pgw->uYPos-1, 0xFFFF, 0xFFFF, bc, 0);
   VioScrollDn (BottomOf(pgw)+1, 0, 0xFFFF, 0xFFFF, 0xFFFF, bc, 0);

   GnuPaint (NULL, 0, 0, 3, 0, "--Masters of ORION Save File Editor--");
   GnuPaint (NULL, pmet->uYSize-4, 0, 0, 0, "  File: ");

   if (pnfo)
      {
      GnuPaint (NULL, pmet->uYSize-2, 3, 0, 0, "\\@01<F1>\\@00-Help  \\@01<F2>\\@00-Load  \\@01<F3>\\@00-Save  \\@01<Esc>\\@00-Exit  \\@01<Arrows>\\@00-Move  \\@01<Enter>\\@00-Edit");
      GnuPaint (NULL, pmet->uYSize-1, 3, 0, 0, "\\@01<+>\\@00-Inc  \\@01<->\\@00-Dec  \\@01<Alt-S>\\@00-Sort  \\@01<Alt-T>\\@00-Edit Technology  \\@01<Alt-H>\\@00-Hex Edit");

      sprintf (sz, "%s (ver 1.0%u, %u planets)", pnfo->pszFile, pnfo->uVer, uPLANETS);
      GnuPaint (NULL, pmet->uYSize-4, 9, 0, 1, sz);
      }
   else
      {
      GnuPaint (NULL, pmet->uYSize-2, 3, 0, 0, "\\@01<F1>\\@00-Help  \\@01<F2>\\@00-Load  <F3>-Save  <Esc>-Exit  <Arrows>-Move  <Enter>-Edit");
      GnuPaint (NULL, pmet->uYSize-1, 3, 0, 0, "<+>-Inc  <->-Dec  <Alt-S>-Sort  <Alt-T>-Edit Technology  <Alt-H>-Hex Edit");
      }
   }


void RefreshLabels (PNFO pnfo)
   {
   USHORT i, uJust, uAtt;

   /*--- repaint labels ---*/
   GnuPaintNChar (NULL, 2, 4, 0, 0,  ' ', uSHOWCOLS*uCOLSIZE);
   GnuPaintNChar (NULL, 3, 4, 0, 0,  ' ', uSHOWCOLS*uCOLSIZE);

   for (i=0; i<uSHOWCOLS; i++)
      {
      uAtt  = (fns[DisplayCol(i)].uType==SHIP ? GetAtt (fns[DisplayCol(i)].uRace, FALSE): 0);
      uJust = (fns[DisplayCol(i)].uType==SHIP ? 1 : 0);
      GnuPaint2 (NULL, 2, 4+i*uCOLSIZE+uJust*9, uJust, uAtt, fns[DisplayCol(i)].pszLabel1, 8);
      GnuPaint2 (NULL, 3, 4+i*uCOLSIZE+uJust*9, uJust, uAtt, fns[DisplayCol(i)].pszLabel2, 8);
      }
   GnuPaint (NULL, 3, 1, 0, 1, (uLEFTCOL ? ".." : "  "));
   GnuPaint (NULL, 3, 77, 0, 1, (uLEFTCOL+uSHOWCOLS < uCOLS ? ".." : "  "));
   }


void RefreshList (PNFO pnfo)
   {
   GnuPaintBorder (pgw);

   /*--- repaint window ---*/
   GnuPaintWin (pgw, 0xFFFF);
   RefreshLabels (pnfo);
   }


void RefreshLine (USHORT uItem, USHORT uCol1, USHORT uCol2)
   {
   PaintItem (pgw, uItem, uItem - pgw->uScrollPos);
   }


USHORT EditField (PNFO pnfo, int iAutoInc)
   {
   USHORT uCol, uRow;

   if (!pnfo || !pnfo->ppl || !pgw->uItemCount)
      return Beep (0);
   if (!fns[uSELCOL].pfnChange)
      return Beep (0);

   uRow = pgw->uClientYPos + pgw->uSelection - pgw->uScrollPos;
   uCol = pgw->uClientXPos + (PhysCol (uSELCOL))*uCOLSIZE;

//   fns[uSELCOL].pfnChange (fns[uSELCOL].uType, pgw->uSelection, uRow, uCol, 2, iAutoInc);
   fns[uSELCOL].pfnChange (fns+uSELCOL, pgw->uSelection, uRow, uCol, 2, iAutoInc);
   RefreshLine (pgw->uSelection, uCol, uCol);
   return 0;
   }


USHORT PropagateNumber (PNFO pnfo, PFNS pfns, USHORT uItem)
   {
   PUSHORT pu, pu2;
   USHORT  i, uOffset, uVal;

   if (!pnfo || !pnfo->ppl || !pgw->uItemCount)
      return Beep (0);

   switch (pfns->uType)
      {
      case POP      : pu = &pnfo->ppl[uItem].uPop;         break;
      case BASEPOP  : pu = &pnfo->ppl[uItem].uBasePop;      break;
      case MODPOP   : pu = &pnfo->ppl[uItem].uModPop;      break;
      case MAXPOP   : pu = &pnfo->ppl[uItem].uMaxPop;      break;
      case FACTORIES: pu = &pnfo->ppl[uItem].uFactories;   break;
      case BASES    : pu = &pnfo->ppl[uItem].uBases;       break;
      case XLOC     : pu = &pnfo->ppl[uItem].uX;           break;
      case YLOC     : pu = &pnfo->ppl[uItem].uY;           break;
      case LASTPOP  : pu = &pnfo->ppl[uItem].uLastPop;     break;
      case SHIPCRED : pu = &pnfo->ppl[uItem].uShipCredits; break;
      case DEFCRED  : pu = &pnfo->ppl[uItem].uDefCredits;  break;
      case FERTILITY: pu = &pnfo->ppl[uItem].uFertility;   break;
      case MINERAL  : pu = &pnfo->ppl[uItem].uMineral;     break;
      case TERRAIN  : pu = &pnfo->ppl[uItem].uTerrain;     break;
      case COLOR    : pu = &pnfo->ppl[uItem].uColor;       break;
      case OWNER    : pu = &pnfo->ppl[uItem].uOwner;       break;
      case SHIELD   : pu = &pnfo->ppl[uItem].uShield;      break;

      case SHIP:
         uVal = flts[pfns->uRace].uShips[pnfo->ppl[uItem].uIndex][pfns->uExtra];
         for (i=uItem+1; i<uPLANETS; i++)
            flts[pfns->uRace].uShips[pnfo->ppl[i].uIndex][pfns->uExtra] = uVal;
         PaintColumn (pgw, uSELCOL);
         return 0;

      default       : return Beep (0);
      }
   pu2 = (PUSHORT) (PVOID) (pnfo->ppl + uItem);
   uOffset = pu - pu2;

   for (i=uItem; i<uPLANETS; i++)
      {
      pu2 = (PUSHORT) (PVOID) (pnfo->ppl + i);
      pu2[uOffset] = *pu;
      }
   if (pfns->uType == OWNER)
      RefreshList (pnfo);
   else
      PaintColumn (pgw, uSELCOL);
   }


USHORT MaxBars (PNFO pnfo, USHORT uItem)
   {
   USHORT i;

   if (GnuMsgBox2 ("[Max Funding]", "[press 'Y' or 'N']", "YN\x0D\x1B",
         0, 0, 3, 1, "Are you sure you want to maximize all fund levels?") != 'Y')
      return 0;

   for (i=0; i<5; i++)
      {
      pnfo->ppl[uItem].uFundLevels[i]   = 100;
      pnfo->ppl[uItem].uControlLocks[i] = TRUE;
      }
   return 0;
   }






USHORT HexEdit (PNFO pnfo, USHORT uItem)
   {
   PGW      pgw2;
   USHORT   c, y, x, uScrX, uScrY;
   unsigned char ch, pszBuff[256];
   int      ix, iy;
   BOOL     bLoop;

   if (!pnfo || !pnfo->ppl || !pgw->uItemCount)
      return Beep (0);

   memcpy (pszBuff, (pnfo->ppl + pgw->uSelection), sizeof (PLANET) -2);

   pgw2 = GnuCreateWin (18, 60, NULL);
   pgw2->pszHeader = "[Edit Offset]";
   pgw2->pszFooter = "[<Enter>=accept <esc>=abort]";
   GnuPaintBorder (pgw2);

   /*--- Paint the window ---*/
   GnuPaint      (pgw2, 1, 5, 0, 0, "00 01 02 03  04 05 06 07  08 09 0A 0B  0C 0D 0E 0F");
   GnuPaintNChar (pgw2, 2, 1, 0, 0, '-', 16 * 3 + 6);
   GnuPaintBig   (pgw2, 3, 1, 12, 4, 0, 0, "00:\n10:\n20:\n30:\n40:\n50:\n"
                                           "60:\n70:\n80:\n90:\nA0:\nB0:\n");
   for (y=0; y<12; y++)
      for (x=0; x<16; x++)
         {
         if (y==11 && x>7)  // only half of the last line
            break;
         sz[0] = sz[1] = '0';
         sz[2] = '\0';
         ch = pszBuff [y * 16 + x];
         sz[0] += ch >> 4;
         sz[1] += ch & 0x0F;
         if (sz[0] > '9') sz[0] += 7;
         if (sz[1] > '9') sz[1] += 7;
         GnuPaint (pgw2, y+3, 5+x*3+x/4, 0, 0, sz);
         }

   /*--- Edit The Window ---*/
   ScrSaveCursor (TRUE);

   ix = iy = 0;
   x = y = 0;
   bLoop = TRUE;
   while (bLoop)
      {
      x += ix;
      if (x == 32)     x = 0,  iy = 1;
      if (x == 0xFFFF) x = 31, iy = -1;

      y += iy;
      if (y == 12)      y = 0;
      if (y == 0xFFFF)  y = 11;

      ix = iy = 0;
      uScrX = 5 + x + x/2 + x/8;
      uScrY = y + 3;

      GnuMoveCursor (pgw2, uScrY, uScrX);

      c = KeyGet (TRUE);

      switch (c)
         {
         case 0x14B:  ix = -1; break;        /*--- Left  Arrow ---*/
         case 0x14D:  ix =  1; break;        /*--- Right Arrow ---*/
         case 0x148:  iy = -1; break;        /*--- Up    Arrow ---*/
         case 0x150:  iy =  1; break;        /*--- Down  Arrow ---*/
         case 0x0D:   bLoop=FALSE; break;    /*--- Enter       ---*/
         case 0x1B:                          /*--- Escape      ---*/
            ScrRestoreCursor ();
            GnuDestroyWin (pgw2);
            return 0;

         default:
            if (!strchr ("0123456789ABCDEF", c))
               Beep (0);
            else
               {
               sz[0] = (unsigned char) c;
               sz[1] = '\0';
               GnuPaint (pgw2, uScrY, uScrX, 0, 1, sz);
               ix = 1;
               }
         }
      }
   ScrRestoreCursor ();

   /*--- Read The Window ---*/
   for (y=0; y<12; y++)
      {
      uScrY = TopOf (pgw2) + 4 + y;

      for (x=0; x<16; x++)
         {
         uScrX = LeftOf (pgw2) + 6 + x*3 + x/4;
         c = 2;
         VioReadCharStr (sz, &c, uScrY, uScrX, 0);
         VioWrtCharStr ("..", 2, uScrY, uScrX, 0);

         ch = (char)((USHORT)(sz[0] - (sz[0] > '9' ? '7' : '0')) << 4);
         ch += sz[1] - (sz[1] > '9' ? '7' : '0');
         pszBuff [y * 16 + x] = ch;
         }
      }
   GnuDestroyWin (pgw2);
   memcpy ((pnfo->ppl + pgw->uSelection), pszBuff, 184);
   RefreshLine (uItem, 0, 0);
   return 0;
   }


void ReadFleet (FILE *fp, USHORT uRaces)
   {
   USHORT uRace;

   /*--- ReadShipInfo ---*/
   for (uRace=0; uRace<uRaces; uRace++)
      {
      fseek (fp, V12SHIPCOUNTPOS+ uRace * V12SHIPCOUNTINC, SEEK_SET);
      fread (&flts[uRace].uTypes, sizeof (USHORT), 1, fp);

      fseek (fp, V12SHIPPOS+ uRace * V12SHIPINC, SEEK_SET);
      fread (&flts[uRace].Ship, sizeof (SHP) * 6, 1, fp);
      }

   /*--- ReadFleetInfo ---*/
   for (uRace=0; uRace<uRaces; uRace++)
      {
      fseek (fp, V12FLEETPOS+ uRace * V12FLEETINC, SEEK_SET);
      fread (flts[uRace].uShips, MAXPLANETS * 12 * 2, 1, fp);
      }
   }


USHORT ReadFileWindow (PNFO *ppnfo, PSZ psz)
   {
   FILE *fp;
   char szFile[80];
   USHORT i;

   if (psz)
      strcpy (szFile, psz);
   else
      if (!GnuFileWindow (szFile, "*.gam;*.bak", "Load File", pszBuff))
         return 0;

   if (*ppnfo)
      {
      FreeNFO (*ppnfo);
      }

   if (!(fp = fopen (szFile, "rb")))
      return GnuMsg ("File Error", "Unable to open input file", szFile);

   *ppnfo = malloc (sizeof (NFO));
   (*ppnfo)->pszFile = strdup (szFile);

   /*--- Read number of planets ---*/
   if (uVER==2) // File version 1.2
      {
      fseek (fp, V12PLANETCOUNTPOS, SEEK_SET);
      fread (&uPLANETS, sizeof (USHORT), 1, fp);
      (*ppnfo)->uVer = 2;
      }
   else if (uVER==1) // File version 1.1
      {
      fseek (fp, V10PLANETCOUNTPOS, SEEK_SET);
      fread (&uPLANETS, sizeof (USHORT), 1, fp);
      (*ppnfo)->uVer = 0;
      }
   else // unknown file version
      {
      (*ppnfo)->uVer = 0;
      fseek (fp, V10PLANETCOUNTPOS, SEEK_SET);
      fread (&uPLANETS, sizeof (USHORT), 1, fp);
      if (uPLANETS != 24 && uPLANETS != 48 &&
          uPLANETS != 70 && uPLANETS != 108)
         {
         fseek (fp, V12PLANETCOUNTPOS, SEEK_SET);
         fread (&uPLANETS, sizeof (USHORT), 1, fp);
         (*ppnfo)->uVer = 2;
         }
      }
   fseek (fp, 0, SEEK_SET);

   if (!((*ppnfo)->ppl = malloc (uPLANETS * sizeof (PLANET))))
      return GnuMsg ("Memory Error", "Unable to alloc sufficient memory for planets", "");

   for (pgw->uItemCount=0; pgw->uItemCount<uPLANETS; pgw->uItemCount++)
      {
      if (!fread ((*ppnfo)->ppl + pgw->uItemCount, sizeof (PLANET) - 2, 1, fp))
         return GnuMsg ("Read Error", "Unable to read planet", "");
      (*ppnfo)->ppl[pgw->uItemCount].uIndex = pgw->uItemCount;
      }
      
   /*--- Get Number of races in galaxy ---*/
   for ((*ppnfo)->uRaces = i=0; i<pgw->uItemCount; i++)
      if ((*ppnfo)->ppl[i].uOwner < 8)
         (*ppnfo)->uRaces = max ((*ppnfo)->uRaces, (*ppnfo)->ppl[i].uOwner);
   (*ppnfo)->uRaces++;

   if ((*ppnfo)->uVer == 2)
      {   
      ReadTech (fp, pnfo->uRaces);
      ReadFleet (fp, pnfo->uRaces);
      }

   fclose (fp);

   pgw->uSelection = 0;
   pgw->uScrollPos = 0;
   uLEFTCOL = 0;
   uSELCOL  = 0;

   SetupScreen (pnfo);

   RefreshStatic (pnfo);
   RefreshList (pnfo);

   return 0;
   }


void WriteFleet (FILE *fp, USHORT uRaces)
   {
   USHORT uRace;

   /*--- WriteShipInfo ---*/
   for (uRace=0; uRace<uRaces; uRace++)
      {
      fseek  (fp, V12SHIPPOS+ uRace * V12SHIPINC, SEEK_SET);
      fwrite (&flts[uRace].Ship, sizeof (SHP) * 6, 1, fp);
      }

   /*--- WriteFleetInfo ---*/
   for (uRace=0; uRace<uRaces; uRace++)
      {
      fseek (fp, V12FLEETPOS+ uRace * V12FLEETINC, SEEK_SET);
      fwrite (flts[uRace].uShips, MAXPLANETS * 12 * 2, 1, fp);
      }
   }


USHORT WriteFileWindow (PNFO pnfo)
   {
   FILE   *fp;
   USHORT i, j;
   PGW    pgw2;

   if (!pnfo || !pnfo->ppl || !pgw->uItemCount)
      return Beep (0);

   if (GnuMsgBox2 ("[Save]", "[press 'Y' or 'N']", "YN\x0D\x1B",
         0, 0, 3, 1, "Are you sure you want to save?") != 'Y')
      return 0;

   if (!(fp = fopen (pnfo->pszFile, "rb+")))
      return GnuMsg ("File Error", "Unable to open output file", pnfo->pszFile);

   pgw2 = GnuCreateWin (5, 20, NULL);
   GnuPaint (pgw2, 1, 0, 3, 0, "Saving ...");

   for (i=0; i<pgw->uItemCount; i++)
      {
      for (j=0; j < pgw->uItemCount; j++)   // items may have been resorted
         if (pnfo->ppl[j].uIndex == i)
            break;

      if (!fwrite (pnfo->ppl + j, sizeof (PLANET) - 2, 1, fp))
         return GnuMsg ("Write Error", "Unable to write planet", pnfo->ppl[i].szName);
      }

   if (pnfo->uVer == 2)
      {
      WriteTech  (fp, pnfo->uRaces);
      WriteFleet (fp, pnfo->uRaces);
      }

   GnuDestroyWin (pgw2);

   fseek (fp, 0, SEEK_END);
   fclose (fp);
   return 0;
   }



int _cdecl pfnNumCmp (PPLANET ppl1, PPLANET ppl2)
   {
   PUSHORT pu1, pu2;

   pu1 = uOfst + (PUSHORT) (PVOID) ppl1 ;
   pu2 = uOfst + (PUSHORT) (PVOID) ppl2 ;
   if (bREVERSE)
      return *pu2 - *pu1;
   else
      return *pu1 - *pu2;
   }


int _cdecl pfnNumCmp2 (PPLANET ppl1, PPLANET ppl2)
   {
   if (bREVERSE)
      return flts[uFlt].uShips[ppl2->uIndex][uOfst] -
             flts[uFlt].uShips[ppl1->uIndex][uOfst];
   else
      return flts[uFlt].uShips[ppl1->uIndex][uOfst] -
             flts[uFlt].uShips[ppl2->uIndex][uOfst];
   }


BOOL SwapPos (PNFO pnfo, PGW pgw, USHORT uNew, USHORT uOld)
   {
   PLANET pl;

   pl = pnfo->ppl[uNew];
   pnfo->ppl[uNew] = pnfo->ppl[uOld];
   pnfo->ppl[uOld] = pl;

   return GnuSelectLine (pgw, uNew, TRUE);
   }


USHORT SortPlanets (PNFO pnfo, PFNS pfns)
   {
   PUSHORT pu;

   if (!pnfo || !pnfo->ppl || !pgw->uItemCount)
      return Beep (0);

   bSorted = TRUE; 

   switch (pfns->uType)
      {
      case POP      : pu = &pnfo->ppl[0].uPop;         break;
      case BASEPOP  : pu = &pnfo->ppl[0].uBasePop;     break;
      case MODPOP   : pu = &pnfo->ppl[0].uModPop;      break;
      case MAXPOP   : pu = &pnfo->ppl[0].uMaxPop;      break;
      case FACTORIES: pu = &pnfo->ppl[0].uFactories;   break;
      case BASES    : pu = &pnfo->ppl[0].uBases;       break;
      case XLOC     : pu = &pnfo->ppl[0].uX;           break;
      case YLOC     : pu = &pnfo->ppl[0].uY;           break;
      case LASTPOP  : pu = &pnfo->ppl[0].uLastPop;     break;
      case SHIPCRED : pu = &pnfo->ppl[0].uShipCredits; break;
      case DEFCRED  : pu = &pnfo->ppl[0].uDefCredits;  break;
      case FERTILITY: pu = &pnfo->ppl[0].uFertility;   break;
      case INDEX    : pu = &pnfo->ppl[0].uIndex;       break;
      case MINERAL  : pu = &pnfo->ppl[0].uMineral;     break;
      case TERRAIN  : pu = &pnfo->ppl[0].uTerrain;     break;
      case COLOR    : pu = &pnfo->ppl[0].uColor;       break;
      case OWNER    : pu = &pnfo->ppl[0].uOwner;       break;
      case SHIELD   : pu = &pnfo->ppl[0].uShield;      break;
      case NAME     : qsort (pnfo->ppl, pgw->uItemCount, sizeof (PLANET), stricmp);
                      RefreshList (pnfo);
                      return 0;

      case SHIP     : uOfst = pfns->uExtra; uFlt = pfns->uRace;
                      qsort (pnfo->ppl, pgw->uItemCount, sizeof (PLANET), pfnNumCmp2);
                      RefreshList (pnfo);
                      return 0;

      default:   return Beep (0);
      }
   uOfst = pu - (PUSHORT) (PVOID) (pnfo->ppl);
   qsort (pnfo->ppl, pgw->uItemCount, sizeof (PLANET), pfnNumCmp);
   RefreshList (pnfo);
   return 0;
   }


//USHORT Test (PVOID p1, PVOID p2)
//   {
//   PUSHORT pu1, pu2;
//   USHORT  uBuff[256];
//   USHORT  uLen, uOff;
//
////   uOff = 28; //-- no
//     uOff = 26; //-- yes
//
//   uLen = (sizeof (PLANET) - 2) - uOff*2;
//
//   pu1 = (PUSHORT)p1 + uOff;
//   pu2 = (PUSHORT)p2 + uOff;
//
//   memcpy (uBuff, pu1, uLen);
//   memcpy (pu1,    pu2, uLen);
//   memcpy (pu2,   uBuff, uLen);
//   return 0;
//   }





USHORT HorizMove (PNFO pnfo, BOOL bRight)
   {
   if (!pnfo || !pnfo->ppl || !pgw->uItemCount)
      return Beep (0);

   if (bRight)
      {
      if (uSELCOL >= uCOLS-1)
         return Beep (0);
      uSELCOL++;
      if (uSELCOL >= uLEFTCOL + uSHOWCOLS)
         {
         uLEFTCOL++;

         VioScrollLf (pgw->uClientYPos, pgw->uClientXPos+uCOLSIZE,
                      pgw->uClientYPos + pgw->uClientYSize-1,
                      pgw->uClientXPos + pgw->uClientXSize-1,
                      uCOLSIZE, bc, 0);
         RefreshLine (pgw->uSelection, uSELCOL, uSELCOL-1);
         PaintColumn (pgw, DisplayCol (uSHOWCOLS-1));
         RefreshLabels (pnfo);
         }
      else
         RefreshLine (pgw->uSelection, uSELCOL, uSELCOL-1);
      }
   else
      {
      if (!uSELCOL)
         return Beep (0);
      uSELCOL--;
      if ((uSELCOL <= uLEFTCOL) && uLEFTCOL)
         {
         uLEFTCOL--;

         VioScrollRt (pgw->uClientYPos, pgw->uClientXPos+uCOLSIZE,
                      pgw->uClientYPos + pgw->uClientYSize-1,
                      pgw->uClientXPos + pgw->uClientXSize-1,
                      uCOLSIZE, bc, 0);
         RefreshLine (pgw->uSelection, uSELCOL, uSELCOL+1);
         PaintColumn (pgw, DisplayCol (1));
         RefreshLabels (pnfo);
         }
      else
         RefreshLine (pgw->uSelection, uSELCOL, uSELCOL+1);
      }
   return 0;
   }


void KeyLoop (void)
   {
   SHORT  c;
   USHORT uOwner;

   while (TRUE)
      {
      switch (uLastKey = c = KeyGet (TRUE))
         {
         case 0x13b:                         /*------ F1     Help    ------*/
         case 'H':                           /*------ H              ------*/
            Help (0);
            break;

         case 0x13c:                         /*------ F2     Load    ------*/
         case 'L':                           /*------ L              ------*/
         case 0x126:                         /*------ Alt-L          ------*/
            ReadFileWindow (&pnfo, NULL);
            break;

         case 0x13d:                         /*------ F3     Save    ------*/
         case 'S':                           /*------ S              ------*/
            WriteFileWindow (pnfo);
            break;

         case 27:                            /*------ F10    Exit    ------*/
         case 0x11:                          /*------ Ctl-Q          ------*/
         case 0x18:                          /*------ Ctl-X          ------*/
         case 0x12d:                         /*------ Alt-X          ------*/
            if (!pnfo || (GnuMsgBox2 ("[Exit]", "[press 'Y' or 'N']", "YN\x0D\x1B",
                0, 0, 3, 1, "Are you sure you want to quit?") == 'Y'))
               {
               ScrRestoreMode ();
               GnuClearWin (NULL, ' ', 0, FALSE);
               GnuMoveCursor (NULL, 0, 0);
               ScrShowCursor (TRUE);
               exit (0);
               }
            break;

         case 18:                            /*------ Ctl-R  Refresh ------*/
            RefreshStatic (pnfo);
            RefreshList (pnfo);
            break;

         case 13:                            /*------ <ENTER>  Edit  ------*/
         case 0x153:                         /*------ <DEL>          ------*/
         case 0x182:                         /*------ <INS>    Edit  ------*/
            uLastKey = 1;
            EditField (pnfo, 0);
            break;

         case '-':
            uLastKey = 1;
            EditField (pnfo, -1);
            break;

         case '+':
            uLastKey = 1;
            EditField (pnfo, 1);
            break;

         case 0x14b:                         /*------ left  arrow    ------*/
            HorizMove (pnfo, FALSE);
            break;

         case 0x14d:                         /*------ right arrow    ------*/
            HorizMove (pnfo, TRUE);
            break;

         case 0x174:                         /*------ ^right arrow    ------*/
            if (pnfo && uLEFTCOL  < uCOLS - uSHOWCOLS)
               {
               uLEFTCOL = uCOLS - uSHOWCOLS;
               uSELCOL  = uCOLS-1;
               RefreshLabels (pnfo);
               RefreshList (pnfo);
               }
            break;

         case 0x173:                         /*------ ^Left arrow    ------*/
            if (pnfo && uLEFTCOL)
               {
               uLEFTCOL = uSELCOL  = 0;
               RefreshLabels (pnfo);
               RefreshList (pnfo);
               }
            break;

         case 0x123:                         /*------ Alt-H hex edit planet-*/
            HexEdit (pnfo, pgw->uSelection);
            break;

         case 0x119:                         /*------ Alt-P propagate ---*/
            PropagateNumber (pnfo, fns+uSELCOL, pgw->uSelection);
            break;

         case 0x113:                         /*------ Alt-R reverse sort order ---*/
            bREVERSE = !bREVERSE;
            break;

         case 0x18D:     /*------ Ctl-UpArrow  bring to top ------*/
            SwapPos (pnfo, pgw, 0, pgw->uSelection);
            break;

         case 0x191:     /*------ Ctl-DnArrow  bring to bottom ------*/
            SwapPos (pnfo, pgw, pgw->uItemCount-1, pgw->uSelection);
            break;

         case 0x11f:                         /*------ Alt-S          ------*/
            SortPlanets (pnfo, fns+uSELCOL);
            break;

         case 0x114:                       /*------ Alt-T          ------*/
            if (!pnfo)
               Beep (0);
            else if (pnfo->uVer != 2)
               GnuMsg ("[Error]", "You need be patched to v1.02 or MOO", "");
            else if ((uOwner = pnfo->ppl[pgw->uSelection].uOwner) > 5)
               GnuMsg ("[Error]", "You need to be on a planet", "that has an owner");
            else
               {
               EditTech (uOwner, pszOwner [uOwner]);
               GnuClearWin (NULL, ' ', 0, FALSE);
               RefreshStatic (pnfo);
               RefreshList (pnfo);
               }
            break;

         case 0x132:                         /*------ Alt-M max fund ------*/
            MaxBars (pnfo, pgw->uSelection);
            break;

         default:
            if (GnuDoListKeys (pgw, c))   /*---- Move Selection up/dn------*/
               break;
            else if (!strchr ("0123456789", c))
               Beep (0);
            else
               EditField (pnfo, 0);
         }
      }

   }


void Usage (void)
   {
   printf ("OREO     Masters of ORION Save File Editor          v1.1  %s  %s\n\n", __DATE__, __TIME__);
   printf ("USAGE: OREO [options] [savefile]\n\n");
   printf ("WHERE: [options]  Are 0 or more of:\n\n");
   printf ("         /Lines=# .. Set Number of screen lines (25, 28, 50)\n");
   printf ("         /V12 ...... Save files are from MOO ver 1.2 (default is auto detect)\n");
   printf ("         /V10 ...... Save files are from MOO ver 1.0 (default is auto detect)\n");
   printf ("         /? ........ This help\n\n");
   printf ("       [savefile]  If specified is loaded immediately.  Masters of ORION\n");
   printf ("             save files are of the form save?.GAM where ? is 1 through 7.\n");

   exit (0);
   }


USHORT _cdecl DebugBreakfn (USHORT c)
   {
   _asm { int 3 };
   return 0;
   }


int _cdecl main (int argc, char *argv[])
   {
   PMET   pmet;
   PSZ    psz;
   USHORT uLines;

   ArgBuildBlk ("*^debug ? *^help *^lines% *^v12 *^v10");

   if (ArgFillBlk (argv))
      {
      fprintf (stderr, "%s\n", ArgGetErr ());
      Usage ();
      }

   if (ArgIs ("help") || ArgIs ("?"))
      Usage ();

   bDEBUG = ArgIs ("debug");
   if (ArgIs ("v10"))
      uVER = 1;

   if (ArgIs ("v12"))
      uVER = 2;

   ScrInitMetrics ();

   if (ArgIs ("lines"))
      {
      psz = ArgGet ("lines", 0);
      if ((uLines = atoi (psz)) && uLines < 61)
         ScrSetMode (uLines, 80);
      }

   bc[0] = ' ';
   bc[1] = GnuGetAtt (NULL, 0);

   GnuClearWin (NULL, ' ', 0, FALSE);

   pmet = ScrGetMetrics ();

//   for (uCOLS=0; fns[uCOLS].pszLabel1; uCOLS++)
//      ;
   SetupScreen (NULL);

   pszBuff = malloc (10240);

   /*---- Create List ----*/
   GnuPaintAtCreate (FALSE);
   pgw = GnuCreateWin2 (4, 2, pmet->uYSize-8, 76, PaintItem);
   gnuFreeDat (pgw);
   pgw->bShowScrollPos = TRUE;
   pgw->uClientXPos  += 1;
   pgw->uClientXSize -= 2;
   pgw->bShadow = FALSE;
   GnuClearWin (pgw, ' ', 0, TRUE);
   GnuPaintAtCreate (TRUE);
   GnuPaintBorder (pgw);
   RefreshStatic (pnfo);
   RefreshList (pnfo);
   ScrShowCursor (FALSE);
   uSHOWCOLS = pgw->uClientXSize / uCOLSIZE;

   if (bDEBUG)
      KeyAddTrap (22, DebugBreakfn);
   KeyAddTrap (0x13b, Help);

   if (ArgIs (NULL))
      ReadFileWindow (&pnfo, ArgGet (NULL, 0));

   KeyLoop ();
   return 0;
   }


