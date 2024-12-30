// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: stringthings.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef STRINGTHINGS_H
#define STRINGTHINGS_H

// Standard Header Files
#include <string>
#include <deque>
#include <algorithm>
#include <iostream>

// Rasfled Includes
#include "helper.h"

using namespace std;

int count_char_in_string(string& Text, char Character);

string char_buf_to_string(char* Buf, int Buf_Len);

string filter_non_printable(const string& input);
// Filter out non printable characters from string by replacing 
//  them with '.'.

string filter_all_non_printable(const string& input);
// Filter out non printable characters from string by replacing 
//  them with '.'.

string line_create(int Size, char Character);
// Returns an character filled line of Size.

string linefill(int size, string text);
// is broken
// Returns a space filled line of size with text in center.

string linemerge_left_justify(int size, string line, string text);
// Overlaps and right justifies text onto line.
// Returns value at size of size. 

string linemerge_left_justify(string line, string text);
// Overlaps and left justifies text onto line.
// No Truncate.
// Returns value at size of line or text size, whichever
//  is larger.

string linemerge_right_justify(int size, string line, string text);
// Overlaps and right justifies text onto line.
// Returns value at size. 

string right_justify(int size, string text);
// Overlaps and right justifies text onto line.
// Returns value at size. 

string left_justify(int size, string text);
// Overlaps and left justifies text onto line.
// Returns value at size. 

string center_justify(int size, string text);
// Overlaps and left justifies text onto line.
// Returns value at size. 

string left_trim(const string &Text);
// Remove leading spaces and special characters from text.

string right_trim(const string &Text);
// Remove trailing spaces and special characters from text.

string trim(const string &Text);
// Remove leading and trailing spaces and special characters from text.

string quotify(string Text);
// Put quotes around string;

string to_string_hex(char Char_Byte);
// Returns string two byte hex value from byte.

string to_string_binary(char Char_Byte);
// Returns 8 bit binary value from byte.

bool left_of_char(string Text, char Break_Char, string &Left);
// Returns left of Break_Char as Left;
// Returns true if Break_Char exist.

bool right_of_char(string Text, char Break_Char, string &Right);
// Returns right of Break_Char as Right;
// Returns true if Break_Char exist.

string remove_first_and_last_characters(char Character, string Text);
// For removing things like start and ending quotes.
// Value will be trimmed.

bool string_contains_word(string String_Value, string Word, bool Case_Sensitive = false);
// Returns true if Word is in String_Value

string string_to_lower_case(string String_Value);
// Returns String Value in lower case form.

bool string_to_int(string String_Value, int &Int_Value);
// Convert in String_Value number to out Int_Value.
//  Returns true if value sucessful.

int string_to_int(string String_Value);
// Convert in String_Value number to out int.
//  Returns Integer Value

bool string_hex_to_int(string String_Value, int &Int_Value);
// Convert in String_Value number to out Int_Value.
//  Returns true if value sucessful.

int string_hex_to_int(string String_Value);
// Convert in String_Value number to out int.
//  Returns Integer Value

bool string_to_ulong(string String_Value, unsigned long &Unsigned_Long_Value);
// Convert in String_Value number to out Int_Value.
//  Returns true if value sucessful.

unsigned long string_to_ulong(string String_Value);
// Convert in String_Value number to out ulong.
//  Returns Unsigned Long Value

bool string_to_ulonglong(string String_Value, unsigned long long &Unsigned_Long_Long_Value);
// Convert in String_Value number to out Unsigned_Long_Long_Value.
//  Returns true if value sucessful.

unsigned long long string_to_ulonglong(string String_Value);
// Convert in String_Value number to out Unsigned_Long_Long_Value.
//  Returns true if value sucessful.
// ! No Error Checking

bool string_to_float(string String_Value, float &Float_Value);
// Convert in String_Value number to out Float_Value.
//  Returns true if value sucessful.

float string_to_float(string String_Value);
// Convert in String_Value number to out Float_Value.
//  Returns true if value sucessful.
// ! No Error Checking

string to_string_round_to_nth(float Value, int nth);
// Returns string of float rounded to nth decimal.
//  like: float_to_string

short xor_checksum(string Line, char Start_Char, char End_Char);
// Short CHECKSUM

/**
 * Prints a vector of strings with a specified title.
 *
 * This function takes in a title and a reference to a vector of strings,
 * and prints out each string in the vector followed by its index, all within
 * a common section titled by the provided title.
 *
 * @param Title The title for the outputted list.
 * @param Vector_String A reference to a vector of strings to be printed.
 */
void print_vector_with_title(string Title, vector<string> &Vector_String);

string ask_a_question(string Question);

string return_vector_as_string(vector<string> &Vector_String, bool Add_LF = false);

class STRING_STRING
// Variable stores original string number and converted value.
// Sets error if conversion unsucessful.
{
  private:
  string  STR_VALUE = "";
  bool    CONVERSION_SUCCESS = false;

  public:

  void store(string str_value);

  string get_str_value();

  bool conversion_success();

};

class STRING_INT
// Variable stores original string number and converted value.
// Sets error if conversion unsucessful.
// Retains value if conversion not sucessful.
{
  private:
  string  STR_VALUE = "";
  int     INT_VALUE = 0;
  int     NEW_INT_VALUE = 0;
  bool    CONVERSION_SUCCESS = false;

  public:
  void store(string str_value);

  string get_str_value();

  int get_int_value();

  bool conversion_success();

};

class STRING_FLOAT
// Variable stores original string number and converted value.
// Sets error if conversion unsucessful.
// Retains value if conversion not sucessful.
{
  private:
  string  STR_VALUE = "";
  float   FLOAT_VALUE = 0;
  float   NEW_FLOAT_VALUE = 0;
  bool    CONVERSION_SUCCESS = false;

  public:
  void store(string str_value);

  void store_val(float value);
  
  string get_str_value();

  float get_float_value();

  int get_int_value();

  bool conversion_success();
};


#endif