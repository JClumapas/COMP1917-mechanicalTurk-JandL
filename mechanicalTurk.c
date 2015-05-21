/*
 *  mechanicalTurk.c
 *
 *  Edited by Josfer and Lachlan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Game.h"
#include "mechanicalTurk.h"

#define MAX_CAMPUSES ? //i forgot the value
#define MAX_ARCS ? //i forgot the value
static action spinoff(action nextAction);
static action buildARC(action nextAction, int arcCounter,int currentPlayer);
static action buildCampus(action nextAction,int currentPlayer);
static action buildGO8(action nextAction, int GO8Counter, int currentPlayer);
action decideAction (Game g) {

   action nextAction;
   int currentTurn = getTurnNumber(g);
   int currentPlayer = getWhoseTurn(g);
   int bpsCounter = getStudents(g,currentPlayer,STUDENT_BPS);
   int bqnCounter = getStudents(g,currentPlayer,STUDENT_BQN);
   int mjCounter = getStudents(g,currentPlayer,STUDENT_MJ);
   int mtvCounter = getStudents(g,currentPlayer,STUDENT_MTV);
   int mmoneyCounter = getStudents(g,currentPlayer,STUDENT_MMONEY);

   int arcCounter = getARCs(g,currentPlayer);
   int campusCounter = getCampuses(g,currentPlayer);
   int totalCampuses = 0;
   totalCampuses = totalCampuses + getCampuses(g,UNI_A)
                   + getCampuses(g,UNI_B) + getCampuses(g,UNI_C);
   int GO8Counter = getGO8s(g,currentPlayer);

   //Actions for our first turn
   // <=3 because we don't know what playerNumber we end up as
   if (currentTurn <= 3){
      if ((bpsCounter >= 1)&&(bqnCounter >= 1)
         &&(mjCounter >= 1)&&(mtvCounter >= 1)
         &&(arcCounter ==2)){
         nextAction = buildCampus(nextAction);
      } else if ((bpsCunter >= 1)&&(bqnCounter >= 1)){
         nextAction = buildArc(nextAction,arcCounter);
      } else if ((mjCounter >= 1)&&(mtvCounter >= 1)&&(mmoneyCounter >= 1){
         nextAction = spinoff(nextAction);
      } else {
         nextAction.actionCode = PASS;
      }
   }else{
      if ((totalCampuses >= (0.7*MAX_CAMPUSES)&&(mjCounter >= 2)
         &&(mmoneyCounter >= 3)&&(GO8Counter <= 8){
         nextAction = buildGO8(nextAction,GO8Counter,currentPlayer);
      } else if ((bpsCounter >= 1)&&(bqnCounter >= 1)
         &&(mjCounter >= 1)&&(mtvCounter >= 1){
         nextAction = buildCampus(nextAction);
      } else if ((bpsCunter >= 1)&&(bqnCounter >= 1)){
         nextAction = buildArc(nextAction,arcCounter);
      } else if ((mjCounter >= 1)&&(mtvCounter >= 1)&&(mmoneyCounter >= 1){
         nextAction = spinoff(nextAction);
      } else {
         nextAction.actionCode = PASS;
      }
   }

   return nextAction;
}

static action spinoff(action nextAction){

   action doSpinoff = nextAction;
   doSpinoff.actionCode = START_SPINOFF;
   return doSpinoff;
}

static action buildARC(action nextAction, int arcCounter,int currentPlayer){
   //maybe we can work something out with your working path to decide
   //where to build
   //We may or may not need to have separate strategies depending
   //which playerNumber we end up
}

static action buildCampus(action nextAction,int currentPlayer){
   //similar to buildARC
}

static action buildGO8(action nextAction, int GO8Counter, int currentPlayer){
   //similar to buildARC and buildCampus
}
