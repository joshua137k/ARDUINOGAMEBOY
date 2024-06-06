// Dictionary.h
#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <Arduino.h>

#define MAX_ITEMS 100

enum ValueType {
  INT,
  FLOAT,
  STRING,
  INT_PTR,
  INT_PTR_PTR
};

union Value {
  int intValue;
  float floatValue;
  const char* stringValue;
  int* intPtrValue;
  int** intPtrPtrValue;
};

struct DictionaryItem {
  const char* key;
  ValueType type;
  Value value;
};

class Dictionary {
  private:
    DictionaryItem items[MAX_ITEMS];
    int numItems;

  public:
    Dictionary();

    void addItem(const char* key, int value);
    void addItem(const char* key, float value);
    void addItem(const char* key, const char* value);
    void addItem(const char* key, int* value);
    void addItem(const char* key, int** value);
    Value getValue(const char* key);
};

#endif
