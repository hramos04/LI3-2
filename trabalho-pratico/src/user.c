#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "user.h"


int Hash(KeyType k) {
    int i = 0;
    unsigned h = 0;
    while (k[i] != '\0') {
        h += k[i];
        h += (h << 10);
        h ^= (h >> 6);
        i++;
    }
    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);
    return h % HASHSIZE;
}

// Função para inicializar a tabela hash
void InitializeTable(hash_user h) {
    int i;
    for (i = 0; i < HASHSIZE; ++i)
        h[i] = NULL;
}

User *RetrieveUser(hash_user h, KeyType k) {
	 int i = Hash(k);
	 User *res;
	 for(res = h[i]; res; res = res->next) {
		 if(strcmp(res->id, k) == 0) {
			 return res;
		 }
	 }
	 return NULL;
}

User* copyUser(User *original) {
    User *copy = malloc(sizeof(User));
    copy->id = strdup(original->id);
    copy->nome = strdup(original->nome);
    copy->next = NULL;
    return copy;
}

/*
int compareNamesWithoutHyphenIgnoreCase(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        while (*str1 == '-' && *str2 == '-') {
            str1++;
            str2++;
        }
		if (isdigit(*str1) && isdigit(*str2)) {
            while (isdigit(*str1) && isdigit(*str2)) {
                if (*str1 != *str2) {
                    return (*str1 - *str2);
                } else{
                str1++;
                str2++;
            	}
			}
		} else{
        	if (tolower(*str1) != tolower(*str2)) {
            	return (tolower(*str1) - tolower(*str2));
        	} else {
            str1++;
            str2++;
       		}
		}
    }

    return (*str1 - *str2);
}*/

int compareNamesWithoutHyphenIgnoreCase(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        while (*str1 == '-' && *str2 == '-') {
            str1++;
            str2++;
        }

        int result = strcoll(str1, str2);

        if (result != 0) {
            return result;
        } else {
            str1 += strlen(str1); 
            str2 += strlen(str2);  
        }
    }
    return strcoll(str1, str2);
}

void addUserToList(User **list, User *newUser) {
    while (*list != NULL) {
        int compare = strcoll(newUser->nome, (*list)->nome);
        if (compare < 0  || (compare == 0 && atoi(newUser->id) < atoi((*list)->id))) {
            newUser->next = *list;
            *list = newUser;
            return;
        }
        list = &(*list)->next;
    }
    *list = newUser;
}


User *GetUserPrefix(hash_user h, KeyType k) {
	User *res = NULL;
    for (int i = 0; i < HASHSIZE; i++) {
        User *currentUser = h[i];
        while (currentUser != NULL) {
            if (strcasecmp(currentUser->account_status, "active") == 0 && strncmp(currentUser->nome, k, strlen(k)) == 0) {
				User *copy = copyUser(currentUser);
				addUserToList(&res, copy);
            }
            currentUser = currentUser->next;
        }
    }
    return res;
}

// Função para inserir na tabela hash usando encadeamento separado em caso de colisão
void InsertTable(hash_user h, KeyType k, User *user) {
    int i = Hash(k);
    if (h[i] == NULL) {
        h[i] = user;
    }
    else {
        user->next = h[i];
        h[i] = user;
    }
}

void InsertReservaUser(hash_user h, KeyType k, Q2 *q2) {
	User *aux = RetrieveUser(h, k);
	if(!aux) {
		return;
	}
	while(aux) {
		if(strcmp(k, aux->id) == 0) {
			break;
		}
		aux = aux->next;
	}
	
	if(aux) {
		aux->total_reservas++;
		aux->total_gasto += q2->total_gasto;
		Q2 *currentQ2 = aux->q2;
		Q2 *prevQ2 = NULL;
		
		while (currentQ2 != NULL && strcmp(currentQ2->data, q2->data) >= 0) {
			prevQ2 = currentQ2;
			currentQ2 = currentQ2->next;
		}

		if (prevQ2 == NULL) {
			// Inserir no início
			q2->next = aux->q2;
			aux->q2 = q2;
		} else {
			// Inserir no meio ou no final
			prevQ2->next = q2;
			q2->next = currentQ2;
		}
	}
}

void InsertVooUser(hash_user h, KeyType k, Q2 *q2) {
	User *aux = RetrieveUser(h, k);
	if(!aux) {
		return;
	}
	while(aux) {
		if(strcmp(k, aux->id) == 0) {
			break;
		}
		aux = aux->next;
	}
	
	if(aux) {
		aux->total_voos++;
		Q2 *currentQ2 = aux->q2;
		Q2 *prevQ2 = NULL;
		
		while (currentQ2 != NULL && strcmp(currentQ2->data, q2->data) >= 0) {
			prevQ2 = currentQ2;
			currentQ2 = currentQ2->next;
		}

		if (prevQ2 == NULL) {
			q2->next = aux->q2;
			aux->q2 = q2;
		} else {
			prevQ2->next = q2;
			q2->next = currentQ2;
		}
	
	}
}


void Printhash_user(hash_user h) {
	int total_users = 0;
	int total_reservas_validas = 0;
    for (int i = 0; i < HASHSIZE; ++i) {
        User *aux = h[i];
        while (aux) {
            Q2 *rr = aux->q2;
            int total_reservas = 0;
            
            //printf("\n\nPos: %d, ID: %s, Nome: %s, Email: %s, Phone: %s, Birth: %s, Sex: %s, Passport: %s, Country: %s, Adrress: %s, Account: %s, Pay: %s, Status: %s\n", i, aux->id, aux->nome, aux->email, aux->phone, aux->birth, aux->sex, aux->passport, aux->country, aux->address, aux->account_creation, aux->pay_method, aux->account_status);
            
            while(rr) {
				printf("%s, %s, %d\n",rr->id, rr->data, rr->tipo);
				total_reservas++;
				total_reservas_validas++;
				rr = rr->next;
			}
			if(total_reservas > 0) {
				
				//printf("Total Reservas: %d\n",total_reservas);
			}
			
            
            aux = aux->next;
            total_users++;
        }
    }
    printf("Total Users: %d\n",total_users);
    printf("Total Reservas Global: %d\n",total_reservas_validas);
}










