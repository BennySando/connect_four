#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cctype>

using namespace std;


//The board that the game is being played in and functions related to it
class Board{
    private:

        //Stores whether or not game has been won
        bool GameWon;

        //The winner of the last game
        int LastWinner;

        //The last winner's win streak
        int WinStreak;

        //Empties the current board
        void CleanBoard(){
            for (int i = 0; i < 6; i++){
                for (int j = 0; j < 7; j++){
                    BoardArray[i][j] = '-';
                }
            }
        }
    
    public:

        //The 2D array that stores the status of the game board
        char **BoardArray;

        //Set up an empty board
        Board(){
            BoardArray = new char *[6];
            for (int i = 0; i < 6; i++){
                BoardArray[i] = new char[7];
                for (int j = 0; j < 7; j++){
                    BoardArray[i][j] = '-';
                }
            }
            GameWon = false;
            LastWinner = 0;
            WinStreak = 0;
        }

        //Prints out the current state of the board
        void PrintBoard(){
            cout << "   1   2   3   4   5   6   7  \n";
            for(int i = 0; i < 6; i++){
                cout << " | ";
                for (int j = 0; j < 7; j++){
                    cout << BoardArray[i][j] << " | ";
                }
                cout << '\n';
            }
        }

         /*A helper function to check horizontal directions genercially.
         Returns number of same-colored pieces connected to given space
         horizontally*/
        int CheckHorizontal (int Row, int Col, char Color){
                int NumSame = 0;

                //Check to the left
                for (int i = 1; i <= 3; i++){
                    if (Col - i < 0 || Color != BoardArray[Row][Col - i]){
                        break;
                    }
                    NumSame++;
                }

                //Check to the right if necessary
                if (NumSame < 3){
                    for (int i = 1; i <= 3; i++){
                        if (Col + i > 6 || Color != BoardArray[Row][Col + i]){
                            break;
                        }
                        NumSame++;
                    }
                }
                return NumSame;
        }

        /*A helper function to check diagonal directions genercially. Does so by 
        Acceptiing parameters for which direction to move upward. Returns number of same-colored
        pieces connected to given space in desired direction*/
        int CheckDiagonal (int Row, int Col, char Color, char UpDirec){
            int NumSame = 0;
            int ColMod = 0;

            //Check upward first
            if (UpDirec == 'L'){
                ColMod = -1;
            } else {
                ColMod = 1;
            }

            for (int i = 1; i <= 3; i++){
                if (Row - i < 0 || Col + (i * ColMod) < 0 || Col + (i * ColMod) > 6
                        || Color != BoardArray[Row - i][Col + (i * ColMod)]){
                    break; 
                }
                NumSame++;
            }

            //Check downward if necessary
            if (NumSame < 3){
                ColMod *= -1;
                for (int i = 1; i <= 3; i++){
                    if (Row + i > 5 || Col + (i * ColMod) < 0 || Col + (i * ColMod) > 6
                            || Color != BoardArray[Row + i][Col + (i * ColMod)]){
                        break; 
                    }
                    NumSame++;
                }
            }

            return NumSame;
        }

        /*A helper function to check spaces directly below the one given.
         Returns number of same-colored pieces connected to given space
         directly below it*/
        int CheckDown (int Row, int Col, char Color){
            int NumSame = 0;
            for (int i = 1; i <= 3; i++){
                if (Row + i > 5 || BoardArray[Row + i][Col] != Color){
                    break;
                }
                NumSame++;
            }
            return NumSame;
        }

        /*Checks if there is a winner based on a piece placed at
        specified coordinates*/
        bool CheckWin(int Row, int Col){

            char CurrColor = BoardArray[Row][Col];

            //Check for win horizontally
            if (CheckHorizontal(Row, Col, CurrColor) >= 3){
                return true;
            }

            //Check for win travelling diagonally up and left/down and right
            if (CheckDiagonal(Row, Col, CurrColor, 'L') >= 3){
                return true;
            }

            //Check for win travelling diagonally up and right/down and left
            if (CheckDiagonal(Row, Col, CurrColor, 'R') >= 3){
                return true;
            }

            if (CheckDown(Row, Col, CurrColor) >= 3){
                return true;
            }

            return false;
        }

        //Checks if board is full
        bool CheckFull (){
            for (int i = 0; i < 7; i++){
                if (BoardArray[0][i] == '-'){
                    return false;
                }
            }
            return true;
        }

