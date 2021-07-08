/*********************************************************************
 * Program Name: Huffman.cpp                                         *
 * Authors NAme: Ivana Louis                                         *
 * Due Date: Apr. 26th, 2021                                         *
 * Purpose: This code takes in a file called freq.txt that           *
 * contains data on the characters in some cleartext file in         *
 * the form of each character's non-zero frequency of occurrence     *
 * in the cleartext file. You can assume that freq.txt contains only * 
 * characters from the standard ASCII 128 character set and that the * 
 * results are ordered by the ASCII integer for each character.      *
 * The end-of-line character in a text file is a non-printable line  *
 * feed character, with ASCII code 10, and will be written in the    *
 * freq.txt file as LF. The program will create a "code table" that  *
 * gives each character and its binary Huffman code and saves this   *
 * information in an external file called codetable.txt where each   *
 * line of the file is a character and its code, ordered by the ASCII*
 * value of the character                                            *
 ********************************************************************/

// Include libraries that will be needed to run the code.
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>

// declare your vectors
std::vector<char> elements;
std::vector<int> freq;
std::vector<struct encoding*> code;


// create the structs you will need. These stucts can be used to store elements
// of different data types. We use these to create instances

// Huffman tree node
struct node {
    node(char val, int f) : left(nullptr), right(nullptr), val(val), f(f) {}
    char val;
    int f;
    node *left, *right;
};

// compare the nodes
struct eval {
    bool operator() (node* l, node* r) {
        return (l->f > r->f);
    }
};

// encodes
struct encoding {
    encoding(char val, std::string encode) : val(val), encode(encode) {}
    char val;
    std::string encode;
};

// structure to sort ASCII values
struct sortASCII
{
    inline bool operator() (const encoding* he1, const encoding* he2)
    {
        return (he1->val < he2->val);
    }
};

// this struct will build the tree
struct node* makeTreeDia() {
    struct node *left, *right, *top = nullptr;
    // create a minHeap
    std::priority_queue<node*, std::vector<node*>, eval> minHeap;
    // push the values onto minHeap
    for (int i = 0; i < freq.size(); i++) {
        minHeap.push(new node(elements[i], freq[i]));
    }
    //Continue until only one node is left on minHeap
    while (minHeap.size() != 1) {
        left = minHeap.top();
        minHeap.pop();
        right = minHeap.top();
        minHeap.pop();
        // a new node is created to sum up the frequencies
        top = new node('*', left->f + right->f);
        top->left = left;
        top->right = right;
        // that is pushed on to minHeap
        minHeap.push(top);
    }
    return top;
}

void getCode(struct node* root, std::string binary) {
// base case
    if (!root) return;
    // store non-internal nodes only
    if (root->val != '*') {
        code.push_back(new encoding(root->val, binary));
    }
    // now add a zero if you go left on the tree
    getCode(root->left, binary + "0");
    // add a one if you go right on the tree
    getCode(root->right, binary + "1");
}


int main() 
{

    std::cout << '\n' << "Huffman Encoding! Let's Get Greedy!" << '\n';
	std::ifstream myfile;
    std::string line;

	// Open the file
	myfile.open("freq.txt");

	//execute statement if file is open else print error message
	if(myfile.is_open())
	{	
		//if open use the file
		while (!myfile.eof()) 
        {
            getline(myfile, line);
            std::string elem = line.substr(0, line.find(' '));
            std::string num = line.substr(line.find(' ') + 1);
            if (elem != "LF") {
                elements.push_back(elem[0]);
            }
            else {
                elements.push_back('\n');
            }
            freq.push_back(stoi(num));
	    }
	}
    //warning message if file is not open
	else std::cout << "unable to open file" << '\n';
	myfile.close();

    // do encoding
    getCode(makeTreeDia(), "");
    

    // ASCII sort
    sort(code.begin(), code.end(), sortASCII());
    // put values into the new file being created
    std::ofstream newFile;
    newFile.open("codetable.txt");
    for (int i = 0; i < code.size(); i++) 
    {
        if (code[i]->val != '\n') 
        {
            newFile << code[i]->val << "     " << code[i]->encode<< std::endl;
        }
        else 
        {
            newFile << "LF    " << code[i]->encode << std::endl;
        }
    }
    newFile.close();
    std::cout << "\ncodetable.txt created " << '\n';
    std::cin.get();
    return 0;
}