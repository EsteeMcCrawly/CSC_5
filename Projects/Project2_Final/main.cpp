 /* 
 * File:    main.cpp
 * Author:  Marrek Pope
 * Created: May 14, 2019 11:53 AM
 * Purpose: Create a game of craps for Project 2
 */

//System Libraries
#include <iostream> //Input/Output library
#include <cstdlib>  //Random number generator
#include <ctime>    //Time for random number seed.0
#include <fstream>  //File I/O
#include <iomanip>  //Formatting output
#include <string>   //String library
#include <cmath>    //Math functions
#include <vector>   //Vector library
using namespace std;

//User Libraries

//Global Constants, no Global Variables are allowed
//Math/Physics/Conversions/Higher Dimensions - i.e. PI, e, etc...
const float PERCENT=100.0;  //Conversion to percent
const int   SECMINS=60;     //Conversion from seconds to minutes

//Function Prototypes
int **alocate(int,int); //Allocate memory for dynamic 2D array
void destroy(int**,int);//Destroy dynamic 2D array
int roll(int);          //Random dice roll
int placeBt(int,int);   //Takes in the wager of the user
void rules(char);       //Show rules if user asks
bool plyCrap(int**,vector<int>&,int*,bool&,int&,int&,int&,int);//Main game of craps
void simCrap(int*,int*,int,int,int&,int&);//Sim multiple games of craps
void payout(bool,int&,int); //Doubles wager if win and keeps it if loss
void results(int**,int,int,int,int,int,int,int);//Display results of game
void rsltSim(int*,int*,int,int,int,int);//Display results of simulation
void resltFi(ofstream &,vector<int>&,int*,int,int,int,int,int,int,int);//Output results to file

