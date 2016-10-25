/*
 ============================================================================
 Name        : L7.cpp
 Author      : Jacob Bracey
 Description : ECE 3220: Lab 7
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <io.h>
#include <conio.h>
#include <string.h>
#include <iostream>
#include <cstring>

using namespace std;
int result=0;

class Signal{
	private:
			int length;
			int max;
			double *data;
			double mean;
			
			
	public:
			Signal();
			Signal(int);
			Signal(const char*);
			Signal& builder();
			Signal& builder(int file_sel);
			Signal& builder(const char* file_name);
			~Signal();
			void offset(double num);
			void scale(double num);
			void center();
			void normal();
			void stats();
			void Sig_info();
			void Print_signal();
			void Save_file(const char*);
};

Signal& Signal::builder(){
	FILE* fp = fopen("Raw_data_01.txt", "r");
	//opens a default input file for reading
	int dum;
	if (fp == NULL) { //making sure the default input file opens correctly
		cout<<"Error opening the default input file Raw_data_01.txt"<<endl;
		result=0;
	}
	
	else{
	fscanf(fp, "%d %d", &length, &max);
	result=1;

	data = new double[length];
	//mallocs space for the array

	int i = 0;
	for (i = 0; i < length; i++) {
		//for loop to put the values into the array
		fscanf(fp, "%d", (&dum));
		data[i]=(double)dum;
	}
	fclose(fp);
	}
	return *this;
}

Signal& Signal::builder(int file_sel){
	char file_name[66];
	int dum;
	if ( file_sel< 10)
		sprintf(file_name, "Raw_data_0%d.txt", file_sel);
		else
		sprintf(file_name, "Raw_data_%d.txt", file_sel);
	
	FILE* fp = fopen(file_name, "r");
	//opens the given input file for reading

	if (fp == NULL) { //making sure the default input file opens correctly
		cout<<"Error opening the default input file "<<file_name<<endl;
		result=0;
	}

	else{
	result=1;
	fscanf(fp, "%d %d", &length, &max);

	data = new double[length];
	//mallocs space for the array

	int i = 0;
	for (i = 0; i < length; i++) {
		//for loop to put the values into the array
		fscanf(fp, "%d", (&dum));
		data[i]=(double)dum;
	}
	fclose(fp);
	}
	return *this;
}

Signal& Signal::builder(const char* file_name){
	FILE* fp = fopen(file_name, "r");
	int dum;
	//opens the given input file for reading

	if (fp == NULL) { //making sure the default input file opens correctly
		cout<<"Error opening the default input file "<<file_name<<endl;
		result=0;
	}
	else{
	result=1;
	fscanf(fp, "%d %d", &length, &max);

	data = new double[length];
	//mallocs space for the array

	int i = 0;
	for (i = 0; i < length; i++) {
		//for loop to put the values into the array
		fscanf(fp, "%d", (&dum));
		data[i]=(double)dum;
	}
	fclose(fp);
	}
	return *this;

}

Signal::~Signal(){
	delete[] data;
	cout << "Signal Destructed" << endl;
}

Signal::Signal(){
	length=0;
	max=0;
	data=NULL;
	mean=0;
}

Signal::Signal(int file_sel){
	builder(file_sel);
}

Signal::Signal(const char* file_name){
	FILE* fp = fopen(file_name, "r");
	//opens the given input file for reading

	if (fp == NULL) { //making sure the default input file opens correctly
		cout<<"Error opening the default input file"<<file_name<<endl;
		
	}

	fscanf(fp, "%d %d", &length, &max);

	data = new double[length];
	//mallocs space for the array

	int i = 0;
	for (i = 0; i < length; i++) {
		//for loop to put the values into the array
		fscanf(fp, "%d", (data + i));
	}
	fclose(fp);
}

void Signal::offset(double num){
	int i=0;
	for(i=0; i<length; i++){ //adds the offset value to all data values
		*(data+i)+=num;
	}
	stats(); //Updates stats after operation
}

void Signal::scale(double num){
	int i=0;
	for(i=0; i<length; i++){ //multiplies the scaling factor to all data values
		*(data+i)=*(data+i)*num;
	}
	stats(); //updates stats after operation
}

void Signal::stats(){ //updates mean and max
	int i=0;
	double add=0;
	max=0;
	for (i = 0; i<length; i++){
		if (*(data+i) > max)
		max = *(data+i);
	}
	for (i = 0; i < length; i++){
		add += *(data+i);
	}
	mean = (double)add/length;
}

void Signal::center(){
	int i=0;
	for(i=0; i<length; i++){ //subtracts mean from all data values
		*(data+i)=*(data+i)-mean;
	}
	stats(); //Updates stats after operation
}

void Signal::normal(){
	int i=0;
	for(i=0; i<length; i++){ //devides by max for all data values
		*(data+i)=*(data+i)/max;
	}
	stats(); //updates stats after operation
}

void Signal::Sig_info(){
	cout << "\nLength: " << length << endl << "\nMax: " << max << endl << "Average: " << mean << endl; //prints out signal info
}

void Signal::Print_signal(){
	int i;
	for(i = 0; i<length; i++) //prints new array values to file
	{
		cout<<*(data+i)<<endl;
	}
}

void Signal::Save_file(const char* file_name){
	FILE* fp = fopen(file_name, "w");
	int i;
	//opens the given output file for write

	if (fp == NULL) //making sure the output file exists and creates a new one if it doesn't
	{
		freopen(file_name, "w", fp);
	}

	fprintf(fp, "%d %d", length, max);
	for(i = 0; i<length; i++) //prints new array values to file
	{
		fprintf(fp, "%.4f\n", *(data+i));
	}
	fclose(fp); //closes file
}


int main(int argc, char *argv[]) {
	
	
	int file_sel=0;
	int choice=0;
	int choice_input_type=0;

	
	//defines all of our file name strings
	char file_name[66];
	char new_file_name[66];
	
	Signal sig1;
	
	double offset_val,scale_val;//vlaues to store scale and offsets	
	
if (argc==3){
	
		if(argv[1][0]=='-' && argv[1][1]=='n'){
				file_sel=atoi(argv[2]);//happens if user gives a valid input
				sig1.builder(file_sel);	
		}
		
		else if(argv[1][0]=='-' && argv[1][1]=='f'){
				sprintf(new_file_name,"%s",argv[2]);
				sig1.builder(new_file_name);
		}
		else	
				cout<<endl<<"The formatting of the command line arguments is incorrect\nYou can only have one tag, either '-f or '-n'.\n";
	
	}


else if(argc==1){
	
	cout<<endl<<"Would you like to\n(1) Enter a filename\n(2) Enter a file number\n(3) Select the default case"<<endl;
	cin>>choice_input_type;
	if(choice_input_type==1){
		cout<<endl<<"Please enter a valid filename\n";
		cin>>file_name;
		sig1.builder(file_name);
	}
	else if(choice_input_type==2){
		cout<<endl<<"Please enter a valid file number\n";
		cin>>file_sel;
		sig1.builder(file_sel);
	}
	else{
		sig1.builder();
	}
	
}
else{
	cout<<endl<<"You entered an incorrect number of command line arguments\nYou may either have no command line arguements or you may have 1 tag ('-f'or '-n') followed by the filename or file number, respectively\n";
}
	
	if(result==0){
		return 0;
	}
	while(choice!=8){
		cout << endl << "Please select an option"<<endl<<"(1):offset signal\n(2):scale signal\n(3):center signal\n(4):normalize signal\n(5):Display Statistics\n(6):Print Signal\n(7):Save Signal\n(8):Exit\n";
		cin>>choice;
		cout<<endl;
	switch(choice){
		case 1:
				cout<<"Please enter an offset value:";
				cin>>offset_val;
				sig1.offset(offset_val);
				cout<<"Signal has been offset"<<endl;
				break;
		case 2:
				cout<<"Please enter a scale value:";
				cin>>scale_val;
				sig1.scale(scale_val);
				cout<<"Signal has been scaled"<<endl;
				break;
		
		case 3:
				sig1.center();
				cout<<"Signal has been centered"<<endl;
				break;
		case 4:
				sig1.normal();
				cout<<"Signal has been normalized"<<endl;
				break;
		case 5:
				sig1.Sig_info();
				cout<<"Signal Statistics have been printed out"<<endl;
				break;
		case 6:
				sig1.Print_signal();
				cout<<"The signal has been printed out"<<endl;
				break;
				
		case 7:
				cout<<"Please enter a file name to save the signal to:";
				cin>>new_file_name;
				sig1.Save_file(new_file_name);
				break;
		case 8:
				
				
				break;
				
		default:
				cout<<"Invalid number entered\nPlease try again\n";
		
	
	}//end of switch statement		
		
	}//end of while

	return 0;

} //end of main

