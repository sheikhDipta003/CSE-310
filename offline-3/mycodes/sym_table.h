#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <algorithm>
#include <regex>
using namespace std;

//SymbolInfo start
class SymbolInfo {
	string name;
	string type;
public:
    string var_type;
    vector<string>param_v;
    bool isFunctionDeclaration;
    bool isFunction;

    SymbolInfo* next;

    SymbolInfo(){ 
        this->name = ""; this-> type = ""; this->next = nullptr;
        isFunctionDeclaration = false;
        isFunction = false;
    }
    SymbolInfo(SymbolInfo* nextP){
        this->next = nextP;
    }
	SymbolInfo(   string name,    string type = "", SymbolInfo* nextP = nullptr){
        this->name = name;
        this->type = type;
        this->next = nextP;
    }
	SymbolInfo(const SymbolInfo&  ob){  //copy constructor
        this->name = ob.name;
        this->type = ob.type;
        this->next = ob.next;
    }
    SymbolInfo(string name,string type,string var_type,vector<string>param_v,bool isFunctionDeclaration,bool isFunction, SymbolInfo* nextP = NULL)
    {
        this->name = name;
        this->type = type;
        this->var_type = var_type;
        this->param_v = param_v;
        this->isFunctionDeclaration = isFunctionDeclaration;
        this->isFunction = isFunction;
        this->next = nextP;
    }
	string getName(){
        return name;
    }
	void setName(   string name){
        this->name = name;
    }
	string getType(){
        return type;
    }
	void setType(   string type){
        this->type = type;
    }

	SymbolInfo& operator=(  SymbolInfo&  another){
        this->name = another.name;
        this->type = another.type;

        return *this;
    }

	bool operator==(SymbolInfo& another){
        if(another.type.empty())  return (name == another.name);
        return (name == another.name && type == another.type);
    }

	bool operator!=(SymbolInfo&  another){
        return !(another == *this);
    }
    
    long int sdbm_hash() {
    	long int hash = 0;
    	long int i = 0;
    	long int len = this->name.length();
    
    	for (i = 0; i < len; i++)
    	{
    		hash = (this->name[i]) + (hash << 6) + (hash << 16) - hash;
    	}
    
    	return hash;
    }

    void setVarType(string var_type){
        this->var_type = var_type;
    }
};
//SymbolInfo end

//linkedlist start
class LList {
private:
    SymbolInfo* head;
    SymbolInfo* tail;
    SymbolInfo* curr;
    int cnt;
public:
    LList() { 
        curr = tail = head = new SymbolInfo;
        cnt = 0;
    }
    ~LList() { 
        while(head != nullptr) {
            curr = head;
            head = head->next;
            delete curr;
        }
    }
    bool insert(  SymbolInfo&  it) {
        if(this->search(it) == nullptr){    //if the object already doesn't exist in this list
            it.next = curr->next;
            curr->next = &it;
            if (tail == curr) tail = curr->next;
            cnt++;
            return true;
        }
        return false;
    }
    bool append(  SymbolInfo& it) { 
        if(this->search(it) == nullptr){    //if the object already doesn't exist in this list
            tail = tail->next = new SymbolInfo(it);
            cnt++;
            return true;
        }

        return false;
    }
    
    SymbolInfo remove_help() {
        if(curr->next == nullptr)   return SymbolInfo("");
        SymbolInfo it = *curr->next; 

        SymbolInfo* temp = curr->next;
        if (tail == curr->next) tail = curr; 
        curr->next = curr->next->next;

        delete temp;
        cnt--; 

        return it;
    }

    bool remove(SymbolInfo& it){
        int pos = this->getPos(it) - 1;

        if(pos >= 0){    //if the object already doesn't exist in this list
            this->moveToPos(pos);
            this->remove_help();
            return true;
        }

        return false;
    }

    SymbolInfo* search(  SymbolInfo& it){     //returns the pointer to the element if found, nullptr otherwise
        SymbolInfo* temp = curr;
        moveToStart();
        for(int i = 0; i < cnt; i++){
            curr = curr->next;
            if(*curr == it) return curr;
        }
        curr = temp;
        return nullptr;
    }

    int getPos(  SymbolInfo&  it){    //returns the position of the element in the list if found, -1 if not found
        SymbolInfo* temp = head;
        for(int i = 0; i < cnt; i++){
            temp = temp->next;
            if(*temp == it) return (i + 1);     //pos in the list starts from 1
        }
        return -1;
    }

    void moveToStart(){ curr = head; }
    void moveToEnd(){ curr = tail; }
    
    void prev() {
        if (curr == head) return;
        SymbolInfo* temp = head;
        while (temp->next != curr) temp = temp->next;
        curr = temp;
    }

    void next(){ if (curr != tail) curr = curr->next; }

    int length() { return cnt; }
    
    int currPos() {
        SymbolInfo* temp = head;
        int i;
        for (i=0; curr != temp; i++)
        temp = temp->next;

        return i;
    }
    
    void moveToPos(int pos) {
        if( (pos < 0) || (pos > cnt) )    return;
        curr = head;
        for(int i = 0; i < pos; i++) curr = curr->next;
    }

    SymbolInfo getValue() {
        if(curr->next == nullptr) return SymbolInfo("");
        return *curr->next;
    }

    void printLL(){
        SymbolInfo* temp = curr;
        moveToStart();
        for(int i = 0; i < cnt; i++){
            curr = curr->next;
            cout << "1: " << "<" << getValue().getName() << "," << getValue().getType() << ">" << endl;
        }
        curr = temp;
    }
};
//linkedlist end

