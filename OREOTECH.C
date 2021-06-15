/*
 *
 * oriotech.c
 * Wednesday, 12/1/1993.
 *
 */

#define INCL_VIO
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <GnuScr.h>
#include <GnuKbd.h>
#include <GnuMisc.h>
#include "Oreo.h"


PSZ szLabels[]= {"Computer",
                  "Construction",
                  "Force Field",
                  "Planetology",
                  "Propultion",
                  "Weapons"};



OBJ bjComp[] = 
  {                        
   {1 , 0, 0, 0, 0, 0, 0, "Battle Comp Mark 1"},
   {2 , 0, 0, 0, 0, 0, 0, "ECM Jammer Mark 1"},
   {4 , 0, 0, 0, 0, 0, 0, "Deep Space Scanner"},
   {5 , 0, 0, 0, 0, 0, 0, "Battle Comp Mark 2"},
   {7 , 0, 0, 0, 0, 0, 0, "ECM Jammer Mark 2"},
   {8 , 0, 0, 0, 0, 0, 0, "Improcved Robotics 3"},
   {10, 0, 0, 0, 0, 0, 0, "Battle Comp Mark 3"},
   {12, 0, 0, 0, 0, 0, 0, "ECM Jammer Mark 3"},
   {13, 0, 0, 0, 0, 0, 0, "Improved Space Scanner"},
   {15, 0, 0, 0, 0, 0, 0, "Battle Comp Mark 4"},
   {17, 0, 0, 0, 0, 0, 0, "ECM Jammer Mark 4"},
   {18, 0, 0, 0, 0, 0, 0, "Improcved Robotics 4"},
   {20, 0, 0, 0, 0, 0, 0, "Battle Comp Mark 5"},
   {22, 0, 0, 0, 0, 0, 0, "ECM Jammer Mark 5"},
   {23, 0, 0, 0, 0, 0, 0, "Advanced Space Scanner"},
   {25, 0, 0, 0, 0, 0, 0, "Battle Comp Mark 6"},
   {27, 0, 0, 0, 0, 0, 0, "ECM Jammer Mark 6"},
   {28, 0, 0, 0, 0, 0, 0, "Improcved Robotics 5"},
   {30, 0, 0, 0, 0, 0, 0, "Battle Comp Mark 7"},
   {32, 0, 0, 0, 0, 0, 0, "ECM Jammer Mark 7"},
   {34, 0, 0, 0, 0, 0, 0, "Hyperspace Communications"},
   {35, 0, 0, 0, 0, 0, 0, "Battle Comp Mark 8"},
   {37, 0, 0, 0, 0, 0, 0, "ECM Jammer Mark 8"},
   {38, 0, 0, 0, 0, 0, 0, "Improcved Robotics 6"},
   {40, 0, 0, 0, 0, 0, 0, "Battle Comp Mark 9"},
   {42, 0, 0, 0, 0, 0, 0, "ECM Jammer Mark 9"},
   {45, 0, 0, 0, 0, 0, 0, "Battle Comp Mark 10"},
   {46, 0, 0, 0, 0, 0, 0, "Oracle Interface"},
   {47, 0, 0, 0, 0, 0, 0, "ECM Jammer Mark 10"},
   {48, 0, 0, 0, 0, 0, 0, "Improved Robotics 7"},
   {49, 0, 0, 0, 0, 0, 0, "Technology Nullifier"},
   {50, 0, 0, 0, 0, 0, 0, "Battle Computer Mark 11"},
   {0 , 0, 0, 0, 0, 0, 0, NULL},
  };


