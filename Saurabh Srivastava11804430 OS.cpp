#include <bits/stdc++.h>
using namespace std;
struct Process_Data
{
	int Num;
	int Pid;  //Process Id
	int Arrival_time; //Process Arrival Time
	int Burst_time; //Process Burst Time
	int Priorityp; //Process Priorityp
	int Finishtime; //Process Finish Time
	int Realtime; //Process Remaining  Time During Execution
	int Waittime; //Waiting Time
	int Stoptime; //Process start Time
	int Restoptimer;

};

struct Process_Data current;
typedef struct Process_Data P_d ;

bool iddsort(const P_d& x , const P_d& y)
{
	return x.Pid < y.Pid;
}

bool arrivalsort( const P_d& x ,const P_d& y)
{
	if(x.Arrival_time < y.Arrival_time)
		return true;
	else if(x.Arrival_time > y.Arrival_time)
		return false;
	if(x.Priorityp < y.Priorityp)
		return true;
	else if(x.Priorityp > y.Priorityp)
		return false;
	if(x.Pid < y.Pid)
		return true;

	return false;
}


bool Numpsort( const P_d& x ,const P_d& y)
{
	return x.Num < y.Num;
}
struct comPare
{
	bool operatorp()(const P_d& x ,const P_d& y)
	{
		if( x.Priorityp > y.Priorityp )
			return true;
		else if( x.Priorityp < y.Priorityp )
			return false;
		if( x.Pid > y.Pid )
			return true;

		return false;
		
	}
	
};

void new_check(vector<P_d> mv)
{
	for(unsigned int i= 0; i < mv.size() ;i++)
	{
		cout<<" Pid :"<<mv[i].Pid<<" _time : "<<mv[i].Arrival_time<<" Burst_time : "<<mv[i].Burst_time<<" Priorityp : "<<mv[i].Priorityp<<endl;
	}

}

int main()
{
	int i;
	vector< P_d > input;
	vector<P_d> input_copy;
	P_d temp;
	int pq_process = 0; // for PQ process
	int rq_process = 0; // for RQ process
	int Arrival_time;
	int Burst_time;
	int Pid;
	int Priorityp;
	int n;
	int clock;
	int total_exection_time = 0;
	cin>>n;
	for( i= 0; i< n; i++ )
	{
		cin>>Pid>>Arrival_time>>Burst_time>>Priorityp;
		temp.Num = i+1;
		temp.Arrival_time = Arrival_time;
		temp.Burst_time = Burst_time;
		temp.Realtime = Burst_time;
		temp.Pid = Pid;
		temp.Priorityp = Priorityp;
		input.push_back(temp);
	}
	input_copy = input;
	sort( input.begin(), input.end(), arrivalsort );
    total_exection_time = total_exection_time + input[0].Arrival_time;
    for( i= 0 ;i< n; i++ )
    {
    	if( total_exection_time >= input[i].Arrival_time )
    	{
    		total_exection_time = total_exection_time +input[i].Burst_time;
    	}
    	else
    	{
    		int diff = (input[i].Arrival_time - total_exection_time);
    		total_exection_time = total_exection_time + diff + Burst_time;

    	}
    }

	int Ghant[total_exection_time]={0};
	for( i= 0; i< total_exection_time; i++ )
	{
		Ghant[i]=-1;
	}

	Priorityp_queue < P_d ,vector<Process_Data> ,comPare> pq; 

	queue< P_d > rq; 
	int cpu_state = 0; 
	int quantum = 4 ; 
	current.Pid = -2;
	current.Priorityp = 999999;

	for ( clock = 0; clock< total_exection_time; clock++ )
	{
		for( int j = 0; j< n ; j++ )
		{
			if(clock == input[j].Arrival_time)
			{
				pq.push(input[j]);
			}
		}
		

		if(cpu_state == 0)
		{
			if(!pq.empty())
			{
				current = pq.top();
				cpu_state = 1;
				pq_process = 1;
				pq.pop();
				quantum = 4; 
			}
			else if(!rq.empty())
			{
				current = rq.front();
				cpu_state = 1;
				rq_process = 1;
				rq.pop();
				quantum = 4;
			}
		}
		else if(cpu_state == 1)
		{
			if(pq_process == 1 && (!pq.empty()))
			{
				if(pq.top().Priorityp < current.Priorityp )
				{
					rq.push(current);
					current = pq.top();
					pq.pop();
					quantum = 4; 
				}
			}
			else if(rq_process == 1 && (!pq.empty()))
			{
				rq.push(current);
				current = pq.top();
				pq.pop();
				rq_process = 0;
				pq_process = 1;
				quantum = 4 ;
			}
			

		}


		if(current.Pid != -2) 
		{
			current.Realtime--;
			quantum--;
			Ghant[clock] = current.Pid;
			if(current.Realtime == 0)
			{
				cpu_state = 0 ;
				quantum = 4 ;
				current.Pid = -2;
				current.Priorityp = 999999;
				rq_process = 0;
				pq_process = 0;
			}
			else if(quantum == 0 )
			{
				rq.push(current);
				current.Pid = -2;
				current.Priorityp = 999999;
				rq_process = 0;
				pq_process = 0;
				cpu_state=0;

			}

		}
		
	}


	sort( input.begin(), input.end(), iddsort );
	
	for(int i=0;i<n;i++)
	{
		for(int k=total_exection_time;k>=0;k--)
		{
			if(Ghant[k]==i+1)
			{
				input[i].Finishtime=k+1;
				break;

			}
		}
	}
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<total_exection_time;k++)
		{

			if(Ghant[k]==i+1)
			{
				input[i].Stoptime=k;
				break;
			}
		}
	}
	
	sort( input.begin(), input.end(), Numpsort );

	for(int i=0;i<n;i++)
	{
		input[i].Restoptimer=input[i].Stoptime-input[i].Arrival_time;
		input[i].Waittime=(input[i].Finishtime-input[i].Arrival_time)-input[i].Burst_time;

	}
	
	for(int i=0;i<n;i++)
	{
		cout<<input[i].Pid<<" "<<input[i].Restoptimer<<" "<<input[i].Finishtime<<" "<<input[i].Waittime<<endl;
		
	}	
	return 0;
}

