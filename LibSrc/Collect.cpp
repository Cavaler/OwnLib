#include "Collect.h"
#include <stdlib.h>
#include <string.h>

CCollection::CCollection():Cnt(0),FreeOnDestroy(1),Items(new void *[0]) {
}
 
void *CCollection::At(int I) {
	if (I<Cnt) return Items[I]; else return NULL;
}

void CCollection::Insert(void *Item) {
	void **New=new void *[Cnt+1];
	memmove(New,Items,Cnt*sizeof(void *));
	delete[] Items;Items=New;
	Items[Cnt++]=Item;
}

void CCollection::AtInsert(int I,void *Item) {
	if (I>Cnt) I=Cnt;
	void **New=new void *[Cnt+1];
	memmove(New,Items,I*sizeof(void *));
	memmove(&New[I+1],&Items[I],(Cnt-I)*sizeof(void *));
	delete[] Items;Items=New;
	Items[I]=Item;Cnt++;
}

void CCollection::AtDelete(int I) {
	if (I>=Cnt) return;
	void **New=new void *[Cnt-1];
	memmove(New,Items,I*sizeof(void *));
	memmove(&New[I],&Items[I+1],(Cnt-I-1)*sizeof(void *));
	delete[] Items;Items=New;Cnt--;
}

void CCollection::AtFree(int I) {
	if (I>=Cnt) return;
	FreeItem(Items[I]);AtDelete(I);
}

void CCollection::DeleteAll() {
	delete[] Items;Items=new void *[0];
}

void CCollection::FreeAll() {
	for (int I=0;I<Cnt;I++) FreeItem(Items[I]);
	DeleteAll();
}

void CCollection::FreeItem(void *Item) {
	delete Item;
}

int CCollection::IndexOf(void *Item) {
	for (int I=0;I<Cnt;I++) if (Items[I]==Item) return I;
	return -1;
}

CCollection::~CCollection() {
	(FreeOnDestroy)?FreeAll():DeleteAll();
	delete[] Items;
}

void CBlockCollection::FreeItem(void *Item) {
	free(Item);
}

void CSortedCollection::Insert(void *Item) {
	AtInsert(IndexOf(Item),Item);
}

int CSortedCollection::IndexOf(void *Item) {
	for (int I=0;I<Cnt;I++) if (Compare(Item,Items[I])<0) return I;
	return Cnt;
}

void CSortedBlockCollection::FreeItem(void *Item) {
	free(Item);
}

int CStringCollection::Compare(void *Key1,void *Key2) {
	return strcmp((char *)Key1,(char *)Key2);
};
