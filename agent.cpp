#include"agent.h"

TicAgent::TicAgent(int id, std::unordered_map<double,int**> all_states,  TicTacToeEnv *env_)
{
    playerId = id;
    epsilon = 0.1;
    stepsize = 0.1;
    training = true;
    std::vector<const int**> states;
    auto valueestimate = new std::unordered_map<double, double>;
    env = env_;
    initValueEstimates(all_states);
};

TicAgent::~TicAgent()
{
    //sfsf
}

void TicAgent::initValueEstimates(std::unordered_map<double,int**> all_states)
{
    for (auto& it: all_states)
    {
        int** state = it.second;
        int ret = env->gameover(state);

        if(ret == 0 || ret == 3)
            valueestimate[it.first] = 0.5;
        else if(ret == playerId)
            valueestimate[it.first] = 1.0;
        else
            valueestimate[it.first] = 0.0;

    }
}

void TicAgent::reset()
{
    std::vector<int**> empty;
    std::swap( states, empty );   // efficient way of clearing and also removing the memory in cpp
}

std::pair<int,int> TicAgent::findaction()
{
    int** curr_state = states.back();//states.at(-1);
    std::vector<std::pair<int**,int>> next_states;
    std::vector<std::pair<int,int>> next_positions;
    for(int i=0; i< env->n; i++)
    {
        for(int j=0; j< env->n; j++)
        {
            if(curr_state[i][j] == 0)
            {
                next_positions.push_back(std::make_pair(i,j));
                int nstate[3][3];
                std::copy(&curr_state[0][0], &curr_state[0][0]+env->n*env->n,&nstate[0][0]);
                nstate[i][j] = playerId;
                double hashId = env->mapstates1(nstate);
                next_states.push_back(std::make_pair(env->all_states[hashId],hashId));
            }
        }
    }
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    if(training == true && distribution(generator) < epsilon)
    {
        int action = rand()%(next_positions.size());
        return next_positions[action];
    }

    double max = INT32_MIN;
    int max_pos = -1;
    for(int i = 0; i< next_states.size(); i++)
    {
        if(valueestimate[next_states[i].second] > max)
        {
            max = valueestimate[next_states[i].second];
            max_pos = i;
        }
    }
    return next_positions[max_pos];
}

void TicAgent::set_state(int **state)
{
    states.push_back(state);
}

void TicAgent::backup()
{
    if(training == false)
        return;
    std::vector<double> hids;
    for(const auto& value: states) 
    {
        double hid = env->mapstates(value);
        hids.push_back(hid);
    }

    for(int i = hids.size()-2; i>=0; i--)
    {
        double td_error = valueestimate[hids[i+1]] - valueestimate[hids[i]];
        //if(td_error != 0)
        //    std::cout<<"td_error: "<<td_error<<std::endl;
        valueestimate[hids[i]] += stepsize * td_error;
    }

}

void TicAgent::save_policy()
{

}

void TicAgent::load_policy()
{

}
