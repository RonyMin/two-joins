/*
 * Leapfrog.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: parallels
 */

#include "../include/Leapfrog.h"

bool iterSorter(TrieIterator* iter1, TrieIterator* iter2) {
	return iter1->key() < iter2->key();
}

// Constructor of Leapfrog, contains a vector of pointers to TrieIterator
// Inplemented based on the code in the paper
Leapfrog::Leapfrog(const vector<TrieIterator*> &iterArrayIn,
		const string& attrNameIn) {
	_iterArray = iterArrayIn;
	_attrName = attrNameIn;
	_atEnd = false;
	_size = _iterArray.size();

	_key = -1;
	_pIdx = -1;
}

// Inplemented based on the code in the paper
void Leapfrog::init() {
	_atEnd = false;

	for (size_t i = 0; i != _size; i++) {
		if (_iterArray[i]->at_end()) {
			_atEnd = true;
			break;
		}
	}
	if (!_atEnd) {
		// sort the array iterArray by keys at which the iterators are positioned
		sort(_iterArray.begin(), _iterArray.end(), iterSorter);
		_pIdx = 0;
		search();
	}
}

// Inplemented based on the code in the paper
// This is the workhorse of leapfrog, with the ability to find the 
// next common value in all TrieIterator
void Leapfrog::search() {
	int maxKey = _iterArray[(_pIdx + _size - 1) % _size]->key();
	while (true) {
		int leastKey = _iterArray[_pIdx]->key();
		if (leastKey == maxKey) {
			_key = leastKey;
			return;
		} else {
			_iterArray[_pIdx]->seek(maxKey);
			if (_iterArray[_pIdx]->at_end()) {
				_atEnd = true;
				return;
			} else {
				maxKey = _iterArray[_pIdx]->key();
				_pIdx = (_pIdx + 1) % _size;
			}
		}
	}
}

// Inplemented based on the code in the paper
void Leapfrog::next() {
	_iterArray[_pIdx]->next();
	if (_iterArray[_pIdx]->at_end()) {
		_atEnd = true;
	} else {
		_pIdx = (_pIdx + 1) % _size;
		search();
	}
}

// Inplemented based on the code in the paper
void Leapfrog::seek(const int seekKey) {
	_iterArray[_pIdx]->seek(seekKey);
	if (_iterArray[_pIdx]->at_end()) {
		_atEnd = true;
	} else {
		_pIdx = (_pIdx + 1) % _size;
		search();
	}
}

bool Leapfrog::at_end() {
	return _atEnd;
}

int Leapfrog::key() {
	assert(!_atEnd);
	return _key;
}

// use for triejoin, to go to deeper layer of trie structure
void Leapfrog::open_all() {
	for (size_t i = 0; i != _size; i++) {
		_iterArray[i]->open();
	}
}

// use for triejoin, to go to upper layer of trie structure
void Leapfrog::up_all() {
	for (size_t i = 0; i != _size; i++) {
		_iterArray[i]->up();
	}
}

void Leapfrog::show() {
	cerr << "pIdx " << _pIdx << "    ";
	for (size_t i = 0; i != _size; i++) {
		cerr << _iterArray[i]->spec->relName << " " << _iterArray[i]->key()
				<< " ";
	}
	cerr << endl;
}

void Leapfrog::show_depth() {
	for (size_t i = 0; i != _size; i++) {
		cerr << _iterArray[i]->spec->relName << ": " << _iterArray[i]->get_depth()
				<< " ";
	}
	cerr << endl;
}