//Execution Begins Here!
int main(int argc, char** argv) {
    //Set the random number seed
    srand(static_cast<unsigned int>(time(0)));
    
    //Declare variables
    const int ROWS=13;//Number of rows in the array
    vector<int>wins(ROWS);//Wins in the main game
    char yesNo;     //User input character if they want to see rules
    bool isWin;     //Boolean statement if user wins or loses
    ifstream in;    //Input File
    ofstream out;   //Output File
    int **table,    //2D array
        *losses=new int[ROWS], //Losses in the main game
        *simWin=new int[ROWS], //Wins in simulation
        *simLoss=new int[ROWS],//Losses in simulation
        numSims,    //How may games user wants to simulate
        maxSims,    //Maximum number of simulations permitted
        mstRlSm,    //Most rolls in a single game of all simulations
        ttlRlSm,    //Total number of rolls in all simulations
        totlGam,    //Total number of non-simulated games
        totlTim,    //Total number of seconds program ran
        totlRol,    //Total number of rolls from all non-simulated games
        gameRol,    //Number of rolls in a given game of craps
        mostRol,    //Highest number of rolls in a single non-simulated game
        numGame,    //Total number of non-simulated games
        numWin,     //Total number of non-simulated wins
        numLoss,    //Total number of non-simulated losses
        menu,       //User input menu choice
        menuSiz,    //How many options are in  menu that user can choose
        wager,      //User input wager put up at beginning of each game
        chips,      //How many chips player has
        cols;       //Columns in the 2D array
            
    //Initialize variables
    string inChips="Chips.dat"; //How many chips user gets
    char   outTabl[]="GameResults.dat"; //Outputs game statistics
    in.open(inChips.c_str());   //Open input file
    out.open(outTabl);          //Open output file
    in>>chips;                  //Read in number of chips from input file
    menuSiz=5;                  //Number of options in the menu
    maxSims=10000000;           //Setting maximum number of simulations
    //Initializing increment variables to 0
    numWin=0;
    numLoss=0;
    mostRol=0;
    totlRol=0;
    totlGam=0;
    cols=2;
    table=alocate(ROWS,cols);
    //Start invisible stopwatch
    int start=time(0);    
    //Ask user if they want to see the rules of the game
    cout<<"Welcome to the Marrek Pope Tuition Foundation Casino!"<<endl;
    cout<<"Today we will be playing craps. Please type Y if you"<<endl;
    cout<<"would like the rules explained or any other key to continue."<<endl;
    cin>>yesNo;
    cout<<endl;
    //Only outputs if user inputs y or Y
    if(yesNo=='y'||yesNo=='Y')
    rules(yesNo);
    //Give user starting chips(read in from file)
    cout<<"Alight let's get started!"<<endl;
    cout<<"Here are the $"<<chips<<" in chips that you purchased."<<endl<<endl;
    //Start menu
    //Loops menu until last menu option is selected
    while(menu!=menuSiz){
        cout<<"Welcome to the Marrek Pope Tuition Foundation Casino!"<<endl;
        cout<<"How can we be of assistance to you today?"<<endl;
        cout<<"Enter 1 to play another round of craps"<<endl;
        cout<<"Enter 2 to see your current chip balance"<<endl;
        cout<<"Enter 3 to see the rules"<<endl;
        cout<<"Enter 4 to simulate a large number of games"<<endl;
        cout<<"Enter 5 to take your winnings and leave"<<endl;
        cin>>menu;
        cout<<endl;
        //User selects option they want
        switch(menu){
            //Play the main game
            case 1:{
                //Boolean from function to increment wins or losses
                if(plyCrap(table,wins,losses,isWin,wager,gameRol,mostRol,chips))
                    numWin++;
                //Statement and increment to keep track of number of losses
                else
                    numLoss++;
                //Checking for most rolls in a single game
                if(mostRol<gameRol)
                    mostRol=gameRol;
                //Incrementing total number of dice rolls
                totlRol+=gameRol;
                cout<<endl;
                payout(isWin,chips,wager);
                break;
            }
            //Output number of chips user currently has
            case 2:{
                cout<<"Your current balance is $"<<chips<<endl<<endl;
                break;
            }
            //Output rules to user
            case 3:{
                rules(yesNo);
                break;
            }
            //Simulate any number of games
            case 4:{
                cout<<"How many games would you like to simulate, up to 10,000,000"<<endl;
                cin>>numSims;
                cout<<endl;
                //Conditional operator to set maximum numbers of simulations
                numSims=numSims>maxSims?maxSims:numSims;
                simCrap(simWin,simLoss,ROWS,numSims,mstRlSm,ttlRlSm);
                rsltSim(simWin,simLoss,ROWS,numSims,mstRlSm,ttlRlSm);
                break;
            }
            //Ends the program and outputs stats
            case 5:{
                //Stop invisible stopwatch
                int finish=time(0);
                //Calculate total number of games and time spent in program
                totlGam=numWin+numLoss;
                totlTim=finish-start;
                //Display game statistics
                results(table,ROWS,numWin,numLoss,mostRol,totlRol,totlTim,totlGam);
                //Output game statistics to file
                resltFi(out,wins,losses,ROWS,numWin,numLoss,mostRol,totlRol,totlTim,totlGam);
                break;
            }
            //Input validation if user does not enter one of the choices.
            default:
                    cout<<"Please enter a number from the list"<<endl;
                    cout<<endl;
                    menu=1;
        }
    }
    
    //Close files
    in.close();
    out.close();
    
    //Deallocate Memory
    delete []simWin;
    delete []simLoss;
    delete []losses;
    destroy(table,ROWS); 
    //Exit stage right or left!
    return 0;
}

void destroy(int **a,int ROWS){
    //Loop to delete array contents, then array
    for(int i=0;i<ROWS;i++){
        delete []a[i];
    }
    delete []a;
}

int **alocate(int ROWS,int cols){
    int **array=new int*[ROWS];
    for(int i=0;i<ROWS;i++){
        array[i]=new int[cols];
    }
    return array;
}

