// Magic Square Puzzle

#include <iostream> // For basic input output
#include <iomanip> // To make the puzzle cleaner in appearance
#include <vector>  // To create the squares
#include <random>  // For generating random blanks
#include <algorithm>  // Includes pair and shuffle
using namespace std;

//Initialisation of functions 

// Function to generate an n x n Magic Square using the Siamese method
void MagicSquare(vector<vector<int>>& square, int n);

// Function to create a puzzle by making blanks
void createPuzzle(vector<vector<int>>& puzzle, const vector<vector<int>>& solution, int blanks);

// Function to display the puzzle grid
void displayPuzzle(const vector<vector<int>>& puzzle);

// Function to take user input and check correctness after each entry
void getUserInput(vector<vector<int>>& userAnswer, const vector<vector<int>>& puzzle, const vector<vector<int>>& solution);

// Function to check if final userAnswer matches the correct solution
bool checkSolution(const vector<vector<int>>& userAnswer, const vector<vector<int>>& solution);

int main() {
    int n = 7, blanks = 15;
    vector<vector<int>> solution, puzzle, userAnswer;

    // Generate full magic square and create puzzle
    MagicSquare(solution, n);
    createPuzzle(puzzle, solution, blanks);

    //Displaying the puzzle
    cout << endl << "Your Puzzle:" << endl;
    displayPuzzle(puzzle);

    // Get user input and validate each entry
    getUserInput(userAnswer, puzzle, solution);

    // Final result check
    if (checkSolution(userAnswer, solution)) {
        cout << endl << "Congratulations! You solved the puzzle correctly!" << endl;
    }
    else {
        cout << endl << "Some answers are incorrect. Try again!" << endl;
    }
    return 0;
}

void MagicSquare(vector<vector<int>>& square, int n) {

    square = vector<vector<int>>(n, vector<int>(n, 0));

    int num = 1;
    int i = 0, j = (n - 1) / 2;

    while (num <= n * n) {
        square[i][j] = num;
        int i_before = i, j_before = j;
        i--; j++;
        if (i < 0) { i = n - 1; }
        if (j == n) { j = 0; }
        if (square[i][j] != 0) {
            i = i_before + 1;
            j = j_before;
            if (i == n) { i = 0; }
        }
        num++;
    }
}

void createPuzzle(vector<vector<int>>& puzzle, const vector<vector<int>>& solution, int blanks) {
    puzzle = solution;
    int n = puzzle.size();
    // for generating random blanks
    random_device rd;
    mt19937 g(rd());

    // Divide blanks across 4 quadrants for balanced distribution
    int quad_blanks = blanks / 4;
    int extra_blanks = blanks % 4;

    int q1 = quad_blanks + (extra_blanks > 0 ? 1 : 0);
    int q2 = quad_blanks + (extra_blanks > 1 ? 1 : 0);
    int q3 = quad_blanks + (extra_blanks > 2 ? 1 : 0);
    int q4 = quad_blanks;

    vector<pair<int, int>> Q1, Q2, Q3, Q4;

    // Divide puzzle into quadrants 
    for (int r = 0; r <= n / 2; r++) {
        for (int c = 0; c <= n / 2; c++) {
            Q1.push_back({ r,c });
        }
    }

    for (int r = (n + 1) / 2; r < n; r++) {
        for (int c = 0; c < n / 2; c++) {
            Q2.push_back({ r,c });
        }
    }

    for (int r = 0; r < n / 2; r++) {
        for (int c = (n + 1) / 2; c < n; c++) {
            Q3.push_back({ r,c });
        }
    }

    for (int r = (n + 1) / 2; r < n; r++) {
        for (int c = (n + 1) / 2; c < n; c++) {
            Q4.push_back({ r,c });
        }
    }

    // Shuffle positions within each quadrant
    shuffle(Q1.begin(), Q1.end(), g);
    shuffle(Q2.begin(), Q2.end(), g);
    shuffle(Q3.begin(), Q3.end(), g);
    shuffle(Q4.begin(), Q4.end(), g);

    // Set blanks (0s)
    for (int k = 0; k < q1; k++) { puzzle[Q1[k].first][Q1[k].second] = 0; }
    for (int k = 0; k < q2; k++) { puzzle[Q2[k].first][Q2[k].second] = 0; }
    for (int k = 0; k < q3; k++) { puzzle[Q3[k].first][Q3[k].second] = 0; }
    for (int k = 0; k < q4; k++) { puzzle[Q4[k].first][Q4[k].second] = 0; }
}

void displayPuzzle(const vector<vector<int>>& puzzle) {
    int n = puzzle.size();
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (puzzle[r][c] == 0) {
                cout << setw(5) << "_";
            }
            else {
                cout << setw(5) << puzzle[r][c];
            }
        }
        cout << endl << endl;
    }
}

void getUserInput(vector<vector<int>>& userAnswer, const vector<vector<int>>& puzzle, const vector<vector<int>>& solution) {
    userAnswer = puzzle;
    int n = puzzle.size();

    int blanks = 0;

    // Count initial blanks
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (puzzle[r][c] == 0) {
                blanks++;
            }
        }
    }

    cout << endl << "There are " << blanks << " blanks to fill." << endl;

    // Loop until all blanks are filled
    while (blanks > 0) {
        int row, col, val;
        cout << endl << "Enter row number ";
        cin >> row;
        cout << "Enter column number ";
        cin >> col;

        row--; col--;


        if (row < 0 || row >= n || col < 0 || col >= n) {
            cout << "Invalid position. Try again." << endl;
            continue;
        }

        // Prevent editing of fixed (non-blank) cells
        if (puzzle[row][col] != 0) {
            cout << "That cell is not blank. Choose another." << endl;
            continue;
        }

        // Allow change if already filled before to correct mistakes
        if (userAnswer[row][col] != 0) {
            char choice;
            cout << "You already entered " << userAnswer[row][col] << " here. Do you want to change it? (y/n): ";
            cin >> choice;
            if (choice != 'y' && choice != 'Y') {
                cout << "Okay, keeping your previous answer." << endl;
                continue;
            }
        }

        cout << "Enter value for (" << row + 1 << "," << col + 1 << "): ";
        cin >> val;

        // Check correctness
        if (val == solution[row][col]) {
            cout << " Correct!" << endl;
            userAnswer[row][col] = val;
        }
        else {
            cout << " Incorrect! Try again." << endl;
            userAnswer[row][col] = val;
        }

        // Recount blanks (simply using blanks-- will not work due to refilling)
        blanks = 0;
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                if (userAnswer[r][c] == 0) {
                    blanks++;
                }
            }
        }

        // Display updated puzzle
        cout << endl << "Updated Puzzle:" << endl;
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                if (userAnswer[r][c] == 0)
                    cout << setw(5) << "_";
                else
                    cout << setw(5) << userAnswer[r][c];
            }
            cout << endl << endl;
        }
    }
}

bool checkSolution(const vector<vector<int>>& userAnswer, const vector<vector<int>>& solution) {
    int n = solution.size();
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (userAnswer[r][c] != solution[r][c])
                return false;
        }
    }
    return true;
}