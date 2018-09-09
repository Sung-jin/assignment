#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){

  FILE *f;
  int j, i, check, arr[5000];

  f = fopen("controlData.txt", "w");

  if(f == NULL){
    perror("File open");
  }

  for(i = 0; i < 5000; i++){
    while(1){
      check = 0;

      arr[i] = rand() % 5000 + 1;
        for(j = 0; j < i; j++){
          if(arr[j] == arr[i]){
            check = 1;
            break;
          }
        }
        if(check == 0){
          break;
        }

    }
  }

  for(i = 0; i < 5000; i ++){
    fprintf(f, "%d ", arr[i]);
  }

  fclose(f);

  return 0;
}
