#include <iostream>
#include <vector>

using namespace std;

struct Task
{
    public:
    Task(string Question="",vector<string> Answers={},int RightAnswer=0)
    {
        question=Question;
        answers=Answers;
        rightAnswer=RightAnswer;
    }
    Task()
    {

    }
    string question;
    vector<string> answers;
    int rightAnswer;
};