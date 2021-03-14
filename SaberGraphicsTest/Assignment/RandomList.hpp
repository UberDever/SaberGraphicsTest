#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <map>

struct ListNode
{
	std::string data;
	ListNode* prev, * next, * rand;

	ListNode() : prev(nullptr), next(nullptr), rand(nullptr), data("") {}
	ListNode(std::string data) : prev(nullptr), next(nullptr), rand(nullptr), data(data) {}
};

/*
	Class named RandomList for obvious reasons, but I am not sure, if it is a list at all
	Some functions are added for testing purposes and they does not affect other functionality

	Important notice - assignment states, that .rand link points to random link of THIS list -
	implying, that random node at least has next/prev link, which can lead to head/tail nodes
	(which means, that node is not part of sublist or separate branch, that is inaccessible via head/tail traversal).
	In other case, algorithm for this task might be overly complicated for test assignment (with indicated constraints).
*/
class RandomList
{
public:
	RandomList() : head(nullptr), tail(nullptr), count(0) {}

	~RandomList()
	{
		ListNode* ptr = head;
		while (ptr != nullptr)
		{
			ListNode* tmp = ptr;
			ptr = ptr->next;
			delete tmp;
		}
	}

	void Serialize(FILE* file); // сохранение в файл (файл открыт с помощью fopen(path, "wb"))
	void Deserialize(FILE* file); // загрузка из файла (файл открыт с помощью fopen(path, "rb"))

	void addNode(ListNode* elem)
	{
		if (head == nullptr)
		{
			head = elem;
			tail = elem;
		}
		else
		{
			tail->next = elem;
			tail->next->prev = tail;
			tail = elem;
		}
		count++;
	}

	void addNode(std::string data)
	{
		ListNode* elem = new ListNode(data);
		addNode(elem);
	}

	int getCount()
	{
		return count;
	}

	void printNodes()
	{
		ListNode* ptr = head;
		while (ptr)
		{
			std::cout << ptr->data << std::endl;
			ptr = ptr->next;
		}
	}

	void printNodesWithRand()
	{
		ListNode* ptr = head;
		while (ptr)
		{
			std::cout << ptr->data << ".r = " << (ptr->rand ? ptr->rand->data : "NULL") << std::endl;
			ptr = ptr->next;
		}
	}

private:
	ListNode* head;
	ListNode* tail;
	int count;
};

