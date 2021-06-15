/*
 *
 * orio.h
 * Wednesday, 12/1/1993.
 *
 */

#define NAME       0
#define POP        1
#define MAXPOP     2
#define FACTORIES  3
#define BASES      4
#define FERTILITY  5
#define MINERAL    6
#define TERRAIN    7
#define XLOC       8
#define YLOC       9
#define LASTPOP    10
#define COLOR      11
#define SHIPCRED   12
#define DEFCRED    13
#define INDEX      14
#define OWNER      15
#define SHIELD     16
#define BASEPOP    17
#define MODPOP     18

#define SHIP       20


#define NORMAL     0x10FF
#define HILITE     0x50FF
#define BASEFG     0x10FF
#define GRAYFG     0x0700


#define DELTA1     0xDD4
#define DELTA2     0x468


#define V10PLANETCOUNTPOS  0xE2C0

#define V12PLANETCOUNTPOS  0xE2D6

#define V12ADVANCENUMPOS   0x749E
#define V12ADVANCENUMINC   DELTA1

#define V12ADVANCELEVELPOS 0x744A

//#define V12FLEETPOS        0x74C4
#define V12FLEETPOS        0x74C6
#define V12FLEETINC        DELTA1

#define V12SHIPCOUNTPOS    0x74B8
#define V12SHIPCOUNTINC    DELTA1

#define V12ADVANCELISTPOS  0xC6EC
#define V12ADVANCELISTDEL  0x3C
#define V12ADVANCELISTINC  DELTA2

#define V12SHIPPOS         0xC410
#define V12SHIPINC         DELTA2

#define MAXPLANETS         108

/*--- planet struct ---*/
typedef struct
   {
   /*--- 0x00 ---*/
   char    szName [12];
   USHORT  uX;
   USHORT  uY;

   /*--- 0x10 ---*/
   USHORT  uColor;
   USHORT  uStarSize;
   USHORT  ux1a[3];
   USHORT  uBasePop;
   USHORT  uModPop;
   USHORT  uMaxPop;

   /*--- 0x20 ---*/
   USHORT  uTerrain;
   USHORT  ux2a;
   USHORT  ux2b;
   USHORT  uFertility;
   USHORT  uMineral;
   USHORT  ux2c;
   USHORT  uShipCredits;
   USHORT  ux2d;

   /*--- 0x30 ---*/
   USHORT  ux3a[3];
   USHORT  uOwner;
   USHORT  uMark;
   USHORT  uPop;
   USHORT  uLastPop;
   USHORT  uFactories;

   /*--- 0x40 ---*/
   USHORT  uNetProd;
   USHORT  uGrossProd;
   USHORT  ux4a[6];

   /*--- 0x50 ---*/
   USHORT  uFundLevels[5];
   USHORT  uShipInProduction;
   USHORT  uShipDestination;
   USHORT  uBases;

   /*--- 0x60 ---*/
   USHORT  uDefCredits;
   USHORT  ux6a [3];
   USHORT  uShield;
   USHORT  ux6b [3];

   /*--- 0x70 ---*/
   USHORT  ux7a;
   USHORT  uControlLocks[5];
   USHORT  ux7b[2];

   /*--- 0x80 ---*/
   USHORT  ux8a[8];
   USHORT  ux9a[8];
   USHORT  uxAa[8];
   USHORT  uxBa[4];

   USHORT  uIndex;       // not part of actual planet structure
   }  PLANET;
typedef PLANET *PPLANET;



/*--- input file information and data ---*/
typedef struct
   {
   PSZ     pszFile;
   USHORT  uVer;
   PPLANET ppl;
   USHORT  uRaces;
   USHORT  uNewAdvances [6][6];
   USHORT  uOldAdvances [6][6];
   } NFO;
typedef NFO *PNFO;


typedef struct
   {
   USHORT uLevel;
   USHORT uGot[6];
   PSZ  pszName;
   } OBJ;
typedef OBJ *POBJ;


typedef struct
   {
   char   szName[12];
   USHORT uUnknown[28];
   } SHP;
typedef SHP *PSHP;


typedef struct
   {
   USHORT uTypes;
   SHP   Ship[6];
   USHORT uShips   [MAXPLANETS][12];
   } FLEET;
typedef FLEET *PFLEET;





/*
 * from Oreo.c
 *
 *
 */
USHORT GetAtt (USHORT uVal, BOOL bHilite);



/*
 * from OreoHelp.c
 *
 *
 */
USHORT _cdecl Help (USHORT c);




/*
 *  from OreoTech.c
 *
 *
 */
void WriteTech (FILE *fp, USHORT uRaces);
void ReadTech (FILE *fp, USHORT uRaces);
void EditTech (USHORT uPlayer, PSZ pszPlayer);


