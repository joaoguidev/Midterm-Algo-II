#pragma once
#include <memory>
#include <iostream>

namespace VGP244{

	template<typename T, typename CMP>
	class BinTree
	{
	private:
		struct Node;
		typedef std::shared_ptr<Node> spNode;

		spNode root;
		CMP cmp;  // used to compare datas of type T: cmp(t1, t2 ) --> true or false
		size_t count; // keeps track of number of nodes in the tree

		bool search_recursive(T val, spNode node)
		{
			if (node == nullptr) // checking if we have reached leaf nodes
				return false;
			if (node->data == val)  // checking if this node is "the" node containing val
				return true;
			if (cmp(val, node->data))
				return search_recursive(val, node->left);
			return search_recursive(val, node->right);
			
		}
	public:

		struct Node
		{
			T data;
			spNode left;
			spNode right;
			Node(T d = {}) : data(d), left(nullptr), right(nullptr)
			{}
			~Node()
			{
				left = nullptr;
				right = nullptr;
			}

			// recursive insertion of val in the substree rooted at this node.
			void insert(T val, spNode& current)
			{
				// 1- stop case for recursion. Here you do check if this node is "the" node2Insert. If it is do the insertion
				if (current == nullptr) //Tree is empty;
				{
					current = std::make_shared<Node>(val);
					return;
				}
				// 2- else, call the proper subtree with the insert call.
				if (val <= current->data)
					insert(val, current->left);
				else
					insert(val, current->right);
			}
		};

		BinTree() : root(nullptr), count(0) {}
		size_t getCount() const { return count; }


		// insert data in the tree recursively:
		void insert_it(T val)
		{
			++count;
			if (root == nullptr)  // means tree is empty, so create the root and return
			{
				root = std::make_shared<Node>(val);
				return;
			}

			// first find where to insert the new node for val.
			spNode temp{ root };
			spNode node2Insert{};
			while (temp != nullptr)
			{
				node2Insert = temp;
				if (cmp(val, temp->data) == true)
					temp = temp->left;
				else
					temp = temp->right;
			}

			// now create node and attach it to node2Insert at the correct side
			if (cmp(val, node2Insert->data))
				node2Insert->left = std::make_shared<Node>(val);
			else
				node2Insert->right = std::make_shared<Node>(val);
		}

		// recursive version of insertion. 
		void insert_recursive(T val)
		{
			++count;
			root->insert(val, root);
		}

		/// <summary>
		/// Removes first instance of node containing val from the tree.
		/// </summary>
		/// <param name="val"></param>
		/// <returns>true if successfully removed a node. returns false if val was not found.</returns>
		bool remove(T val)
		{
			spNode& curr{ root };
			spNode currB{ root };
			// first find the node containing the data val. We also keep track of the parent node
			spNode par = nullptr;
			bool found{ false };
			while (curr != nullptr && found == false)
			{
				if (curr->data == val)
					found = true;
				else
				{
					par = curr;
					if (cmp(val, curr->data))
						curr = curr->left;
					else
						curr = curr->right;
				}
			}
			
			if (curr == nullptr || curr->data != val)
			{
				curr = currB;
				currB.reset();
				return false;  // val is not found in the tree
			}

			// node containing val is found (curr). Now take care of 3 cases:
			// 1) if curr is a leaf node:
			if (curr->left == nullptr && curr->right == nullptr)  // checking if curr is leaf node
			{
				if (par->left == curr)
					par->left = nullptr;
				else
					par->right = nullptr;

				curr.reset();
				curr = currB;
				--count;
				return true;
			}
			// 2) if curr is half leaf node. The relink its only child to curr's par. Then del curr
			if (curr->left == nullptr && curr->right != nullptr)  // checking if curr is leaf node
			{
				par->right = curr->right;
				curr.reset();
				curr = currB;
				--count;
				return true;
			}
			if (curr->left != nullptr && curr->right == nullptr)  // checking if curr is leaf node
			{
				par->left = curr->left;
				curr.reset();
				curr = currB;
				--count;
				return true;
			}

			//3) if curr is internal node. Then find the "smallest" node, rep, in the right subtree and replace curr->data with
			// rep->data and then delete rep.
			if (curr->left != nullptr && curr->right != nullptr)
			{
				spNode& rep{ curr };
				spNode parRep = par;
				while (rep->right != nullptr)
				{
					parRep = rep;
					rep = rep->right;
				}
				curr->data = rep->data;

				if (rep->left != nullptr)
				{
					parRep->data = rep->left->data;
				}
				rep.reset();
				curr = currB;
				--count;
				return true;
			}

			return false;  // update this return once you have done cases 2 and 3.

		}
		// traversal support to the tree
		enum eOrder { PreOrder, InOrder, PostOrder, BreadthFirst, DepthFirst };

