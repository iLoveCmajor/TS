#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(){

srand((unsigned)time(0));
int N = 100;
int Arr[N];

for(int i = 0; i < N; i++)
        Arr[i] = (rand()%500) + 1;

//initializing clock
const clock_t begin_time = clock();
//sorting
for( int i = 0; i < N; i++ )
{
        bool stop = true;
        for( int j = 0; j < N-1; j++ )
        {
                if( Arr[ j ] > Arr[ j+1 ] )
                {
                        std::swap( Arr[ j ], Arr[ j+1 ] );
                        stop = false;
                }
        }
        if( stop )
                break;
}
cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC;
//for(int i = 0; i < N; ++i)  cout<<Arr[i]<<" ";
}
