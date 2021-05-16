
class HHashtable {
 public:
  HHashtable() {
	num_tables = _size * _size;
	hht = new myhashtable*[num_tables];
	for (int i = 0; i < num_tables; i++) {
	  hht[i] = new myhashtable();
	}
  }
  
  ~HHashtable() {
	for (int i = 0; i < num_tables; i++) {
	  delete hht[i];
	}
	delete hht;
  }
  
  void deleteAllStates() {
	for (int i = 0; i < num_tables; i++) {
	  hht[i]->deleteAllStates();
	}
  }
  
  void deleteAllEntries() {
	for (int i = 0; i < num_tables; i++) {
	  hht[i]->deleteAllEntries();
	}
  }
  
  void resetAllEntries() {
	for (int i = 0; i < num_tables; i++) {
	  hht[i]->resetAllEntries();
	}
  }
  
  List* getList(PuzState* s) {
	return hht[s->GetHKey()]->getList(s);
  }
  
  void pop(PuzState* s) {
	hht[s->GetHKey()]->getList(s)->pop(s);
  }
  
  void push(PuzState* s) {
	hht[s->GetHKey()]->push(s);
  }
  
 private:
  myhashtable** hht;
  int num_tables;
};
