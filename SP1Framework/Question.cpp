#include "Question.h"


Question::Question() : question(""), answer(0), var1(0), var2(0), var3(0), name1(""), name2("")
{
	choices[0] = 0;
	choices[1] = 0;
	choices[2] = 0;
}

void Question::SetNewQns()
{	//setting of variables required for question

	//setting question type
	int a = rand() % 5; //randomly chooses btw + - x %
	switch (a)
	{
	case 0:
		type = ADDITION;
		break;
	case 1:
		type = SUBTRACTION;
		break;
	case 2:
		type = MULTIPLICATION;
		break;
	case 3:
		type = DIVISION;
		break;
	case 4:
		type = AVERAGE;
		break;
	}
	
	//setting of values
	switch (type)
	{
	case ADDITION:
		var2 = (rand() % 1000) + 1;
		var1 = (rand() % 1000) + 1;
		break;
	case SUBTRACTION:
		var2 = (rand() % 1000) + 1;
		var1 = (rand() % var2) + 1;
		break;
	case MULTIPLICATION:
		var1 = (rand() % 11) + 1;
		var2 = (rand() % 11) + 1;
		break;
	case DIVISION:
		var1 = (rand() % 11) + 2;
		var2 = var1 * ((rand() % 10) + 2);
		break;
	case AVERAGE:
		do
		{
			var3 = (rand() % 10) + 2;
			var1 = (rand() % 50) + 1;
			var2 = ((rand() % 300) + 1) * var3 - var1;
		} while (var2 < 0);
		break;
	}

	//setting of names
	setName();
	
	//setting of question using generated variables
	setQuestion();

	//setting of answers
	setAnswer();

	//setting of wrong answers
	setChoices();


}

void Question::setName()
{
	int who = rand() % 10;
	switch (who)
	{
	case 0:
		name1 = "Jerry";
		name2 = "Tom";
		break;
	case 1:
		name1 = "Renee";
		name2 = "Tiffany";
		break;
	case 2:
		name1 = "Nicole";
		name2 = "JingJie";
		break;
	case 3:
		name1 = "Bob";
		name2 = "Alex";
		break;
	case 4:
		name1 = "Ryan";
		name2 = "Jun";
		break;
	case 5:
		name1 = "Louis";
		name2 = "Richard";
		break;
	case 6:
		name1 = "Wylie";
		name2 = "Ali";
		break;
	case 7:
		name1 = "Aaron";
		name2 = "Eugene";
		break;
	case 8:
		name1 = "Camelia";
		name2 = "Luke";
		break;
	case 9:
		name1 = "Rose";
		name2 = "Mary";
		break;
	}

}

