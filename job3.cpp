

#include <bits/stdc++.h>
using namespace std;

struct Job 
{ 
    int id; // Job Id 
    int dead; // Deadline of job 
    int profit; 
};

bool compare(Job a, Job b)
{
    return a.profit>b.profit;
}

    vector<int> JobScheduling(Job arr[], int n) 
    { 
        //sorting  the jobs in the decreasing order of their profits
        
         sort(arr,arr+n,compare);
        int maxline=0,j=0;
        
        // finding the max deadline
        for(int i=0;i<n;i++)
        {
            if(arr[i].dead>maxline)
            {
                maxline=arr[i].dead;
            }
        }
        int i=0;
        int a[maxline];
        memset(a,-1,sizeof(a));
        
        //finding time slot for the jobs 
        while(j!=maxline && i<n )
        {
         //if slot is not filled, fill the slot with current job id
            if(a[arr[i].dead-1]==-1)
            {
                a[arr[i].dead-1]=arr[i].id;
                 j++;
            }
            
            //otherwise search for any empty time slot less than the deadline of the current job
            else{
                for(int k=arr[i].dead-1;k>=0;k--)
                {
                 // if such slot found, fill it with current job id and move to next job id
                    if(a[k]==-1)
                    {
                        a[k]=arr[i].id;
                        j++;
                        break;
                    }
                }
            }
            i++;
        }
        
        
      
        vector<int> schedule;
        for(i=0;i<maxline;i++)
        {
            if(a[i]==-1)
            {
                continue;
            }
            else{
                schedule.push_back(a[i]);
            }
        }
        
    
        return schedule;
    } 
    
    
    //driver code
    
int main()
{
Job arr[] = { {1, 2, 100}, {2, 1, 19}, {3, 2, 27},{4, 1, 25}, {5, 3, 15}};
vector<int> schedule=JobScheduling( arr, 5);

cout<<"order of scheduled jobs for maximum profit: ";
for(int i=0;i<schedule.size();i++)
{
cout<<schedule[i]<<" ";
}
}