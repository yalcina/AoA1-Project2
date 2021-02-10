#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>

using namespace std;


const int LIST_CAPACITY = 891;  //  890 employees in total  +  1  initial unused object to ease index calculations in Heapsort 


//--------------------------  EMPLOYEE  CLASS  --------------------------
class  Employee {
  public:
	int ID;			  //	Employee ID
	int total_calls;  //    total number of calls
	int positive;  	  //    number of positive feedback
	int negative;  	  //    number of negative feedback
	int score;   	  //    = 2*total_calls + positive - negative
};



//-------------------------  CALL_CENTER CLASS  -------------------------
class  Call_Center {
	Employee  employee_List_by_calls[LIST_CAPACITY];    // an array of 890 Employee objects plus 1 redundant/dummy object(index:0)
	Employee  employee_List_by_score[LIST_CAPACITY];    // an array of 890 Employee objects plus 1 redundant/dummy object(index:0)
	int heapsize;
	int listsize;
	
  public:
	Call_Center() : heapsize(0), listsize(0){}
	
	void  read_from_CSV(int);
	void  split_string( const char *, int& , int& , int& , int& );
	int   search_employee( int, char);
	void  print_list_by_calls();
	void  print_list_by_score();

	void  max_heapify_by_calls(int);
	void  build_max_heap_by_calls();
	void  heapsort_by_calls();
	void  insert_by_calls(const Employee&);
	//Employee  extract_max_by_calls();
	void  increase_key_by_calls(int, const Employee &);

	void  max_heapify_by_score(int);
	void  build_max_heap_by_score();
	void  heapsort_by_score();
	void  insert_by_score(const Employee&);
	//Employee  extract_max_by_score();
	void  increase_key_by_score(int, const Employee &);

	void  print_the_bests_and_worsts_daily(int);
	void  FIND_THE_MOSTS();
};



//-----------------------------------------------------------------------
void  Call_Center::print_the_bests_and_worsts_daily(int day){

	if( day < 1  ||  10 < day ){ cout << "Day number provided is wrong..!!!\n\n";  return; }
	
	cout  <<  "\n AFTER DAY-" << day << endl;
	
	cout  <<  " BEST PERFORMANCE :   "  <<  employee_List_by_score[listsize].ID  << ", " <<  employee_List_by_score[listsize-1].ID  << ", " <<  employee_List_by_score[listsize-2].ID 	<< endl;
	cout  <<  " WORST PERFORMANCE :  "  <<  employee_List_by_score[1].ID  	    << ", " <<  employee_List_by_score[2].ID  		   << ", " <<  employee_List_by_score[3].ID 			<< endl;
	
	cout  <<  " MAXIMUM CALLS :      "     <<  employee_List_by_calls[listsize].ID  << ", " <<  employee_List_by_calls[listsize-1].ID  << ", " <<  employee_List_by_calls[listsize-2].ID << endl;
	cout  <<  " MINIMUM CALLS :      "     <<  employee_List_by_calls[1].ID  	   << ", " <<  employee_List_by_calls[2].ID  		  << ", " <<  employee_List_by_calls[3].ID 			<< endl;
}



//-----------------------------------------------------------------------
void  Call_Center::FIND_THE_MOSTS(){

	cout << "\n\nPress enter to print the figures: ";
	getchar();
	
	for(int i=1; i <= 10; i++){

		read_from_CSV(i);

		heapsort_by_calls();
		heapsort_by_score();
		
		print_the_bests_and_worsts_daily(i);

		if( i == 10 ){ break; }
		
		build_max_heap_by_calls();
		build_max_heap_by_score();

		getchar();
	}
}



//-----------------------------------------------------------------------
void  Call_Center::insert_by_calls( const Employee& key ){
	Employee *ptr = employee_List_by_calls;
	
	if( LIST_CAPACITY-1 <= listsize ){
		cout << "\nERROR:    List Overflow  -->  insert call tried to insert too many employees..!!!" << endl << endl;
		return;
	}
	
	heapsize++;
	listsize++;

	ptr[heapsize].ID = -100;
	ptr[heapsize].total_calls = -100;
	ptr[heapsize].positive = -100;
	ptr[heapsize].negative = -100;
	ptr[heapsize].score = -100;
	
	increase_key_by_calls(heapsize, key);
}


