#pragma once
#define PATIENT_H

#include <string>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream> 
using namespace std; 

class Patient {
	static int patient_count;
	
	string id;
	
	struct event {
		string facility;
		string inpeme;
		string complaint;
		string admidisc;
		time_t timestamp = 0;

		bool operator<(const event& a) const
		{
			return (difftime(a.timestamp, timestamp) > 0);
		}
	};
	vector<event> events;

	struct stay {
		double length=0;
		string facility;
		string inpeme;
		string complaint;
		time_t begin = 0;
		time_t end = 0;
	};
	vector<stay> stays;

	int numberOfStays;
	double totalStays;
	int severe = 0; 

public: 
	Patient(string this_id);

	double avgStays();
	bool severity();
	size_t stayCount();
	void addEvent(string one, string two, string three, string four, time_t five);	
	
	void calculateStays();
	void printStays(ofstream& outFile);

	static int getPatientCount();
};