#ifndef BTREE_H 
#define BTREE_H

#include <iostream>

#define M 2
#define MM (M * 2)

struct record {
  long long CPF;
  char name[200];
  char age[3];
};

struct page {
  short n;
  record r[MM];
  page *p[MM + 1];
};

void search(page *p, record *r);
void insertOnPage(page *p, page *pdir, record r);
void InsertFixUp(page **p, page **pr, record r, record *rr, bool *overflow);
void insert(page **p, record r);

void removeFixUp(page *p, page *parent, int Posparent, bool *underflow);
void  predecessor(page *p, int i, page *parent, bool *underflow);
void remove(page **p, long long CPF, bool *underflow);

#endif

