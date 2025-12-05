#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#define PORT 7777

#define ROW 5
#define COL 5

void disp(void);
int calling(int);
int search(void);
int is_dup(int);
int create_rand_num(void);
void clear_lines(int);
void loading_quote(void);

int arr[ROW][COL];

int count = 0;
int arr_size = 25;

int player2_fd, player1_fd;
struct sockaddr_in addr;
socklen_t addrlen = sizeof(addr);
char buff[100];

int socket_setup(int player){

    if(player==1){
        // create_server_for_player_1
        while (1){
            player1_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (player1_fd < 0)
                printf("\rSocket failing..");
            else
                break;
        }
        // set server address to player_1
        memset(&addr, 0, sizeof(addr)); // clearing the buffer before loading data
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);

        //
        printf("Enter the port number:\n");
        char port_num[20];
        scanf("%s", port_num);
        inet_pton(AF_INET,port_num, &addr.sin_addr);

        if (connect(player1_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
            printf("connect failed...try again");
            sleep(3);
            return 1;
        }
        printf("Player_2 Connected succesfully\n");
        return 0;
    }
    else{
        // create_server_for_player_2
        while(1){
            player2_fd = socket(AF_INET, SOCK_STREAM, 0);
            if(player2_fd < 0)
                printf("\rSocket failing..");
            else break;
        }

        // set server address to player_1
        memset(&addr, 0, sizeof(addr)); //clearing the buffer before loading data
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        while(1){
            if(bind(player2_fd,(struct sockadder *)&addr,sizeof(addr))<0)
                printf("\rBind failing..");
            else break;
        }
        listen(player2_fd, 1);
        printf("Waiting for player_1\n");
        player1_fd = accept(player2_fd, (struct sockaddr *)&addr, &addrlen);
        printf("Player_1 connected succesfully!\n");
        return 0;
    }
    return 1;
}

int main(){
    pid_t pid = fork();
    if(pid == 0){
        srand(time(NULL));
        for (int i = 0; i < ROW; i++){
            for (int j = 0; j < COL; j++){
                arr[i][j] = create_rand_num();
            }
        }
        _exit(0);
    }
    printf("Choose player :\nPlayer - 1\nPlayer - 2\n");
    int player;
    scanf("%d", &player);
    while(socket_setup(player)!=0);
    int WIN = 0, val;
    clear_lines(100);
    disp();
    while (1){
        if(player==1){
            printf("Your's Turn :");
            fgets(buff, sizeof(buff), stdin);
            write(player2_fd, buff, strlen(buff));
            if (strncmp(buff, "exit", 4) == 0){
                printf("Player-2 Exited\n");
                return 0;
            }
            val = atoi(buff);
            WIN = calling(val);
            if (WIN)break;
            memset(buff, 0, sizeof(buff));
            read(player2_fd, buff, sizeof(buff));
            printf("Player_2's choice : %s\n",buff);
            val = atoi(buff);
            WIN = calling(val);
            if (WIN)break;
        }else{
            memset(buff, 0, sizeof(buff));
            read(player1_fd, buff, sizeof(buff));
            printf("Player_1's Turn : %s\n", buff);
            val = atoi(buff);
            WIN = calling(val);
            if (WIN)
                break;
            printf("Your's Turn :");
            if (strncmp(buff, "exit", 4) == 0){
                printf("Player-1 Exited\n");
                return 0;
            }
            fgets(buff, sizeof(buff), stdin);
            write(player1_fd, buff, strlen(buff));
            val = atoi(buff);
            WIN = calling(val);
            if (WIN)
                break;
        }
    }
    if (WIN && (player == 1)){
        close(player1_fd);
    }
    return 0;
}

int create_rand_num()
{
    int min = 1, max = 25;
    int rand_num = 0;
    while (1)
    {
        rand_num = (rand() % (max - min + 1)) + min;
        if (is_dup(rand_num))
            continue;
        else
        {
            return rand_num;
        }
    }
}
int is_dup(int val)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
            if (arr[i][j] == val)
                return 1;
    }
    return 0;
}
void disp()
{
    printf("\n");
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            printf(" %2d ", arr[i][j]);
        }
        printf("\n\n");
    }
    printf("\n");
}
int calling(int val)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (arr[i][j] == val)
            {
                arr[i][j] = 0;
            }
        }
    }
    int present_count = search();
    if (count < present_count)
    {
        count = present_count;
        clear_lines(100);
        printf("\n Congratulations Cleared : %d \n", count);
        disp();
    }
    if (count >= ROW)
    {
        clear_lines(100);
        printf("\n\n//***** BINGO *****\\\\\n\n");
        disp();
        printf("\\\\***** BINGO ****//\n\n\n");
        return 1;
    }
    else
    {
        return 0;
    }
}
int search()
{
    // ROW
    int count = 0;
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL - 1; j++)
        {
            if (arr[i][j] == arr[i][j + 1])
            {
                if (j == COL - 2)
                    ++count;
            }
            else
                break;
        }
    }
    for (int i = 0; i < COL; i++)
    {
        for (int j = 0; j < ROW - 1; j++)
        {
            if (arr[j][i] == arr[j + 1][i])
            {
                if (j == ROW - 2)
                    ++count;
            }
            else
                break;
        }
    }
    for (int i = 0; i < COL - 1; i++)
    {
        if (arr[i][i] != arr[i + 1][i + 1])
            break;
        if (i == COL - 2)
            count++;
    }
    for (int i = 0, j = COL - 1; i < COL - 1; i++, j--)
    {
        if (arr[i][j] != arr[i + 1][j - 1])
            break;
        if (i == COL - 2)
            count++;
    }
    return count;
}
void loading_quote(void)
{
    char quote[1000] = "Eyes down, hearts up. The game begins now!";
    clear_lines(100);
    for (int i = 0; quote[i]; i++)
    {
        printf("%c", quote[i]);
        fflush(stdout);
        usleep(1000 * 100);
    }
    printf("\n\nLoading BINGO");
    for (int i = 25; i >= 0; i--)
    {
        printf(".");
        fflush(stdout);
        usleep(1000 * 100);
    }
}
void clear_lines(int l)
{
    for (int i = 0; i < l - 1; i++)
    {
        printf("\033[2K");
        printf("\033[A");
    }
    printf("\r\033[2k");
    fflush(stdout);
}