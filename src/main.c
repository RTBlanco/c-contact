#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>


int add_contact(int *run);
void display_actions();
void display_contact_actions();
int destroy_contact();
int init_setup();
int update_data(const char *contact);
int read_data_callback(char mode[], int (*callback)(), void *passed_args);
int display_all_contact(void *passed_args);
int contact_search(void *passed_args);
int edit_contact(void *args);


typedef struct {
  int line;
  char *first_name;
  char *last_name;
  char *phone_number;
} Contact;

typedef struct {
  Contact contact_data;
  int line;
  char search[50];
  char buffer[2000];
  char contact[2000];
} CBArguments;


int main(int argc, char *argv[]) {
  
  if (argc > 1 && strcmp(argv[1], "init") == 0) {
    init_setup();
    return EXIT_SUCCESS;
  } else {
    
    int running = 0;
    int *run = &running;
    char user_input[10];
    CBArguments args;

    while (running == 0) {
      display_actions();
      fgets(user_input, sizeof(user_input), stdin);

      switch (user_input[0]) {
        case '1':

          printf("showing all Contacts\n");
          printf("First Name | Last Name | Phone number \n");
          printf("=======================================");
          read_data_callback("r", display_all_contact, &args);
          *run = 1;
          // return EXIT_SUCCESS;

          break;
        case '2':
          add_contact(run);
          break;

        case '3':

          printf("Enter first or last name \n");
          fgets(args.search, sizeof(args.search), stdin);
          args.search[strcspn(args.search, "\n")] = '\0';

          if (read_data_callback("r", contact_search, &args) == EXIT_SUCCESS) {
            char answer[5];
            printf("would your like to edit contact? (y|n): ");
            fgets(answer, sizeof(answer), stdin);
            answer[strcspn(answer, "\n")] = '\0';
            if (strcmp(answer, "y") == EXIT_SUCCESS) {
              edit_contact(&args);
            }
          } else {
            printf("Contact Not found\n");
          }

          *run = 1;
          break;
        default:
          break;
      }
    }
    
    return EXIT_SUCCESS;
  }
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
  
  return EXIT_SUCCESS;
}


int contact_search(void *passed_args) {
  CBArguments *args = (CBArguments *)passed_args;

  strcpy(args->contact, args->buffer);
  char *token = strtok(args->buffer, ",");
  
  while (token != NULL) {
    
    if (args->line != 0){
      if (strcmp(token, args->search) == 0){
        printf("Found Contact\n");
        printf("%s\n", args->contact);
        
        Contact contact_data = {args->line, strtok(args->contact,","),strtok(NULL,","), strtok(NULL,",")};
        args->contact_data = contact_data;
        return EXIT_SUCCESS;

      }
    }

    token = strtok(NULL, ",");
  }

  return EXIT_FAILURE;
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

  return EXIT_SUCCESS;
}

int update_data(const char *contact) {
  FILE *f = fopen("./contact_data/contacts.csv", "a");
  if (f == NULL ) {return 1;}

  fprintf(f, "\n%s", contact);

  fclose(f);
  return EXIT_SUCCESS;
}

int display_all_contact(void *passed_args) {
  CBArguments *args = (CBArguments *)passed_args;

  char *token = strtok(args->buffer, ",");
  
  while (token != NULL) {
    if (args->line == 0){
      token = strtok(NULL, ",");
      continue;
    } else {

      printf("%s | ", token);
      token = strtok(NULL, ",");

    }
  }


  printf("\n");
  return EXIT_SUCCESS;
}

int read_data_callback(char mode[], int (*callback)(void *), void *passed_args) {
  FILE *f = fopen("./contact_data/contacts.csv", mode);
  if (f == NULL ) {return 1;}

  CBArguments *args = (CBArguments *)passed_args;
  args->line = 0;

  int return_value;

  while(fgets(args->buffer, sizeof(args->buffer), f)) {
    args->buffer[strcspn( args->buffer, "\n")] = '\0';
    
    
    return_value = callback(args);
  
    args->line ++;
  }
  
  fclose(f);

  return return_value;
}

int edit_contact(void *args) {
  CBArguments *cb_args = (CBArguments *)args;
  // int sed = system("sed '2s/^[^,]*/NEW_NAME/' ./contact_data/contacts.csv"); 
  int running = 1;
  char answer[5];
  char sed_string[500];
  printf("1) First Name (%s)\n", cb_args->contact_data.first_name);
  printf("2) Last Name (%s)\n", cb_args->contact_data.last_name);
  printf("3) Phone Number (%s)\n", cb_args->contact_data.phone_number);

  while(running) {
    fgets(answer, sizeof(answer), stdin);
    answer[strcspn(answer, "\n")] = '\0';

    switch (answer[0]){
    case '1': {
      char new_name[50];

      printf("Enter new First Name: ");
      fgets(new_name, sizeof(new_name), stdin);
      new_name[strcspn(new_name, "\n")] = '\0';

      snprintf(sed_string, sizeof(sed_string), "sed -i '' '%is/^[^,]*/%s/' ./contact_data/contacts.csv",cb_args->contact_data.line + 1, new_name);
      // printf("%s\n", sed_string);
      system(sed_string);

      printf("%s\n", new_name);
      break;
    }

    case '2': {

      char new_name[50];
  
      printf("Enter new Last Name: ");
      fgets(new_name, sizeof(new_name), stdin);
      new_name[strcspn(new_name, "\n")] = '\0';
  
      snprintf(sed_string, sizeof(sed_string), "sed -i '%is/^\\([^,]*,\\)[^,]*/\\1%s/' ./contact_data/contacts.csv", cb_args->contact_data.line + 1, new_name);
      system(sed_string);
  
      break;
    }

    case '3': {
      char new_phone[50];
  
      printf("Enter new Phone Number: ");
      fgets(new_phone, sizeof(new_phone), stdin);
      new_phone[strcspn(new_phone, "\n")] = '\0';
  
  
  
      snprintf(sed_string, sizeof(sed_string), "sed -i '%is/^\\([^,]*,[^,]*,\\).*/\\1%s/' ./contact_data/contacts.csv", cb_args->contact_data.line + 1 , new_phone);
      system(sed_string);
  
      break; 
    }
    default:
      break;
    }
  }

  return EXIT_SUCCESS;
}