//============================================================================
// Name        : homework3.cpp
// Author      : Daniel Matsunaga
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <unordered_map>
#include <list>
#include <algorithm>

using namespace std;

class query
{
private:
	string sent;
	string predicate;
	vector<string> arguments;

public:
	query(string _sent)
	{
		if(_sent[0] == '~')
		{
			_sent.erase(0, 1);
		}
		else
		{
			_sent.insert(0, "~");

		}
		sent = _sent;

		string x;
		vector<string> arg;

		for(unsigned int i = 0; i < sent.length(); i++)
		{
			if(sent[i] != '(' && sent[i] != ')' && sent[i] != ',' && sent[i] != ' ')
			{
				x += sent[i];
			}
			else
			{
				if(sent[i] == '(')
				{
					predicate = x;
					x.clear();
				}
				if(sent[i] == ')')
				{
					arg.push_back(x);
					arguments = arg;
					x.clear();
				}
				if(sent[i] == ',')
				{
					arg.push_back(x);
					x.clear();
				}
			}
		}
	}

	unsigned int argSize() const { return arguments.size(); }
	string arg(unsigned int i) const { return arguments[i]; }
	string pred() const { return predicate; }
	string line() const { return sent; }
};

class sentence
{
private:
	//literal - 1 = predicate, 2 = argument
	string sent;
	vector<string> predicates;
	vector<vector<string>> arguments;

public:
sentence(string _sent)
{
	sent = _sent;

	string x;
	int count = 0;
	int yes = 0;
	int size = 0;
	vector<string> pred;
	vector<vector<string>> arg;

	for(unsigned int i = 0; i < sent.length(); i++)
	{
		if(sent[i] == '&' || sent[i] == '>')
		{
			yes = 1;
		}

		if(sent[i] == '(')
		{
			size += 1;
		}
	}

	arg.resize(size);

	for(unsigned int i = 0; i < sent.length(); i++)
	{
		if(sent[i] != '(' && sent[i] != ')' && sent[i] != ',' && sent[i] != ' ' && sent[i] != '&' && sent[i] != '|')
		{
			x += sent[i];
		}
		else
		{
			if(yes == 0)
			{
				if(sent[i] == '(')
				{
					pred.push_back(x);
					x.clear();
				}
				if(sent[i] == ')')
				{
					arg[count].push_back(x);
					x.clear();
					count += 1;
				}
				if(sent[i] == ',')
				{
					arg[count].push_back(x);
					x.clear();
				}
			}
			else if(yes == 1)
			{
				if(x == "=>")
				{
					yes = 0;
					x.clear();
				}
				if(sent[i] == '(')
				{
					x.insert(0, "~");
					pred.push_back(x);
					x.clear();
				}
				if(sent[i] == ')')
				{
					arg[count].push_back(x);
					x.clear();
					count += 1;
				}
				if(sent[i] == ',')
				{
					arg[count].push_back(x);
					x.clear();
				}
			}
		}
	}
	predicates = pred;
	arguments = arg;
}

sentence(string p, vector<string> a)
{
	vector<string> pred;
	vector<vector<string>> arg;
	pred.push_back(p);
	predicates = pred;
	arg.resize(1);
	string s = p;
	s += "(";
	for(unsigned int i = 0; i < a.size(); i++)
	{
		if(i == a.size() - 1)
		{
			s = s + a[i] + ")";
		}
		else
		{
			s = s + a[i] + ",";
		}
		arg[0].push_back(a[i]);
	}
	sent = s;
	arguments = arg;
}

sentence(vector<string> pred, vector<vector<string>> arg)
{
	predicates = pred;
	arguments = arg;
	string x;

	for(unsigned int i = 0; i < pred.size(); i++)
	{
		x = x + pred[i] + "(";
		for(unsigned int j = 0; j < arg[i].size(); j++)
		{
			if(j == arg[i].size() - 1)
			{
				x = x + arg[i][j] + ")";
			}
			else
			{
				x = x + arg[i][j] + ",";
			}
		}
		if(i != pred.size() - 1)
		{
			x = x + " | ";
		}
	}

	sent = x;
}

unsigned int predSize() const { return predicates.size(); }
unsigned int argSize(unsigned int i) const {return arguments[i].size(); }
string pred(unsigned int i) const { return predicates[i]; }
string arg(unsigned int i, unsigned int j) const { return arguments[i][j]; }
vector<string> args(unsigned int i) { return arguments[i]; }
void setPred(unsigned int i, string x) { predicates[i] = x; }
void setArg(unsigned int i, unsigned int j, string x) { arguments[i][j] = x; }
string line() const {return sent; }
bool predEmpty() {
	if(predicates.empty())
	{
		return true ;
	}
	else
	{
		return false;
	}
}

};