		template<typename Visitor>
		void applyPreOrderVisit(Visitor ob, spNode node)
		{
			if (node == nullptr)
				return;
			ob(node->data);
			applyPreOrderVisit(ob, node->left);
			applyPreOrderVisit(ob, node->right);
		}
		template<typename Visitor>
		void applyInOrderVisit(Visitor ob, spNode node)
		{
			if (node == nullptr)
				return;
			applyInOrderVisit(ob, node->left);
			ob(node->data);
			applyInOrderVisit(ob, node->right);
		}
		template<typename Visitor>
		void applyPostOrderVisit(Visitor ob, spNode node)
		{
			if (node == nullptr)
				return;
			applyPostOrderVisit(ob, node->left);
			applyPostOrderVisit(ob, node->right);
			ob(node->data);
		}

		/// <summary>
		/// Use std::queue<spNode>, as explained in ppt Tree_deletion... in class, to implement breadthFirst Traveral visit 
		/// </summary>
		/// <typeparam name="Visitor"></typeparam>
		/// <param name="ob">observer operator to apply to each node</param>
		/// <param name="node">starting node</param>
		template<typename Visitor>
		void applyBreadthFirstVisit(Visitor ob, spNode node)
		{
			std::cout << "applyBreadthFirstVisit(): to be implemented by students\n";
		}

		/// <summary>
		/// Use recursion to implement depth first traversal and apply the visitor ob to each node twice. As you remember from
		/// the ppt Tree_delet... in depth first, we visit a node twice: once on the way down, and second on the backtracking way out.
		/// </summary>
		/// <typeparam name="Visitor"></typeparam>
		/// <param name="ob">observer operator to apply to each node</param>
		/// <param name="node">starting node</param>
		template<typename Visitor>
		void applyDepthFirstVisit(Visitor ob, spNode node)
		{
			std::cout << "applyDepthFirstVisit(): to be implemented by students\n";
		}

		template<typename Visitor>
		void apply(Visitor ob, eOrder order)
		{
			switch (order)
			{
			case PreOrder:
				applyPreOrderVisit(ob, root);
				break;
			case InOrder:
				applyInOrderVisit(ob, root);
				break;
			case PostOrder:
				applyPostOrderVisit(ob, root);
				break;
			case BreadthFirst:
				applyBreadthFirstVisit(ob, root);
				break;
			case DepthFirst:
				applyDepthFirstVisit(ob, root);
				break;

			default:
				std::cout << "ERROR: apply:: invalid order was passed in\n";
				exit(1);
			}
		}

		// entry point for recursive search 
		bool search_re(T val)
		{
			if (root == nullptr)
				return false;
			return search_recursive(val, root);
		}

		void rangePrintRecursive(T start, T end, spNode& current)
		{
			// algorithm:
			/*
			* start from the root. At every node:
			1) check if this node is in the range. If so print it and call recursively left and right child
			* 2) if node.data is less than start then call rangePrint on the right child
			* 3) else call rangePrint on the left child.
			*/
			if (current == nullptr) // checking if we have reached leaf nodes
				return;

			if (current->data > start && current->data < end)
			{
				std::cout << "Range Print (" << current->data << ")\n";
			}

			if (current->left != nullptr)
				rangePrintRecursive(start, end, current->left);
			if (current->right != nullptr)
				rangePrintRecursive(start, end, current->right);
		}

		// print all the data items in the tree with values in the range [start, end]
		void rangePrint(T start, T end)
		{
			rangePrintRecursive(start, end, root);
		}

		size_t HeightRecursive(spNode& current)
		{
			if (current == nullptr)
				return 0;
			else
			{
				size_t heightL = HeightRecursive(current->left);
				size_t heightR = HeightRecursive(current->right);
				if (heightL > heightR)
				{
					return(heightL + 1);
				}
				else {
					return(heightR + 1);
				}
			}
		}

		size_t Height()
		{
			//size_t height = 0;
			return HeightRecursive(root);

		}

		void print(eOrder order)
		{
			auto printer{ [](T val) { std::cout << "Node(" << val << ")\n"; } };
			apply(printer, order);
		}

		/// <summary>
		/// Use the algorithm explained on page 17 of ppt Tree_deletion... to print
		/// the tree in hierachical style as presented there.
		/// </summary>
		void printHierarchy()
		{
			std::cout << "printHierarchy(): to be done by students.\n";
		}
	};

}
