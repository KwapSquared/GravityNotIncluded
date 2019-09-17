#include "PCH.h"
#include "Debugging/message.h"

message::message(short t_Color, std::string t_text, std::string t_file, short t_line)
{
	m_TextColor = t_Color;
	m_Text = t_text;
	m_File = t_file;
	m_Line = t_line;

	this->addMessage(*this);
}

message::message() {}

short message::Priority()
{
	return m_TextColor;
}
void message::ToString()
{
	std::cout << m_File.c_str() << " : Line " << m_Line << ": \n\tMessage: " << m_Text.c_str() <<"\n" << std::endl;
}


//---------------MESSAGEMANAGER---------------------

void message::printMessages(short t_priority)
{

	for (int x = 0; x < index; x++)
	{
		//std::cout << "HELLO???" << std::endl;
		if (Messages[x].Priority() == t_priority || t_priority == 0)
		{
			FlushConsoleInputBuffer(hConsole);
			SetConsoleTextAttribute(hConsole, Messages[x].Priority());

			Messages[x].ToString();
		}
	}
	SetConsoleTextAttribute(hConsole, _LOG);
}

void message::addMessage(message &t_message)
{
	if (index < maxSize)
	{
		Messages[index] = t_message;
		index++;
	}
	else
	{
		std::cout << "MANAGER FULL" << std::endl;
	}

}

void message::clear()
{
	delete[] Messages;
	message *Messages = new message[maxSize];
	index = 0;
}