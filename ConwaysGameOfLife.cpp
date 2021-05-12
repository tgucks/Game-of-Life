/*-------------------------------*/
// Program Name: Conway's Game of Life using C++ and Command Line
// Devloper's Name: Trey Guckian
// Date Developed: 5/12/2021
// Purpose: A program that implements the Game of Life cellular automata system invented by John Conway
// Notes:
//  -
/*-------------------------------*/

#include <conio.h>    //_khbit() -- keyboard input for pause and quit
#include <windows.h>  //Sleep()

#include <fstream>   //for input files
#include <iostream>  //cout and cin
#include <limits>    //for cin.ignore((numeric_limits<streamsize>::max)(), '\n');
#include <sstream>   //for getline()
#include <vector>    //for vectors

using namespace std;

//Global variables
const int globalNumRows = 50;
const int globalNumColumns = 50;
bool gameOver;

//attempts to open a file with user provided file name
//retries until user gives a valid file name or decides to quit (by entering the character 'q')
bool openFile(ifstream& file) {
    string fileName;

    cout << "Please enter the exact name of a file to read including extension e.g. 'File.txt': ";
    cin >> fileName;
    file.open(fileName);

    //if file.open() operation fails, then retry until the user enters a valid filename or quits
    while (!file.is_open()) {
        perror("\nSorry, we cannot open that file");  //will output: "Sorry, we cannot open that file: {system error message}\n"
        cout << "The file name used was: " << fileName << "\nPlease enter the name of a valid file to read e.g. 'File.txt' (or enter 'q' to quit): ";
        cin >> fileName;
        if (fileName == "q")
            return false;  //causes a return to main() and will end the program
        file.open(fileName);
    }

    return true;  //file.is_open() is true and user did not quit program
}

//ensures users enter the char 'y' or 'n' when asked yes or no questions
void validateYorN(char& c) {
    c = tolower(c);
    //while the user's input is (1) not of char data type or (2) not equal to 'y' or 'n'
    while ((!(cin >> c)) || (!((c == 'y') || (c == 'n')))) {
        cout << "Please enter 'y' or 'n': " << endl;
        cin.clear();                                            //clears last saved value in cin
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');  //ignores unseen characters space which can mess up cin >> c
    }

    cin.clear();                                            //clears last saved value in cin
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');  //ignores unseen characters space which can mess up cin >> c
}

//gets input from user on a percentage of cells to start in the "alive" state
//validates that input is an int between 1 and 100 (inclusive)
int getPercentage() {
    int userInput;
    cout << "What percentage of the cells should be 'alive' to start the game? Enter a number between 1 and 100:" << endl;

    while ((!(cin >> userInput)) || (userInput > 100) || (userInput < 1)) {
        cout << "\nPlease enter a number between 1 and 100:" << endl;
        cin.clear();                                            //clears last saved value in cin
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');  //ignores unseen characters space which can mess up cin >> c
    }
    return userInput;
}

//prints the game of life board to the console
//using either '.' or 'O' depending on if the given cell in the grid is set to true (O) or false (.)
void printBoard(vector<vector<bool>> grid) {
    //ensures the console is blank before drawing the game board
    system("cls");

    cout << endl;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j]) {
                cout << 'O';  //uses 'O' to represent alive cells
            } else {
                cout << '.';  //and '.' to represent dead cells
            }
        }
        cout << endl;
    }
    cout << endl;
}

//counts the number of neighboring cells (compared to a cell with coords grid[row][column]) which are "alive" (or set to true)
//neighboring cells are the 8 cells immediately surrounding a given cell
int countNeighbors(vector<vector<bool>>& grid, int row, int column) {
    int count = 0;
    int nRow, nCol;  //neighboring row and column
    for (int i = -1; i <= 1; i++) {
        nRow = row + i;
        for (int j = -1; j <= 1; j++) {
            nCol = column + j;
            if (nRow < 0 || nCol < 0 || nRow >= grid.size() || nCol >= grid.size() || (nRow == row && nCol == column)) {
                //skip cells which are out of bounds of the grid or equal to the current cell
                continue;
            } else if (grid[nRow][nCol]) {
                count++;
            }
        }
    }
    return count;
}

