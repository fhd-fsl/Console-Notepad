#include <iostream>
#include <Windows.h>
#include<fstream>
#include<filesystem>
using namespace std;


bool isAlphabet(char ch)
{
	return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}
void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

template <class T>
class StackNode
{
public:
	T data;
	StackNode<T>* next;

	StackNode(T data)
	{
		this->data = data;
		next = nullptr;
	}
	StackNode()
	{
		next = nullptr;
	}
};

template <class T>
class Stack
{
	StackNode<T>* top;

public:

	Stack()
	{
		top = nullptr;
	}

	bool isEmpty()
	{
		return top == nullptr;
	}

	T& Top()
	{
		return this->top->data;
	}

	void push(T val)
	{
		StackNode<T>* temp = new StackNode<T>(val);
		temp->next = top;
		top = temp;
	}
	T pop()
	{
		if (isEmpty())
		{
			return NULL;
		}

		T temp = top->data;
		StackNode<T>* deletingNode = top;
		top = top->next;
		delete deletingNode;
		return temp;
	}
};

struct NAryNode;
//notepad Node
struct Node
{
	char ch;
	Node* up;
	Node* down;
	Node* left;
	Node* right;
	bool newLine;
	NAryNode* treeNode;
	bool color;
	int lineNumber;


	Node(char ch = '\0') :ch(ch)
	{
		up = nullptr;
		down = nullptr;
		left = nullptr;
		right = nullptr;
		treeNode = nullptr;
		newLine = 0;
		color = 0;
		lineNumber = 0;
	}
};

//notepad node double pointer
struct NodeDp
{
	Node* node;
	NodeDp* next;
	char backup;

	NodeDp(Node* node = nullptr)
	{
		this->node = node;
		this->next = nullptr;
		this->backup = '\0';

	}
};

//mini string class for basic word operations
class String
{

public:
	char* arr;
	int length;
	String()
	{
		this->length = 0;
		this->arr = new char[length + 1];
		this->arr[length] = '\0';
	}
	String(const String& copy)
	{
		this->length = copy.length;
		this->arr = new char[length + 1];
		for (int a = 0; a < length; a++)
		{
			this->arr[a] = copy.arr[a];
		}
		this->arr[length] = '\0';
	}
	String& operator =(const String& copy)
	{
		if (this->arr != nullptr)
			delete[] this->arr;

		this->length = copy.length;
		this->arr = new char[length + 1];
		for (int a = 0; a < length; a++)
		{
			this->arr[a] = copy.arr[a];
		}
		this->arr[length] = '\0';
		return *this;
	}

	~String()
	{
		delete[] arr;
	}

	void operator+=(char ch)
	{
		this->length++;
		char* temp = new char[length + 1];
		int a = 0;
		for (; a < length - 1; a++)
		{
			temp[a] = arr[a];
		}
		temp[a] = ch;
		temp[length] = '\0';
		delete arr;
		arr = temp;
	}
	void operator--(int dummy)
	{
		if (this->length == 0)
			return;
		this->length--;
		char* temp = new char[length + 1];
		int a = 0;
		for (; a < length; a++)
		{
			temp[a] = arr[a];
		}
		temp[length] = '\0';
		delete arr;
		arr = temp;
	}

	//store word from start of word till end of word
	void newWord(Node* current)
	{
		this->empty();
		if (!current || current->ch == ' ' || current->ch == '\n')
			return;
		while (current->left != nullptr && current->left->ch != ' ' && current->left->ch != '\n')
		{
			current = current->left;
		}
		while (current && current->ch != ' ' && current->ch != '\n')
		{
			if (current->ch != ' ' && current->ch != '\n')
				*this += current->ch;
			current = current->right;
		}
	}

	//store word from start of word till current
	void wordSelectingHelper(Node* current)
	{
		this->empty();
		if (!current || current->ch == ' ' || current->ch == '\n')
			return;
		Node* temp = current;
		while (current->left != nullptr && current->left->ch != ' ' && current->left->ch != '\n')
		{
			current = current->left;
		}
		while (current && current != temp)
		{
			if (current->ch != ' ' && current->ch != '\n')
				*this += current->ch;
			current = current->right;
		}
		if (current && current->ch != ' ' && current->ch != '\n')
			*this += current->ch;
	}

	void empty()
	{
		delete arr;
		this->length = 0;
		this->arr = new char[length + 1];
		this->arr[length] = '\0';
	}

	void removeFirstCharacters(int n)
	{
		if (n > this->length)
			return;
		else
		{
			char* temp = new char[this->length - n + 1];
			int b = 0;
			for (int a = n; a < this->length; a++)
			{
				temp[b++] = this->arr[a];
			}
			temp[b] = '\0';
			this->length -= n;
			this->arr = temp;
		}
	}

	bool operator==(String s)
	{
		if (this->length != s.length)
			return false;
		for (int a = 0; a < this->length; a++)
		{
			if (this->arr[a] != s.arr[a])
			{
				if (this->arr[a] + 32 == s.arr[a] || this->arr[a] - 32 == s.arr[a])
				{
					continue;
				}
				else
				{
					return false;
				}
			}

		}
		return true;
	}

};





//N-ary tree node
struct NAryNode
{
	char ch;
	bool endOfWord;
	NodeDp* head;//for storing the nodes in the notepad that correspond to this treeNode
	const static int noOfChildren = 26;
	NAryNode* children[noOfChildren];

	NAryNode(char ch)
	{
		this->endOfWord = 0;
		this->ch = ch;
		this->head = nullptr;
		for (int a = 0; a < noOfChildren; a++)
		{
			children[a] = nullptr;
		}
	}
	~NAryNode()
	{
		while (head)
		{
			NodeDp* temp = head;
			head = head->next;
			delete temp;
		}
	}

	//for telling if a node corresponds to that tree node
	bool nodeExists(Node* targetNode)
	{
		if (!targetNode)
			return false;
		NodeDp* temp = this->head;
		while (temp)
		{
			if (temp->node == targetNode)
				return true;
			temp = temp->next;
		}
		return false;
	}
	//for storing the nodes that correspond to that tree Node
	void addNode(Node* node)
	{
		if (head == nullptr)
		{
			head = new NodeDp(node);
		}
		else
		{
			NodeDp* temp = head;
			while (temp->next != nullptr)
				temp = temp->next;
			temp->next = new NodeDp(node);
		}
	}
	//for removing nodes that correspond to that tree node
	void subtractNode(Node* targetNode)
	{
		if (!targetNode)
			return;
		NodeDp* prev = nullptr;
		NodeDp* temp = head;
		while (temp)
		{
			if (temp->node == targetNode)
			{
				targetNode->treeNode = nullptr;
				if (prev == nullptr)
				{
					head = temp->next;
					delete temp;
					return;
				}
				else
				{
					prev->next = temp->next;
					delete temp;
					return;
				}
			}
			else
			{
				prev = temp;
				temp = temp->next;
			}
		}
	}
	bool hasNoCorrespondingNodes()
	{
		return head == nullptr;
	}

