#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#define size 1000

int generation = 0;
int numofgeneration = 1000;
int initualNumOfInfected = 200;
int currentGen [size][size][2];
int nextGen [size][size][2];
float rangenerate(float minn,float maxx);
int checkN(int i, int j);
int nextgen(int i,int j,float chance);
void savetofile(int num);
void populate();
void* childfunc (void* rank);
void* childfun (void* rank);
long thread_count =4;
int flag = 0;
int flag2 =0;
int flag3 =0;
pthread_t* thread_handles;

int main(int argc, char *argv[]) {
  time_t startTime = time(NULL);
  long thread;
  long my_rank =4;
  int u;
  srand((unsigned int)time(NULL));
  populate();
  savetofile(0);
  thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 
  for (thread = 0; thread < thread_count; thread++)  
        pthread_create(&thread_handles[thread], NULL, childfunc, (void*)thread);
  while(generation < numofgeneration){
    while(flag2 !=my_rank);  
    generation++;
    flag2 =0;
    memcpy(currentGen, nextGen, sizeof(currentGen));
    savetofile(generation);
    flag =0;
  }
  free(thread_handles);
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

int nextgen(int i,int j,float chance){
  if(currentGen[i][j][0] !=4){
    float ranNum;
    ranNum = rangenerate(0.0,1.0);
    if(currentGen[i][j][0] == 1){
      if(ranNum < chance){
	return 2;
      }
      else{
	return 1;
      }
    }
    else if (currentGen[i][j][0] == 2){
      if(ranNum < 0.50){
	return 3;
      }
      else{
	return 1;
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
void* childfunc (void* rank){
  long my_rank = (long) rank;
  int i;
  int j;
  int numofinfected;
  int my_n = size/thread_count;
  int my_first_i = my_n*my_rank;
  int my_last_i = my_first_i + my_n;
  float chance;
  while(generation < numofgeneration-1){
      while(flag !=my_rank);  
      flag = flag+1;	
      
      for(i = my_first_i;i<my_last_i;i++){
        for(j =0;j<size;j++){
	  if(currentGen[j][i][0] == 2){
	    while(flag3 !=my_rank);
	    nextGen[j][i][0] = nextgen(j,i,0);
	    flag3 = (flag3+1)%thread_count;
	  }
	  else if(currentGen[j][i][0] == 3){
              if(currentGen[j][i][1] == 3){
                  while(flag3 !=my_rank);
	          nextGen[j][i][0] = 4;
	          flag3 = (flag3+1)%thread_count;
              }
              else{
                while(flag3 !=my_rank);
	        nextGen[j][i][1] = currentGen[j][i][1] += 1;
	        flag3 = (flag3+1)%thread_count;
              }
          }
          else{
	    numofinfected = checkN(j,i);
            if(numofinfected == 0){
              chance = 0.0;
            }
            else{
              chance = 0.10 * numofinfected;
	    }
	    while(flag3 !=my_rank);
            nextGen[j][i][0] = nextgen(j,i,chance);
            flag3 = (flag3+1)%thread_count;
	  }
        }
      }
      
      while(flag2 !=my_rank);  
      flag2 = flag2+1;
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
