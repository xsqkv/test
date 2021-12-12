#include <iostream>

using namespace std;

string title = "Welcome to this test on the topic of the operator of electronic computers!";
string start = "START TEST";
string exit_ = "EXIT";

bool isAnswer(string text)
{
	return text[text.size()-1] == ' ';
}