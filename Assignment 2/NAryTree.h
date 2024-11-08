#pragma once
#include<iostream>
#include<Windows.h>
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


	Node(char ch = '\0') :ch(ch)
	{
		up = nullptr;
		down = nullptr;
		left = nullptr;
		right = nullptr;
		treeNode = nullptr;
		newLine = 0;
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
	void newWord(Node* current)
	{
		this->empty();
		if (!current || current->ch == ' ' || current->ch == '\n')
			return;
		while (current->left != nullptr && current->left->ch != ' '  && current->left->ch != '\n')
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

	void empty()
	{
		delete arr;
		this->length = 0;
		this->arr = new char[length + 1];
		this->arr[length] = '\0';
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
		if(ch>='A' && ch<='Z')
			return children[ch - 'A'];
		if (ch >= 'a' && ch <= 'z')
			return children[ch - 'a'];
		cout << "INVALID PARAMETER FOR N-ARY NODE CHILD" << endl;
		Node* temp = nullptr;
		cout <<ch<< temp->right->ch << endl;
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

	

	void printSuggestion(String currentWord)
	{

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
		helpingPrint(temp, currentWord);
	}
	void helpingPrint(NAryNode* temp, String currentWord)
	{
		if (!temp)
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
				helpingPrint(temp->children[a], currentWord);
			}
		}
		if (leaf || temp->endOfWord)
		{
			cout << currentWord.arr << ' ';
		}
	}

	
	//delete the node and it's subtree
	void deleteTree(NAryNode*& rootNode)
	{
		this->traverse(rootNode, 'D');
	}


	//add a character under current NaryNode
	void addChar(Node* node)
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
			current = current->Child(ch);

			//store the address of the corresponding node in the tree's node
			current->addNode(node);

			//make the node point to it's corresponding tree node
			node->treeNode = current;
		}
	}
	void addChar(Node* node, NAryNode*& optionalParent)
	{
		if (!optionalParent)
			return;
		//reset current if a space is entered
		char ch = node->ch;
		if (ch == ' ')
		{
			optionalParent->endOfWord = true;
			optionalParent = root;
			return;
		}

		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			//if optionalParent doesn't have that character as a child
			if (!optionalParent->Child(ch))
				optionalParent->Child(ch) = new NAryNode(ch);		//add a child

			//move optionalParent to the newly added character
			optionalParent = optionalParent->Child(ch);

			//store the address of the corresponding node in the tree's node
			optionalParent->addNode(node);

			//make the node point to it's corresponding tree node
			node->treeNode = optionalParent;
		}
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
								char index=a+'A';

								//duplicate the remaining word under the to-be-subtracted treeNode's parent and remove from original location
								while (nextNaryNode->nodeExists(nextNotepadNode) && nextNaryNode->getNoOfCorrespondingNodes() >= 2)
								{
									nextNaryNode->subtractNode(nextNotepadNode);
									NAryNode* potentialDeletedNode = nextNaryNode;
									addChar(nextNotepadNode, temp);
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
								if(prev)
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
		if (!child || child->ch=='\0')
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

	void updateCurrent(Node* node)
	{
		if (node->treeNode)
			current = node->treeNode;
		else
			current = root;
	}

};

