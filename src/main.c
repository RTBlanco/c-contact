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

int add_contact();
void display_contacts();
int search_contact();
int destroy_contact();
int init_setup();


int main(int argc, char *argv[]) {
  
  if (argc > 1 && strcmp(argv[1], "init") == 0) {
    init_setup();
  } else {
    printf("No argument\n");
  } 

  
  return 0;
}

int init_setup() {
  printf("initializing\n");

  int folder = mkdir("contact_data", 0777);

  if (folder == 0) {
    printf("Folder was created\n");
  } else {
    printf("folder exists already\n");
  }

  FILE *f = fopen("./contact_data/contacts.json", "w");
  if (f == NULL ) {return 1;}
  fclose(f);

  return 0;
}