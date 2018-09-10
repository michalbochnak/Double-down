#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QPixmap>
using namespace std;


// global variables
QString prompt = "Player 2 choose a piece, player 1 choose a spot on a board.";
QString moveNumber;





void PrintABoard(char board[]);
void UpdateABoard(char board[], char pieceChoice, int spot);
void SwapPlayers(int &pieceChooser, int &spotChooser);
void UpdatePieces(char pieces[], char pieceChoice);
bool ValidatePieceChoice(char pieces[], char pieceChoice);
bool ValidateSpotChoice(char board[], int spotChoice);
bool ValidateUserInput(char pieces[], char pieceChoice, char board[], int spotNum);
int	ChangeLevel(char board[]);
bool CheckForSubset(char subsetToCheck[], char mainSet[]);
bool TestForWinningCombination(char checkString[]);
bool CheckForVerticalWin(char board[]);
bool CheckForHorizontalWin(char board[]);
bool CheckForDiagonalWin(char board[]);
bool CheckForCornerWin(char board[]);
bool CheckForFourInACornerWin(char board[]);
bool TestForWin(char board[], int level);


QString pieceChoice = ' ';                      // piece chosen by player QString
char pieceChoiceChar = 'X';                     // piece choice by player for win check
    char board[17] = "................";        // 16 destination spots, initially periods
    char pieces[20] = "AEIO aeio BCDF bcdf";	// 16 pieces to play
    int spotNum = -1;							// spot number
    int player1 = 1;							// player one identifier
    int	player2 = 2;							// player two identifier
    int winner = -1;							// while loop condition
    int pieceChooser = 2;						// player who choses piece
    int spotChooser = 1;						// player who choses spot
    int moveNum = 1;							// count moves
    int level = 1;								// level of game


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("DoubleDown");
    ui->pushButton_exit->setVisible(false);


}


MainWindow::~MainWindow()
{
    delete ui;
}





