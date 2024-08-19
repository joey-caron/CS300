//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Joseph Caron
// Version     : 1.0
// Description : ABCU course program
//============================================================================

#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>


using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================


struct Course {
	string courseId;
	string courseName;
	vector<string> coursePreReq;
};

struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() {
		left = nullptr;
		right = nullptr;
	}

	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

class BinarySearchTree {

private:
	Node* root;

	void addNode(Node* node, Course course);
	void inOrder(Node* node);
	void postOrder(Node* node);
	void preOrder(Node* node);
	Node* removeNode(Node* node, string courseId);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void InOrder();
	void PostOrder();
	void PreOrder();
	void Insert(Course course);
	void Remove(string courseId);
	Course Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
	//root is equal to nullptr
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
	// recurse from root deleting every node
	delete root;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
	// call inOrder fuction and pass root 
	inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {

	if (root == nullptr) {
		root = new Node(course);
		root->left = nullptr;
		root->right = nullptr;
	}
	else {
		addNode(root, course);
	}
}


/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {
	Node* currentNode = root;
	// set current node equal to root
	while (currentNode != nullptr) {
		if (currentNode->course.courseId == courseId) {
			return currentNode->course; // found
		}
		else if (currentNode->course.courseId < courseId) {
			currentNode = currentNode->right;
		}
		else {
			currentNode = currentNode->left;
		}

		// keep looping downwards until bottom reached or matching courseId found
			// if match found, return current course

			// if course is smaller than current node then traverse left
			// else larger so traverse right
	}

	Course course;
	return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
	Node* currentNode = root;

	while (currentNode != nullptr) {
		if (course.courseId < currentNode->course.courseId) {
			if (currentNode->left == nullptr) {
				currentNode->left = new Node(course);
				currentNode = nullptr;
			}
			else {
				currentNode = currentNode->left;
			}
		}
		else {
			if (currentNode->right == nullptr) {
				currentNode->right = new Node(course);
				currentNode = nullptr;
			}
			else {
				currentNode = currentNode->right;
			}
		}
	}
	// if node is larger then add to left
		// if no left node
			// this node becomes left
		// else recurse down the left node
	// else
		// if no right node
			// this node becomes right
		//else
			// recurse down the left node
}

void BinarySearchTree::inOrder(Node* node) {
	Node* currentNode = node;
	if (currentNode == nullptr) { 
		return;
	}
	inOrder(currentNode->left); // cycles through Search Tree to print all courses in alphanumeric order
	cout << currentNode->course.courseId << ": " << currentNode->course.courseName << " | Pre-Requisites: ";

	for (int i = 0; i < currentNode->course.coursePreReq.size(); i++) {
		if (i != 0) {
			cout << ", ";
		}
		cout << currentNode->course.coursePreReq[i];
	}
	cout << endl;
	
	inOrder(currentNode->right);

}


void displayCourse(Course course) {
	cout << course.courseId << ": " << course.courseName << " | Pre-Requisites: "; // Displays course and pre requisites
	
	
	for (int i = 0; i < course.coursePreReq.size(); i++) {
		if (i != 0) {
			cout << ", ";
		}
		cout << course.coursePreReq[i];
	}
	cout << endl;
	
}

void loadCourses(string csvPath, BinarySearchTree* bst) {
	cout << "Enter file name: " << endl; 
	cin.ignore();
	getline(cin, csvPath);
	cout << "Loading CSV file " << csvPath << endl;

	// Loads file and reads each line, creating course objects for each

	ifstream inFS;
	string currentLine;


	inFS.open(csvPath); //opens the file

	if (!inFS.is_open()) { //Error in case the file can't be opened
		cout << "Could not open file. Try again." << endl;
		return;
	}

	while (getline(inFS, currentLine)) { // Parses each line
		Course aCourse;

		string tempID = "";
		string tempName = "";
		string tempPreReq = "";

		stringstream inputString(currentLine);
		getline(inputString, tempID, ',');
		aCourse.courseId = tempID;
		getline(inputString, tempName, ',');
		aCourse.courseName = tempName;

		if (tempID.compare("") == 0 || tempName.compare("") == 0) { // This will catch errors and skip any lines without at least a course ID and name
			cout << "Error: invalid course format" << endl;
			continue;
		}
		
		

		while (getline(inputString, tempPreReq, '\n')) { //Looks at prerequisites
			stringstream ss(tempPreReq);
			// Seperate tempPreReq by the ',' delimiter
			while (getline(ss, tempPreReq, ',')) {
				// add it to coursePreReq vector
				aCourse.coursePreReq.push_back(tempPreReq);
			}

		}

		bst->Insert(aCourse);

		currentLine = ""; // resets temp variables
		aCourse.coursePreReq.clear();
	}
	inFS.close(); // closes file
}

	

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

	// process command line arguments
	string csvPath, courseKey;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		courseKey = "0";
		break;
	case 3:
		csvPath = argv[1];
		courseKey = argv[2];
		break;
	default:
		csvPath = "0";
	}


	// Define a binary search tree to hold all bids
	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	Course course;

	int choice = 0;
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "  1. Load Courses" << endl;
		cout << "  2. Display All Courses" << endl;
		cout << "  3. Find Course" << endl;
		cout << "  9. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;

		switch (choice) {

		case 1:

			// Complete the method call to load the courses
			loadCourses(csvPath, bst);


			break;

		case 2:
			bst->InOrder();
			break;

		case 3:

			cout << "Enter Id: " << endl;
			cin.ignore();
			getline(cin, courseKey);
			course = bst->Search(courseKey);


			if (!course.courseId.empty()) {
				displayCourse(course);
			}
			else {
				cout << "Course Id " << courseKey << " not found." << endl;
			}

			break;

		}
	}

	cout << "Good bye." << endl;

	return 0;
}