        //Resets the game's settings
        void ResetGame(){
            CleanBoard();
            GameWon = false;
        }

        //Sets the winner of current game and returns their winstreak
        int SetWinner(int Num){
            if (Num == LastWinner){
                WinStreak++;
            } else {
                LastWinner = Num;
                WinStreak = 1;
            }
            return WinStreak;
        }
};

//Methods for someone playing the game
class Player{
    protected:

        //The "color" of a player's piece. 'R' for red, 'Y' for yellow
        char Color;

        //Whether or not this player is a computer
        bool IsCPU;

         /*Finds row piece would fall to if placed at given column. Returns -1
        if column is full*/
        int FindNextRow (char **Board, int Col){
            int CurrRow = 0;
            while (CurrRow < 6 && Board[CurrRow][Col] == '-'){
                CurrRow++;
            }
            return CurrRow - 1;
        }
    
    public:

        //Unsure what to put as default
        Player(){
            Color = 'R';
            IsCPU = true;
        }

        //Assigns the new player a color. Assumes human player
        Player(char ColorChoice){
            Color = ColorChoice;
            IsCPU = false;
        }

        /*Places a new piece in the board at column specified. Returns row 
        that piece fell to. Reference paramter is for personal practice 
        and can be replaced with the class if function is changed necessarily.
        Also, needs to be virtual for polymorphism?*/
        virtual int MakeMove(char **&Board, int Col){
            int Row = FindNextRow(Board, Col);
            if (Row == -1){
                return -1;
            }
            Board[Row][Col] = Color;
            return Row;
        }

        //Returns whether or not this player is a CPU
        bool GetCPU(){
            return IsCPU;
        }
};

//A free space on the board
struct Space {
    int Col; //Column this space is found in
    int HorizConn; //Number of same-colored pieces connected horizontally
    int UpLeftConn; //Number of same-colored pieces connected in up-left diagonal
    int UpRightConn; //Number of same-colored pieces connected in up-right diagonal
    int DownConn; //Number of same-colored pieces connected downward
    int HorizBlock; //Number of opponent's pieces this space would block horizontally
    int UpLeftBlock; //Number of opponent's pieces this space would block in up-left diagonal
    int UpRightBlock; //Number of opponent's pieces this space would block in up-right diagonal
    int DownBlock;  //Number of opponent's pieces this space would block below
    int GreatestConn; //Greatest number among connections
    int HighMidConn; //Higher-middle number among connections
    int LowMidConn; //Lower-middle number among connections
    int LeastConn; //Lowest number among connections
    int GreatestBlock; //Greatest number of opponent's pieces this space can block
    int HighMidBlock; //Higher-middle number of opponent's pieces this space can block
    int LowMidBlock; //Lower-middle number of opponent's pices this space can block
    int LeastBlock; //Lowest number of opponent's pieces this space can block
};

class Computer : public Player{
    private: 

        //The difficulty of the AI player. Ranked 1 to 5
        int Difficulty;
        
        //Stores the most optimal columns a piece can be placed in
        vector<struct Space *> OptimalCols; 

        //Sorts 3 given values from greatest to least
        void SortVals (int a, int b, int c, int d, int (&sorted)[4]){
            vector<int> Vals = {a, b, c, d};
            sorted[0] = max(a, max(b, max(c, d)));
            for (int i = 0; i < 4; i++){
                if (Vals.at(i) == sorted[0]){
                    Vals.erase(Vals.begin() + i);
                    break;
                }
            }
            sorted[3] = min(a, min(b, min(c, d)));
            for (int i = 0; i < 3; i++){
                if (Vals.at(i) == sorted[3]){
                    Vals.erase(Vals.begin() + i);
                    break;
                }
            }
            sorted[1] = max(Vals.at(0), Vals.at(1));
            sorted[2] = min(Vals.at(0), Vals.at(1));
            Vals.clear();
        }

