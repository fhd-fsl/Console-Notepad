#pragma once
#include<iostream>
#include<Windows.h>
using namespace std;


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

//notepad note double pointer
struct NodeDp
{
	Node* node;
	NodeDp* next;
	char backup;

	NodeDp(Node* node = nullptr)
	{
		this->node = node;
		this->next = nullptr;
	}
};


//N-ary tree node
struct NAryNode
{
	char ch;
	NodeDp* head;
	const static int noOfChildren = 26;
	NAryNode* children[noOfChildren];

	NAryNode(char ch = '\0')
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

	//for storing all the nodes that correspond to that character
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
		this->root = new NAryNode();
		current = root;
	}
	~NAryTree()
	{
		traverse(this->root, 'D');
	}
	void reset()
	{
		current = root;
	}

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
			//delete lead
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
		/*COORD c = { 1,25};
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);*/
		system("cls");
		traverse(root, 'P');
		cin.ignore(10000, '\n');
		cin.get();
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

};

