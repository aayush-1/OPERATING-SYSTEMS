I-Create a file "inp-params.txt". Give the processes data in the file named inp-params.txt
    Format to be followed is :

    n (number of processes)
    k (Access frequency)
    lambda1
    lambda2
    
    Example:
    10
    1000
    10
	20


II-Execution

Compile the TAS code by executing following command-
g++ SrcAssgn2-tas-ee17btech11003.cpp -o tas -lpthread

Run the TAS code by executing following command- 
./tas


Compile the CAS code by executing following command-
g++ SrcAssgn2-cas-ee17btech11003.cpp -o cas -lpthread

Run the CAS code by executing following command- 
./cas

Compile the BOUNDED_WAITING_CAS code by executing following command-
g++ SrcAssgn2-cas-bounded-ee17btech11003.cpp -o bwcas -lpthread

Run the BOUNDED_WAITING_CAS code by executing following command- 
./bwcas


FILES:
check TAS-Log.txt file for logs of TAS

check CAS-Log.txt file for logs of CAS

check CAS-bounded-Log.txt file for logs of BOUNDED_WAITING_CAS