// my functions definitions
    //-----------------------------------------------------------------------------
    // -This function displays the baord after each move
    // -input: #1.) The game board array with actual pieces in it
    void PrintABoard(char board[]) {
        printf("|---------|\n");
        printf("| %c %c %c %c |  1  2  3  4\n", board[0], board[1], board[2], board[3]);
        printf("| %c %c %c %c |  5  6  7  8\n", board[4], board[5], board[6], board[7]);
        printf("| %c %c %c %c |  9 10 11 12\n", board[8], board[9], board[10], board[11]);
        printf("| %c %c %c %c | 13 14 15 16\n", board[12], board[13], board[14], board[15]);
        printf("|---------|\n");
    }


    //-----------------------------------------------------------------------------
    // -This function updates the game board after each selection of piece and spot
    // -Input: #1.) Game board array #2.) Piece choosen by player #3.) Spot chosen
    //  by another player
    void UpdateABoard(char board[], char pieceChoice, int spot) {
        board[spot - 1] = pieceChoice;
    }


    //-----------------------------------------------------------------------------
    // -This function swaps the roles for players
    // -Input: #1.) Identifier for pieceChooser (player1 or player2) #2.) Identifier
    //  for the spotChooser (player1 or player2)
    void SwapPlayers(int &pieceChooser, int &spotChooser) {
        int temp;
        temp = pieceChooser;
        pieceChooser = spotChooser;
        spotChooser = temp;
    }


    //-----------------------------------------------------------------------------
    // -This function updades the pieces set by overwriting selected piece spot by space
    // -Input: #1.) Char array with pieces #2.) Char variable with actual piece chosen by player
    void UpdatePieces(char pieces[], char pieceChoice) {
        char *index;
        index = strchr(pieces, pieceChoice);
        pieces[index - pieces] = ' ';
    }


    //-----------------------------------------------------------------------------
    // -This function verifies if the piece chosen by player is still available in
    //  the set of pieces
    // -Input: #1.) Char array with pieces #2.) Char variable with piece chosen by player
    // -Output(bool): true = piece is still available, false = piece is not available
    //  (displays a message about invalid piece selection)
    bool ValidatePieceChoice(char pieces[], char pieceChoice) {
        if (strchr(pieces, pieceChoice) == NULL) {
            cout << "*** " << pieceChoice << " is an invalid piece selection." << endl;
            return false;	// piece already taken
        }
        else {
            return true;	// piece is available
        }
    }


    //-----------------------------------------------------------------------------
    // -This function verifies if the spot chosen by a player is not taken
    // -Input: #1.) Char array with board, #2.) Int which indicates the spot chosen by player
    // -Output(bool): true = chosen spot is free, false = chosen spot is already taken
    bool ValidateSpotChoice(char board[], int spotChoice) {

        // stores input validity: true = valid, false = invalid
        bool inputValidity = true;

        // verify if the chosen spot number is in th range of board,
        // display error messsage otherwise and return false
        if (spotChoice < 1 || spotChoice > 16) {
            cout << "*** Invalid Destination" << endl;
            inputValidity = false;		// chosen spot number outside of board range
        }

        // verify if board is empty at chosen spot,
        // display error message otherwise
        if ((inputValidity == true) && (board[spotChoice - 1] != '.')) {
            cout << "*** Board square " << spotChoice << " is not empty" << endl;
            inputValidity = false;		// chosen spot number already taken
        }

        // return value of inputValidity, true = valid, false = invalid
        return inputValidity;
    }


    //-----------------------------------------------------------------------------
    // -This function validates the user input including the chosen piece and chosen
    //  spot by calling the two functions: ValidatePieceChoice, ValidateSpotChoice
    // -Input: #1.) Char array with pieces, #2.) Char variable with actual chosen piece,
    //  #3.) Char array with board, #4.) Int which indicates the spot chosen by player
    // -Output(bool): true = user input is correct, false = at least one the pieceChoice
    //  and spotNum are incorrent
    bool ValidateUserInput(char pieces[], char pieceChoice, char board[], int spotNum) {

        bool pieceValidity = false;		// stores selected piece validity
        bool spotValidity = false;		// stores selected spot validity

        // verify validity of piece
        pieceValidity = ValidatePieceChoice(pieces, pieceChoice);
        // verify validity of spot
        spotValidity = ValidateSpotChoice(board, spotNum);

        cout << endl;			// add extra line after error messages

        if (pieceValidity && spotValidity) {
            return true;		// user input is valid
        }
        else {
            return false;		// user input is invalid
        }
    }

    //--------------------------------------------------------------------------------------
    // -This function checks if a string is a subset of another string.
    // -Input: #1.) The string that will be checked #2.) The sting that will be used
    //  to check if input #1 is a subset.
    // -Output(bool): true = subset was found, false = subset was not found

    bool CheckForSubset(char subsetToCheck[], char mainSet[]) {

        //loop through each element of subsetToCheck
        for (int i = 0; i< strlen(subsetToCheck); i++) {

            //return false if a char is found that is not in mainSet
            if (strchr(mainSet, subsetToCheck[i]) == NULL) {
                return false;
            }
        }

        return true;    //subsetToCheck is a subset of mainSet
    }


    //--------------------------------------------------------------------------------------
    // -This function checks if the string passed to it is a winning combination of the game
    // -Input: #1.) The string to be checked
    // -Output(bool): true = winning combination found, false = winning combination not found

    bool TestForWinningCombination(char checkString[]) {

        char upperCase[] = "AEIOBCDF";  //set of all uppercase pieces
        char lowerCase[] = "aeiobcdf";  //set of all lowercase pieces
        char vowels[] = "AEIOaeio";     //set of all vowels
        char consonants[] = "BCDFbcdf"; //set of all consonants

        //check for subset of upperCase
        if (CheckForSubset(checkString, upperCase)) {
            return true;
        }

        //check for subset of lowerCase
        if (CheckForSubset(checkString, lowerCase)) {
            return true;
        }

        //check for subset of vowels
        if (CheckForSubset(checkString, vowels)) {
            return true;
        }

        //check for subset of consonants
        if (CheckForSubset(checkString, consonants)) {
            return true;
        }

        return false;   //Subset was not found

    }

    //--------------------------------------------------------------------------------------
    // -This function checks all columns of the board for a vertical win.
    // -Input: #1.) The game board
    // -Output(bool): true = win found, false = win not found

    bool CheckForVerticalWin(char board[]) {

        char piecesToTest[5];       //Array to store the pieces to test for win
        int piecesToTestIndex = 0;  //Index for PiecesToTest

        //step through each column of the board
        for (int i = 0; i < 4; i++) {

            //step through each row at the current column
            for (int j = 0; j < 16; j += 4) {

                //put current piece in piecesToTest array and increase index
                piecesToTest[piecesToTestIndex] = board[i + j];
                piecesToTestIndex++;
            }

            piecesToTest[4] = '\0'; //add a null char to the end of array
            piecesToTestIndex = 0;  //reset index

            //if piecesToTest is a winning combination, return true
            if (TestForWinningCombination(piecesToTest)) {
                return true;
            }
        }

        return false;   //A win was not found
    }

    //--------------------------------------------------------------------------------------
    // -This function checks all rows of the board for a horizontal win.
    // -Input: #1.) The game board
    // -Output(bool): true = win found, false = win not found

    bool CheckForHorizontalWin(char board[]) {

        char piecesToTest[5];       //Array to store the pieces to test for win
        int piecesToTestIndex = 0;  //Index for PiecesToTest

                                    //step through each row of the board
        for (int i = 0; i < 16; i += 4) {

            //step through each column at the current row
            for (int j = i; j < i + 4; j++) {

                //put current piece in piecesToTest array and increase index
                piecesToTest[piecesToTestIndex] = board[j];
                piecesToTestIndex++;
            }

            piecesToTest[4] = '\0'; //add a null char to the end of array
            piecesToTestIndex = 0;  //reset index

                                    //if piecesToTest is a winning combination, return true
            if (TestForWinningCombination(piecesToTest)) {
                return true;
            }
        }

        return false;   //A win was not found
    }

    //--------------------------------------------------------------------------------------
    // -This function checks the board for a diagonal win.
    // -Input: #1.) The game board
    // -Output(bool): true = win found, false = win not found

    bool CheckForDiagonalWin(char board[]) {

        char forwardDiagonal[5];    //Array to store the forward diagonal pieces to test for win
        char backwardDiagonal[5];   //Array to store the backward diagonal pieces to test for win
        int diagonalIndex = 0;      //Index used for forwardDiagonal and backwardDiagonal

                                    //step through each backward diagonal board space
        for (int i = 0; i < 16; i += 5) {

            //put current piece in backwardDiagonal array and increase index
            backwardDiagonal[diagonalIndex] = board[i];
            diagonalIndex++;
        }

        diagonalIndex = 0;  //Reset diaginalIndex

        //step through each forward diagonal board space
        for (int i = 3; i < 16; i += 3) {

            //put current piece in forwardDiagonal array and increase index
            forwardDiagonal[diagonalIndex] = board[i];
            diagonalIndex++;
        }

        //Add null char to the end of each diagonal string
        backwardDiagonal[4] = '\0';
        forwardDiagonal[4] = '\0';

        //if backwardDiagonal or forwardDiagonal is a winning combination, return true
        if (TestForWinningCombination(backwardDiagonal) || TestForWinningCombination(forwardDiagonal)) {
            return true;
        }

        return false;   //A win was not found
    }

    //--------------------------------------------------------------------------------------
    // -This function checks all corner spots of the board for a corner win.
    // -Input: #1.) The game board
    // -Output(bool): true = win found, false = win not found

    bool CheckForCornerWin(char board[]) {

        char piecesToTest[5];       //Array to store the pieces to test for win
        int piecesToTestIndex = 0;  //Index for piecesToTest

                                    //step through rows 1 and 4
        for (int i = 0; i < 16; i += 12) {

            //step through columns 1 and 4 at current row
            for (int j = i; j < i + 4; j += 3) {

                //put current piece in piecesToTest array and increase index
                piecesToTest[piecesToTestIndex] = board[j];
                piecesToTestIndex++;
            }
        }

        piecesToTest[4] = '\0'; //add a null char to the end of array

        //if piecesToTest is a winning combination, return true
        if (TestForWinningCombination(piecesToTest)) {
            return true;
        }

        return false;   //A win was not found

    }

    //--------------------------------------------------------------------------------------
    // -This function checks for a four in a corner win.
    // -Input: #1.) The game board
    // -Output(bool): true = win found, false = win not found

    bool CheckForFourInACornerWin(char board[]) {

        char piecesToTest[5];       //Array to store the pieces to test for win
        int piecesToTestIndex = 0;  //Index for piecesToTest

        //step through rows 1 and 3
        for (int i = 0; i<16; i += 8) {

            //step through columns 1 and 3 at current row
            for (int j = i; j<(i + 3); j += 2) {

                //add all four pieces at current corner to piecesToTest array
                for (int k = j; k<(j + 2); k++) {

                    //add piece at index k to piecesToTest array and increase index
                    piecesToTest[piecesToTestIndex] = board[k];
                    piecesToTestIndex++;

                    //add piece directly below index k to piecesToTest array and increase index
                    piecesToTest[piecesToTestIndex] = board[k + 4];
                    piecesToTestIndex++;

                }

                piecesToTest[4] = '\0'; //add a null char to the end of array
                piecesToTestIndex = 0;  //reset index

                                        //if piecesToTest is a winning combination, return true
                if (TestForWinningCombination(piecesToTest)) {
                    return true;
                }
            }
        }
        return false;   //A win was not found
    }

    //--------------------------------------------------------------------------------------
    // -This function checks the game board for all possible wins at the given level.
    // -Input: #1.) The game board #2.) game level to be used (1 = level 1 (default), 2 = level 2)
    // -Output(bool): true = win found, false = win not found

    bool TestForWin(char board[], int level) {

        bool winStatus = false; //stores current win status

        //check for a vertical win
        winStatus = CheckForVerticalWin(board);

        //check for a horizontal win if a win has not been found
        if (winStatus == false) {
            winStatus = CheckForHorizontalWin(board);
        }

        //check for a diagonal win if a win has not been found
        if (winStatus == false) {
            ;
            winStatus = CheckForDiagonalWin(board);
        }

        //check for level 2 wins if enabled
        if (level == 2) {

            //check for a corner win if a win has not been found
            if (winStatus == false) {
                winStatus = CheckForCornerWin(board);
            }

            //check for a four in a corner win if a win has not been found
            if (winStatus == false) {
                winStatus = CheckForFourInACornerWin(board);
            }
        }

        return winStatus;   //return the win status
    }

    //-----------------------------------------------------------------------------
    // -This function changes level of game
    // -Input: Char array with board
    // -Output(int): 1 = level one, 2 = level two.
    int ChangeLevel(char board[]) {
        int level = 0;		// stores the level of game

        // promt user to enter the prefered game level
        cout << endl << "What level of play would you like? 1: Normal, 2: Add corners & groups --> ";
        cin >> level;		// read in level chosen by user

        if (level == 1) {
            return 1;		// return level 1
        }
        else if (level == 2) {
            // if board already contains the winning combination for level 2,
            // display the message that level cannot be changed and reset level to 1
            if (TestForWin(board, level)) {
                cout << "Sorry, changing level would lead to a pre-existing win." << endl
                    << "Resetting the level to level 1." << endl;
                return 1;	// return level 1
            }
            else {
                return 2;	// return level 2
            }
        }
        else {
            cout << "Sorry, level should only be 1 or 2."
                << endl << "Resetting the level to level 1." << endl;
            return 1;		// return level 1
        }
    }

    // -This function is switching the prompt message for moves
    void PromptForMove(QString &prompt) {
        if (prompt == "Player 2 choose a piece, player 1 choose a spot on a board." ){
            prompt = "Player 1 choose a piece, player 2 choose a spot on a board.";
        }
       else {
            prompt = "Player 2 choose a piece, player 1 choose a spot on a board.";
        }
    }






 // function switches move number for displaying purposes
 void MoveNum(int moveNum) {
    if (moveNum == 1) {
        moveNumber = "1";
    }
    if (moveNum == 2) {
        moveNumber = "2";
    }
    if (moveNum == 3) {
        moveNumber = "3";
    }
    if (moveNum == 4) {
        moveNumber = "4";
    }
    if (moveNum == 5) {
        moveNumber = "5";
    }
    if (moveNum == 6) {
        moveNumber = "6";
    }
    if (moveNum == 7) {
        moveNumber = "7";
    }
    if (moveNum == 8) {
        moveNumber = "8";
    }
    if (moveNum == 9) {
        moveNumber = "9";
    }
    if (moveNum == 10) {
        moveNumber = "10";
    }
    if (moveNum == 11) {
        moveNumber = "11";
    }
    if (moveNum == 12) {
        moveNumber = "12";
    }
    if (moveNum == 13) {
        moveNumber = "13";
    }
    if (moveNum == 14) {
        moveNumber = "14";
    }
    if (moveNum == 15) {
        moveNumber = "15";
    }
    if (moveNum == 16) {
        moveNumber = "16";
    }
 }





    void MainWindow::on_pushButton_A_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_A->setVisible(false);
          pieceChoice = "A";
          pieceChoiceChar = 'A';
        }
    }

    void MainWindow::on_pushButton_E_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_E->setVisible(false);
          pieceChoice = 'E';
          pieceChoiceChar = 'E';
        }
    }

    void MainWindow::on_pushButton_I_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_I->setVisible(false);
          pieceChoice = 'I';
          pieceChoiceChar = 'I';
        }
    }

    void MainWindow::on_pushButton_O_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_O->setVisible(false);
          pieceChoice = 'O';
          pieceChoiceChar = 'O';
        }

    }

    void MainWindow::on_pushButton_a_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_a->setVisible(false);
          pieceChoice = 'a';
          pieceChoiceChar = 'a';
        }
    }

    void MainWindow::on_pushButton_e_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_e->setVisible(false);
          pieceChoice = 'e';
          pieceChoiceChar = 'e';
        }
    }

    void MainWindow::on_pushButton_i_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_i->setVisible(false);
          pieceChoice = 'i';
          pieceChoiceChar = 'i';
        }
    }

    void MainWindow::on_pushButton_o_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_o->setVisible(false);
          pieceChoice = 'o';
          pieceChoiceChar = 'o';
        }
    }

    void MainWindow::on_pushButton_B_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_B->setVisible(false);
          pieceChoice = 'B';
          pieceChoiceChar = 'B';
        }
    }

    void MainWindow::on_pushButton_C_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_C->setVisible(false);
          pieceChoice = 'C';
          pieceChoiceChar = 'C';
        }
    }

    void MainWindow::on_pushButton_D_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_D->setVisible(false);
          pieceChoice = 'D';
          pieceChoiceChar = 'D';
        }
    }

    void MainWindow::on_pushButton_F_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_F->setVisible(false);
          pieceChoice = 'F';
          pieceChoiceChar = 'F';
        }
    }


    void MainWindow::on_pushButton_b_clicked()
    {        
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_b->setVisible(false);
          pieceChoice = 'b';
          pieceChoiceChar = 'b';
        }
    }

    void MainWindow::on_pushButton_c_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_c->setVisible(false);
          pieceChoice = 'c';
          pieceChoiceChar = 'c';
        }
    }

    void MainWindow::on_pushButton_d_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_d->setVisible(false);
          pieceChoice = 'd';
          pieceChoiceChar = 'd';
        }
    }

    void MainWindow::on_pushButton_f_clicked()
    {
        if ((strchr(pieces, pieceChoiceChar) == NULL) && (!TestForWin(board, level))) {
          ui-> pushButton_f->setVisible(false);
          pieceChoice = 'f';
          pieceChoiceChar = 'f';
        }
    }


    void MainWindow::on_pushButton_exit_clicked()
    {
        exit(-1);
    }



    void MainWindow::on_pushButton_1_clicked()
    {
        if (board[0] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
             UpdatePieces(pieces, pieceChoiceChar);
             ui->pushButton_1->setText(pieceChoice);
             UpdateABoard(board, pieceChoiceChar, 1);
             if (TestForWin(board, level)){
                 if (moveNum % 2 == 1) {
                   ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
                 }
                 else {
                   ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
                 }
                   QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
                   ui->label_trophy->setPixmap(trophy.scaled(100, 100));
                   ui->pushButton_exit->setVisible(true);

             }

             PromptForMove(prompt);
             ui->label_move_info->setText(prompt);
             MoveNum(moveNum);
             ui->label_move_num->setText(moveNumber);
             moveNum++;
        }
    }

    void MainWindow::on_pushButton_2_clicked()
    {
        if (board[1] == '.' && (strchr(pieces, pieceChoiceChar))) {
             UpdatePieces(pieces, pieceChoiceChar);
             ui->pushButton_2->setText(pieceChoice);
             UpdateABoard(board, pieceChoiceChar, 2);
             if (TestForWin(board, level)){
                 if (moveNum % 2 == 1) {
                   ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
                 }
                 else {
                   ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
                 }
                 QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
                 ui->label_trophy->setPixmap(trophy.scaled(100, 100));
                 ui->pushButton_exit->setVisible(true);
             }

             PromptForMove(prompt);
             ui->label_move_info->setText(prompt);
             MoveNum(moveNum);
             ui->label_move_num->setText(moveNumber);
             moveNum++;
        }
    }

    void MainWindow::on_pushButton_3_clicked()
    {
        if (board[2] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
             UpdatePieces(pieces, pieceChoiceChar);
             ui->pushButton_3->setText(pieceChoice);
             UpdateABoard(board, pieceChoiceChar, 3);
             if (TestForWin(board, level)){
                 if (moveNum % 2 == 1) {
                   ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
                 }
                 else {
                   ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
                 }
                 QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
                 ui->label_trophy->setPixmap(trophy.scaled(100, 100));
                 ui->pushButton_exit->setVisible(true);
             }

             PromptForMove(prompt);
             ui->label_move_info->setText(prompt);
             MoveNum(moveNum);
             ui->label_move_num->setText(moveNumber);
             moveNum++;
        }
    }

    void MainWindow::on_pushButton_4_clicked()
    {
        if (board[3] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
             UpdatePieces(pieces, pieceChoiceChar);
             ui->pushButton_4->setText(pieceChoice);
             UpdateABoard(board, pieceChoiceChar, 4);
             if (TestForWin(board, level)){
                 if (moveNum % 2 == 1) {
                   ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
                 }
                 else {
                   ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
                 }
                 QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
                 ui->label_trophy->setPixmap(trophy.scaled(100, 100));
                 ui->pushButton_exit->setVisible(true);
             }

             PromptForMove(prompt);
             ui->label_move_info->setText(prompt);
             MoveNum(moveNum);
             ui->label_move_num->setText(moveNumber);
             moveNum++;
        }
    }

    void MainWindow::on_pushButton_5_clicked()
    {
        if (board[4] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
            UpdatePieces(pieces, pieceChoiceChar);
            ui->pushButton_5->setText(pieceChoice);
            UpdateABoard(board, pieceChoiceChar, 5);
            if (TestForWin(board, level)){
                if (moveNum % 2 == 1) {
                  ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
                }
                else {
                  ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
                }
                QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
                ui->label_trophy->setPixmap(trophy.scaled(100, 100));
                ui->pushButton_exit->setVisible(true);
            }

            PromptForMove(prompt);
            ui->label_move_info->setText(prompt);
            MoveNum(moveNum);
            ui->label_move_num->setText(moveNumber);
            moveNum++;
        }
    }

