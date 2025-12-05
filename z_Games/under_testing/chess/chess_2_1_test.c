

/*
 * Chess Game Implementation in C
 *
 * This program simulates a basic chess game with piece movements, board display,
 * pawn promotion, and check detection. It uses an 8x8 board represented by a 2D array.
 * Pieces are identified by integer indices, with -1 indicating empty squares.
 *
 * Piece indices:
 * White: 0-15 (Rooks: 0,7; Knights: 1,6; Bishops: 2,5; Queen: 3; King: 4; Pawns: 8-15)
 * Black: 16-31 (Rooks: 16,23; Knights: 17,22; Bishops: 18,21; Queen: 19; King: 20; Pawns: 24-31)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Constants
#define BLOCKS 8          // Board size (8x8)
#define WHITE_INITIAL 1   // Starting row for white pawns
#define BLACK_INITIAL 6   // Starting row for black pawns

// Function prototypes
void board_print(void);
int find_pieces(int, int);
int valid_path(int, int, int, int);
void load_board(void);
void soldire_to_power_check(void);
int check_to_king(void);
void clear_lines(int );

// Global variables
int Board[BLOCKS][BLOCKS];  // 8x8 chess board, holds piece indices or -1 for empty
char pices[BLOCKS * 4][10] = {"W_R-1", "W_H-1", "W_B-1", " W_K ", " W_Q ", "W_B-2", "W_H-2", "W_R-2 ", " W_S ", " W_S ", " W_S ", " W_S ", " W_S ", " W_S ", " W_S ", " W_S ", "B_R-1", "B_H-1", "B_B-1", " B_Q ", " B_K ", "B_B-2", "B_H-2", "B_R-2", " B_S ", " B_S ", " B_S ", " B_S ", " B_S ", " B_S ", " B_S ", " B_S "};  // Piece names for display

/*
 * Attempts to move a piece from (current_pos_i, current_pos_j) to (next_pos_i, next_pos_j).
 * Validates the move based on piece type and chess rules.
 * Returns 0 on success, 1 on invalid move.
 */
