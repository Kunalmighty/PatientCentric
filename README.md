# PatientCentric
A C++ Linux command line program that extracts patient stay information from a CSV file containing ADT (Admit-Discharge-Transfer) events.

Consists of three files: 
PatientCentric.cpp 

Patient.cpp 

Patient.h. 

Written and tested on a Linux system, and as such I cannot guarantee that it will reliably work on other operating systems without modification. 
In order to run it, you first need to compile and link the source code to generate an executable file. 
On your Linux terminal, navigate to the directory with the three files, 
then type the following command (assuming you have the GNU C++ compiler installed):

`g++ -o PatientCentric PatientCentric.cpp Patient.cpp -I.`

You can also use the following command instead if you want to check for a wider possibility of compilation warnings: 

`g++ -std=c++11 -Wall -o PatientCentric PatientCentric.cpp Patient.cpp -I.`

Upon successful compilation, use this command to run the program: 

`./PatientCentric`

Then simply follow the program's prompts. 

The program has already been run on the originally provided EVENT_DATA.csv file, 
and the computed output is in the file output.csv. 
If you want to run it on other input files, please feel free to do so! 

A few things to keep in mind: 
1) It should work on any input file as long as it is in exactly the same format as the EVENT_DATA.csv file. 
If the format is different, the program will most likely throw an exception and exit safely, but no output will be written.
2) Some patients didn't have a "Discharge" event, so they were not included in the output. 
Only patients with completed stays at a hospital have been included in the output. 
3) The output file is in the requested format, but also includes three extra pieces of information for each patient. 
For each patient, the program calculates: 1) The number of times they were admitted, 2) Average length of stay and 3) Severity (based on whether any of their admissions was an Emergency.)
I make use of a map of Patient class objects, each Patient object has a sorted vector of structs - each struct includes a facility name, Inpatient/Emergency, complaint name, Admission/Discharge, timestamp etc. Look at the code to learn more! I've tried to provide useful comments and function specifications to make your life easier. 
4) The Begin time and End time in the output has been standardized to GMT time, as I felt that this was simpler. 
5) My driving philosophy in designing the program was that it should provide as much information for the benefit of the patient as possible, hence the extra features that provide much needed information about each patient. These pieces of info could then be used to caldculate which patients are the most at-risk, for example. This is also the reason why the program is called "PatientCentric."
6) You may notice that the program runs very fast, and computes the output file for the original EVENT_DATA.csv file in less than a second after receivving the filenames of the input and output files.  
