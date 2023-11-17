#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"
#include "parsing.h"
#define MAX_LINE_LENGTH 1024
#include "user.h"

int valid_date(char *string) {
    if (strlen(string) != 10) {
        return 0;
    }

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            // Se estamos nas posições das barras, verifica se o caractere é '/'
            if (string[i] != '/') {
                return 0;
            }
        } else {
            // Se não estamos nas posições das barras, verifica se o caractere é um dígito entre '0' e '9'
            if (!isdigit(string[i])) {
                return 0;
            }
        }
    }

    int ano, mes, dia;
    if (sscanf(string, "%4d/%2d/%2d", &ano, &mes, &dia) != 3) {
        return 0;
    }
    if (ano < 0 || mes < 1 || mes > 12 || dia < 1 || dia > 31) {
        return 0;
    }
    return 1;  // A data é válida
}

int valid_date_hour(char *string) {
    // Verifica se a string tem o comprimento correto
    if (strlen(string) != 19) {
        return 0; // A string não tem o comprimento correto
    }

    if (string[4] != '/' || string[7] != '/' || string[10] != ' ' ||
        string[13] != ':' || string[16] != ':') {
        return 0; // Formato incorreto
    }

    for (int i = 0; i < 19; i++) {
        if (i != 4 && i != 7 && i != 10 && i != 13 && i != 16) {
            if (!isdigit(string[i])) {
                return 0; // Caracteres inválidos
            }
        }
    }

    int year, month, day, hour, minute, second;
    if (sscanf(string, "%4d/%2d/%2d %2d:%2d:%2d", &year, &month, &day, &hour, &minute, &second) != 6) {
        return 0; // Falha ao analisar a data e hora
    }
    if (month < 1 || month > 12 || day < 1 || day > 31 ||
        hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return 0; // Valores inválidos
    }

    return 1; // A data e hora são válidas
}

int compare_birth_with_account_cr(char *string1, char *string2) { //string1==birth e string2== account_creation
    // Verifica se as strings têm o comprimento correto
    if (strlen(string1) != 10 || strlen(string2) != 19) {
        return -1; // Comprimento incorreto
    }

    // Extrai os componentes do birth
    int year_date, month_date, day_date;
    if (sscanf(string1, "%4d/%2d/%2d", &year_date, &month_date, &day_date) != 3) {
        return -1; // Falha ao analisar o birth
    }

    // Extrai os componentes da acc
    int year_date_hour, month_date_hour, day_date_hour, hour_date_hour, minute_date_hour, second_date_hour;
    if (sscanf(string2, "%4d/%2d/%2d %2d:%2d:%2d", &year_date_hour, &month_date_hour, &day_date_hour, &hour_date_hour, &minute_date_hour, &second_date_hour) != 6) {
        return -1; // Falha ao analisar o acc
    }

    // Compara os anos, meses e dias
    if (year_date < year_date_hour || (year_date == year_date_hour && (month_date < month_date_hour || (month_date == month_date_hour && day_date < day_date_hour)))) {
        return 1; // birth é anterior a acc
    }

    return 0; // birth não é anterior a acc
}

int valid_email(char *string) {
    int i, atFound = 0, dotFound = 0;
    int usernameLength = 0, domainLength = 0, tldLength = 0;
    int emailLength = strlen(string);

    // Verificar se o email tem pelo menos tamanho 5 (x@y.z)
    if (emailLength < 5) {
        return 0;  // Formato inválido
    }

    for (i = 0; i < emailLength; i++) {
        if (string[i] == '@') {
            atFound = 1;
            usernameLength = i;  // Comprimento do <username>
        } else if (string[i] == '.') {
            dotFound = 1;
            domainLength = i - usernameLength - 1;  // Comprimento do <domain>
            tldLength = emailLength - i - 1;  // Comprimento do <TLD>
        }
    }

    // Verificar se '@' e '.' foram encontrados, e se os comprimentos são válidos
    if (atFound && dotFound && usernameLength > 0 && domainLength > 0 && tldLength >= 2) {
        return 1;  // Formato válido
    } else {
        return 0;  // Formato inválido
    }
}

int valid_country_code(char *string){
    if(strlen(string) == 2) return 1;
    else return 0;
}

int valid_account_status(char *string) {
    char *s_aux = strdup(string);
    for (int i = 0; s_aux[i] != '\0'; i++) {
        s_aux[i] = tolower(s_aux[i]);
    }

    int result = (strcmp(s_aux, "active") == 0 || strcmp(s_aux, "inactive") == 0);

    free(s_aux);  // Mova a chamada de free para este ponto

    return result;
}

