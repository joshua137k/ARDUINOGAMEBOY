#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <Arduino.h>

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
    DictionaryItem* items;
    int numItems;
    int maxItems;

  public:
    Dictionary(int maxItems = 100);
    ~Dictionary();

    void addItem(const char* key, int value);
    void addItem(const char* key, float value);
    void addItem(const char* key, const char* value);
    void addItem(const char* key, int* value);
    void addItem(const char* key, int** value);
    Value getValue(const char* key, ValueType &type);
    String valueToString(Value value, ValueType type);
    bool exists(const char* key);
    void change(const char* key, Value newValue, ValueType newType);
};

#endif
