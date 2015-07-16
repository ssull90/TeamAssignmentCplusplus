//
//  SuckerPunchTeamMaker.cpp
//
//  Created by Sean Patrick Sullivan on 7/13/15.
//
//  My Program Response to:
//
/* You'll be writing a C-style command line program to assign teams for a simple game. Here's how the

game works:

 Each team in the game is composed of units. Each unit has two attributes: an "attack power" and a

"defense power", both integers.

 Each round of the game, we choose a random order for the teams. This defines a set of pairs of teams,

        which face off against each other. So, the first team in the list fights the second team in the list, the third

team fights the fourth team, and so on. If we have an odd number of teams, the last team in the list

        always becomes the first team in the list for the next round.

 When two teams face off, they do simultaneous damage to each other. The maximum amount of

damage done to a team is the difference between the sum of the team's unit's "defense power"

attributes and the sum of the other team's unit's "attack power". The actual damage done to each team

        in a fight is a random integer between 0 and that difference.

 Each team starts with the same number of hit points, which goes down as that team takes damage.

When a team's hit point value hits zero, it's removed from the game.

 The last team left wins.

Your program will take a list of units and a number of teams, and assign units to teams to create a

balanced fight.

The input for your program is a CSV-style list (to stdin or std::cin). The first line of the input is the

number of teams in the game. After that, the units are listed, one per line, with each unit's attack power

and defense power separated by a comma. There's no white space in the file other than the ends of line.

Here's an example input file:

2

5,1

9,1

7,4

1,3

1,3

10,4

4,5

7,4

Your program generates a CSV-style list (to stdout or std::cout) assigning teams to each of the units.

Your goal is to generate a balanced set of N teams. Your output file lists the unit assigned to each team

by unit index.

Given the example input file, here's a reasonable output file:

1,3,6,7

0,2,4,5 

This output file has assigned units 1, 3, 6 and 7 to team 0 and units 0, 2, 4 and 5 to team 1. Team 0 has

        total attack strength 21 and total defense strength 13, while team 1 has total attack strength 23 and

total defense strength 12.

 */
//
// NOTE: error checking is not included.

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <iostream>


using namespace std;


/*
 * A struct used to hold a datatype for easier managability of a units stats withing an vector
 * where worth is attack+defence
 */
typedef struct{
    int id;
    int attack;
    int defence;
    int worth;
    int team;
}unit;

/*
 * This object holds the functions that build the team for easier managability and reusability
 */
class teamBuilder{
    public:
        teamBuilder();
        ~teamBuilder();
        // this public funtion is the driver of the object. Called from Main().
        void assign();
    private:
        // Various variables needed to hold data, special notes:
        // unitLists contains each individual unit and it's attributes
        // teamWorth is used to compare the amount of Attack and Defence the team has as a sum of the two.
        string teams;
        string unitInfo;
        int unitNum;
        int delimiterPos;
        unit tempData;
        vector<unit> unitList;
        vector<int> teamWorth;
        void parseCSV();
        void sortUnits();
        void assignTeams();
        void outputTeams();




        /*
         *
         * Text Book Merge Functions modified very slightly to allow me to look at the specific value "worth"
         * to sort.
         * I did not write the majority of these.(However I do understand and could write them) Credit Goes to Knuth, Donald
         *
         */
        void mergeSortStart(vector<unit> &A, int front, int end, vector<unit> &B);
        void mergeSortMerge(vector<unit> &A, int front, int middle, int end, vector<unit> &B);
        void Copy(vector<unit> &B, int front, int end, vector<unit> &A);
};

teamBuilder::teamBuilder() {
}

teamBuilder::~teamBuilder(){
}
/*
 * Driving Function
 */
void teamBuilder::assign(){
    parseCSV();
    sortUnits();
    assignTeams();
    outputTeams();
}

/*
 * This reads in the CSV input and assigns the corresponding information to the variables in the object
 */
void teamBuilder::parseCSV(){
    unitNum = 0;
    getline(cin, teams);
    teamWorth.resize(atoi(teams.c_str()));
    while(getline(cin, unitInfo)){
        delimiterPos = unitInfo.find(',');
        tempData.id = unitNum;
        tempData.attack = atoi(unitInfo.substr(0,delimiterPos).c_str());
        tempData.defence = atoi(unitInfo.substr(delimiterPos+1,unitInfo.length()).c_str());
        tempData.worth= tempData.attack+tempData.defence;
        unitList.push_back(tempData);
        unitNum++;
    }
}

/*
 * This simply calls the merge sort function
 */
void teamBuilder::sortUnits() {
    vector<unit> unitListCopy(unitList.size());
    mergeSortStart(unitList,0,unitList.size(),unitListCopy);
}

/*
 * This function checks the worth of each team and assigns the units with the largest worth first to each team. It is similar to
 * the knapsack problem solution. Unfortunately this is O(n^2) time.
 */
void teamBuilder::assignTeams(){
    for(int i =0;i < teamWorth.size();i++){
        teamWorth[i] =0;
    }
    int lowest = 0;
    for(int i =0;i < unitList.size();i++){
        for(int j=0;j < teamWorth.size()-1;j++){
            if(teamWorth[lowest]<= teamWorth[j+1]){
            }
            else{
                lowest = j+1;
            }
        }
        unitList[i].team = lowest;
        teamWorth[lowest] = teamWorth[lowest]+unitList[i].worth;
        lowest = 0;
    }

}

/*
 * This function simply reads the "team" value from each unit and outputs the corresponding units to their lines in descending worth order.
 * Also unfortunately O(n^2) time.
 */
void teamBuilder::outputTeams(){
    bool commacheck = false;
    for(int i = 0; i<teamWorth.size();i++){
        for(int j =0;j<unitList.size();j++){
            if(unitList[j].team == i){
                if(commacheck){
                    cout << ',';
                }
                else{
                    commacheck = true;
                }
                cout << unitList[j].id;
            }
        }
        commacheck = false;
        cout << endl;
    }

}



void teamBuilder::mergeSortStart(vector<unit> &A, int front, int end, vector<unit> &B) {
    if((end-front)<2){
        return;
    }
    int middle = (front+end)/2;
    mergeSortStart(A, front,  middle, B);
    mergeSortStart(A, middle,    end, B);
    mergeSortMerge(A, front, middle, end, B);
    Copy(B, front, end, A);

}

void teamBuilder::mergeSortMerge(vector<unit> &A, int front,int middle, int end, vector<unit> &B) {
    int frontTemp = front;
    int middleTemp= middle;

    for (int i = front; i < end; i++) {
        // If left run head exists and is <= existing right run head.
        if (frontTemp < middle && (middleTemp >= end || A[frontTemp].worth >= A[middleTemp].worth)) {
            B[i] = A[frontTemp];
            frontTemp = frontTemp + 1;
        }
        else {
            B[i] = A[middleTemp];
            middleTemp = middleTemp + 1;
        }
    }
}

void teamBuilder::Copy(vector<unit> &B, int front, int end, vector<unit> &A) {
    for(int i = front; i < end; i++)
    A[i] = B[i];
}

/*
 * The Main Function simply instantiazes the oject and calls its public driver function.
 * Then deletes the object upon termination in attempts to avoid memory leaks.
 */
int main(){
    teamBuilder *initiate = new teamBuilder();
    initiate->assign();

    delete(initiate);

    return 0;
}


