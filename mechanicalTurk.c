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

//#define MAX_CAMPUSES 54
#define MAX_ARCS 92
#define PLAYER_A_PATH {'R','L','L','R','R','R','L','L','L','R','R','R','R','R','B','R','R','R','R','B','R','R','R','R','B','R','R','R','L','R','R','R','R','B','R','B','R','R','R','R','B','R','L','L','L','L','B','L','L','L','B','L','L','L','L','B','L','L','L','B','L','L','L','R','L','L','B','L','L','L','L','B','L','L','L','B','L','L','L','L','B','L','L','L','B','L','L','L','L','B','L','L'}
#define PLAYER_B_PATH {'R','L','L','R','R','B','R','R','R','R','B','R','R','R','R','B','R','R','R','R','B','R','R','R','R','L','B','L','L','L','L','L','R','L','R','L','L','L','L','L','B','L','B','L','L','L','L','B','L','L','L','L','B','L','L','L','B','L','L','L','L','B','L','L','L','B','L','L','L','L','B','L','L','L','B','L','L','L','L','B','L','L','L','B','L','L','L','R','L','L','L','L'}
#define PLAYER_C_PATH {'L','R','R','R','L','L','B','L','L','L','L','B','L','L','L','L','B','L','L','L','L','B','L','L','L','L','L','R','R','R','R','R','B','R','R','L','R','R','R','R','L','B','L','B','L','R','R','R','B','R','R','R','R','B','R','R','R','B','R','R','R','R','B','R','R','R','B','R','R','R','R','B','R','R','R','B','R','R','R','R','B','R','R','R','B','R','R','R','L','R','R','R'}
#define NUM_VERTEXES 54

#define TRUE 1
#define FALSE 0
#define NO_MATCH 0
#define NOT_CHOSEN -1

static action spinoff(action nextAction);
static action buildARC(Game g, action nextAction,int currentPlayer,int arcCounter);
static action buildCampus(Game g, action nextAction,int currentPlayer,int campusCounter);
static action buildGO8(Game g,action nextAction,int currentPlayer);
static action exchangeBPS(Game g,action nextAction,int mjCounter, int mtvCounter);
static action exchangeBQN(Game g,action nextAction,int mtvCounter,int mmoneyCounter);
static action exchangeMJ(Game g,action nextAction,int mmoneyCounter, int bpsCounter);
static action exchangeMTV(Game g,action nextAction,int bpsCounter,int bqnCounter);
static action exchangeMMONEY(Game g,action nextAction,int bqnCounter,int mjCounter);

