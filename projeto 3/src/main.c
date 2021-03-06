#include "header.h"

void choiceOption();
void choiceOption1();
void choiceOption2();
void choiceOption3();

int main(int argv, char **argc)
{

  int menu = 1, op;

  DataType *list, *show; 

  list = voidList();
  list = readDataFile();

  do{
  /* Menu functions:
      see all registers
      insert register
      remove register by name
      see register by name
      see in order register
      exit

      @todo
        do the validations for each options
 */

    system("clear");
    printf("=========================================\n\n");
    printf("\tAGENDA DE CONTATOS\n\n");
    printf("=========================================\n");
    printf("[0] - Visualizar todos os registros\n");
    printf("[1] - Inserir Registro\n");
    printf("[2] - Remover Registro por nome\n");
    printf("[3] - Visualizar Registro por nome\n");
    printf("[4] - Visualizar em Ordem Alfabética\n");
    printf("[5] - Sair\n");
    printf("=========================================\n\n");
    
    printf("Digite sua escolha: ");
    scanf("%d", &op);
    
    switch(op) {

      case 0:
        system("clear");
        printf("=========================================\n\n");
        printf("\tVisualizar Todos os Registros\n\n");
        printDataFile(list);
        
        printf("\n=========================================\n\n");

        choiceOption();

        break;

      case 1:
        system("clear");
        printf("=========================================\n\n");
        printf("\tInserir Registro\n\n");
        list = registerData(list);
        printDataFile(list);
        printf("\n=========================================\n\n");
        
        choiceOption1();
        break;
        
      case 2:
        system("clear");
        printf("=========================================\n\n");
        printf("\tRemover Registro por Nome\n\n");

        char *rm = inputName();
        removeByName(list, rm);

        printf("\n=========================================\n\n");

        choiceOption2();
        break;
        
      case 3:
        system("clear");
        printf("=========================================\n\n");
        printf("\tVisualizar Registro por Nome\n\n");

        char *resposta = inputName();
        //printf("%s", resposta); 
        seeByName(list, resposta);

        printf("\n=========================================\n\n");

        choiceOption3();
        break;
        
      case 4:
        system("clear");
        printf("=========================================\n\n");
        printf(" Visualizar Todos os Registros em Ordem\n\n");
        list = insertionSort(printDataFile(list));
        printf("\n=========================================\n\n");

        choiceOption();
        break;
        
      case 5:
        printf("\nAgenda Finalizada\n\n");
        menu = 0;

        exitProgram(list);
        freeList(list);

        break;
        
      default:
        break;
    
    }

  } while(menu == 1);


  return 0;
}

void choiceOption(){
  int ch;
  printf("[0] - Voltar ao Menu da Agenda\n");
  printf("[5] - Sair\n");
  printf("\n=========================================\n");
  printf("Digite sua escolha: ");
  scanf("%d", &ch);

  if(ch == 5){
    printf("\nAgenda Finalizada\n\n");
    
    DataType *list;
    exitProgram(list);
    freeList(list);
    
    exit(0);
  }
}

void choiceOption1(){
  int ch;
  printf("[0] - Voltar ao Menu da Agenda\n");
  printf("[1] - Inserir outro Registro\n");
  printf("[5] - Sair\n");
  printf("\n=========================================\n");
  printf("Digite sua escolha: ");
  scanf("%d", &ch);

  if(ch == 5){
    printf("\nAgenda Finalizada\n\n");

    DataType *list;
    exitProgram(list);
    freeList(list);

    exit(0);
  }

  else if(ch == 1){
    system("clear");
    printf("=========================================\n\n");
    printf("\tInserir Registro\n\n");

    DataType *list;
    registerData(list);
    printDataFile(list);

    // list = insertBeginning(list, newr);
    printf("\n=========================================\n\n");

    choiceOption1();
  }

}

void choiceOption2(){
  int ch;
  printf("[0] - Voltar ao Menu da Agenda\n");
  printf("[2] - Remover outro Registro\n");
  printf("[5] - Sair\n");
  printf("\n=========================================\n");
  printf("Digite sua escolha: ");
  scanf("%d", &ch);

  if(ch == 5){
    printf("\nAgenda Finalizada\n\n");

    DataType *list;
    exitProgram(list);
    freeList(list);

    exit(0);
  }

  else if(ch == 2){
    system("clear");
    printf("=========================================\n\n");
    printf("\tRemover Registro por Nome\n\n");

    printf("\n=========================================\n\n");

    choiceOption2();
  }

}

void choiceOption3(){
  int ch;
  printf("[0] - Voltar ao Menu da Agenda\n");
  printf("[3] - Visualizar outro Registro\n");
  printf("[5] - Sair\n");
  printf("\n=========================================\n");
  printf("Digite sua escolha: ");
  scanf("%d", &ch);

  if(ch == 5){
    printf("\nAgenda Finalizada\n\n");

    DataType *list;
    exitProgram(list);
    freeList(list);

    exit(0);
  }

  else if(ch == 3){
    system("clear");
    printf("=========================================\n\n");
    printf("\tVisualizar Registro por Nome\n\n");
    
    DataType *list;
    
    char *resposta = inputName();
    seeByName(list, resposta);

    printf("\n=========================================\n\n");

    choiceOption3();
  }
}