OBJ bjCons[] = 
  {                        
   {1 , 0, 0, 0, 0, 0, 0, "Titanium Armor"},
   {3 , 0, 0, 0, 0, 0, 0, "Indistrial Tech 9"},
   {5 , 0, 0, 0, 0, 0, 0, "Reduced Waste 80%"},
   {8 , 0, 0, 0, 0, 0, 0, "Indistrial Tech 8"},
   {10, 0, 0, 0, 0, 0, 0, "Duralloy Armor"},
   {11, 0, 0, 0, 0, 0, 0, "Battle Suit"},
   {13, 0, 0, 0, 0, 0, 0, "Indistrial Tech 7"},
   {14, 0, 0, 0, 0, 0, 0, "Automated Repair Units"},
   {15, 0, 0, 0, 0, 0, 0, "Reduced Waste 60%"},
   {17, 0, 0, 0, 0, 0, 0, "Zortrium Armor"},
   {18, 0, 0, 0, 0, 0, 0, "Indistrial Tech 6"},
   {23, 0, 0, 0, 0, 0, 0, "Indistrial Tech 5"},
   {24, 0, 0, 0, 0, 0, 0, "Armored Exoskeleton"},
   {25, 0, 0, 0, 0, 0, 0, "Reduced Waste 40%"},
   {26, 0, 0, 0, 0, 0, 0, "Andrium Armor"},
   {28, 0, 0, 0, 0, 0, 0, "Indistrial Tech 4"},
   {33, 0, 0, 0, 0, 0, 0, "Indistrial Tech 3"},
   {34, 0, 0, 0, 0, 0, 0, "Tritanium Armor"},
   {35, 0, 0, 0, 0, 0, 0, "Reduced Waste 20%"},
   {36, 0, 0, 0, 0, 0, 0, "Advanced Damage Control"},
   {38, 0, 0, 0, 0, 0, 0, "Indistrial Tech 2"},
   {40, 0, 0, 0, 0, 0, 0, "Powered Armor"},
   {42, 0, 0, 0, 0, 0, 0, "Adamantium Armor"},
   {45, 0, 0, 0, 0, 0, 0, "Waste Elimination"},
   {50, 0, 0, 0, 0, 0, 0, "Neutronium Armor"},
   {0 , 0, 0, 0, 0, 0, 0, NULL},
  };

OBJ bjFeld[] = 
  {                        
   {1 , 0, 0, 0, 0, 0, 0, "Class 1 Deflector"},
   {4 , 0, 0, 0, 0, 0, 0, "Class 2 Deflector"},
   {8 , 0, 0, 0, 0, 0, 0, "Personal Deflector"},
   {10, 0, 0, 0, 0, 0, 0, "Class 3 Deflector"},
   {12, 0, 0, 0, 0, 0, 0, "Class 5 Planet Shield"},
   {14, 0, 0, 0, 0, 0, 0, "Class 4 Deflector"},
   {16, 0, 0, 0, 0, 0, 0, "Repulsor Beam"},
   {20, 0, 0, 0, 0, 0, 0, "Class 5 Deflector"},
   {21, 0, 0, 0, 0, 0, 0, "Personal Absorb Shield"},
   {22, 0, 0, 0, 0, 0, 0, "Class 10 Planet Shield"},
   {24, 0, 0, 0, 0, 0, 0, "Class 6 Deflector"},
   {27, 0, 0, 0, 0, 0, 0, "Cloaking Device"},
   {30, 0, 0, 0, 0, 0, 0, "Class 7 Deflector"},
   {31, 0, 0, 0, 0, 0, 0, "Zyro Shield"},
   {32, 0, 0, 0, 0, 0, 0, "Class 15 Planet Shield"},
   {34, 0, 0, 0, 0, 0, 0, "Class 9 Deflector"},
   {37, 0, 0, 0, 0, 0, 0, "Stasis Field"},
   {38, 0, 0, 0, 0, 0, 0, "Personal Barrier Shield"},
   {40, 0, 0, 0, 0, 0, 0, "Class 11 Deflector"},
   {42, 0, 0, 0, 0, 0, 0, "Class 20 Planet Shield"},
   {43, 0, 0, 0, 0, 0, 0, "Black Hole Generator"},
   {44, 0, 0, 0, 0, 0, 0, "Class 13 Deflector"},
   {46, 0, 0, 0, 0, 0, 0, "Lightning Shield"},
   {50, 0, 0, 0, 0, 0, 0, "Class 15 Deflector"},
   {0 , 0, 0, 0, 0, 0, 0, NULL},
  };