	//to convert alphateic index to array index
	NAryNode*& Child(char ch)
	{
		if (ch >= 'A' && ch <= 'Z')
			return children[ch - 'A'];
		if (ch >= 'a' && ch <= 'z')
			return children[ch - 'a'];
		cout << "INVALID PARAMETER FOR N-ARY NODE CHILD" << endl;
		Node* temp = nullptr;
		cout << ch << temp->right->ch << endl;
		return children[ch];
	}
	int getNoOfChildren()
	{
		int n = 0;
		for (int a = 0; a < this->noOfChildren; a++)
		{
			if (this->children[a])
				n++;
		}
		return n;
	}

	int getNoOfCorrespondingNodes()
	{
		NodeDp* temp = head;
		int len = 0;
		while (temp)
		{
			len++;
			temp = temp->next;
		}
		return len;
	}


};
class NAryTree
{
	//preorder traversal
	void traverse(NAryNode*& current, char command = 'P', int level = 0)
	{
		if (current == nullptr)
			return;
		bool leaf = true;
		//print the character
		if (command == 'P' && current->ch != '\0')
		{
			for (int a = 0; a < level; a++)
			{
				cout << "  ";
			}
			cout << current->ch << endl;
		}


		for (int a = 0; a < current->noOfChildren; a++)
		{
			//find a child that exists
			if (current->children[a])
			{
				leaf = false;

				//for printing
				if (command == 'P')
				{
					if (current->ch != '\0')
					{
						traverse(current->children[a], command, level + 1);
					}
					else
						traverse(current->children[a], command);
				}

				//for deleting
				else if (command == 'D')
				{
					traverse(current->children[a], command);
					NAryNode* temp = current;
					current = nullptr;
					delete temp;
				}
			}
		}
		if (leaf)
		{
			//delete leaf
			if (command == 'D')
			{
				NAryNode* temp = current;
				current = nullptr;
				delete temp;
			}
		}
	}
public:
	NAryNode* root;
	NAryNode* current;

	NAryTree()
	{
		this->root = new NAryNode('\0');
		current = root;
	}
	~NAryTree()
	{
		deleteTree(this->root);
	}
	void reset()
	{
		current = root;
	}

	void print()
	{
		system("cls");
		this->traverse(this->root, 'P');
	}


	//delete the node and it's subtree
	void deleteTree(NAryNode*& rootNode)
	{
		this->traverse(rootNode, 'D');
	}


