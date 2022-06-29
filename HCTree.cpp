/*
 * Name: Paul Nguyen
 *
 *
 * This file implement the function definitions for HCTree.hpp
 */
#include "HCTree.hpp"

HCTree::~HCTree() {
	if (root == nullptr) return;
	destructorHelper(root);
}

void HCTree::destructorHelper(HCNode *Node){
	// if 0 child exist then delete it
	if (Node->c0 != nullptr){
		destructorHelper(Node->c0);
	}
	// if 1 child exist then delete it
	if (Node->c1 != nullptr){
		destructorHelper(Node->c1);
	}
	// delete input node
	delete Node;
}

void HCTree::build(const vector<unsigned int>& freqs) {

	// creates leafs nodes for all bytes
	for(unsigned int i = 0; i < freqs.size(); i++){
		if (freqs[i] > 0){
			HCNode *newLeaf = new HCNode(freqs[i], i);
			leaves[i] = newLeaf;
		}
	}
	
	// create priority queue to function as min-heap
	priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

	// adds all leaves to priority queue
	for(unsigned int i = 0; i < leaves.size(); i++){
		if (leaves[i] == nullptr) continue;
		pq.push(leaves[i]);
	}
	
	// combines all nodes into tree
	while(pq.size() > 1){
		
		// pops the next two nodes in queue
		HCNode *pqFirst = pq.top(); // first node in queue
		pq.pop();
		HCNode *pqSecond = pq.top(); // second node in queue
		pq.pop();

		int count = pqFirst->count + pqSecond->count; // count of nodes
		// creates internal node connecting next nodes in queue
		HCNode *internalNode = new HCNode(count, pqFirst->symbol);
		internalNode->c0 = pqFirst;
		internalNode->c1 = pqSecond;

		// assign parent for next nodes in queue
		pqFirst->p = internalNode;
		pqSecond->p = internalNode;

		// adds internal node
		pq.push(internalNode);
	}

	// tree is fully built so asign root
	root = pq.top();
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const {
	HCNode *currNode = leaves[symbol]; // keep track of current node

	// recursively go up tree to get bit encoding
	encodeHelper(currNode, out);
}

void HCTree::encodeHelper(HCNode *currNode, FancyOutputStream& out)const{
	// do nothing since at root node
	if (currNode == root){
		return;
	}
	// node is left child so write a 0 bit
	else if(currNode == currNode->p->c0){
		encodeHelper(currNode->p, out);
		out.write_bit(0);
	}
	// node is right child so write a 1 bit
	else{
		encodeHelper(currNode->p, out);
		out.write_bit(1);
	}
}

void HCTree::encodeOne(FancyOutputStream & out) const{
	// write a bit to output file
	out.write_bit(1);
}

unsigned char HCTree::decode(FancyInputStream & in) const {
	HCNode *currNode = root; // keep trackof current node

	// iterate down tree to leaf node
	while(currNode->c0 != nullptr || currNode->c1 != nullptr){
		char bitRead = in.read_bit(); // gets a bit
		if (!in.good()) return 0; // check EOF
		// go to 0 node
		if (bitRead == 0){
			// check no problem
			if (currNode->c0 == nullptr){ 
				fprintf(stderr, "error finding symbol");
				return 0;
			}
			currNode = currNode->c0;
		}
		// go to 1 node
		else {
			// check no problem
			if (currNode->c1 == nullptr){ 
				fprintf(stderr, "error finding symbol");
				return 0;
			}
			currNode = currNode->c1;
		}
	}

	// at leaf node so get symbol
	return currNode->symbol;
}
