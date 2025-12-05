#include <stdio.h>

void clear_lines(int l){
        for(int i =0;i<l-1;i++){
                printf("\033[2K");
                printf("\033[A");
        }
        printf("\r\033[2k");
        fflush(stdout);
}
int main(){
	char arr[3][3];
	int x, y;
	int j =1,k = 1,count_row=0,count_col=0,count_dig=0;
	while(k<=9){
		if(j>2){j =1;}
		printf("\nPlayer-%d Enter the position: ",j);
		scanf("%d %d",&x,&y);
		arr[x][y]=(j==1)?'X':'O';
		j++;
		//clear_lines(4);
		for(int i =0 ; i< 3; i++){
			for(int j =0; j< 3; j++){
				(j == 0)?printf("   "):printf("  | ");
				(arr[i][j]=='X')?printf(" X "):(arr[i][j]=='O')?printf(" O "):printf("   ");
			}
			(i!=2)?printf("\n  --------------------  \n"):1;
		}
		if(k>4){
			for(int i =0;i<3;i++){
				(arr[i][0]==arr[i][1] && arr[i][1]==arr[i][2])?count_row++:count_row;
				(count_row>0)?printf("\nplayer-%d %c is the winner\n",j,arr[i][0]):count_row;
				if(count_row>0||count_col>0||count_dig>0){return 0; }
				(arr[0][i]==arr[1][i] && arr[1][i]==arr[2][i])?count_col++:count_col;
				(count_col>0)?printf("\nplayer-%d %c is the winner\n",j,arr[0][i]):count_col;
				if(count_row>0||count_col>0||count_dig>0){return 0; }
				(arr[1][1]==arr[2][2] && arr[1][1]==arr[0][0])?count_dig++:count_dig;
				(arr[0][2]==arr[1][1] && arr[1][1]==arr[2][0])?count_dig++:count_dig;
				(count_dig>0)?printf("\nplayer-%d %c is the winner\n",j,arr[i][i]):count_dig;
				if(count_row>0||count_col>0||count_dig>0){return 0; }
			}
		}
		k++;

	}
	printf("\nMatch Draw\n");
}