//-----------------------------------------------------------------------
void  Call_Center::insert_by_score(const Employee& key){

	Employee *ptr = employee_List_by_score;
	
	if( LIST_CAPACITY-1 == listsize ){
		cout << "\nERROR:   List Overflow  -->  insert call tried to insert too many employees..!!!" << endl << endl;
		return;
	}
	
	ptr[heapsize].ID  =  -100;
	ptr[heapsize].total_calls =  -100;
	ptr[heapsize].positive =  -100;
	ptr[heapsize].negative =  -100;
	ptr[heapsize].score =  -100;
	
	increase_key_by_score(heapsize, key);
}



//-----------------------------------------------------------------------
void  Call_Center::increase_key_by_calls(int ind, const Employee& key){

	Employee *ptr = employee_List_by_calls;
	Employee  temp;

	if( key.total_calls  <  ptr[ind].total_calls ){
		cout << "ERROR:  New key is smaller than the current key..!!!"  << endl << endl;
		return;
	}
	
	ptr[ind] = key;
	
	while (ind > 1  &&  ptr[ind/2].total_calls < ptr[ind].total_calls){
		temp = ptr[ind];
		ptr[ind] = ptr[ind/2];
		ptr[ind/2] = temp;
		
		ind = ind/2;
	}
}



void  Call_Center::increase_key_by_score(int ind, const Employee & key){

	Employee *ptr = employee_List_by_score;
	Employee  temp;

	if( key.score  <  ptr[ind].score ){
		cout << "ERROR:  New key is smaller than the current key..!!!"  << endl << endl;
		return;
	}
	
	ptr[ind] = key;
	
	while ( ind > 1  &&   ptr[ind/2].score  <  ptr[ind].score ){
		temp = ptr[ind];
		ptr[ind] = ptr[ind/2];
		ptr[ind/2] = temp;
		
		ind = ind/2;
	}
}




//-----------------------------------------------------------------------
void  Call_Center::heapsort_by_calls(){

	build_max_heap_by_calls();

	Employee *ptr = employee_List_by_calls;
	Employee  temp;
	
	for( int j = listsize;  j >= 2;  j-- ){
		temp = ptr[1];
		ptr[1] = ptr[j];
		ptr[j] = temp;
		
		heapsize--;
		max_heapify_by_calls(1);
	}
}


void  Call_Center::heapsort_by_score(){
	build_max_heap_by_score();
	Employee *ptr = employee_List_by_score;
	Employee  temp;
	
	for( int j = listsize;  j >= 2;  j-- ){
		temp = ptr[1];
		ptr[1] = ptr[j];
		ptr[j] = temp;
		
		heapsize--;
		max_heapify_by_score(1);
	}
}


//-----------------------------------------------------------------------
void  Call_Center::build_max_heap_by_calls(){

	heapsize = listsize;
	
	for( int j = listsize/2;  j >= 1;  j-- ){
		max_heapify_by_calls(j);
	}
}



void  Call_Center::build_max_heap_by_score(){

	heapsize = listsize;
	
	for( int j = listsize/2;  j >= 1;  j-- ){
		max_heapify_by_score(j);
	}
}


//-----------------------------------------------------------------------
void  Call_Center::max_heapify_by_calls(int ind){

	int left = 2*ind;
	int right = 2*ind + 1;
	int largest = ind;
	
	Employee *ptr = employee_List_by_calls;
	Employee  temp;
	
	if( left <= heapsize  &&  ptr[left].total_calls > ptr[ind].total_calls ){
		largest = left;
	}
	else {
		largest = ind;
	}
	
	if( right <= heapsize  &&  ptr[right].total_calls > ptr[largest].total_calls ){
		largest = right;
	}

	if( largest != ind ){
		temp = ptr[ind];
		ptr[ind] = ptr[largest];
		ptr[largest] = temp;
		
		max_heapify_by_calls(largest);
	}
}