int placeBt (int chips,int wager){
    cout<<"Remember the minimum wager is $5 and the maximum is $500"<<endl;
    //Do-while loop to validate wager is between 5 & 500
    do{
        cout<<"How much would you like to wager this round?"<<endl;
        cin>>wager;
        if(wager>chips){
        //Do-while loop to validate player has enough chips to make wager
        do{
            cout<<endl;
            cout<<"I'm sorry you do not have enough chips to make that wager."<<endl;
            cout<<"You currently only have $"<<chips<<" chips"<<endl;
            cout<<"Please make another wager"<<endl;
            cin>>wager;
        }
        while(wager>chips);
    }
        //If statement to make sure limit of [5,500] is 
        //validated on inner do-while loop
        if(wager<5||wager>500)
        cout<<endl<<"Please enter an amount between $5 and $500."<<endl;
    }
    while(wager<5||wager>500);
    cout<<endl;
    cout<<"Ok your wager has been locked in."<<endl;
    return wager;
}

void resltFi(ofstream &out,vector<int>&wins,int *losses,int ROWS,int numWin,int numLoss,int totlRol,int mostRol,int totlTim,int totlGam){
    //Create 2D array table that outputs which rolls resulted in win/loss
    out<<fixed<<setprecision(2)<<showpoint;
    out<<"Total number of Games = "<<totlGam<<endl;
    out<<"First Roll     Wins     Losses"<<endl;
    int sWins=0,sLosses=0;
    //Number of dice rolls from 2-12
    for(int sum=2;sum<ROWS;sum++){
        sWins+=wins[sum];
        sLosses+=*(losses+sum);
        out<<setw(4)<<sum<<setw(10)<<wins[sum]<<setw(10)<<*(losses+sum)<<endl;
    }
    //Output other basic game statistics
    out<<"Number of wins  : "<<numWin<<endl;
    out<<"Number of losses: "<<numLoss<<endl;
    out<<"Total number of dice rolls: "<<totlRol<<endl;
    //Static casts to output decimals
    out<<"Win percentage :  "<<static_cast<float>(numWin)/totlGam*PERCENT<<"%"<<endl;
    out<<"Loss percentage: "<<static_cast<float>(numLoss)/totlGam*PERCENT<<"%"<<endl;
    out<<"Average number of rolls per game: "<<static_cast<float>(totlRol)/totlGam<<endl;
    out<<"Most rolls in a single game     : "<<mostRol<<endl;
    //If-else statement to decide if game was played long enough to
    //output minutes and seconds or just minutes
    if(totlTim<SECMINS)
        out<<"Total time in the casino: "<<totlTim<< " seconds"<<endl;
    else{
        out<<"Total time in the casino: "<<totlTim/SECMINS<<" minutes and ";
        //If-else statement to ensure remainder function prints out
        //positive seconds every time
        if(remainder(totlTim,SECMINS)<0)
             //Static casts to output integers and remove decimals
             out<<static_cast<int>(remainder(totlTim,SECMINS)+SECMINS)<<" seconds"<<endl;
        else out<<static_cast<int>(remainder(totlTim,SECMINS))<<" seconds"<<endl;
    }out<<endl;
}

void results(int **table,int ROWS,int numWin,int numLoss,int mostRol,int totlRol,int totlTim,int totlGam){
    //Create 2D array table that outputs which rolls resulted in win/loss
    cout<<fixed<<setprecision(2)<<showpoint;
    cout<<"Total number of games = "<<totlGam<<endl;
    cout<<"First Roll   Wins   Losses"<<endl;
    int aWins=0,aLosses=0;
    //Number of dice rolls from 2-12
    for(int sum=2;sum<ROWS;sum++){
        aWins+=table[sum][0];
        aLosses+=table[sum][1];
        cout<<setw(5)<<sum<<setw(12)<<table[sum][0]<<setw(9)<<table[sum][1]<<endl;
    }
    //Output other basic game statistics
    cout<<"Number of wins  : "<<numWin<<endl;
    cout<<"Number of losses: "<<numLoss<<endl;
    cout<<"Total number of dice rolls: "<<totlRol<<endl;
    //Static casts to output decimals
    cout<<"Win percentage : "<<static_cast<float>(numWin)/totlGam*PERCENT<<"%"<<endl;
    cout<<"Loss percentage: "<<static_cast<float>(numLoss)/totlGam*PERCENT<<"%"<<endl;
    cout<<"Average number of rolls per game: "<<static_cast<float>(totlRol)/totlGam<<endl;
    cout<<"Most rolls in a single game     : "<<mostRol<<endl;
    //If-else statement to decide if game was played long enough to
    //output minutes and seconds or just minutes
    if(totlTim<SECMINS)
        cout<<"Total time in the casino: "<<totlTim<<" seconds"<<endl;
    else{
        cout<<"Total time in the casino: "<<totlTim/SECMINS<<" minutes and ";
        //If-else statement to ensure remainder function prints out
        //positive seconds every time
        if(remainder(totlTim,SECMINS)<0)
             //Static casts to output integers and remove decimals
             cout<<static_cast<int>(remainder(totlTim,SECMINS)+SECMINS)<<" seconds"<<endl;
        else cout<<static_cast<int>(remainder(totlTim,SECMINS))<<" seconds"<<endl;
    }
}

