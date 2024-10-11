
struct Node
{
	char ch;
	Node* up;
	Node* down;
	Node* left;
	Node* right;
	bool newLine;


	Node(char ch = '\0') :ch(ch)
	{
		up = nullptr;
		down = nullptr;
		left = nullptr;
		right = nullptr;
		newLine = 0;
	}
};

Node* baboing;

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
//Parent entry class that will store 1 insertion/deletion operation in stack
class Entry
{
public:
	bool isInsertion;
	Entry* top;
	Entry* bottom;


	Entry()
	{
		top = nullptr;
		bottom = nullptr;
	}
	virtual ~Entry()
	{}

	//updates endNode pointer for insertion
	virtual void add(Node* node)
	{}
};

struct Insertion : public Entry
{
	Node* startingNode;
	Node* endingNode;
	Insertion() : Entry()
	{
		this->isInsertion = true;
		startingNode = nullptr;
		endingNode = nullptr;
	}

	void add(Node* node)
	{
		if (startingNode == nullptr)
		{
			startingNode = node;
		}
		endingNode = node;
	}
	void decrementEndingNode()
	{
		if (endingNode->left)
		{
			endingNode = endingNode->left;

		}
		else
		{
			if (endingNode->up)
			{
				endingNode = endingNode->up;
			}
			while (endingNode->right != nullptr)
			{
				endingNode = endingNode->right;
			}
		}
	}
};

struct NodeDp
{
	Node* node;
	NodeDp* next;

	NodeDp(Node* node = nullptr)
	{
		this->node = node;
		this->next = nullptr;
	}
};
struct Deletion : public Entry
{
	Node* insertionPoint;
	NodeDp* head;

	Deletion() : Entry()
	{
		isInsertion = false;
		head = nullptr;
		insertionPoint = nullptr;
	}
	~Deletion()
	{
		NodeDp* temp = head;
		while (head)
		{
			temp = head;
			head = head->next;
			delete temp;
		}
	}
	void deleteNodes()
	{
		NodeDp* temp = head;
		while (temp)
		{
			delete temp->node;
			temp = temp->next;
		}
	}
	bool isEmpty()
	{
		return head == nullptr;
	}

	void add(Node* node)
	{

		NodeDp* temp = new NodeDp(node);
		if (isEmpty())
		{
			head = temp;
		}
		else
		{
			temp->next = head;
			head = temp;
		}
		setInsertionPoint();
		//print();
	}
	void print()
	{
		int startingx = 0;
		int startingy = 20;
		if (isEmpty())
			return;
		NodeDp* temp = head;

		gotoxy(startingx, startingy);
		cout << "Ins: " << this->insertionPoint->ch;
		startingy++;
		while (temp)
		{
			gotoxy(startingx++, startingy);
			cout << temp->node->ch;
			temp = temp->next;
		}
		cout << "                 ";
	}

	void incrementStartingNode()
	{
		if (!isEmpty())
		{
			NodeDp* temp = head;
			head = head->next;
			delete temp;
		}
	}
	Node* getStartingNode()
	{
		if (!head)
			return nullptr;
		else
			return head->node;
	}
	void setInsertionPoint()
	{
		if (!isEmpty())
		{
			if (head->node)
			{
				if (head->node->left)
				{
					insertionPoint = head->node->left;
				}
				else if (head->node->up)
				{
					insertionPoint = head->node->up;
					while (insertionPoint->right)
					{
						insertionPoint = insertionPoint->right;
					}
				}
				else if (!insertionPoint)
				{
					insertionPoint = head->node;
				}
			}
		}
	}
};



struct stack
{
	Entry* topEntry;
	Entry* lastEntry;
	bool insertionActivated;
	bool deletionActivated;
	const int maxSize = 5;
	int currSize = 0;

	stack()
	{
		topEntry = nullptr;
		lastEntry = nullptr;
		insertionActivated = 0;
		deletionActivated = 0;
	}
	~stack()
	{
		lastEntry = topEntry;
		while (lastEntry)
		{
			lastEntry = topEntry->bottom;
			delete topEntry;
			topEntry = lastEntry;
		}
	}
	bool isFull()
	{
		return currSize >= maxSize;
	}
	bool isEmpty()
	{
		return topEntry == nullptr;
	}
	bool topIsInsertion()
	{
		if (topEntry == nullptr)
			return false;
		else
			return topEntry->isInsertion;
	}
	void removeLastEntry()
	{
		if (lastEntry == nullptr)
			return;
		if (lastEntry == topEntry)
			topEntry = nullptr;

		Entry* temp = lastEntry;
		lastEntry = lastEntry->top;
		if (lastEntry)
			lastEntry->bottom = nullptr;
		if (!temp->isInsertion)
			((Deletion*)temp)->deleteNodes();
		delete temp;
		currSize--;
	}
	void pop()
	{
		if (!topEntry)
			return;
		if (topEntry == lastEntry)
			lastEntry = nullptr;
		Entry* temp = topEntry;
		this->topEntry = topEntry->bottom;
		currSize--;
		delete temp;
	}


