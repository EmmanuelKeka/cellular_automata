#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include <unistd.h>
#include <time.h>
#define size 1000

int numofgeneration = 1000;
int initualNumOfInfected = 200;
int currentGen [size][size][2];
int nextGen [size][size][2];
float rangenerate(float minn,float maxx);
int checkN(int i, int j);
void nextgen(int i,int j,float chance);
void savetofile(int num);
void populate();
void run();

int main(int argc, char *argv[]) {
  clock_t startTime = clock();
  srand((unsigned int)time(NULL));
  populate();
  run();
  time_t endTime   = time(NULL);
  float  totalTime = (float)endTime - (float)startTime;
  printf("program end %f\n",totalTime);
}

float rangenerate(float minn,float maxx){
  float min = minn;
  float max = maxx;
  float myrandomfloat = min + (rand() / (float) RAND_MAX) * ( max - min );
  return myrandomfloat;
}

int checkN(int i, int j){ 
  int numofIn = 0;
  int jNom;
  int iNom;
  int adder = 2;
  int sus = 1;
  for(iNom=i-sus;iNom<(i+adder);iNom++){
    for(jNom=j-sus;jNom<(j+adder);jNom++){
      if(iNom<0 || jNom<0 || iNom ==size || jNom == size || (iNom == i && jNom == j)){
        ;
      }
      else{
	if(currentGen[iNom][jNom][0] == 2 || currentGen[iNom][jNom][0] == 3){
	  numofIn++;
	}
      }
    }
  }
  return numofIn;
}

void nextgen(int i,int j,float chance){
  if(currentGen[i][j][0] !=4){
    float ranNum;
    ranNum = rangenerate(0.0,1.0);
    if(currentGen[i][j][0] == 1){
      if(ranNum < chance){
	nextGen[i][j][0] = 2;
      }
      else{
	nextGen[i][j][0] = 1;
      }
    }
    else if (currentGen[i][j][0] == 2){
      if(ranNum < 0.50){
	nextGen[i][j][0] = 3;
      }
      else{
	nextGen[i][j][0] = 1;
      }
    }
    else if(currentGen[i][j][0] == 3){
      if(currentGen[i][j][1] == 3){
	nextGen[i][j][0] = 4;
      }
      else{
	nextGen[i][j][1] = currentGen[i][j][1] += 1;
      }
    }
  }
}

void populate(){
  int i;
  int j;
  int u;
  for (i = 0; i < size; i++){
    for(j = 0; j < size; j++){
      currentGen[j][i][0] = 1;
      currentGen[j][i][1] = 0;
    }  
  }
  for(u=0;u<initualNumOfInfected;u++){
    int i = rangenerate(1.0,1000.0);
    int j = rangenerate(1.0,1000.0);
    if(currentGen[j][i][0] == 2){
      i--;
    }
    else{
      currentGen[j][i][0] = 2;
    }
  }
}

void run(){
  int i;
  int j;
  int u;
  int numofinfected;
  float chance;
  for(u =0;u<numofgeneration;u++){
    savetofile(u);
    for(i = 0;i<size;i++){
      for(j =0;j<size;j++){
	if(currentGen[j][i][0] == 2 || currentGen[j][i][0] == 3){
	  nextgen(j,i,0);
	}
	else{
	  numofinfected = checkN(j,i);
          if(numofinfected == 0){
            chance = 0.0;
          }
          else{
            chance = 0.125 * numofinfected;
	  }
          nextgen(j,i,chance);
	}
      }
    }
    memcpy(currentGen, nextGen, sizeof(currentGen));
  }
}

void savetofile(int num){
  char *filename, text[50];
  sprintf(text,"visual/data/data%d",num);
  filename = text;
  FILE * fpointer = fopen(filename,"w");
  int i;
  int j;
  for(i = 0;i<size;i++){
    for(j =0;j<size;j++){
      fprintf(fpointer,"%d",currentGen[j][i][0]);
    }
    fprintf(fpointer,"\n");
  }
  fclose(fpointer);
}
