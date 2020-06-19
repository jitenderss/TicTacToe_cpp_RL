#include"ticTacToeEnv.h"

TicTacToeEnv::TicTacToeEnv()
{
    state = new int *[n];
        for(int i = 0; i < n; ++i)
            state[i] = new int[n];

    for(int i =0; i<3; i++)
        for(int j=0; j<3; j++)
            state[i][j] = 0;
    iter = 0;
    reward = 0.0;
    done = false;
    auto all_states = new std::unordered_map<double, int**>;
    generate_allstates();
}

TicTacToeEnv::~TicTacToeEnv()
{
    for (auto& it: all_states)
    {
        int** state = it.second;
        if(state != NULL)
            delete state;
    }
    all_states.clear();

    // Free the memory
}

double TicTacToeEnv::mapstates(int** state)
{
    double h = 0;
    for(int i=0;i<n;i++)
        for(int j=0; j<n; j++)
            h = 3*h + state[i][j];
    return h;
}

double TicTacToeEnv::mapstates1(int state[3][3])
{
    double h = 0;
    for(int i=0;i<n;i++)
        for(int j=0; j<n; j++)
            h = 3*h + state[i][j];
    return h;
}

std::vector<int> TicTacToeEnv::count1and2(int** state)
{
    std::vector<int> ret(2,0);  // vector of 2 dim with initial value of 0
     for(int i=0; i<n; i++)
     {
        for(int j = 0; j<n; j++)
        {
                if(state[i][j] == 1)
                    ret[0]++;
                else if(state[i][j] == 2)
                    ret[1]++;
        }
     }
     return ret;
}

int TicTacToeEnv::gameover(int **state)
{
    for(int id = 1; id < 3; id++) // player id 1 and 2
    {
        // row check
        for(int i = 0 ; i < 3; i++)
        {
            bool res = true;
            for(int j = 0; j<3; j++)
            {
                    if(state[i][j] != id)
                    {
                        res = false;
                        continue;
                    }
            }
            if(res == true)
                return id;
        }

        // col check
        for(int i = 0 ; i < 3; i++)
        {
            bool res = true;
            for(int j = 0; j<3; j++)
            {
                    if(state[j][i] != id)
                    {
                        res = false;
                        continue;
                    }
            }
            if(res == true)
                return id;
        }

        // Diag check
        if(state[0][0] == id && state[1][1] == id && state[2][2] == id)
            return id;
        if(state[0][2] == id && state[1][1] == id && state[2][0] == id)
            return id;
    }

    // Check if it's a tie and game is ended
    for(int i = 0; i< n; i++) 
        for(int j = 0; j< n; j++) 
            if(state[i][j] == 0)   // means game is still going on as there is a 0 remaining
                return 0;

    return 3;   // 3 is Tie
}

void TicTacToeEnv::recursivegenerate(int i, int j, int** init_state)
{
    if(i == n && j ==n)
    {
        auto ret = count1and2(init_state);
        if(abs(ret[0]-ret[1]) > 1)
            return;
        if(ret[0] < ret[1])
            return;
        if(ret[0] == ret[1] && gameover(init_state) == 1)
            return;
        if(ret[0] > ret[1] && gameover(init_state) == 2)
            return;

        int** newstate = new int *[n];
        for(int i = 0; i < n; ++i)
            newstate[i] = new int[n];
        
        for(int i = 0; i< n; i++) 
            for(int j = 0; j< n; j++)
                newstate[i][j] = init_state[i][j];
        double hashId = mapstates(newstate);
        (all_states)[hashId] = newstate;

        return  ;
    }

    int new_j = j+1;
    int new_i = i;
    if(new_j == n)
    {
        if(new_i != n-1)
            new_j = 0;
        new_i = i + 1; 
    }
    init_state[i][j] = 0;
    recursivegenerate(new_i,new_j,init_state);
    init_state[i][j] = 1;
    recursivegenerate(new_i,new_j,init_state);
    init_state[i][j] = 2;
    recursivegenerate(new_i,new_j,init_state);
}

void TicTacToeEnv::generate_allstates()
{
    int** init_state = new int *[n];
    for(int i = 0; i < n; ++i)
        init_state[i] = new int[n];
    double hashId = mapstates(init_state);
    (all_states)[hashId] = init_state;
    recursivegenerate(0,0,init_state);
}

std::tuple<int **,double,bool,int> TicTacToeEnv::observe(int winnerplayerId)
{
    return std::make_tuple(state,reward,done,winnerplayerId);
}

std::tuple<int **,double,bool,int> TicTacToeEnv::step(int action)
{
    if(action <0 || action >8)
    {
        std::cout<<"Invalid Action\n"<<std::endl;
        return observe(-1);
    }
    int row = (int)action/3;
    int col = (int)action%3;

    if(state[row][col] != 0)
    {
        std::cout<<"Invalid Action\n"<<std::endl;
        return observe(-1);
    }

    // No need to create another array as satate will be fetch from all_states[hash] where it was already malloced
    if(iter%2 == 0)
        state[row][col] = 1;
    else
        state[row][col] = 2;
    
    int ret = gameover(state);
    if(ret)
        done = true;
    iter++;

    if(iter > 8)
        done = true;

    return observe(ret);
    
}

std::tuple<int **,double,bool,int> TicTacToeEnv::reset()
{
    for(int i =0; i<3; i++)
        for(int j=0; j<3; j++)
            state[i][j] = 0;
    iter = 0;
    reward = 0.0;
    done = false;
    return observe(-1);
}
