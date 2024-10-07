#pragma once
#include "Node.h"
#include<iostream>
#include "stack.h"




using namespace std;

struct coord
{
	int x;
	int y;
};

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

class Notepad
{
public:
	Node* head;
	Node* current;
	Node* currRow;
	stack& Stack;

	int maxX;
	int maxY;

	//cursor coordinates(point to current pointer)
	int x;
	int y;

	//constructor destructor
	Notepad(int maxX, int maxY, stack& Stack ): Stack(Stack)
	{
		head = new Node('\n');
		head->newLine = true;
		current = head;
		currRow = current;
		this->maxX = maxX;
		this->maxY = maxY;

		x = 1;
		y = 1;

	}
	~Notepad()
	{
		while (head)
		{
			Node* temp = head->down;
			while (head)
			{
				current = head;
				head = head->right;
				delete current;
			}
			head = temp;
		}
	}

	//moves current to the specified node
	void moveCurrentTo(Node* node, bool xIsAtStartOfRow=0)
	{
		if (!node)
			return;
		node->ch;

		current = node;
		currRow = startOfRow(current);
		x = currentCoordinates().x;
		y = currentCoordinates().y;
		if (!xIsAtStartOfRow)
			x++;
		else
			x = 1;
	}
	//get x,y coordinates of where the current pointer is pointing
	coord currentCoordinates()
	{

		Node* curr = head;
		Node* currRow = head;
		coord c;
		c.x = 1;
		c.y = 1;
		while (curr != current)
		{
			if (curr->right)
			{
				curr = curr->right;
				c.x++;
			}
			else if (currRow->down)
			{
				currRow = currRow->down;
				curr = currRow;
				c.x = 1;
				c.y++;
			}
		}
		return c;
	}
	//functions to move the current pointer as well as cursor coordinates
	bool moveDown()
	{
		if (current && current->down)
		{

			current = current->down;
			currRow = startOfRow(current);
			if (x == 1 || current->ch == '\n')
				x = 1;
			else
				x = currentCoordinates().x + 1;
			y = currentCoordinates().y;
			gotoxy(x, y);
			return true;
		}
		return false;
	}
	bool moveUp()
	{
		if (current && current->up)
		{
			current = current->up;
			currRow = startOfRow(current);
			if (x == 1 || current->ch=='\n')
				x = 1;
			else
				x = currentCoordinates().x + 1;
			y = currentCoordinates().y;
			gotoxy(x, y);
			return true;
		}
		return false;

	}
	bool moveRight()
	{
		if (current)
		{
			if (x == 1 && current->left == nullptr && current->ch!='\n')
			{
				x++;
				gotoxy(x, y);
				return true;
			}
			else if (current && current->right)
			{
				current = current->right;
				x = currentCoordinates().x + 1;
				y = currentCoordinates().y;
				gotoxy(x, y);
				return true;
			}
			else if (current && currRow->down)
			{
				current = currRow->down;
				currRow = currRow->down;
				x = 1;
				y = currentCoordinates().y;
				gotoxy(x, y);
				return true;
			}

			else
			{
				if (current->ch != '\n')
					x = currentCoordinates().x + 1;
				else
					x = currentCoordinates().x;
				gotoxy(x, y);
				return false;
			}
		}
		return false;
	}
	bool moveLeft()
	{
		if (current && current->left)
		{
			current = current->left;
			x = currentCoordinates().x + 1;
			y = currentCoordinates().y;
			gotoxy(x, y);
			return true;
		}
		else if (current && x == 1 && current->left == nullptr )
		{
			if (currRow->up)
			{
				currRow = currRow->up;
				current = endOfRow(currRow);
				if(current->ch == '\n')
					x = currentCoordinates().x;
				else
					x= currentCoordinates().x+1;
				y = currentCoordinates().y;
				gotoxy(x, y);
				return true;
			}
		}
		else
		{
			if (x == 1)
			{
				x = 1;
				y = currentCoordinates().y;
				gotoxy(x, y);
				return false;
			}
			else
			{
				x = 1;
				y = currentCoordinates().y;
				gotoxy(x, y);
				return true;
			}
			
		}
		return false;

	}
	//functions to return start of row/col of given node
	Node* startOfRow(Node* current)
	{
		if (!current)
			return current;
		while (current->left)
		{
			current = current->left;
		}
		return current;
	}
	Node* endOfRow(Node* current)
	{
		if (!current)
			return current;
		while (current->right)
		{
			current = current->right;
		}
		return current;
	}
	Node* startOfCol(Node* current)
	{
		if (!current)
			return current;
		while (current->up)
		{
			current = current->up;
		}
		return current;
	}
	Node* endOfCol(Node* current)
	{
		if (!current)
			return current;
		while (current->down)
		{
			current = current->down;
		}
		return current;
	}
	//functions to get row/col sizes
	int rowSize(Node* startingPoint)
	{
		int s = 0;
		Node* temp = startingPoint;
		while (temp)
		{
			s++;
			temp = temp->right;
		}
		return s;
	}
	int colSize(Node* startingPoint)
	{
		int s = 0;
		Node* temp = startingPoint;
		while (temp)
		{
			s++;
			temp = temp->down;
		}
		return s;
	}
	// function to tell if row/col is full
	bool rowIsFull(Node* rowNode)
	{

		rowNode = startOfRow(rowNode);
		return rowSize(rowNode) >= maxX;
	}
	bool colsAreFull()
	{
		return colSize(this->head) >= maxY;
	}
	//reconnets all vertical connections
	void reconnectAllVertical()
	{
		if (head == nullptr)
			return;
		Node* row1 = head;
		Node* row2 = row1->down;

		Node* temp = head;
		while (temp)
		{
			temp->up = nullptr;
			temp = temp->right;
		}
		while (row2)
		{
			Node* curr1 = row1;
			Node* curr2 = row2;


			bool curr1End = 0;
			bool curr2End = 0;

			while (!curr1End || !curr2End)
			{

				if (!curr1End)
					curr1->down = curr2;
				if (!curr2End)
					curr2->up = curr1;

				if (curr1->right)
				{
					curr1 = curr1->right;
				}
				else
					curr1End = true;

				if (curr2->right)
				{
					curr2 = curr2->right;
				}
				else
					curr2End = true;
			}
			if (row1)
				row1 = row1->down;
			if (row2)
				row2 = row2->down;
		}
		while (row1)
		{
			row1->down = nullptr;
			row1 = row1->right;
		}
	}