OBJ bjPlnt[] =
  {                        
   {1 , 0, 0, 0, 0, 0, 0, "Eco Restoration"},
   {2 , 0, 0, 0, 0, 0, 0, "Terraforming +10"},
   {3 , 0, 0, 0, 0, 0, 0, "Barren Landings"},
   {5 , 0, 0, 0, 0, 0, 0, "Improved Eco Restoration"},
   {6 , 0, 0, 0, 0, 0, 0, "Tundra Landings"},
   {8 , 0, 0, 0, 0, 0, 0, "Terraforming +20"},
   {9 , 0, 0, 0, 0, 0, 0, "Dead Planet Landings"},
   {10, 0, 0, 0, 0, 0, 0, "Death Spores"},
   {12, 0, 0, 0, 0, 0, 0, "Inferno Landings"},
   {13, 0, 0, 0, 0, 0, 0, "Enhanced Eco Restoration"},
   {14, 0, 0, 0, 0, 0, 0, "Terraforming +30"},
   {15, 0, 0, 0, 0, 0, 0, "Toxic Landings"},
   {16, 0, 0, 0, 0, 0, 0, "Soil Enrichment"},
   {17, 0, 0, 0, 0, 0, 0, "Bio Toxin Antidote"},
   {18, 0, 0, 0, 0, 0, 0, "Radioactive Landings"},
   {20, 0, 0, 0, 0, 0, 0, "Terraforming +40"},
   {21, 0, 0, 0, 0, 0, 0, "Cloning"},
   {22, 0, 0, 0, 0, 0, 0, "Atmospheric Terraform"},
   {24, 0, 0, 0, 0, 0, 0, "Advanced Eco Restoration"},
   {26, 0, 0, 0, 0, 0, 0, "Terraforming +50"},
   {27, 0, 0, 0, 0, 0, 0, "Doom Virus"},
   {30, 0, 0, 0, 0, 0, 0, "Advanced Soil Enrichment"},
   {32, 0, 0, 0, 0, 0, 0, "Terraforming +60"},
   {34, 0, 0, 0, 0, 0, 0, "Complete Eco Restoration"},
   {36, 0, 0, 0, 0, 0, 0, "Universal Antidote"},
   {38, 0, 0, 0, 0, 0, 0, "Terraforming +80"},
   {40, 0, 0, 0, 0, 0, 0, "Bio Terminator"},
   {42, 0, 0, 0, 0, 0, 0, "Advanced Cloning"},
   {44, 0, 0, 0, 0, 0, 0, "Terraforming +100"},
   {50, 0, 0, 0, 0, 0, 0, "Terraforming +120"},
   {0 , 0, 0, 0, 0, 0, 0, NULL},
  };


OBJ bjProp[] = 
  {                        
   {1 , 0, 0, 0, 0, 0, 0, "Retro Engines (Warp 1)"},
   {3 , 0, 0, 0, 0, 0, 0, "Hydrogen Cells (Rng 4)"},
   {5 , 0, 0, 0, 0, 0, 0, "Deuterium Cells (Rng 5)"},
   {6 , 0, 0, 0, 0, 0, 0, "Nuclear Engines (Warp 2)"},
   {9 , 0, 0, 0, 0, 0, 0, "Irridium Cells (Rng 6)"},
   {10, 0, 0, 0, 0, 0, 0, "Inertial Stabilizer"},
   {12, 0, 0, 0, 0, 0, 0, "Sub-Light Drive (Warp 3)"},
   {14, 0, 0, 0, 0, 0, 0, "Dotomite Crystal (Rng 7)"},
   {16, 0, 0, 0, 0, 0, 0, "Energy Pulser"},
   {18, 0, 0, 0, 0, 0, 0, "Fusion Drive (Warp 4)"},
   {19, 0, 0, 0, 0, 0, 0, "Uridium Cells (Rng 8)"},
   {20, 0, 0, 0, 0, 0, 0, "Warp Dissipator"},
   {23, 0, 0, 0, 0, 0, 0, "Reajax 2 Cells (Rng 9)"},
   {24, 0, 0, 0, 0, 0, 0, "Impulse Engines (Warp 5)"},
   {27, 0, 0, 0, 0, 0, 0, "Star Gates"},
   {29, 0, 0, 0, 0, 0, 0, "Trilithium Cell (Rng 10)"},
   {30, 0, 0, 0, 0, 0, 0, "Ion Drive (Warp 6)"},
   {34, 0, 0, 0, 0, 0, 0, "High Energy Focus"},
   {36, 0, 0, 0, 0, 0, 0, "~Matter Engines (Warp 7)"},
   {38, 0, 0, 0, 0, 0, 0, "Sub space Teleporter"},
   {40, 0, 0, 0, 0, 0, 0, "Ionic Pulsar"},
   {41, 0, 0, 0, 0, 0, 0, "Thorium Cells (Rng 99)"},
   {42, 0, 0, 0, 0, 0, 0, "Interphase drive (Warp 8)"},
   {43, 0, 0, 0, 0, 0, 0, "Sub space Interdictor"},
   {45, 0, 0, 0, 0, 0, 0, "Combat Transporters"},
   {46, 0, 0, 0, 0, 0, 0, "Inertial Nullifier"},
   {48, 0, 0, 0, 0, 0, 0, "Hyper Drive (Warp 9)"},
   {50, 0, 0, 0, 0, 0, 0, "Displacement Device"},
   {0 , 0, 0, 0, 0, 0, 0, NULL},
  };


