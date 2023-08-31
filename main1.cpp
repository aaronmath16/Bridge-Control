//Aaron Mathias 2053167
#include <iostream>
#include <pthread.h> 
#include <signal.h> 
#include <fstream>
#include <unistd.h>
using namespace std;

const int MAXTHREADS = 50;

struct carData {
    string vehicleName;
    string vehiclePlate;
    int timeOne;
    int timeTwo;

};

static pthread_mutex_t bridgeUp;
pthread_cond_t carGoes = PTHREAD_COND_INITIALIZER;;
pthread_cond_t shipGoes;
int bridgeStat = 0;
int cars=0 , ships=0;
int T1, T2;
void *car( void *arg){
    carData *v = (carData*) arg;
    cout << "Car " << v->vehiclePlate <<  " arrives at the bridge." << endl;
    //sleep(v->timeOne);
    pthread_mutex_lock(&bridgeUp);
   cars++;
    while (bridgeStat==0) {
        pthread_cond_wait(&carGoes, &bridgeUp);
    }
    //cout << "Car " << v->vehiclePlate <<  " arrives at the bridge." << endl;
    cout << "Car " << v->vehiclePlate << " goes on the bridge." << endl;
    //bridgeStat = 1; 
    sleep(v->timeTwo);
    cout << "Car " << v->vehiclePlate <<  " leaves the bridge." << endl;
    //cars++;
    //bridgeStat = 0;
    //pthread_cond_signal(&shipGoes);
    pthread_mutex_unlock(&bridgeUp);
    pthread_exit(NULL);

    /*delete v;
    return NULL;*/
}

void *ship( void *arg){
  carData *v = (carData*) arg;
  cout << "Ship " << v->vehiclePlate << " arrives at the bridge." << endl;
    //sleep(v->timeOne);
  bridgeStat = 0;
  cout << "Bridge is closed to car traffic." << endl;
    pthread_mutex_lock(&bridgeUp);
  ships++;
    /*while (bridgeStat) { 
        pthread_cond_wait(&shipGoes, &bridgeUp);
    }*/
    //cout << "Ship " << v->vehiclePlate << " arrives at the bridge." << endl;
    //cout << "Bridge is closed to car traffic." << endl;
    cout << "Bridge can now be raised." << endl;
    sleep(T1); 
    cout << "Ship " << v->vehiclePlate << " goes under the raised bridge." << endl;
    sleep(v->timeTwo);
    cout << "Ship " << v->vehiclePlate  << " is leaving." << endl;
    //ships++; 
  sleep(T2);
    cout << "Bridge can now accommodate car traffic." << endl;
    //sleep(T2);
  bridgeStat = 1;
    pthread_cond_signal(&carGoes);
    pthread_mutex_unlock(&bridgeUp);
    pthread_exit(NULL); 

  /*delete v;
  return NULL;*/
  
}

int main() {

carData data[100];
  
string bridgeName;
int count=0;
cin >> bridgeName >> T1 >> T2;

string transportName, transportPlate;
int tOne, tTwo;
while(cin >> transportName){
cin >> transportPlate >> tOne >> tTwo;
data[count].vehicleName = transportName;
data[count].vehiclePlate = transportPlate;
data[count].timeOne = tOne;
data[count].timeTwo = tTwo;
count++;
}
pthread_t tid[MAXTHREADS];
pthread_mutex_init(&bridgeUp, NULL);
for( int i=0; i<count ; i++){
  
    pthread_t x;
    carData* pdata = new carData;
    pdata->vehicleName = data[i].vehicleName;
    pdata->vehiclePlate = data[i].vehiclePlate;
    pdata->timeOne = data[i].timeOne;
    pdata->timeTwo = data[i].timeTwo;
  if(data[i].vehicleName == "Car"){
    //sleep(data[i].timeOne);
    pthread_create(&tid[i], NULL, car, pdata);
  }
  else if(data[i].vehicleName == "Ship")
    //sleep(data[i].timeOne);
    pthread_create(&tid[i], NULL, ship, pdata);
}

for (int i=0; i< count; i++){
    pthread_join(tid[i], NULL);
}

cout << cars << " car(s) crossed the bridge." << endl;
  cout << ships << " ships(s) went under the raised bridge." << endl;




  


return 0;
}