	//shifts last word to next line
	bool makeSpace(Node* rowNode)
	{
		if (!rowNode)
			return false;
		Node* lastNode = endOfRow(rowNode);
		Node* rowStart = startOfRow(rowNode);
		bool oneTimer = true;
		int rollBackCounter = 0;


		while (lastNode->ch != ' ' && lastNode != rowStart)
		{
			if (oneTimer && rowStart->down && rowStart->down->newLine==false)
			{
				Node* tempCurrent = this->current;
				Node* tempCurrRow = this->currRow;
				int tempX = this->x;
				int tempY = this->y;

				current = rowStart->down;
				currRow = rowStart->down;
				y = currentCoordinates().y;
				x = 1;
				insert(' ');

				current = tempCurrent;
				currRow = tempCurrRow;
				x = tempX;
				y = tempY;
				gotoxy(x, y);
			}
			oneTimer = false;
			if (!make1Space(rowStart))
			{
				for (int a = 0; a < rollBackCounter; a++)
				{
					roll1Back(rowStart);
				}
				if (rowStart->down && !oneTimer && rowStart->down->ch == ' ')
					removeNode(rowStart->down, 'L');
				return false;
			}
			else
			{
				rollBackCounter++;
			}

			lastNode = endOfRow(rowStart);
		}
		//reached end of row
		if (lastNode == rowStart)
		{
			make1Space(rowStart);
			return false;
		}
		
		//remove last space
		else
		{
			if (current == lastNode)
				if (moveLeft());

				else
					moveRight();
			removeNode(lastNode);
			return true;
		}

	}
	//shifts the last node of a row to the starting of the next row
	bool make1Space(Node* rowNode)
	{
		if (!rowNode)
			return false;

		//store starting point's row head
		Node* currentRow = rowNode;
		currentRow = startOfRow(currentRow);

		//store the next row's head
		Node* nextRow = currentRow->down;

		//store the last node of the row that is to be shifted
		Node* last = endOfRow(rowNode);

		//if the next row doesn't exist or nextRow is a new paragraph(add new line)
		if (nextRow == nullptr || nextRow->newLine==true)
		{
			//if theres no space to add new row then failure
			if (colsAreFull())
				return false;

			//else add new row
			else
			{
				if (last->left)
					last->left->right = nullptr;
				last->left = nullptr;
				last->down = nextRow;
				if (nextRow)
				{
					nextRow->up = last;
				}
				last->right = nullptr;
				last->up = currentRow;
				currentRow->down = last;
				reconnectAllVertical();
				return true;
			}
		}

		//else if the next row exists and is full
		else if (rowIsFull(nextRow))
		{
			//clear space in the next row
			if (makeSpace(nextRow))
			{
				if (last->left)
					last->left->right = nullptr;
				last->left = nullptr;
				last->down = nextRow->down;
				if (nextRow->down)
				{
					nextRow->down->up = last;
				}
				last->right = nextRow;
				nextRow->left = last;
				last->up = currentRow;
				currentRow->down = last;
				reconnectAllVertical();
				return true;
			}
			else
				return false;
		}

		//if there is space in the next row, remove from end of current row and insert at start of next row
		else
		{
			if (last->left)
				last->left->right = nullptr;
			last->left = nullptr;
			last->down = nextRow->down;
			if (nextRow->down)
			{
				nextRow->down->up = last;
			}
			last->right = nextRow;
			nextRow->left = last;
			if (last != currentRow)
			{
				last->up = currentRow;
				currentRow->down = last;
			}
			reconnectAllVertical();
			return true;
		}
	}

