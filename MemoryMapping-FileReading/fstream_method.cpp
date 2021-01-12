//code taken from recitation 6 slides and modified
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
//using fstream to open file
int main() 
{
	string line;
	ifstream myfile ("loremipsum.txt");
    int count = 0; //initialize count of char a to 0
    
	while ( getline(myfile,line) ){
        for (int i=0;i<line.length();i++){ //traverse line
        if (line[i] == 'a')
            count++; //increment count when char is a
        }
        
    }
	
        
	myfile.close(); //close file
	cout << "In loremipsum.txt, ocurrence of character 'a' is " << count << endl; //print count of char a
	return 0;
}
