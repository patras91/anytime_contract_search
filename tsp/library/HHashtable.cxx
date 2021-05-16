
class HHashtable {
 public:
  HHashtable() {
	hht = new myhashtable*[nodes];
	for (int i = 0; i < nodes; i++) {
	  hht[i] = new myhashtable();
	}
  }
  
  ~HHashtable() {
	for (int i = 0; i < nodes; i++) {
	  delete hht[i];
	}
	delete hht;
  }
  
  void deleteAllStates() {
	for (int i = 0; i < nodes; i++) {
	  hht[i]->deleteAllStates();
	}
  }
  
  void deleteAllEntries() {
	for (int i = 0; i < nodes; i++) {
	  hht[i]->deleteAllEntries();
	}
  }
  
  void resetAllEntries() {
	for (int i = 0; i < nodes; i++) {
	  hht[i]->resetAllEntries();
	}
  }
  
  List* getList(TspState* s) {
	return hht[s->GetDepth()]->getList(s);
  }
  
  void pop(TspState* s) {
	hht[s->GetDepth()]->getList(s)->pop(s);
  }
  
  void push(TspState* s) {
	hht[s->GetDepth()]->push(s);
  }
  
 private:
  myhashtable** hht;
};