action decideAction (Game g) {

   action nextAction;
   nextAction.actionCode = PASS;
   int i = 0;
   while (i < PATH_LIMIT) {
      nextAction.destination[i] = '\0';
      i++;
   }
   
   int currentTurn = getTurnNumber(g);
   int currentPlayer = getWhoseTurn(g);
   int bpsCounter = getStudents(g,currentPlayer,STUDENT_BPS);
   int bqnCounter = getStudents(g,currentPlayer,STUDENT_BQN);
   int mjCounter = getStudents(g,currentPlayer,STUDENT_MJ);
   int mtvCounter = getStudents(g,currentPlayer,STUDENT_MTV);
   int mmoneyCounter = getStudents(g,currentPlayer,STUDENT_MMONEY);

   int arcCounter = getARCs(g,currentPlayer);
   int campusCounter = getCampuses(g,currentPlayer);

   //Actions for our first turn
   if (currentTurn <= 3){
      if(currentPlayer == UNI_A){
	     if(arcCounter == 0){
   	        nextAction.actionCode = OBTAIN_ARC;
		    strncpy(nextAction.destination, "R", PATH_LIMIT);
			if ((isLegalAction(g,nextAction) == FALSE)){
				nextAction.actionCode = PASS;
			}
		 }else if (arcCounter == 1){
		 	nextAction.actionCode = OBTAIN_ARC;
			strncpy(nextAction.destination, "RL", PATH_LIMIT);
			if ((isLegalAction(g,nextAction) == FALSE)){
				nextAction.actionCode = PASS;
			}
		 }else if ((arcCounter == 2)&&(campusCounter == 2)){
		 	nextAction.actionCode = BUILD_CAMPUS;
			strncpy(nextAction.destination, "RL", PATH_LIMIT);
			if ((isLegalAction(g,nextAction) == FALSE)){
				nextAction.actionCode = PASS;
			}
		 }else{
		 	nextAction.actionCode = PASS;
		 }
	  }else if(currentPlayer == UNI_B){
	     if(arcCounter == 0){
   	        nextAction.actionCode = OBTAIN_ARC;
		    strncpy(nextAction.destination, "RRLRLL", PATH_LIMIT);
			if ((isLegalAction(g,nextAction) == FALSE)){
				nextAction.actionCode = PASS;
			}
		 }else if (arcCounter == 1){
		 	nextAction.actionCode = OBTAIN_ARC;
			strncpy(nextAction.destination, "RRLRLLL", PATH_LIMIT);
			if ((isLegalAction(g,nextAction) == FALSE)){
				nextAction.actionCode = PASS;
			}
		 }else if ((arcCounter == 2)&&(campusCounter == 2)){
		 	nextAction.actionCode = BUILD_CAMPUS;
			strncpy(nextAction.destination, "RRLRLLL", PATH_LIMIT);
			if ((isLegalAction(g,nextAction) == FALSE)){
				nextAction.actionCode = PASS;
			}
		 }else{
		 	nextAction.actionCode = PASS;
		 }
	  }else if(currentPlayer == UNI_C){
	     if(arcCounter == 0){
   	        nextAction.actionCode = OBTAIN_ARC;
		    strncpy(nextAction.destination, "LRLRLB", PATH_LIMIT);
			if ((isLegalAction(g,nextAction) == FALSE)){
				nextAction.actionCode = PASS;
			}
		 }else if (arcCounter == 1){
		 	nextAction.actionCode = OBTAIN_ARC;
			strncpy(nextAction.destination, "LRLRR", PATH_LIMIT);
			if ((isLegalAction(g,nextAction) == FALSE)){
				nextAction.actionCode = PASS;
			}
		 }else if ((arcCounter == 2)&&(campusCounter == 2)){
		 	nextAction.actionCode = BUILD_CAMPUS;
			strncpy(nextAction.destination, "LRLRR", PATH_LIMIT);
			if ((isLegalAction(g,nextAction) == FALSE)){
				nextAction.actionCode = PASS;
			}
		 }else{
		 	nextAction.actionCode = PASS;
		 }
	  }
   }else{
      if (bpsCounter >=4){
         nextAction = exchangeBPS(g,nextAction,mjCounter,mtvCounter);
      }
      if (bqnCounter >=4){
          nextAction = exchangeBQN(g,nextAction,mtvCounter,mmoneyCounter);
      }
      if (mjCounter >=5){
         nextAction = exchangeMJ(g,nextAction,mmoneyCounter,bpsCounter);
      }
      if (mtvCounter >=4){
         nextAction = exchangeMTV(g,nextAction,bpsCounter,bqnCounter);
      }
      if (campusCounter <=4 && mmoneyCounter >=3){
         nextAction = exchangeMMONEY(g,nextAction,bqnCounter,mjCounter);
      }else if(mmoneyCounter >= 6){
         nextAction = exchangeMMONEY(g,nextAction,bqnCounter,mjCounter);
      }
      if((nextAction.actionCode == PASS || nextAction.actionCode == RETRAIN_STUDENTS) && (mjCounter >= 2)&&(mmoneyCounter >= 3)&&(campusCounter > 0)){
         nextAction = buildGO8(g,nextAction,currentPlayer);
      }
      if ((nextAction.actionCode == PASS) && (bpsCounter >= 1)&&(bqnCounter >= 1)&&(mjCounter >= 1)&&(mtvCounter >= 1)){
         nextAction = buildCampus(g,nextAction,currentPlayer,campusCounter);
      }
      if (nextAction.actionCode == PASS && (bpsCounter >= 1)&&(bqnCounter >= 1)){
         nextAction = buildARC(g,nextAction,currentPlayer,arcCounter);
      }
      if (campusCounter >=5 && nextAction.actionCode == PASS && (mjCounter >= 1)&&(mtvCounter >= 1)&&(mmoneyCounter >= 1)){
         nextAction = spinoff(nextAction);
      }
   }
   return nextAction;
}