        /*Compares the given spaces by their connections. Returns if curr space was
        added to optimal list*/
        bool CompConns(struct Space *CurrSpace, struct Space *OptimalSpace, int index){
            if (CurrSpace -> GreatestConn > OptimalSpace -> GreatestConn){
                OptimalCols.insert(OptimalCols.begin() + index, CurrSpace);
                return true;
            } else if (CurrSpace -> GreatestConn == OptimalSpace -> GreatestConn && 
                        CurrSpace -> HighMidConn > OptimalSpace -> HighMidConn){
                OptimalCols.insert(OptimalCols.begin() + index, CurrSpace);
                return true;
            } else if (CurrSpace -> GreatestConn == OptimalSpace -> GreatestConn && 
                        CurrSpace -> HighMidConn == OptimalSpace -> HighMidConn && 
                        CurrSpace -> LowMidConn > OptimalSpace -> LowMidConn){
                OptimalCols.insert(OptimalCols.begin() + index, CurrSpace);
                return true;
            } else if (CurrSpace -> GreatestConn == OptimalSpace -> GreatestConn && 
                        CurrSpace -> HighMidConn == OptimalSpace -> HighMidConn && 
                        CurrSpace -> LowMidConn == OptimalSpace -> LowMidConn &&
                        CurrSpace -> LeastConn > OptimalSpace -> LeastConn){
                OptimalCols.insert(OptimalCols.begin() + index, CurrSpace);
                return true;
            } else if (CurrSpace -> GreatestConn == OptimalSpace -> GreatestConn && 
                        CurrSpace -> HighMidConn == OptimalSpace -> HighMidConn && 
                        CurrSpace -> LowMidConn == OptimalSpace -> LowMidConn &&
                        CurrSpace -> LeastConn == OptimalSpace -> LeastConn) {
                if (abs(3 - CurrSpace -> Col) < abs(3 - OptimalSpace -> Col)){
                    OptimalCols.insert(OptimalCols.begin() + index, CurrSpace);
                    return true;
                } else if (abs(3 - CurrSpace -> Col) == abs(3 - OptimalSpace -> Col)){
                    int Temp = rand() % 2;
                    if (Temp == 1){
                        OptimalCols.insert(OptimalCols.begin() + index, CurrSpace);
                        return true;  
                    }
                }
            }
            return false;
        }

