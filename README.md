In this homework, we were supposed to implement a deadlock-free solution for sleeping barber problem. At the end of the program, we were asked to calculate the waiting time of each customer. For this reason, I used Pthreads and semaphores to synchronize the events. The basic algorithm for my implementation is as follows:
• At the beginning, the barber is sleeping since there are no customers at the saloon.
• Then, the customers are created and left for barber. According to their arrival time, they arrive at the barber and the first one wakes up the barber.
• When barber is cutting hair, other customers wait for barber to be available and if there are no space in the waiting room, they leave the saloon.
Arrival time and maximum duration for a haircut are calculated with the aid of a function named rand_wait() which generates a random value between 0 and a given integer .

You can run the code with the aid of a make command
- Open the directory in terminal
- First, run the command make
- Then, run the command /.barber