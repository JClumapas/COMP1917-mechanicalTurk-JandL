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
#define WORKING_PATH {'R','L','L','R','R','R','L','L','L','R','R','R','R','R','B','R','R','R','R','B','R','R','R','R','B','R','R','R','L','R','R','R','R','B','R','B','R','R','R','R','B','R','L','L','L','L','B','L','L','L','B','L','L','L','L','B','L','L','L','B','L','L','L','R','L','L','B','L','L','L','L','B','L','L','L','B','L','L','L','L','B','L','L','L','B','L','L','L','L','B','L','L'}

/*enter values according to working path
/#define UNI_A_CAMPUS_A 53
#define UNI_A_CAMPUS_B 26
#define UNI_B_CAMPUS_A 17
#define UNI_B_CAMPUS_B 44
#define UNI_C_CAMPUS_A 8
#define UNI_C_CAMPUS_B 35
*/

static action spinoff(action nextAction);
static action buildARC(Game g, action nextAction,int currentPlayer,int arcCounter);
static action buildCampus(Game g, action nextAction,int currentPlayer,int campusCounter);
static action buildGO8(Game g,action nextAction,int currentPlayer);
static action exchangeBPS(Game g,action nextAction,int mjCounter, int mtvCounter);

static action exchangeBQN(Game g,action nextAction,int mtvCounter,int mmoneyCounter);

static action exchangeMJ(Game g,action nextAction,int mmoneyCounter, int bpsCounter);

static action exchangeMTV(Game g,action nextAction,int bpsCounter,int bqnCounter);

static action exchangeMMONEY(Game g,action nextAction,int bqnCounter,int mjCounter);
//static action buildGO8(action nextAction, int GO8Counter, int currentPlayer);
//static path arcPathGenerator(Game g, action nextAction, int arcCounter, int currentPlayer);
//static path campusPathGenerator(action nextAction, int currentPlayer);
//static path go8PathGenerator(action nextAction, int currentPlayer);