	void activateInsertion()
	{
		if (insertionActivated == 1)
			return;
		insertionActivated = 1;
		deletionActivated = 0;
		Entry* newInsertion = new Insertion();
		if (isEmpty())
		{
			topEntry = newInsertion;
			lastEntry = newInsertion;
			currSize++;
		}
		else
		{
			if (isFull())
			{
				removeLastEntry();
			}
			newInsertion->bottom = topEntry;
			topEntry->top = newInsertion;
			topEntry = newInsertion;
			currSize++;
		}
	}
	void deactivate()
	{
		insertionActivated = 0;
		deletionActivated = 0;
	}
	void activateDeletion()
	{
		if (deletionActivated == 1)
			return;
		insertionActivated = 0;
		deletionActivated = 1;
		Entry* newDeletion = new Deletion();
		if (isEmpty())
		{
			topEntry = newDeletion;
			lastEntry = newDeletion;
			currSize++;
		}
		else
		{
			if (isFull())
			{
				removeLastEntry();
			}
			newDeletion->bottom = topEntry;
			topEntry->top = newDeletion;
			topEntry = newDeletion;
			currSize++;
		}
	}

	void addToInsertion(Node* node)
	{
		if (insertionActivated && topEntry->isInsertion)
		{
			topEntry->add(node);
		}
	}

	void addToDeletion(Node* node)
	{
		if (deletionActivated && !topEntry->isInsertion)
		{
			topEntry->add(node);
		}
	}

	void print()
	{
		if (isEmpty())
		{
			cout << "              \n                  \n                 \n";
			return;

		}
		if (this->topEntry->isInsertion)
		{
			cout << "INSERTION\n";
			cout << "Start: ";
			((Insertion*)topEntry)->startingNode->ch == ' ' ? cout << "_" : ((Insertion*)topEntry)->startingNode->ch == '\n' ? cout << "\\n" : cout << ((Insertion*)topEntry)->startingNode->ch;
			cout << "          " << endl;
			cout << "End: ";
			((Insertion*)topEntry)->endingNode->ch == ' ' ? cout << "_" : ((Insertion*)topEntry)->endingNode->ch == '\n' ? cout << "\\n" : cout << ((Insertion*)topEntry)->endingNode->ch;
			cout << "          " << endl;
		}
		else
		{
			cout << "DELETION \n";
			if (((Deletion*)topEntry)->isEmpty())
				return;
			NodeDp* curr = ((Deletion*)topEntry)->head;
			cout << "Ins point: ";
			((Deletion*)topEntry)->insertionPoint->ch == ' ' ? cout << "_" : ((Deletion*)topEntry)->insertionPoint->ch == '\n' ? cout << "\\n" : cout << ((Deletion*)topEntry)->insertionPoint->ch;
			cout << "          " << endl;
			while (curr)
			{
				curr->node->ch == ' ' ? cout << "_" : curr->node->ch == '\n' ? cout << "\\n" : cout << curr->node->ch;
				curr = curr->next;
			}
			cout << "                 ";
		}
	}
};


struct coord
{
	int x;
	int y;
};

class Notepad
{
public:
	Node* head;
	Node* current;
	Node* currRow;
	stack& Stack;
	stack& redoStack;

	int maxX;
	int maxY;

	//cursor coordinates(point to current pointer)
	int x;
	int y;


	//constructor destructor
	Notepad(int maxX, int maxY, stack& Stack , stack& redoStack): Stack(Stack), redoStack(redoStack)
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

		char ch=node->ch;
		node->left;
		node->right;

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

		if (current == node)
		{
			if (currDir == 'R')
			{
				if (moveRight());
				else if (moveLeft());
			}
			else if (currDir == 'L')
			{
				if (moveLeft());
				else if (moveRight());
			}
		}