         /*Finds the three most optimal columns to place a piece*/
        void FindOptimalCols(Board CurrBoard){

            char OppColor = Color == 'Y' ? 'R' : 'Y';

            //Really big for loop. Possibly improve efficiency?
            for (int i = 0; i < 6; i++){
                int Row = FindNextRow(CurrBoard.BoardArray, i);

                if (Row == -1){
                    //This column is full
                    continue;
                }
                
                struct Space *CurrSpace = new struct Space;
                CurrSpace -> Col = i;

                //Could possibly limit these values at 3, but 3 and > 3 both result in wins anyways
                CurrSpace -> HorizConn = CurrBoard.CheckHorizontal(Row, i, Color);
                CurrSpace -> UpLeftConn = CurrBoard.CheckDiagonal(Row, i, Color, 'L');
                CurrSpace -> UpRightConn = CurrBoard.CheckDiagonal(Row, i, Color, 'R');
                CurrSpace -> DownConn = CurrBoard.CheckDown(Row, i, Color);
                CurrSpace -> HorizBlock = CurrBoard.CheckHorizontal(Row, i, OppColor);
                CurrSpace -> UpLeftBlock = CurrBoard.CheckDiagonal(Row, i, OppColor, 'L');
                CurrSpace -> UpRightBlock = CurrBoard.CheckDiagonal(Row, i, OppColor, 'R');
                CurrSpace -> DownBlock = CurrBoard.CheckDown(Row, i, OppColor);
                
                //Space with nothing around is unoptimal
                if (CurrSpace -> HorizConn + CurrSpace -> UpLeftConn + CurrSpace -> UpRightConn + CurrSpace -> DownConn == 0 && 
                    CurrSpace -> HorizBlock + CurrSpace -> UpLeftBlock + CurrSpace -> UpRightBlock + CurrSpace -> DownBlock == 0){
                    continue;
                }

                //Fill in values for connections and blocks for comparison purposes

                int Vals[4] = {0, 0, 0, 0};
                SortVals(CurrSpace -> HorizConn, CurrSpace -> UpLeftConn, CurrSpace -> UpRightConn, CurrSpace -> DownConn, Vals);
                CurrSpace -> GreatestConn = Vals[0];
                CurrSpace -> HighMidConn = Vals[1];
                CurrSpace -> LowMidConn = Vals[2];
                CurrSpace -> LeastConn = Vals[3];
                
                SortVals(CurrSpace -> HorizBlock, CurrSpace -> UpLeftBlock, CurrSpace -> UpRightBlock, CurrSpace -> DownBlock, Vals);
                CurrSpace -> GreatestBlock= Vals[0];
                CurrSpace -> HighMidBlock = Vals[1];
                CurrSpace -> LowMidBlock = Vals[2];
                CurrSpace -> LeastBlock = Vals[3];

                //Place space in correct place in optimal spaces

                if (OptimalCols.size() == 0){
                    //Nothing else is here, so go ahead and add this one
                    OptimalCols.push_back(CurrSpace);
                    continue;
                }

                bool WasAdded = false;
                for (int i = 0; i < OptimalCols.size(); i++){
                    struct Space *OptimalSpace = OptimalCols.at(i);

                    /*CURRENT AI LOGIC: Checks if opponent stands to gain 
                    more in the next turn than AI currently does. If so, blocks opponent.
                    Otherwise, makes move that leads to most connections. Doesn't care about
                    spaces that are only connected to one opponent's piece. Also prefers 
                    moves that both block and connect*/

                    if (CurrSpace -> GreatestBlock >= 2 && CurrSpace -> GreatestBlock > OptimalSpace -> GreatestBlock){
                        OptimalCols.insert(OptimalCols.begin() + i, CurrSpace);
                        WasAdded = true;
                        break;
                    } else if (CurrSpace -> GreatestBlock >= 2 && CurrSpace -> GreatestBlock == OptimalSpace -> GreatestBlock){
                        //Check HighMidBlock
                        if (CurrSpace -> HighMidBlock >= 2 && CurrSpace -> HighMidBlock > OptimalSpace -> HighMidBlock){
                            OptimalCols.insert(OptimalCols.begin() + i, CurrSpace);
                            WasAdded = true;
                            break;
                        } else if (CurrSpace -> HighMidBlock >= 2 && CurrSpace -> HighMidBlock == OptimalSpace -> HighMidBlock){
                            //Check LowMidBlock
                            if (CurrSpace -> LowMidBlock >= 2 && CurrSpace -> LowMidBlock > OptimalSpace -> LowMidBlock){
                                OptimalCols.insert(OptimalCols.begin() + i, CurrSpace);
                                WasAdded = true;
                                break;
                            } else if (CurrSpace -> LowMidBlock >= 2 && CurrSpace -> LowMidBlock == OptimalSpace -> LowMidBlock){
                                //Check LeastBlock
                                if (CurrSpace -> LeastBlock >= 2 && CurrSpace -> LeastBlock > OptimalSpace -> LeastBlock){
                                    OptimalCols.insert(OptimalCols.begin() + i, CurrSpace);
                                    WasAdded = true;
                                    break; 
                                } else if (CurrSpace -> LeastBlock >= 2 && CurrSpace -> LeastBlock == OptimalSpace -> LeastBlock || 
                                            OptimalSpace -> LeastBlock <= 1){
                                    if (CompConns(CurrSpace, OptimalSpace, i)){
                                        WasAdded = true;
                                        break;
                                    }
                                }
                            } else if (OptimalSpace -> LowMidBlock <= 1){
                                if (CompConns(CurrSpace, OptimalSpace, i)){
                                    WasAdded = true;
                                    break;
                                }
                            }
                        } else if (OptimalSpace -> HighMidBlock <= 1){
                            if (CompConns(CurrSpace, OptimalSpace, i)){
                                WasAdded = true;
                                break;
                            }
                        }
                    } else if (OptimalSpace -> GreatestBlock <= 1){
                        if (CompConns(CurrSpace, OptimalSpace, i)){
                            WasAdded = true;
                            break;
                        }
                    }
                }

                if (!WasAdded && OptimalCols.size() < 3){
                    //May as well add at end since we have space
                    OptimalCols.push_back(CurrSpace);
                }

                //If we now have more than 3 optimal columns, remove last one
                if (OptimalCols.size() == 4){
                    delete OptimalCols.at(3);
                    OptimalCols.pop_back();
                }
            }
        }

        //Frees up OptimalCols vector for later use
        void CleanVector(){
            for (int i = 0; i < OptimalCols.size(); i++){
                delete OptimalCols.at(i);
            }
            OptimalCols.clear();
        }

