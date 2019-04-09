//============================================================================
// Name        : assignment-3.cpp
// Author      : Reyyan Oflaz
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
// 				 ASSIGNMENT 3-LINKED LIST
//
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <sstream>
using namespace std;

#define OPERATION_PAIR_IDX_0	0
#define OPERATION_PAIR_IDX_1 	1
#define NUMBER_OF_GOALS_HAT_TRICK	3
struct doubly_node_t{
	vector<string> away_team;
	vector<string> minute_of_goal;
	vector<string> match_id;
	doubly_node_t *prev_p;
	doubly_node_t *next_p;
};
struct singly_node_t{
	vector<string> footballers_name;
	vector<string> team_name;
	doubly_node_t *head_doubly;   	//pointer to head of doubly linked list
	singly_node_t *next_p;			//pointer to next node of singly linked list
};

struct football_league_t{
	singly_node_t *head_p;
	int number_of_footballer;
	pair<int,string> *footballer_goal_p;	//pointer to footballer(s) # of goal-footballer name pair
	vector<string> *match_id_v;
};
struct operation_pair_t{
	vector<string> f_match_pair;
	vector<string> f_ascending_pair;
	vector<string> f_descending_pair;
};
enum operation_e{
	ENUM_OPERATION_MOST_SCORED_HALF,
	ENUM_OPERATION_GOAL_SCORER,
	ENUM_OPERATION_SCORED_HAT_TRICK,
	ENUM_OPERATION_LIST_OF_TEAMS,
	ENUM_OPERATION_LIST_OF_FOOTBALLERS,
	ENUM_OPERATION_MATCH_OF_GIVEN_FOOTBALLER,
	ENUM_OPERATION_ASCENDING_ORDER_MATCH_ID,
	ENUM_OPERATION_DESCENDING_ORDER_MATCH_ID,
	ENUM_OPERATION_END
};
enum print_order_e{
	ENUM_ORDER_ASCENDING,
	ENUM_ORDER_DESCENDING
};
template<char delimiter>
class WordDelimitedBy : public std::string
{};
class WordDelimitedByComma : public std::string
{};
bool parse_InputFile(ifstream& file,football_league_t& football_league, std::vector<std::string>& vecOfStrs);
void parse_OperationFile(ifstream& file, operation_pair_t& operation_pair, vector<string>& operation_v);
void league_allocate_memory(football_league_t& football_league);
void league_initialize_footballers(football_league_t& football_league, vector<string>& file_v);
bool league_CheckFootballersExist(singly_node_t*& head,string& footballer_name,singly_node_t *&current_footballer);
int league_number_of_goal(doubly_node_t*& doubly_node);
void league_MathcOfGivenFootballer(singly_node_t*& head_s,doubly_node_t*& head_d,ofstream& output_file);
bool compare_by_goal(const std::pair<int, string>& p1, const std::pair<int, string>& p2);
void league_search_engine(football_league_t& football_league,vector<operation_e> operation,operation_pair_t& operation_pair,ofstream& output_file);
void league_Search_MostScoredHalf(football_league_t& football_league,ofstream& output_file);
void league_Search_GoalScorer(football_league_t& football_league,ofstream& output_file);
void league_Search_ScoredHatTricks(football_league_t& football_league,ofstream& output_file);
void league_Search_ListOfTeams(football_league_t& football_league,ofstream& output_file);
void league_Search_ListOfFootballers(football_league_t& football_league,ofstream& output_file);
void league_Search_MatchOfFootballers(football_league_t& football_league,operation_pair_t& operation_pair,ofstream& output_file);
void league_Search_OrderMatchID(football_league_t& football_league,string& footballer_1,string& footballer_2,print_order_e print_order,ofstream& output_file);
void init_TokenizeFootballerInfo(string& line,vector<string>& tokenized_line);
struct doubly_node_t* doubly_GetNewNode(string& away_team,string& minute_of_goal,string& match_id);
void doubly_sortedInsert(struct doubly_node_t*& head_ref,doubly_node_t*& new_node);
void doubly_InsertAtHead(doubly_node_t*& head,string& away_team,string& minute_of_goal,string& match_id);
void doubly_printAscendingOrder(doubly_node_t*& head,singly_node_t*& singly_head,ofstream& output_file);
void doubly_printDescendingOrder(doubly_node_t*& head,singly_node_t*& singly_head,ofstream& output_file);
void doubly_Print(doubly_node_t*& head);
void singly_Print(singly_node_t*& head);
void singly_PushAtHead(singly_node_t*& head,string& _footballers_name,string& team_name);
/*
 *
 */
