#include "b_tree.hpp"
#include <iostream>
using namespace std;

void search(page *p, record *r){ 
  short cont = 1;

  if(p == NULL){
    cout<<"\nNenhum cliente encontrado\n\n";
    return;
  }

  while (cont < p->n && r->CPF > p->r[cont-1].CPF){
  	cont++;
  } 

  if (r->CPF == p->r[cont-1].CPF){ 
    *r = p->r[cont-1];
    cout<<"\nCliente encontrado: "<<r->name<<"\nIdade: "<<r->age<<"\n\n";
    return;
  }

  if (r->CPF < p->r[cont-1].CPF){
  	search(p->p[cont-1], r);
  }
  else {
  	search(p->p[cont], r);
  }
} 

void insertOnPage(page *p, page *pdir, record r){ 
  bool inPosition;
  int k;  
  k = p->n;  
  
  if (k > 0){
  	inPosition = true;
  }
  else{
  	inPosition = false;
  }
  
  while (inPosition){   
    if (r.CPF >= p->r[k-1].CPF){ 
      inPosition = false;
      break;
    }
    
    p->r[k] = p->r[k-1];
    p->p[k+1] = p->p[k];
    k--;
      
    if (k < 1){
    	inPosition = false;
	}      
  }  
  p->r[k] = r; 
  p->p[k+1] = pdir;
  p->n++;
} 

void InsertFixUp(page **p, page **pr, record r, record *rr, bool *overflow){ 
  long i=1, j; 
  page *paux;

  if (*p == NULL){ 
    *overflow = true; 
    *rr = r; 
    *pr = NULL;
    return;
  }

  while (i < (*p)->n && r.CPF > (*p)->r[i-1].CPF){
  	i++;
  }  

  if (r.CPF == (*p)->r[i-1].CPF){ 
    printf("\n[ERRO]: Cliente ja cadastrado\n\n"); 
    *overflow = false;
    return;
  }

  if (r.CPF < (*p)->r[i-1].CPF){
  	i--;
  }  
  InsertFixUp(&(*p)->p[i], pr, r, rr, overflow);
  
  if (!*overflow){
  	return;
  } 

  if ((*p)->n < MM){
      insertOnPage(*p, *pr, *rr);
      *overflow = false;
      return;
  }
  
  paux = (page*)malloc(sizeof(page));
  paux->n = 0;  
  paux->p[0] = NULL;

  if (i < M + 1){ 
    insertOnPage(paux, (*p)->p[MM], (*p)->r[MM-1]);
    (*p)->n--;
    insertOnPage(*p, *pr, *rr);
  }else 
    insertOnPage(paux, *pr, *rr);
  
  for (j=M+2; j<=MM; j++) 
    insertOnPage(paux, (*p)->p[j], (*p)->r[j-1]);

  (*p)->n = M;  
  paux->p[0] = (*p)->p[M+1];
  *rr = (*p)->r[M];  
  *pr = paux;
}

void insert(page **p, record r){ 
  bool overflow;
  record rr;
  page *pr, *paux;
  InsertFixUp(p, &pr, r, &rr, &overflow);

  if (overflow){
    paux = (page*) malloc(sizeof(page));
    paux->n = 1;  
    paux->r[0] = rr;
    paux->p[0] = *p;  
    paux->p[1] = pr;
    *p = paux;
  }
}

void removeFixUp(page *p, page *parent, int Posparent, bool *underflow){ 
  page *aux; 
  long disp;
  
  if (Posparent < parent->n){ 
    aux = parent->p[Posparent + 1];  
    disp = (aux->n - M + 1) / 2;
    p->r[p->n] = parent->r[Posparent];
    p->p[p->n + 1] = aux->p[0];  
    p->n++;
    
    if (disp > 0){ 
      for (int j = 1; j < disp; j++){
      	insertOnPage(p, aux->p[j], aux->r[j-1]);
	  }  
      parent->r[Posparent] = aux->r[disp-1];  
      aux->n -= disp;
      
      for (int j = 0; j < aux->n; j++){
      	aux->r[j] = aux->r[j + disp];
	  }
        
      for (int j = 0; j <= aux->n; j++){
      	aux->p[j] = aux->p[j + disp];
	  }    
      *underflow = false;   
    }
	else{ 
      
      for (int j = 1; j <= M; j++){
      	insertOnPage(p, aux->p[j], aux->r[j-1]);
	  }
        
      free(aux);
        
      for (int j = Posparent + 1; j < parent->n; j++){ 
        parent->r[j-1] = parent->r[j];
        parent->p[j] = parent->p[j+1];
      }
      
      parent->n--;
      if (parent->n >= M){
      	*underflow = false;
	  }        
    }
  }
  else{
    aux = parent->p[Posparent-1]; 
    disp = (aux->n - M + 1) / 2;
    
    for (int j = p->n; j >= 1; j--){
    	 p->r[j] = p->r[j-1];
	}    
    p->r[0] = parent->r[Posparent-1];
    
    for (int j = p->n; j >= 0; j--){
    	p->p[j+1] = p->p[j];
	}  
    p->n++;
    if (disp > 0){ 
      for (int j = 1; j < disp; j++){
      	insertOnPage(p, aux->p[aux->n - j + 1], aux->r[aux->n - j]);
	  }         
      p->p[0] = aux->p[aux->n - disp + 1];
      parent->r[Posparent-1] = aux->r[aux->n - disp];
      aux->n -= disp;  
      *underflow = false;
    }
	else{
      
      for (int j = 1; j <= M; j++){
      	insertOnPage(aux, p->p[j], p->r[j-1]);
	  }    
      free(p);  
      parent->n--;
      
      if (parent->n >= M) {
      	*underflow = false;
	  }     
    }
  }
}

void  predecessor(page *p, int i, page *parent, bool *underflow){ 

  if (parent->p[parent->n] != NULL){ 
     predecessor(p, i, parent->p[parent->n], underflow);
    if (*underflow){
    	removeFixUp(parent->p[parent->n], parent, parent->n, underflow);
	}   
    return;
  } 
  p->r[i-1] = parent->r[parent->n - 1]; 
  parent->n--;  
  
  if (parent->n < M){
  	*underflow = true;
  }
  else{
  	*underflow = false;
  }
}

void remove(page **p, long long CPF, bool *underflow){ 
  long j, i = 1;
  page *aux;

  if (*p == NULL){ 
    printf("\n[ERRO]: Cliente nao encontrado.\n\n"); 
    *underflow = false;
    return;
  }

  aux = *p;
  while (i < aux->n && CPF > aux->r[i-1].CPF){
  	i++;
  } 

  if (CPF == aux->r[i-1].CPF){ 
    if (aux->p[i-1] == NULL){ 
      aux->n--;
      *underflow = (aux->n < M);
      for (j = i; j <= aux->n; j++){ 
        aux->r[j-1] = aux->r[j];  
        aux->p[j] = aux->p[j+1]; 
      }
      return;
    }

    predecessor(*p, i, aux->p[i-1], underflow);
    if (*underflow){
    	removeFixUp(aux->p[i-1], *p, i - 1, underflow);
	}    
    return;
  }
  if (CPF > aux->r[i-1].CPF){
  	i++;
  } 
  remove(&aux->p[i-1], CPF, underflow);
  
  if (*underflow){
  	removeFixUp(aux->p[i-1], *p, i - 1, underflow);
  }    
} 

void takeOut(page **p, long long CPF){ 
  bool underflow;
  page *aux;
  remove(p, CPF, &underflow);
  
  if (underflow && (*p)->n == 0){
    aux = *p;   
    *p = aux->p[0]; 
    free(aux);
  }
}    