//hashtable start
template <typename T>
class HashTable
{
private:
	LList* arr;
	int size;
	int countItem;
	int hash(T& ob){return (int) (ob.sdbm_hash() % size);}
public:
	HashTable(int num_bkt = 10){
        size = num_bkt;
        arr = new LList[size];
        countItem = 0;
    }
	virtual ~HashTable(){ delete[] arr; }

	int getItemCount(){ return countItem; }
	int getMaxSize(){return size;}
    
	bool insert(T& ob){
		int i = hash(ob);
		bool result = arr[i].append(ob);
		if (result) countItem++;

		return result;
	}
	bool remove(T& ob){
		int i = hash(ob);
		bool result = arr[i].remove(ob);
		if(result) countItem--;
		
		return result;
	}

	T* search(T& ob){
        int i = hash(ob);
	    return arr[i].search(ob);
    }

	int getHashIndex(T& ob){
        if(arr[hash(ob)].getPos(ob) != -1) return (hash(ob) + 1);   //bucket index starts from 1
        return -1;
    } //index of the bucket the SymbolInfo object has been hashed to

	int getLLPos(T& ob){return arr[hash(ob)].getPos(ob);} //position in the linked list of the bucket

	void printHashTable(FILE* fp);
};

template<typename T>
void HashTable<T>::printHashTable(FILE* fp) {    
	for (int i = 0; i < size; i++) {
        if(arr[i].length() > 0){
            fprintf(fp, "\t%d--> ", (i+1));

            for (int j = 0; j < arr[i].length(); j++) {
                arr[i].moveToPos(j);
                fprintf(fp, "<%s,%s> ", arr[i].getValue().getName().data(), arr[i].getValue().getType().data());
            }
            fprintf(fp, "\n");
        }
	}
}
//hashtable end


//ScopeTable class start
class ScopeTable : public HashTable<SymbolInfo> {
	ScopeTable* parent_scope;
	int count;
public:
	ScopeTable(ScopeTable* ps = nullptr){
        this->parent_scope = ps;
        if (!parent_scope) count = 1;
        else count = parent_scope->count + 1;
    }
	ScopeTable(ScopeTable *ps, int count){
        this->parent_scope = ps;
        this->count = count;
    }
	ScopeTable(ScopeTable *ps, int count, int size) : HashTable(size), parent_scope(ps), count(count) {}
	~ScopeTable(){
        count = -1;
	    parent_scope = nullptr;
    }
	ScopeTable* getParentScope(){
        return parent_scope;
    }
	void setParentScope(ScopeTable* parent_scope){
        this->parent_scope = parent_scope;
    }
	int getCount(){return this->count;}
	void setCount(int count){this->count = count;}

	bool _insert(  SymbolInfo&  sym){return insert(sym);}

	SymbolInfo* _search(  SymbolInfo&  sym){return search(sym);}

	bool _delete(  SymbolInfo&  sym){return remove(sym);}

	void print(FILE* fp){
        fprintf(fp, "\tScopeTable# %d\n", this->count);
        printHashTable(fp);
    }
};
//ScopeTable End

//SymbolTable start
class SymbolTable {
	ScopeTable *curr_scope;
	int max_count;
	int num_bkt;
    FILE* fp;
public:
	SymbolTable(FILE* fp, int num_bkt = 10){
        max_count = 1;
        curr_scope = nullptr;
        this->fp = fp;
        this->num_bkt = num_bkt;

        curr_scope = new ScopeTable(nullptr, 1, num_bkt);
    }
	virtual ~SymbolTable(){
        while (curr_scope) {
            ScopeTable *p = curr_scope->getParentScope();
            delete curr_scope;
            curr_scope = p;
        }
        num_bkt = 0;
    }

	void enterScope(){
        ScopeTable *p = new ScopeTable(curr_scope, max_count + 1, num_bkt);
        curr_scope = p;
        max_count++;
    }
	void exitScope(){
        if (curr_scope) {
            if(curr_scope->getCount() > 1){
                ScopeTable *p = curr_scope->getParentScope();
                delete curr_scope;
                curr_scope = p;
            }
	    }
    }
	bool __insert(SymbolInfo sym){
        if (curr_scope == nullptr) return false;
        if (curr_scope->_insert(sym)) return true;
        fprintf(fp, "\t%s already exists in the current ScopeTable\n", sym.getName().data());
        return false;
    }

    bool __insert(   string sym_name,    string sym_type){
        SymbolInfo sym(sym_name, sym_type);
        return __insert(sym);
    }

	bool __remove(   string sym_name){
        if (curr_scope == nullptr) return false;

        ScopeTable *p = curr_scope;
        SymbolInfo sym(sym_name);

        int hashPos = p->getHashIndex(sym);
        int listPos = p->getLLPos(sym);

        if (p  && p->_delete(sym)) return true;

        return false;
    }

	SymbolInfo* __search(   string sym_name){
        if (curr_scope == nullptr) return nullptr;

        ScopeTable *p = curr_scope;
        SymbolInfo sym(sym_name);

        while (p) {
            int h_i = p->getHashIndex(sym);
            int L_i = p->getLLPos(sym);

            if (h_i != -1 && L_i != -1) {
                return p->_search(sym);
            } 
            else p = p->getParentScope();
        }

        return nullptr;
    }

    void __print(string printType){  //"A" or "C"

        if(printType == "C"){
            curr_scope->print(fp);
        }
        else if(printType == "A"){
            for (ScopeTable *p = curr_scope; p; p = p->getParentScope()) {
                p->print(fp);
            }
        }

    }

    void quit(){
        while (curr_scope) {
            ScopeTable *p = curr_scope->getParentScope();
            delete curr_scope;
            curr_scope = p;
        }
        num_bkt = 0;
    }

};
//SymbolTable end