int main(int argc,char* argv[]) {
	vector<string> input_file_v;
	vector<string> operation_v;
	football_league_t football_league ;
	operation_pair_t operation_pair;
	vector<operation_e> operation;
	std::ifstream input_file(argv[1]);
	std::ifstream operation_file(argv[2]);
	ofstream output_file;
	output_file.open ("output.txt");
	league_allocate_memory(football_league);
	parse_InputFile(input_file,football_league, input_file_v);
	parse_OperationFile(operation_file,operation_pair,operation_v);
	football_league.footballer_goal_p= new pair<int,string>[football_league.number_of_footballer];
	football_league.match_id_v=new vector<string>[football_league.number_of_footballer];
	int i;
	for(i = 0; i < ENUM_OPERATION_END; i++){
		operation.push_back(static_cast<operation_e>(i));
	}
	league_search_engine(football_league,operation,operation_pair,output_file);
	input_file.close();
	operation_file.close();
	output_file.close();
	return 0;
}
/*
 *
 */
void league_allocate_memory(football_league_t& football_league){
	football_league.head_p = NULL;		//no footballer exist in start
}
/*
 * @brief Checks file can be opened.Reads line into string vector.Tokenize line into parameter.Fills pairs parameter
 * 			with given footballer name pair to corresponding operation pair.
 */
void parse_OperationFile(ifstream& file,operation_pair_t& operation_pair, vector<string>& operation_v){
	if(!file)
	{
		std::cerr << "Cannot open the operation File : "<<std::endl;
		return ;
	}
	std::string line;
	// Read the next line from File until it reaches the end.
	while (std::getline(file, line))
	{
		// Line contains string of length > 0 then save it in vector
		if(line.size() > 0){
			operation_v.push_back(line);
		}
	}
	vector<std::string> tokenized_line;
	vector<string>::iterator iter;
	for (iter = operation_v.begin(); iter != operation_v.end(); ++iter) {
		init_TokenizeFootballerInfo((*iter),tokenized_line);
		auto position = std::distance( operation_v.begin(), iter ) ;
		if(position == 0){
			operation_pair.f_match_pair.push_back(tokenized_line[OPERATION_PAIR_IDX_0]);
			operation_pair.f_match_pair.push_back(tokenized_line[OPERATION_PAIR_IDX_1]);
		}
		else if(position ==1){
			operation_pair.f_ascending_pair.push_back(tokenized_line[OPERATION_PAIR_IDX_0]);
			operation_pair.f_ascending_pair.push_back(tokenized_line[OPERATION_PAIR_IDX_1]);
		}
		else if(position==2){
			operation_pair.f_descending_pair.push_back(tokenized_line[OPERATION_PAIR_IDX_0]);
			operation_pair.f_descending_pair.push_back(tokenized_line[OPERATION_PAIR_IDX_1]);

		}
	}
}
/*
 * @brief Checks file can be opened.Reads line into string vector.Tokenize line into parameter.
 * 			Call league_initialize_footballers() function for construct linked list sructure.
 */
bool parse_InputFile(ifstream& file, football_league_t& football_league, vector<string>& file_v)
{
	// Check if object is valid
	if(!file)
	{
		std::cerr << "Cannot open the File : "<<std::endl;
		return false;
	}
	std::string line;
	// Read the next line from File until it reaches the end.
	while (std::getline(file, line))
	{
		// Line contains string of length > 0 then save it in vector
		if(line.size() > 0){
			file_v.push_back(line);
		}
	}
	league_initialize_footballers(football_league,file_v);
	return true;
}
/*
 * @brief Overloaded function for parse line by comma.
 */
