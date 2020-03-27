#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
int main() {
	string str;
	stringstream buffer;
	ifstream f;
	f.open("output.txt");
	getline(f, str);
	buffer << str;
	f.close();
	std::string filename = "/home/pi/releas/build/http.py";
	std::string command = "python ";
	command += filename + " " + buffer.str();
	cout<<"command :"<< buffer.str() <<endl;
	system(command.c_str());
}
