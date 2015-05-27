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

typedef struct _node *link;    
typedef struct _node {
	int value;
	link next;
} node;

typedef struct _list {
    link head;
} *list;     

/*static list newList(void);
static void showList (list listToPrint);
static void frontInsert (list l, int item);
static int numItems (list l);
static void append (list l, int value);
static int lookup (list l, int position);

static int vertexDecider(Game g, list regionsAtVertex[]);
//returns the length of the working path corresponding to the given vertex
static int pathToVertex(Game g, int vertex);
*/
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
                   + getCampuses(g,UNI_B) + getCampuses(g,UNI_C);*/
   //int GO8Counter = getGO8s(g,currentPlayer);

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
      
      if (campusCounter <=4 && mmoneyCounter >=3){
         
         nextAction = exchangeMMONEY(g,nextAction,bqnCounter,mjCounter);
      
      }else if(mmoneyCounter >= 6){
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
      if (campusCounter >=6 && nextAction.actionCode == PASS && (mjCounter >= 1)&&(mtvCounter >= 1)&&(mmoneyCounter >= 1)){
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

/*

static void showList (list listToPrint) {
   link current = listToPrint->head;
   while (current != NULL) {
      printf("[%d] ->", current->value);
      current = current->next;
   }
   printf("X\n");
}
static list newList(void) {
   list newList = malloc(sizeof(list));
   newList->head = NULL;
}

static void frontInsert (list l, int item) {
   link ptrToNewNode = malloc(sizeof(node));
   assert(ptrToNewNode != NULL);
   ptrToNewNode->value = item;
link oldHead = l->head;
   ptrToNewNode->next = oldHead;
   l->head = ptrToNewNode;
}
static int numItems (list l) {
   int counter = 0;
   link current = l->head;
   while(current != NULL){
      counter++;
      current = current->next;
   }
   return counter;
}
static void append (list l, int value) {
   link ptrToNewNode = malloc(sizeof(node));
   assert(ptrToNewNode != NULL);
   ptrToNewNode->value = value;
   ptrToNewNode->next = NULL;
if (l->head == NULL) {
      l->head = ptrToNewNode;
   } else {
      link current = l->head;
      while (current->next != NULL) {
         current = current->next;
      }
      current->next = ptrToNewNode;
   }
}
static int lookup (list l, int position) {
   assert (l != NULL);
   assert (position >= 0);
int counter = 0;
   int value = 0;
   link current = l->head;
   while (counter < position) {
      assert (current->next != NULL);
      current = current->next;
      counter++;
   }
value = current->value;
   return value;
}

//choosing a vertex/road to that vertex
static int vertexDecider(Game g, list regionsAtVertex[]) {
   list regionsAtVertex[NUM_VERTEXES] = {NULL};
   
   //im manually typing up these now

   int vertexCounter = 0;
   int vertexChosen = FALSE;
   while ((vertexCounter < 54)||(vertexChosen == FALSE) {
      int listLength = numItems(regionsAtVertex[vertexCounter]);
      if (listLength == 3){
         int region[3] = {0};
         int pos = 0;
         while (pos < 3) {
            region[pos] = lookup(regionsAtVertex[vertexCounter],pos);
            pos++;
         }
         pos = 0;
         int region[0] = lookup(regionsAtVertex[vertexCounter],0);
         int region[1] = lookup(regionsAtVertex[vertexCounter],1);
         int region[2] = lookup(regionsAtVertex[vertexCounter],2);
         int student[3] = {0};
         while (pos < 3) {
            int student[pos] = getDiscipline(g,region[pos]);
            pos++;
         }
         int student[0] = getDiscipline(g,region1);
         int student[1] = getDiscipline(g,region2);
         int student[2] = getDiscipline(g,region3);
      }
      pos = 0
      int matchCount = 0;
      while (pos < 3) {
         if (student[pos] == student that we want) {
            matchCount++;
         }else if (student[pos] == another student we want){
            matchCount++;
         }else if (student[pos] == third student we want){
            matchCount++;
         }
         pos++;
      }
      if (matchCount == 3) {
         vertexChosen = TRUE;
      }
      vertexCounter++;
   }
   return vertexCounter--;
}

static int pathToVertex(Game g, int vertex){
   //corresponding lengths for player A
   //I will do the same once u finish paths for B and C
   int pathLengthA[NUM_VERTEXES] = {0};
   pathLengthA[0] = 68;
   pathLegthA[1] = 69;
   pathLengthA[2] = 70;
   pathlengthA[3] = 73;
   pathlengthA[4] = 74;
   pathlengthA[5] = 77;
   pathlengthA[6] = 78;
   pathLengthA[7] = 64;
   pathlengthA[8] = 67;
   pathlengthA[9] = 22;
   pathlengthA[10] = 21;
   pathlengthA[11] = 20;
   pathlengthA[12] = 17;
   pathLengthA[13] = 16;
   pathlengthA[14] = 81;
   pathlengthA[15] = 82;
   pathlengthA[16] = 62;
   pathlengthA[17] = 1;
   pathlengthA[18] = 2;
   pathlengthA[19] = 25;
   pathlengthA[20] = 6;
   pathlengthA[21] = 7;
   pathlengthA[22] = 8;
   pathlengthA[23] = 13;
   pathlengthA[24] = 12;
   pathlengthA[25] = 85;
   pathlengthA[26] = 86;
   pathlengthA[27] = 61;
   pathlengthA[28] = 60;
   pathlengthA[29] = 3;
   pathlengthA[30] = 4;
   pathlengthA[31] = 5;
   pathlengthA[32] = 36;
   pathlengthA[33] = 9;
   pathlengthA[34] = 10;
   pathlengthA[35] = 11;
   pathlengthA[36] = 88;
   pathlengthA[37] = 87;
   pathlengthA[38] = 57;
   pathlengthA[39] = 56;
   pathlengthA[40] = 29;
   pathlengthA[41] = 30;
   pathlengthA[42] = 31;
   pathlengthA[43] = 38;
   pathlengthA[44] = 41;
   pathlengthA[45] = 42;
   pathlengthA[46] = 91;
   pathlengthA[47] = 53;
   pathlengthA[48] = 52;
   pathlengthA[49] = 51;
   pathlengthA[50] = 48;
   pathlengthA[51] = 47;
   pathlengthA[52] = 44;
   pathlengthA[53] = 43;
   return pathLength;
}
*/
