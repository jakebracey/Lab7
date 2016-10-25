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

int isNumeric (const char * s);
//int *load_array(char* file, int* length, int* max_val);

/*
These are the functions that were used before converting this code to C++

void do_offset(int* array, int* length, char*file, double val);
void do_scale(int* array, int* length, char*file,double val);
double getmean(int[], int*);
int getmax(int[], int*);
void write_stats(char[], int[], int*);
void do_center(int[], int*, char*);
void do_normal(int[], int*, char*);
*/

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
			Signal builder();
			Signal builder(int file_sel);
			Signal builder(const char* file_name);
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

Signal Signal::builder(){
	FILE* fp = fopen("Raw_data_01.txt", "r");
	//opens a default input file for reading

	if (fp == NULL) { //making sure the default input file opens correctly
		printf("Error opening the default input file Raw_data_01.txt");
		return 0;
		//terminates program
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
	printf("before reutnr\n");
	return *this;
}

Signal Signal::builder(int file_sel){
	char file_name[66];
	if ( file_sel< 10)
		sprintf(file_name, "Raw_data_0%d.txt", file_sel);
		else
		sprintf(file_name, "Raw_data_%d.txt", file_sel);
	
	FILE* fp = fopen(file_name, "r");
	//opens the given input file for reading

	if (fp == NULL) { //making sure the default input file opens correctly
		printf("Error opening the default input file Raw_data_01.txt");
		return 0;
		//terminates program
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
	return *this;
}

Signal Signal::builder(const char* file_name){
	FILE* fp = fopen(file_name, "r");
	//opens the given input file for reading

	if (fp == NULL) { //making sure the default input file opens correctly
		printf("Error opening the default input file Raw_data_01.txt");
		return 0;
		//terminates program
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
	char file_name[66];
	if ( file_sel< 10)
		sprintf(file_name, "Raw_data_0%d.txt", file_sel);
		else
		sprintf(file_name, "Raw_data_%d.txt", file_sel);
	
	FILE* fp = fopen(file_name, "r");
	//opens the given input file for reading

	if (fp == NULL) { //making sure the default input file opens correctly
		printf("Error opening the default input file Raw_data_01.txt");
		//return 0;
		//terminates program
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

Signal::Signal(const char* file_name){
	FILE* fp = fopen(file_name, "r");
	//opens the given input file for reading

	if (fp == NULL) { //making sure the default input file opens correctly
		printf("Error opening the default input file Raw_data_01.txt");
		//return 0;
		//terminates program
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
	cout << "\nLength: " << length << endl << "\nMax: " << max << endl << "Average" << mean << endl; //prints out signal info
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
	
	/*
	int* length = malloc(sizeof(int));
	int* max_val = malloc(sizeof(int));
	*/
	
	//defines all of our file name strings
	char file_name[66];
	char new_file_name[66];
	
	/*
	char stat_file[73];
	char offset_file[69];
	char scaled_file[69];
	char center_file[72];
	char normal_file[73];
	char temp_file_name[16];
	*/
	
	//defines the counter and all the flags for error checking
	int i=1;
	int n_flag=0;
	int r_flag=0;
	
	Signal sig1;
	
	double offset_val,scale_val;//vlaues to store scale and offsets
	char* ptr_1;//empty pointer needed for strtod to work
	char* new_name;//pointer to hold the new file name from user if needed.
	
	
if (argc==3){
	
		if(argv[1][0]=='-' && argv[1][1]=='n'){
			if(isNumeric(argv[2])==0){//checks to make sure the value given is a number
				n_flag=-1;
				//goto print_outs;//if the user doesn't give a file number no action can be taken so we go to help screen and print out message
			}
			else{
				file_sel=atoi(argv[2]);//happens if user gives a valid input
				n_flag=1;
				i++;
			}
		}
		else if(argv[1][0]=='-' && argv[1][1]=='f'){
				i++;
				if(1<argc){//makes sure that there is something after the "-n". Failure to check for this could cause Segmentation Faults
				
				char new_name[66];
				sprintf(new_name,"%s",argv[i]);
				r_flag=1;
				}
				else
					r_flag=-1;
		}
		else	
			printf("in the argc==3 part");
			
	
	}


else{
	int choice=0;
	int choice_input_type=0;
	
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
		printf("before build\n");
		sig1.builder();
		printf("after build\n");
	}
	printf("after loop\n");
	while(choice!=8){
		cout << endl << "Please select an option"<<endl<<"(1):offset signal\n(2):scale signal\n(3):center signal\n(4):normalize signal\n(5):Display Statistics\n(6):Print Signal\n(7):Save Signal\n(8):Exit\n";
		cin>>choice;
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
				sig1.stats();
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
	
	

	
}
/*
	//defines strings for our data 
	int* array;
	double* array_changed;
	
	if(n_flag!=1){//makes sure that the user gives a file number
		goto print_outs;
	}
	
	if(r_flag!=1){
	//this is run if no filename was selected to be renamed
	//actually creates the file names to be used
		if (file_sel < 10) {
		sprintf(file_name, "Raw_data_0%d.txt", file_sel);
		
		} 
		else {
		sprintf(file_name, "Raw_data_%d.txt", file_sel);
		
		
		}
	}
	else{
	//this is run if a new filename was selected	
		
		//creates the new file names to be used by the program
		sprintf(file_name, "%s.txt", new_name);
	
	}//end of else for the rename part

	//loads the array from the input file
	array = load_array(file_name, length, max_val);

	
	
print_outs:	
	
	//frees the memory we allocated for our strings. 
	free(array);
	
	//Prints out error message if certain user errors were made
	if(n_flag!=1)
		printf("\nProgram did not receive a a file number to read from\n");
	if(o_flag==-1)
		printf("\nProgram did not receive a valid offset value from the user\nProgram was not able to perform this function\n");
	if(s_flag==-1)
		printf("\nProgram did not receive a valid scale value from the user\nProgram was not able to perform this function\n");
	if(r_flag==-1)
		printf("\nProgram could not find a new name to name the files\nAll other commands were executed using the default file name\n");
	if(r_flag==-1||s_flag==-1||o_flag==-1||n_flag!=1){
	printf("\n\n\nSince you had some trouble with the formatting we thought you might like some help\n\nYou have the following options to put in your command line:\n-n <File Number>	Selects the file number which you would like to open\n-o <Offset Value>	Lets you instruct the program to offset the data and lets you offset it by the value you choose\n-s <Scale Factor>	Lets you instruct the program to scale the data and lets you scale it by the value you choose\n-S	Lets you instruct the program to get the statistics from the data\n-C	Lets you instruct the program to Center the data points\n-N	Lets you instruct the program to Normalize the data points\n-r <NewName>	Lets you create a copy of the input file you select and then formats the output files to this name\n-h	Lets you display the help menu\n\nThe following are example calls\n./a.exe -f 2 -o 43 -S\n./a.exe -f 2 -s -2.5 -C\n./a.exe -f 11 -r NewName1 -N\n\nEven though you had some incorrect formatting we executed as many commands as possible\n");	
	}
	*/
	return 0;

} //end of main
/*
int *load_array(char* file, int* length, int* max_val) {
	//function to load the data from the input file in the array
	FILE* fp = fopen(file, "r");
	//opens the given input file for reading

	if (fp == NULL) { //making sure the input file opens correctly
		printf("Error opening input file");
		return 0;
		//terminates program
	}

	fscanf(fp, "%d %d", length, max_val);

	int* array = malloc(sizeof(int) * *length);
	//mallocs space for the array

	int i = 0;
	for (i = 0; i < *length; i++) {
		//for loop to put the values into the array
		fscanf(fp, "%d", (array + i));
	}
	fclose(fp);
	return array;
}
*/

int isNumeric (const char * s)//determines if a given string is a numeric value or not, returns 0 if it is not and non-0 value if it is a numeric value.
{
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}
