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

//result is used to determine if files and memory allocation behaved correctly
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
	//default case which opens file 1
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
	//method to be used if a file number is given
	char file_name[66];
	int dum;//used to temporarily hold the number in the input file
	if ( file_sel< 10)//makes the file names
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
	//method if a filename is given by the user
	FILE* fp = fopen(file_name, "r");
	int dum;//used to temporarily hold the number in the input file
	
	//opens the given input file for reading
	if (fp == NULL) { //making sure the default input file opens correctly
		cout<<"Error opening the default input file "<<file_name<<endl;
		
		//sets result to 0 that way main will know that this was unsuccessful
		result=0;
		
	}
	else{
	//this happens if file opening was successful
	
	//sets result to 1 that way main will know that this was successful
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
	//destructor that frees the data that we allocated
	delete[] data;
	cout << "Signal Destructed" << endl;
}

Signal::Signal(){
	//default constructor
	length=0;
	max=0;
	data=NULL;
	mean=0;
}

Signal::Signal(int file_sel){
	//this constructor calls the method that gets all data when given a file number
	builder(file_sel);
}

Signal::Signal(const char* file_name){
	//this constructor calls the method that gets all data when given a filename
	builder(file_name);
}

void Signal::offset(double num){
	//this method offsets the given input string
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
	
	//ints to hold user inputs
	int file_sel=0;
	int choice=0;
	int choice_input_type=0;

	
	//defines all of our file name strings
	char file_name[66];
	char new_file_name[66];
	
	//defines the signal for us to use
	Signal sig1;
	
	double offset_val,scale_val;//vlaues to store scale and offsets	
	
	if (argc==3){
		//if argc=3 then we have one tag and then 1 user input
		//this is the only way this runs
		
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
		
	}//end of case where user enters a tag and then 1 input


	else if(argc==1){
		//if the user leaves the command line blank then this part is executed and input is received from the user
		
		
		//lets the user decide if they want to enter a filename or file number or do the default which is to open data file 1
		cout<<endl<<"Would you like to\n(1) Enter a filename\n(2) Enter a file number\n(3) Select the default case"<<endl;
		cin>>choice_input_type;
		
		//if user wants to enter a full filename
		if(choice_input_type==1){
			cout<<endl<<"Please enter a valid filename\n";
			cin>>file_name;
			sig1.builder(file_name);
		}
		
		//if user wants to enter a file number
		else if(choice_input_type==2){
			cout<<endl<<"Please enter a valid file number\n";
			cin>>file_sel;
			sig1.builder(file_sel);
		}
		
		//if user wants to perform the default case which is to open text file 1
		else{
			sig1.builder();
		}
		
	}//end of case where user leaves command line empty


	else{
		//if the user enters an incorrect number of command line arguments then an error message is displayed an the program is terminated
		cout<<endl<<"You entered an incorrect number of command line arguments\nYou may either have no command line arguements or you may have 1 tag ('-f'or '-n') followed by the filename or file number, respectively\n";
	}
		
		
	if(result==0){
		//if no files are read then result will equal 0
		//if there is no data there is no need to get user input
		//ending program if this is the case
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
					//exits out of the switch
					break;
					
			default:
					cout<<"Invalid number entered\nPlease try again\n";
			
		
		}//end of switch statement		
		
	}//end of while

return 0;

} //end of main