int valid_pay_method(char *string) {
    return (strcasecmp(string, "CASH") == 0 || strcasecmp(string, "DEBIT_CARD") == 0 || strcasecmp(string, "CREDIT_CARD") == 0);
}

int valid_seat_numbers(char *string, char *string1){
    int n1 = atoi(string);
    int n2 = atoi(string1);

    // Verificar se o número de lugares é maior ou igual ao número de passageiros
    return n1 >= n2;
}

int valid_aeroport(char *string) {
    // Verificar se a string tem exatamente 3 caracteres
    if (strlen(string) != 3) {
        return 0;
    }

    // Verificar se todos os caracteres são letras
    for (int i = 0; string[i] != '\0'; i++) {
        if (!isalpha(string[i])) {
            return 0;
        }
    }

    return 1;
}


int valid_stars(char *string) {
    if (isdigit(string[0]) && (string[0] >= '1' && string[0] <= '5') && string[1]=='\0') {
        return 1;
    }
    return 0;
}

int valid_id(char *string){
    if (string[0] == '\0') return 0;
    for (int i = 0; string[i] != '\0'; i++){
        if (isdigit(string[i]) == 0) return 0;
    }
    return 1;
}


int valid_phone_number(char *string){
    if(string[0] == '\0') return 0;
    for (int i = 0; string[i] != '\0'; i++){
        if (isdigit(string[i]) == 0) return 0;
    }
    return 1;
}

///caso exista erros pode ser de esta só funcionar com maiusculas
int valid_sex(char *string){
    if (string[0] == 'M' || string[0] == 'F') return 1;
    else return 0;
}




int valid_breakfast(char *string) {
    // Converte a string para minúsculas
    for (int i = 0; string[i]; i++) {
        string[i] = tolower(string[i]);
    }

    if (!strcmp(string, "true") || !strcmp(string, "t") || !strcmp(string, "1") ||
        !strcmp(string, "false") || !strcmp(string, "f") || !strcmp(string, "0") ||
        !strcmp(string, "")) {
        return 1; // Valor considerado válido
    }

    return 0; // Valor considerado inválido
}

int valid_tax(char *string) {
    for (int i = 0; string[i] != '\0'; i++) {
        if (!isdigit(string[i])) {
            return 0;  // Caracteres não numéricos encontrados
        }
    }
    int tax = atoi(string);  // Converte a string para um número inteiro
    // Verifica se o número é maior ou igual a 0
    if (tax >= 0) {
        return 1;
    }

    return 0;
}

int valid_price(char *string) {
    for (int i = 0; string[i] != '\0'; i++) {
        if (!isdigit(string[i])) {
            return 0;  // Caracteres não numéricos encontrados
        }
    }
    int tax = atoi(string);  // Converte a string para um número inteiro
    // Verifica se o número é maior ou igual a 0
    if (tax > 0) {
        return 1;
    }

    return 0;
}



int valid_rating(char *string) {
    if (string[0] == '\0') {
        return 1;  // String vazia, considerada válida
    }

    if (isdigit(string[0])) {
        int rating = atoi(string);
        if (rating >= 1 && rating <= 5) {
            return 1;  // Valor de classificação válido
        }
    }

    return 0;  // Valor de classificação inválido
}


//para verificar que alguns critérios não são vazios 
int is_non_empty_string(char *string) {
    return (strlen(string) > 0);
}

int compare_begin_with_end(char *string1, char *string2) {
    // Verifica se as strings têm o comprimento correto
    if (strlen(string1) != 10 || strlen(string2) != 10) {
        return -1; // Comprimento incorreto
    }

    // Extrai os componentes do begin
    int year_date, month_date, day_date;
    if (sscanf(string1, "%4d/%2d/%2d", &year_date, &month_date, &day_date) != 3) {
        return -1; 
    }

    // Extrai os componentes do end
    int year_date_hour, month_date_hour, day_date_hour;
    if (sscanf(string2, "%4d/%2d/%2d", &year_date_hour, &month_date_hour, &day_date_hour) != 3) {
        return -1; 
    }

    // Compara os anos, meses e dias
    if (year_date < year_date_hour || (year_date == year_date_hour && (month_date < month_date_hour || (month_date == month_date_hour && day_date < day_date_hour)))) {
        return 1; // begin é anterior a end
    }

    return 0; // begin não é anterior a end
}



//void parse_reservas_csv(hash_user hoteis, hash_userUsers users, char *line) {
	 //InsertTableUsers(h, id, name, email, phone_number, birth_date, sex, passport, country_code, address, account_creation, pay_method, account_status);
	 //InsertTableHoteis(h, id, name, email, phone_number, birth_date, sex, passport, country_code, address, account_creation, pay_method, account_status);
//}
