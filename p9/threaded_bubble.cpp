#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

int main(){

srand((unsigned)time(0));
int N = 1000;
int Arr[N];

for(int i = 0; i < N; i++)
        Arr[i] = (rand()%500) + 1;
//initializing clock
const clock_t begin_time = clock();

//sorting
for( int i = 0; i < N; i++ )
{
    int first = i % 2;
#pragma omp parallel for default(none),shared(Arr,first)
    for( int j = first; j < N-1; j += 2 )
    {
            if( Arr[ j ] > Arr[ j+1 ] )
            {
                    std::swap( Arr[ j ], Arr[ j+1 ] );
            }
    }
}
cout << float( clock () - begin_time ) /  CLOCKS_PER_SEC;

//for(int i = 0; i < N; ++i)
//        cout<<Arr[i]<<" ";
}
