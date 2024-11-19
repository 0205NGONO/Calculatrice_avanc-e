#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <string>
using namespace std ;

const int MAX_SIZE = 100  ;
//Tableau de pointeurs de fonctions
const char* nomsFonctions[9] = {"sin", "cos", "tan", "log", "exp", "sqrt", "arccos", "arcsin", "arctan"} ;
//Déclaration des fonctions
bool evaluer_Expression(const char* expression, double& result) ;
bool evaluer_SousExpression(const char* expression, int& index, double& result) ;
int evaluer_fonction(const char* expr, int& i) ;
bool chiffre_point(char c) ;
bool Egales(const char* str1, const char* str2) ;
int priorite(char op) ;
//Fonction principale
int main(int argc, char const *argv[]) {
    char choix = 'y' ;

    while (choix == 'y') {
        cout << "Entrez une expression  : \n" ;
        char expression[MAX_SIZE] ;
        cin.getline(expression, MAX_SIZE) ;

        double resultat ;
        if (evaluer_Expression(expression , resultat)) {
            cout << "Résultat : " << resultat << endl ;
        }

        cout << "Voulez-vous entrer une autre expression ? (y/n) : " ;
        cin >> choix ;
        cin.ignore() ; 
    }

    return 0 ;
}
//Vérifier si un caractère est un chiffre ou un point
bool chiffre_point(char c) {
    return (c >= '0' && c <= '9') || c == '.' ;
}
//Vérifier si un caractère est une lettre
bool Lettre(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ;
}
//Comparer des chaines de caractères
bool Egales(const char* chaine1, const char* chaine2) {
    while (*chaine1 && *chaine2) {
        if (*chaine1 != *chaine2) return false ;
        chaine1++ ;
        chaine2++ ;
    }
    return *chaine1 == *chaine2 ;
}
//Rechercher les paramètres d'une fonction dans l'expression
int evaluer_fonction(const char* expression, int& i) {
    for (int j = 0 ; j < 9 ; ++j) {
        const char* nom = nomsFonctions[j] ;
        int k = 0 ;
        while (nom[k] != '\0' && expression[i + k] == nom[k]) {
            k++ ;
        }

        if (nom[k] == '\0' && (expression[i + k] == '(' || expression[i + k] == '\0')) {
            i += k ; 
            return j ;
        }
    }
    return -1 ; 
}
/*Détermine si une partie de l'expression correspond ) une fonction mathématique
et renvoie le résultat si la valeur associée est conforme aux contraintes*/
double evaluer_Fonction(const char* fonction, double valeur) {
    if (Egales(fonction , "sin")) return sin(valeur) ;
    if (Egales(fonction , "cos")) return cos(valeur) ;
    if (Egales(fonction , "tan")){
        if (valeur == 1.5708 ){
            cout <<"Erreur : tangente de Pi/2 n'existe pas !\n" ;
        }
        else return tan(valeur) ;
    } 
    if (Egales(fonction , "log")) {
        if (valeur <= 0){
            cout <<"Erreur : Logarithme d'un nombre négatif ou null !\n" ;
        }
        else return log(valeur) ; 
    }
    if (Egales(fonction , "exp")) return exp(valeur) ;
    if (Egales(fonction , "sqrt")) {
        if (valeur < 0){
                cout <<"Erreur : Racine carrée d'un nombre négatif !\n" ;
        }
        else return sqrt(valeur) ;
    } 
    if(Egales(fonction , "arccos")){
        if (valeur < -1 || valeur > 1){
            cout <<"Erreur : arccos défini entre -1 et 1 !\n" ;
        }
        else return acos(valeur) ;
    }
    if(Egales(fonction , "arcsin")){
        if (valeur < -1 || valeur > 1){
            cout <<"Erreur : arcsin défini entre -1 et 1 !\n" ;
        }
        else return asin(valeur) ;
    }
    if(Egales(fonction , "arctan")) return atan(valeur) ;
    return valeur; //On renvoie la valeur si aucune fonction ne correspond
}