OBJ bjWeap[] = 
  {
   {1 , 0, 0, 0, 0, 0, 0, "Lasers and Nukes"},
   {2 , 0, 0, 0, 0, 0, 0, "Hand Laser"},
   {4 , 0, 0, 0, 0, 0, 0, "Hyper-V Rockets"},
   {5 , 0, 0, 0, 0, 0, 0, "Gattling Laser"},
   {6 , 0, 0, 0, 0, 0, 0, "Anti-Missile Rockets"},
   {7 , 0, 0, 0, 0, 0, 0, "Neutron Pellet Gun"},
   {8 , 0, 0, 0, 0, 0, 0, "Hyper-X Missiles"},
   {9 , 0, 0, 0, 0, 0, 0, "Fusion Bomb"},
   {10, 0, 0, 0, 0, 0, 0, "Ion Cannon"},
   {11, 0, 0, 0, 0, 0, 0, "Scatter Pack V Rockets"},
   {12, 0, 0, 0, 0, 0, 0, "Ion Rifle"},
   {13, 0, 0, 0, 0, 0, 0, "Mass Driver"},
   {14, 0, 0, 0, 0, 0, 0, "Merculite Missiles"},
   {15, 0, 0, 0, 0, 0, 0, "Neutron Blaster"},
   {16, 0, 0, 0, 0, 0, 0, "Anti-Matter Bomb"},
   {17, 0, 0, 0, 0, 0, 0, "Graviton Beam"},
   {18, 0, 0, 0, 0, 0, 0, "Stinger Missiles"},
   {19, 0, 0, 0, 0, 0, 0, "Hard Beam"},
   {20, 0, 0, 0, 0, 0, 0, "Fusion Beam"},
   {21, 0, 0, 0, 0, 0, 0, "Ion Stream Projector"},
   {22, 0, 0, 0, 0, 0, 0, "Omega-V Bomb"},
   {23, 0, 0, 0, 0, 0, 0, "Anti-Matter Torpedos"},
   {24, 0, 0, 0, 0, 0, 0, "Fusion Rifle"},
   {25, 0, 0, 0, 0, 0, 0, "Megabolt Cannon"},
   {26, 0, 0, 0, 0, 0, 0, "Phasor"},
   {27, 0, 0, 0, 0, 0, 0, "Scatter Pack-7 Rockets"},
   {28, 0, 0, 0, 0, 0, 0, "Auto Blaster"},
   {29, 0, 0, 0, 0, 0, 0, "Pulson Missile"},
   {30, 0, 0, 0, 0, 0, 0, "Tachyon Beam"},
   {31, 0, 0, 0, 0, 0, 0, "Hand Phasor"},
   {32, 0, 0, 0, 0, 0, 0, "Gauss Auto Cannon"},
   {33, 0, 0, 0, 0, 0, 0, "Particle Beam"},
   {34, 0, 0, 0, 0, 0, 0, "Hercular Missiles"},
   {35, 0, 0, 0, 0, 0, 0, "Plasma Cannon"},
   {36, 0, 0, 0, 0, 0, 0, "Death Ray"},
   {37, 0, 0, 0, 0, 0, 0, "Disruptor"},
   {38, 0, 0, 0, 0, 0, 0, "Pulse Phasor"},
   {39, 0, 0, 0, 0, 0, 0, "Neutronium Bomb"},
   {40, 0, 0, 0, 0, 0, 0, "Hellfire Torpidos"},
   {41, 0, 0, 0, 0, 0, 0, "Zeon Missiles"},
   {42, 0, 0, 0, 0, 0, 0, "Plasma Rifle"},
   {43, 0, 0, 0, 0, 0, 0, "Proton Torpedos"},
   {44, 0, 0, 0, 0, 0, 0, "Scatter Pack X Missiles"},
   {45, 0, 0, 0, 0, 0, 0, "Tri-Focus Plasma"},
   {46, 0, 0, 0, 0, 0, 0, "Stellar Converter"},
   {47, 0, 0, 0, 0, 0, 0, "Neutronium Prjector"},
   {48, 0, 0, 0, 0, 0, 0, "Mauler Device"},
   {50, 0, 0, 0, 0, 0, 0, "Plasma Torpedos"},
   {0 , 0, 0, 0, 0, 0, 0, NULL},
  };