vector<query> queries;
vector<sentence> kb;
int num = 1;

void getInput()
{
	string sent;
	int num = 0;

	ifstream input ("input.txt");
	if(input.is_open())
	{
		//Get Number of Queries
		getline(input, sent);
		while(sent[sent.length() - 1] == '\n' || sent[sent.length() - 1] == '\r')
		{
			sent.erase(sent.length() - 1);
		}
		num = stoi(sent);

		//Add all Queries to Query Library
		for(int i = 0; i < num; i++)
		{
			getline(input, sent);
			while(sent[sent.length() - 1] == '\n' || sent[sent.length() - 1] == '\r')
			{
				sent.erase(sent.length() - 1);
			}
			queries.push_back(query(sent));
		}

		//Get Number of Sentence in KB
		getline(input, sent);
		while(sent[sent.length() - 1] == '\n' || sent[sent.length() - 1] == '\r')
		{
			sent.erase(sent.length() - 1);
		}
		num = stoi(sent);

		//Add all Sentences to KB
		for(int i = 0; i < num; i++)
		{
			getline(input, sent);
			while(sent[sent.length() - 1] == '\n' || sent[sent.length() - 1] == '\r')
			{
				sent.erase(sent.length() - 1);
			}
			kb.push_back(sentence(sent));
		}

		input.close();
	}

	return;
}

void createOutput(vector<string> solns)
{
	ofstream output("output.txt");
	for(unsigned int i = 0; i < solns.size(); i++)
	{
		output << solns[i];
		if(i != solns.size() -1)
		{
			output << "\n";
		}
	}
	output.close();

	return;
}

vector<sentence> formatKB(vector<sentence> k)
{
	//Get rid of double negation
	string x;
	for(unsigned int i = 0; i < k.size(); i++)
	{
		for(unsigned int j = 0; j < k[i].predSize(); j++)
		{
			if(k[i].pred(j)[0] == '~' && k[i].pred(j)[1] == '~')
			{
				x = k[i].pred(j);
				x.erase(0, 2);
				k[i].setPred(j, x);
			}
		}
	}

	return k;
}

bool inKB(vector<sentence> knowbase, sentence in)
{

	unsigned int count = 0;
	for(unsigned int i = 0; i < knowbase.size(); i++)
	{
		//Check if a sentence has the same # of predicates as input sentence
		if(knowbase[i].predSize() == in.predSize())
		{
			count = 0;
			for(unsigned int j = 0; j < in.predSize(); j++)
			{
				for(unsigned int k = 0; k < knowbase[i].predSize(); k++)
				{
					//Check all the combinations of the predicates between the two sentences
					if(in.pred(j) == knowbase[i].pred(k))
					{
						for(unsigned int m = 0; m < in.argSize(j); m++)
						{
							//Check if the arguments are not equal => not in KB
							if((!islower(knowbase[i].arg(k, m)[0]) && !islower(in.arg(j, m)[0]) && in.arg(j, m) != knowbase[i].arg(k, m)) || (!islower(knowbase[i].arg(k, m)[0]) && islower(in.arg(j, m)[0])) || (islower(knowbase[i].arg(k, m)[0]) && !islower(in.arg(j, m)[0])) || (islower(knowbase[i].arg(k, m)[0]) && islower(in.arg(j, m)[0]) && in.arg(j, m)[0] != knowbase[i].arg(k, m)[0]))
							{
								break;
							}
							else if(m == in.argSize(j) - 1) //arguments are all the same
							{
								count += 1;
							}
						}
					}
				}
			}
		}
		if(count == in.predSize())
		{
			return true;
		}
	}
	return false;
}

