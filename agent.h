#ifndef AGENT_ENV_H
#define AGENT_ENV_H

#include<vector>
#include<unordered_map>
#include"ticTacToeEnv.h"
#include <random>
#include<list>

class TicAgent{
    public:
        TicAgent(int id, std::unordered_map<double,int**> all_states, TicTacToeEnv *_env);
        ~TicAgent();

        int playerId;
        double epsilon;
        double stepsize;
        bool training;
        std::vector<int**> states;
        std::unordered_map<double,double> valueestimate;
        TicTacToeEnv* env;

        void initValueEstimates(std::unordered_map<double,int**> all_states);
        void reset();
        std::pair<int,int> findaction();
        void set_state(int** state);
        void backup();
        void save_policy();
        void load_policy();
};


#endif //AGENT_ENV_H