USHORT uPLAYER = 0;



POBJ ObjType (USHORT i)
   {
   switch (i)
     {
     case 0: return bjComp;
     case 1: return bjCons;
     case 2: return bjFeld;
     case 3: return bjPlnt;
     case 4: return bjProp;
     case 5: return bjWeap;
     }
   }




void ReadTech (FILE *fp, USHORT uRaces)
   {
   USHORT uRace, uTech, i, j;
   USHORT uAdvances[6];
   POBJ   pbj;
   CHAR   ch;

   for (uRace=0; uRace<uRaces; uRace++)
      {
      fseek (fp, V12ADVANCENUMPOS+ uRace * V12ADVANCENUMINC, SEEK_SET);
      fread (uAdvances, sizeof (USHORT), 6, fp);

      for (uTech=0; uTech<6; uTech++)
         {
         fseek (fp, V12ADVANCELISTPOS+ uRace*V12ADVANCELISTINC+ 
                    uTech*V12ADVANCELISTDEL, SEEK_SET);

         pbj = ObjType (uTech);

         for (i=0; pbj[i].pszName; i++)
            pbj[i].uGot[uRace] = FALSE;

         for (i=0; i<uAdvances[uTech]; i++)
            {
            fread (&ch, 1, 1, fp);
            for (j=0; pbj[j].pszName; j++)
               if (pbj[j].uLevel == (USHORT)ch)
                  {
                  pbj[j].uGot[uRace] = TRUE;
                  break;
                  }
            }
         }
      }
   }


void WriteTech (FILE *fp, USHORT uRaces)
   {
   USHORT uRace, uTech, i, uCount, uMax;
   POBJ   pbj;

   for (uRace=0; uRace<uRaces; uRace++)
      {
      /*--- write counts ---*/
      fseek (fp, V12ADVANCENUMPOS+ uRace * V12ADVANCENUMINC, SEEK_SET);

      for (uTech=0; uTech<6; uTech++)
         {
         pbj = ObjType (uTech);

         for (uCount = i = 0; pbj[i].pszName; i++)
            uCount += !!pbj[i].uGot[uRace];

         fwrite (&uCount, sizeof (USHORT), 1, fp);
         }

      /*--- Write Guess at Tech Levels ---*/
      fseek (fp, V12ADVANCELEVELPOS+
                 uRace*V12ADVANCENUMINC, SEEK_SET);
      for (uTech=0; uTech<6; uTech++)
         {
         pbj = ObjType (uTech);
         for (uMax=i= 0; pbj[i].pszName; i++)
            if (pbj[i].uGot[uRace])
               uMax = max (uMax, pbj[i].uLevel);
         fwrite (&uMax, sizeof (USHORT), 1, fp);
         }


      /*--- Write Lists ---*/
      for (uTech=0; uTech<6; uTech++)
         {
         fseek (fp, V12ADVANCELISTPOS+ 
                    uRace*V12ADVANCELISTINC+ 
                    uTech*V12ADVANCELISTDEL, SEEK_SET);

         pbj = ObjType (uTech);

         for (i = 0; pbj[i].pszName; i++)
            if (pbj[i].uGot[uRace])
               fwrite (&(pbj[i].uLevel), 1, 1, fp);
         }
      }
   }




