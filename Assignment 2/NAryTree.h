#pragma once
#include<iostream>
#include<Windows.h>
using namespace std;

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

//notepad Node
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
	NodeDp* head;//for storing the nodes in the notepad that correspond to this treeNode
	const static int noOfChildren = 26;
	NAryNode* children[noOfChildren];

	NAryNode(char ch)
	{
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

	//for storing the nodes that correspond to that tree Node
	void addNode(Node* node)
	{
		if (head == nullptr)
		{
			head = new NodeDp(node);
		}
		else
		{
			head->next = new NodeDp(node);
		}
	}

	//to convert alphateic index to array index
	NAryNode*& Child(char ch)
	{
		if(ch>='A' && ch<='Z')
			return children[ch - 'A'];
		if (ch >= 'a' && ch <= 'z')
			return children[ch - 'a'];
		cout << "INVALID PARAMETER FOR N-ARY NODE CHILD" << endl;
		return children[ch];
	}


};
class NAryTree
{
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

	//preorder traversal
	void traverse(NAryNode*& current,char command = 'P', int level=0)
	{
		if (current == nullptr)
			return;
		bool leaf = true;
		//print the character
		if (command == 'P' && current->ch!='\0')
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
						traverse(current->children[a], command, level+1);
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

	void print()
	{
		system("cls");
		traverse(root, 'P');
		cin.ignore(10000, '\n');
		cin.get();
	}
	
	//delete the node and it's subtree
	void deleteTree(NAryNode*& rootNode)
	{
		this->traverse(rootNode, 'D');
	}


	//add a character under current
	void addChar(Node* node)
	{

		//reset current if a space is entered
		char ch = node->ch;
		if (ch == ' ')
		{
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
		}
	}

	//completely remove a tree node and attach its chidlren to its parent(use current word as a path guidance)
	void deleteTreeNode(NAryNode* target, String word)
	{
		NAryNode* temp = root;
		int a = 0;
		for (; a < word.length; a++)
		{
			if (temp && temp->Child(word.arr[a]) != target)
				temp = temp->Child(word.arr[a]);
			else
				break;
		}
		if (temp->Child(word.arr[a]) == target)
		{
			NAryNode* toBeDeleted = target;
			for (int b = 0; b < target->noOfChildren; b++)
			{
				attachSubtree(temp, target->children[b]);
			}
			delete toBeDeleted;
		}
	}

	//add a subtree to the children of a root
	bool attachSubtree(NAryNode*& parent, NAryNode*& child)
	{
		if (!child)
			return false;
		char ch = child->ch;
		if (ch == '\0')
			return false;
		//if the parent doesn't have a child for that character, attach it directly
		if (parent->Child(ch) == nullptr)
		{
			parent->Child(ch) = child;
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
			child = parent->Child(ch);
			delete temp;
			return true;
		}
	}


};

