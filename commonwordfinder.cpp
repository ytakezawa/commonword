/*
 * commonwordfinder.cpp
 * 	I pledge my honor that I have abided by the Stevens Honor System.
 *	ytakezaw
 *  Created on: Nov 29, 2018
 *      Author: Yoshika Takezawa
 */

#include "rbtree.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>

#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>


//using namespace std;

/*
 * displays keys and values in an ordered way. very nice
 */
void display_top (const std::vector<std::pair<std::string, int>> &nodes, int limit){
	// if the number of words is less than given limit
	if (nodes.size() < (unsigned int)limit) {
		limit = nodes.size();
	}

	std::cout << "Total unique words: " << nodes.size() << "\n";
	int dig_spaces = 1;
	int dig_lim = limit;
	// snippet from stairclimber problem
	//for numbering
	while (dig_lim > 9) {
		dig_lim = dig_lim / 10;
		++dig_spaces;
	}

	//for space between string and value
	int max_str_len= 1;
	int current;
	for (int i =0; i < limit; ++i) {
		current = nodes[i].first.length();
		if (current > max_str_len){
			max_str_len = current;
		}
	}

	for (int i = 0; i < limit; ++i) { // printing rows
		std::cout << std::setw(dig_spaces) << i + 1 << ". "<<
				std::left << std::setw(max_str_len + 1) << nodes[i].first << std::right <<
				nodes[i].second << "\n";
	}
}

// Driver function to sort the vector elements
// by second element of pairs
// see https://www.geeksforgeeks.org/sorting-vector-of-pairs-in-c-set-1-sort-by-first-and-second/
bool sortbysec(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
	return (a.second > b.second);
}

int main(int argc, const char *argv[]) {
	std::ios_base::sync_with_stdio(false);

	// Make sure the right number of command line arguments exist.
	if ((argc <2) || (argc > 3)) {
		std::cerr << "Usage: " << argv[0] << " <filename> [limit]" << std::endl;
		return 1;
	}
	int limit;
	if (argc==3){
		std::istringstream iss(argv[2]);
		if (!(iss>>limit) || (limit < 0)) {
			std::cerr << "Error: Invalid limit '" << argv[2] << "' received.";
			return 1;
		}else{
			iss>>limit;
		}
		iss.clear();
	}else{
		limit= 10;
	}
	// Create an ifstream object.
	std::ifstream input_file(argv[1]);
	// If it does not exist, print an error message.
	if (!input_file) {
		std::cerr << "Error: Cannot open file '" << argv[1] << "' for input.\n";
		return 1;
	}
	// Add read errors to the list of exceptions the ifstream will handle.
	input_file.exceptions(std::ifstream::badbit);
	std::string line;
	std::string lower;
	RedBlackTree<std::string, int> *rbt = new RedBlackTree<std::string, int>();
	try {
		// Use getline to read in a line.
		// See http://www.cplusplus.com/reference/string/string/getline/
		while (getline(input_file, line)) {
			//gets rid of unwanted characters
			line.erase( remove_if(line.begin(),
					line.end(),
					[](char c) { return !(isalpha(c)) && (c != '-') && (c != '\'') && (c != ' ') ; }),
					line.end());
			transform(line.begin(), line.end(), line.begin(), ::tolower);

			std::stringstream stream(line);
			std::string token;

			while (getline(stream, token, ' ')){
				if (token != "") {
					if (rbt->find(token) == rbt->end()) { // new word! IF HAVE TIME, OPTIMIZE THIS PART
						try {
							rbt->insert(token, 1);
						} catch (const tree_exception &te) {
							std::cerr << "Warning: " << te.what() << "\n";
						}
					} else { //duplicate, add to value
						rbt->find(token)->set_value(
								rbt->find(token)->value() + 1);
					}
				}
			}
			stream.clear();
		}
		// Don't forget to close the file.
		input_file.close();
	} catch (const std::ifstream::failure &f) {
		std::cerr << "Error: An I/O error occurred reading '" << argv[1] << "'.";
		return 1;
	}

	//adding tree items into a vector
	std::vector< std::pair< std::string, int > > nodes;
	RedBlackTree<std::string, int>::iterator it = rbt->begin();
	std::pair<std::string, int> temp;

	while (it != rbt->end()) {
		temp = make_pair(it->key(), it->value());
		nodes.push_back(temp);
		++it;
	}

	//sorting the vector, sortbysec see top
	stable_sort(nodes.begin(), nodes.end(), sortbysec);

	display_top(nodes, limit);


	delete rbt;
	return 0;
}