std::istream& operator>>(std::istream& is, WordDelimitedByComma& output)
{
    std::getline(is, output, ',');
    return is;
}
/*
 *@brief Sort vector string with descending order.Insert singly node at head,since addition done by head now linked list is constructed with
 *			ascending order.By looking at whether the player has been added,add footballer to singly linked list and insert doubly linked list with given
 *			parameter.If footballer already present only insert doubly linked list.
 *			Node added an ascending order to the doubly linked list.This is done by doubly_sorted_Insert().
 *			Counts number of footballer and initialize footballer count.
 */
void league_initialize_footballers(football_league_t& football_league, vector<string>& file_v){
	std::sort(std::begin(file_v), std::end(file_v), [](std::string const &a, std::string const &b)
			{
		return std::lexicographical_compare(std::begin(a), std::end(a), std::begin(b), std::end(b), [](std::string::value_type a, std::string::value_type b)
				{
			return std::tolower(a) > std::tolower(b);
				});
			});
	singly_node_t *current_footballer=football_league.head_p;			//initially is head
	vector<string>::iterator iter;
	std::vector<std::string> tokenized_line;
	int i=0;
	doubly_node_t *new_node;
	for (iter = file_v.begin(); iter != file_v.end(); ++iter) {
	    init_TokenizeFootballerInfo((*iter), tokenized_line);
	    //if footballer  does not exist
	    if(league_CheckFootballersExist(football_league.head_p,tokenized_line[0],current_footballer)==true){
	    	new_node = doubly_GetNewNode(tokenized_line[2], tokenized_line[3], tokenized_line[4]);
	    	doubly_sortedInsert(current_footballer->head_doubly, new_node);
//	    	doubly_InsertAtHead(current_footballer->head_doubly, tokenized_line[2], tokenized_line[3], tokenized_line[4]);
	    }
	    else{
	    	i++;
	    	singly_PushAtHead(football_league.head_p, tokenized_line[0], tokenized_line[1]);
	    	new_node = doubly_GetNewNode(tokenized_line[2], tokenized_line[3], tokenized_line[4]);
	    	doubly_sortedInsert(football_league.head_p->head_doubly, new_node);
//	    	doubly_InsertAtHead(football_league.head_p->head_doubly, tokenized_line[2], tokenized_line[3], tokenized_line[4]);
	    }
	}
	football_league.number_of_footballer=i;
}
/*
 * @brief If no any footballer exist simply return false.
 * 			Iterates singly linked list.If footballer with given footballer name is not exist returns false.
 * 			If exist; return the pointer of the current footballer node.
 */
bool league_CheckFootballersExist(singly_node_t*& head,string& footballer_name,singly_node_t*& current_footballer) {
	struct singly_node_t *temp = head;
	if(temp==NULL){			//no any footballer exist
		return false;
	}
	std::string s_footballers_name;
	for (const auto &piece : temp->footballers_name) s_footballers_name += piece;
	while(temp != NULL){
		if (!s_footballers_name.compare(footballer_name)){
			current_footballer=temp;
			return true;
		}
			temp = temp->next_p;
	}
	printf("\n");
	return false;
}
/*
 * @brief This is the mechanism of the search engine.Enumerated operation pushed to vector before this mechanism called.So simply check the state of
 * 			mechanism, do corresponding job of the enumerated search command.
 */
