I-Create a file "inp-params.txt". Give the processes data in the file named inp-params.txt
    Format to be followed is :

capacity, np, nc, cntp, cntc, μp, μc
    
    Example:
100 10 15 15 10 8 12


II-Execution

Compile the semaphore code by executing following command-
g++ prod_cons-sems-ee17btech11003.cpp -o sem -lpthread

Run the TAS code by executing following command- 
./sem


Compile the CAS code by executing following command-
g++ prod_cons-locks-ee17btech11003.cpp -o lock -lpthread

Run the CAS code by executing following command- 
./lock


FILES:
check prod_cons-locks-LOG.txt file for logs of prod_cons-sems-ee17btech11003.cpp

check prod_cons-locks-LOG.txt file for logs of prod_cons-locks-ee17btech11003.cpp