	int rowsFirstWordLength(Node* rowNode)
	{
		if (!rowNode)
			return 0;
		Node* currentRow = startOfRow(rowNode);
		Node* curr = currentRow;
		if (curr->ch == ' ' || curr->ch=='\n')
			return 1;
		else
		{
			int s = 1;
			while (curr->right && curr->right->ch != ' ')
			{
				curr = curr->right;
				s++;
			}
			return s;
		}
	}
	//if a line has space, move the word from the next line to the current line
	bool rollBack(Node* rowNode)
	{
		if (!rowNode)
			return false;
		Node* currentRow = startOfRow(rowNode);
		Node* nextRow = currentRow->down;
		//if next line doesn't exist or next line is a new line, can't shift a word
		if (nextRow == nullptr || nextRow->newLine == true)
		{
			return false;
		}
		

		while (true)
		{
			//if next line doesn't exist or next line is a new line, can't shift a word
			if (nextRow == nullptr || nextRow->newLine == true)
			{
				break;
			}
			//find avaialable space and the space required to fit the next word(+ space)
			int currentSpace = maxX - rowSize(currentRow);
			if (nextRow->ch == ' ')
			{
				if (currentSpace >= 1)
				{
					if (nextRow->right)
					{
						nextRow = nextRow->right;
					}
					else
						nextRow = nextRow->down;
					roll1Back(currentRow);
				}
				else
					break;
			}
			else
			{
				int neededSpace = rowsFirstWordLength(nextRow);
				neededSpace++;
				//if the needed space is available
				if (neededSpace <= currentSpace)
				{
					//save current location
					Node* tempCurrent = this->current;
					int tempX = x;
					int tempY = y;

					//move to end of row
					current = endOfRow(rowNode);
					currRow = currentRow;
					y = currentCoordinates().y;
					x = currentCoordinates().x + 1;

					//insert a space at end of row
					if (current->ch != ' ')
						insert(' ');
					neededSpace--;

					//rollback letters from next word to current line
					for (int a = 0; a < neededSpace;a++)
					{
						nextRow = nextRow->right;
						roll1Back(currentRow);
					}
					if (nextRow && nextRow->ch == ' ')
					{
						Node* temp = nextRow;
						if (nextRow->right)
						{
							nextRow = nextRow->right;
						}
						else
							nextRow = nextRow->down;
						removeNode(temp);
					}
					moveCurrentTo(tempCurrent, tempX==1);
				}
				else
					break;
			}
		}
		return rollBack(nextRow);

	}
	//shifts next row's first node to targetRow's last 
	bool roll1Back(Node* targetRow)
	{
		if (targetRow == nullptr || rowIsFull(targetRow))
			return false;
		Node* currRow = startOfRow(targetRow);
		Node* last = endOfRow(targetRow);

		//if next row exists and next row's first node is not the start of a new paragraph
		if (currRow->down && currRow->down->newLine==false)
		{
			Node* nextRow = currRow->down;

			if (nextRow->right)
			{
				currRow->down = nextRow->right;
				nextRow->right->up = currRow;
				nextRow->right->down = nextRow->down;
				if (nextRow->down)
				{
					nextRow->down->up = nextRow->right;
				}
				nextRow->right->left = nullptr;

			}
			else
			{
				currRow->down = nextRow->down;
				if (nextRow->down)
					nextRow->down->up = currRow;
			}

			last->right = nextRow;
			nextRow->left = last;
			nextRow->right = nullptr;
			reconnectAllVertical();


		}
		return true;

	}
	//removes node(if the node==current, shift current to right/ next row or left/prev row depending on currDir)
	void removeNode(Node* node, char currDir = 'R')
	{
		if (!node)
			return;

		//if a node is about to be deleted and it is in stack, move the pointer in stack accordingly
		if (!Stack.isEmpty())
		{
			Entry* currentEntry = Stack.topEntry;
			for (int a = 0; a < Stack.currSize; a++)
			{
				
				if (currentEntry->startingNode == node) 
				{
					Node* tempCurrent = current;
					int tempX = x;

					moveCurrentTo(node);
					if (moveLeft());
					else
						moveRight();

					currentEntry->startingNode = current;
					moveCurrentTo(tempCurrent, (tempX == 1));
				}
				if (currentEntry->isInsertion && ((Insertion*)currentEntry)->endingNode == node)
				{
					Node* tempCurrent = current;
					int tempX = x;

					moveCurrentTo(node);
					if (moveLeft());
					else
						moveRight();

					((Insertion*)currentEntry)->endingNode = current;
					moveCurrentTo(tempCurrent, (tempX == 1));
				}
				currentEntry = currentEntry->bottom;
			}
		}
		if (current == node)
		{
			if (currDir == 'R')
			{
				if (moveRight());
				else if (moveLeft());
			}
			else if( currDir=='L')
			{
				if (moveLeft());
				else if (moveRight());
			}
		}
		
		if (head == node)
		{
			if (head->right)
			{
				head = head->right;
			}
			else if (head->down)
				head = head->down;
			else
			{
				head->ch='\n';
				current = head;
				currRow = head;
				x = 1;
				y = 1;
				return;
			}
		}


		//if node is in middle or end
		if (node->left)
		{
			node->left->right = node->right;
			if (node->right)
			{
				node->right->left = node->left;
			}
		}

		//if node is the first of the row
		else
		{
			//if it is a new paragraph
			if (node->newLine == true)
			{
				if (node->right)
					node->right->newLine = true;
				else if (node->down)
				{
					node->down->newLine = true;
				}
			}


			//node on it's right exists
			if (node->right)
			{
				node->right->up = node->up;
				if (node->up)
				{
					node->up->down = node->right;
				}
				node->right->down = node->down;
				if (node->down)
				{
					node->down->up = node->right;
				}
				node->right->left = node->left;
			}

			//it is the only node in the line
			else
			{
				if (node->up)
				{
					node->up->down = node->down;
				}
				if (node->down)
				{
					node->down->up = node->up;
				}
			}
		}
		if (node == currRow)
		{
			currRow = startOfRow(current);
		}
		delete node;
		reconnectAllVertical();

	}

