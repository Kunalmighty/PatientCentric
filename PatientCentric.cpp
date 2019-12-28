// PatientCentric.cpp : This file contains the 'main' function. Program execution begins and ends here.

#include <stdio.h>
#include <map>

#include "Patient.h"

using namespace std; 

int main()
{
    cout << "Welcome to PatientCentric.\n This application will take your input patient ADT (Admit-Discharge-Transfer) events data as a single CSV file \n and will create two CSV files with corresponding patient data.";
    cout << "\n\nPlease provide the name of your CSV file: (Do not forget to include the .csv file extension.)\n";

    string filename, filename2;
    cin >> filename; 

    cout << "\nThank you. What should we name your CSV output file? (Do not forget to include the .csv file extension.)\n";
    cin >> filename2; 

    //File pointer 
    fstream fin;
    
    try {
        //Open the existing CSV input file 
        fin.open(filename, ios::in);
    } catch (...) {
        cerr << "Can't open input file. Perhaps a bad filename?" << endl; 
    }

    //Read from file into a vector of strings, one string per row. Then, separate the row into patient data. 
    vector<vector<string>> rows;
    vector<string> patientData;
    string line, word, firstline;    

    getline(fin, firstline);

    //A while loop that populates the vector of rows
    while(getline(fin, line)) {

        try {            

            patientData.clear();

            //processing each line of the input file so that the quoted Complaint column is extracted
            size_t pos1 = line.find("\"");
            if (pos1 != string::npos) {

                string sub = line.substr(pos1+1);                
                size_t pos2 = sub.find("\"");
                string sub2 = sub.substr(0, pos2+1);
                sub2.insert(0, "\"");                

                line.erase(pos1, sub2.size()+1);

                stringstream breaker(line);

                int counter = 0;
                //A while loop to read all the columns of a row & store them in our vector 
                while (getline(breaker, word, ',')) { 

                    if (counter != 3) {
                        patientData.push_back(word);
                    }
                    else {
                        patientData.push_back(sub2);
                        patientData.push_back(word);
                    }            
                    counter++;
                }                

            }

            else {
                stringstream breaker2(line);
                while (getline(breaker2, word, ',')) {
                    patientData.push_back(word);
                }
            }
        }   catch(...) {
            cerr << "Error occurred while trying to populate data vectors." << endl; 
        }        

        rows.push_back(patientData);
    
    }

    //Creating map of Patients, with pointers to Patient objects in memory
    map<string, Patient*> patients; 

    int y, M, d, h, m, s, tzh, tzm;
    tm time = {0};
    time_t toinsert;

    //Populating map of Patients    
    for (size_t i = 0; i < rows.size(); i++) {        

        patients.insert(make_pair(rows[i][0], new Patient(rows[i][0])));
        //Converting datetime string to time_t
        const char* dateStr = rows[i][5].c_str();
        if (sscanf(dateStr, "%d-%d-%dT%d:%d:%d-%d:%d", &y, &M, &d, &h, &m, &s, &tzh, &tzm) != 8) {
            cerr << "There is something wrong with the input file format. Exiting." << endl; 
            return -1;
        };
        if (tzh < 0) {
            tzm = -tzm;    // Fixing the sign on minutes.
        }
        h = h - tzh;
        m = m - tzm;

        
        time.tm_year = y; 
        time.tm_mon = M-1;     
        time.tm_mday = d;        
        time.tm_hour = h;        
        time.tm_min = m;         
        time.tm_sec = s;    
        time.tm_isdst = 0; 

        toinsert = mktime(&time);

        patients.at(rows[i][0])->addEvent(rows[i][1], rows[i][2], rows[i][3], rows[i][4], toinsert);        
        
    }

    ofstream outFile;
    outFile.open(filename2);
    if (!outFile) {
        cerr << "can't open output file" << endl;
    }
    map<string, Patient*>::iterator it = patients.begin();

    outFile << "Patient Identifier,Facility,Patient Complaint,Encounter Class,Encounter Begin Time (GMT),Encounter End Time (GMT),Length of Stay, Number of Stays, Average Length of Stay, Severity\n";

   
    // Iterate over the map using Iterator till end.
    while (it != patients.end()) {        
        Patient count = *it->second;   
        count.calculateStays();        
        count.printStays(outFile);
        // Incrementing the iterator to point to next entry
        it++;
    } 

    outFile.close();

    cout << "\nAll operations were completed successfully. Please check your output CSV file for results. Thank you for using PatientCentric!\n"; 

    return 0;
}