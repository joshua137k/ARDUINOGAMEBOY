// Dictionary.cpp
#include "Dictionary.h"

Dictionary::Dictionary() {
  numItems = 0;
}

void Dictionary::addItem(const char* key, int value) {
  if (numItems < MAX_ITEMS) {
    items[numItems].key = key;
    items[numItems].type = INT;
    items[numItems].value.intValue = value;
    numItems++;
  }
}

void Dictionary::addItem(const char* key, float value) {
  if (numItems < MAX_ITEMS) {
    items[numItems].key = key;
    items[numItems].type = FLOAT;
    items[numItems].value.floatValue = value;
    numItems++;
  }
}

void Dictionary::addItem(const char* key, const char* value) {
  if (numItems < MAX_ITEMS) {
    items[numItems].key = key;
    items[numItems].type = STRING;
    items[numItems].value.stringValue = value;
    numItems++;
  }
}

void Dictionary::addItem(const char* key, int* value) {
  if (numItems < MAX_ITEMS) {
    items[numItems].key = key;
    items[numItems].type = INT_PTR;
    items[numItems].value.intPtrValue = value;
    numItems++;
  }
}

void Dictionary::addItem(const char* key, int** value) {
  if (numItems < MAX_ITEMS) {
    items[numItems].key = key;
    items[numItems].type = INT_PTR_PTR;
    items[numItems].value.intPtrPtrValue = value;
    numItems++;
  }
}

Value Dictionary::getValue(const char* key) {
  for (int i = 0; i < numItems; i++) {
    if (items[i].key == key) {
      return items[i].value;
    }
  }
  Value emptyValue;
  emptyValue.intValue = -1; 
  return emptyValue;
}