		bool ret = false;
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
				head->ch = '\n';
				current = head;
				currRow = head;
				x = 1;
				y = 1;
				ret = true;
			}
		}

		//INSERTION ENTRY
		//if a node is about to be deleted and it is in insertion stack, move the pointer in insertion stack accordingly
		if (!Stack.isEmpty())
		{
			Entry* currentEntry = Stack.topEntry;
			for (int a = 0; a < Stack.currSize; a++)
			{
				if (currentEntry->isInsertion)
				{
					if (((Insertion*)currentEntry)->startingNode == node)
					{
						Node* tempCurrent = current;
						int tempX = x;

						moveCurrentTo(node);
						if (moveLeft());
						else
							moveRight();

						((Insertion*)currentEntry)->startingNode = current;
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
		}


		//INSERTION ENTRY
		//redo stack
		if (!redoStack.isEmpty())
		{
			Entry* currentEntry = redoStack.topEntry;
			for (int a = 0; a < redoStack.currSize; a++)
			{
				if (currentEntry->isInsertion)
				{
					if (((Insertion*)currentEntry)->startingNode == node)
					{
						Node* tempCurrent = current;
						int tempX = x;

						moveCurrentTo(node);
						if (moveLeft());
						else
							moveRight();

						((Insertion*)currentEntry)->startingNode = current;
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
		}
		if (ret)
			return;
		


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

		//DELETION ENTRY
		//if a node is about to be deleted and it is in deletion stack, don't delete
		bool dontDelete = false;
		if (!Stack.isEmpty())
		{
			Entry* currentEntry = Stack.topEntry;
			for (int a = 0; a < Stack.currSize; a++)
			{
				if (!currentEntry->isInsertion)
				{
					NodeDp* temp = ((Deletion*)currentEntry)->head;
					while (temp)
					{
						if (temp->node == node)
						{
							dontDelete = true;
							break;
						}
						else
							temp = temp->next;
					}

				}
				currentEntry = currentEntry->bottom;
			}
		}
		if ( !Stack.isEmpty() && !Stack.topEntry->isInsertion && ((Deletion*)Stack.topEntry)->insertionPoint == node)
			dontDelete = true;

		//DELETION ENTRY
		//redoStack
		if (!redoStack.isEmpty())
		{
			Entry* currentEntry = redoStack.topEntry;
			for (int a = 0; a < redoStack.currSize; a++)
			{
				if (!currentEntry->isInsertion)
				{
					NodeDp* temp = ((Deletion*)currentEntry)->head;
					while (temp)
					{
						if (temp->node == node)
						{
							dontDelete = true;
							break;
						}
						else
							temp = temp->next;
					}

				}
				currentEntry = currentEntry->bottom;
			}
		}
		if (!redoStack.isEmpty() && !redoStack.topEntry->isInsertion && ((Deletion*)redoStack.topEntry)->insertionPoint == node)
			dontDelete = true;

		if(!dontDelete)
			delete node;
		reconnectAllVertical();

	}

	//delete text
	void backSpace(bool isFromMain=false)
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

		//storing deletions in deletion stack for undoing
		if (isFromMain)
		{

			if (Stack.deletionActivated == false && ((current->ch >= 'A' && current->ch <= 'Z') || (current->ch >= 'a' && current->ch <= 'z')))
			{
				Stack.activateDeletion();
				Stack.addToDeletion(current);
			}
			else if (Stack.deletionActivated == true && ((current->ch >= 'A' && current->ch <= 'Z') || (current->ch >= 'a' && current->ch <= 'z')))
			{
				Stack.addToDeletion(current);
			}
			else if (Stack.deletionActivated == true && current->ch == ' ')
			{
				Stack.addToDeletion(current);
				Stack.deactivate();
			}
			else if (Stack.deletionActivated == true && (current->ch == '\n' || current->left == nullptr || current->left->ch == '\n'))
			{
				((Deletion*)Stack.topEntry)->getStartingNode()->newLine = true;
				Stack.deactivate();
			}
		}

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
	bool insert(char ch, Node* optional=nullptr)
	{
		if (current && current->ch == '\n')
		{
			if (optional)
				current->ch = optional->ch;
			else
				current->ch = ch;
			x++;
			return true;
		}
		
		Node* temp;
		if (optional && optional!=current)
		{
			temp = optional;
			ch = temp->ch;
		}
		else
		{
			temp = new Node(ch);
		}

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
					if(!optional)
						delete temp;
					insert(ch, optional);
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

		gotoxy(0, 20);
		cout << head->ch;
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
		// After printing, move the cursor back to its original position
		gotoxy(x, y);
	}

	void undo()
	{
		if (Stack.isEmpty())
			return;


		if (Stack.topEntry->isInsertion)
		{
			Stack.deactivate();


			///////////////// adding as deletion to redo stack ////////////////////
			/**/redoStack.activateDeletion();
			/**/Node* curr=((Insertion*)Stack.topEntry)->endingNode;
			/**/while (curr != ((Insertion*)Stack.topEntry)->startingNode)
			/**/{
			/**/	redoStack.addToDeletion(curr);
			/**/	if (curr->left)
					{
						curr = curr->left;
					}
					else
					{
						if (curr->up)
						{
							curr = curr->up;
						}
						while (curr->right != nullptr)
						{
							curr = curr->right;
						}
					}
			/**/}
			/**/redoStack.addToDeletion(curr);
			/**/redoStack.deactivate();
			///////////////// adding as deletion to redo stack ////////////////////
			
			while (((Insertion*)Stack.topEntry)->endingNode != ((Insertion*)Stack.topEntry)->startingNode)
			{
				moveCurrentTo(((Insertion*)Stack.topEntry)->endingNode);
				((Insertion*)Stack.topEntry)->decrementEndingNode();
				backSpace();
			}
			moveCurrentTo(((Insertion*)Stack.topEntry)->startingNode);
			backSpace();
			Stack.pop();
		}
		else if (!Stack.topEntry->isInsertion)
		{

			Stack.deactivate();

			///////////////// adding as insertion to redo stack ////////////////////
			/**/redoStack.activateInsertion();									/**/	
			/**/NodeDp* curr = ((Deletion*)Stack.topEntry)->head;				/**/
			/**/while (curr)													/**/
			/**/{																/**/
			/**/	redoStack.addToInsertion(curr->node);						/**/
			/**/	curr = curr->next;											/**/	
			/**/}		
			/**/redoStack.deactivate();
			///////////////// adding as insertion to redo stack ////////////////////

			

			if (((Deletion*)Stack.topEntry)->insertionPoint)
				moveCurrentTo(((Deletion*)Stack.topEntry)->insertionPoint);
			/*if (((Deletion*)Stack.topEntry)->getStartingNode()->newLine == true)
				insert(' ');*/
			
			while (!((Deletion*)Stack.topEntry)->isEmpty())
			{
				Node* temp = ((Deletion*)Stack.topEntry)->head->node;
				((Deletion*)Stack.topEntry)->incrementStartingNode();
				insert(' ', temp);
			}
			Stack.pop();
		}
	}



	void redo()
	{
		if (redoStack.isEmpty())
			return;
		if (redoStack.topEntry->isInsertion)
		{
			redoStack.deactivate();


			///////////////// adding as deletion to undo stack ////////////////////
			/**/Stack.activateDeletion();
			/**/Node* curr = ((Insertion*)redoStack.topEntry)->endingNode;
			/**/while (curr != ((Insertion*)redoStack.topEntry)->startingNode)
			{
				redoStack.addToDeletion(curr);
				if (curr->left)
				{
					curr = curr->left;
				}
				else
				{
					if (curr->up)
					{
						curr = curr->up;
					}
					while (curr->right != nullptr)
					{
						curr = curr->right;
					}
				}
			}
			/**/Stack.addToDeletion(curr);
			/**/Stack.deactivate();
			///////////////// adding as deletion to undo stack ////////////////////


			while (((Insertion*)redoStack.topEntry)->endingNode != ((Insertion*)redoStack.topEntry)->startingNode)
			{
				moveCurrentTo(((Insertion*)redoStack.topEntry)->endingNode);
				((Insertion*)redoStack.topEntry)->decrementEndingNode();
				backSpace();
			}
			moveCurrentTo(((Insertion*)redoStack.topEntry)->startingNode);
			backSpace();
			redoStack.pop();
		}
		else if (!redoStack.topEntry->isInsertion)
		{

			redoStack.deactivate();

			///////////////// adding as insertion to redo stack ////////////////////
			/**/Stack.activateInsertion();										/**/
			/**/NodeDp* curr = ((Deletion*)redoStack.topEntry)->head;			/**/
			/**/while (curr)													/**/
			/**/ {																/**/
			/**/	Stack.addToInsertion(curr->node);							/**/
			/**/	curr = curr->next;											/**/
			/**/
				 }			
			/**/Stack.deactivate();
			///////////////// adding as insertion to redo stack ////////////////////

			if (((Deletion*)redoStack.topEntry)->insertionPoint)
				moveCurrentTo(((Deletion*)redoStack.topEntry)->insertionPoint);
			if (((Deletion*)redoStack.topEntry)->getStartingNode()->newLine == true)
				insert(' ');
			while (!((Deletion*)redoStack.topEntry)->isEmpty())
			{
				Node* temp = ((Deletion*)redoStack.topEntry)->head->node;
				((Deletion*)redoStack.topEntry)->incrementStartingNode();
				insert(' ', temp);
			}
			redoStack.pop();
			
		}
	}

};