bool evaluer_Expression(const char* expression , double& result) {
    int index = 0 ; 
    return evaluer_SousExpression(expression , index , result) ;
}
/*Evaluer les sous-expressions en gérant les nombres, les opérateurs, les fonctions et les parenthèses.
Les valeurs et les opérateurs sont stockés dans des tableaux.
Une priorité est accordée aux contenus des parenthèses, viennent ensuite les fonctions et l'opérateur "^", 
puis les opérateurs "*" et "/" et enfin, les opérateurs "+" et "-".*/
bool evaluer_SousExpression(const char* expression , int& index , double& result) {
    double valeurs[MAX_SIZE] ;//Tableau des valeurs de la sous-expression
    char operations[MAX_SIZE] ;//Tableau pour stocker les opérations
    int valeur_compteur = 0 ;
    double valeur_courante = 0.0 ;//La valeur en cours d'utilisation
    char lastOp = '+' ; //Dernière opération rencontrée
    char fonction[10] ; 
    bool fonction_Active = false ; 

    while (expression[index] != '\0') {
        if (expression[index] == ' ') {
            index++ ;
            continue ;
        }//Ignorer les espaces

        if (chiffre_point(expression[index])) {
            double valeur = 0.0 ;
            int pointCount = 0 ;
            double fraction = 1.0 ;

            while (chiffre_point(expression[index])) {
                if (expression[index] == '.') {
                    pointCount++ ;//Compte le nombre de points

                } else {
                    if (pointCount == 0) {
                        valeur = valeur * 10 + (expression[index] - '0') ;/*Ajoute un chiffre situé avant le point
                        décimal à la valeur*/
                    } else {
                        fraction *= 10 ;
                        valeur += (expression[index] - '0') / fraction ;
                    }/*Ajoute un chiffre situé après le point
                        décimal à la valeur*/
                }
                index++ ;
            }

            valeur_courante = valeur ; //Mise à hour de la valeur courante

        } else if (Lettre(expression[index])) {//Récupère le nom d'une fonction dans l'expression
            int j = 0 ;

            while (Lettre(expression[index]) && j < 9) { 
                fonction[j++] = expression[index++] ;
            }
            fonction[j] = '\0' ; 
            fonction_Active = true ; 

        } else if (expression[index] == '(') {//Traitement du contenu des parenthèses
            index++ ; 
            if (!evaluer_SousExpression(expression, index , valeur_courante)) {/*Evaluer la sous-expression entre 
            parenhèses*/
                return false ; 
            }

        } else if (expression[index] == ')') {
            index++  ; 
            break  ; 
        } else {
            if (fonction_Active) {//Calcul de la valeur d'une fonction
                valeur_courante = evaluer_Fonction(fonction , valeur_courante) ;
                fonction_Active = false ; 
            }

            if (lastOp == '^') {//Calcul d'une puissance
                valeurs[valeur_compteur - 1] = pow(valeurs[valeur_compteur - 1], valeur_courante) ; 
            }else if (lastOp == '*') {//Calcul d'un produit
                valeurs[valeur_compteur - 1] *= valeur_courante ; 
            } else if (lastOp == '/') {//Calcul d'une division
                if (valeur_courante == 0) {
                    cout << "Erreur : Division par zéro !\n" ;
                    return false ;
                }
                valeurs[valeur_compteur - 1] /= valeur_courante ; 

            } else if (lastOp == '-') {//Calcul d'une différence
                valeurs[valeur_compteur++] = -valeur_courante ; 

            } else if (lastOp == '+') {//Calcul d'une somme
                valeurs[valeur_compteur++] = valeur_courante ; 
            }

            lastOp = expression[index] ;
            index++ ; 
            valeur_courante = 0.0 ; 
            continue ;
        }
    }
    //Calcul du résultat renvoyé par le dernier opérateur
    if (fonction_Active) {
        valeur_courante = evaluer_Fonction(fonction , valeur_courante) ;
    }

    if (lastOp == '^') {
        valeurs[valeur_compteur - 1] = pow(valeurs[valeur_compteur - 1], valeur_courante) ;

    } else if (lastOp == '*') {
        valeurs[valeur_compteur - 1] *= valeur_courante ;

    } else if (lastOp == '/') {
        if (valeur_courante == 0) {
            cout << "Erreur : Division par zéro !\n" ;
            return false;
        }
        valeurs[valeur_compteur - 1] /= valeur_courante ;

    } else if (lastOp == '-') {
        valeurs[valeur_compteur++] = -valeur_courante ;

    } else if (lastOp == '+') {
        valeurs[valeur_compteur++] = valeur_courante ;
    }
    //Renvoyer 0 si aucune opération n'a été effectuée
    result = 0 ;
    for (int j = 0 ; j < valeur_compteur ; j++) {
        result += valeurs[j] ;
    }

    return true ; 
}
