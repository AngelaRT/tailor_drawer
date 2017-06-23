using namespace std;
class HashEntry {

private:
      int key;
      string* value;

public:
      HashEntry(int key, string[] value) {
            this->key = key;
            this->value = value;
      } 

      int getKey() {
            return key;
      } 

      string[] getValue() {
            return value;
      }
};

const int TABLE_SIZE = 128; 

class HashMap {

private:
      HashEntry **table;

public:
      HashMap() {
            table = new HashEntry*[TABLE_SIZE];
            for (int i = 0; i < TABLE_SIZE; i++)
                  table[i] = NULL;
      } 

      string[] get(int key) {
            int hash = (key % TABLE_SIZE);
            while (table[hash] != NULL && table[hash]->getKey() != key)
                  hash = (hash + 1) % TABLE_SIZE;
            if (table[hash] == NULL)
                  return -1;
            else
                  return table[hash]->getValue();

      } 

      void put(int key, string[] value) {
            int hash = (key % TABLE_SIZE);
            while (table[hash] != NULL && table[hash]->getKey() != key)
                  hash = (hash + 1) % TABLE_SIZE;
            if (table[hash] != NULL)
                  delete table[hash];
            table[hash] = new HashEntry(key, value);
      }     

      ~HashMap() {
            for (int i = 0; i < TABLE_SIZE; i++)
                  if (table[i] != NULL)
                        delete table[i];
            delete[] table;
      }

};