void updateBoard(vector<vector<bool>>& inputGrid, vector<vector<bool>>& outputGrid) {
    //update the output grid to match the input before any edits
    outputGrid = inputGrid;
    //for each cell in input grid
    for (int i = 0; i < inputGrid.size(); i++) {
        for (int j = 0; j < inputGrid.size(); j++) {
            //counts the number of neighboring cells which are "alive" (or set to true)
            //neighboring cells are the 8 cells immediately surrounding a given cell
            int numNeighbors = countNeighbors(inputGrid, i, j);
            if (inputGrid[i][j]) {
                //if cell is alive and has <2 alive neighbors or has >4 alive neighbors, it dies
                //if cell is alive with 2-3 neighbors, it lives
                outputGrid[i][j] = (numNeighbors < 2 || numNeighbors > 3) ? false : true;
            } else if (numNeighbors == 3) {
                //if cell is dead and has exactly 3 neighbors, it lives
                outputGrid[i][j] = true;
            }
        }
    }
}

void pause() {  //will sleep the game until a key is pressed
    while (!_kbhit())
        Sleep(0.1);
}

void input() {  //to pause the game with 'p' or quit with 'x'
    if (_kbhit()) {
        switch (_getch()) {
            case 'p':
                cout << "\nGame paused. Press any key to continue." << endl;
                pause();
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

void fillRandomly(vector<vector<bool>>& grid) {
    int onOff;                         //switch used to decide whether to make a cell dead or alive
    int percentage = getPercentage();  //prompts user for % of cells to turn on between 1 and 100

    for (int i = 0; i < grid.size(); i++) {  //fill grid using %
        for (int j = 0; j < grid[i].size(); j++) {
            onOff = rand() % 100;                               //generates random integer between 1 and 100
            grid[i][j] = (onOff <= percentage) ? true : false;  //makes cell "alive" (i.e. true boolean value) if it is below the percentage threshold set by the user
        }
    }
}

bool fillWithFile(vector<vector<bool>>& grid) {
    // Create an input filestream
    ifstream myFile;
    //this will either open the file or quit the program
    if (!openFile(myFile)) return false;

    string line;
    bool cellState = false;
    int startRow = globalNumRows / 10;  //along with startColumn, will be used to track insertion position of the input pattern
    int startColumn = globalNumColumns / 10;

    while (getline(myFile, line)) {  //iterate through each line of file
        if (line[0] == '#') {        //print out comments and move to next line in file (next loop iteration)
            cout << line << endl;
        } else {                                              //if not a comment, this is input for our game baord
            for (int i = 0; i < line.size(); i++) {           //go through each cell in the input pattern
                cellState = (line[i] == '.') ? false : true;  //if it is a '.', the cell is dead (= false), else alive (= true)
                grid[startRow][startColumn] = cellState;      //fill our local grid using this input
                startColumn++;
            }
            startRow++;
            startColumn = globalNumColumns / 10;
        }
    }
    myFile.close();
    return true;
}

int main() {
    int percentage, onOff;
    vector<vector<bool>> gridOne(globalNumRows, vector<bool>(globalNumColumns, false));
    vector<vector<bool>> gridTwo(globalNumRows, vector<bool>(globalNumColumns, false));

    //fill gridOne depending on user preference
    cout << "Welcome to Conway's Game of Life!" << endl;
    cout << "Would you like me to randomly generate a game (y) or provide your own pattern file (n)?" << endl;
    char answer;
    validateYorN(answer);  //ensures user inputs 'y' or 'n'
    if (answer == 'y') {
        fillRandomly(gridOne);  //fills gridOne randomly with user input on % of cells to start as alive
    } else {
        if (!fillWithFile(gridOne)) return 0;  //fills gridOne using pattern in input file (or quits program if user cannot input a valid file)
    }

    //the following prints the initial game board and waits for the user to press any key before beginning the Game of Life
    gameOver = false;
    cout << endl;
    printBoard(gridOne);  //prints the initial grid to the console
    cout << endl;
    cout << "Above is the initial game board. Press 'p' at any time to pause and 'x' at any time to quit." << endl;
    cout << "Kindly hit return when you're ready to begin." << endl;
    pause();  //waits for user to press any key
    cout << "Starting the Game of Life!" << endl;
    cout << endl;

    //Continually updates and prints the game board, using gridOne and gridTwo to store each successive generation
    while (!gameOver) {
        input();                        //allows user to pause the game at any time with 'p' and quit with 'x'
        updateBoard(gridOne, gridTwo);  //uses the current state of gridOne to fill in gridTwo with the next generation of cells
        printBoard(gridTwo);            //prints gridTwo to console
        updateBoard(gridTwo, gridOne);  //updates gridOne using current state of gridTwo
        printBoard(gridOne);            //prints gridOne to console
        Sleep(10);                      //waits 10ms then repeats until user quits
    }

    return 0;
}