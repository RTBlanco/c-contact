#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

/*
Add a contact
View all contacts
Search by name
Delete a contact
Save contacts to a file
Load contacts when the program starts
*/

int add_contact(int run);
void show_all(int *run);
void display_actions();
void display_contact_actions();
int search_contact(int run);
int destroy_contact();
int init_setup();


int main(int argc, char *argv[]) {
  
  if (argc > 1 && strcmp(argv[1], "init") == 0) {
    init_setup();
    return 0;
  } else {
    
    int running = 0;
    int *run = &running;
    char user_input[2];

    while (running == 0) {
      display_actions();
      fgets(user_input, sizeof(user_input), stdin);

      switch (user_input[0]) {
        case '1':
          show_all(run);
          break;
        case '2':
          add_contact(*run);
          break;
        case '3':
          search_contact(*run);
          break;
        default:
          break;
      }
    }
    
    return 0;
  }
}

void show_all(int *run) {
  *run = 1;
  printf("showing all Contacts\n");
}

int add_contact(int run) {

  run = 1;
  printf("add new contact\n");

  return 0;
}

int search_contact(int run){
  run = 1;

  printf("Searching contact \n");

  return 0;
}

void display_actions() {
  printf("\033[H\033[J");
  printf("\n\n");
  printf("1) Show All Contacts\n");
  printf("2) Add New Contact\n");
  printf("3) Search Contact\n");
  printf("\n");
}



int init_setup() {
  printf("initializing\n");

  int folder = mkdir("contact_data", 0777);

  if (folder == 0) {
    printf("Folder was created\n");
  } else {
    printf("folder exists already\n");
  }

  FILE *f = fopen("./contact_data/contacts.csv", "w");
  if (f == NULL ) {return 1;}
  fclose(f);

  return 0;
}