void  Call_Center::max_heapify_by_score(int ind){
	int left = 2*ind;
	int right = 2*ind + 1;
	int largest = ind;
	
	Employee *ptr = employee_List_by_score;
	Employee  temp;
	
	if( left <= heapsize  &&  ptr[left].score > ptr[ind].score ){
		largest = left;
	}
	else {
		largest = ind;
	}
	
	if( right <= heapsize  &&  ptr[right].score > ptr[largest].score ){
		largest = right;
	}

	if( largest != ind ){
		temp = ptr[ind];
		ptr[ind] = ptr[largest];
		ptr[largest] = temp;
		
		max_heapify_by_score(largest);
	}
}




//-----------------------------------------------------------------------
int  Call_Center::search_employee( int id, char which_list){
	int trv;
	
	if( listsize == 0 )  return -2; 			 //  No employees in the list to be searched for
	
	if( which_list == 'c' ){  					//  if employee_List_by_calls list is to be searched
		for( trv = 1; trv <= listsize; trv++ ){
			if( employee_List_by_calls[trv].ID == id )  return trv;   // if found, return the INDEX of the employee searched for
		}
		return -1; 			 // ID is not found in the list
	}
	
	else if( which_list == 's' ){  				//  if employee_List_by_score list is to be searched
		for( trv = 1; trv <= listsize; trv++ ){
			if( employee_List_by_score[trv].ID == id )  return trv;   // if found, return the INDEX of the employee searched for
		}
		return -1;  		// ID is not found in the list
	}

	cout << "You entered the wrong criterion, choose either of these: c / s" << endl << endl;
	
	return -3;  // Invalid entry
}




//-----------------------------------------------------------------------
void Call_Center::split_string( const char *line, int& in_id, int& in_calls, int& in_pos, int& in_neg ){
	char temp[4] = {0};
	
	for(int i = 0; i < 3; i++){
			temp[i] = line[i];
		}
		in_id = atoi(temp);
		
		temp[0] = '\0';
		temp[1] = '\0';
		temp[2] = '\0';
		temp[3] = '\0';

		int j;
		for(j=0; j < 2; j++){
			if( line[j+4] == ',' )  break;
			temp[j] = line[j+4];
		}
		in_calls = atoi(temp);
		
		temp[0] = '\0';
		temp[1] = '\0';
		temp[2] = '\0';
		temp[3] = '\0';
		
		j++;
		int ind=0;
		while(1){
			if( line[j+4] == ',' )  break;
			temp[ind] = line[j+4];
			j++;
			ind++;
		}
		in_pos = atoi(temp);
		
		temp[0] = '\0';
		temp[1] = '\0';
		temp[2] = '\0';
		temp[3] = '\0';
		
		j++;
		ind=0;
		while(1){
			if( line[j+4] == '\0' )  break;
			temp[ind] = line[j+4];
			j++;
			ind++;
		}
		in_neg = atoi(temp);
}