int move(int current_pos_i, int current_pos_j, int next_pos_i, int next_pos_j)
{
    // printf("\nEntered MOVE FUNCTION\n");
    // Get the piece type at the current position
    int piece_type = Board[current_pos_i][current_pos_j];
    // Get the piece type at the target position
    int future_piece = Board[next_pos_i][next_pos_j];
    // printf("%s : %s \n", pices[piece_type], pices[future_piece]);
    // Prevent moving to a square occupied by the same color piece
    if ((piece_type < 16 && future_piece < 16) || (piece_type >= 16 && future_piece >= 16)){
        // printf("%s\n",pices[piece_type]);
        if ((future_piece != -1) && (piece_type != -1))
            return 1;
    }

    // For sliding pieces (rook, bishop, queen), check if the path is clear
    if (valid_path(current_pos_i, current_pos_j, next_pos_i, next_pos_j) && (piece_type != 1 && piece_type != 6 && piece_type != 17 && piece_type != 22))
        return 1;
    // Pawn movement logic
    if ((8 <= piece_type && piece_type < 16) || (24 <= piece_type && piece_type < 32))
    {
        /*********************  Soldiers (Pawns) ***********************/

        // Check for forward move: one step or two from starting position if empty
        if ((((next_pos_j == current_pos_j) && (next_pos_i == (piece_type < 16) ? current_pos_i + 1 : current_pos_i - 1)) || ((next_pos_j == current_pos_j) && (current_pos_i == piece_type < 16 ? WHITE_INITIAL : BLACK_INITIAL) && (next_pos_i == piece_type < 16 ? current_pos_i + 2 : current_pos_i - 2))) && (Board[next_pos_i][next_pos_j] == -1))
        {
            ///////////// Soldiers - Forward Move ///////////////////
            // printf("\n %d %d  %d %d\n", current_pos_i,current_pos_j, next_pos_i,next_pos_j);
            // Move the piece to the new position
            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            // Clear the old position
            Board[current_pos_i][current_pos_j] = -1;
            // Check for pawn promotion
            soldire_to_power_check();
        }
        // Check for diagonal attack on opponent piece
        else if ((((next_pos_j == current_pos_j + 1) || (next_pos_j == current_pos_j - 1)) && (next_pos_i == (piece_type < 16) ? current_pos_i + 1 : current_pos_i - 1)) && (Board[next_pos_i][next_pos_j] != -1) && ((Board[current_pos_i][current_pos_j] <= 15 && Board[next_pos_i][next_pos_j] >= 15) || (Board[current_pos_i][current_pos_j] >= 15 && Board[next_pos_i][next_pos_j] <= 15)))
        {
            ///////////// Soldiers - Attack Move ///////////////////
            // Capture the opponent piece
            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            Board[current_pos_i][current_pos_j] = -1;
            soldire_to_power_check();
        }
        else
        {
            return 1;
        }
    }
    // Rook movement logic
    else if (piece_type == 0 || piece_type == 16 || piece_type == 7 || piece_type == 23)
    {
        // Rook moves horizontally or vertically
        if (((next_pos_j == current_pos_j) || (next_pos_i == current_pos_i)) && (((Board[current_pos_i][current_pos_j] <= 15 && Board[next_pos_i][next_pos_j] >= 15) || (Board[current_pos_i][current_pos_j] >= 15 && Board[next_pos_i][next_pos_j] <= 15)) || Board[next_pos_i][next_pos_j] == -1))
        {
            ///////////// Rook Move ///////////////////
            // printf("\nhappy_rook\n");
            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            Board[current_pos_i][current_pos_j] = -1;
        }
        else
        {
            return 1;
        }
    }
    // Bishop movement logic
    else if (piece_type == 2 || piece_type == 18 || piece_type == 5 || piece_type == 21)
    {
        // printf("\nhappy-2_a\n");
        // Bishop moves diagonally
        if ((abs(current_pos_i - next_pos_i) == abs(current_pos_j - next_pos_j)) && ((((Board[current_pos_i][current_pos_j] <= 15) && (Board[next_pos_i][next_pos_j] >= 15)) || ((Board[current_pos_i][current_pos_j] >= 15) && (Board[next_pos_i][next_pos_j] <= 15))) || (Board[next_pos_i][next_pos_j] == -1)))
        {
            ///////////// Bishop Move ///////////////////
            // printf("\nhappy-2-b\n");
            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            Board[current_pos_i][current_pos_j] = -1;
        }
        else
        {
            return 1;
        }
    }
    // Queen movement logic
    else if (piece_type == 4 || piece_type == 19)
    {
        // Queen moves like rook or bishop
        if (((current_pos_i == next_pos_i) || (current_pos_j == next_pos_j) || (abs(current_pos_i - next_pos_i) == abs(current_pos_j - next_pos_j))) && (((Board[current_pos_i][current_pos_j] <= 15 && Board[next_pos_i][next_pos_j] >= 15) || (Board[current_pos_i][current_pos_j] >= 15 && Board[next_pos_i][next_pos_j] <= 15)) || Board[next_pos_i][next_pos_j] == -1))
        {
            ///////////////// Queen Move ///////////////////////
            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            Board[current_pos_i][current_pos_j] = -1;
        }
        else
        {
            return 1;
        }
    }
    // Knight movement logic
    else if (piece_type == 1 || piece_type == 6 || piece_type == 17 || piece_type == 22)
    {
        // Knight moves in L-shape: 2 in one direction, 1 perpendicular
        if (((((current_pos_i + 2 == next_pos_i) || (current_pos_i - 2 == next_pos_i)) && ((current_pos_j + 1 == next_pos_j) || (current_pos_j - 1 == next_pos_j))) || (((current_pos_i + 1 == next_pos_i) || (current_pos_i - 1 == next_pos_i)) && ((current_pos_j + 2 == next_pos_j) || (current_pos_j - 2 == next_pos_j)))) && (((Board[current_pos_i][current_pos_j] <= 15 && Board[next_pos_i][next_pos_j] >= 15) || (Board[current_pos_i][current_pos_j] >= 15 && Board[next_pos_i][next_pos_j] <= 15)) || Board[next_pos_i][next_pos_j] == -1))
        {
            ///////////////// Knight Move ///////////////////////
            // printf(" %d %d \n", current_pos_i,current_pos_j);
            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            Board[current_pos_i][current_pos_j] = -1;
        }
        else
        {
            return 1;
        }
    }
    // King movement logic
    else if(piece_type == 3 || piece_type == 20){
        // King moves one square in any direction
        if(current_pos_i+1 == next_pos_i || current_pos_j+1 == next_pos_j || current_pos_i-1 == next_pos_i || current_pos_j-1 == next_pos_j){
            ///////////////// King Move ///////////////////////
            Board[next_pos_i][next_pos_j] = Board[current_pos_i][current_pos_j];
            Board[current_pos_i][current_pos_j] = -1;
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

/*
 * Main function: Initializes the board, prints it, and demonstrates some pawn moves.
 */
int main()
{
    // Initialize the chess board with starting positions
    load_board();
    // Print the initial board
    board_print();
    int current_pos_i, current_pos_j, next_pos_i, next_pos_j;

    for (int i = 1;; i++){
        if(i&1){
            printf("Whites Move : ");
        }else{
            printf("Blacks Move : ");
        }
        // char input_buffer[10];
        // fgets(input_buffer,100,stdin);
        // printf("%s",input_buffer);
        scanf("%d%d%d%d", &current_pos_i, &current_pos_j, &next_pos_i, &next_pos_j);
        // printf("%d %d %d %d \n",current_pos_i,current_pos_j,next_pos_i,next_pos_j);
        --current_pos_i,--current_pos_j;
        --next_pos_i,--next_pos_j;
        // current_pos_j = toupper(current_pos_j);
        // next_pos_j = toupper(next_pos_j);

        // printf("%d %d %d %d \n",current_pos_i,current_pos_j,next_pos_i,next_pos_j);
        // Get the piece type at the current position
        int piece_type = Board[current_pos_i][current_pos_j];

        // printf("%d : %d", piece_type,(i&1));
        if (piece_type < 16 && (i%2==0))
        {
            printf("\nInvalid Move_W\n\n");
            i--;
        }
        else if (piece_type >= 16 && (i % 2 == 1))
        {
            printf("\nInvalid Move_B\n\n");
            i--;
        }
        else
        {
            int move_flag = move(current_pos_i, current_pos_j, next_pos_i, next_pos_j);
            if(move_flag){
                printf("\nInvalid Move_M\n\n");
                i--;
                continue;
            }
            board_print();
        }
    }

        /////////// //////////////     HORSE     /////////////////////////
        // (move(7, 6, 5, 5)) ? printf("\nInvalid Move\n") : board_print();
        // (move(5, 5, 3, 6)) ? printf("\nInvalid Move\n") : board_print();
        // (move(3, 6, 1, 7)) ? printf("\nInvalid Move\n") : board_print();

        ////////////////////////     SOLDIURE     ///////////////////////
        // (move(1, 1, 3, 1)) ? printf("\nInvalid Move\n") : board_print();
        // (move(3, 1, 4, 1)) ? printf("\nInvalid Move\n") : board_print();
        // (move(4, 1, 5, 1)) ? printf("\nInvalid Move\n") : board_print();
        // (move(5, 1, 6, 0)) ? printf("\nInvalid Move\n") : board_print();
        // (move(6, 0, 7, 1)) ? printf("\nInvalid Move\n") : board_print();
        // (move(7, 1, 7, 2)) ? printf("\nInvalid Move\n") : board_print();
        // (move(7, 2, 7, 3)) ? printf("\nInvalid Move\n") : board_print();
        // (move(7, 4, 7, 3)) ? printf("\nInvalid Move\n") : board_print();

        ////////////////////////     ROOK     ////////////////////////
        // (move(7, 0, 1, 0)) ? printf("\nInvalid Move\n") : board_print();
        // (move(1, 0, 1, 2)) ? printf("\nInvalid Move\n") : board_print();
        // (move(1, 2, 1, 3)) ? printf("\nInvalid Move\n") : board_print();
        // (move(1, 3, 1, 4)) ? printf("\nInvalid Move\n") : board_print();
        // (move(1, 4, 0, 4)) ? printf("\nInvalid Move\n") : board_print();

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

        // if(move(0,1,0,3)) printf("\nInvalid Move\n"); else board_print();
        // if(move(6,,5,3)) printf("\nInvalid Move\n"); else board_print();

        // printf("\n%d\n", Board[1][3]);

        return 0;
}

/*
 * Checks if the path between current and next positions is clear for sliding pieces (rook, bishop, queen).
 * Returns 0 if path is clear, 1 if blocked or invalid path.
 */
int valid_path(int current_pos_i, int current_pos_j, int next_pos_i, int next_pos_j)
{
    // Horizontal or vertical movement (Rook or Queen)
    if ((current_pos_i == next_pos_i) || (current_pos_j == next_pos_j))
    {
        /**************** ROOK (OR) QUEEN *****************/
        if (current_pos_i == next_pos_i)
        {
            // Moving horizontally right
            while (current_pos_j < next_pos_j - 1)
            {
                // printf("happy %d %d %d\n",current_pos_i+1,current_pos_j,Board[current_pos_i+1][current_pos_j]);
                if (Board[current_pos_i][++current_pos_j] != -1)
                    return 1;  // Path blocked
            }
            // Moving horizontally left
            while (current_pos_j > next_pos_j + 1)
            {
                if (Board[current_pos_i][--current_pos_j] != -1)
                    return 1;  // Path blocked
            }
        }
        else
        {
            // Moving vertically down
            while (current_pos_i < next_pos_i - 1)
            {
                // printf("happy %d %d %d\n",current_pos_i+1,current_pos_j,Board[current_pos_i+1][current_pos_j]);
                if (Board[++current_pos_i][current_pos_j] != -1)
                    return 1;  // Path blocked
            }
            // Moving vertically up
            while (current_pos_i > next_pos_i + 1)
            {
                // printf("happy\n %d %d %d\n",current_pos_i-1,current_pos_j,Board[current_pos_i-1][current_pos_j]);
                if (Board[--current_pos_i][current_pos_j] != -1)
                    return 1;  // Path blocked
            }
        }
    }
    // Diagonal movement (Bishop or Queen)
    else if (abs(current_pos_i - next_pos_i) == abs(current_pos_j - next_pos_j))
    {
        /**************** BISHOP (OR) QUEEN *****************/
        // Diagonal up-left
        while ((current_pos_i > next_pos_i + 1) && (current_pos_j > next_pos_j + 1))
        {
            //  printf("\npresent %d %d %d\n",current_pos_i,current_pos_j,Board[current_pos_i][current_pos_j]);
            //  printf("\nfuture %d %d %d\n",current_pos_i-1,current_pos_j-1,Board[current_pos_i-1][current_pos_j-1]);
            if (Board[--current_pos_i][--current_pos_j] != -1)
                return 1;  // Path blocked
        }
        // Diagonal down-right
        while ((current_pos_i < next_pos_i - 1) && (current_pos_j < next_pos_j - 1))
        {
            // printf("\npresent %d %d %d\n", current_pos_i, current_pos_j, Board[current_pos_i][current_pos_j]);
            if (Board[++current_pos_i][++current_pos_j] != -1)
                return 1;  // Path blocked
        }
        // Diagonal up-right
        while ((current_pos_i - 1 > next_pos_i) && (current_pos_j + 1 < next_pos_j))
        {
            // printf("\npresent %d %d %d\n",current_pos_i,current_pos_j,Board[current_pos_i][current_pos_j]);
            // printf("\nfuture %d %d %d\n", current_pos_i - 1, current_pos_j +1, Board[current_pos_i - 1][current_pos_j +1]);
            if (Board[--current_pos_i][++current_pos_j] != -1)
                return 1;  // Path blocked
        }
        // Diagonal down-left
        while ((current_pos_i + 1 < next_pos_i - 1) && (current_pos_j - 1 > next_pos_j + 1))
        {
            // printf("\nfuture %d %d %d\n", current_pos_i + 1, current_pos_j - 1, Board[current_pos_i + 1][current_pos_j - 1]);
            if (Board[++current_pos_i][--current_pos_j] != -1)
                return 1;  // Path blocked
        }
    }
    else{
        return 1;  // Invalid path (not straight or diagonal)
    }
    return 0;  // Path is clear
}
/*
 * Initializes the chess board with starting positions for all pieces.
 * White pieces on rows 0-1, black pieces on rows 6-7, empty squares in between.
 */
void load_board(){
    for (int i = 0; i < BLOCKS; i++){
        for (int j = 0; j < BLOCKS; j++){
            if (i == 0){
                // White back rank: Rooks, Knights, Bishops, Queen, King
                Board[i][j] = BLOCKS * 0 + j;
            }else if (i == 1){
                // White pawns
                Board[i][j] = BLOCKS * 1 + j;
            }else if (i == 7){
                // Black back rank: Rooks, Knights, Bishops, Queen, King
                Board[i][j] = BLOCKS * 2 + j;
            }else if (i == 6){
                // Black pawns
                Board[i][j] = BLOCKS * 3 + j;
            }else{
                // Empty squares
                Board[i][j] = -1;
            }
        }
    }
}

/*
 * Prints the current state of the chess board to the console.
 * Includes column and row numbers, alternating empty square patterns, and checks for check conditions.
 */
void board_print(){
    clear_lines(100);
    // Check for check conditions before printing
    int check_val = check_to_king();
    
    // Print column headers
    for (int i = 0; i < BLOCKS; i++)
        printf("     %2d", i+1);
    printf("\n\n");

    // Print each row
    for (int i = 0; i < BLOCKS; i++)
    {
        // Print row number
        printf(" %d  ", i+1);
        for (int j = 0; j < BLOCKS; j++)
        {
            // Print empty squares with alternating patterns, or piece names
            (Board[i][j] == -1) ? ((i + j) & 1) ? printf("  ---  ") : printf("   -   ") : printf(" %s ", pices[Board[i][j]]);
        }
        printf("\n\n");
    }
    printf("\n\n");
    if (check_val == -1)
    {
        printf("Black's WON\n\n");
        exit(0);
    }if(check_val == -2){
        printf("Whites WON\n\n");
        exit(0);
    }
}

/*
 * Checks for pawn promotion when a pawn reaches the opposite end of the board.
 * Prompts user to choose promotion piece: Queen, Rook, Bishop, or Knight.
 */
void soldire_to_power_check(){
    for (int i = 0; i < BLOCKS; i++){
        for (int j = 0; j < BLOCKS; j++){
            // White pawn promotion (reaches row 7)
            if ((8 <= Board[i][j]) && (Board[i][j] < 16) && (i == 7)){
                printf("Enter Choice to change the Soldier :\n1. QUEEN\n2. ROOK\n3. Bishop\n4. Horse\n choice : ");
                int choice;
                scanf("%d", &choice);
                // Promote based on choice
                Board[i][j] = (choice == 1) ? 4 : choice == 2 ? 0
                                              : choice == 3   ? 2
                                                              : 1;
            }
            // Black pawn promotion (reaches row 0)
            else if ((24 <= Board[i][j]) && (Board[i][j] < 32) && (i == 0)){
                printf("Enter Choice to change the Soldier :\n1. QUEEN\n2. ROOK\n3. Bishop\n4. Horse\n choice : ");
                int choice;
                scanf("%d", &choice);
                // Promote based on choice
                Board[i][j] = (choice == 1) ? 19 : choice == 2 ? 16
                                               : choice == 3   ? 18
                                                               : 17;
            }
        }
    }
    printf("\n");
}
/*
 * Checks if any piece can attack the opponent's king, indicating check.
 * Prints "Check TO Black" or "Check TO White" if a king is in check.
 */
int check_to_king(void){
    int Black_king_pos_i, Black_king_pos_j, White_king_pos_i, White_king_pos_j;
    int White_king_not_found_flag = 1,Black_king_not_found_flag = 1;
    for (int i = 0; i < BLOCKS; i++)
    {
        for (int j = 0; j < BLOCKS; j++){
            if(Board[i][j]==20 ){
                Black_king_pos_i = i;
                Black_king_pos_j = j;
                Black_king_not_found_flag = 0;
            }
            else if (Board[i][j] == 3){
                White_king_pos_i = i;
                White_king_pos_j = j;
                White_king_not_found_flag = 0;
            }
        }
    }
    if(White_king_not_found_flag)
        return -1;
    if(Black_king_not_found_flag)
        return -2;
    // printf("Black King Position [20] : %d %d\n", Black_king_pos_i, Black_king_pos_j);
    // printf("White King Position [4] : %d %d\n", White_king_pos_i, White_king_pos_j);
    for (int i = 0; i < BLOCKS; i++)
    {
        for (int j = 0; j < BLOCKS; j++){
            int black_check = move(i, j, Black_king_pos_i, Black_king_pos_j);
            int White_check = move(i, j, White_king_pos_i, White_king_pos_j);
            if(black_check==0){
                // printf("%s: %d %d\n",pices[Board[i][j]],i,j);
                Board[i][j] = Board[Black_king_pos_i][Black_king_pos_j];
                Board[Black_king_pos_i][Black_king_pos_j] = 20; // Black king _index : 20
                printf("\nCheck TO Black\n");
              
            }
            if (White_check == 0)
            {
                // printf("%s: %d %d\n", pices[Board[i][j]], i, j);
                Board[i][j] = Board[White_king_pos_i][White_king_pos_j];
                Board[White_king_pos_i][White_king_pos_j] = 4;// White king _index : 4
                printf("\nCheck TO White\n");
                
            }
        }
    }
    return 0;
}
/*
 * Finds the piece index at the given position by comparing piece names.
 * Returns the piece index if found, -1 if the square is empty.
 * Note: This function seems redundant as Board already contains piece indices.
 */
int find_pieces(int current_pos_i, int current_pos_j){
    if (Board[current_pos_i][current_pos_j] == -1){
        return -1;  // Empty square
    }
    for (int i = 0; i < BLOCKS * 4; i++){
        if (strcmp(pices[Board[current_pos_i][current_pos_j]], pices[i]) == 0)
            return i;  // Return matching piece index
    }
    return -1;  // Should not reach here if piece exists
}
void clear_lines(int l){
	for (int i = 0; i < l - 1; i++){
		printf("\033[2K");
		printf("\033[A");
	}
	printf("\r\033[2k");
	fflush(stdout);
}
