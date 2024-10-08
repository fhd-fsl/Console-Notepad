#include <iostream>
#include <Windows.h>
#include "Notepad.h"
#include "stack.h"

using namespace std;


int main(int argc, char* argv[]) {

	system("cls");
	HANDLE  rhnd = GetStdHandle(STD_INPUT_HANDLE);  // handle to read console

	DWORD Events = 0;     // Event count
	DWORD EventsRead = 0; // Events read from console

	CONSOLE_SCREEN_BUFFER_INFO screen;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen);//get info about console screen
	const int maxX = (screen.srWindow.Right + 1) * 0.2;  
	const int maxY = (screen.srWindow.Bottom + 1) * 0.2; //set boundaries for cursor

	stack stack;
	Notepad notepad(maxX, maxY, stack);
	

	bool Running = true;
	


	for (int a = 0; a <= maxY; a++)
	{
		cout << '|';
		int b;
		if (a == 0)
		{
			cout << "Notepad:";
			b = 8;
		}
		else
			b = 0;
		for (;b < maxX; b++)
		{
			cout << ' ';
		}
		cout << '|' << endl;
	}
	for (int b = 0; b < maxX + 2; b++)
	{
		cout << '_';
	}
	cout << endl;

	int x = 1, y = 1;
	gotoxy(x, y);
	//programs main loop
	while (Running) {

		

		// gets the systems current "event" count
		GetNumberOfConsoleInputEvents(rhnd, &Events);

		if (Events != 0) { // if something happened we will handle the events we want

			// create event buffer the size of how many Events
			INPUT_RECORD eventBuffer[200];

			// fills the event buffer with the events and saves count in EventsRead
			ReadConsoleInput(rhnd, eventBuffer, Events, &EventsRead);
			
			// loop through the event buffer using the saved count
			for (DWORD i = 0; i < EventsRead; ++i) {

				// check if event[i] is a key event && if so is a press not a release
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown) {

					
					// check if the key press was an arrow key
					switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode) {
				
					//move the cursor as well as current pointer
					case VK_UP: //up
						notepad.moveUp();
						stack.deactivate();
						break;

					case VK_DOWN: //down
						notepad.moveDown();
						stack.deactivate();
						break;

					case VK_RIGHT: //right
						notepad.moveRight();
						stack.deactivate();
						break;

					case VK_LEFT: //left
						notepad.moveLeft();
						stack.deactivate();
						break;

					case VK_CONTROL: //undo
						notepad.undo();
						notepad.print();
						break;

					default:
						int keyCode = eventBuffer->Event.KeyEvent.uChar.AsciiChar;

						//input
						if (keyCode == ' ' || (keyCode >= 'A' && keyCode <= 'Z') || (keyCode >= 'a' && keyCode <= 'z')) {

							if (notepad.insert(static_cast<char>(keyCode)))
							{
								//actiavate insertion entry in stack if a char is pressed
								if (stack.insertionActivated == false && keyCode != ' ')
								{
									stack.activateInsertion();
									stack.addToInsertion(notepad.current);
								}
								//if insertion is activated and a space isn't entered, add the new node to the insertion entry
								else if (stack.insertionActivated == true && keyCode != ' ')
								{
									stack.addToInsertion(notepad.current);
								}
								//if space is entered, finish taking input in undo stack (1 word completed)
								else if (stack.insertionActivated == true && keyCode == ' ')
								{
									stack.addToInsertion(notepad.current);
									stack.deactivate();
								}
							}
						}

						//delete
						else if (keyCode == 8)
						{
							notepad.backSpace(true);
						}

						//emter
						else if (keyCode == 13)
						{
							stack.deactivate();
							notepad.enter();
						}
						notepad.print();
						
						break;
					}
				}

			} // end EventsRead loop

		}

	} // end program loop

	return 0;
}