        //Finds every open column in CurrBoard and places them in FreeCols
        void FindOpenCols (char **CurrBoard, vector<int> &FreeCols){
            for (int i = 0; i < 7; i++){
                if (FindNextRow(CurrBoard, i) != -1){
                    FreeCols.push_back(i);
                }
            }
        }

        //Given the chances for different choices, selects a choice based off of percentage
        int SelectCol(int FirstChance, int SecondChance, int ThirdChance, int Percent, char **Board){
            if (Percent <= FirstChance && OptimalCols.size() >= 1){
                return OptimalCols.at(0) -> Col;
            } else if (Percent <= SecondChance && OptimalCols.size() >= 2) {
                return OptimalCols.at(1) -> Col;
            } else if (Percent <= ThirdChance && OptimalCols.size() >= 3) {
                return OptimalCols.at(2) -> Col;
            } else {
                vector<int> FreeCols;
                FindOpenCols(Board, FreeCols);
                if (FreeCols.size() == 0){
                    return -1;
                }
                int GreatestIndex = FreeCols.size() - 1;
                int RandomIndex = (rand() % (GreatestIndex - 0 + 1));
                int RandomCol = FreeCols.at(RandomIndex);
                FreeCols.clear();
                return RandomCol;
            }
        }
    
    public:

        //Assigns desired difficulty and color to computer player
        Computer(char ColorChoice, int Diff){
            Color = ColorChoice;
            IsCPU = true;
            Difficulty = Diff;
            srand(time(0));
        }

        /*Decides which column to place a piece in. Does so using random
        number generation weighted based on difficulty setting*/
        int DecideCol(Board CurrBoard){
            FindOptimalCols(CurrBoard);
            int Percentage = (rand() % (100 - 1 + 1)) + 1;
            int ColChosen = -1;
            switch (Difficulty){
                case (5):
                    ColChosen = SelectCol(90, 96, 99, Percentage, CurrBoard.BoardArray);
                    break;
                case (4):
                    ColChosen = SelectCol(70, 85, 95, Percentage, CurrBoard.BoardArray);
                    break;
                case (3):
                    ColChosen = SelectCol(50, 80, 90, Percentage, CurrBoard.BoardArray);
                    break;
                case (2):
                    ColChosen = SelectCol(30, 65, 85, Percentage, CurrBoard.BoardArray);
                    break;
                case (1):
                    ColChosen = SelectCol(10, 35, 75, Percentage, CurrBoard.BoardArray);
                    break;
                default:
                    vector<int> FreeCols;
                    FindOpenCols(CurrBoard.BoardArray, FreeCols);
                    if (FreeCols.size() == 0){
                        ColChosen = -1;
                        break;
                    }
                    int GreatestIndex = FreeCols.size() - 1;
                    int RandomIndex = (rand() % (GreatestIndex - 0 + 1));
                    int RandomCol = FreeCols.at(RandomIndex);
                    FreeCols.clear();  
                    ColChosen = RandomCol;
            }
            CleanVector();
            return ColChosen;
        }
};