action decideAction (Game g) {

   action nextAction;
   nextAction.actionCode = PASS;
   int i = 0;
   while (i < PATH_LIMIT) {
      nextAction.destination[i] = '\0';
      i++;
   }
//   int currentTurn = getTurnNumber(g);
   int currentPlayer = getWhoseTurn(g);
   int bpsCounter = getStudents(g,currentPlayer,STUDENT_BPS);
   int bqnCounter = getStudents(g,currentPlayer,STUDENT_BQN);
   int mjCounter = getStudents(g,currentPlayer,STUDENT_MJ);
   int mtvCounter = getStudents(g,currentPlayer,STUDENT_MTV);
   int mmoneyCounter = getStudents(g,currentPlayer,STUDENT_MMONEY);

   int arcCounter = getARCs(g,currentPlayer);
   int campusCounter = getCampuses(g,currentPlayer);
   /*int totalCampuses = 0;
   totalCampuses = totalCampuses + getCampuses(g,UNI_A)
                   + getCampuses(g,UNI_B) + getCampuses(g,UNI_C);
   int GO8Counter = getGO8s(g,currentPlayer);*/

   //Actions for our first turn
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
      
      if (mmoneyCounter >=5){
         
         nextAction = exchangeMMONEY(g,nextAction,bqnCounter,mjCounter);
      
      }
      if((nextAction.actionCode == PASS || nextAction.actionCode == RETRAIN_STUDENTS) && (mjCounter >= 2)&&(mmoneyCounter >= 3)){
         nextAction = buildGO8(g,nextAction,currentPlayer);
      }
      if ((nextAction.actionCode == PASS) && (bpsCounter >= 1)&&(bqnCounter >= 1)&&(mjCounter >= 1)&&(mtvCounter >= 1)){
         nextAction = buildCampus(g,nextAction,currentPlayer,campusCounter);
         //printf("%s\n", nextAction.destination);
      }
      if (nextAction.actionCode == PASS && (bpsCounter >= 1)&&(bqnCounter >= 1)){
         nextAction = buildARC(g,nextAction,currentPlayer,arcCounter);
         //printf("%s\n", nextAction.destination);
      }
      if (nextAction.actionCode == PASS && (mjCounter >= 1)&&(mtvCounter >= 1)&&(mmoneyCounter >= 1)){
         nextAction = spinoff(nextAction);
         //printf("%s\n", nextAction.destination);
      } 
      
   
      /*
      if ((totalCampuses >= (0.7*MAX_CAMPUSES)&&(mjCounter >= 2)
         &&(mmoneyCounter >= 3)&&(GO8Counter <= 8){
         nextAction = buildGO8(nextAction,GO8Counter,currentPlayer);
      } else if ((bpsCounter >= 1)&&(bqnCounter >= 1)
         &&(mjCounter >= 1)&&(mtvCounter >= 1){
         nextAction = buildCampus(nextAction, currentPlayer);
      } else {
      if ((bpsCounter >= 1)&&(bqnCounter >= 1)){
         nextAction = buildARC(g,nextAction,arcCounter,currentPlayer);
      } else if ((mjCounter >= 1)&&(mtvCounter >= 1)&&(mmoneyCounter >= 1)){
         nextAction = spinoff(nextAction);
      } else {
         nextAction.actionCode = PASS;
      }
   */

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

   char tempPath[92] = {'\0'};
   char workingPath[92] = WORKING_PATH;

   i = 0;
   while (i < 90) {
      tempPath[i] = '\0';
      i++;
   }

   
   int counter = 0;
   int moveMade = 0;
   //printf("max arcs %d\n", MAX_ARCS);
   tempPath[0] = workingPath[0];
   newAction.destination[0] = tempPath[0];
   while (moveMade == 0 && counter < MAX_ARCS) {
      tempPath[counter] = workingPath[counter];
      //printf("%s\n", tempPath);
      //printf("Here and counter is %d\n", counter);
      newAction.destination[counter] = tempPath[counter];
      if (isLegalAction(g, newAction) == TRUE){
         moveMade = 1;
         //printf("changing movemade %d\n", moveMade);
      }
      counter++;
   }

   if (moveMade == 0) {
      newAction.actionCode = PASS;
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

   if(arcCounter >= 16){
      newAction.actionCode = PASS;
   } else {
      char tempPath[92] = {'\0'};
      char workingPath[92] = WORKING_PATH;

     i = 0;
      while (i < 90) {
         tempPath[i] = '\0';
         i++;
      }

   
      int counter = 0;
      int moveMade = 0;
      //printf("max arcs %d\n", MAX_ARCS);
      tempPath[0] = workingPath[0];
      newAction.destination[0] = tempPath[0];
      while (moveMade == 0 && counter < MAX_ARCS) {
         tempPath[counter] = workingPath[counter];
         //printf("%s\n", tempPath);
         //printf("Here and counter is %d\n", counter);
         newAction.destination[counter] = tempPath[counter];
         if (isLegalAction(g, newAction) == TRUE){
            moveMade = 1;
            //printf("changing movemade %d\n", moveMade);
         }
         counter++;
      }

      if (moveMade == 0) {
         newAction.actionCode = PASS;
      }
   }
   return newAction;
}

static action buildCampus(Game g,action nextAction,int currentPlayer,int campusCounter){
   
   action newAction = nextAction;
   newAction.actionCode = BUILD_CAMPUS;
   
   if(campusCounter >= 8){
      newAction.actionCode = PASS;
   } else {
      int i = 0;
      while (i < PATH_LIMIT) {
         newAction.destination[i] = '\0';
         i++;
      }

      char tempPath[92] = {'\0'};
      char workingPath[92] = WORKING_PATH;

      i = 0;
      while (i < 90) {
         tempPath[i] = '\0';
         i++;
      }

   
      int counter = 0;
      int moveMade = 0;
      //printf("max arcs %d\n", MAX_ARCS);
      tempPath[0] = workingPath[0];
      newAction.destination[0] = tempPath[0];
      while (moveMade == 0 && counter < MAX_ARCS) {
         tempPath[counter] = workingPath[counter];
         //printf("%s\n", tempPath);
         //printf("Here and counter is %d\n", counter);
         newAction.destination[counter] = tempPath[counter];
         if (isLegalAction(g, newAction) == TRUE){
            moveMade = 1;
            //printf("move Made %s\n", tempPath);
            //printf("changing movemade %d\n", moveMade);
         }
         counter++;
      }

      if (moveMade == 0) {
         newAction.actionCode = PASS;
         //printf("unchanging movemade %d\n", moveMade);
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

/*static action buildGO8(action nextAction, int GO8Counter, int currentPlayer){
   action newAction = nextAction;
   nextAction = go8PathGenerator(nextAction, int currentPlayer);
   return newAction;
   //similar to buildARC and buildCampus
}*/

/*static action arcPathGenerator(Game g, action nextAction, int arcCounter, int currentPlayer){
 
   action newAction = nextAction;
   char tempPath[90] = {'\0'};
   char workingPath[90] = WORKING_PATH;
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
   return newAction;
   // doesnt matter where it starts this just looks where we can make an arc on a legal action and makes it straight away
   
   action newAction = nextAction;
   //path destination[PATH_LIMIT]={'\0'};
   int initialPos = 0;
   if (currentPlayer == UNI_A){
      initialPos = UNI_A_CAMPUS_A; //or CAMPUS_B depend on strategy
   } else if (currentPlayer == UNI_B){
      initialPos = UNI_B_CAMPUS_A;
   } else if (currentPlayer == UNI_C){
      initialPos = UNI_C_CAMPUS_A;
   }


   int counter = 0;
   while (counter <= (initialPos+1)){
      tempPath[counter] = workingPath[counter];
      counter++;
   }
   while (counter <= (initialPos+arcCounter+1)){
      while (getARC(g,tempPath) != VACANT_ARC){
         if( tempPath[counter]== 'R'){
            tempPath[counter] = 'L';
         }else if (tempPath[counter] == 'L'){
            tempPath[counter]= 'R';
         }else if (tempPath[counter] == 'B'){
            tempPath[counter] = 'R';
         }
      }
      counter++;
   }



   strncpy(newAction.destination, tempPath, sizeof (tempPath));
   return newAction;
}*/

///////////////////////////////

   count = 0;

   char tempPath2[91] = {'\0'};

   char workingPath2[90] = WORKING_PATH;

   while(count<90){

      tempPath2[count] = workingPath2[count];

      printf("[%d]:", count);

      if(count == 51 || count == 52 || count == 53){

         assert(getARC(g, tempPath2) == ARC_A);

         printf("testing arc A\n");

      } else if(count == 44 || count == 43 || count == 42){

         assert(getARC(g, tempPath2) == ARC_B);

         printf("testing arc B\n");

      } else if(count == 8 || count == 6 || count == 7 || count == 58){

         assert(getARC(g, tempPath2) == ARC_C);

         printf("testing arc C\n");

      } else {

         assert(getARC(g, tempPath2) == VACANT_ARC);

         printf("testing arc %d\n", count);

      }
 
      count++;

   }