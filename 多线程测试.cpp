#include<pthread.h>
#include<iostream>
using namespace std;

void* f1(void* p){
    cout << "f1" << endl;
    for(int i = 0;i < 10;++i)
        cerr << " ";
        pthread_exit(0);
}

int main(){


pthread_t pd;
    int a = 4;
    pthread_create(&pd,NULL,f1,NULL);



}
