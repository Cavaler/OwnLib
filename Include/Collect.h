// Collect.h

#ifndef __COLLECT_H
#define __COLLECT_H

class CCollection {
public:
	CCollection();
	virtual void *At(int I);
	virtual void Insert(void *Item);
	virtual void AtInsert(int I,void *Item);
	virtual void AtDelete(int I);
	virtual void AtFree(int I);
	virtual void DeleteAll();
	virtual void FreeAll();
	virtual void FreeItem(void *Item);
	virtual int  IndexOf(void *Item);
	virtual int  Count() {return Cnt;};
	virtual ~CCollection();
	int FreeOnDestroy;
protected:
	void **Items;
	int Cnt;
};

class CBlockCollection:public CCollection {
public:
	virtual void FreeItem(void *Item);
};

class CSortedCollection:public CCollection {
public:
	virtual void *KeyOf(void *Item) {return Item;};
	virtual int  Compare(void *Key1,void *Key2) {return 0;};
	virtual void Insert(void *Item);
	virtual int  IndexOf(void *Item);
};

class CSortedBlockCollection:public CSortedCollection {
public:
	virtual void FreeItem(void *Item);
};

class CStringCollection:public CSortedBlockCollection {
	virtual int  Compare(void *Key1,void *Key2);
};

#endif
