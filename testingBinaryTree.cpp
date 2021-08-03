#include <iostream>

#include "binaryTree.h"


using namespace VGP244;
using namespace std;

int main()
{
	BinTree<int, std::less<int>> intBTree;

	intBTree.insert_it(24);
	intBTree.insert_it(15);
	intBTree.insert_it(33);
	intBTree.insert_it(14);
	intBTree.insert_it(20);
	intBTree.insert_it(82);
	intBTree.insert_it(21);

	cout << "Height of the tree: " << intBTree.Height() << endl;
	intBTree.insert_it(25);
	intBTree.insert_it(31);
	intBTree.insert_it(18);
	intBTree.insert_it(5);
	intBTree.insert_it(15);

	cout << "Height of the tree: " << intBTree.Height() << endl;
	intBTree.insert_recursive(16);
	intBTree.insert_recursive(10);

	// apply accumulator operation to the tree: means add all the data  together and return it
	int total{ 0 };
	auto Accumulator{ [&total](int val) { total += val; } };
	intBTree.apply(Accumulator, intBTree.PreOrder);

	cout << "ApplyAccumulator PreOrder: total=" << total << endl;

	total = 0;
	intBTree.apply(Accumulator, intBTree.InOrder);

	cout << "ApplyAccumulator InOrder: total=" << total << endl;

	total = 0;
	intBTree.apply(Accumulator, intBTree.PostOrder);

	cout << "ApplyAccumulator PostOrder: total=" << total << endl;

	cout << "Simpling tree printing:\n";
	intBTree.print(intBTree.InOrder);


	cout << "\nsearch for 19: " << intBTree.search_re(19) << endl;
	cout << "search for 18: " << intBTree.search_re(18) << endl;
	cout << "search for 17: " << intBTree.search_re(17) << endl;

	cout << "Height of the tree: " << intBTree.Height() << endl;

	cout << "Testing removing 17 (does not exist): " << intBTree.remove(17);

	cout << "\nTesting removing 18: " << intBTree.remove(18);
	cout << "\nPrintHierachy: \n"; intBTree.printHierarchy();

	intBTree.insert_it(45);
	intBTree.insert_it(41);
	intBTree.insert_it(48);
	intBTree.insert_it(35);
	intBTree.insert_it(55);
	cout << "\nPrintHierachy after adding 45, 41, 48, 35, 55: \n"; intBTree.printHierarchy();
	cout << "\nHeight of the tree: " << intBTree.Height() << endl;

	cout << "\nRangePrint(25, 45):\n"; intBTree.rangePrint(25, 45);

	cout << "\nBreadthFirstTraveral test: print: "; intBTree.print(intBTree.BreadthFirst);
	cout << "\nDepthFirstTraveral test: print: "; intBTree.print(intBTree.DepthFirst);

	cout << "\nType any char to finish: ";
	char c{};
	cin >> c;

	return 0;
}