	//delete text
	void backSpace()
	{
		if (!current || (x==1 && y==1 && head->ch!='\n'))
			return;

		//backspace at start of line
		if (x == 1 && current->ch!='\n')
		{
			current->newLine = false;
			if (current->up)
				rollBack(current->up);
			moveLeft();
			return;
		}
		Node* temp = current;

		//backspace the first character of a line that has more than 1 character
		if (current->left == nullptr && x != 1 && current->right)
		{
			moveRight();
			x = 1;
			gotoxy(x, y);
		}
		//backspace in the middle/end of line or the only character in a line
		else if (current->left || current->up)
		{
			moveLeft();
		}

		//backspace at head
		else
		{
			moveRight();
			x = 1;
			y = 1;
			gotoxy(x, y);
		}
		removeNode(temp,'L');

		temp = currRow;
		while (temp && temp->newLine == false)
		{
			rollBack(currRow);
			temp = temp->up;
		}
		if (temp)
		{
			rollBack(temp);
		}
		
	}
	void enter()
	{
		//if lines are full, can't insert new line
		if (!current || colsAreFull())
			return;

		//pressing enter at end of line
		if (current->right == nullptr)
		{
			Node* newNode = new Node('\n');
			newNode->newLine = true;
			newNode->down = currRow->down;
			if (currRow->down)
			{
				currRow->down->up = newNode;
			}
			newNode->up = currRow;
			currRow->down = newNode;
			reconnectAllVertical();
			moveRight();

		}
		//pressing enter at start of line
		else if (x == 1)
		{
			Node* newNode = new Node('\n');
			newNode->newLine = true;
			current->newLine = true;

			newNode->down = current;
			newNode->up = current->up;
			if (current->up)
			{
				current->up->down = newNode;
			}
			current->up = newNode;

			//if pressing enter before head
			if (current == head)
				head = newNode;

			current = newNode;
			currRow = newNode;
			reconnectAllVertical();
		}

		//pressing enter in middle of line
		else if (current->right)
		{
			Node* nextLine = current->right;
			nextLine->newLine = true;
			if (currRow->down)
				currRow->down->newLine = true;

			current->right = nullptr;
			nextLine->left = nullptr;
			nextLine->up = currRow;
			nextLine->down = currRow->down;
			if (currRow->down)
			{
				currRow->down->up = nextLine;
			}
			currRow->down = nextLine;

			reconnectAllVertical();

		}

		
	}

