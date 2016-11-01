#include <iostream>
#include <vector>
#include <string>

using namespace std; // std:: is implied.

void split(string& s, char delim, vector<string>& v)
{
    int i = 0;
    char pos = s.find(delim);

    if(s.back() == ' ')
    {
        s.pop_back();
    }

    while (pos != string::npos) 
    {
        v.push_back(s.substr(i, pos-i));
        i = ++pos; 

        if(s[i] == ' '){
            i++;
        }
        pos = s.find(delim, pos);

        if (pos == string::npos)
        {
            v.push_back(s.substr(i, s.length()));
        }
    }
}

struct node{
	string name;
	vector<string> type;
};

int main(int arg, char** argv)
{
	vector<string> v, v1, v2, attributes;
	vector<vector<string>> data;
	string relation;
	string s;

	while(getline(cin, s)){
			if(s[0] != '%' && !s.empty()){
				//read only @
				if(s[0] == '@'){
					//lecture of relation
					if (s[1] == 'r'){
						//saving the relation in a string
						split(s,' ',v);
						string relation = v[v.size()-1];
						// cout<< relation << "\n";
						v.clear();

					}
					//lecture of attribute
					if (s[1] == 'a'){
						// split the the string in name and attributes
						split(s,'{',v);
						cout << v[0]<< "\n";

						//split the @attribute and name
						split(v[0],' ',v1);
						cout << v1[1]<< "\n";

						//split the attributes by a ,
						split(v[1],',',v2);
						cout << v2[v2.size()-1]<< "\n";

						//clear the vector to recive the next attributes correctly
						v.clear();
						v1.clear();
						v2.clear();
					}
				}
				//lectura de @data
				split(s,',',v);
				cout << v[0]<< "\n";
				v.clear();
				cout << s << "\n"; 
			}
	}
	return 0;
}