static action spinoff(action nextAction){

   action doSpinoff = nextAction;
   doSpinoff.actionCode = START_SPINOFF;
   return doSpinoff;
}
static action buildGO8(Game g,action nextAction,int currentPlayer){

   action newAction = nextAction;
   newAction.actionCode = BUILD_GO8;
   
   int i = 0;
   while (i < PATH_LIMIT) {
      newAction.destination[i] = '\0';
      i++;
   }
   char workingPath[92] = {'\0'};
   char workingPathA[92] = PLAYER_A_PATH;
   char workingPathB[92] = PLAYER_B_PATH;
   char workingPathC[92] = PLAYER_C_PATH;
   char tempPath[92] = {'\0'};
   
   if(currentPlayer ==  UNI_A){
      i = 0;
      while (i < 92) {
         workingPath[i] = workingPathA[i];
         i++;
      }
   }else if(currentPlayer == UNI_B){
      i = 0;
      while (i < 92) {
         workingPath[i] = workingPathB[i];
         i++;
      }
   }else if(currentPlayer == UNI_C){
      i = 0;
      while (i < 92) {
         workingPath[i] = workingPathC[i];
         i++;
      } 
   } 

   i = 0;
   while (i < 92) {
      tempPath[i] = '\0';
      i++;
   }

   
   int counter = 0;
   int moveMade = FALSE;
   tempPath[0] = workingPath[0];
   newAction.destination[0] = tempPath[0];
   while (moveMade == FALSE && counter < MAX_ARCS) {
      tempPath[counter] = workingPath[counter];
      newAction.destination[counter] = tempPath[counter];
      if (isLegalAction(g, newAction) == TRUE){
         moveMade = TRUE;
      }
      counter++;
   }
   if (moveMade == FALSE) {
      nextAction = spinoff(nextAction);
	  if ((isLegalAction(g,nextAction) == FALSE)){
         newAction.actionCode = PASS;
      }
   }
   return newAction;

}

static action buildARC(Game g, action nextAction,int currentPlayer,int arcCounter){
   
   action newAction = nextAction;
   newAction.actionCode = OBTAIN_ARC;
   int i = 0;
   while (i < PATH_LIMIT) {
      newAction.destination[i] = '\0';
      i++;
   }

   if(arcCounter >= 20){
      newAction.actionCode = PASS;
   } else {
      char workingPath[92] = {'\0'};
      char workingPathA[92] = PLAYER_A_PATH;
      char workingPathB[92] = PLAYER_B_PATH;
      char workingPathC[92] = PLAYER_C_PATH;
      char tempPath[92] = {'\0'};
      if(currentPlayer ==  UNI_A){
         i = 0;
         while (i < 92) {
            workingPath[i] = workingPathA[i];
            i++;
         }
      }else if(currentPlayer == UNI_B){
         i = 0;
         while (i < 92) {
            workingPath[i] = workingPathB[i];
            i++;
         }
      }else if(currentPlayer == UNI_C){
         i = 0;
         while (i < 92) {
            workingPath[i] = workingPathC[i];
            i++;
         } 
      } 


     i = 0;
      while (i < 90) {
         tempPath[i] = '\0';
         i++;
      }
   
      int counter = 0;
      int moveMade = FALSE;
      tempPath[0] = workingPath[0];
      newAction.destination[0] = tempPath[0];
      while (moveMade == FALSE && counter < MAX_ARCS) {
         tempPath[counter] = workingPath[counter];
         newAction.destination[counter] = tempPath[counter];
         if (isLegalAction(g, newAction) == TRUE){
            moveMade = TRUE;
         }
         counter++;
      }

      if (moveMade == FALSE) {
         nextAction = spinoff(nextAction);
	     if ((isLegalAction(g,nextAction) == FALSE)){
            newAction.actionCode = PASS;
         }
      }
   }
   return newAction;
}