	//insert text
	bool insert(char ch)
	{
		if (current && current->ch == '\n')
		{
			current->ch = ch;
			x++;
			return true;
		}

		Node* temp = new Node(ch);

		//insert when notepad is empty
		if (head == nullptr)
		{
			head = temp;
			head->newLine = true;
			current = head;
			currRow = current;
			x = 1;
			y = 1;
			x++;
			return true;
		}

		//insert at current
		else
		{

			//inserting at first node of a row(x==1)
			if (x == 1 && current->left == nullptr)
			{
				if (rowIsFull(current) && !makeSpace(current))
				{
					return false;
				}
				if (current->newLine)
				{
					temp->newLine = true;
					current->newLine = false;
				}
				temp->up = current->up;
				temp->down = current->down;
				if (current->up)
					current->up->down = temp;
				if (current->down)
					current->down->up = temp;
				temp->right = current;
				current->left = temp;

				if (current == head)
				{
					head = temp;
				}
				moveLeft();
			}
			//inserting at end of full row(add to next line)
			else if (rowIsFull(current) && current->right == nullptr)
			{
				//move whole word to next line
				if (ch != ' ' && current->ch != ' ')
				{
					if (!makeSpace(current))
						return false;
					currRow = startOfRow(current);
					delete temp;
					insert(ch);
				}
				//charavter is a space, add it to next line
				else
				{
					//if next line exists and isnt a new paragraph
					if (currRow->down && currRow->down->newLine==false)
					{

						Node* nextRow = currRow->down;
						if (rowIsFull(nextRow) && !makeSpace(nextRow))
							return false;
						temp->up = currRow;
						currRow->down = temp;
						temp->left = nullptr;
						temp->right = nextRow;
						nextRow->left = temp;
						temp->down = nextRow->down;
						if (nextRow->down)
						{
							nextRow->down->up = temp;
						}

					}
					//if doesn't exist or is a new paragraph
					else
					{
						if (colsAreFull())
							return false;
						temp->up = currRow;
						if (currRow->down)
						{
							currRow->down->up = temp;
						}
						temp->down = currRow->down;
						currRow->down = temp;
					}
					reconnectAllVertical();
					moveRight();
					x++;
					gotoxy(x, y);
				}


			}

			//if there is space in the current row
			else if (!rowIsFull(current))
			{
				if (current->right)
				{
					current->right->left = temp;
				}
				temp->right = current->right;
				temp->left = current;
				current->right = temp;
				reconnectAllVertical();
				moveRight();
			}
			//inserting between full row and space available in notepad
			else if (rowIsFull(current) && makeSpace(current))
			{
				if (current->right)
				{
					current->right->left = temp;
				}
				temp->right = current->right;
				temp->left = current;
				current->right = temp;
				reconnectAllVertical();
				moveRight();
			}

		}
		return true;
	}

	void print()
	{
		Node* currRow = head;
		int rowNum = 1;

		while (currRow)
		{
			Node* current = currRow;
			int colNum = 1;


			while (current)
			{
				
				
				gotoxy(colNum, rowNum);
				if (current->ch != '\n')
				{
					cout << current->ch;
					colNum++;
				}

				current = current->right;
				
			}
			for (int a = colNum; a <= maxX; a++)
			{
				cout << " ";
			}

			currRow = currRow->down;
			rowNum++;
		}

		int max = rowNum + 1;
		for (; rowNum <= max && rowNum<=maxY; rowNum++)
		{
			int colNum = 1;
			gotoxy(colNum, rowNum);
			for (int a = colNum; a <= maxX; a++)
			{
				cout << " ";
			}
		}
		/*gotoxy(600, 600);
		if (current)
		{
			cout << current->newLine << ' ';
			current->ch == '\n' ? cout << "\\n" : current->ch==' '?cout<<"SPACE": cout<<current->ch<<"       ";
		}*/

		

		// After printing, move the cursor back to its original position
		gotoxy(x, y);
	}

	void undoInsertion()
	{
		if (Stack.isEmpty())
			return;
		if (Stack.topEntry->isInsertion)
		{
			Stack.deactivate();
			while (((Insertion*)Stack.topEntry)->endingNode != Stack.topEntry->startingNode)
			{
				moveCurrentTo(((Insertion*)Stack.topEntry)->endingNode);
				((Insertion*)Stack.topEntry)->decrementEndingNode();
				backSpace();
			}
			moveCurrentTo(((Insertion*)Stack.topEntry)->startingNode);
			backSpace();
			Stack.pop();
		}
	}

};

