1) input file - "inp-params.txt"

format for input:
n, the number of processes; followed by n lines consisting of Pi
(process id), t (processing time), p (period) and k (the number of times each process
repeats).

for example:
2
P1 10 25 10
P2 16 30 10


2)
    a) Compile the RMS file by executing following command:
    g++ Assgn1-RMSee17btech11003.cpp -o rms

    b) Compile the EDF file by executing following commands:
    g++ Assgn1-EDFee17btech11003.cpp -o edf


3) 
    a) Run the RMS file by executing :
    ./rms

    b) Run the EDF file by executing :
    ./edf

4) Check RMS-Log.txt file for logs of RMS

5) check EDF-Log.txt file for logs of EDF.

6) statistics are present in RMS-Stats.txt and EDF-Stats.txt for rms and edf respectively