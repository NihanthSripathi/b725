#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define ROW 5
#define COL 5

void disp(void);
int  calling(int);
int search(void);
int is_dup(int);
int create_rand_num(void);
void clear_lines(int);
void loading_quote(void);

int arr[ROW][COL];

int count = 0;
int arr_size = 25;
int main(){
    loading_quote();
    srand(time(NULL));
    for(int i = 0 ; i< ROW ; i++){
        for(int j = 0 ; j< COL ; j++){
            arr[i][j] = create_rand_num();
        }
    }
    int val;
    clear_lines(100);
    disp();
    while(1){
        printf("\nEnter the input : ");
        scanf("%d",&val);
        if(val<0 || val >ROW*COL){
            printf("Inavlid Input");
            continue;
        }
        int WIN = calling(val);
        if(WIN) break;        
    }
}

int create_rand_num(){
    int min = 1, max =25;
    int rand_num = 0;
    while(1){
        rand_num = (rand()%(max-min+1))+min;
        if(is_dup(rand_num))
            continue;
        else{
            return rand_num;
        }
    }
    
}
int is_dup(int val){
    for(int i =0 ;i<ROW; i++){
        for(int j = 0; j<COL; j++)
            if(arr[i][j]==val)return 1;
    }
    return 0;
}
void disp(){
    printf("\n");
    for(int i = 0 ; i< ROW ; i++){
        for(int j = 0 ; j< COL ; j++){
            printf(" %2d ",arr[i][j]); 
        }printf("\n\n");
    }
    printf("\n");
    
}
int calling(int val){
    for(int i = 0 ; i< ROW ; i++){
        for(int j = 0 ; j< COL ; j++){
            if(arr[i][j] == val){
                arr[i][j] = 0;
            }
        }
    }
   int present_count = search();
   if(count < present_count){
       count = present_count;
       clear_lines(100);
       printf("\n Congratulations Cleared : %d \n",count);
       disp();
   }
   if(count >= ROW){
       clear_lines(100);
       printf("\n\n//***** BINGO *****\\\\\n\n"); 
       disp();
       printf("\\\\***** BINGO ****//\n\n\n"); 
        return 1;
    }else{
        return 0;
   }

}
int search(){
    //ROW
    int count = 0;
    for(int i = 0 ; i< ROW ; i++){
        for(int j = 0 ; j< COL-1; j++){
            if(arr[i][j] == arr[i][j+1]){
                if(j==COL-2) ++count;
            }else break;
        }
    }
    for(int i = 0 ; i< COL ; i++){
        for(int j = 0 ; j< ROW-1; j++){
            if(arr[j][i] == arr[j+1][i]){
                if(j==ROW-2) ++count;
            }else break;
        }
    }
    for(int i = 0 ; i< COL-1; i++){
        if(arr[i][i] != arr[i+1][i+1]) break;
        if(i == COL-2) count++;
    }
    for(int i = 0 ,j = COL-1; i< COL-1; i++,j--){
        if(arr[i][j] != arr[i+1][j-1]) break;
        if(i == COL-2) count++;
    }
    return count;
}
void loading_quote(void){
    char quote[1000]= "Eyes down, hearts up. The game begins now!";
    clear_lines(100);
    for(int i = 0 ; quote[i]; i++){
        printf("%c",quote[i]);
        fflush(stdout);
        usleep(1000*100);
    }
    printf("\n\nLoading BINGO");
    for(int i = 25 ; i>=0; i--){
        printf(".");
        fflush(stdout);
        usleep(1000*100);
    }
}
void clear_lines(int l){
	for (int i = 0; i < l - 1; i++){
		printf("\033[2K");
		printf("\033[A");
	}
	printf("\r\033[2k");
	fflush(stdout);
}