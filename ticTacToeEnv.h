#ifndef TICTACTOE_ENV_H
#define TICTACTOE_ENV_H

#include<iostream>
#include<vector>
#include<unordered_map>
#include<cmath>

class TicTacToeEnv{
    public:
        TicTacToeEnv();
        ~TicTacToeEnv();
        int** state; // malloc in constructor
        //int state[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
        int iter = 0;
        double reward = 0.0;
        bool done = false;
        const int n = 3;
        std::unordered_map<double,int**> all_states;

        
        void generate_allstates();
        void recursivegenerate(int i, int j, int** init_state);
        double mapstates(int **state);
        double mapstates1(int array[3][3]);
        std::vector<int> count1and2(int **state);
        int gameover(int **state);
        std::tuple<int **,double,bool,int> observe(int winnerplayerId);
        std::tuple<int **,double,bool,int> step(int action);
        std::tuple<int **,double,bool,int> reset();
};




#endif //TICTACTOE_ENV_H