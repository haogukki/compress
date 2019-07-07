/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-07 11:51
 *   Filename        : HuffmanTree.h
 *   Description     : 
 * *******************************************************/

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "Common.h"

template<typename T>
struct HuffmanNode {
	T _data;
	HuffmanNode<T>* left_;
	HuffmanNode<T>* right_;
	HuffmanNode<T>* parent_;
	HuffmanNode(const T& data = T())
		:_data(data),
        left_(nullptr),
        right_(nullptr),
        parent_(nullptr)
	{}
};


template<typename T>
class HuffmanTree {
	typedef HuffmanNode<T> Node;
public:
	HuffmanTree()
		:root_(nullptr)
	{}

	HuffmanTree(T  *a, size_t size, const T& invalid = T()) {
		struct NodeLess {
			bool operator()(Node* l, Node* r) {
				assert(l);
				assert(r);
				return l->_data < r->_data;
			}
		};

		Heap<Node*, NodeLess>  minHeap;
		for (size_t i = 0; i < size; i++) {
			if (a[i]._count != invalid._count) {
				Node* _node = new Node(a[i]);
				minHeap.Push(_node);
			}
		}

		Node* frist = nullptr;
		Node* second = nullptr;
		Node* parent = nullptr;
		while (minHeap.Size() > 1) {
			frist = minHeap.Top();
			minHeap.Pop();
			second = minHeap.Top();
			minHeap.Pop();

			parent = new Node(frist->_data + second->_data);
			parent->left_ = frist;
			parent->right_ = second;

			frist->parent_ = parent;
			second->parent_ = parent;

			minHeap.Push(parent);
		}

		root_ = minHeap.Top();
	}

	Node* GetRoot() {
		return root_;
	}


	~HuffmanTree() {
		if (root_ != nullptr) {
			Destory(root_);
		}
	}
protected:
	void Destory(Node * root) {
		if (root == nullptr)
			return;
		Node* cur = root;
		Destory(cur->left_);
		Destory(cur->right_);
		delete cur;
		cur = nullptr;
	}

private:
	Node* root_;
};

#endif
