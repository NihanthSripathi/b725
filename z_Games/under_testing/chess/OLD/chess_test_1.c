#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BLOCKS 8
#define WHITE_INITIAL 1
#define BLACK_INITIAL 6

void board_print(void);
int find_pieces(int, int);
int valid_path(int, int, int, int);
void load_board(void);

int Board[BLOCKS][BLOCKS];
char pices[BLOCKS * 4][10] = {"W_R-1", "W_H-1", "W_B-1", " W_Q ", " W_K ", "W_B-2", "W_H-2", "W_R-2 ", " W_S ", " W_S ", " W_S ", " W_S ", " W_S ", " W_S ", " W_S ", " W_S ", "B_R-1", "B_H-1", "B_B-1", " B_Q ", " B_K ", "B_B-2", "B_H-2", "B_R-2", " B_S ", " B_S ", " B_S ", " B_S ", " B_S ", " B_S ", " B_S ", " B_S "};

int move(int current_pos_i, int current_pos_j, int next_pos_i, int next_pos_j)
{
    int piece_type = Board[current_pos_i][current_pos_j];

    if (valid_path(current_pos_i, current_pos_j, next_pos_i, next_pos_j) && (piece_type != 1 && piece_type != 6 && piece_type != 17 && piece_type != 22))
        return 1;

    if ((8 <= piece_type && piece_type < 16) || (24 <= piece_type && piece_type < 32))
    {

        /*********************  Soldiers ***********************/

        if ((((next_pos_j == current_pos_j) && (next_pos_i == (piece_type < 16) ? current_pos_i + 1 : current_pos_i - 1)) || ((next_pos_j == current_pos_j) && (current_pos_i == piece_type < 16 ? WHITE_INITIAL : BLACK_INITIAL) && (next_pos_i == piece_type < 16 ? current_pos_i + 2 : current_pos_i - 2))) && (Board[next_pos_i][next_pos_j] == -1))
        {

            ///////////// Soldiers - Forward ///////////////////
            // printf("\n %d %d  %d %d\n", current_pos_i,current_pos_j, next_pos_i,next_pos_j);
            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            Board[current_pos_i][current_pos_j] = -1;
        }
        else if ((((next_pos_j == current_pos_j + 1) || (next_pos_j == current_pos_j - 1)) && (next_pos_i == (piece_type < 16) ? current_pos_i + 1 : current_pos_i - 1)) && (Board[next_pos_i][next_pos_j] != -1) && ((Board[current_pos_i][current_pos_j] < 15 && Board[next_pos_i][next_pos_j] >= 15) || (Board[current_pos_i][current_pos_j] >= 15 && Board[next_pos_i][next_pos_j] < 15)))
        {

            ///////////// Soldiers - Attack ///////////////////

            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            Board[current_pos_i][current_pos_j] = -1;
        }
        else
        {
            return 1;
        }
    }
    else if (piece_type == 0 || piece_type == 16 || piece_type == 7 || piece_type == 23)
    {
        if (((next_pos_j == current_pos_j) || (next_pos_i == current_pos_i)) && (((Board[current_pos_i][current_pos_j] < 15 && Board[next_pos_i][next_pos_j] >= 15) || (Board[current_pos_i][current_pos_j] >= 15 && Board[next_pos_i][next_pos_j] < 15)) || Board[next_pos_i][next_pos_j] == -1))
        {

            ///////////// Rook  ///////////////////
            // printf("\nhappy_rook\n");
            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            Board[current_pos_i][current_pos_j] = -1;
        }
        else
        {
            return 1;
        }
    }
    else if (piece_type == 2 || piece_type == 18 || piece_type == 5 || piece_type == 21)
    {
        // printf("\nhappy-2_a\n");
        if ((abs(current_pos_i - next_pos_i) == abs(current_pos_j - next_pos_j)) && ((((Board[current_pos_i][current_pos_j] < 15) && (Board[next_pos_i][next_pos_j] >= 15)) || ((Board[current_pos_i][current_pos_j] >= 15) && (Board[next_pos_i][next_pos_j] < 15))) || (Board[next_pos_i][next_pos_j] == -1)))
        {

            ///////////// Bishop  ///////////////////

            // printf("\nhappy-2-b\n");
            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            Board[current_pos_i][current_pos_j] = -1;
        }
        else
        {
            return 1;
        }
    }
    else if (piece_type == 3 || piece_type == 19)
    {
        if (((current_pos_i == next_pos_i) || (current_pos_j == next_pos_j) || (abs(current_pos_i - next_pos_i) == abs(current_pos_j - next_pos_j))) && (((Board[current_pos_i][current_pos_j] < 15 && Board[next_pos_i][next_pos_j] >= 15) || (Board[current_pos_i][current_pos_j] >= 15 && Board[next_pos_i][next_pos_j] < 15)) || Board[next_pos_i][next_pos_j] == -1))
        {

            ///////////////// QUEEN ///////////////////////

            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            Board[current_pos_i][current_pos_j] = -1;
        }
        else
        {
            return 1;
        }
    }
    else if (piece_type == 1 || piece_type == 6 || piece_type == 17 || piece_type == 22)
    {
        if (((((current_pos_i + 2 == next_pos_i) || (current_pos_i - 2 == next_pos_i)) && ((current_pos_j + 1 == next_pos_j) || (current_pos_j - 1 == next_pos_j))) || (((current_pos_i + 1 == next_pos_i) || (current_pos_i - 1 == next_pos_i)) && ((current_pos_j + 2 == next_pos_j) || (current_pos_j - 2 == next_pos_j)))) && (((Board[current_pos_i][current_pos_j] < 15 && Board[next_pos_i][next_pos_j] >= 15) || (Board[current_pos_i][current_pos_j] >= 15 && Board[next_pos_i][next_pos_j] < 15)) || Board[next_pos_i][next_pos_j] == -1))
        {

            ///////////////// HORSE ///////////////////////

            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            Board[current_pos_i][current_pos_j] = -1;
        }
        else
        {
            return 1;
        }
    }else if(piece_type == 4 || piece_type == 20){
        if((current_pos_i+1 == next_pos_i || current_pos_j+1 == next_pos_j || current_pos_i-1 == next_pos_i || current_pos_j-1 == next_pos_j) && (((Board[current_pos_i][current_pos_j] < 15 && Board[next_pos_i][next_pos_j] >= 15) || (Board[current_pos_i][current_pos_j] >= 15 && Board[next_pos_i][next_pos_j] < 15)) || Board[next_pos_i][next_pos_j] == -1)){
            ///////////////// KING ///////////////////////

            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            Board[current_pos_i][current_pos_j] = -1;
        } else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

int main()
{
    load_board();
    board_print();

    // (move(1, 1, 3, 1)) ? printf("\nInvalid Move\n") : board_print();
    // (move(3, 1, 4, 1)) ? printf("\nInvalid Move\n") : board_print();
    // (move(4, 1, 5, 1)) ? printf("\nInvalid Move\n") : board_print();
    // (move(6, 0, 5, 1)) ? printf("\nInvalid Move\n") : board_print();
    // (move(7, 0, 1, 0)) ? printf("\nInvalid Move\n") : board_print();
    // (move(6, 3, 5, 3)) ? printf("\nInvalid Move\n") : board_print();
    // (move(7, 2, 5, 4)) ? printf("\nInvalid Move\n") : board_print();
    // (move(5, 4, 4, 3)) ? printf("\nInvalid Move\n") : board_print();
    // (move(4, 3, 5, 4)) ? printf("\nInvalid Move\n") : board_print();
    // (move(0, 2, 2, 0)) ? printf("\nInvalid Move\n") : board_print();
    // (move(7, 1, 5, 2)) ? printf("\nInvalid Move\n") : board_print();
    // (move(5, 2, 3, 3)) ? printf("\nInvalid Move\n") : board_print();
    // (move(3, 3, 1, 4)) ? printf("\nInvalid Move\n") : board_print();
    // (move(7, 6, 5, 5)) ? printf("\nInvalid Move\n") : board_print();
    // (move(5, 5, 6, 3)) ? printf("\nInvalid Move\n") : board_print();
    // (move(0, 6, 2, 7)) ? printf("\nInvalid Move\n") : board_print();
    // (move(0, 4, 1, 4)) ? printf("\nInvalid Move\n") : board_print();
    // (move(1, 4, 3, 4)) ? printf("\nInvalid Move\n") : board_print();

    int i = 2 ,current_pos_i,current_pos_j,next_pos_i,next_pos_j;
    while(1){
        
        printf("\n%s : ",(++i&1)?"White's Move":"Black's Move" );
        scanf("%d%d%d%d",&current_pos_i,&current_pos_j,&next_pos_i,&next_pos_j);
        if(move(current_pos_i,current_pos_j,next_pos_i,next_pos_j)) printf("\nInvalid Move\n"); else board_print();
    }
    
    // printf("\n%d\n", Board[1][3]);
}

int valid_path(int current_pos_i, int current_pos_j, int next_pos_i, int next_pos_j)
{
    if ((current_pos_i == next_pos_i) || (current_pos_j == next_pos_j))
    {
        /**************** ROOKE (OR) QUEEN *****************/
        if (current_pos_i == next_pos_i)
        {

            while (current_pos_j < next_pos_j - 1)
            {
                // printf("happy %d %d %d\n",current_pos_i+1,current_pos_j,Board[current_pos_i+1][current_pos_j]);
                if (Board[current_pos_i][++current_pos_j] != -1)
                    return 1;
            }
            while (current_pos_j > next_pos_j + 1)
            {
                if (Board[current_pos_i][--current_pos_j] != -1)
                    return 1;
            }
        }
        else
        {
            while (current_pos_i < next_pos_i - 1)
            {
                // printf("happy %d %d %d\n",current_pos_i+1,current_pos_j,Board[current_pos_i+1][current_pos_j]);
                if (Board[++current_pos_i][current_pos_j] != -1)
                    return 1;
            }
            // printf("\nhappy2");
            while (current_pos_i > next_pos_i + 1)
            {
                // printf("happy\n %d %d %d\n",current_pos_i-1,current_pos_j,Board[current_pos_i-1][current_pos_j]);
                if (Board[--current_pos_i][current_pos_j] != -1)
                    return 1;
            }
        }
    }
    else if (abs(current_pos_i - next_pos_i) == abs(current_pos_j - next_pos_j))
    {
        /**************** BISHOP (OR) QUEEN *****************/

        // printf("happy_B");
        while ((current_pos_i > next_pos_i + 1) && (current_pos_j > next_pos_j + 1))
        {
            //  printf("\npresent %d %d %d\n",current_pos_i,current_pos_j,Board[current_pos_i][current_pos_j]);
            //  printf("\nfuture %d %d %d\n",current_pos_i-1,current_pos_j-1,Board[current_pos_i-1][current_pos_j-1]);
            if (Board[--current_pos_i][--current_pos_j] != -1)
                return 1;
        }
        while ((current_pos_i < next_pos_i - 1) && (current_pos_j < next_pos_j - 1))
        {
            // printf("\npresent %d %d %d\n", current_pos_i, current_pos_j, Board[current_pos_i][current_pos_j]);
            if (Board[++current_pos_i][++current_pos_j] != -1)
                return 1;
        }
        while ((current_pos_i - 1 > next_pos_i) && (current_pos_j + 1 < next_pos_j))
        {
            // printf("\npresent %d %d %d\n",current_pos_i,current_pos_j,Board[current_pos_i][current_pos_j]);
            // printf("\nfuture %d %d %d\n", current_pos_i - 1, current_pos_j +1, Board[current_pos_i - 1][current_pos_j +1]);
            if (Board[--current_pos_i][++current_pos_j] != -1)
                return 1;
        }
        while ((current_pos_i + 1 < next_pos_i - 1) && (current_pos_j - 1 > next_pos_j + 1))
        {
            // printf("\nfuture %d %d %d\n", current_pos_i + 1, current_pos_j - 1, Board[current_pos_i + 1][current_pos_j - 1]);
            if (Board[++current_pos_i][--current_pos_j] != -1)
                return 1;
        }
    }
    else
    {
        return 1;
    }
    // printf("happy");
    return 0;
}
void load_board()
{
    for (int i = 0; i < BLOCKS; i++)
    {
        for (int j = 0; j < BLOCKS; j++)
        {
            if (i == 0)
            {
                Board[i][j] = BLOCKS * 0 + j;
            }
            else if (i == 1)
            {
                Board[i][j] = BLOCKS * 1 + j;
            }
            else if (i == 7)
            {
                Board[i][j] = BLOCKS * 2 + j;
            }
            else if (i == 6)
            {
                Board[i][j] = BLOCKS * 3 + j;
            }
            else
            {
                Board[i][j] = -1;
            }
        }
    }
}

void board_print()
{
    for (int i = 0; i < BLOCKS; i++)
        printf("     %2d", i);
    printf("\n\n");

    for (int i = 0; i < BLOCKS; i++)
    {
        printf(" %d ", i);
        for (int j = 0; j < BLOCKS; j++)
        {
            (Board[i][j] == -1) ? ((i + j) & 1) ? printf("  ---  ") : printf("   -   ") : printf(" %s ", pices[Board[i][j]]);
            // printf("   %2d  ",Board[i][j]);
        }
        printf("\n\n");
    }
    printf("\n\n");
}
int find_pieces(int current_pos_i, int current_pos_j)
{
    if (Board[current_pos_i][current_pos_j] == -1)
    {
        return -1;
    }
    for (int i = 0; i < BLOCKS * 4; i++)
    {
        if (strcmp(pices[Board[current_pos_i][current_pos_j]], pices[i]) == 0)
            return i;
    }
}