	//add a character under current NaryNode
	void addChar(Node* node, NAryNode*& current, String currentWord)
	{
		//reset current if a space is entered
		char ch = node->ch;
		if (ch == ' ')
		{
			current->endOfWord = true;
			current = root;
			return;
		}

		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			//if current doesn't have that character as a child
			if (!current->Child(ch))
				current->Child(ch) = new NAryNode(ch);		//add a child

			//move current to the newly added character
			NAryNode* prev = current;
			current = current->Child(ch);

			//store the address of the corresponding node in the tree's node
			current->addNode(node);

			//make the node point to it's corresponding tree node
			node->treeNode = current;

			//see if inserting in the middle/start of a word
			if (node->right && isAlphabet(node->right->ch))
			{
				NAryNode* temp = current;
				for (int a = 0; a < prev->noOfChildren; a++)
				{
					if (prev->children[a])
					{
						//find the tree node that corresponds to the notepad node's right
						NAryNode* nextNaryNode = prev->children[a];
						Node* nextNotepadNode = node->right;
						Stack<NAryNode*> nAryStack;

						//we traverse the word in the notepad and tree simultaneously
						while (nextNotepadNode && nextNaryNode && nextNaryNode->nodeExists(nextNotepadNode))
						{
							//remove the notepad node from under its current parent and attach it to the new parent(the newly inserted notepad node)
							nextNaryNode->subtractNode(nextNotepadNode);
							NAryNode* potentialDeletedNode = nextNaryNode;
							addChar(nextNotepadNode, temp, currentWord);
							nextNotepadNode = nextNotepadNode->right;
							if (!nextNotepadNode || !isAlphabet(nextNotepadNode->ch))
							{
								//if a NaryNode has become empty, push it on stack for deletion
								if (nextNaryNode->hasNoCorrespondingNodes())
									nAryStack.push(nextNaryNode);
								nextNaryNode = nullptr;
								break;
							}
							nextNaryNode = nextNaryNode->Child(nextNotepadNode->ch);
							if (potentialDeletedNode->hasNoCorrespondingNodes())
								nAryStack.push(potentialDeletedNode);
						}
						while (!nAryStack.isEmpty())
						{
							NAryNode* deleteThis = nAryStack.Top();
							nAryStack.pop();
							this->deleteTreeNode(deleteThis, currentWord);
						}
					}
				}
			}

		}
	}
	void addChar(Node* node, String currentWord)
	{
		addChar(node, this->current, currentWord);

	}

	//completely remove a tree node and attach its chidlren to its parent(use current word as a path guidance)
	void deleteTreeNode(NAryNode* target, String word)
	{
		if (!target || target == root)
			return;
		NAryNode* temp = root;
		int a = 0;

		//find the target node by following the word-path
		for (; a < word.length; a++)
		{
			if (temp && temp->Child(word.arr[a]) != target)
				temp = temp->Child(word.arr[a]);
			else
				break;
		}

		//if target is found
		if (isAlphabet(word.arr[a]) && temp->Child(word.arr[a]) == target)
		{
			NAryNode* toBeDeleted = target;

			//if curren=target, move current to it's parent
			if (this->current == target)
				this->current = temp;

			//remove the link between the target's parent and target
			temp->Child(word.arr[a]) = nullptr;

			//attach all subtrees of target to it's parent
			for (int b = 0; b < target->noOfChildren; b++)
			{
				attachSubtree(temp, target->children[b]);
			}

			//delete target
			delete toBeDeleted;
		}
	}

	void subtractNodepadNode(Node* node, String currentWord)
	{
		if (!node || !isAlphabet(node->ch))
			return;
		NAryNode* treeNode = node->treeNode;
		//remove the notepad node that the tree node points to
		if (treeNode)
		{
			treeNode->subtractNode(node);

			//to reconfigure the tree when a letter is removed from a whole word
			NAryNode* temp = root;
			int a = 0;
			//find parent of the NAryNode from which we are removing the notepadnode
			for (; a < currentWord.length; a++)
			{
				if (temp && temp->Child(currentWord.arr[a]) != treeNode)
					temp = temp->Child(currentWord.arr[a]);
				else
					break;
			}
			//if the parent is found
			if (temp && temp->Child(currentWord.arr[a]) == treeNode)
			{
				//shift current to it's parent
				if (current == treeNode)
				{
					current = temp;
				}
				//find if the notepadNode's right exists in one of the NaryNode's children
				for (int a = 0; a < treeNode->noOfChildren; a++)
				{
					if (treeNode->children[a])
					{
						//reattach the subtree
						NAryNode* nextNaryNode = treeNode->children[a];
						Node* nextNotepadNode = node->right;

						//if it does
						if (nextNaryNode->nodeExists(nextNotepadNode))
						{
							NAryNode* prev = treeNode;
							char index = a + 'A';

							//duplicate the remaining word under the to-be-subtracted treeNode's parent and remove from original location
							while (nextNaryNode->nodeExists(nextNotepadNode) && nextNaryNode->getNoOfCorrespondingNodes() >= 2)
							{
								nextNaryNode->subtractNode(nextNotepadNode);
								NAryNode* potentialDeletedNode = nextNaryNode;
								addChar(nextNotepadNode, temp, currentWord);
								nextNotepadNode = nextNotepadNode->right;
								if (!nextNotepadNode)
								{
									if (nextNaryNode->hasNoCorrespondingNodes())
										this->deleteTreeNode(nextNaryNode, currentWord);
									nextNaryNode = nullptr;
									prev = nullptr;
									break;
								}
								prev = nextNaryNode;
								index = nextNotepadNode->ch;
								nextNaryNode = nextNaryNode->Child(index);
								if (potentialDeletedNode->hasNoCorrespondingNodes())
									this->deleteTreeNode(potentialDeletedNode, currentWord);
							}
							if (prev)
								attachSubtree(temp, prev->Child(index));
						}
					}
				}
			}
			//if a treenode points to no notepad nodes, delete it
			if (treeNode->hasNoCorrespondingNodes())
				this->deleteTreeNode(treeNode, currentWord);
		}

	}

	//add a subtree to the children of a root
	bool attachSubtree(NAryNode*& parent, NAryNode*& child)
	{
		if (!child || child->ch == '\0')
			return false;

		char ch = child->ch;
		//if the parent doesn't have a child for that character, attach it directly
		if (parent->Child(ch) == nullptr)
		{
			parent->Child(ch) = child;
			child = nullptr;
			return true;
		}
		//if it does, attach the children of the child to the parent's child
		else
		{
			NAryNode* temp = child;
			for (int a = 0; a < child->noOfChildren; a++)
			{
				attachSubtree(parent->Child(ch), child->children[a]);
			}
			while (!child->hasNoCorrespondingNodes())
			{
				Node* firstNode = child->head->node;
				child->subtractNode(firstNode);
				parent->Child(ch)->addNode(firstNode);
			}
			child = nullptr;
			delete temp;
			return true;
		}
	}
	void adjustEnter(Node* node, bool doNotDoAnything, String currentWord)
	{
		//if enter pressed in middle of a word and it is not at x==1
		if (!doNotDoAnything && node->right && isAlphabet(node->right->ch))
		{
			Node* currentNode = node->right;
			NAryNode* treeNode = currentNode->treeNode;
			NAryNode* temp = root;
			Stack<NAryNode*> deletionStack;

			//take all characters after the cursor and create a new word for them in the tree(seperate from original word)
			while (currentNode && isAlphabet(currentNode->ch) && treeNode && treeNode->nodeExists(currentNode))
			{
				treeNode->subtractNode(currentNode);
				if (treeNode->hasNoCorrespondingNodes())
					deletionStack.push(treeNode);
				addChar(currentNode, temp, currentWord);

				currentNode = currentNode->right;
				if (currentNode && isAlphabet(currentNode->ch))
					treeNode = treeNode->Child(currentNode->ch);
				else
					treeNode = nullptr;
			}
			while (!deletionStack.isEmpty())
			{
				NAryNode* deleteThis = deletionStack.Top();
				deletionStack.pop();
				this->deleteTreeNode(deleteThis, currentWord);
			}
		}
	}

	void updateCurrent(Node* node)
	{
		if (node->treeNode)
			current = node->treeNode;
		else
			current = root;
	}
	void search()
	{
		String word;

		int x = 86;
		int y = 1;
		gotoxy(94, 0);
		char c = 'a';
		while (c != '\n')
		{
			c = cin.get();
			if (c == '\n' || c == ' ')
			{
				break;
			}
			if (isAlphabet(c))
				word += c;
		}

		NAryNode* temp = root;
		bool found = true;
		for (int a = 0; a < word.length; a++)		//take the temp pointer till the word's last NAryNode
		{
			if (isAlphabet(word.arr[a]) && temp)
				temp = temp->Child(word.arr[a]);
			else
			{
				found = false;
				break;
			}
		}
		//traverse all the words from the last character till the first and turn color on
		if (temp && found)
		{
			NodeDp* nodedp = temp->head;
			while (nodedp)
			{
				Node* currNode = nodedp->node;
				gotoxy(x, y);
				cout << "Line " << currNode->lineNumber << endl;
				y++;
				for (int a = 0; a < word.length; a++)
				{
					currNode->color = true;
					currNode = currNode->left;
				}
				nodedp = nodedp->next;
			}
		}
	}


	void wordCompletion(String currentWord, String& insertingWord, Node* current)
	{

		currentWord.wordSelectingHelper(current);
		int currWordLength = currentWord.length;
		if (currWordLength == 0)
			return;
		CONSOLE_SCREEN_BUFFER_INFO screen;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen);
		int y = (screen.srWindow.Bottom + 1) * 0.7;
		int x = ((screen.srWindow.Right + 1));
		y += 4;
		gotoxy(1, y);
		for (int a = 0; a < x; a++)
			cout << ' ';
		gotoxy(1, y);



		NAryNode* temp = root;
		for (int a = 0; a < currentWord.length; a++)
		{
			if (!temp)
				return;
			temp = temp->Child(currentWord.arr[a]);
		}
		currentWord--;
		helpingPrint(temp, currentWord, insertingWord, currWordLength);
	}
	void helpingPrint(NAryNode* temp, String currentWord, String& insertingWord, int currWordLength)
	{

		if (!temp || insertingWord.length > 0)
		{
			return;
		}
		currentWord += temp->ch;
		bool leaf = 1;
		for (int a = 0; a < temp->noOfChildren; a++)
		{
			if (temp->children[a])
			{
				leaf = 0;
				helpingPrint(temp->children[a], currentWord, insertingWord, currWordLength);
			}
		}
		if (leaf || temp->endOfWord && insertingWord.length == 0)
		{
			CONSOLE_SCREEN_BUFFER_INFO screen;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen);
			int y = (screen.srWindow.Bottom + 1) * 0.7;
			y += 3;
			gotoxy(0, y);
			cout << "                                                   ";
			gotoxy(0, y);
			cout << currentWord.arr << '\n';
			cout << "Insert this word?(1/0): ";
			cout << "           ";
			gotoxy(0, y + 1);
			cout << "Insert this word?(1/0): ";
			int input;
			cin >> input;
			if (cin.fail())
			{
				cin.clear();
				input = 0;
			}
			cin.ignore(1000, '\n');
			if (input == 1)
			{
				insertingWord = currentWord;
				insertingWord.removeFirstCharacters(currWordLength);
			}
		}
	}




};

