#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK
//
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,
		unsigned int month_date,  unsigned year_date,
		bool buy_sell_trans,  unsigned int number_shares,
		double trans_amount ){
	day = day_date;
	month = month_date;
	year = year_date;
	symbol = ticker_symbol;
	shares = number_shares;
	amount = trans_amount;
	p_next = nullptr;
	trans_id = assigned_trans_id;
	assigned_trans_id++;
	acb = 0;
	share_balance = 0;
	acb_per_share = 0;
	cgl = 0;
	if(buy_sell_trans){
		trans_type = "Buy";
	}
	else{
		trans_type = "Sell";
	}
}


// Destructor
// TASK
//
Transaction::~Transaction(){
	day = 0;
	month = 0;
	year = 0;
	symbol = '\0';
	shares = 0;
	amount = 0;
	acb = 0;
	share_balance = 0;
	acb_per_share = 0;
	cgl = 0;
	trans_type = '\0';
}

// TASK
// Overloaded < operator.
//
bool Transaction::operator<( Transaction const &other ){
	if(this->year == other.year){
		if(this->month == other.month){
			if(this->day == other.day){
				return false;
			}
			else if(this->day < other.day){
				return true;
			}
			else{
				return false;
			}
		}
		else if(this->month < other.month){
			return true;
		}
		else{
			return false;
		}
	}
	else if(this->year < other.year){
		return true;
	}
	else{
		return false;
	}
}

// GIVEN
// Member functions to get values. 
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values. 
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
	std::cout << std::fixed << std::setprecision(2);
	std::cout << std::setw(4) << get_trans_id() << " "
			<< std::setw(4) << get_symbol() << " "
			<< std::setw(4) << get_day() << " "
			<< std::setw(4) << get_month() << " "
			<< std::setw(4) << get_year() << " ";


	if ( get_trans_type() ) {
		std::cout << "  Buy  ";
	} else { std::cout << "  Sell "; }

	std::cout << std::setw(4) << get_shares() << " "
			<< std::setw(10) << get_amount() << " "
			<< std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
			<< std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
			<< std::setw(10) << std::setprecision(3) << get_cgl()
			<< std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK
//
History::History(){
	p_head = nullptr;
}

// Destructor
// TASK
//
History::~History(){
	Transaction *p_new_head = p_head;
	while(p_head != nullptr){
		p_new_head = p_head;
		p_head = p_head -> get_next();
		delete p_new_head;
		p_new_head = nullptr;
	}
	delete p_head;
	p_head = nullptr;
}

// TASK
// read_transaction(...): Read the transaction history from file. 
//
void History::read_history(){
	ece150::open_file();
	while(ece150::next_trans_entry()){
		Transaction *pt_trans =new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(),
				ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount());
		insert(pt_trans);
	}
	ece150::close_file();
}

// insert(...): Insert transaction into linked list.
//
void History::insert(Transaction *p_new_trans){
	if(p_head == nullptr){
		p_head = p_new_trans;
	}
	else{
		Transaction *p_list_tail = p_head;
		while(p_list_tail -> get_next() != nullptr){
			p_list_tail = p_list_tail -> get_next();
		}
		p_list_tail -> set_next(p_new_trans);
		p_new_trans -> set_next(nullptr);
	}
}


// TASK
// sort_by_date(): Sort the linked list by trade date.
//
void History::sort_by_date(){
	Transaction *p_sorted = nullptr;
	p_sorted = p_head;
	p_head = p_head -> get_next();
	p_sorted->set_next(nullptr);
	Transaction *p_sorted_head = p_sorted;
	Transaction *p_unlinked;
	Transaction *p_temp_before;
	while(p_head != nullptr){
		p_unlinked = p_head;
		p_head = p_head -> get_next();
		if(p_unlinked->operator <(*p_sorted_head)){
			p_unlinked->set_next(p_sorted_head);
			p_sorted_head = p_unlinked;
		}
		else{
			p_sorted = p_sorted_head;
			while(!(p_unlinked->operator <(*p_sorted)) && p_sorted->get_next() != nullptr){
				p_temp_before = p_sorted;
				p_sorted = p_sorted -> get_next();
			}
			if(p_sorted->get_next() == nullptr && !(p_unlinked->operator <(*p_sorted))){
				p_sorted->set_next(p_unlinked);
				p_unlinked->set_next(nullptr);
			}
			else{
				p_temp_before->set_next(p_unlinked);
				p_unlinked->set_next(p_sorted);
			}
		}
	}
	p_head = p_sorted_head;
}



// TASK
// update_acb_cgl(): Updates the ACB and CGL values. 
//
void History::update_acb_cgl(){
	Transaction *temp = p_head;
	double total_acb = 0;
	int total_share_balance = 0;
	double prev_acb_per_share = 0;
	while(temp != nullptr){
		if(temp->get_trans_type()){
			total_acb += temp->get_amount();
			temp->set_acb(total_acb);
			total_share_balance += temp->get_shares();
		}
		else if(!(temp->get_trans_type())){
			total_acb -= (temp->get_shares() * (prev_acb_per_share));
			temp->set_acb(total_acb);
			total_share_balance -= temp->get_shares();
			temp->set_cgl(temp->get_amount() - (temp->get_shares() * prev_acb_per_share));
		}
		temp->set_share_balance(total_share_balance);
		temp->set_acb_per_share(temp->get_acb()/temp->get_share_balance());
		prev_acb_per_share = temp->get_acb_per_share();
		temp = temp -> get_next();
	}
}

// TASK
// compute_cgl(): )Compute the ACB, and CGL.
//
double History::compute_cgl(unsigned int year){
	Transaction *p_current = p_head;
	double yearly_cgl = 0;
	while(p_current != nullptr){
		if(p_current->get_year() == year){
			yearly_cgl += p_current->get_cgl();
		}
		p_current = p_current->get_next();
	}
	return yearly_cgl;
}

// TASK
// print() Print the transaction history.
//
void History::print(){
	std::cout << "============= BEGIN TRANSACTION =============" << std::endl;
	Transaction *p_step = p_head;
	while(p_step != nullptr){
		p_step->print();
		p_step = p_step->get_next();
	}
	std::cout << "============= END TRANSACTION =============" << std::endl;
}


// GIVEN
// get_p_head(): Full access to the linked list.
// 
Transaction *History::get_p_head() { return p_head; }


#endif

