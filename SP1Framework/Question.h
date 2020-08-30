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
	std::string question; //question
	QNSTYPE type; //question type
	int answer; //answer to the question
	int choices[3]; //wrong answer choices
	int var1; //numbers thatll be used in question
	int var2; 
	int var3;
	std::string name1; //names thatll be used in question
	std::string name2;

public:
	Question();
	void SetNewQns(); //generates new question 

	std::string get_question(); 
	int get_answer();
	int get_choice(int index);

	void setName(); //generates random names
	void setQuestion(); //generates question context
	void setAnswer(); //calculates answer to the question
	void setChoices(); //generates wrong answer choices


};