sentence unification(sentence k1, sentence k2, unsigned int index1, unsigned int index2)
{
	//sentence k1's predicate size may not equal sentence k2's predicate size
	//however, both their argument sizes will be the same
	vector<string> preds;
	vector<vector<string>> args;
	unordered_map<string, string> vars1;
	unordered_map<string, string> vars2;
	string temp;
	int count = k1.predSize() + k2.predSize();

	//Check if there is a variable in only one of the arguments of the predicates
	for(unsigned int j = 0; j < k1.argSize(index1); j++)
	{
		if((islower(k1.arg(index1, j)[0]) && !islower(k2.arg(index2, j)[0])) /*|| (islower(k1.arg(index1, j)[0]) && islower(k2.arg(index2, j)[0]) && k1.arg(index1, j)[0] != k2.arg(index2, j)[0])*/)
		{
			if(vars1.find(k1.arg(index1, j)) == vars1.end()) //If the variable is not already in vars1
			{
				vars1[k1.arg(index1, j)] = k2.arg(index2, j); //Assign k1(variable) = k2(constant)
			}
			else if(vars1[k1.arg(index1, j)] != k2.arg(index2, j) && !islower(vars1[k1.arg(index1, j)][0]))//case of trying to unify A(x,x) and A(Alice,Max)
			{
				preds.push_back("INVALID");
				args.resize(1);
				return sentence(preds, args); //Exit unification early
			}
			else if(vars1[k1.arg(index1, j)] != k2.arg(index2, j) && islower(vars1[k1.arg(index1, j)][0]))
			{
				temp = vars1[k1.arg(index1, j)];
				vars1[k1.arg(index1, j)] = k2.arg(index2, j);

				for(pair<string, string> element: vars2)
				{
					if(element.second == temp)
					{
						vars2[element.first] = k2.arg(index2, j);
					}
				}
			}
		}
		else if(islower(k2.arg(index2, j)[0]) && !islower(k1.arg(index1, j)[0]))
		{
			if(vars2.find(k2.arg(index2, j)) == vars2.end())//If the variable is not already in vars2
			{
				vars2[k2.arg(index2, j)] = k1.arg(index1, j); //Assign k2(variable) = k1(constant)
			}
			else if(vars2[k2.arg(index2, j)] != k1.arg(index1, j) && !islower(vars2[k2.arg(index2, j)][0]))
			{
				preds.push_back("INVALID");
				args.resize(1);
				return sentence(preds, args); //Exit unification early
			}
			else if(vars2[k2.arg(index2, j)] != k1.arg(index1, j) && islower(vars2[k2.arg(index2, j)][0]))
			{
				temp = vars2[k2.arg(index2, j)];
				vars2[k2.arg(index2, j)] = k1.arg(index1, j);

				for(pair<string, string> element: vars1)
				{
					if(element.second == temp)
					{
						vars1[element.first] = k1.arg(index1, j);
					}
				}
			}
		}
		else if((k1.arg(index1, j) != k2.arg(index2, j)) && (!islower(k1.arg(index1, j)[0]) && !islower(k2.arg(index2, j)[0])))
		{
			//k1 and k2 have arguments that are not equal and are both constants, so they cannot be unified
			preds.push_back("INVALID");
			args.resize(1);
			return sentence(preds, args); //Exit unification early
		}
		else if((islower(k1.arg(index1, j)[0]) && islower(k2.arg(index2, j)[0]) && k1.arg(index1, j)[0] != k2.arg(index2, j)[0]))
		{
			if(vars1.find(k1.arg(index1, j)) == vars1.end() && vars2.find(k2.arg(index2, j)) == vars2.end())
			{
				temp = k2.arg(index2, j) + to_string(num);
				num += 1;
				vars2[k2.arg(index2, j)] = temp;
				vars1[k1.arg(index1, j)] = temp;
			}
			else if(vars1.find(k1.arg(index1, j)) == vars1.end() && vars2.find(k2.arg(index2, j)) != vars2.end())
			{
				vars1[k1.arg(index1, j)] = vars2[k2.arg(index2, j)];
			}
			else if(vars1.find(k1.arg(index1, j)) != vars1.end() && vars2.find(k2.arg(index2, j)) == vars2.end())
			{
				vars2[k2.arg(index2, j)] = vars1[k1.arg(index1, j)];
			}
			else if(vars1.find(k1.arg(index1, j)) != vars1.end() && vars2.find(k2.arg(index2, j)) != vars2.end())
			{
				preds.push_back("INVALID");
				args.resize(1);
				return sentence(preds, args); //Exit unification early
			}
		}

		if(j == k1.argSize(index1) - 1) //If vars1 && vars2 are both empty => pred + args are the exact same
		{
			count = count - 2; //Resize new sentence
		}
	}

	args.resize(count);
	count = 0;
	//Add the items in sentences k1 to the new sentence and unify the variables with the constants found in vars1
	for(unsigned int i = 0; i < k1.predSize(); i++)
	{
		if(i != index1)
		{
			preds.push_back(k1.pred(i));
			for(unsigned int j = 0; j < k1.argSize(i); j++)
			{
				if(vars1.find(k1.arg(i, j)) == vars1.end()) //If the variable from 2nd sentence isn't in the 1st sentence
				{
					args[count].push_back(k1.arg(i,j)); //add variable/constant from k1
				}
				else
				{
					args[count].push_back(vars1.find(k1.arg(i, j))->second); //add variable/constant from vars1
				}
			}
			count++;
		}
	}

	//Add the items in sentences k2 to the new sentence and unify the variables with the constants found in vars2
	for(unsigned int i = 0; i < k2.predSize(); i++)
	{
		if(i != index2)
		{
			preds.push_back(k2.pred(i));
			for(unsigned int j = 0; j < k2.argSize(i); j++)
			{
				if(vars2.find(k2.arg(i,j)) == vars2.end())
				{
					args[count].push_back(k2.arg(i,j));
				}
				else
				{
					args[count].push_back(vars2.find(k2.arg(i, j))->second);
				}
			}
			count++;
		}
	}

	//Delete Duplicates
	for(unsigned int w = 0; w < preds.size(); w++)
	{
		for(unsigned int i = w + 1; i < preds.size(); i++)
		{
			if(preds[w] == preds[i])
			{
				for(unsigned int j = 0; j < args[i].size(); j++)
				{
					if(args[w][j] != args[i][j])
					{
						break;
					}
					else if(j == args[i].size() - 1)
					{
						preds.erase(preds.begin() + i);
						args.erase(args.begin() + i);
					}
				}
			}
		}
	}

	return sentence(preds, args);
}

