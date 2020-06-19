#include"ticTacToeEnv.h"
#include"agent.h"
#include"heuristics.h"

void myprint(int **state)
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(state[i][j] == 1)
                std::cout<<" "<<" X";
            else if(state[i][j] == 2)
                std::cout<<" "<<" O";
            else
                std::cout<<" "<<" -";
        }
        std::cout<<std::endl;
    } 
    std::cout<<std::endl;
}

void train_agent(TicTacToeEnv* env, TicAgent* agent1, TicAgent* agent2, int train_duration)
{
    std::cout<<"train_duration: "<<train_duration<<std::endl;
    int agent1Wincount = 0;
    int agent2Wincount = 0;
    int drawcount = 0;

    for(int i = 0; i < train_duration; i++)
    {
        if(i%10000 == 0)
        {
            std::cout<<"Iteration: "<<i<<std::endl;
            std::cout<<"Agent 1 won: "<<agent1Wincount<<std::endl;
            std::cout<<"Agent 2 won: "<<agent2Wincount<<std::endl;
            std::cout<<"Draw count: "<<drawcount<<std::endl;
        }
        std::tuple<int **,double,bool,int> state =  env->reset();
        agent1->reset();
        agent2->reset();
        double hid = env->mapstates(std::get<0>(state));
        agent1->set_state(env->all_states[hid]);
        agent2->set_state(env->all_states[hid]);

        int turnId = 0;
        bool done = false;
        while(!done)
        {
            int x,y;
            std::pair<int,int>ret;
            if(turnId == 0)
                ret = agent1->findaction();
            else
                ret = agent2->findaction();
            x = ret.first;
            y = ret.second;
            int action = 3*x + y;
            std::tuple<int **,double,bool,int> Tuple = env->step(action);
            double hid = env->mapstates(std::get<0>(Tuple));
            agent1->set_state(env->all_states[hid]);
            agent2->set_state(env->all_states[hid]);
            turnId = (turnId + 1)%2;
            done = std::get<2>(Tuple);
            if(done)
            {
                int result = std::get<3>(Tuple);
                if(result == 1)
                    agent1Wincount++;
                else if(result == 2)
                    agent2Wincount++;
                else
                    drawcount++;
            }

        }
        agent1->backup();
        agent2->backup();

    }
    agent1->save_policy();
    agent2->save_policy();
    std::cout<<"Agent 1 won: "<<agent1Wincount<<std::endl;
    std::cout<<"Agent 2 won: "<<agent2Wincount<<std::endl;
    std::cout<<"Draw count: "<<drawcount<<std::endl;
}

void test_agent(TicTacToeEnv* env, TicAgent* agent1, TicAgent* agent2, int test_duration)
{
    std::cout<<"test_duration: "<<test_duration<<std::endl;
    int agent1Wincount = 0;
    int agent2Wincount = 0;
    int drawcount = 0;
    agent1->training = false;
    agent2->training = false;
    
    /*
    for (auto& it: agent2->valueestimate)
    {
        double hid = it.first;
        std::cout<<" "<<agent2->valueestimate[hid]<<std::endl;
    }
    */

    for(int i = 0; i < test_duration; i++)
    {
        std::tuple<int **,double,bool,int> state =  env->reset();
        agent1->reset();
        agent2->reset();
        double hid = env->mapstates(std::get<0>(state));
        agent1->set_state(env->all_states[hid]);
        agent2->set_state(env->all_states[hid]);
        //myprint(std::get<0>(state));
        int turnId = 0;
        bool done = false;
        while(!done)
        {
            int x,y;
            std::pair<int,int>ret;
            if(turnId == 0)
                ret = agent1->findaction();
            else
                ret = agent2->findaction();
            x = ret.first;
            y = ret.second;
            int action = 3*x + y;
            std::tuple<int **,double,bool,int> Tuple = env->step(action);
            //myprint(std::get<0>(Tuple));
            double hid = env->mapstates(std::get<0>(Tuple));
            agent1->set_state(env->all_states[hid]);
            agent2->set_state(env->all_states[hid]);
            turnId = (turnId + 1)%2;
            done = std::get<2>(Tuple);
            if(done)
            {
                int result = std::get<3>(Tuple);
                if(result == 1)
                    agent1Wincount++;
                else if(result == 2)
                    agent2Wincount++;
                else
                    drawcount++;
            }

        }

    }
    std::cout<<"Agent 1 won: "<<agent1Wincount<<std::endl;
    std::cout<<"Agent 2 won: "<<agent2Wincount<<std::endl;
    std::cout<<"Draw count: "<<drawcount<<std::endl;
}

void test_agent_rl_random(TicTacToeEnv* env, TicAgent* agent1, RandomAgent* agent2, int test_duration)
{
    std::cout<<"test_duration: "<<test_duration<<std::endl;
    int agent1Wincount = 0;
    int agent2Wincount = 0;
    int drawcount = 0;
    agent1->training = false;
    

    for(int i = 0; i < test_duration; i++)
    {
        std::tuple<int **,double,bool,int> Tuple =  env->reset();
        agent1->reset();
        
        double hid = env->mapstates(std::get<0>(Tuple));
        agent1->set_state(env->all_states[hid]);
        
        //myprint(std::get<0>(Tuple));
        int turnId = 0;
        bool done = false;
        while(!done)
        {
            int x,y;
            std::pair<int,int>ret;
            if(turnId == 0)
                ret = agent1->findaction();
            else
                ret = agent2->findaction(std::get<0>(Tuple));
            x = ret.first;
            y = ret.second;
            int action = 3*x + y;
            std::tuple<int **,double,bool,int> Tuple = env->step(action);
            //myprint(std::get<0>(Tuple));
            double hid = env->mapstates(std::get<0>(Tuple));
            agent1->set_state(env->all_states[hid]);
            
            turnId = (turnId + 1)%2;
            done = std::get<2>(Tuple);
            if(done)
            {
                int result = std::get<3>(Tuple);
                if(result == 1)
                    agent1Wincount++;
                else if(result == 2)
                    agent2Wincount++;
                else
                    drawcount++;
            }

        }

    }
    std::cout<<"Agent 1 won: "<<agent1Wincount<<std::endl;
    std::cout<<"Agent 2 won: "<<agent2Wincount<<std::endl;
    std::cout<<"Draw count: "<<drawcount<<std::endl;
}

int main()
{
    TicTacToeEnv env;
    TicAgent agent1(1,env.all_states,&env);
    TicAgent agent2(2,env.all_states,&env);
    int train_duration = (int)1e6;
    int test_duration = (int)1e3;
    train_agent(&env,&agent1,&agent2,train_duration);
    //test_agent(&env,&agent1,&agent2,test_duration);

    RandomAgent random_agent(2,&env);
    test_agent_rl_random(&env,&agent1,&random_agent,test_duration);
}
