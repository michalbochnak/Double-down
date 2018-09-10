//	Program #3, DoubleDown.
//
//	Class: CS 141, Fall 2016.
//	System : Windows 10, Microsoft Visual Studio 2015
//	Lab : Tuesday 8am lab,
//	TA : Guixiang Ma
//	Authors : Michal Bochnak, Sean Martinelli

//	Write a program to play the game of DoubleDown, a cousin(that I invented) of
//	Tic - Tac - Toe and Quarto.This is a two - player game unlike Tic - Tac - Toe
//	in that you don't have "your" pieces and "their" pieces, but rather the winner
//	is the one who is first to complete a configuration of 4 pieces of the same type,
//	drawing upon all the pieces remaining to play.  Once placed on the board pieces
//	cannot be moved.
//
//	Stages: 
//	1.	Display the blank board, square numbers and pieces to play, which should
//	look like I have shown above.
//	Board : Your board should be declared as an array of 16 characters that has
//	all  '.' in it to start.Displaying the board simply displays whatever
//	values are stored in each of the board squares.Later as play progresses the '.'
//	characters will be replaced by other letters, one at a time, and displaying
//	the board array will then end up displaying those characters instead.
//	Pieces : Your pieces should likewise be stored in an array.
//	2.	Prompt for user input of the piece to be played and the destination
//	and make the move.Note that the opponent selects the piece,
//	and then the player chooses where it should go.You should list move numbers
//	that increment as valid moves are made.Your program should handle user input
//	with or without a space between the piece and destination.
//	3.	Add error checking and appropriate error messages if a player chooses an
//	invalid piece or an invalid destination as illustrated above.Your program must
//	then redisplay the board and re - prompt for the move.Note that the move number
//	only increases after each valid move.
//	4.	Add the option to set the level of play.Before changing the level of play
//	from 1 to 2 you must first check and make sure that there is not already
//	a winning combination using the 4 corner squares or a group of 4 squares
//	in one of the corners(e.g.move 5 in the example above).When prompting for
//	a piece also allow entering 'x' to exit the program.
//	5.	Check for a win in any of the valid configurations.For level 1 this
//	is any row, any column, or either of the two diagonals.For level 2 you would
//	additionally need to check the 4 corners and the 4 corner groups.Once there
//	is a win your program should give a message and exit.



#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;


// function declaration
void DisplayIdentifyingInformation();
void DisplayGameInstructions();
void PrintABoard(char board[]);
void UpdateABoard(char board[], int pieceChoice, char spot);
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



int main() {
	char board[17] = "................";		// 16 destination spots, initially periods
	char pieces[20] = "AEIO aeio BCDF bcdf";	// 16 pieces to play
	char pieceChoice = ' ';						// piece chosen by player
	int spotNum = -1;							// spot number
	int winner = -1;							// while loop condition	
	int pieceChooser = 2;						// player who choses piece
	int spotChooser = 1;						// player who choses spot
	int moveNum = 1;							// count moves
	int level = 1;								// level of game


	DisplayIdentifyingInformation();
	DisplayGameInstructions();

	while (winner == -1) {
		PrintABoard(board);							// display board
		cout << "	Pieces: " << pieces << endl;	// display pieces

													// prompt users for input
		cout << endl << moveNum << ". Player " << pieceChooser
			<< " enter piece, and player " << spotChooser << " enter destination: ";
		cin >> pieceChoice;						// read in piece selected by player

												// change the game level, (verify if there is no
												// pre-existing winning combination before changing to level two)
		if (pieceChoice == 's') {
			level = ChangeLevel(board);
			cout << endl;		// new line after level change
			continue;			// go to the main while loop
		}

		// exit the program if player enters "x"
		if (pieceChoice == 'x') {
			cout << endl;
			return 0;
		}

		cin >> spotNum;		// read in spot number selected by player

							// valitade the piece choice, if piece or spot is invalid, 
							// display error message and restart main while loop
		if (ValidateUserInput(pieces, pieceChoice, board, spotNum) == false) {
			continue;
		}

		moveNum++;			// increment move number each time after valid move

							// update board after each valid move
		UpdatePieces(pieces, pieceChoice);
		cout << endl;		// for output formatting
							// update board by chosen piece at chosen spot
		UpdateABoard(board, pieceChoice, spotNum);

		// after each move verify if there is a winning combination on the board
		// and print out the message who won in case of winning
		if (TestForWin(board, level)) {
			PrintABoard(board);
			cout << endl << "*** Congratulations! ***  Player " << spotChooser << " wins the game!\n" << endl;
			cout << "Done." << endl << endl;
			return 0;
		}

		// switch roles for players
		SwapPlayers(pieceChooser, spotChooser);
	}

	return 0;
}	// end of main


	//-----------------------------------------------------------------------------
	// -This function displays the student identyfying information
void DisplayIdentifyingInformation()
{
	printf("Program #4, DoubleDown\n");
	printf("Author: Michal Bochnak, Sean Martinelli\n");
	printf("TA: Guixiang Ma\n");
	printf("Lab: Tuesday 8am\n");
	printf("Oct 27, 2016\n\n");
}


//-----------------------------------------------------------------------------
// -This function displays the game instructions on the screen
void DisplayGameInstructions() {
	cout << "Welcome to the game of DoubleDown, where you try to complete a set" << endl
		<< "of four pieces that are alike.Players take turns making moves." << endl
		<< "On each move your OPPONENT chooses the piece, then YOU get to" << endl
		<< "place it on the board.If you create a set of four alike when" << endl
		<< "you place your piece, then you win!A set of four alike can be" << endl
		<< "completed by using four upper case characters, four lower - case" << endl
		<< "characters, four vowels(a, e, i, o, u) or four consonants." << endl
		<< endl
		<< "The game level determines which positions can be used in completing" << endl
		<< "four alike pieces.Level one checks for four alike in every row, " << endl
		<< "column, and diagonal.Level two additionally checks for the pieces" << endl
		<< "in the four corners as well as each cluster of four in each corner." << endl
		<< endl
		<< "When prompted for piece to use you may also : " << endl
		<< "Enter 's' to change program level." << endl
		<< "Enter 'x' to exit the program." << endl << endl;
}


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
void UpdateABoard(char board[], int pieceChoice, char spot) {
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