// Shoimya Chowdhury Project 02 header file
/** header file containing funtions that take in a multiline string as
 * instruction and returns output. Fucntions for a bank balance 
 * calculator, makes deposists, allows withdrawls and calculates 
 * interest.**/

//Most Idea and structure from Dr. Nahum videos and review session
//Helproom TAs clarified  doubts and aided in building fucntions
#ifndef PROJ_2
#define PROJ_2

#include <iostream>
#include <string>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>

// adds double y to the balance
void deposit(double& x, double y) { 
  x = x + y;
}
/** returns true and subtracts y from balance or return false if y greater than
 balance**/
bool withdraw(double& x, double y) {
  if (x >= y) {
    x = x - y;
    return true;
  }
  return false;
}
// if withdraw returns false, overdraft is performed on the balance
void overdraft(double& x, double y) {
  x = x - y;
  x = x - 35.00;
}
// parses a string and updates dereferenced values of pointers
void string_date_to_int_ptrs(std::string s, int* y, int* m, int* d) {
  std::stringstream ss(s);
  char tmp1;
  char tmp2;
  ss >> *y >> tmp1 >> *m >> tmp2 >> *d;
}
// uses above function to update int values
void string_date_to_ints(std::string str, int& yy, int& mm, int& dd) {
  string_date_to_int_ptrs(str, &yy, &mm, &dd);
}
/**interest calculates once every month, this fucntion calculates number of
 months between current and previous date**/
int number_of_first_of_months(std::string d1, std::string d2) {
  int year1, month1, day1;
  int year2, month2, day2;
  string_date_to_ints(d1, year1, month1, day1);
  string_date_to_ints(d2, year2, month2, day2);
  if (year2 - year1 == 0) {
    if (month2 - month1 == 0) {
      return 0;
    } else {
      return month2 - month1;
    }
  } else {
    return (((year2 - year1) * 12) - (month1 - month2));
  }
}
// calculates the interest of a month using the apr value provided
double interest_for_month(double balance, double apr) {
  double interest = balance * (apr / 12) * 0.01;
  interest = std::floor(interest * 100) / 100;
  return interest;
}
/** adds interest to the balance and subtracts original value to find the total
interest earned**/
double interest_earned(double balance, double apr, std::string date_early,
                       std::string date_late) {
  int count = number_of_first_of_months(date_early, date_late);
  if (balance < 0) {
    return 0;
  }
  double copy = balance;
  for (int i = 1; i <= count; i++) {
    double intr = interest_for_month(balance, apr);
    balance += intr;
  }
  return balance - copy;
}
/**helper fucntion to process command to process each command and add to output
stream**/
auto perform(std::vector<std::string>& c, double& balance, double apr,
             std::string& previous_date) {
  std::ostringstream oss;
  for (auto& itr : c) {
    std::stringstream ss(itr);
    std::string date, command;
    char dollar;
    double amount;
    while (ss >> date >> command >> dollar >> amount) {
      std::string only_com = itr.substr(11);  // only the command
      bool ovr_draft = false;
      oss << "On " << date << ": Instructed to perform "
          << "\"" << only_com << "\"\n";
      if (previous_date == "") {  // if date is empty, updates it
        previous_date = date;
      }
      int num_of_intr_performed =
          number_of_first_of_months(previous_date, date);
      double intr = interest_earned(balance, apr, previous_date, date);
      if (command == "Deposit") {
        deposit(balance, amount);
      } else if (command == "Withdraw") {
        if (withdraw(balance, amount) == true) {
        } else {
          ovr_draft = true;
          overdraft(balance, amount);
        }
      }
      if (num_of_intr_performed > 0) {
        balance += intr;
        oss << "Since " << previous_date << ", interest has accrued "
            << num_of_intr_performed << " times."
            << "\n" << dollar << std::setprecision(2) << std::fixed << intr
            << " interest has been earned."
            << "\n";
      }
      if (ovr_draft == true) {
        oss << "Overdraft!"
            << "\n";
      }
      oss << "Balance: " << std::setprecision(2) << std::fixed << balance
          << "\n";
      previous_date = date;  // updates date after each command is processed
    }
  }
  std::string result_str = oss.str();
  return result_str;
}
// takes line of command, could be multiple, and process it
std::string process_command(std::string input_str, std::string& previous_date,
                            double& balance, double apr) {
  std::istringstream iss(input_str);
  std::string each_command;
  std::vector<std::string> commands;
  while (std::getline(iss, each_command, '\n')) {
    commands.push_back(
        each_command);  // adds each coomand to a vector of strings
  }
  std::string rslt = perform(commands, balance, apr, previous_date);
  return rslt;
}
/**takes multiline string and apr from input, uses process command to return a
final output string**/
std::string process_commands(std::string input, int apr) {
  double balance = 0;
  std::string previous_date = "";
  std::string final_result =
      process_command(input, previous_date, balance, apr);
  return final_result;
}

#endif