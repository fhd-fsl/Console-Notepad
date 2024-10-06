#pragma once

// node to create a linked list based string
struct charNode
{
	char ch;
	charNode* left;
	charNode* right;

	charNode(char ch)
	{
		left = nullptr;
		right = nullptr;
		this->ch = ch;
	}
};
//string to be stored in deletion entry that will be inserted upon undo
struct String
{
	charNode* start;
	int size;

	String() {
		start = nullptr;
		size = 0;
	}
	~String()
	{
		charNode* next = start;
		while (next)
		{
			next = start->right;
			delete start;
			start = next;
		}
	}
	void insertAtStart(char ch)
	{
		charNode* newNode = new charNode(ch);
		if (start == nullptr)
		{
			start = newNode;
			size++;
		}
		else
		{
			start->left = newNode;
			newNode->right = start;
			start = newNode;
			size++;
		}
	}
	char getStartCh()
	{
		return this->start->ch;
	}
	void popStart()
	{
		if (start == nullptr)
			return;
		charNode* temp = start;
		start = start->right;
		delete temp;
		size--;
	}

};

//Parent entry class that will store 1 insertion/deletion operation in stack
struct Entry
{
	bool isInsertion;
	Node* startingNode;
	Entry* top;
	Entry* bottom;

	Entry()
	{
		startingNode = nullptr;
		top = nullptr;
		bottom = nullptr;
	}
	virtual ~Entry()
	{

	}

	//updates endNode pointer for insertion
	virtual void add(Node* node)
	{}

};

struct Insertion : public Entry
{
	Node* endingNode;
	Insertion(): Entry()
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

struct Deletion : public Entry
{
	String word;
	Deletion(): Entry()
	{
		isInsertion = false;
		startingNode = nullptr;
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
		insertionActivated=0;
		deletionActivated=0;
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

};