static action buildCampus(Game g,action nextAction,int currentPlayer,int campusCounter){
   
   action newAction = nextAction;
   newAction.actionCode = BUILD_CAMPUS;
   
   if(campusCounter >= 10){
      newAction.actionCode = PASS;
   } else {
      int i = 0;
      while (i < PATH_LIMIT) {
         newAction.destination[i] = '\0';
         i++;
      }
   
   char workingPath[92] = {'\0'};
   char workingPathA[92] = PLAYER_A_PATH;
   char workingPathB[92] = PLAYER_B_PATH;
   char workingPathC[92] = PLAYER_C_PATH;
   char tempPath[92] = {'\0'};
   
   if(currentPlayer ==  UNI_A){
      i = 0;
      while (i < 92) {
         workingPath[i] = workingPathA[i];
         i++;
      }
   }else if(currentPlayer == UNI_B){
      i = 0;
      while (i < 92) {
         workingPath[i] = workingPathB[i];
         i++;
      }
   }else if(currentPlayer == UNI_C){
      i = 0;
      while (i < 92) {
         workingPath[i] = workingPathC[i];
         i++;
      } 
   } 

      i = 0;
      while (i < 90) {
         tempPath[i] = '\0';
         i++;
      }

      int counter = 0;
      int moveMade = 0;
      tempPath[0] = workingPath[0];
      newAction.destination[0] = tempPath[0];
      while (moveMade == 0 && counter < MAX_ARCS) {
         tempPath[counter] = workingPath[counter];
         newAction.destination[counter] = tempPath[counter];
         if (isLegalAction(g, newAction) == TRUE){
            moveMade = 1;
         }
         counter++;
      }

      if (moveMade == 0) {
        nextAction = spinoff(nextAction);
	if ((isLegalAction(g,nextAction) == FALSE)){
	newAction.actionCode = PASS;
	}
      }
   }
   return newAction;
}

static action exchangeBPS(Game g,action nextAction,int mjCounter, int mtvCounter){
   
   action newAction = nextAction;
   newAction.actionCode = RETRAIN_STUDENTS;
   if (mjCounter <= 1){
      newAction.disciplineFrom = STUDENT_BPS;
      newAction.disciplineTo = STUDENT_MJ;
   } else if (mtvCounter <= 1){
      newAction.disciplineFrom = STUDENT_BPS;
      newAction.disciplineTo = STUDENT_MTV;
   } else {
      newAction.disciplineFrom = STUDENT_BPS;
      newAction.disciplineTo = STUDENT_BQN;
   }
   return newAction;
}

static action exchangeBQN(Game g,action nextAction,int mtvCounter, int mmoneyCounter){
   
   action newAction = nextAction;
   newAction.actionCode = RETRAIN_STUDENTS;
   if (mtvCounter <= 1){
      newAction.disciplineFrom = STUDENT_BQN;
      newAction.disciplineTo = STUDENT_MTV;
   } else if (mmoneyCounter <= 1){
      newAction.disciplineFrom = STUDENT_BQN;
      newAction.disciplineTo = STUDENT_MMONEY;
   } else {
      newAction.disciplineFrom = STUDENT_BQN;
      newAction.disciplineTo = STUDENT_BPS;
   }
   return newAction;
}

static action exchangeMJ(Game g,action nextAction,int mmoneyCounter, int bpsCounter){
   
   action newAction = nextAction;
   newAction.actionCode = RETRAIN_STUDENTS;
   if (mmoneyCounter <= 1){
      newAction.disciplineFrom = STUDENT_MJ;
      newAction.disciplineTo = STUDENT_MMONEY;
   } else if (bpsCounter <= 1){
      newAction.disciplineFrom = STUDENT_MJ;
      newAction.disciplineTo = STUDENT_BPS;
   } else {
      newAction.disciplineFrom = STUDENT_MJ;
      newAction.disciplineTo = STUDENT_BQN;
   }
   return newAction;
}

static action exchangeMTV(Game g,action nextAction,int bpsCounter, int bqnCounter){
   
   action newAction = nextAction;
   newAction.actionCode = RETRAIN_STUDENTS;
   if (bpsCounter <= 1){
      newAction.disciplineFrom = STUDENT_MTV;
      newAction.disciplineTo = STUDENT_BPS;
   } else if (bqnCounter <= 1){
      newAction.disciplineFrom = STUDENT_MTV;
      newAction.disciplineTo = STUDENT_BQN;
   } else {
      newAction.disciplineFrom = STUDENT_MTV;
      newAction.disciplineTo = STUDENT_MJ;
   }
   return newAction;
}

static action exchangeMMONEY(Game g,action nextAction,int bqnCounter, int mjCounter){
   
   action newAction = nextAction;
   newAction.actionCode = RETRAIN_STUDENTS;
   if (bqnCounter <= 1){
      newAction.disciplineFrom = STUDENT_MMONEY;
      newAction.disciplineTo = STUDENT_BQN;
   } else if (mjCounter <= 1){
      newAction.disciplineFrom = STUDENT_MMONEY;
      newAction.disciplineTo = STUDENT_MJ;
   } else {
      newAction.disciplineFrom = STUDENT_MMONEY;
      newAction.disciplineTo = STUDENT_BPS;
   }
   return newAction;
}