int main(){
    cout << "Welcome to Connect 4!\n";
    Board *PlayBoard = new Board;
    Player *PlayOne;
    char PlayType;
    char ColorChoice;
    int Difficulty;

    bool SetUp = false;
    while (!SetUp){
        cout << "Now tell me, is Player 1 a human or computer (H or C)?\n";
        cin >> PlayType;
        if (toupper(PlayType) != 'H' && toupper(PlayType) != 'C'){
            cout << "Sorry, that's an invalid entry, please try again\n";
            continue;
        } else {
            SetUp = true;
        }
    }
    SetUp = false;

    while (!SetUp){
        cout << "What color would Player 1 like to use (R or Y)?\n";
        cin >> ColorChoice;
        ColorChoice = toupper(ColorChoice);
        if (ColorChoice != 'R' && ColorChoice != 'Y'){
            cout << "Sorry, that's an invalid entry, please try again\n";
            continue;
        } else {
            SetUp = true;
        }
    }
    SetUp = false;

    if (PlayType == 'H' || PlayType == 'h'){
        PlayOne = new Player(toupper(ColorChoice));
    } else if (PlayType == 'C' || PlayType == 'c'){
        while (!SetUp){
            cout << "What difficulty setting will the computer have (1 - 5)?\n";
            cin >> Difficulty;
            if (Difficulty < 1 || Difficulty > 5){
                cout << "Sorry, that's an invalid entry, please try again\n";
                continue;
            } else {
                SetUp = true;
            }
        }
        PlayOne = new Computer(toupper(ColorChoice), Difficulty);
    }
    SetUp = false;

    Player *PlayTwo;
    while (!SetUp){
        cout << "Is Player 2 a human or computer (H or C)?\n";
        cin >> PlayType;
        if (toupper(PlayType) != 'H' && toupper(PlayType) != 'C'){
            cout << "Sorry, that's an invalid entry, please try again\n";
            continue;
        } else {
            SetUp = true;
        }
    }
    SetUp = false;

    ColorChoice = ColorChoice == 'R' ? 'Y' : 'R';
    if (PlayType == 'H' || PlayType == 'h'){
        PlayTwo = new Player(toupper(ColorChoice));
    } else if (PlayType == 'C' || PlayType == 'c'){
        while (!SetUp){
            cout << "What difficulty setting will the computer have (1 - 5)?\n";
            cin >> Difficulty;
            if (Difficulty < 1 || Difficulty > 5){
                cout << "Sorry, that's an invalid entry, please try again\n";
                continue;
            } else {
                SetUp = true;
            }
        }
        PlayTwo = new Computer(toupper(ColorChoice), Difficulty);
    }

    bool Play = true;
    int Turn = 1;
    Player *CurrPlayer = PlayOne;

    PlayBoard->PrintBoard();
    while (Play){
        CurrPlayer = Turn == 1 ? PlayOne : PlayTwo;
        cout << "Player " << Turn << "\'s turn!\n";
        bool PlayCPU = CurrPlayer -> GetCPU();
        int ColChosen = -1;
        int Row = -1;
        while (Row == -1){
            if (!PlayCPU){
                while (ColChosen < 0 || ColChosen > 6){
                    cout << "Please select a column to place a piece (1 - 7)\n";
                    cin >> ColChosen;
                    ColChosen--;
                    if (ColChosen < 0 || ColChosen > 6 || !cin.good()){
                        cout  << "Sorry, that's an invalid entry. Please try again\n";
                        ColChosen = -1;
                        cin.clear();

                        //Apparently this neat line helps limit us to numbers instead of chars. Avoids infinite loop
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                }
            } else {
                cout << "Computer is making move\n";
                //IS THERE A BETTER WAY TO DO THIS BESIDES CASTING?
                Computer *CompPlayer = dynamic_cast<Computer *>(CurrPlayer);
                ColChosen = CompPlayer -> DecideCol(*PlayBoard);
            }
            Row = CurrPlayer -> MakeMove(PlayBoard -> BoardArray, ColChosen);
            if (Row == -1){
                cout << "Sorry, you can't place anything in that column\n";
            }
        }
        PlayBoard -> PrintBoard();
        bool IsWon = PlayBoard -> CheckWin(Row, ColChosen);
        bool IsDraw = PlayBoard -> CheckFull();
        if (IsWon || IsDraw){
            if (IsWon){
                int Streak = PlayBoard -> SetWinner(Turn);
                cout << "Player " << Turn << " Wins!\n";
                if (Streak > 1){
                    cout << "This is win number " << Streak << " for them!\n";
                }
            } else if (IsDraw){
                PlayBoard -> SetWinner(0);
                cout << "It's a Draw!\n";
            }
            char Decision = ' ';
            while (toupper(Decision) !=  'Y' && toupper(Decision) != 'N'){
                cout << "Would you like to play again (Y/N)?\n";
                cin >> Decision;
                if (Decision == 'Y' || Decision == 'y'){
                    Turn = 1;
                    PlayBoard -> ResetGame();
                } else if (Decision == 'N' || Decision == 'n'){
                    Play = false;
                } else {
                    cout  << "Sorry, that's an invalid entry. Please try again\n";
                }
            }
        } else {
            Turn = Turn == 1 ? 2 : 1;
        }
    }
    cout << "Okay, Thanks for playing!\n";
    
    //Free up the memory we used
    delete PlayOne;
    delete PlayTwo;
    for (int i = 0; i < 6; i++){
        delete PlayBoard -> BoardArray[i];
    }
    delete PlayBoard;
    return 1;
}