struct chillNode;
struct chillNodeDp
{
	chillNode* node;
	chillNodeDp* next;

	chillNodeDp(chillNode* node = nullptr) : node(node), next(nullptr)
	{}
};

struct chillNode
{
	String s;
	chillNodeDp* head;

	chillNode(String s)
	{
		this->s = s;
		head = nullptr;
	}
	~chillNode()
	{
		while (head)
		{
			chillNodeDp* temp = head;
			head = head->next;
			delete temp;
		}
	}

	void addChild(chillNode* child)
	{
		if (!child)
			return;
		if (head == nullptr)
		{
			head = new chillNodeDp(child);
		}
		else
		{
			chillNodeDp* temp = head;

			//child already exists
			if (temp->node == child)
				return;
			while (temp->next)
			{
				//child already exists
				if (temp->node == child)
					return;
				temp = temp->next;
			}
			temp->next = new chillNodeDp(child);
		}
	}

};


class chilliMilliTree
{
public:
	chillNodeDp* head;
	chillNodeDp* current;

	chilliMilliTree()
	{
		head = nullptr;
		current = nullptr;
	}
	~chilliMilliTree()
	{
		while (head)
		{
			chillNodeDp* temp = head;
			delete temp->node;
			head = head->next;
			delete temp;
		}
	}
	bool isEmpty()
	{
		return head == nullptr;
	}
	chillNodeDp* wordExists(String word)
	{
		if (isEmpty())
			return nullptr;
		chillNodeDp* temp = head;
		while (temp)
		{
			if (temp->node->s == word)
				return temp;
			temp = temp->next;
		}
		return nullptr;
	}
	void addWord(String word)
	{
		if (word.length == 0)
			return;

		//insertion for first time
		if (isEmpty())
		{
			head = new chillNodeDp(new chillNode(word));
			current = head;
			return;
		}

		//see if a word already exists in CM tree
		chillNodeDp* temp = wordExists(word);

		//if it doesnt exist, add to the base linked list record of words(head)
		if (!temp)
		{
			chillNodeDp* insertionPoint = head;
			while (insertionPoint->next)
			{
				insertionPoint = insertionPoint->next;
			}
			insertionPoint->next = new chillNodeDp(new chillNode(word));
			temp = insertionPoint->next;
		}


		//temp points to the current inserted words node
		//make current point to temp
		current->node->addChild(temp->node);
		current = temp;
	}

	void displaySuggestions(String currentWord, String& insertingWord)
	{
		if (currentWord.length == 0)
			return;
		CONSOLE_SCREEN_BUFFER_INFO screen;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen);
		int y = (screen.srWindow.Bottom + 1) * 0.7;
		y += 3;

		chillNodeDp* temp = current->node->head;
		while (temp)
		{
			gotoxy(0, y);
			cout << "                                                   ";
			gotoxy(0, y);
			cout << temp->node->s.arr << '\n';
			cout << "Insert this word?(1/0): ";
			cout << "           ";
			gotoxy(0, y + 1);
			cout << "Insert this word?(1/0): ";
			int input;
			cin >> input;
			if (cin.fail())
			{
				cin.clear();
				input = 0;
			}
			cin.ignore(1000, '\n');
			if (input == 1)
			{
				insertingWord = temp->node->s;
				break;
			}
			else
			{
				temp = temp->next;
			}
		}
	}
	void setCurrent(String word)
	{
		chillNodeDp* temp = wordExists(word);
		if (temp)
			this->current = temp;
	}

};