void league_search_engine(football_league_t& football_league,vector<operation_e> operation,operation_pair_t& operation_pair,ofstream& output_file){
	vector<operation_e>::iterator iter;
	operation_e temp_operation;
	print_order_e print_order;
	for (iter = operation.begin(); iter != operation.end(); ++iter) {
		auto position = std::distance( operation.begin(), iter ) ;
		temp_operation=operation[position];
		if(temp_operation ==ENUM_OPERATION_MOST_SCORED_HALF){
			output_file<<"1)THE MOST SCORED HALF"<<endl;
			league_Search_MostScoredHalf(football_league,output_file);
		}
		else if(temp_operation ==ENUM_OPERATION_GOAL_SCORER){
			output_file<<"2)GOAL SCORER"<<endl;
			league_Search_GoalScorer(football_league,output_file);
		}
		else if(temp_operation == ENUM_OPERATION_SCORED_HAT_TRICK){
			output_file<<"3)THE NAMES OF FOOTBALLERS WHO SCORED HAT-TRICK"<<endl;
			league_Search_ScoredHatTricks(football_league,output_file);
		}
		else if(temp_operation== ENUM_OPERATION_LIST_OF_TEAMS){
			output_file<<"4)LIST OF TEAMS"<<endl;
			league_Search_ListOfTeams(football_league,output_file);
		}
		else if(temp_operation ==ENUM_OPERATION_LIST_OF_FOOTBALLERS){
			output_file<<"5)LIST OF FOOTBALLERS"<<endl;
			league_Search_ListOfFootballers(football_league,output_file);
		}
		else if(temp_operation==ENUM_OPERATION_MATCH_OF_GIVEN_FOOTBALLER){
			output_file<<"6)MATCHES OF GIVEN FOOTBALLER"<<endl;
			league_Search_MatchOfFootballers(football_league,operation_pair,output_file);
		}
		else if(temp_operation ==ENUM_OPERATION_ASCENDING_ORDER_MATCH_ID){
			output_file<<"7)ASCENDING ORDER ACCORDING TO MATCH ID"<<endl;
			print_order=ENUM_ORDER_ASCENDING;
			league_Search_OrderMatchID(football_league,operation_pair.f_ascending_pair[0],operation_pair.f_ascending_pair[1],print_order,output_file);
		}
		else if(temp_operation == ENUM_OPERATION_DESCENDING_ORDER_MATCH_ID){
			output_file<<"8)DESCENDING ORDER ACCORDING TO MATCH ID"<<endl;
			print_order=ENUM_ORDER_DESCENDING;
			league_Search_OrderMatchID(football_league,operation_pair.f_descending_pair[0],operation_pair.f_descending_pair[1],print_order,output_file);
		}
	}
}
/*
 *@brief Iterates doubly node for every singly node.Pass minute of goal to integer.Takes statistic of minute of goals for find leagues
 *		most scored half.
 */
void league_Search_MostScoredHalf(football_league_t& football_league,ofstream& output_file){
	int total_sc_first_h=0;
	int minute_of_goal;
	int total_sc_second_h=0;
	singly_node_t *temp_s=football_league.head_p;
	doubly_node_t *temp_d;
	while(temp_s!=NULL){
		temp_d=temp_s->head_doubly;
		while(temp_d!=NULL){
			std::istringstream ss(temp_d->minute_of_goal[0]);
			ss >> minute_of_goal;
			if(minute_of_goal > 45 && minute_of_goal<=90 ){
				total_sc_second_h=total_sc_second_h+1;
			}
			else if(minute_of_goal<=45 && minute_of_goal >=0){
				total_sc_first_h=total_sc_first_h+1;
			}
			temp_d=temp_d->next_p;
		}
		temp_s=temp_s->next_p;
	}
	if(total_sc_first_h>total_sc_second_h){
		output_file<<"0"<<endl;
	}
	else if(total_sc_second_h> total_sc_first_h){
		output_file<<"1"<<endl;
	}
	else{
		output_file<<"-1"<<endl;
	}
}
/*
 *
 */