USHORT _cdecl PaintTech (PGW pgw, USHORT uItem, USHORT uRow)
   {
   USHORT uAtt;
   POBJ   pbj;
   BOOL   bGot;

   pbj = pgw->pUser1;
   bGot = pbj[uItem].uGot[uPLAYER];

   uAtt = (bGot ? 1 : 0);

   if (uItem == pgw->uSelection) uAtt += 2;

   GnuPaintNChar (pgw, uRow, 0, 0, uAtt,  ' ', 25);

   if (uItem >= pgw->uItemCount)
      return 0;

   GnuPaintNChar (pgw, uRow, 0, 0, uAtt, (pbj[uItem].uGot[uPLAYER] ? '*':' '), 1);
   GnuPaint2 (pgw, uRow, 1, 0, uAtt, pbj[uItem].pszName, 24);
   return 1;
   }



void EvenTechnologies (USHORT uPLAYER)
   {
   USHORT i, uPlayer, uTech;
   POBJ   pbj;


   if (GnuMsgBox2 ("[Even up Technology]", "[press 'Y' or 'N']", "YN\x0D\x1B",
      0, 0, 3, 1, "Do you want to make this technoloty profile global to all races?") != 'Y')
      return;

   for (uTech=0; uTech<6; uTech++)
      {
      pbj = ObjType (uTech);

      for (i=0; pbj[i].pszName; i++)
         for (uPlayer=0; uPlayer<6; uPlayer++)
            pbj[i].uGot[uPlayer] = pbj[i].uGot[uPLAYER];
      }
   }



void DoTechKeys (PGW *pgws)
   {
   USHORT uSelections [6];
   USHORT uSelWin = 0;
   USHORT i, c, uSel;
   PGW    pgw2;
   POBJ   pbj;
   int    is;

   for (i=0; i<6; i++)
      uSelections[i] = 0;

   uSelWin = 5;
   is      = 1;

   pgw2 = pgws[uSelWin];
   pgw2->uSelection = 0;


   while (TRUE)
      {
      if (is)
         {
         uSelections[uSelWin] = pgw2->uSelection;
         pgw2->uSelection = 0xFFFF;
         GnuPaintWin (pgw2, uSelections[uSelWin]);

         uSelWin += is;
         if (uSelWin==0xFFFF) uSelWin=5; else if (uSelWin>5) uSelWin=0;
         is=0;

         pgw2 = pgws[uSelWin];
         pgw2->uSelection = uSelections[uSelWin];
         GnuPaintWin (pgw2, pgw2->uSelection);
         }

      uSel = pgw2->uSelection;
      pbj  = (POBJ) pgw2->pUser1;

      switch (c = KeyGet (TRUE))
         {
         case 0x14B:  /*--- Left  ---*/
            is = -1;
            break;

         case 0x14D:  /*--- Right ---*/
         case 0x09:   /*--- tab   ---*/
            is = 1;
            break;

         case 0x0D:   /*--- Enter ---*/
         case 0x1B:   /*--- Esc   ---*/
            return;
            break;

         case ' ':
            pbj[uSel].uGot[uPLAYER] = !pbj[uSel].uGot[uPLAYER];
            GnuPaintWin (pgw2, uSel);
            GnuDoListKeys (pgw2, 0x150);  /*--- down arrow ---*/
            break;

         case '+':
            for (i=uSel+1; i; i--)
               pbj[i-1].uGot[uPLAYER] = TRUE;
            GnuPaintWin (pgw2, 0xFFFF);
            break;

         case '-':
            for (i=uSel; pbj[i].pszName; i++)
               pbj[i].uGot[uPLAYER] = FALSE;
            GnuPaintWin (pgw2, 0xFFFF);
            break;

         case 'G':
            EvenTechnologies (uPLAYER);
            break;

         default:
            if (!GnuDoListKeys (pgw2, c))
               Beep (0);
         }
      }
   }