void SetConsoleColor(int color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

//Parent entry class that will store 1 insertion/deletion operation in stack
class Entry
{
public:
	NodeDp* head;
	bool isInsertion;
	Entry* top;
	Entry* bottom;


	Entry()
	{
		top = nullptr;
		bottom = nullptr;
		head = nullptr;
	}
	virtual ~Entry()
	{}

	//updates endNode pointer for insertion
	virtual void add(Node* node)
	{}
	Node* getStartingNode()
	{
		if (!head)
			return nullptr;
		else
			return head->node;
	}
	bool isEmpty()
	{
		if(this==nullptr)
			return true;
		return head == nullptr;
	}

	NodeDp* nodeExists(Node* node)
	{
		if (isEmpty())
			return nullptr;
		else
		{
			NodeDp* curr = head;
			while (curr)
			{
				if (curr->node == node)
					return curr;
				curr = curr->next;
			}
		}
		return nullptr;
	}
};


struct Insertion : public Entry
{
	
	Insertion() : Entry()
	{
		this->isInsertion = true;
		head = nullptr;
	}

	void add(Node* node)
	{
		NodeDp* temp = new NodeDp(node);
		temp->backup = node->ch;
		if (head== nullptr)
		{
			head = temp;
		}
		else
		{
			temp->next = head;
			head = temp;
		}
	}
	void decrementEndingNode()
	{
		if (isEmpty())
			return;
		NodeDp* temp = head;
		head = head->next;
		delete temp;
	}
};


struct Deletion : public Entry
{
	Node* insertionPoint;
	bool resetInsertion;
	
	Deletion() : Entry()
	{
		isInsertion = false;
		head = nullptr;
		insertionPoint = nullptr;
		resetInsertion = false;
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
	void deleteNodes(Node* notepadHead)
	{
		return;
		NodeDp* temp = head;
		while (temp)
		{
			if(temp->node!=notepadHead)
				delete temp->node;
			temp = temp->next;
		}
	}
	void add(Node* node)
	{

		NodeDp* temp = new NodeDp(node);
		temp->backup = node->ch;
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
	
	void setInsertionPoint()
	{
		if (!isEmpty())
		{
			if (head->node)
			{
				if (head->node->left)
				{
					insertionPoint = head->node->left;
					resetInsertion = false;
				}
				else if (head->node->up)
				{
					insertionPoint = head->node->up;
					while (insertionPoint->right)
					{
						insertionPoint = insertionPoint->right;
					}
					resetInsertion = true;
				}
				else if (!insertionPoint)
				{
					insertionPoint = head->node;
					incrementStartingNode();
					resetInsertion = false;
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
	int currSize;

	stack()
	{
		topEntry = nullptr;
		lastEntry = nullptr;
		insertionActivated = 0;
		deletionActivated = 0;
		currSize = 0;
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
	void clear()
	{
		lastEntry = topEntry;
		while (lastEntry)
		{
			lastEntry = topEntry->bottom;
			delete topEntry;
			topEntry = lastEntry;
		}
		topEntry = nullptr;
		lastEntry = nullptr;
		insertionActivated = 0;
		deletionActivated = 0;
		currSize = 0;
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
	void removeLastEntry(Node* notepadHead)
	{
		if (lastEntry == nullptr)
			return;
		if (lastEntry == topEntry)
			topEntry = nullptr;

		Entry* temp = lastEntry;
		lastEntry = lastEntry->top;
		if (lastEntry)
			lastEntry->bottom = nullptr;
		if (!(temp->isInsertion))
			((Deletion*)temp)->deleteNodes(notepadHead);
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


	void activateInsertion(Node* notepadHead)
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
				removeLastEntry(notepadHead);
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
	void activateDeletion(Node* notepadHead)
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
				removeLastEntry(notepadHead);
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
			if (topEntry->isEmpty())
				return;
			NodeDp* curr = topEntry->head;
			while (curr)
			{
				curr->node->ch == ' ' ? cout << "_" : curr->node->ch == '\n' ? cout << "\\n" : cout << curr->node->ch;
				curr = curr->next;
			}
			cout << "                       " << endl << "                 ";
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
	filesystem::path currentFile;

	int maxX;
	int maxY;

	//cursor coordinates(point to current pointer)
	int x;
	int y;


	//constructor destructor
	Notepad(int maxX, int maxY, stack& Stack, stack& redoStack) : Stack(Stack), redoStack(redoStack)
	{
		head = new Node('\n');
		head->newLine = true;
		current = head;
		currRow = current;
		this->maxX = maxX;
		this->maxY = maxY;
		currentFile.clear();

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
	bool hasFile() {
		return !currentFile.empty();
	}

	void linkToFile(filesystem::path& filePath) {
		currentFile = filePath;
	}
	void unlinkFile() {
		currentFile.clear();
	}

	void clear()
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
		Stack.clear();
		redoStack.clear();
		head = new Node('\n');
		head->newLine = true;
		current = head;
		currRow = current;
		this->maxX = maxX;
		this->maxY = maxY;

		x = 1;
		y = 1;
	}

	//moves current to the specified node
	void moveCurrentTo(Node* node, bool xIsAtStartOfRow = 0)
	{
		if (!node)
			return;

		char ch = node->ch;
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
			bool notFound = true;
			if (curr->right)
			{
				curr = curr->right;
				c.x++;
				notFound = false;
			}
			else if (currRow->down)
			{
				currRow = currRow->down;
				curr = currRow;
				c.x = 1;
				c.y++;
				notFound = false;
			}
			if (notFound)
			{
				current = head;
				currRow = head;
				return { 1,1 };
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
	bool moveRight()
	{
		if (current)
		{
			if (x == 1 && current->left == nullptr && current->ch != '\n')
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
		else if (current && x == 1 && current->left == nullptr)
		{
			if (currRow->up)
			{
				currRow = currRow->up;
				current = endOfRow(currRow);
				if (current->ch == '\n')
					x = currentCoordinates().x;
				else
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
		row1->left = nullptr;
		if(row2)
			row2->left = nullptr;

		Node* temp = head;
		while (temp)
		{
			temp->lineNumber = 1;
			temp->up = nullptr;
			temp = temp->right;
		}
		int ln = 0;
		while (row2)
		{
			ln++;
			Node* curr1 = row1;
			Node* curr2 = row2;
			row1->left = nullptr;
			row2->left = nullptr;

			bool curr1End = 0;
			bool curr2End = 0;

			while (!curr1End || !curr2End)
			{
				curr1->lineNumber = ln;
				curr2->lineNumber = ln + 1;
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
			if (oneTimer && rowStart->down && rowStart->down->newLine == false)
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
		if (nextRow == nullptr || nextRow->newLine == true)
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
		if (curr->ch == ' ' || curr->ch == '\n')
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
					moveCurrentTo(tempCurrent, tempX == 1);
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
		if (currRow->down && currRow->down->newLine == false)
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
				x = 1;
				y = 1;
				head->treeNode = nullptr;
				ret = true;
			}
			head->left = nullptr;
			head->up = nullptr;
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
					NodeDp* target = currentEntry->nodeExists(node);
					if (target)
					{
						Node* tempCurrent = current;
						int tempX = x;

						moveCurrentTo(node);
						if (moveLeft());
						else
							moveRight();

						target->node= current;
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
					NodeDp* target = currentEntry->nodeExists(node);
					if (target)
					{
						Node* tempCurrent = current;
						int tempX = x;

						moveCurrentTo(node);
						if (moveLeft());
						else
							moveRight();

						target->node = current;
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
		if (!Stack.isEmpty() && !Stack.topEntry->isInsertion && ((Deletion*)Stack.topEntry)->insertionPoint == node)
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
		{
			dontDelete = true;
		}
		if (!Stack.isEmpty() && !Stack.topEntry->isInsertion && ((Deletion*)Stack.topEntry)->insertionPoint == node)
		{
			dontDelete = true;
		}

		if (!dontDelete)
			delete node;
		reconnectAllVertical();

	}

	//delete text
	void backSpace(bool isFromMain = false)
	{
		if (!current || (x == 1 && y == 1 && head->ch != '\n'))
			return;

		//backspace at start of line
		if (x == 1 && current->ch != '\n')
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
				Stack.activateDeletion(this->head);
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
				if(!Stack.topEntry->isEmpty())
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
		removeNode(temp, 'L');

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
	bool insert(char ch, Node* optional = nullptr)
	{
		if (current && current->ch == '\n')
		{
			
			current->ch = ch;
			x++;
			return true;
		}

		Node* temp;
		if (optional && optional != current)
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
					if (!optional)
						delete temp;
					insert(ch, optional);
				}
				//charavter is a space, add it to next line
				else
				{
					//if next line exists and isnt a new paragraph
					if (currRow->down && currRow->down->newLine == false)
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

		static bool nextTimeClearSearch = 0;
		if (nextTimeClearSearch)
		{
			for (int a = 1; a < 15; a++)
			{
				gotoxy(86, a);
				cout << "                   ";
			}
			nextTimeClearSearch = 0;
		}
		CONSOLE_SCREEN_BUFFER_INFO screen;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen);
		int yC = (screen.srWindow.Bottom + 1) * 0.7;
		yC += 3;
		gotoxy(0, yC);
		cout << "                                                   ";
		gotoxy(0, yC+1);
		cout << "                                                   ";
		gotoxy(0, yC + 2);
		cout << "                                                   ";
		gotoxy(0, yC + 3);
		cout << "                                                   ";
		gotoxy(0, yC + 4);
		cout << "                                                   ";


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
					if (current->color)
					{
						SetConsoleColor(2);
					}
					cout << current->ch;
					if (current->color)
					{
						current->color = false;
						SetConsoleColor(7);
						nextTimeClearSearch = 1;
					}
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
		for (; rowNum <= max && rowNum <= maxY; rowNum++)
		{
			int colNum = 1;
			gotoxy(colNum, rowNum);
			for (int a = colNum; a <= maxX; a++)
			{
				cout << " ";
			}
		}
		gotoxy(94, 0);
		cout << "                         ";
		// After printing, move the cursor back to its original position
		gotoxy(x, y);
	}

	void undo()
	{
		if (Stack.isEmpty())
			return;
		while (Stack.topEntry->isEmpty())
			Stack.pop();
		if (Stack.isEmpty())
			return;

		if (Stack.topEntry->isInsertion)
		{
			Stack.deactivate();


			///////////////// adding as deletion to redo stack ////////////////////
			/**/redoStack.activateDeletion(this->head);
			/**/NodeDp* curr = Stack.topEntry->head;
			/**/while (curr)
			/**/{
			/**/	redoStack.addToDeletion(curr->node);
			/**/	curr = curr->next;
			/**/
			/**/}
			/**/redoStack.deactivate();
			///////////////// adding as deletion to redo stack ////////////////////

			
			while (!Stack.topEntry->isEmpty())
			{
				moveCurrentTo(Stack.topEntry->head->node);
				((Insertion*)Stack.topEntry)->decrementEndingNode();
				backSpace();
			}
			if (((Deletion*)redoStack.topEntry)->resetInsertion)
			{
				if (((Deletion*)redoStack.topEntry)->insertionPoint->right)
					((Deletion*)redoStack.topEntry)->insertionPoint = ((Deletion*)redoStack.topEntry)->insertionPoint->right;
			}
			Stack.pop();
		}
		else if (!Stack.topEntry->isInsertion)
		{

			Stack.deactivate();

			///////////////// adding as insertion to redo stack ////////////////////
			/**/redoStack.activateInsertion(this->head);									/**/
			/**/NodeDp* curr = Stack.topEntry->head;				/**/
			/**/while (curr)													/**/
				/**/ {																/**/
				/**/	redoStack.addToInsertion(curr->node);						/**/
				/**/	curr = curr->next;											/**/
				/**/
			}
			/**/redoStack.deactivate();
			///////////////// adding as insertion to redo stack ////////////////////



			if (((Deletion*)Stack.topEntry)->insertionPoint)
				moveCurrentTo(((Deletion*)Stack.topEntry)->insertionPoint);
			if (!Stack.topEntry->isEmpty() && Stack.topEntry->getStartingNode()->newLine == true)
			{
				Stack.topEntry->getStartingNode()->newLine = false;
				if (current->ch != '\n')
					insert(' ');
				else
					enter();
			}

			while (!Stack.topEntry->isEmpty())
			{
				Node* temp = Stack.topEntry->head->node;
				char backup = Stack.topEntry->head->backup;
				((Deletion*)Stack.topEntry)->incrementStartingNode();
				if (temp->ch != '\n')
					insert(temp->ch, temp);
				else
					insert(backup);
			}
			Stack.pop();
		}
	}























	void redo()
	{
		if (redoStack.isEmpty())
			return;
		while (redoStack.topEntry->isEmpty())
			redoStack.pop();
		if (redoStack.isEmpty())
			return;


		if (redoStack.topEntry->isInsertion)
		{
			redoStack.deactivate();


			///////////////// adding as deletion to UNDO stack ////////////////////
			/**/Stack.activateDeletion(this->head);
			/**/NodeDp* curr = Stack.topEntry->head;
			/**/while (curr)
			/**/ {
			/**/	Stack.addToDeletion(curr->node);
			/**/	curr = curr->next;
			/**/
			/**/
			/**/ }
			/**/Stack.deactivate();
			///////////////// adding as deletion to UNDO stack ////////////////////


			while (!redoStack.topEntry->isEmpty())
			{
				moveCurrentTo(redoStack.topEntry->head->node);
				((Insertion*)redoStack.topEntry)->decrementEndingNode();
				backSpace();
			}
			if (((Deletion*)Stack.topEntry)->resetInsertion)
			{
				if (((Deletion*)Stack.topEntry)->insertionPoint->right)
					((Deletion*)Stack.topEntry)->insertionPoint = ((Deletion*)Stack.topEntry)->insertionPoint->right;
			}
			redoStack.pop();
		}
		else if (!redoStack.topEntry->isInsertion)
		{

			redoStack.deactivate();

			///////////////// adding as insertion to redo stack ////////////////////
			/**/Stack.activateInsertion(this->head);										/**/
			/**/NodeDp* curr = redoStack.topEntry->head;						/**/
			/**/while (curr)													/**/
			/**/ {																/**/
			/**/	Stack.addToInsertion(curr->node);							/**/
			/**/	curr = curr->next;											/**/
			/**/
			/**/ }
			/**/Stack.deactivate();
			///////////////// adding as insertion to redo stack ////////////////////

			if (((Deletion*)redoStack.topEntry)->insertionPoint)
				moveCurrentTo(((Deletion*)redoStack.topEntry)->insertionPoint);
			if (!redoStack.topEntry->isEmpty() && redoStack.topEntry->getStartingNode()->newLine == true && current->ch != '\n')
				insert(' ');
			while (!redoStack.topEntry->isEmpty())
			{
				Node* temp = redoStack.topEntry->head->node;
				char backup = redoStack.topEntry->head->backup;
				((Deletion*)redoStack.topEntry)->incrementStartingNode();
				if (temp->ch != '\n')
					insert(temp->ch, temp);
				else
					 insert(backup);
			}
			redoStack.pop();

		}
	}

};



struct Menu
{
private:
	filesystem::path inputFileName() {
		filesystem::path filePath;
		char ch;
		cout << "Enter file name (end by '.'): ";

		while (std::cin.get(ch) && ch != '.') {
			if(ch!='\n')
				filePath += ch;  // Construct file path one character at a time
		}

		filePath += ".txt";  // Append the .txt extension
		return filePath;
	}

	void saveNotepadToFile(ostream& file, Notepad& notepad)
	{
		Node* currRow = notepad.head;
		while (currRow)
		{
			Node* curr = currRow;
			while (curr)
			{
				//press enter
				if (curr->newLine && curr!=notepad.head)
				{
					file.put('.');
				}

				//no need to save \n as enter will deal with it
				if(curr->ch!='\n')
					file.put(curr->ch);

				curr = curr->right;
			}
			currRow = currRow->down;

			//space for word wrapping
			if (currRow && currRow->newLine == false)
			{
				file.put(' ');
			}
		}
	}
	void loadFileToNotepad(ifstream& file, Notepad& notepad)
	{
		char ch;
		while (file.get(ch))
		{
			if (ch == '.') 
			{
				notepad.enter(); 
			}
			else 
			{
				notepad.insert(ch);  
			}
		}
	}
public:
	
	int display()
	{
		int choice=0;
		do {
			system("cls");
			cout << "________Main Menu_______\n";
			cout << "1. Create New File\n";
			cout << "2. Save File\n";
			cout << "3. Load File\n";
			cout << "4. Exit\n";
			cout << "Enter your choice: ";
			cin >> choice;

			if (choice < 1 || choice > 4) {
				cout << "Invalid choice, please try again.\n";
				Sleep(1000); 
			}
		} while (choice < 1 || choice > 4);
		return choice;

	}
	bool createFile(Notepad& notepad)
	{
		filesystem::path filePath = inputFileName();
		ofstream file(filePath);  

		if (file.is_open()) {
			notepad.linkToFile(filePath);  
			file.close();
			return true;
		}
		else {
			cout << "File creation error"<<endl;
			Sleep(1000);
			file.close();
			return false;
		}
		
	}

	void saveFile(Notepad& notepad)
	{
		if (!notepad.hasFile())
		{
			cout << "No file loaded." << endl;
			Sleep(1000);
			return;
		}
		ofstream file(notepad.currentFile, std::ios::out | std::ios::trunc); 
		if (file.is_open()) {
			saveNotepadToFile(file, notepad); 
			cout<<"Saved."<<endl;
			Sleep(1000);
		}
		else {
			cout << "Failed to open file."<<endl;
			Sleep(1000);
		}
		file.close();
	}

	bool loadFile(Notepad& notepad)
	{
		filesystem::path filePath = inputFileName();  
		if (filesystem::exists(filePath)) 
		{  	
			ifstream file(filePath, ios::in);  
			if (file.is_open()) {
				notepad.clear();  
				loadFileToNotepad(file, notepad); 
				notepad.linkToFile(filePath); 
				file.close();
				return true;
			}
			else {
				cout << "Failed to open file."<<endl;
				Sleep(1000);
				return false;
			}

			
		}
		else {
			cout << "File does not exist."<<endl;
			Sleep(1000);  
			return false;
		}
	}

};


void displayNotePadLayout(int maxX, int maxY)
{
	gotoxy(0, 0);
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
		cout << '|';
		if (a == 0)
			cout << "Search:";
		cout << endl;
	}
	for (int b = 0; b < maxX + 2; b++)
	{
		if (b == 0 || b == maxX + 1)
			cout << '|';
		else
			cout << '_';
	}
	cout << endl << " Word suggestions:";

}

int main(int argc, char* argv[]) {

	system("cls");
	HANDLE  rhnd = GetStdHandle(STD_INPUT_HANDLE);  // handle to read console

	DWORD Events = 0;     // Event count
	DWORD EventsRead = 0; // Events read from console

	CONSOLE_SCREEN_BUFFER_INFO screen;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen);//get info about console screen
	const int maxX = (screen.srWindow.Right + 1) * 0.7;  
	const int maxY = (screen.srWindow.Bottom + 1) * 0.7; //set boundaries for cursor

	Menu menu;
	stack shtack;
	stack redoStack;
	Notepad notepad(maxX, maxY, shtack, redoStack);
	NAryTree nAryTree;
	String currentWord;
	String prevWord;
	chilliMilliTree CMtree;
	

	bool Running = true;
	bool menuRunning = true;
	

	while (Running) 
	{

		bool createdSuccessfully = false;
		bool loadedSuccessfully = false;
		if (menuRunning)
		{
			int c = menu.display();
			
			switch (c)
			{
			case 1:
				createdSuccessfully = menu.createFile(notepad);
				break;

			case 2:
				menu.saveFile(notepad);
				break;

			case 3:
				loadedSuccessfully = menu.loadFile(notepad);
				break;

			case 4:
				return 0;
			}
		}
		

		if (createdSuccessfully || loadedSuccessfully)
		{
			system("cls");
			menuRunning = false;
			if(createdSuccessfully)
				notepad.clear();
			bool notepadRunning = true;
			displayNotePadLayout(maxX, maxY);
			int x = 1, y = 1;
			gotoxy(x, y);
			notepad.print();
			//programs main loop
			while (notepadRunning)
			{

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
								shtack.deactivate();
								if (currentWord.length > 0)
									prevWord = currentWord;
								currentWord.newWord(notepad.current);
								nAryTree.updateCurrent(notepad.current);
								break;

							case VK_DOWN: //down
								notepad.moveDown();
								shtack.deactivate();
								if (currentWord.length > 0)
									prevWord = currentWord;
								currentWord.newWord(notepad.current);
								nAryTree.updateCurrent(notepad.current);
								break;

							case VK_RIGHT: //right
								notepad.moveRight();
								shtack.deactivate();
								if (currentWord.length > 0)
									prevWord = currentWord;
								currentWord.newWord(notepad.current);
								nAryTree.updateCurrent(notepad.current);
								break;

							case VK_LEFT: //left
								notepad.moveLeft();
								shtack.deactivate();
								if (currentWord.length > 0)
									prevWord = currentWord;
								currentWord.newWord(notepad.current);
								nAryTree.updateCurrent(notepad.current);
								break;


							case VK_ESCAPE:
								notepadRunning = false;
								menuRunning = true;
								currentWord.newWord(notepad.current);
								break;


							default:
								//ignore this
								notepad.currRow = notepad.startOfRow(notepad.current);
								notepad.head = notepad.currRow;
								while (notepad.head->up)
								{
									notepad.head = notepad.head->up;
								}

								//input
								int keyCode = eventBuffer->Event.KeyEvent.uChar.AsciiChar;
								if (keyCode == ' ' || (keyCode >= 'A' && keyCode <= 'Z') || (keyCode >= 'a' && keyCode <= 'z')) {

									if (keyCode == ' ')
									{
										String temp;
										temp.wordSelectingHelper(notepad.current);
										CMtree.addWord(temp);

										nAryTree.adjustEnter(notepad.current, notepad.x == 1, currentWord);
									}
									//insert
									if (notepad.insert(static_cast<char>(keyCode)))
									{

										/////////////////////N-ary TREE/////////////////////////////
										nAryTree.addChar(notepad.current, currentWord);
										////////////////////current word string ////////////////////
										if(currentWord.length>0)
											prevWord = currentWord;
										currentWord.newWord(notepad.current);


										//////////////////////UNDO REDO/////////////////////////////
										//actiavate insertion entry in stack if a char is pressed
										if (shtack.insertionActivated == false && keyCode != ' ')
										{
											shtack.activateInsertion(notepad.head);
											shtack.addToInsertion(notepad.current);
										}
										//if insertion is activated and a space isn't entered, add the new node to the insertion entry
										else if (shtack.insertionActivated == true && keyCode != ' ')
										{
											shtack.addToInsertion(notepad.current);
										}
										//if space is entered, finish taking input in undo stack (1 word completed)
										else if (shtack.insertionActivated == true && keyCode == ' ')
										{
											shtack.addToInsertion(notepad.current);
											shtack.deactivate();
										}


									}
									redoStack.clear();
								}

								//delete
								else if (keyCode == 8)
								{
									nAryTree.subtractNodepadNode(notepad.current, currentWord);
									notepad.backSpace(true);
									nAryTree.updateCurrent(notepad.current);
									redoStack.clear();
									if (currentWord.length > 0)
										prevWord = currentWord;
									currentWord.newWord(notepad.current);
									if (notepad.current->ch == ' ')
									{
										Node* temp = notepad.current;
										while (temp && !isAlphabet(temp->ch))
										{
											temp = temp->left;
										}
										if (temp)
										{
											prevWord.newWord(temp);
											CMtree.setCurrent(prevWord);
										}
									}
								}

								//enter
								else if (keyCode == 13)
								{
									String temp;
									temp.wordSelectingHelper(notepad.current);
									CMtree.addWord(temp);
									nAryTree.adjustEnter(notepad.current, notepad.x == 1, currentWord);
									shtack.deactivate();
									notepad.enter();
									nAryTree.updateCurrent(notepad.current);
									redoStack.clear();
									if (currentWord.length > 0)
										prevWord = currentWord;
									currentWord.newWord(notepad.current);
								}

								//search
								else if (keyCode == 6)
								{
									nAryTree.search();
								}

								//@==NAry word Completion, *==chilli milli word completion
								else if (keyCode == 64 || (keyCode == 42 && !isAlphabet(notepad.current->ch)))
								{
									String ins;
									if (keyCode == 64)
										nAryTree.wordCompletion(currentWord, ins, notepad.current);
									else if (keyCode == 42)
										CMtree.displaySuggestions(prevWord, ins);
									for (int a = 0; a < ins.length; a++)
									{
										//insert
										if ((isAlphabet(ins.arr[a]) || ins.arr[a]==' ') && notepad.insert(ins.arr[a]))
										{

											/////////////////////N-ary TREE/////////////////////////////
											nAryTree.addChar(notepad.current, currentWord);
											////////////////////current word string ////////////////////
											currentWord.newWord(notepad.current);


											//////////////////////UNDO REDO/////////////////////////////
											//actiavate insertion entry in stack if a char is pressed
											if (shtack.insertionActivated == false && keyCode != ' ')
											{
												shtack.activateInsertion(notepad.head);
												shtack.addToInsertion(notepad.current);
											}
											//if insertion is activated and a space isn't entered, add the new node to the insertion entry
											else if (shtack.insertionActivated == true && keyCode != ' ')
											{
												shtack.addToInsertion(notepad.current);
											}
											//if space is entered, finish taking input in undo stack (1 word completed)
											else if (shtack.insertionActivated == true && keyCode == ' ')
											{
												shtack.addToInsertion(notepad.current);
												shtack.deactivate();
											}


										}
										redoStack.clear();
									}
								}
								
								//print N-ary tree using '/'
								else if (keyCode == 47)
								{
									nAryTree.print();
									cin.get();
									cin.ignore();
									displayNotePadLayout(maxX, maxY);
								}



								//print notepad
								notepad.currRow = notepad.startOfRow(notepad.current);
								notepad.head = notepad.currRow;
								while (notepad.head->up)
								{
									notepad.head = notepad.head->up;
								}
								notepad.print();								
								break;


								////undo
								//else if (keyCode == 44)
								//{
								//	notepad.undo();
								//	nAryTree.reset();
								//	currentWord.empty();
								//}

								////redo
								//else if (keyCode == 46)
								//{
								//	notepad.redo();
								//	nAryTree.reset();
								//	currentWord.empty();
								//}
							}
						}

					} // end EventsRead loop

				}
			}
		}

	} // end program loop

	return 0;
}