/* COMP1011 milestone 3 Growing tree maze generatior, by Tang Ka Chun 17059339D and Tong ka Wing 17063879D. The program use growing tree method. user input will include the wall inside and cell. Then the program compute the number of cells (i.e for a 5 x 5 maze. have 9 cell nodes.)
 
Use link list to store the cell number. Draw one cell from the link list and check is there any unnvisited neighbor. If so, random a possible direction and carve a passage and add the new cell number in the cell list. if not delete the cell node from link list.
 
 Then repeat the process untill all cell node in the link list are all deleted. The program make use of structure and pointer and two dimesional pointer to generate the maze. all the memory used will free at the end of the generation program.
 
 Then the prgram will random a cell and check is it a dead end. if so, then will replace one nearby cell from "#" to " " indoicationg to make a loop and imperfect maze.
 
 Finally, will print out the maze in the text file and user output. "config.txt" is the text file used to store the diamension and the number of level of the maze. "maze.txt" is used to store the maze.
 
 cell structure is for the node in the maze. cellnode structure is for the link list to store the cell number.
 
 If the method to generate is mixed with Prim and backtracking there is a chance the ending point maybe the same as starting point when the size of maze is small.
 
 
 **** Please make the working directory the same as the solver to ensure both program work. ******
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

// Define cell structure for the program. initially have 3 attribustes but then after modification some attributes were deleted and only visit left.
typedef struct cell {
    int visit ;
}cell ;

// Define the cellnode structure for the link list.
typedef struct cellnode {
    int value ;
    struct cellnode* nextnode ;
}cellnode ;
typedef cellnode* cellnodeptr ;

void insertnode( cellnodeptr* topptr, int value ); //To insert celllnode to the link list.
void deletenode( cellnodeptr* topptr, int value ); //To delete the cellnode in the link list.
int findnode( cellnodeptr* topptr, int key ); // Find the keyth cellnode in the list.
int checknode( cellnodeptr* topptr, int value);// Check is the value(cellnode number inside the list.
int generation(int length ,int width, int start ,int method, int time); // Generation of the maze.
void printmaze(char** maze, int time, int length, int width );// print the maze into external txt file
void clearfile(int length, int width, int level); //clear the txt file data if it is already existed and add the diamension of the the maze and how many levels into the txt.
void makeloop(char ** maze,int length, int width); // delete one wall and make the maze inperfect.
void make_start_and_end(char** maze, int start, int end, int width);//mark the starting and ending point at the maze
int is_valid_dead_end (char ** maze, int length, int width, int x, int y ); //check is the choosen wall is valid.

int numberofnode = 0; //count the number of node in the link list.

int main() {
    
    int level = 0,flag = 0, length = 0 , width = 0 , start = 0, method = 0 ;
    srand(time(NULL));
    
    // Input the diamension of the maze and check is the size is valid.
    while (length <2 || width <2) {
        printf("Please input the size of the maze (at least 2x2) : length width : ");
        scanf("%d %d",&length,&width);
    };
    
    // If the size is even number, the size will increased by 1 automatically.
    if (length%2 == 0 ){
        length++;
        flag++;
    } ;
    if (width%2 == 0 ) {
        width++;
        flag++;
    }
    
    // Print to the user the maze size have been changed.
    if (flag != 0 ) printf("The input is even number, and it is not sufficient, therefore the maze size inceased by 1. The new diamension of the maze is %d x %d \n",length ,width);
    
    //make txt file
    
    // Input the number of level the maze will generate.
    do{
        printf("Please input the number of level will be generated : ");
        scanf("%d",&level);
    }while( level == 0);
    
    clearfile(length+2,width+2,level);
    
    // Seclect the method the growing tree use because it can minic Backtracking or Prim's or mix with both method. 0 = Prim's, 1 = backtracking, 2 = mixed
    printf("Please input the method the growing tree use : Prim = 0 , Back tracking = 1 , mixed = 2 : ");
    scanf("%d",&method);
    
    // Random the first cellnode number
    start = rand()%(( (length/2)+1) * ((width/2)+1));
    
    // Generate the mazes
    for (int i = 1 ; i <= level ; i++ ) start = generation(length,width,start,method,i);
    
}

int generation(int length , int width, int start, int method, int time ){
    int x = length/2 +1 ,y = width/2 +1 ,cordinx = 0 , cordiny = 0 , end = 0, currentnode = 0 ,counter = 0, counter1 = 0,dummy = 0;
    
    // Declaear a cellptr for thr list of cell
    cell* cellptr;
    cellptr = malloc( (x * y) * sizeof (cell));
    
    // Initialize the cell's visit value to 0 indicte that all cell are not visited when the program start
    for (int i = 0 ; i < x ; i++){
        for (int j = 0 ; j < y ; j++) {
            (cellptr + ((y * i)+j))->visit = 0 ;
        }
    }
    
    // Declear a pointer to cellnode for the link list
    cellnodeptr topptr = NULL;
    
    // Insert the starting node to the link list
    insertnode(&topptr , start);
    
    // Make a two diamensional maze pointer for display
    char **maze;
    maze = malloc((length+2)*sizeof(char*));
    for (int i = 0 ; i < length+2 ; i++) *(maze + i) = malloc((width+2) * sizeof(char));
    
    // Initialize the mase to all wall indicated by '#'
    for (int i = 0 ; i < length+2 ; i++)
        for (int j = 0 ; j < width+2 ; j++)
            maze[i][j] = '#';
    
    // Mark all the cellnode in the maze as space as it will be always open
    for (int i = 0 ; i < length/2+1 ; i++)
        for (int j = 0 ; j < width/2+1 ; j++)
            maze[i*2+1][j*2+1]= ' ';
    
    while (numberofnode != 0 ) {
        
        // Random the current cell for the loop corresponding to the method used
        if (method == 0) currentnode = findnode(&topptr , rand()%numberofnode);
        else if ( method == 1 )currentnode = findnode(&topptr , numberofnode-1);
        else {
            counter = rand()%2 ;
            if (counter == 1 ) currentnode = findnode(&topptr , rand()%numberofnode);
            else currentnode = findnode(&topptr , numberofnode-1);
        }
        (cellptr + currentnode )->visit = 1;
        
        // Check is all the neighbor cell are all visited. If so, delete the cell from the link list.
        if ( currentnode%y == 0 || (cellptr + currentnode - 1 )->visit == 1 )
            if ( currentnode/y == 0 || (cellptr + currentnode-y )-> visit == 1)
                if ( currentnode%y == y-1 || (cellptr + currentnode+1 )-> visit == 1)
                    if ( currentnode/y == x-1 || (cellptr + currentnode+y )-> visit == 1){
                        if (numberofnode > counter1 && method != 0){ //generate the end point for Prim method
                            end = currentnode;
                            counter1 = numberofnode;
                        }
                        else if( numberofnode == 1 && method == 0) end = currentnode; // Generate the end point for the Backtracking or mixed method
                        deletenode(&topptr, currentnode);
                        continue;
                    }
        
        // Check is the random direction is possible for the current cell. Carve a passage according to the direction and add the cell node
        cordiny = currentnode%y;
        cordinx = currentnode/y;
        
        // Random the direction 0 = north, 1 = east , 2 = south, 3 = west
        dummy = rand() % 4;
        
        // To check whatever the node is at the boundary and contradict with the diction. if the node is not at the boundary, direction match the unvisited cell will carve a passage form the current cell node and add the new visited node to the link list.
        if ( dummy == 0 && currentnode/y != 0 && (cellptr + (currentnode-y))->visit != 1){
            (cellptr + currentnode-y)->visit = 1;
            insertnode(&topptr,currentnode-y);
            maze[cordinx*2][cordiny*2+1] = ' ';
        }
        else if ( dummy == 1 && currentnode%y != y-1 && (cellptr + (currentnode+1))->visit != 1){
            (cellptr + currentnode+1)->visit = 1;
            insertnode(&topptr,currentnode+1);
            maze[cordinx*2+1][cordiny*2+2] = ' ';
        }
        else if ( dummy == 2 && currentnode/y != x-1 && (cellptr + (currentnode+y))->visit != 1){
            (cellptr + currentnode+y)->visit = 1;
            insertnode(&topptr,currentnode+y);
            maze[cordinx*2+2][cordiny*2+1] = ' ';
        }
        else if ( dummy == 3 && currentnode%y != 0 && (cellptr + (currentnode-1))->visit != 1 ){
            (cellptr + currentnode-1)->visit = 1;
            insertnode(&topptr,currentnode-1);
            maze[cordinx*2+1][cordiny*2] = ' ';
        }
    }
    //mark the start and ending point at the map
    make_start_and_end(maze, start, end,y);
    
    //make a loop in the perfect maze
    makeloop(maze,length+2,width+2);
   
    // print out the maze thought the function
    printmaze(maze,time,length+2,width+2);
    
    
    for (int i = 0 ; i < length+2 ; i++){
        for (int j = 0 ; j < width+2 ; j++){
            printf("%c",maze[i][j]);
        }
        printf("\n");
    }

    // Free the cellptr which is the list of cellnode and maze.
    free(maze);
    free(cellptr);
    return end;
    
} // End of generation

// To check whatever the cellnode number is at the link list
int checknode( cellnodeptr* topptr, int value ) {
    cellnodeptr current;
    current = *topptr;
    while ( current != NULL ) {
        if(current->value == value) return 1;
        current = current->nextnode;
    }
    if (current != NULL &&current->value == value) return 1;
    return 0;
}

//  To find the key th cellnode number in the list
int findnode( cellnodeptr* topptr, int key ) {
    cellnodeptr current = *topptr;
    for (int i = 0 ; i<key ; i++) current = current->nextnode;
    return current->value;
}

// Add the node to the link list at the last of the link list
void insertnode(cellnodeptr* topptr , int value) {
    numberofnode++;
    cellnodeptr current = *topptr , previous = NULL,newptr = NULL ;
    newptr = malloc(sizeof(cellnode));
    newptr->nextnode = NULL;
    newptr->value = value;
    while (current != NULL) { // loop until it is end
        previous = current ;
        current = current->nextnode;
    }
    if ( previous == NULL) { // list is empty add at first
        newptr->nextnode = *topptr;
        *topptr = newptr;}
    else {
        previous->nextnode = newptr; // add at the end of the list
        newptr->nextnode = current;
    }
}

// To delete the node from the list and free the unused node space
void deletenode( cellnodeptr* topptr, int value ){
    numberofnode--;
    cellnodeptr previous = *topptr, current = NULL,temp;
    int number;
    if (previous->value == value ){ // ndoe being deleted is at the beginning
        temp = previous;
        number = (*topptr)->value;
        *topptr = (*topptr)->nextnode ;
        free(previous);
    }
    else{
        current = previous->nextnode; // node being deleted is at the middle of the list
        while(current->value != value ){
            previous = current;
            current = current->nextnode;
        }
        temp = current;
        previous->nextnode = current->nextnode;
        number = current->value;
        free(temp);
    }
}

//print out the maze at the txt file located with the xcode file
void printmaze (char** maze, int time, int length, int width ){
    FILE *fptr;
    fptr = fopen("maze.txt","a+");
    for (int i = 0 ; i < length ; i++){
        for (int j = 0 ; j < width ; j++){
            fprintf(fptr,"%c",maze[i][j]);
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
    printf("\nlevel %d successfully printed at text file.\n",time);
}

//clear the current maze.txt file content to ensure the output file is correct and clean.
void clearfile(int length, int width, int level){
    FILE * fptr;
    
    fptr = fopen("maze.txt","w");
    fclose(fptr);
    
    fptr = fopen("config.txt","w");
    fprintf(fptr,"%d %d %d",length,width,level);
    fclose(fptr);
}

//make a loop in the maze by eliminate one valid wall.
void makeloop(char ** maze,int length, int width){
    int x ,y, counter1 = 0;
    while (counter1 != 1){
        x = 1 + rand() % (length-2);
        y = 1 + rand() % (width-2);
        if (maze[x][y] == ' '){
            if (is_valid_dead_end(maze, length, width,x,y) == 1){
                counter1++;
            }
        }
    }
}

void make_start_and_end(char** maze, int start, int end, int width){
    
    // Mark the starting cellnode location
    maze[(start / width)*2+1][(start % width)*2+1] = 'S';
    
    // Mark the ending cellnode location
    maze[(end / width)*2+1][(end % width)*2+1] = 'G';
}

int is_valid_dead_end (char ** maze, int length, int width, int x, int y ){
    int counter = 0, dummy = 0, done = 0;
    int **mask = malloc(4 * sizeof(int*));
    for (int i = 0 ; i < 4 ; i++) mask[i] = malloc(2*sizeof(int));
    
    mask[0][0] = -1;
    mask[0][1] = 0;
    mask[1][0] = 0;
    mask[1][1] = 1;
    mask[2][0] = 1;
    mask[2][1] = 0;
    mask[3][0] = 0;
    mask[3][1] = -1;
    
    if (maze[x+1][y] == '#') counter++;
    if (maze[x-1][y] == '#') counter++;
    if (maze[x][y+1] == '#') counter++;
    if (maze[x][y-1] == '#') counter++;
    if( counter == 3 ){
        while (done != 1){
            dummy = rand()%4 ;
            if ( maze[x+mask[dummy][0]][y+mask[dummy][1]] == '#' && (x+mask[dummy][0] != 0) && (x+mask[dummy][0] != length-1 ) && (y+mask[dummy][1] != 0) && (y+mask[dummy][1] != width-1)  ){
                maze[x+mask[dummy][0]][y+mask[dummy][1]] = ' ';
                free(mask);
                return 1;
            }
        }
    }
    return 0;
}