/// <summary>
/// Serialize(FILE* file) - serializes RandomList to file in binary format
/// <para>
/// Function expects opened file in wb or w+b mode, closing the file is not performed. 
/// File position pointer is managed by the user.
/// </para>
/// </summary>
/// <param name="file">File, which will be written</param>
void RandomList::Serialize(FILE* file)
{
	/*
		RandomList is traversed ONCE, meaning there O(n) time complexity for this operation.
		However, some metadata must be constructed in order to keep random links for deserialization.
		Therefore, there are (count * sizeof(int)) additional memory allocated and written to file,
		which is bigger than data, only when sizeof(data) < 4 (or 8) bytes.
		This is, along with allocation / deallocation of additional memory, is the main drawback of this approach.

		Also, I used some STL containers that can also invoke some allocations, but time complexity
		of this containers is worth it.
		std::unordered_map insertion and lookup is O(1) (however, latter is average for low loadfactor)
		std::multimap insertion and lookup is O(log n) (as far, as I can tell, because it is based on rb tree)

		Algorithm consists of following directions:
		Firstly, write count of nodes and EMPTY metadata array to file
		Then, do for all nodes:
			write current node to visited;
		line1 :
			if there are any nodes, that have random link to this node:
				go to them and assign current node index
			if we have visited current node random:
				assign current node random index
			else:
				push request for this random node with unknown index (when we find him, we will go to line 1)
			write current node data to file
		Return to place in file, where metadata array is located and write ACTUAL metadata.
		Therefore, file structure is as follows
			{ sizeof(count) [ count * sizeof(int) ] string\0string\0...string\0 }
	*/
	int* nodesRandomLinks = new int[count]();
	std::unordered_map<ListNode*, int> visitedNodes;
	std::multimap<ListNode*, ListNode*> requestedNodes;
	int curIndex = 0;

	fwrite(&count, sizeof(count), 1, file);
	fwrite(nodesRandomLinks, count * sizeof(*nodesRandomLinks), 1, file);

	ListNode* curPtr = head;
	while (curPtr && (curIndex < count))
	{
		visitedNodes[curPtr] = curIndex;

		auto itr1 = requestedNodes.lower_bound(curPtr);
		auto itr2 = requestedNodes.upper_bound(curPtr);
		while (itr1 != itr2)
		{
			nodesRandomLinks[visitedNodes.at(itr1->second)] = curIndex;
			itr1++;
		}

		if (curPtr->rand != nullptr)
		{
			if (visitedNodes.find(curPtr->rand) != visitedNodes.end())
			{
				nodesRandomLinks[curIndex] = visitedNodes[curPtr->rand];
			}
			else
			{
				requestedNodes.insert(std::pair<ListNode*, ListNode*>(curPtr->rand, curPtr));
			}
		}
		else
		{
			nodesRandomLinks[curIndex] = -1;
		}
		fwrite(curPtr->data.data(), sizeof(char) * (curPtr->data.size() + 1), 1, file);

		curPtr = curPtr->next;
		curIndex++;
	}

	fseek(file, sizeof(int), SEEK_SET);
	fwrite(nodesRandomLinks, count * sizeof(*nodesRandomLinks), 1, file);

	delete[] nodesRandomLinks;
}

/// <summary>
/// Deserialize(FILE* file) - deserializes RandomList from binary file
/// <para>
/// Function expects opened file in rb or r+b mode, closing the file is not performed. 
/// File position pointer is managed by the user.
/// </para>
/// </summary>
/// <param name="file">File, which will be read</param>
void RandomList::Deserialize(FILE* file)
{
	/*
		Deserialization if performed on the fly and consists of two stages:
		Stage 1:
			Read {$count} and metadata array from file
			Create {$count} nodes and allocate them instantly
		Stage 2:
			Read strings byte by byte until \0, then assign that string to corresponding node
			prev and next nodes are easy - they are just curIndex +- 1, but random node is different.
			Then metadata array is used like array indicies for assigning nodes random links
		After all this, metadata array and pointer to nodes array are deallocated.
		List if fully restored back to its previous state
	*/
	fread(&count, sizeof(int), 1, file);
	int* nodesRandomLinks = new int[count]();
	fread(nodesRandomLinks, count * sizeof(*nodesRandomLinks), 1, file);

	ListNode** nodesPtrArray = new ListNode * [count];
	for (int i = 0; i < count; i++)
	{
		nodesPtrArray[i] = new ListNode();
	}

	char c = 0;
	int curIndex = 0;
	std::string data;
	data.reserve(64);
	while (1 == fread(&c, sizeof(char), 1, file))
	{
		if (c == '\0')
		{
			nodesPtrArray[curIndex]->data = data;
			data.clear();

			if (curIndex > 0)
				nodesPtrArray[curIndex]->prev = nodesPtrArray[curIndex - 1];
			else
				head = nodesPtrArray[curIndex];

			if (curIndex < count - 1)
				nodesPtrArray[curIndex]->next = nodesPtrArray[curIndex + 1];
			else
				tail = nodesPtrArray[curIndex];

			if (nodesRandomLinks[curIndex] > -1)
				nodesPtrArray[curIndex]->rand = nodesPtrArray[nodesRandomLinks[curIndex]];

			curIndex++;
			if (curIndex == count)
				break;
		}
		else
			data += c;
	}
	delete[] nodesRandomLinks;
	delete[] nodesPtrArray;
}