#include <iostream>
#include <cstdlib>
#include <thread>	// thread::hardware_concurrency()
#include <pthread.h>

using namespace std;

#define NUM_THREADS     5

void *PrintHello(void *threadid)
{
   long tid;
   tid = (long)threadid;
   cout << "Hello World! Thread ID, " << tid << endl;
   pthread_exit(NULL);
}

int main ()
{
	cout << "CPU Count: " << std::thread::hardware_concurrency() << endl;
   pthread_t threads[NUM_THREADS];
   int rc;
   int i;
   for( i=0; i < NUM_THREADS; i++ ){
      cout << "main() : creating thread, " << i << endl;
      rc = pthread_create(&threads[i], NULL, 
                          PrintHello, (void *)i);
      if (rc){
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   pthread_exit(NULL);
}

// void threader()
// {
// 	pthread_t threads[NUM_THREADS];
// 	int rc, i;

// 	for (i = 0; i < NUM_THREADS; i++)
// 	{
// 		rc = pthread_create(&threads[i], NULL, solver(i,relaxation,convergence,iterations), (void *)i);
// 	}
// }