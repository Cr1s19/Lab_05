#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <cmath>
#include <queue>
#include <map>

using namespace std; // std:: is implied.

struct node{
	string name;
    vector< struct node *> childs;
    string answer;
    int tabs;
};

//Get the classification of the examples in a set (column)
//Returns a vector of pairs: [ (none, 15), (soft, 5), (hard, 4) ]
vector< pair< string, int > >probability( vector<string> col)
{
    int count;
    string name;
    vector< pair< string, int > > prob;

    sort(col.begin(), col.end());
    name = col[1];
    count = 1;
    for( int i = 2; i < col.size(); i++ )
    {
        if( col[i] != name )
        {
            prob.push_back( make_pair(name, count) );
            name = col[i];
            count = 1;
        }   
        else
        {
            count++;
        }
    }
    prob.push_back( make_pair(name, count) );
    return prob;
}


//Get the number of examples by attibute given the target
//Returns a vector of pairs: [ (youngnone, 4), (youngsoft, 2), (younghard, 2), (pre-presbyopicnone, 5), (pre-presbyopicsoft, 2), (pre-presbyopichard, 1), ...  ]
vector< pair< string, int > > subsetProbability( vector<string> predictor, vector<string> target )
{
    int count;
    string actual;
    vector<string> key;
    vector< pair< string, int > > subsetProb;

    for( int i = 1; i < predictor.size(); i++ )
    {
        key.push_back( predictor[i] + target[i] );                
    }
    sort( key.begin(), key.end() );

    actual = key[0];
    count = 1;
    for(int i = 1; i < key.size(); i++ )
    {
        if( actual != key[i] )
        {
            subsetProb.push_back( make_pair(actual, count) );
            actual = key[i];
            count = 1;
        }
        else
        {
            count++;
        }
    }
    subsetProb.push_back( make_pair(actual, count) );

    return subsetProb;
}

float entropy( vector< pair< string, int > > probability)
{
    float result = 0.0;
    float total = 0.0;

    for(int i = 0; i < probability.size(); i++ )
    {
        total += probability[i].second;  
    }

    for(int i = 0; i < probability.size(); i++ )
    {
        result -= (probability[i].second / total ) * log2f( (probability[i].second / total ) );        
    }
    return result;
}

//Calcuate the subset entropy
//Uses the vectors returned by probability() and subProbability() to look for the attributes to count
float subsetEntropy( int total, vector< pair< string, int > > prob, vector< pair< string, int >  > subsetProb )
{
    string attribute;
    float result = 0;
    vector< pair< string, int > > valuesToEntropy;

    for( int i = 0; i < prob.size(); i++ )
    {
        attribute = prob[i].first;
        while(subsetProb.size() > 0)
        {
            if( subsetProb.front().first.substr(0, attribute.length()) == attribute)
            {
                valuesToEntropy.push_back( make_pair(attribute, subsetProb.front().second) );    
                subsetProb.erase( subsetProb.begin() );
            }
            else
            {
                break;
            }
        }
        result += ( prob[i].second / (float)total ) * entropy( valuesToEntropy );
        valuesToEntropy.clear();
    }
    return result;
}

float calculateGain( float targetEntropy, float subEntropy )
{
    return targetEntropy - subEntropy;
}

//Get the gain for each attribute and choose the one with the max gain
//Returns the index of the attribute to split on
int chooseAttribute( vector< vector< string > > dataSet )
{
    float targetEntropy;
    float subEntropy;
    float gain, maxGain;
    int choosenIndex;

    maxGain = 0;
    targetEntropy = entropy( probability( dataSet.back() ) );

    for(int  i = 0; i < dataSet.size() - 1; i++)
    {
        ///agregar validacion para pure set
        subEntropy = subsetEntropy(dataSet[0].size(), probability( dataSet[i] ),  subsetProbability( dataSet[i], dataSet.back() ));
        gain = calculateGain(targetEntropy, subEntropy);

        if(gain > maxGain)
        {
            maxGain = gain;
            choosenIndex = i;
        }
    }
    return choosenIndex;
}

