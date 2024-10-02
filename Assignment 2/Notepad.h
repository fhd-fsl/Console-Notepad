#pragma once
#include "Node.h"
#include<iostream>
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
private:
	Node* head;
	Node* current;
	Node* currRow;

	int maxX;
	int maxY;

public:

	//cursor coordinates(point to current pointer)
	int x;
	int y;

	//constructor destructor
	Notepad(int maxX, int maxY)
	{
		head = nullptr;
		current = head;
		currRow = nullptr;
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
			if (x == 1)
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
			if (x == 1)
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
			if (x == 1 && current->left == nullptr)
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
				x = currentCoordinates().x + 1;
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
				x = currentCoordinates().x + 1;
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
			if (oneTimer && rowStart->down)
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
		//shifting is done, remove the last space 
		else
		{

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

		//if the next row doesn't exist
		if (nextRow == nullptr)
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
				last->down = nullptr;
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

	//shifts next row's first node to targetRow's last 
	bool roll1Back(Node* targetRow, bool doForAllRows = 0)
	{
		if (targetRow == nullptr || rowIsFull(targetRow))
			return false;
		Node* currRow = startOfRow(targetRow);
		Node* last = endOfRow(targetRow);
		if (currRow->down)
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
		if (doForAllRows)
		{
			roll1Back(currRow->down, doForAllRows);
		}
		return true;

	}
	//removes node(if the node==current, shift current to right/ next row or left/prev row depending on currDir)
	void removeNode(Node* node, char currDir = 'R')
	{
		if (!node)
			return;

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
				head = nullptr;
				current = nullptr;
				currRow = nullptr;
				x = 1;
				y = 1;
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
		if (!current || (x==1 && y==1))
			return;
		if (x == 1)
		{
			moveLeft();
			return;
		}
		Node* temp = current;

		if (current->left || current->up)
		{
			moveLeft();
		}
		else
		{
			moveRight();
			x = 1;
			gotoxy(x, y);
		}

		removeNode(temp,'L');
	}


	//insert text
	void insert(char ch)
	{

		Node* temp = new Node(ch);

		//insert when notepad is empty
		if (head == nullptr)
		{
			head = temp;
			current = head;
			currRow = current;
			x = 1;
			y = 1;
			x++;
			return;
		}

		//insert at current
		else
		{

			//inserting at first node of a row(x==1)
			if (x == 1 && current->left == nullptr)
			{
				if (rowIsFull(current) && !makeSpace(current))
				{
					return;
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
						return;
					currRow = startOfRow(current);
					delete temp;
					insert(ch);
				}
				//charavter is a space, add it to next line
				else
				{
					//if next line exists
					if (currRow->down)
					{

						Node* nextRow = currRow->down;
						if (rowIsFull(nextRow) && !makeSpace(nextRow))
							return;
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
					//if doesn't exist
					else
					{
						if (colsAreFull())
							return;
						temp->up = currRow;
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
			//full row and space available in notepad
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
				cout << current->ch;

				current = current->right;
				colNum++;
			}
			for (int a = colNum; a <= maxX; a++)
			{
				cout << " ";
			}

			currRow = currRow->down;
			rowNum++;
		}
		for (; rowNum <= maxY; rowNum++)
		{
			int colNum = 1;
			gotoxy(colNum, rowNum);
			for (int a = colNum; a <= maxX; a++)
			{
				cout << " ";
			}
		}

		// After printing, move the cursor back to its original position
		gotoxy(x, y);
	}

};
