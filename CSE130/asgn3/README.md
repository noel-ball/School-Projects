//------------------------------------------------------------------------------
//  Noel Ball njball 02/26/2024
//  README.md for CSE 130 Programming Assignment 3
//------------------------------------------------------------------------------

Queue.c

Constructor and destructor initializes or deletes the necessary fields and pthread mutexes and conditions needed for queue

In order for queue_push and queue_pop to be blocked correctly, with my implementation choice of mutexes and condition variables I needed to use the pthread functions cond_wait and cond_signal. 

Queue_push waits for pthread_cond_signal(not_full) when the queue is full, pushes the element, then signals not_empty since an element has been pushed.

Queue_pop waits for pthread_cond_signal(not_empty) when the queue is empty, pops the element, then signals not_full since an element has been popped.

//------------------------------------------------------------------------------

Rwlock.c

Constructor and destructor initializes or deletes the necessary fields and pthread mutexes and conditions needed for rwlock

In order for my lock functions to be blocked correctly, I needed to use condition variables to keep track of how many readers/writers are waiting and how many readers/writers are active. 

Reader_lock waits if the priority is WRITERS and there are any writers waiting, if the priority is N_WAY and the number of active readers is more than the n_limit, or if there are any active writers, then increases the number of active readers and acquires the rwlock. 

Reader_unlock releases the rwlock, then decreases the n_limit for N_WAY priority when an active reader is done to keep track of how many readers have the lock before it is given to the next writer or batch of readers. Ends with pthread_cond_signal to waiting readers/writers. 

Writer_lock waits if there are active readers or writers, then increases the number of active writers and acquires the lock.

Writer_unlock releases the rwlock, resets the n_limit to n_init for N_WAY priority, then pthread_cond_signal to waiting readers/writers.  