string resolution(query q)
{
	//reset kbase and add negated query to kbase
	list<int> visited;
	vector<sentence> kbase = kb;
	kbase.push_back(sentence(q.line()));
	kbase = formatKB(kbase);

	bool finished = false;
	bool found = false;
	unsigned int kbsize = kbase.size();
	string one, two;
	unsigned int pointer = 0;
	int number = kbase.size();
	int kbs = kbsize - 1;
	unsigned int noSolution = 0;


	for(unsigned int i = 0; i < kbase.size(); i++)
	{
		cout << "======= " << i << " =======" << endl;
		for(unsigned int j = 0; j < kbase[i].predSize(); j++)
		{
			cout << kbase[i].pred(j) << ": ";
			for(unsigned int w = 0; w < kbase[i].argSize(j); w++)
			{
				cout << kbase[i].arg(j, w) << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	int count = 0;

	while(!finished)
	{
		for(unsigned int x = 0; x < kbsize; x++)
		{
			if(kbase[x].predSize() == 1 && find(visited.begin(), visited.end(), x) == visited.end())
			{
				pointer = x;
				visited.push_back(x);
				break;
			}
		}
		unsigned int i = 0;
		count = 0;
		found = false;
		while(i < kbsize)
		{
			NEXT:
			if(found)
			{
				i += 1;
			}
			found = false;
			for(unsigned int j = 0; j < kbase[i].predSize(); j++) //iterating sentences kbase[i]
			{
				two = kbase[i].pred(j);
				for(unsigned int k = 0; k < kbase[pointer].predSize(); k++) //iterating sentences kbase[pointer]
				{
					if(i != pointer)
					{
						one = kbase[pointer].pred(k);
						if((one[0] == '~' && two[0] != '~' && one.substr(1, one.length()) == two) || (one[0] != '~' && two[0] == '~' && two.substr(1, two.length()) == one))
						{
							sentence u = unification(kbase[pointer], kbase[i], k, j);
							if(u.predEmpty())
							{
								finished = true;
								cout <<"**************************** TRUE ****************************" << endl;
								cout << "CONTRADITION: " << pointer << " & " << i << endl;
								return "TRUE";
							}
							else if(u.pred(0) != "INVALID" && !inKB(kbase, u))
							{
								kbase.push_back(u);
								found = true;
								cout << "======== " << number << " ======= " << "FROM: i=" << i <<  " & p=" << pointer << " ======= " << endl;
								count++;
								number++;
								for(unsigned int m = 0; m < u.predSize(); m++)
								{
									cout << u.pred(m) << ": ";
									for(unsigned int n = 0; n < u.argSize(m); n++)
									{
										cout << u.arg(m, n) << " ";
									}
									cout << endl;
								}
								goto NEXT;
							}
						}
					}

				}
			}
			i += 1;

			if(count > 0 && i == kbsize)
			{
				kbs += 1;
				pointer = kbs;
				count--;
				i = 0;
				found = false;
			}
		}
		kbsize = kbase.size();
		kbs = kbsize - 1;
		if(noSolution == kbsize)
		{
			noSolution -= 5;
		}
		else if(noSolution == kbsize - 5)
		{
			finished = true;
		}
		else
		{
			noSolution = kbsize;
		}
	}

	cout << "**************************** FALSE ****************************" << endl;
	return "FALSE";
}

void completeResolution()
{
	vector<string> solutions;
	for(unsigned int i = 0; i < queries.size(); i++)
	{
		solutions.push_back(resolution(queries[i]));
	}
	createOutput(solutions);
	return;
}

int main() {

	getInput();


	//Print Queries
	cout << "QUERIES: " << endl;
	for(unsigned int i = 0; i < queries.size(); i++)
	{
		cout << queries[i].pred() << ": ";
		for(unsigned int j = 0; j < queries[i].argSize(); j++)
		{
			cout << queries[i].arg(j) << " ";
		}
		cout << endl;
	}

	cout << endl;

	completeResolution();

	return 0;
}