void league_Search_GoalScorer(football_league_t& football_league,ofstream& output_file){

	singly_node_t *temp_s=football_league.head_p;
	pair<int,string> *temp_pair=football_league.footballer_goal_p;
	int i=0;
	int max_goal;
	temp_pair[0].first=league_number_of_goal(temp_s->head_doubly);
	max_goal = temp_pair[0].first;
	temp_pair[0].second=temp_s->footballers_name[0];
	while(temp_s!=NULL){
		temp_pair[i].first=league_number_of_goal(temp_s->head_doubly);
		temp_pair[i].second=temp_s->footballers_name[0];
		if(max_goal < temp_pair[i].first){
			max_goal = temp_pair[i].first;
		}
		i++;
		temp_s=temp_s->next_p;
	}
	int j;
	for(j=0;j<football_league.number_of_footballer ;j++){
		if(temp_pair[j].first == max_goal){
			output_file <<temp_pair[j].second<<endl;
		}
	}
}
bool compare_by_goal(const std::pair<int, string>& p1, const std::pair<int, string>& p2){
	return (p1.first < p2.first);
}
/*
 * @brief Counts number of goal for current footballer.
 */
int league_number_of_goal(doubly_node_t*& doubly_node){
	doubly_node_t *temp_d=doubly_node;
	int number_of_goal=0;
	while(temp_d!=NULL){
		number_of_goal++;
		temp_d=temp_d->next_p;
	}
	return number_of_goal;
}

/*
 *
 */
void league_Search_ScoredHatTricks(football_league_t& football_league,ofstream& output_file){

	singly_node_t *head_s=football_league.head_p;
	doubly_node_t *head_d;
	int i=0;
	while(head_s!=NULL){
		head_d=head_s->head_doubly;
		while(head_d!=NULL){
			football_league.match_id_v[i].push_back(head_d->match_id[0]);
			head_d=head_d->next_p;
		}
		vector<string>::iterator it;
		int freq;
		for( it = football_league.match_id_v[i].begin(); it != football_league.match_id_v[i].end(); ++it){
			freq = std::count(football_league.match_id_v[i].begin(), football_league.match_id_v[i].end(), (*it));
			if(freq >=NUMBER_OF_GOALS_HAT_TRICK){
				output_file<< head_s->footballers_name[0]<<endl;
				break;
			}
		}
		i++;
		head_s=head_s->next_p;
	}
}
/*
 *iteratorle vectoru dolas.Print et
 */
void league_Search_ListOfTeams(football_league_t& football_league,ofstream& output_file){
	vector<string> team_v;
	singly_node_t *head_s=football_league.head_p;
	team_v.push_back(head_s->team_name[0]);			//initial addition
	output_file<<head_s->team_name[0]<<endl;
	while(head_s!=NULL){
		if ( !(std::find (team_v.begin(), team_v.end(), head_s->team_name[0]) != team_v.end()))
		{
			output_file<<head_s->team_name[0]<<endl;
			team_v.push_back(head_s->team_name[0]);
		}
		head_s=head_s->next_p;
	}

}
void league_Search_ListOfFootballers(football_league_t& football_league,ofstream& output_file){
	singly_node_t *temp_head=football_league.head_p;
	while(temp_head!=NULL){
		output_file<<temp_head->footballers_name[0]<<endl;
		temp_head=temp_head->next_p;
	}
}
/*
 *@brief Since operation file only 2 footballer pair for print match of footballer,it is guaranteed that 2 fotballers pointer to singly node and pointer to
 * doubly node enought to print info about footballer,no need to traverse for each footballer,instead hold footballer's pointer info.
 * Checks 2 footballers exist and
 */
void league_Search_MatchOfFootballers(football_league_t& football_league,operation_pair_t& operation_pair,ofstream& output_file){
	singly_node_t *head_s=football_league.head_p;
	singly_node_t *footballer_1_s;
	singly_node_t *footballer_2_s;
	doubly_node_t *footballer_1_d;
	doubly_node_t *footballer_2_d;
	bool footballer_1_found;
	bool footballer_2_found;
	while(head_s!=NULL){
		if(head_s->footballers_name[0].compare(operation_pair.f_match_pair[0])==0 ){
			footballer_1_s=head_s;
			footballer_1_d= head_s->head_doubly;
			footballer_1_found=true;
		}
		else if(head_s->footballers_name[0].compare(operation_pair.f_match_pair[1])==0 ){
			footballer_2_s=head_s;
			footballer_2_d= head_s->head_doubly;
			footballer_2_found=true;
		}
		head_s=head_s->next_p;
	}
	if(footballer_1_found==true && footballer_2_found==true){
		league_MathcOfGivenFootballer(footballer_1_s,footballer_1_d,output_file);
		league_MathcOfGivenFootballer(footballer_2_s,footballer_2_d,output_file);
	}
	else{
		//Error number 1 : given footballer is not exist in linked list Nothing printed
	}
}
/*
 *
 */
