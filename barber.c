#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/time.h>


void* barber();
void* customer(void*num);
void rand_wait(int secs);

sem_t seat_smf;
sem_t customer_smf;
sem_t barber_smf;
sem_t mutex;


int num_customers;
int max_arrival_time;
int max_haircut_duration;
int haircut_repetition;
int num_seats;
int current_haircut;
int avail_seat;
int is_completed;
int total_waiting;

pthread_t barberThread;
pthread_t customersThread[27];


void main(){

	printf("Enter the number of customers:");
	scanf("%d", &num_customers);
	printf("Enter the number of seats:");
	scanf("%d", &num_seats);
	printf("Enter the max arrival time:");
	scanf("%d", &max_arrival_time);
	printf("Enter the max haircut duration:");
	scanf("%d", &max_haircut_duration);
	printf("Enter the haircut repetition:");
	scanf("%d", &haircut_repetition);
	
	if(num_customers>27){
		printf("Customers cannot be mre than 27.");
		exit(0);
	}
	
	if(num_customers % 2 == 0){
		printf("Number of customers cannot be an even number.");
	}
	
	current_haircut = 0;
	int Number[27];
	
	for(int i=0; i<27; i++){
		Number[i] = i;
	}
		
	sem_init(&seat_smf,0,1);
	sem_init(&customer_smf,0,0);
	sem_init(&barber_smf,0,0);
	sem_init(&mutex, 0,0);
	
	//Create the barber
	pthread_create(&barberThread, NULL, (void*)barber,NULL);
	
	//Create the customers
	for(int i =0; i< num_customers; i++){
		pthread_create(&customersThread[i], NULL, (void*)customer,(void*)&Number[i]);
		sleep(1);
	}
	
	for(int i =0; i< num_customers; i++){
		for(int j=0; j< haircut_repetition; j++){
		pthread_join(customersThread[i], NULL);
		sleep(1);
		}
		printf("----------------------------\n");
		printf("Waiting time for customer %d: %d ms.\n",i+1, total_waiting);
		printf("----------------------------\n");
	}
	
	is_completed =1;
	sem_post(&mutex);
	pthread_join(barberThread, NULL);
	
	
}
void* barber(){
int counter =0;
	while(!is_completed){
		//if no customers, barber is sleeping
		if(counter == 0){
		printf("Barber is sleeping\n");
		sem_wait(&mutex);
		}
		if(!is_completed){
			//waiting barber to be ready for next customer
			sem_wait(&barber_smf);
			printf("Barber is cutting hair\n");
			rand_wait(max_haircut_duration);
			printf("Barber has finished cutting hair\n");
			//barber is ready
			sem_post(&barber_smf);
		}
		else{
			printf("Barber is sleeping\n");
		}
}

}
void* customer(void* number){
	struct timeval start, stop;
	int num = *(int*)number;
	int avail_seat = num_seats;
	int counter = 1;
	
	printf("Customer %d left for barber\n",num+1);
	rand_wait(max_arrival_time);
	printf("Customer %d arrived at barber\n", num+1);
	
	while(!is_completed){
		if(avail_seat>=1){
			
			if(counter==1){
				printf("Customer %d wakes up the barber\n",num+1);
			}
			avail_seat--;
			counter++;
			
			
			printf("Customer %d is waiting for haircut\n", num+1);
			gettimeofday(&start,NULL);
			sem_post(&barber_smf);
			rand_wait(max_haircut_duration);
			sem_wait(&barber_smf);
			printf("Customer %d is having haircut\n", num+1);
			gettimeofday(&stop, NULL);
			double sec = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
			total_waiting += 1000 * sec;
			
		
		}
		else if(avail_seat==0){
			
		printf("Customer %d leaves since there are no seats\n",num+1);
			//exit(0);
			break;
		
		}
	}
	
}

//crate random values for haircut duration and arrival time
void rand_wait(int secs){
	srand(time(NULL));
	int arrive;
	arrive = rand()%(secs+1);
	arrive = arrive/1000;
	sleep(arrive);
}
