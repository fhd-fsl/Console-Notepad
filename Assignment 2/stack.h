#pragma once

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

struct NodeDp
{
	Node* node;
	NodeDp* next;

	NodeDp(Node* node=nullptr)
	{
		this->node = node;
		this->next = nullptr;
	}
};
struct Deletion : public Entry
{
	Node* insertionPoint;
	NodeDp* head;

	Deletion(): Entry()
	{
		isInsertion = false;
		head = nullptr;
		insertionPoint = nullptr;
	}
	~Deletion()
	{
		NodeDp* temp = head;
		while (temp)
		{
			delete temp->node;
			temp = temp->next;
		}
		while (head)
		{
			temp = head;
			head = head->next;
			delete temp;
		}
	}
	bool isEmpty()
	{
		return head== nullptr;
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

	void addToDeletion(Node* node)
	{
		if (deletionActivated && !topEntry->isInsertion)
		{
			topEntry->add(node);
		}
	}

};