//-----------------------------------------------------------------------
void  Call_Center::read_from_CSV(int file_no){

	char temp[4] = {0};
	int id, calls, pos, neg;
	
	string single_line;
	char *char_line;
	char filename[18] = {0};

	if (1 <= file_no && file_no <= 10){
		sprintf (filename, "dataset/day%d.csv", file_no);
	}
	else{
		cout << "\n\nFile number specified is WRONG..!!!\n\n";
		return;
	}

	ifstream CSV_File(filename);
	
	for(int k = 0; k < 800; k++){
		getline(CSV_File, single_line, '\n');
		char_line = new char [single_line.size()+1];
		strcpy(char_line, &single_line[0]);
		
		split_string(char_line, id, calls, pos, neg);

		if( file_no == 1 ){
			heapsize++;
			listsize++;

			employee_List_by_calls[listsize].ID = id;
			employee_List_by_score[listsize].ID = id;
			
			employee_List_by_calls[listsize].total_calls = calls;
			employee_List_by_score[listsize].total_calls = calls;
			
			employee_List_by_calls[listsize].positive = pos;
			employee_List_by_score[listsize].positive = pos;
			
			employee_List_by_calls[listsize].negative = neg;
			employee_List_by_score[listsize].negative = neg;
			
			employee_List_by_calls[listsize].score = 2*calls + pos - neg;
			employee_List_by_score[listsize].score = 2*calls + pos - neg;
		}
		
		
		else if( file_no > 1  &&  file_no <= 10 ){
			int index_1 = search_employee( id, 'c');
			
			if(  index_1 > listsize ){
				cout << "\n\nThere is a problem with the INDEX found:  (index > listsize)\n";
				cout << "Fetching from  day" << file_no << ".csv  is terminated..!!\n\n";
				return;
			}
			
			if( index_1 > 0){
				
				Employee  arg1;
				
				arg1.ID = id;
				arg1.total_calls = calls + employee_List_by_calls[index_1].total_calls;
				arg1.positive = pos + employee_List_by_calls[index_1].positive;
				arg1.negative = neg + employee_List_by_calls[index_1].negative;
				arg1.score = 2*calls + pos - neg + employee_List_by_calls[index_1].score;

				increase_key_by_calls(index_1, arg1);
			}
			
			else if( index_1 == -1 ){
				Employee  arg2;
				
				arg2.ID = id;
				arg2.total_calls = calls;
				arg2.positive = pos;
				arg2.negative = neg;
				arg2.score = 2*calls + pos - neg;
				
				insert_by_calls(arg2);
			}
			
			else if ( index_1 == -2 ){
				cout << "\nLIST is empty..!!!\n\n";
			}

			int index_2 = search_employee( id, 's');
			
			if(  index_2 > listsize ){
				cout << "\n\nThere is a problem with the INDEX found:  (index > listsize)\n";
				cout << "Fetching from  day" << file_no << ".csv  is terminated..!!\n\n";
				return;
			}
			
			if( index_2 > 0){

				Employee  arg3;
				
				arg3.ID = id;
				arg3.total_calls = calls + employee_List_by_score[index_2].total_calls;
				arg3.positive = pos + employee_List_by_score[index_2].positive;
				arg3.negative = neg + employee_List_by_score[index_2].negative;
				arg3.score = 2*calls + pos - neg + employee_List_by_score[index_2].score;
				
				increase_key_by_score(index_2, arg3);
			}
			
			else if( index_2 == -1 ){

				Employee  arg4;
				
				arg4.ID = id;
				arg4.total_calls = calls;
				arg4.positive = pos;
				arg4.negative = neg;
				arg4.score = 2*calls + pos - neg;
				
				insert_by_score(arg4);
			}
			
			else if ( index_2 == -2 ){
				cout << "\nLIST is empty..!!!\n\n";
			}
		}
	}
}




void  Call_Center::print_list_by_calls(){
	cout << "EMPLOYEE LIST  sorted by Number of Calls" << endl << endl;
	
	for( int i = 1; i <= listsize; i++ ){
		cout << " [" << i << ". INDEX]\t-->\tID: " <<  employee_List_by_calls[i].ID  <<  "\tcalls: " << employee_List_by_calls[i].total_calls << "\tpositive: "  << employee_List_by_calls[i].positive     << "\tnegative: "  << employee_List_by_calls[i].negative << "\tscore: "  << employee_List_by_calls[i].score << endl;                        
	}
	
	cout << endl << endl << endl << endl;
}
void  Call_Center::print_list_by_score(){
	cout << "EMPLOYEE LIST  sorted by Score" << endl << endl;
	
	for( int i = 1; i <= listsize; i++ ){
		cout << " [" << i << ". INDEX]\t-->\tID: " <<  employee_List_by_score[i].ID  <<  "\tcalls: " << employee_List_by_score[i].total_calls << "\tpositive: "  << employee_List_by_score[i].positive     << "\tnegative: "  << employee_List_by_score[i].negative << "\tscore: "  << employee_List_by_score[i].score << endl;                        
	}
	
	cout << endl << endl << endl << endl;
}



int main(){

	Call_Center call_center;
	char p;

	call_center.FIND_THE_MOSTS();

	cout << "\nDo you want to sort the List by the number of calls and Score (Y/N): ";
	cin  >>  p;

	if( p == 'Y'  ||  p == 'y' ){
		call_center.print_list_by_calls();
		call_center.print_list_by_score();
	}

	getchar();
	return 0;
}