void MainWindow::on_pushButton_6_clicked()
{
    if (board[5] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
        UpdatePieces(pieces, pieceChoiceChar);
        ui->pushButton_6->setText(pieceChoice);
        UpdateABoard(board, pieceChoiceChar, 6);
        if (TestForWin(board, level)){
            if (moveNum % 2 == 1) {
              ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
            }
            else {
              ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
            }
            QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
            ui->label_trophy->setPixmap(trophy.scaled(100, 100));
            ui->pushButton_exit->setVisible(true);
        }
        PromptForMove(prompt);
        ui->label_move_info->setText(prompt);
        MoveNum(moveNum);
        ui->label_move_num->setText(moveNumber);
        moveNum++;
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    if (board[6] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
        UpdatePieces(pieces, pieceChoiceChar);
        ui->pushButton_7->setText(pieceChoice);
        UpdateABoard(board, pieceChoiceChar, 7);
        if (TestForWin(board, level)){
            if (moveNum % 2 == 1) {
              ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
            }
            else {
              ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
            }
            QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
            ui->label_trophy->setPixmap(trophy.scaled(100, 100));
            ui->pushButton_exit->setVisible(true);
        }

        PromptForMove(prompt);
        ui->label_move_info->setText(prompt);
        MoveNum(moveNum);
        ui->label_move_num->setText(moveNumber);
        moveNum++;
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    if (board[7] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
        UpdatePieces(pieces, pieceChoiceChar);
        ui->pushButton_8->setText(pieceChoice);
        UpdateABoard(board, pieceChoiceChar, 8);
        if (TestForWin(board, level)){
            if (moveNum % 2 == 1) {
              ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
            }
            else {
              ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
            }
            QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
            ui->label_trophy->setPixmap(trophy.scaled(100, 100));
            ui->pushButton_exit->setVisible(true);
        }

        PromptForMove(prompt);
        ui->label_move_info->setText(prompt);
        MoveNum(moveNum);
        ui->label_move_num->setText(moveNumber);
        moveNum++;
    }
}

void MainWindow::on_pushButton_9_clicked()
{
    if (board[8] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
        UpdatePieces(pieces, pieceChoiceChar);
        ui->pushButton_9->setText(pieceChoice);
        UpdateABoard(board, pieceChoiceChar, 9);
        if (TestForWin(board, level)){
            if (moveNum % 2 == 1) {
              ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
            }
            else {
              ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
            }
            QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
            ui->label_trophy->setPixmap(trophy.scaled(100, 100));
            ui->pushButton_exit->setVisible(true);
        }

        PromptForMove(prompt);
        ui->label_move_info->setText(prompt);
        MoveNum(moveNum);
        ui->label_move_num->setText(moveNumber);
        moveNum++;
    }
}

void MainWindow::on_pushButton_10_clicked()
{
    if (board[9] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
        UpdatePieces(pieces, pieceChoiceChar);
        ui->pushButton_10->setText(pieceChoice);
        UpdateABoard(board, pieceChoiceChar, 10);
        if (TestForWin(board, level)){
            if (moveNum % 2 == 1) {
              ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
            }
            else {
              ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
            }
            QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
            ui->label_trophy->setPixmap(trophy.scaled(100, 100));
            ui->pushButton_exit->setVisible(true);
        }

        PromptForMove(prompt);
        ui->label_move_info->setText(prompt);
        MoveNum(moveNum);
        ui->label_move_num->setText(moveNumber);
        moveNum++;
    }
}

void MainWindow::on_pushButton_11_clicked()
{
    if (board[10] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
        UpdatePieces(pieces, pieceChoiceChar);
        ui->pushButton_11->setText(pieceChoice);
        UpdateABoard(board, pieceChoiceChar, 11);
        if (TestForWin(board, level)){
            if (moveNum % 2 == 1) {
              ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
            }
            else {
              ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
            }
            QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
            ui->label_trophy->setPixmap(trophy.scaled(100, 100));
            ui->pushButton_exit->setVisible(true);
        }

        PromptForMove(prompt);
        ui->label_move_info->setText(prompt);
        MoveNum(moveNum);
        ui->label_move_num->setText(moveNumber);
        moveNum++;
    }
}

void MainWindow::on_pushButton_12_clicked()
{
    if (board[11] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
        UpdatePieces(pieces, pieceChoiceChar);
        ui->pushButton_12->setText(pieceChoice);
        UpdateABoard(board, pieceChoiceChar, 12);
        if (TestForWin(board, level)){
            if (moveNum % 2 == 1) {
              ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
            }
            else {
              ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
            }
            QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
            ui->label_trophy->setPixmap(trophy.scaled(100, 100));
            ui->pushButton_exit->setVisible(true);
        }

        PromptForMove(prompt);
        ui->label_move_info->setText(prompt);
        MoveNum(moveNum);
        ui->label_move_num->setText(moveNumber);
        moveNum++;
    }
}

void MainWindow::on_pushButton_13_clicked()
{
    if (board[12] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
        UpdatePieces(pieces, pieceChoiceChar);
        ui->pushButton_13->setText(pieceChoice);
        UpdateABoard(board, pieceChoiceChar, 13);
        if (TestForWin(board, level)){
            if (moveNum % 2 == 1) {
              ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
            }
            else {
              ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
            }
            QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
            ui->label_trophy->setPixmap(trophy.scaled(100, 100));
            ui->pushButton_exit->setVisible(true);
        }

        PromptForMove(prompt);
        ui->label_move_info->setText(prompt);
        MoveNum(moveNum);
        ui->label_move_num->setText(moveNumber);
        moveNum++;
    }
}

void MainWindow::on_pushButton_14_clicked()
{
    if (board[13] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
        UpdatePieces(pieces, pieceChoiceChar);
        ui->pushButton_14->setText(pieceChoice);
        UpdateABoard(board, pieceChoiceChar, 14);
        if (TestForWin(board, level)){
            if (moveNum % 2 == 1) {
              ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
            }
            else {
              ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
            }
            QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
            ui->label_trophy->setPixmap(trophy.scaled(100, 100));
            ui->pushButton_exit->setVisible(true);
        }

        PromptForMove(prompt);
        ui->label_move_info->setText(prompt);
        MoveNum(moveNum);
        ui->label_move_num->setText(moveNumber);
        moveNum++;
    }
}

void MainWindow::on_pushButton_15_clicked()
{
    if (board[14] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
        UpdatePieces(pieces, pieceChoiceChar);
        ui->pushButton_15->setText(pieceChoice);
        UpdateABoard(board, pieceChoiceChar, 15);
        if (TestForWin(board, level)){
            if (moveNum % 2 == 1) {
              ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
            }
            else {
              ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
            }
            QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
            ui->label_trophy->setPixmap(trophy.scaled(100, 100));
            ui->pushButton_exit->setVisible(true);
        }

        PromptForMove(prompt);
        ui->label_move_info->setText(prompt);
        MoveNum(moveNum);
        ui->label_move_num->setText(moveNumber);
        moveNum++;
    }
}

void MainWindow::on_pushButton_16_clicked()
{
    if (board[15] == '.' && ((strchr(pieces, pieceChoiceChar))) != NULL) {
        UpdatePieces(pieces, pieceChoiceChar);
        ui->pushButton_16->setText(pieceChoice);
        UpdateABoard(board, pieceChoiceChar, 16);
        if (TestForWin(board, level)){
            if (moveNum % 2 == 1) {
              ui->label_win_info->setText("PLAYER 1 IS A WINNER!");
            }
            else {
              ui->label_win_info->setText("PLAYER 2 IS A WINNER!");
            }
            QPixmap trophy("C:/Qt/Tools/QtCreator/bin/DoubleDown/DoubleDownGame/images/champ.png");
            ui->label_trophy->setPixmap(trophy.scaled(100, 100));
            ui->pushButton_exit->setVisible(true);
        }

        PromptForMove(prompt);
        ui->label_move_info->setText(prompt);
        MoveNum(moveNum);
        ui->label_move_num->setText(moveNumber);
        moveNum++;
    }
}


void MainWindow::on_radioButton_1_clicked()
{
    level = 1;
}

void MainWindow::on_radioButton_2_clicked()
{
    if (TestForWin(board, 2)) {
        ui->label_level_change_err->setText("Pre-existing win detected, level cannot be changed now.");
        ui->radioButton_1->setChecked(true);
    }
    else level = 2;
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox::information(
        this,
        tr("Instructions"),
        tr("About: \n"
           "DoubleDown\n"
           "Authors: Michal Bochnak, Sean Martinelli\n"
           "Professor: Dale Reed\n"
           "UIC, CS-141, Fall '16\n\n"


           "Instructions:\n"
           "Welcome to the game of DoubleDown, where you try to complete a set "
           "of four pieces that are alike. Players take turns making moves. "
           "On each move your OPPONENT chooses the piece, then YOU get to "
           "place it on the board. If you create a set of four alike when "
           "you place your piece, then you win! A set of four alike can be "
           "completed by using four upper case characters, four lower-case "
           "characters, four vowels (a,e,i,o,u) or four consonants."
           "\n"
           "The game level determines which positions can be used in completing "
           "four alike pieces. Level one checks for four alike in every row, "
           "column, and diagonal. Level two additionally checks for the pieces "
           "in the four corners as well as each cluster of four in each corner.") );
}
