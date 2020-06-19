#include"ticTacToeEnv.h"

class RandomAgent{
    public:
        TicTacToeEnv* env;
        int playerId;
        RandomAgent(int id, TicTacToeEnv *_env):playerId(id),env(_env){

        }

        std::pair<int,int> findaction(int **curr_state)
        {
            std::vector<std::pair<int,int>> next_positions;
            for(int i=0; i< env->n; i++)
            {
                for(int j=0; j< env->n; j++)
                {
                    if(curr_state[i][j] == 0)
                        next_positions.push_back(std::make_pair(i,j));
                }
            }

            int action = rand()%(next_positions.size());
            return next_positions[action];
        }
        

};