#include <iostream>
#include <vector>
#include <string>

using namespace std; // std:: is implied.

int main(int arg, char** argv)
{
	vector<string> v;
	string temp;
	string s;
	while(getline(cin, s)){
		if(!s.empty()){
			if(s[0] != '%'){
				if(s[0] == '@'){
				}
				cout << s << "\n";
			}
		}
	}
	return 0;
}