void league_MathcOfGivenFootballer(singly_node_t*& head_s,doubly_node_t*& head_d,ofstream& output_file){
	doubly_node_t *temp_head_d=head_d;
	while(temp_head_d!=NULL){
		output_file<<"Footballer Name: "<<head_s->footballers_name[0]<<",Away Team: "<<temp_head_d->away_team[0]<<",Min of Goal: "<<temp_head_d->minute_of_goal[0]<<",Match ID: "<<temp_head_d->match_id[0]<<endl;
		temp_head_d=temp_head_d->next_p;
	}
}
void league_Search_OrderMatchID(football_league_t& football_league,string& footballer_1,string& footballer_2,print_order_e print_order,ofstream& output_file){
	singly_node_t *head_s=football_league.head_p;
	singly_node_t *footballer_1_s;
	singly_node_t *footballer_2_s;
	doubly_node_t *footballer_1_d;
	doubly_node_t *footballer_2_d;
	bool footballer_1_found;
	bool footballer_2_found;
	while(head_s!=NULL){
		if(head_s->footballers_name[0].compare(footballer_1)==0 ){
			footballer_1_s=head_s;
			footballer_1_d= head_s->head_doubly;
			footballer_1_found=true;
		}
		else if(head_s->footballers_name[0].compare(footballer_2)==0 ){
			footballer_2_s=head_s;
			footballer_2_d= head_s->head_doubly;
			footballer_2_found=true;
		}

		head_s=head_s->next_p;
	}
	if(footballer_1_found==true && footballer_2_found==true){
		if(print_order==ENUM_ORDER_ASCENDING){
			doubly_printAscendingOrder(footballer_1_d,footballer_1_s,output_file);
			doubly_printAscendingOrder(footballer_2_d,footballer_2_s,output_file);
		}
		else if(print_order==ENUM_ORDER_DESCENDING){
			doubly_printDescendingOrder(footballer_1_d,footballer_1_s,output_file);
			doubly_printDescendingOrder(footballer_2_d,footballer_2_s,output_file);
		}
	}
	else{
			//Error number 1 : given footballer is not exist in linked list Nothing printed
		}
}

void doubly_printAscendingOrder(doubly_node_t*& head,singly_node_t*& singly_head,ofstream& output_file) {
	struct doubly_node_t *temp = head;
	string match_id;
	if(temp == NULL) return;
	while(temp != NULL) {
		if(temp->prev_p==NULL){
			output_file<<"Footballer Name: "<<singly_head->footballers_name[0]<<",Match ID: "<<temp->match_id[0]<<endl;
			match_id=temp->match_id[0];
		}
		else{
			if(match_id!=temp->match_id[0]){
				output_file<<"Footballer Name: "<<singly_head->footballers_name[0]<<",Match ID: "<<temp->match_id[0]<<endl;
				match_id=temp->match_id[0];
			}
		}
		temp = temp->next_p;
	}
}
/*
 *@brief Prints footballers name and match id in reverse traversal order.
 */
void doubly_printDescendingOrder(doubly_node_t*& head,singly_node_t*& singly_head,ofstream& output_file) {
	struct doubly_node_t *temp = head;
	string match_id;
	if(temp == NULL) return; 	// empty list, exit
	// Going to last Node
	while(temp->next_p != NULL) {
		temp = temp->next_p;
	}
	// Traversing backward using prev pointer
	while(temp != NULL) {
		if(temp->next_p==NULL){
			output_file<<"Footballer Name: "<<singly_head->footballers_name[0]<<",Match ID: "<<temp->match_id[0]<<endl;
			match_id=temp->match_id[0];
		}
		else{
			if(match_id!=temp->match_id[0]){
				output_file<<"Footballer Name: "<<singly_head->footballers_name[0]<<",Match ID: "<<temp->match_id[0]<<endl;
				match_id=temp->match_id[0];
			}
		}
		temp = temp->prev_p;
	}
}
/*
 * @brief tokenize line with generalized delimiter template.
 * Solution 1.2 of given link : https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
 */
