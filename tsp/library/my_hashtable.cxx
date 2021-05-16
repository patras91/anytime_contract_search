
class myhashtable {
 public:
  myhashtable() {
  }
  
  ~myhashtable() {
  }
  
  void deleteAllStates() {
	map<string, List*>::iterator t;
	for (t = mht.begin(); t != mht.end(); t++) {
	  List* h = (List*)(t->second);
	  h->deleteAllStates();
	  delete h;
	}
	mht.clear();
  }
  
  void deleteAllEntries() {
	map<string, List*>::iterator t;
	for (t = mht.begin(); t != mht.end(); t++) {
	  List* h = (List*)(t->second);
	  h->clear();
	  delete h;
	}
	mht.clear();
  }
  
  void resetAllEntries() {
	map<string, List*>::iterator t;
	for (t = mht.begin(); t != mht.end(); t++) {
	  List* h = (List*)(t->second);
	  h->clear();
	}
  }
  
  List* getList(TspState* s) {
	map<string, List*>::iterator t;
	if((t = mht.find(s->GetHash())) == mht.end()) {
	  return NULL;
    }
    return (List*)(t->second);
  }
  
  void pop(TspState* s) {
	getList(s)->pop(s);
  }
  
  void push(TspState* s) {
	char* hash = s->GetHash();
	map<string, List*>::iterator t;
	if((t = mht.find(hash)) == mht.end()) {
	  List* h = new List();
	  h->push(s);
	  mht[hash] = h;
    } else {
	  ((List*)(t->second))->push(s);
	}
  }
  
 private:
  map <string, List*> mht;
};
