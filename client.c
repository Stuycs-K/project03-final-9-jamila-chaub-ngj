#include "networking.h"
#include "game.h"
#include <pwd.h> 



void checkConnection (int server_socket, char * message) {
    if (server_socket <= 0) {
        printf("Error: %s\n", message);
        if (server_socket == 0 ) {
            printf("Lost connection with the server\n");
        }
        exit(1);
    }
}


int server_handling (int server_socket) {
    int flag = 1; // this will be used to identify the winner
    char * buff = malloc (sizeof(char) * BUFFER_SIZE); 
    // maybe add a struct here? 
    // this can help identify which one this is from? 
    while ( flag) {
        // THIS READ IS TO CHECK 
        // strcpy(buff, "Ready to play");
        // int bytes = write(server_socket, buff, BUFFER_SIZE);
        // checkConnection(bytes, "could not write to the server socket [LINE 28]"); 
        // printf("Waiting for round to start\n");
        // // COMMENT THIS OUT LATER
        // printf("%d bytes written [LINE 28]\n", bytes);
    
        //THIS IS READING THE CONFIRMATION FROM THE SERVER SIDE 
        int bytes = read(server_socket, buff, BUFFER_SIZE);
        checkConnection(bytes, "could not read from the server socket [LINE 32]"); 

        printf("%s %d\n", buff, bytes); 
        if(!strcmp(buff, "the winner is you")){
            free(buff);
            return 0;
        }

        char * data = malloc(sizeof(char) * BUFFER_SIZE);

        //THIS IS FROM THE CLIENT HANDLING FUNCTION IN SERVER 
        //NVM ... WHAT IS THIS EVEN READING?????? 
        bytes = read(server_socket, data, BUFFER_SIZE); 
        checkConnection(bytes, "could not read [LINE 41]"); 

        //COMMENT THIS OUT LATER
        printf("%s %d [LINE 41]\n", data, bytes);

        // struct clientDetails * data = malloc(sizeof(struct clientDetails)); 
        // read(server_socket, data, sizeof (struct clientDetails)); 
        printf("enter a number: "); 
        fgets(buff, BUFFER_SIZE, stdin);
        buff = strsep(&buff, "\n"); 
        // preprocess the buff

        // maybe add logic here to check whether or not this
        // part is the right input? 
        // should we have error checking to be local? 

        // it would be cool to have a help command here! 

        //WRITE THE BUFF MESSAGE 
        bytes = write(server_socket, buff, BUFFER_SIZE); 
        checkConnection(bytes, "could not write to the server socket [LINE 62]"); 

        //READ THE SERVER RESPONSE 
        bytes = read(server_socket, buff, BUFFER_SIZE); 
        checkConnection(bytes, "could not read from server socket [LINE 65]"); 

        // probably check if less than or equal and then throw an error here if 0 bytes
        sscanf(buff, "%d", &flag); 
        if(flag == 0){
            printf("You've lost :(\n");
        } 
    }
    free(buff); 

    return flag; 

} 



int main (int argc, char *argv[]) {
    char* IP = "127.0.0.1";
    if (argc > 1)
    {
        IP = argv[1];
    }


    // this is for getting your user  
    char * userName = malloc(sizeof (char) * NAME_SIZE); 
    uid_t uid = geteuid(); 
    struct passwd *pw = getpwuid(uid);
    if (pw) {
        strcpy (userName, pw->pw_name); 
    }
    else { // if we can't retrieve a username for this 
        printf("enter a username: "); 
        if (fgets(userName, NAME_SIZE, stdin)) {
            printf("failed to fgets");
            exit(1);  
        } 
        
    }  

    int server_socket = client_tcp_handshake(IP);
    printf("user: %s joined server successfully!\n", userName);

    // WRITE THE USER TO THE SERVER 
    int bytes = write(server_socket, userName, NAME_SIZE);  
    err(bytes, "could not write the bytes to the server socket"); 
    // add logic for everything here 
    server_handling (server_socket); 

    free(userName);
} 

