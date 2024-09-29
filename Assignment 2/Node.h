#pragma once
struct Node
{
	char ch;
	Node* up;
	Node* down;
	Node* left;
	Node* right;


	Node(char ch='\0') :ch(ch)
	{
		up = nullptr;
		down = nullptr;
		left= nullptr;
		right= nullptr;

	}
};

