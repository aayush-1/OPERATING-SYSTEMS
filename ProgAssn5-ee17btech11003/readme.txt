I-Create a file "inp-params.txt". Give the processes data in the file named inp-params.txt
    Format to be followed is :

    n (no. of philosophers)
    h (no. of times each philosopher eats)
    ucs
    urem
    
    Example:
    5
    10
    1
    2


II-Execution

Compile the dining philosophers code by executing following command-
g++ dphil-ee17btech11003.cpp -o dphil -lpthread

Run the dining philosophers code by executing following command- 
./dphil


FILES:
check Phil-log.txt file for logs of dining philosophers.
check Time.txt file for average waiting time and worst case waiting time for a thread to gain entry to the critical section.