vector< vector< vector<  string > > > splitOn( int splitOn, vector <string> attributes, vector< vector< string > > setToSplit )
{
    int numSubSet;
    int row, size;
    vector< vector< vector<  string > > > subSets;

    for(int numSubSet = 0; numSubSet < attributes.size(); numSubSet++)
    {
        //Create a copy of the original set for each attribute
        subSets.push_back( setToSplit );

        row = 1;
        size = subSets[numSubSet][splitOn].size();   

        //cout << "SPLIT ON: " << attributes[numSubSet] << endl;
        //cout << "-----------------" << endl;
        while(row < size)
        {
            //Delete the rows that dont match with the attribute
            if(subSets[numSubSet][splitOn][row] != attributes[numSubSet])
            {
                for(int col = 0; col < subSets[numSubSet].size(); col++)
                {
                    subSets[numSubSet][col].erase( subSets[numSubSet][col].begin() + row );
                }
                size--;
            }
            else
            {
                row++;

                //Print the new set with format
                /*for(int col = 0; col < subSets[numSubSet].size(); col++)
                {
                    if(col != splitOn)
                        cout << subSets[numSubSet][col][row] << "  ";
                }
                cout << endl;*/
            }
        }
        //cout << endl;

        //Delete the column of the attribute splitted
        subSets[numSubSet].erase( subSets[numSubSet].begin() + splitOn );
    }
    return subSets;
}


int a;
struct node *n;
map<string, vector<string>>::iterator attr_it;
vector< vector< vector< string > > > subSets;

struct node* createTree( map<string, vector<string>> attributes,  vector< vector< string > > dataSet, int tabs )
{
    a = chooseAttribute( dataSet );


   if(entropy( probability( dataSet.back() ) ) != 0 )
    {       
        a = chooseAttribute( dataSet );    
        attr_it = attributes.find( dataSet[a][0] );
        subSets = splitOn(a,  attr_it->second, dataSet); 
        n = new node;
        n->name = dataSet[a][0];

        for(int i = 0; i < subSets.size(); i++)
        {
            for(int j = 0; j < tabs; j++)
            {
                cout << " ";
            } 
            cout << "i = " << i << "size = " << subSets.size() << " " << attr_it->first << endl;
            cout << n->name << ": ";
            cout << attr_it->second[i] << endl;
            n->childs.push_back( createTree( attributes, subSets[i], tabs+2 ) );
        }
    }
    else
    {
        n->answer = dataSet[ dataSet.size() - 1 ][1];
        for(int i = 0; i < tabs; i++)
        {
            cout << " ";
        }        
        cout << "ANSWER: " << n->answer << endl;
    }
    return n;
}


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

int main(int arg, char** argv)
{
	vector<string> v, v1, v2;
	string relation;
	string s;

    int i;
    bool dataCreated = 0;
    vector<string> names, aux_attr, aux_data;
    map< string, vector< string > > attributes;
    vector< vector< string > > data;

	while(getline(cin, s))
    {
        if(s[0] != '%' && !s.empty())
        {
			//read only @
			if(s[0] == '@')
            {
				//lecture of relation
				if (s[1] == 'r')
                {
					//saving the relation in a string
					split(s,' ',v);
					string relation = v[v.size()-1];
					// cout<< relation << "\n";
					v.clear();
				}
				//lecture of attribute
				if (s[1] == 'a')
                {
					// split the the string in name and attributes
					split(s,'{',v);
					//cout << v[0]<< "\n";

					//split the @attribute and name
					split(v[0],' ',v1);
                    v1[1].erase( remove(v1[1].begin(), v1[1].end(), ' ') , v1[1].end() );
                    v1[1].erase( remove(v1[1].begin(), v1[1].end(), '\t'), v1[1].end() );
                    names.push_back(v1[1]);
					//cout << "name -" << v1[1] << "-\n";

					//split the attributes by a ,
					split(v[1],',',v2);
                    for(i = 0; i < v2.size(); i++)
                    {
                        if(v2[i].back() == '}')
                        {
                            v2[i].pop_back();
                        }
                        aux_attr.push_back(v2[i]);
                    }
                    attributes.emplace(  names.back(), aux_attr );
                    aux_attr.clear();
				}
			}
            else
            {
			    //lectura de @data
                if(data.size() == 0)
                {
                    for(int w = 0; w < names.size(); w++)
                    {
                        aux_data.push_back(names[w]);
                        data.push_back(aux_data);
                        aux_data.clear();
                    }
                }
                split(s,',',v);
                for(int w = 0; w < v.size(); w++)
                {
                    data[w].push_back(v[w]);
                }
            }
			//clear the vector to recive the next attributes correctly
			v.clear();
			v1.clear();
			v2.clear();
		}
	}


    /*******/
/*    vector< pair< string, int > > test;   
    vector< pair< string, int > > subTest;   
    test = probability(data[3]);
    subTest = subsetProbability(data[3], data[4]);

    int a = chooseAttribute(data);
    cout << a << endl;
    splitOn(a, attributes[a], data);
*/

    struct node *b;
    if(data.size() > 0)
    {
        b = createTree(attributes, data, 0);
    }
    else
    {
        cout << "Error: Empty data set"<< endl;
    }
   // printTree(b);
    
}