void init_TokenizeFootballerInfo(string& line,vector<string>& tokenized_line){
	std::istringstream iss(line);
	std::vector<std::string> result_tokens((std::istream_iterator<WordDelimitedByComma>(iss)),
			std::istream_iterator<WordDelimitedByComma>());
	tokenized_line=result_tokens;
}

/*
 * Creates a new doubly node and returns pointer to it.
 */
struct doubly_node_t* doubly_GetNewNode(string& away_team,string& minute_of_goal,string& match_id){
	struct doubly_node_t *new_node= new doubly_node_t();
	new_node->away_team.push_back(away_team);
	new_node->minute_of_goal.push_back(minute_of_goal);
	new_node->match_id.push_back(match_id);
	new_node->prev_p = NULL;
	new_node->next_p = NULL;
	return new_node;
}

/*
 *@brief Function to insert a new node in sorted way in a sorted doubly linked list
 *		https://www.geeksforgeeks.org/insert-value-sorted-way-sorted-doubly-linked-list/
 */
void doubly_sortedInsert(doubly_node_t*& head_ref,doubly_node_t*& new_node)
{
    struct doubly_node_t *current_node;
    // if list is empty
    if (head_ref == NULL)
        head_ref = new_node;
    // if the node is to be inserted at the beginning
    // of the doubly linked list
    else if (stoi(head_ref->match_id[0]) >= stoi(new_node->match_id[0])) {
        new_node->next_p = head_ref;
        new_node->next_p->prev_p = new_node;
        head_ref = new_node;
    }
    else {
        current_node = head_ref;
        // locate the node after which the new node
        // is to be inserted
        while (current_node->next_p != NULL && stoi(current_node->next_p->match_id[0]) <stoi(new_node->match_id[0])){
            current_node = current_node->next_p;
        }
        /* Make the appropriate links */
        new_node->next_p = current_node->next_p;
        // if the new node is not inserted at the end of the list
        if (current_node->next_p != NULL){
        	new_node->next_p->prev_p = new_node;
        }
        current_node->next_p = new_node;
        new_node->prev_p = current_node;
    }
}
/*
 * @Brief	Inserts a Node at head of doubly linked list
 * 			Implemented -not used.
 */
void doubly_InsertAtHead(doubly_node_t*& head,string& away_team,string& minute_of_goal,string& match_id) {
	struct doubly_node_t *new_node = doubly_GetNewNode(away_team,minute_of_goal,match_id);
	if(head == NULL) {
		head = new_node;
		return;
	}
	head->prev_p = new_node;
	new_node->next_p = head;
	head = new_node;
}

/*
 *@brief Prints all the elements in linked list in forward traversal order
 *			For debugging.
 */
void doubly_Print(doubly_node_t*& head) {
	struct doubly_node_t *temp = head;
	printf("Forward: ");
	while(temp != NULL) {
		temp = temp->next_p;
	}
	printf("\n");
}

/*
 *@brief Prints all the elements in linked list in forward traversal order
 */
void singly_Print(singly_node_t*& head) {
	struct singly_node_t *temp = head;

	std::vector<std::string> node_iterator;

	while(temp != NULL) {
		doubly_Print(temp->head_doubly);
		temp = temp->next_p;
	}
	printf("\n");
}


void singly_PushAtHead(singly_node_t*& head,string& _footballers_name,string& _team_name){

	struct singly_node_t *new_node = new singly_node_t;
	new_node->footballers_name.push_back(_footballers_name);
	new_node->team_name.push_back(_team_name);
	new_node->head_doubly = NULL;
	new_node->next_p = head;
	head = new_node;
}