void rsltSim(int *simWin,int *simLoss,int ROWS,int numSims,int mstRlSm,int ttlRlSm){
    //Create 2D array table that outputs which rolls resulted in win/loss
    cout<<fixed<<setprecision(2)<<showpoint;
    cout<<"Total number of games: "<<numSims<<endl;
    cout<<"First Roll    Wins    Losses"<<endl;
    int sumWin=0,sumLoss=0;
    //Number of dice rolls from 2-12
    for(int sum=2;sum<ROWS;sum++){
        sumWin+=*(simWin+sum);
        sumLoss+=*(simLoss+sum);
        cout<<setw(5)<<sum<<setw(13)<<*(simWin+sum)<<setw(10)<<*(simLoss+sum)<<endl;
    }
    //Static casts to output decimals
    cout<<"Win percentage : "
            <<static_cast<float>(sumWin)/numSims*PERCENT<<"%"<<endl;
    cout<<"Loss percentage: "
            <<static_cast<float>(sumLoss)/numSims*PERCENT<<"%"<<endl;
    cout<<"Average number of rolls per game: "<<static_cast<float>(ttlRlSm)/numSims<<endl;
    cout<<"Most rolls in a single game     : "<<mstRlSm<<endl;
    cout<<endl;
}

void payout(bool isWin,int& chips,int wager){
    //Double wager if the game is a win
    if(isWin){
        cout<<"Your wager of $"<<wager<<" has been doubled. Good work!"<<endl;
        wager*=2;
    }
    //User loses wager by making it negative and adding to their total
    else{
        cout<<"Your lost your wager of $"<<wager<<". Play another round to win it back!"<<endl;
         wager*=-1;
        }
        chips=chips+wager;//Add wager to chips to get new chips value
        cout<<"Your new chip balance is $"<<chips<<endl<<endl;
}

void rules(char yesNo){
        cout<<"Craps is a dice rolling game. Before the dice are"<<endl;
        cout<<"rolled you will place a wager between $5 and $500."<<endl;
        cout<<"If the dice roll results in a 7 or 11 you win and"<<endl;
        cout<<"will receive your wager back plus the same amount"<<endl;
        cout<<"from the casino. If the result is 2, 3, or 12 your"<<endl;
        cout<<"wager will be kept by the casino and donated to the"<<endl;
        cout<<"Marrek Pope Tuition Foundation. If the result is none"<<endl;
        cout<<"of these numbers you will roll again. This time if you"<<endl;
        cout<<"reroll your first roll you win! However, if you roll a"<<endl;
        cout<<"7 you will lose. You will continue to reroll until"<<endl;
        cout<<"you either get a 7 or your initial roll."<<endl<<endl;
}

int roll(int sides=6){
    int die1,die2;
    if(sides==6){
        die1=rand()%sides+1;//Random number between 1 and 6
        die2=rand()%sides+1;//Random number between 1 and 6
    }
    else if(sides==12){
        die1=rand()%(sides/2)+1;//Random number between 1 and 6
        die2=rand()%(sides/2)+1;//Random number between 1 and 6
    }else{
        //Stop program if dice simulator is broken
        cout<<"The dice simulator is broken.";exit(0);
    }
    int frstSum=die1+die2;
    return frstSum;
}

