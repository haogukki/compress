/**********************************************************
 *   Author          : Apriluestc
 *   Email           : 13669186256@163.com
 *   Last modified   : 2019-07-07 11:24
 *   Filename        : Common.h
 *   Description     : 
 * *******************************************************/

#ifndef HEAP_H
#define HEAP_H

#include <cassert>
#include <vector>
#include <cstddef>

template<typename T>
struct Less {
	bool operator()(const T& left, const T& right) {
		return left < right;
	}
};

template<typename T>
struct Great {
	bool operator()(const T& left, const T& right) {
		return left > right;
	}
};

template<typename T, class Compare = Less<T>>
class Heap {
public:
	Heap() {}
	Heap(T *a, int size) {
		num_.reserve(size);
		for (int i = 0; i < size; i++) {
			num_.push_back(a[i]);
		}

		// 建堆
		for (int i = (size - 2) / 2; i >= 0; --i) {
			AdjustDown(i, size);
		}
	}

	void Push(const T& x) {
		//插入到尾部，再从最后一个元素开始向上调整
		num_.push_back(x);
		AdjustUp(num_.size() - 1);
	}

	void Pop() {
		// 将堆顶与最后一个元素交换，再从堆顶下滑调整
		assert(!num_.empty());
		swap(num_[0], num_[num_.size() - 1]);
		num_.pop_back();
		if (num_.size() > 1) {
			AdjustDown(0, num_.size());
		}
	}

	size_t Size() {
		return num_.size();
	}

	bool Empty() {
		return num_.empty();
	}

	const T& Top() {
		assert(!num_.empty());
		return num_[0];
	}
protected:
	void AdjustDown(int root, int size) {
		assert(!num_.empty());
		int parent = root;
		int child = parent * 2 + 1;
		while (child < size) {

			if ((child + 1) < size
				&&Compare()(num_[child + 1], num_[child]))
                // 找左右孩子中最大的下标
				child++;
			if (Compare()(num_[child], num_[parent])) {
				swap(num_[parent], num_[child]);
				parent = child;
				child = parent * 2 + 1;
			} else {
				break;
			}
		}
	}

	void AdjustUp(int child) {
		assert(!num_.empty());
		while (child > 0) {
			int parent = (child - 1) / 2;
			if (Compare()(num_[child], num_[parent])) {
				swap(num_[parent], num_[child]);
				child = parent;
			} else {
				break;
			}
		}
	}
private:
    std::vector<T> num_;
};

#endif
