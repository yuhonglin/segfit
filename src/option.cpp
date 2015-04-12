#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

#include "logger.hpp"
#include "option.hpp"

using namespace std;

namespace dms {

Option::Option() { num_opt = 0; }

Option::~Option() {}

void Option::set_head(string h) { head = h; }

void Option::set_tail(string t) { tail = t; }

/*
  interface for float
 */

void Option::add_float(string key, string comment, float defval,
                       set<float> domain) {
  stringstream ss;
  ss << defval;
  key_defval[key] = ss.str();
  key_type[key] = string("float");
  key_comment[key] = comment;
  if (domain.size() != 0) {
    set<string> dm;
    for (set<float>::iterator iter = domain.begin(); iter != domain.end();
         iter++) {
      ss.str("");
      ss << (*iter);
      dm.insert(ss.str());
    }
    key_domain[key] = dm;
  }
}

void Option::add_float(string key, string comment) {
  key_type[key] = string("float");
  key_comment[key] = comment;
  key_musthave.insert(key);
}

float Option::get_float(string k) {
  float ret;
  if (key_type.find(k) == key_type.end()) {
    stringstream ss1;
    LOG_ERROR("unused options: \"", k, "\", please check the program source");
    return -1;
  }
  stringstream ss2;
  if (key_value.find(k) != key_value.end()) {
    ss2 << key_value[k];
  } else {
    ss2 << key_defval[k];
  }
  ss2 >> ret;
  if (ss2.fail() or ss2.bad() or !ss2.eof()) {
    stringstream ss3("");
    LOG_ERROR("invalid float value for option: \"", k.c_str(),
              "\", please check your input options");
    return -1;
  }
  return ret;
}

/*
  interface for int
 */

void Option::add_int(string key, string comment, int defval, set<int> domain) {
  stringstream ss;
  ss << defval;
  key_defval[key] = ss.str();
  key_type[key] = string("int");
  key_comment[key] = comment;
  if (domain.size() != 0) {
    set<string> dm;
    for (set<int>::iterator iter = domain.begin(); iter != domain.end();
         iter++) {
      ss.str("");
      ss << (*iter);
      dm.insert(ss.str());
    }
    key_domain[key] = dm;
  }
}

void Option::add_int(string key, string comment) {
  key_type[key] = string("int");
  key_comment[key] = comment;
  key_musthave.insert(key);
}

int Option::get_int(string k) {
  int ret;
  if (key_type.find(k) == key_type.end()) {
    stringstream ss1;
    LOG_ERROR("unused options: \"", k, "\", please check the program source");
    return -1;
  }
  stringstream ss2;
  if (key_value.find(k) != key_value.end()) {
    ss2 << key_value[k];
  } else {
    ss2 << key_defval[k];
  }
  ss2 >> ret;
  if (ss2.fail() or ss2.bad() or !ss2.eof()) {
    stringstream ss3("");
    LOG_ERROR("invalid int value for option: \"", k.c_str(),
              "\", please check your input options");
    exit(1);
    return -1;
  }
  return ret;
}

/*
  interface for double
 */

void Option::add_double(string key, string comment, double defval,
                        set<double> domain) {
  stringstream ss;
  ss << defval;
  key_defval[key] = ss.str();
  key_type[key] = string("double");
  key_comment[key] = comment;
  if (domain.size() != 0) {
    set<string> dm;
    for (set<double>::iterator iter = domain.begin(); iter != domain.end();
         iter++) {
      ss.str("");
      ss << (*iter);
      cout << ss.str() << endl;
      dm.insert(ss.str());
    }
    key_domain[key] = dm;
  }
}

void Option::add_double(string key, string comment) {
  key_type[key] = string("double");
  key_comment[key] = comment;
  key_musthave.insert(key);
}

double Option::get_double(string k) {
  double ret;
  if (key_type.find(k) == key_type.end()) {
    stringstream ss1;
    ss1 << "unused options: \"" << k << "\", please check the program source";
    exit(1);
    return -1.;
  }
  stringstream ss2;
  if (key_value.find(k) != key_value.end()) {
    ss2 << key_value[k];
  } else {
    ss2 << key_defval[k];
  }
  ss2 >> ret;
  if (ss2.fail() or ss2.bad() or !ss2.eof()) {
    stringstream ss3("");
    ss3 << "invalid double value for option: \"" << k.c_str()
        << "\", please check your input options";
    exit(1);
    return -1.;
  }
  return ret;
}

/*
  interface for string
 */

void Option::add_string(string key, string comment, string defval,
                        set<string> domain) {
  key_defval[key] = defval;
  key_type[key] = string("string");
  key_comment[key] = comment;
  if (domain.size() != 0) {
    key_domain[key] = domain;
  }
}

void Option::add_string(string key, string comment) {
  key_type[key] = string("string");
  key_comment[key] = comment;
  key_musthave.insert(key);
}

string Option::get_string(string k) {
  if (key_type.find(k) == key_type.end()) {
    stringstream ss1;
    ss1 << "unused options: \"" << k << "\", please check the program source";
    exit(1);
    return "";
  }
  if (key_value.find(k) != key_value.end()) {
    return key_value[k];
  } else {
    return key_defval[k];
  }
}

void Option::add_bool(string key, string comment, bool defval) {
  if (defval == true) {
    key_defval[key] = "true";
  } else {
    key_defval[key] = "false";
  }
  key_type[key] = string("bool");
  key_comment[key] = comment;
}

bool Option::get_bool(string k) {
  if (key_type.find(k) == key_type.end()) {
    stringstream ss1;
    ss1 << "unused options: \"" << k << "\", please check the program source";
    exit(1);
    return "";
  }
  if (key_value.find(k) != key_value.end()) {
    if (key_value[k] == "true") {
      return true;
    } else if (key_value[k] == "false") {
      return false;
    } else {
      stringstream ss2;
      ss2 << "option \"" << k
          << "\" is not a bool type, please check the program source";
      exit(1);
      return -1;
    }
  } else {
    if (key_defval[k] == "true") {
      return true;
    } else {
      return false;
    }
  }
}

void Option::dump_help() {

  // output head of the help
  cout << head;

  // output must have options
  cout << "    "
       << "[Necessary]" << endl;

  for (map<string, string>::iterator iter = key_comment.begin();
       iter != key_comment.end(); iter++) {
    if (key_musthave.find(iter->first) !=
        key_musthave.end()) // a "must have" option
    {
      cout << '\t' << iter->first << " : " << iter->second << endl;
      cout << "\t  type : " << key_type[iter->first]
           << "\t|\tdefault : none, must specify" << endl;
    }
  }

  // output optinal options
  cout << "    "
       << "[Optional]" << endl;

  for (map<string, string>::iterator iter = key_comment.begin();
       iter != key_comment.end(); iter++) {
    if (key_musthave.find(iter->first) ==
        key_musthave.end()) // not a "must have" option
    {
      cout << '\t' << iter->first << " : " << iter->second << endl;
      cout << "\t  type : " << key_type[iter->first]
           << "\t|\tdefault : " << key_defval[iter->first];
      set<string> dm = key_domain[iter->first];
      if (dm.size() == 0) {
        cout << endl;
      } else {
        cout << "\t|\tdomain : {";
        set<string>::iterator iter1 = dm.begin();
        cout << ' ' << (*iter1);
        iter1++;
        for (; iter1 != dm.end(); iter1++) {
          cout << ", " << (*iter1);
        }
        cout << " }" << endl;
      }
    }
  }

  // output the tail of the help info
  cout << tail;
}

Option::PARSE_RETURN_TYPE Option::parse(int argc, char **argv) {
  string hs("--help");
  set<string> kmh = key_musthave;
  for (int i = 1; i < argc; i++) {
    string s(argv[i]);
    kmh.erase(s);
    if (s == hs) // if it is "--help"
    {
      dump_help();
      return PARSE_HELP;
    }
    if (key_type.find(s) != key_type.end()) // if it is registered
    {
      if (key_type[s] == "bool") // if it is a bool type
      {
        key_value[s] = "true";
      } else // it needs a value
      {
        if (i < argc - 1) // if it is not the last term
        {
          // test if the value is in domain
          if (key_domain.find(s) != key_domain.end()) {
            if (key_domain[s].find(argv[i + 1]) == key_domain[s].end()) {
              LOG_ERROR("value of option \"", argv[i],
                        "\" is not in the specified domain, use --help to see "
                        "more info");
              return PARSE_ERROR;
            }
          }
          key_value[s] = argv[i + 1];
          i++;
        } else // it is the last term
        {
          LOG_ERROR("option \"", argv[i], "\" need a ", key_type[s], " value");
          return PARSE_ERROR;
        }
      }
    } else // option not registered
    {
      LOG_ERROR("unkown options: \"", argv[i],
                "\", use --help to see more info");
      return PARSE_ERROR;
    }
  }
  if (!kmh.empty()) {
    for (set<string>::iterator iter = kmh.begin(); iter != kmh.end(); iter++) {
      LOG_ERROR("option \"", *iter, "\" must be used");
    }
    return PARSE_ERROR;
  }
  return PARSE_CORRECT;
}
}
