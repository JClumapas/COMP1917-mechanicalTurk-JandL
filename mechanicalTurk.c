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

typedef struct _node *link;    
typedef struct _node {
	int value;
	link next;
} node;

typedef struct _list {
    link head;
} *list;     

static list newList(void);
static void showList (list listToPrint);
static void frontInsert (list l, int item);
static int numItems (list l);
static void append (list l, int value);
static int lookup (list l, int position);

static int vertexComparison (Game g, int vertex, int studentWanted1, int studentWanted2, int studentWanted3);
static int chooseVertex(int currentPlayer);
static int vertexAToDefault(int vertex);
static int vertexBToDefault(int vertex);
static int vertexCToDefault(int vertex);
static int vertexToPathA(int vertexA);
static int vertexToPathB(int vertexB);
static int vertexToPathC(int vertexC);

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
   int moveMade = FALSE;
   //printf("max arcs %d\n", MAX_ARCS);
   tempPath[0] = workingPath[0];
   newAction.destination[0] = tempPath[0];
   while (moveMade == FALSE && counter < MAX_ARCS) {
      tempPath[counter] = workingPath[counter];
      //printf("%s\n", tempPath);
      //printf("Here and counter is %d\n", counter);
      newAction.destination[counter] = tempPath[counter];
      if (isLegalAction(g, newAction) == TRUE){
         moveMade = TRUE;
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
      int moveMade = FALSE;
      //printf("max arcs %d\n", MAX_ARCS);
      tempPath[0] = workingPath[0];
      newAction.destination[0] = tempPath[0];
      while (moveMade == FALSE && counter < MAX_ARCS) {
         tempPath[counter] = workingPath[counter];
         //printf("%s\n", tempPath);
         //printf("Here and counter is %d\n", counter);
         newAction.destination[counter] = tempPath[counter];
         if (isLegalAction(g, newAction) == TRUE){
            moveMade = TRUE;
            //printf("changing movemade %d\n", moveMade);
         }
         counter++;
      }

      if (moveMade == FALSE) {
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
      int pathCount;
      int vertex = chooseVertex(currentPlayer);
      if (vertex != NOT_CHOSEN){
         if (currentPlayer == UNI_A){
            pathCount = vertexToPathA(vertex);
         } else if(currentPlayer == UNI_B){
            pathCount = vertexToPathB(vertex);
         } else if(currentPlayer == UNI_C){
            pathCount = vertexToPathC(vertex);
         }
         int counter = 0;
         int moveMade = FALSE;
         while (counter < pathCount) {
            tempPath[counter] = workingPath[counter];
            counter++;
         }
         newAction.destination = counter;
         if (isLegalAction(g, newAction) == TRUE){
            moveMade = TRUE;
            //printf("move Made %s\n", tempPath);
            //printf("changing movemade %d\n", moveMade);
         }else{
           counter = 0;
           newAction.actionCode = OBTAIN_ARC;
           char aimPath[pathCount+1] = {'\0'};
           while (moveMade == FALSE && counter < MAX_ARCS) {
              aimPath[counter] = workingPath[counter];
              //printf("%s\n", tempPath);
              //printf("Here and counter is %d\n", counter);
              newAction.destination[counter] = aimPath[counter];
              if (isLegalAction(g, newAction) == TRUE){
                 moveMade = TRUE;
                 //printf("changing movemade %d\n", moveMade);
               }
            counter++;
            }
         }
         if (moveMade == FALSE) {
            newAction.actionCode = PASS;
            //printf("unchanging movemade %d\n", moveMade);
         }
         
      } else {
         int counter = 0;
         int moveMade = FALSE;
         //printf("max arcs %d\n", MAX_ARCS);
         tempPath[0] = workingPath[0];
         newAction.destination[0] = tempPath[0];
         while (moveMade == FALSE && counter < MAX_ARCS) {
            tempPath[counter] = workingPath[counter];
            //printf("%s\n", tempPath);
            //printf("Here and counter is %d\n", counter);
            newAction.destination[counter] = tempPath[counter];
            if (isLegalAction(g, newAction) == TRUE){
               moveMade = TRUE;
               //printf("move Made %s\n", tempPath);
               //printf("changing movemade %d\n", moveMade);
            }
            counter++;
         }
         if (moveMade == FALSE) {
            newAction.actionCode = PASS;
            //printf("unchanging movemade %d\n", moveMade);
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

static void showList (list listToPrint) {
   link current = listToPrint->head;
   while (current != NULL) {
      printf("[%d] ->", current->value);
      current = current->next;
   }
   printf("X\n");
}
static void list newList(void) {
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

static int vertexComparison (Game g, int vertex, int studentWanted1, int studentWanted2, int studentWanted3){
   list regionsAtVertex[NUM_VERTEXES];

   int counter = 0;
   while ( counter < 54){
      regionsAtVertex[counter] = newList(void);
      counter++;
   }

   frontInsert(regionsAtVertex[0], 0);
   frontInsert(regionsAtVertex[1], 0);
   frontInsert(regionsAtVertex[2], 0);
   append(regionsAtVertex[2],1);
   frontInsert(regionsAtVertex[3], 1);
   frontInsert(regionsAtVertex[4], 1);
   append(regionsAtVertex[4],2);
   frontInsert(regionsAtVertex[5], 2);
   frontInsert(regionsAtVertex[6], 2);
   frontInsert(regionsAtVertex[7], 3);
   frontInsert(regionsAtVertex[8], 0);
   append(regionsAtVertex[8],3);
   frontInsert(regionsAtVertex[9], 0);
   append(regionsAtVertex[9],3);
   append(regionsAtVertex[9],4);
   frontInsert(regionsAtVertex[10], 0);
   append(regionsAtVertex[10],1);
   append(regionsAtVertex[10],4);
   frontInsert(regionsAtVertex[11], 1);
   append(regionsAtVertex[11],4);
   append(regionsAtVertex[11],5);
   frontInsert(regionsAtVertex[12], 1);
   append(regionsAtVertex[12],2);
   append(regionsAtVertex[11],5);
   frontInsert(regionsAtVertex[13], 2);
   append(regionsAtVertex[13],5);
   append(regionsAtVertex[13],6);
   frontInsert(regionsAtVertex[14], 2);
   append(regionsAtVertex[14],6);
   frontInsert(regionsAtVertex[15], 6);
   frontInsert(regionsAtVertex[16], 7);
   frontInsert(regionsAtVertex[17], 3);
   append(regionsAtVertex[17],7);
   frontInsert(regionsAtVertex[18], 3);
   append(regionsAtVertex[18],7);
   append(regionsAtVertex[18],8);
   frontInsert(regionsAtVertex[19], 3);
   append(regionsAtVertex[19],4);
   append(regionsAtVertex[19],8);
   frontInsert(regionsAtVertex[20], 4);
   append(regionsAtVertex[20],8);
   append(regionsAtVertex[20],9);
   frontInsert(regionsAtVertex[21], 4);
   append(regionsAtVertex[21],5);
   append(regionsAtVertex[21],9);
   frontInsert(regionsAtVertex[22], 5);
   append(regionsAtVertex[22],9);
   append(regionsAtVertex[22],10);
   frontInsert(regionsAtVertex[23], 5);
   append(regionsAtVertex[23],6);
   append(regionsAtVertex[23],10);
   frontInsert(regionsAtVertex[24], 6);
   append(regionsAtVertex[24],10);
   append(regionsAtVertex[24],11);
   frontInsert(regionsAtVertex[25], 6);
   append(regionsAtVertex[25],11);
   frontInsert(regionsAtVertex[26], 11);
   frontInsert(regionsAtVertex[27], 7);
   frontInsert(regionsAtVertex[28], 7);
   append(regionsAtVertex[28],12);
   frontInsert(regionsAtVertex[29], 7);
   append(regionsAtVertex[29],8);
   append(regionsAtVertex[29],12);
   frontInsert(regionsAtVertex[30], 8);
   append(regionsAtVertex[30],12);
   append(regionsAtVertex[30],13);
   frontInsert(regionsAtVertex[31], 8);
   append(regionsAtVertex[31],9);
   append(regionsAtVertex[31],13);
   frontInsert(regionsAtVertex[32], 9);
   append(regionsAtVertex[32],13);
   append(regionsAtVertex[32],14);
   frontInsert(regionsAtVertex[33], 9);
   append(regionsAtVertex[33],10);
   append(regionsAtVertex[33],14);
   frontInsert(regionsAtVertex[34], 10);
   append(regionsAtVertex[34],14);
   append(regionsAtVertex[34],15);
   frontInsert(regionsAtVertex[35], 10);
   append(regionsAtVertex[35],11);
   append(regionsAtVertex[35],15);
   frontInsert(regionsAtVertex[36], 11);
   append(regionsAtVertex[36],15);
   frontInsert(regionsAtVertex[37], 11);
   frontInsert(regionsAtVertex[38], 12);
   frontInsert(regionsAtVertex[39], 12);
   append(regionsAtVertex[39],16);
   frontInsert(regionsAtVertex[40], 12);
   append(regionsAtVertex[40],13);
   append(regionsAtVertex[40],16);
   frontInsert(regionsAtVertex[41], 13);
   append(regionsAtVertex[41],16);
   append(regionsAtVertex[41],17);
   frontInsert(regionsAtVertex[42], 13);
   append(regionsAtVertex[42],14);
   append(regionsAtVertex[42],17);
   frontInsert(regionsAtVertex[43], 14);
   append(regionsAtVertex[43],17);
   append(regionsAtVertex[43],18);
   frontInsert(regionsAtVertex[44], 14);
   append(regionsAtVertex[44],15);
   append(regionsAtVertex[44],18);
   frontInsert(regionsAtVertex[45], 15);
   append(regionsAtVertex[45],18);
   frontInsert(regionsAtVertex[46], 15);
   frontInsert(regionsAtVertex[47], 16);
   frontInsert(regionsAtVertex[48], 16);
   frontInsert(regionsAtVertex[49], 16);
   append(regionsAtVertex[49],17);
   frontInsert(regionsAtVertex[50], 17);
   frontInsert(regionsAtVertex[51], 17);
   append(regionsAtVertex[51],18);
   frontInsert(regionsAtVertex[52], 18);
   frontInsert(regionsAtVertex[53], 18);

   int listLength == numItems(regionsAtVertex[vertex];
   if (listLength == 3){
      int regionID[3] = {0};
      int student[3] = {0};
      counter = 0;
      int matchCounter = NO_MATCH;
      while (counter < 3){
         regionID[counter] = lookup(regionsAtVertex[vertex], counter);
         student[counter] = getDisciplines(g, regionID[counter]);
         if (student[counter] == studentWanted1){
            matchCounter++;
         } else if (student[counter] == studentWanted2){
            matchCounter++;
         } else if (student[counter] == studentWanted3){
            matchCounter++;
         }
         counter++;
      }
   }
   counter = 0;
   while ( counter < 54){
      free(regionsAtVertex[counter]);
      counter++;
   }
   return matchCounter;
}

static int chooseVertex(int currentPlayer){
   int vertex = 0;
   int defVertex = 0;
   int matchCount = 0;
   int vertexChosen = FALSE;

   if(currentPlayer == UNI_A){
      int vertexA = 0;
      while ((vertexA <= 14)||(vertexChosen == TRUE)) {
         defVertex = vertexAToDefault(vertexA);
         matchCount = vertexComparison(g, defVertex, STUDENT_BPS, STUDENT_BQN, STUDENT_MJ);
         if (matchCount == 3){
            vertexChosen = TRUE;
         }
         vertexA++;
      }
      if (vertexChosen = FALSE){
         vertexA = NOT_CHOSEN;
      }
      vertex = vertexA;
   } else if (currentPlayer == UNI_B){
      int vertexB = 10;
      while ((vertexB <= 24)||(vertexChosen == TRUE)) {
         defVertex = vertexBToDefault(vertexB);
         matchCount = vertexComparison(g, defVertex, STUDENT_BPS, STUDENT_BQN, STUDENT_MJ);
         if (matchCount == 3){
            vertexChosen = TRUE;
         }
         vertexB++;
      }
      if (vertexChosen == FALSE){
         vertexB = NOT_CHOSEN;
      }
      vertex = vertexB;
   } else if (currentPlayer == UNI_C){
      int vertexC = 10;
      while ((vertexC <= 24)||(vertexChosen == TRUE)) {
         defVertex = vertexCToDefault(vertexC);
         matchCount = vertexComparison(g, defVertex, STUDENT_BPS, STUDENT_BQN, STUDENT_MJ);
         if (matchCount == 3){
            vertexChosen = TRUE;
         }
         vertexC++;
      }
      if (vertexChosen == FALSE){
         vertexC = NOT_CHOSEN;
      }
      vertex = vertexC;
   }
   return vertex;
}

static int vertexAToDefault(int vertex){
   int vertexAToDef[54] = {0};

    vertexAToDef[0] = 16;
    vertexAToDef[1] = 17;
    vertexAToDef[2] = 18;
    vertexAToDef[3] = 29;
    vertexAToDef[4] = 30;
    vertexAToDef[5] = 31;
    vertexAToDef[6] = 20;
    vertexAToDef[7] = 21;
    vertexAToDef[8] = 22;
    vertexAToDef[9] = 33;
    vertexAToDef[10] = 34;
    vertexAToDef[11] = 35;
    vertexAToDef[12] = 24;
    vertexAToDef[13] = 23;
    vertexAToDef[14] = 13;

   return vertexAToDef[vertex];
}

static int vertexBToDefault(int vertex){
   int vertexBToDef[54] = {0};

    vertexBToDef[10] = 43;
    vertexBToDef[11] = 44;
    vertexBToDef[12] = 34;
    vertexBToDef[13] = 33;
    vertexBToDef[14] = 35;
    vertexBToDef[15] = 24;
    vertexBToDef[16] = 23;
    vertexBToDef[17] = 22;
    vertexBToDef[18] = 13;
    vertexBToDef[19] = 12;
    vertexBToDef[20] = 11;
    vertexBToDef[21] = 21;
    vertexBToDef[22] = 20;
    vertexBToDef[23] = 19;
    vertexBToDef[24] = 9;

   return vertexBToDef[vertex];
}

static int vertexCToDefault(int vertex){
   int vertexCToDef[54] = {0};

    vertexCToDef[10] = 15;
    vertexCToDef[11] = 16;
    vertexCToDef[12] = 13;
    vertexCToDef[13] = 12;
    vertexCToDef[14] = 14;
    vertexCToDef[15] = 35;
    vertexCToDef[16] = 34;
    vertexCToDef[17] = 33;
    vertexCToDef[18] = 44;
    vertexCToDef[19] = 43;
    vertexCToDef[20] = 42;
    vertexCToDef[21] = 32;
    vertexCToDef[22] = 31;
    vertexCToDef[23] = 30;
    vertexCToDef[24] = 40;

   return vertexCToDef[vertex];
}

static int vertexToPathA(int vertexA){
   int countToVertex[NUM_VERTEXES] = {0};

   countToVertex[1] = 1;
   countToVertex[2] = 2;
   countToVertex[3] = 3;
   countToVertex[4] = 4;
   countToVertex[5] = 5;
   countToVertex[6] = 6;
   countToVertex[7] = 7;
   countToVertex[8] = 8;
   countToVertex[9] = 9;
   countToVertex[10] = 10;
   countToVertex[11] = 11;
   countToVertex[12] = 12;
   countToVertex[13] = 15;
   countToVertex[14] = 16;   

   int pathLength = countToVertex[vertexA];
   return pathLength;
}
     
static int vertexToPathB(int vertexB){
   int countToVertex[NUM_VERTEXES] = {0};

   countToVertex[10] = 12;
   countToVertex[11] = 13;
   countToVertex[12] = 14;
   countToVertex[13] = 15;
   countToVertex[14] = 17;
   countToVertex[15] = 18;
   countToVertex[16] = 19;
   countToVertex[17] = 20;
   countToVertex[18] = 22;
   countToVertex[19] = 23;
   countToVertex[20] = 24;
   countToVertex[21] = 25;
   countToVertex[22] = 26;
   countToVertex[23] = 33;
   countToVertex[24] = 34;   

   int pathLength = countToVertex[vertexB];
   return pathLength;
}

static int vertexToPathC(int vertexC){
   int countToVertex[NUM_VERTEXES] = {0};

   countToVertex[10] = 12;
   countToVertex[11] = 13;
   countToVertex[12] = 14;
   countToVertex[13] = 15;
   countToVertex[14] = 17;
   countToVertex[15] = 18;
   countToVertex[16] = 19;
   countToVertex[17] = 20;
   countToVertex[18] = 22;
   countToVertex[19] = 23;
   countToVertex[20] = 24;
   countToVertex[21] = 25;
   countToVertex[22] = 26;
   countToVertex[23] = 33;
   countToVertex[24] = 34;   

   int pathLength = countToVertex[vertexC];
   return pathLength;
}




