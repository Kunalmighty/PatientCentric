#include "Patient.h"

using namespace std; 

int Patient::patient_count;

Patient::Patient(string this_id) {
	id = this_id;
	patient_count++;
}

double Patient::avgStays()
{
	double avgcalc;
	for (size_t i = 0; i < stays.size(); i++) {
		avgcalc += stays[i].length;
	}	
	return avgcalc/stays.size();
}

bool Patient::severity()
{
	for (size_t i = 0; i < events.size(); i++) {

		if ((events[i].inpeme).compare("Emergency") == 0) {
			return true;
		}

	}
	return false;
}

size_t Patient::stayCount()
{
	return stays.size(); 	
}

void Patient::addEvent(string one, string two, string three, string four, time_t five)
{
	event topush;

	topush.facility = one;
	topush.inpeme = two;
	topush.complaint = three;
	topush.admidisc = four;
	topush.timestamp = five;

	events.push_back(topush);

}


void Patient::calculateStays() {

	try {
		sort(events.begin(), events.end());
	} catch(...) {
		cerr << "Events were not sorted. Unexpected results." << endl; 
	}	

	stay topush;

	for (size_t i = 0; i < events.size()-1; i++) {		 
		//check if two consecutive timestamps match Admission - Discharge pairs		
		if ((events[i].admidisc).compare("Admission") == 0 && (events[i+1].admidisc).compare("Discharge")==0) {		
			topush.length = difftime(events[i+1].timestamp, events[i].timestamp);
			topush.facility = events[i].facility;
			topush.inpeme = events[i+1].inpeme;
			topush.complaint = events[i].complaint;
			topush.begin = events[i].timestamp;
			topush.end = events[i+1].timestamp;
			stays.push_back(topush);		
		}
		//if the two consecutive timestamps do not match Admission - Discharge pairs
		//A patient currently admitted as an inpatient may be transfered to a different location in the same hospital 
		//& this may trigger another admission event
		if (events[i].admidisc == "Admission" && events[i+1].admidisc == "Admission") {
			if (i+2 != events.size()) {
				topush.length = difftime(events[i+2].timestamp, events[i].timestamp);
				topush.facility = events[i+2].facility;
				topush.inpeme = events[i+2].inpeme;
				topush.complaint = events[i+2].complaint;
				topush.begin = events[i].timestamp;
				topush.end = events[i+2].timestamp;
				stays.push_back(topush);
			}
			else {
				continue; 
			}			
			
		}
	}
}

void Patient::printStays(ofstream& outFile) {	

	char outstr[100];
	char outstr2[100];
	char outstr3[100];
	char outstr4[100];
	struct tm out;
	struct tm out2;	

	for (size_t i = 0; i < stays.size(); i++) {		

		try {
			gmtime_r(&stays[i].begin, &out);		
			gmtime_r(&stays[i].end, &out2);
		} catch(...) {
			cerr << "Processing time values unsuccessful." << endl;
		}			

		int year = out.tm_year; 
        int month = out.tm_mon;     
        int day = out.tm_mday;        
        int hour = out.tm_hour;        
        int minutes = out.tm_min;         
        int seconds = out.tm_sec;    

        int year2 = out2.tm_year; 
        int month2 = out2.tm_mon;     
        int day2 = out2.tm_mday;        
        int hour2 = out2.tm_hour;        
        int minutes2 = out2.tm_min;         
        int seconds2 = out2.tm_sec; 

        int cx = snprintf(outstr, 100, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", year, month+1, day, hour, minutes, seconds);
        int cx2 = snprintf(outstr2, 100, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", year2, month2+1, day2, hour2, minutes2, seconds2);

  		//processing stays[i].length, which is a double of seconds, into days, hours and minutes
  		int toprint = int(stays[i].length);
        int cx3 = snprintf(outstr3, 100, "%.3d Days %.2d Hours %.2d Minutes %.2d Seconds", toprint/60/60/24, (toprint/60/60)%24, (toprint/60)%60, toprint%60);		
		int toprint2 = int(avgStays());
        int cx4 = snprintf(outstr4, 100, "%.3d Days %.2d Hours %.2d Minutes %.2d Seconds", toprint2/60/60/24, (toprint2/60/60)%24, (toprint2/60)%60, toprint2%60);

		string severe;
		if (severity()) {
			severe = "Severe!";
		}
		else {
			severe = "Not Severe.";
		}
		//Patient Identifier,Facility,Patient Complaint,Encounter Class,Encounter Begin Time (GMT),Encounter End Time (GMT),Length of Stay, Number of Stays, Average Length of Stay, Severity
		outFile << id << "," << stays[i].facility << "," << stays[i].complaint << "," 
		<< stays[i].inpeme << "," << outstr << "," << outstr2 << "," << outstr3 << "," << stayCount() << "," << outstr4 << "," << severe << "\n";
		
	}

}

int Patient::getPatientCount() {
	return patient_count;
}