void DrawWinOutlines (USHORT uStartLine, USHORT uSize)
   {
   USHORT y = uStartLine;

   GnuPaint (NULL, y, 0, 0, 0,    "ษอออออออออออออออออออออออออหอออออออออออออออออออออออออหอออออออออออออออออออออออออป");

   for (y++; uSize-2; y++, uSize--)
      GnuPaint (NULL, y, 0, 0, 0, "บ                         บ                         บ                         บ");

   GnuPaint (NULL, y, 0, 0, 0,    "ศอออออออออออออออออออออออออสอออออออออออออออออออออออออสอออออออออออออออออออออออออผ");
   }


void DrawStaticTech (PSZ pszPlayer)
   {
   PMET   pmet;
   USHORT uAtt;

   uAtt = GetAtt (uPLAYER, FALSE);

   pmet = ScrGetMetrics ();

   GnuPaint (NULL, 0, 0, 3, 0, "--Masters Of ORION Technology Editor--");

   GnuPaint (NULL, pmet->uYSize-1, 3,  0, 0, "Player: ");
   GnuPaint (NULL, pmet->uYSize-1, 10, 0, uAtt, pszPlayer);
   GnuPaint (NULL, pmet->uYSize-1, 20, 0, 0, "\\@01<Arrows>\\@00-Move \\@01<Spc>\\@00-Toggle \\@01<Enter>\\@00-Done \\@01<+><-><G>\\@00");
   }



void EditTech (USHORT uPlayer, PSZ pszPlayer)
   {
   PGW  pgws[6];
   PMET pmet;
   USHORT i, j, x, y;
   USHORT uWinSize, uTopWinPos, uBotWinPos;

   uPLAYER = uPlayer;

   pmet = ScrGetMetrics ();

   GnuClearWin (NULL, ' ', 0, FALSE);

   uWinSize = (pmet->uYSize - 5) / 2 - 1;
   uTopWinPos = 3;
   uBotWinPos = uTopWinPos + uWinSize + 2;

   DrawWinOutlines (uTopWinPos, uWinSize);
   DrawWinOutlines (uBotWinPos, uWinSize);
   DrawStaticTech (pszPlayer);

   /*---- Create Lists ----*/
   GnuPaintAtCreate (FALSE);
   for (i=0; i<6; i++)
      {
      y = (i<3 ? uTopWinPos : uBotWinPos);
      x = 1 + (i % 3) * 26;

      GnuPaint (NULL, y-1, x+1, 0, 0, szLabels[i]);

      pgws[i] = GnuCreateWin2 (y+1, x, uWinSize-2, 25, PaintTech);

      gnuFreeDat (pgws[i]);
      pgws[i]->bBorder = FALSE;
      pgws[i]->bShadow = FALSE;
      pgws[i]->uClientYPos  = pgws[i]->uYPos;
      pgws[i]->uClientXPos  = pgws[i]->uXPos;
      pgws[i]->uClientYSize = pgws[i]->uYSize;
      pgws[i]->uClientXSize = pgws[i]->uXSize;
      GnuClearWin (pgws[i], ' ', 0, TRUE);

      pgws[i]->pUser1 = ObjType (i);
      pgws[i]->pUser2 = malloc (sizeof (USHORT));

      *((PUSHORT)pgws[i]->pUser2) = i;

      for (j=0; ((POBJ)(pgws[i]->pUser1))[j].pszName; j++)
         ;
      pgws[i]->uItemCount = j;
      GnuPaintWin (pgws[i], 0xFFFF);
      }
   GnuPaintAtCreate (TRUE);

   DoTechKeys (pgws);

   for (i=0; i<6; i++)
      GnuDestroyWin (pgws[i]);
   }
