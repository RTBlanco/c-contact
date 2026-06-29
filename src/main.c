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

int add_contact(int *run);
int show_all(int *run);
void display_actions();
void display_contact_actions();
int search_contact(int *run);
int destroy_contact();
int init_setup();
int read_data();
int update_data(const char *contact);
int read_data_callback(char mode[], int (*callback)(char *buffer, int line));
int display_all_contact(char *buffer, int line);


int main(int argc, char *argv[]) {
  
  if (argc > 1 && strcmp(argv[1], "init") == 0) {
    init_setup();
    return 0;
  } else {
    
    int running = 0;
    int *run = &running;
    char user_input[10];

    while (running == 0) {
      display_actions();
      fgets(user_input, sizeof(user_input), stdin);

      switch (user_input[0]) {
        case '1':
          show_all(run);
          break;
        case '2':
          add_contact(run);
          break;

        case '3':
          search_contact(run);
          break;
        default:
          break;
      }
    }
    
    return 0;
  }
}

int show_all(int *run) {
  printf("showing all Contacts\n");
  printf("First Name | Last Name | Phone number \n");
  printf("=======================================");
  // read_data();
  read_data_callback("r", display_all_contact);
  *run = 1;
  return 0;
}

int add_contact(int *run) {
  int contact_running = 0;

  char first_name[50];
  char last_name[50];
  char phone_number[50];


  printf("add new contact\n");
  while (contact_running == 0) {

    printf("Enter first name: ");
    fgets(first_name, sizeof(first_name), stdin);
    first_name[strcspn(first_name, "\n")] = '\0';

    printf("Enter Last name: "); 
    fgets(last_name, sizeof(last_name), stdin);
    last_name[strcspn(last_name, "\n")] = '\0';

    printf("Enter Phone Number: "); 
    fgets(phone_number, sizeof(phone_number), stdin);
    phone_number[strcspn(phone_number, "\n")] = '\0';


    if (strcmp(first_name, "done") == 0) {
      contact_running = 1;
      *run = 1;
    } else if (first_name[0] != '\0' && last_name[0] != '\0' && phone_number[0] != '\0') {
      contact_running = 1;

      printf("You entered: %s | %s | %s \n", first_name, last_name, phone_number);

      char contact[(sizeof(first_name) + sizeof(last_name) + sizeof(phone_number))];
      snprintf(contact, sizeof(contact), "%s,%s,%s", first_name, last_name, phone_number);
      update_data(contact);

      *run = 1;
    }
  }
  
  return 0;
}

int search_contact(int *run){
  

  char buffer[2000];
  FILE *f = fopen("./contact_data/contacts.csv", "r");
  if (f == NULL ) {return 1;}
  
  int line = 0;
  int found_contact = 0;
  char search[50];
  int show_contact = 1;
  char contact[sizeof(buffer)];


  printf("Enter first or last name \n");
  while(found_contact == 0) {

    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = '\0';

    while(fgets(buffer, sizeof(buffer), f)) {
      buffer[strcspn(buffer, "\n")] = '\0';
      
      
      strcpy(contact, buffer);
      char *token = strtok(buffer, ",");
      
      while (token != NULL) {
        
        if (line != 0){
          if (strcmp(token, search) == 0){
            printf("Found Contact\n");
            printf("%s\n", contact);
            // return line;
          }
        }

        token = strtok(NULL, ",");
      }

      
      line ++;
  
      printf("\n");
    }

  }
  
  fclose(f);

  *run = 1;
  // return 1;
  return 0;
}

void display_actions() {
  // printf("\033[H\033[J");
  printf("\n");
  printf("1) Show All Contacts\n");
  printf("2) Add New Contact\n");
  printf("3) Search Contact\n");
  // printf("\n");
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
  fprintf(f, "%s", "first_name,last_name,phone_number");

  fclose(f);

  return 0;
}

int update_data(const char *contact) {
  FILE *f = fopen("./contact_data/contacts.csv", "a");
  if (f == NULL ) {return 1;}

  fprintf(f, "\n%s", contact);

  fclose(f);
  return 0;
}

int read_data(){

  char buffer[2000];
  FILE *f = fopen("./contact_data/contacts.csv", "r");
  if (f == NULL ) {return 1;}
  
  int line = 0;

  while(fgets(buffer, sizeof(buffer), f)) {
    buffer[strcspn(buffer, "\n")] = '\0';
    
    char *token = strtok(buffer, ",");
    
    while (token != NULL) {
      
      if (line == 0){
        token = strtok(NULL, ",");
        continue;
      } else {

        printf("%s | ", token);
        token = strtok(NULL, ",");

      }
    }

    line ++;

    printf("\n");
  

  }
  
  fclose(f);

  return 0;
}

int display_all_contact(char *buffer, int line) {
  char *token = strtok(buffer, ",");
  
  while (token != NULL) {
    if (line == 0){
      // printf("%s\n", token);
      token = strtok(NULL, ",");
      continue;
    } else {

      printf("%s | ", token);
      token = strtok(NULL, ",");

    }
  }


  printf("\n");
  return 0;
}

int read_data_callback(char mode[], int (*callback)(char *buffer, int line)) {
  char buffer[2000];
  FILE *f = fopen("./contact_data/contacts.csv", mode);
  if (f == NULL ) {return 1;}
  
  int line = 0;

  while(fgets(buffer, sizeof(buffer), f)) {
    buffer[strcspn(buffer, "\n")] = '\0';
    
    
    callback(buffer, line);
  
    line ++;
  }
  
  fclose(f);

  return 0;
}