bool plyCrap(int **table,vector<int>&wins,int *losses,bool& isWin,int& wager,int& gameRol,int& mostRol,int chips){
    gameRol=1;  //Initialize to 1 to account for first roll of every game
    int frstSum=roll();//Call dice function for random dice roll
    //Call wager function to get user game wager
    wager=placeBt(chips,wager);
    cout<<"Your first roll is a "<<frstSum<<endl;
    //Switch statement to evaluate first roll
    switch(frstSum){
        //7 or 11 on first roll is a win
        case  7:
        case 11:
            wins[frstSum]++;    //Increment wins in vector
            table[frstSum][0]++;//Increment wins in 2D array
            isWin=true;//Set boolean true
            cout<<"Congratulations! You won!"<<endl;break;
        //2, 3, or 12 on first roll is a loss
        case  2:
        case  3:
        case 12:
            (*(losses+frstSum))++;//Increment losses in 1D array
            table[frstSum][1]++;  //Increment losses in 2D array
            isWin=false;//Set boolean false
            cout<<"Looks like you lost this round. Better luck next time."<<endl;break;
        //Roll again loop for all other numbers
        default:{
            bool rollAgn=true;//Initialize to true to continue rolling
            do{
                int scndSum=roll(12);//Get another dice roll
                cout<<"Your next roll is a "<<scndSum<<endl;
                gameRol++;//Increment number of rolls in game
                //Loss if roll is a 7
                if(scndSum==7){
                    rollAgn=false;//Stop rolling dice
                    wins[frstSum]++;    //Increment vector counting wins
                    table[frstSum][1]++;//Increment losses in 2D array
                    isWin=false;//Set boolean false
                    cout<<"An unfortunate loss. Better luck next time"<<endl;
                    //Win if matches initial roll
                }else if(frstSum==scndSum){
                    rollAgn=false;//Stop rolling dice
                    (*(losses+frstSum))++;//Increment losses in 1D array
                    table[frstSum][0]++;  //Increment wins in 2D array
                    isWin=true;//Set boolean true
                    cout<<"You rerolled your original roll. You win!"<<endl;
                }
            }while(rollAgn);//Continue rolling while true
        }
    }
    return isWin;
}

void simCrap(int *simWin,int *simLoss,int ROWS,int numSims,int& mstRlSm,int& ttlRlSm){
    //Initialize increment varibles to 0
    ttlRlSm=0;
    mstRlSm=0;
    //Loop simulations until it reaches user input number
    for(int sim=1;sim<=numSims;sim++){
        int gameRol=1;//Initialize to 1 to account for first roll of every game
        int frstSum=roll();//Call dice function for random dice roll
        //Determine wins and losses
        switch(frstSum){
            //7 or 11 on first roll is a win
            case  7:
            case 11:(*(simWin+frstSum))++;break;//Increment wins array
            //2,3, or 12 on first roll is a loss
            case  2:
            case  3:
            case 12:(*(simLoss+frstSum))++;break;//Increment losses in array
            default:{
                //Loop until a 7 or previous sum is thrown
                bool rollAgn=true;
                do{
                    //Roll dice again
                    int scndSum=roll(12);//Set another dice roll
                    gameRol++;//Increment the number of throws
                    //7 results in a loss
                    if(scndSum==7){
                        (*(simLoss+frstSum))++;//Increment losses in array
                        rollAgn=false;         //Stop rerolling
                    //Rerolling original roll is a win
                    }else if(frstSum==scndSum){
                        (*(simWin+frstSum))++;//Increment wins in array
                        rollAgn=false;        //Stop rerolling
                    }
                }while(rollAgn);
            }
        }
        //Keep track of total and maximum rolls
        ttlRlSm+=gameRol;
        if(mstRlSm<gameRol)mstRlSm=gameRol;
    }
}