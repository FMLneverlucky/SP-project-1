#pragma once
#include <string>

enum QNSTYPE
{
	ADDITION,
	SUBTRACTION,
	DIVISION,
	MULTIPLICATION,
	AVERAGE
};


class Question
{
private:
	std::string question;
	QNSTYPE type;
	int answer;
	int choices[3];
	unsigned int var1;
	unsigned int var2; //the bigger no.
	unsigned int var3;
	std::string name1;
	std::string name2;

public:
	Question();
	void SetNewQns();

	std::string get_question();
	int get_answer();
	int get_choice(int index);

	void setName();
	void setQuestion();
	void setAnswer();
	void setChoices();


};

