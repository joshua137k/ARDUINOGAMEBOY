#include "Dictionary.h"

Dictionary::Dictionary(int maxItems) {
  this->maxItems = maxItems;
  this->items = new DictionaryItem[maxItems];
  numItems = 0;
}

Dictionary::~Dictionary() {
  delete[] items;
}

void Dictionary::addItem(const char* key, int value) {
  if (numItems < maxItems) {
    items[numItems].key = key;
    items[numItems].type = INT;
    items[numItems].value.intValue = value;
    numItems++;
  }
}

void Dictionary::addItem(const char* key, float value) {
  if (numItems < maxItems) {
    items[numItems].key = key;
    items[numItems].type = FLOAT;
    items[numItems].value.floatValue = value;
    numItems++;
  }
}

void Dictionary::addItem(const char* key, const char* value) {
  if (numItems < maxItems) {
    items[numItems].key = key;
    items[numItems].type = STRING;
    items[numItems].value.stringValue = value;
    numItems++;
  }
}

void Dictionary::addItem(const char* key, int* value) {
  if (numItems < maxItems) {
    items[numItems].key = key;
    items[numItems].type = INT_PTR;
    items[numItems].value.intPtrValue = value;
    numItems++;
  }
}

void Dictionary::addItem(const char* key, int** value) {
  if (numItems < maxItems) {
    items[numItems].key = key;
    items[numItems].type = INT_PTR_PTR;
    items[numItems].value.intPtrPtrValue = value;
    numItems++;
  }
}

String Dictionary::valueToString(Value value, ValueType type) {
  switch(type) {
    case INT:
      return String(value.intValue);
    case FLOAT:
      return String(value.floatValue);
    case STRING:
      return String(value.stringValue);
    case INT_PTR:
      return String(*value.intPtrValue);
    case INT_PTR_PTR:
      return String(**value.intPtrPtrValue);
    default:
      return String("Unknown Type");
  }
}

Value Dictionary::getValue(const char* key, ValueType &type) {
  for (int i = 0; i < numItems; i++) {
    if (strcmp(items[i].key, key) == 0) {
      type = items[i].type;
      return items[i].value;
    }
  }
  Value emptyValue;
  emptyValue.intValue = -1;
  type = INT; 
  return emptyValue;
}

bool Dictionary::exists(const char* key) {
  for (int i = 0; i < numItems; i++) {
    if (strcmp(items[i].key, key) == 0) {
      return true;
    }
  }
  return false;
}

void Dictionary::change(const char* key, Value newValue, ValueType newType) {
  for (int i = 0; i < numItems; i++) {
    if (strcmp(items[i].key, key) == 0) {
      items[i].value = newValue;
      items[i].type = newType;
      return;
    }
  }
  // If the key does not exist, add a new item
  if (numItems < maxItems) {
    items[numItems].key = key;
    items[numItems].value = newValue;
    items[numItems].type = newType;
    numItems++;
  }
}