void Question::setQuestion()
{
	int context = rand() % 4;
	switch (type)
	{
	case ADDITION:
		switch (context)
		{
		case 0:
			question = "Stuck at home during the covid-19 pandemic, " + name1 + " decides to order GrabFood. His/Her chicken rice costs $" + std::to_string(var1) + " and his/her delivery fee is $" + std::to_string(var2) + ". How much does " + name1 + " pay in total?";
			break;
		case 1:
			question = name1 + " has not left the house for " + std::to_string(var1) + " days. " + name1 + " is notified by his/her school that he/she is not to return to school for the next " + std::to_string(var2) + " days. By the time he/she goes back to school, what will his/her new did-not-leave-the-house streak be?";
			break;
		case 2:
			question = std::to_string(var1) + " people died from the corona virus in Singapore. " + std::to_string(var2) + " people died from the corona virus in Malaysia. How many people did the corona virus take the lives of in both countries?";
			break;
		case 3:
			question = name1 + " has " + std::to_string(var2) + " rolls of toilet paper. If " + name2 + " gives " + name1 + " " + std::to_string(var1) + " rolls of toilet paper, How many rolls of toilet paper will " + name1 + " have in total?";
			break;
		}
		break;
	case SUBTRACTION:
		switch (context)
		{
		case 0:
			question = name1 + " has " + std::to_string(var2) + " rolls of toilet paper. If " + name1 + " gives " + name2 + " " + std::to_string(var1) + " rolls of toilet paper, How many rolls of toilet paper will " + name1 + " have left?";
			break;
		case 1:
			question = name1 + "'s mask can be washed " + std::to_string(var2) + " times. He/She has washed it " + std::to_string(var1) + " times. How many more times can his/her mask be washed?";
			break;
		case 2:
			question = "Hospital A has a capacity of " + std::to_string(var2) + ". If " + std::to_string(var1) + " patients are currently being warded, how many more patients can the hospital take in?";
			break;
		case 3:
			question = name1 + " has $" + std::to_string(var2) + ". " + name1 + " wants to buy a bottle of bleach which costs $" + std::to_string(var1) + ". How much will he/she have left?";
			break;
		}
		break;
	case MULTIPLICATION:
		switch (context)
		{
		case 0:
			question = "Extinguishing 1 km^2 of forest fire takes " + std::to_string(var1) + " litres of water. How many litres of water will be needed to extinguish " + std::to_string(var2) + "km^2 of forest fire?";
			break;
		case 1:
			question = "If " + name1 + " uses " + std::to_string(var1) + " rolls of toilet paper every month, how many rolls of toilet paper should he/she buy to sustain herself/himself for " + std::to_string(var2) + " months?";
			break;
		case 2:
			question = "If " + std::to_string(var1) + " people are infected with the corona virus everyday. In " + std::to_string(var2) + " days, how many people will be infected with corona?";
			break;
		case 3:
			question = "If one mask costs $" + std::to_string(var1) + ", how much will " + std::to_string(var2) + " masks cost?";
			break;
		}
		break;
	case DIVISION:
		switch (context)
		{
		case 0:
			question = name1 + " bought a " + std::to_string(var2) + " ml bottle of hand sanitizer. If " + name1 + " uses " + std::to_string(var1) + " ml of hand sanitizer everyday, how many days will the hand sanitizer last?";
			break;
		case 1:
			question = "Due to social distancing rules, not all " + std::to_string(var2) + " students can return to school. If only one lab can be used and the lab can hold " + std::to_string(var1) + " students, how many groups will the students be divided into?";
			break;
		case 2:
			question = name1 + " earnt a profit of $" + std::to_string(var2) + " selling masks over a period of " + std::to_string(var1) + " days. How much did he/she earn on average per day?";
			break;
		case 3:
			question = "If the Australian bushfires killed a total of " + std::to_string(var2) + " people in " + std::to_string(var1) + " days, how many people died on average each day?";
			break;
		}
		break;
	case AVERAGE:
		switch (context)
		{
		case 0:
			question = "Due to social distancing rules, " + name1 + " went out alone to buy dinner for his/her " + std::to_string(var3) + " brothers. He/She spent $" + std::to_string(var1) + " on the bubble teas and $" + std::to_string(var2) + " on the burgers, 1 burger and bubble tea for each brother. To split the bill equally, how much should each brother pay?";
			break;
		case 1:
			question = name1 + " bought a packet of " + std::to_string(var1) + " masks from NTUC and another packet of " + std::to_string(var2) + " masks from Giant for his/her family of " + std::to_string(var3) + ". How many masks can each member use?";
			break;
		case 2:
			question = "If " + name1 + " has " + std::to_string(var1) + "litres of water. " + name2 + " has " + std::to_string(var2) + "litres of water. If there are " + std::to_string(var3) + " bushfires, How many litres of water can be used on each bushfire?";
			break;
		case 3:
			question = "If the number of people infected with the corona virus over the past " + std::to_string(var3) + " days in Singapore and Malaysia was " + std::to_string(var1) + " and " + std::to_string(var2) + " respectively, in total, how many people were infected on average each day in the two countries?";
			break;
		}
		break;
	}
}

void Question::setAnswer()
{
	switch (type)
	{
	case ADDITION:
		answer = var1 + var2;
		break;
	case SUBTRACTION:
		answer = var2 - var1;
		break;
	case MULTIPLICATION:
		answer = var1 * var2;
		break;
	case DIVISION:
		answer = var2 / var1;
		break;
	case AVERAGE:
		answer = (var1 + var2) / var3;
		break;
	}
}

void Question::setChoices()
{
	int wronganscombi = (rand() % 4) + 1;
	switch (wronganscombi)
	{
	case 1: 
		//biggest num
		if (answer >= 3)
		{
			choices[0] = answer - 1;
			choices[1] = answer - 2;
			choices[2] = answer - 3;
			break;
		}
	case 2: 
		//third
		if (answer >= 2)
		{
			choices[0] = answer - 1;
			choices[1] = answer - 2;
			choices[2] = answer + 1;
			break;
		}
	case 3: 
		//second
		if (answer >= 1)
		{
			choices[0] = answer - 1;
			choices[1] = answer + 1;
			choices[2] = answer + 2;
			break;
		}
	case 4:
		//smallest num
		choices[0] = answer + 1;
		choices[1] = answer + 2;
		choices[2] = answer + 3;
		break;
	}
}

std::string Question::get_question()
{
	return question;
}

int Question::get_answer()
{
	return answer;
}

int Question::get_choice(int index)
{
	return choices[index];
}