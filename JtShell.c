/*
JT Shell
James 
Tom
*/
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define JTSH_TOK_BUFSIZE 64
#define JTSH_TOK_DELIM " \t\r\n\a"

/*
  Function Declarations for builtin shell commands:
 */
int jtsh_cd(char **args);
int jtsh_homepage(char **args);
int jtsh_help(char **args);
int jtsh_exit(char **args);
int jtsh_launch(char **args);
int jtsh_search_file(char **args);
int jtsh_replace_file(char **args);
int jtsh_install_github(char **args);
int jtsh_hello_world(char **args);
char *homepage;


/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "help",
  "exit", 
  "cd",
  "net",
  "search",
  "replace", 
  "github",
  "helloworld"
};

int (*builtin_func[]) (char **) = {
  &jtsh_help,
  &jtsh_exit,
  &jtsh_cd,
  &jtsh_homepage,
  &jtsh_search_file,
  &jtsh_replace_file, 
  &jtsh_install_github,
  &jtsh_hello_world
};

int jtsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int jtsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "jtsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("jtsh");
    }
  }
  return 1;
}

/**
   @brief Bultin command: open firefox.
   @param args List of args.  args[0] is "net".  args[1] is the command.
   @return Always returns 1, to continue executing.
 */
int jtsh_homepage(char **args)
{
  if (args[1] == NULL) {
    args[0] = "firefox";
  }else if (strcmp(args[1], "-h") == 0) {
     printf("Opening Homepage: %s\n", homepage);
     args[0] = "firefox";
     args[1] = homepage;
  }else if (strcmp(args[1], "-s") == 0) {
    if (args[2] == NULL){
      printf("Expected net -s URL\n");
      return 1;
    }
    printf("Changing homepage\n");
    sprintf(homepage, "%s", args[2]);    
    FILE *config;
    config = fopen("config", "a");
    fputs("homepage ", config);
    fputs(homepage, config);
    fputs("\n", config);
    fclose(config);
    
    return 1;
  }
  pid_t pid;
  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp("firefox", args) == -1) {
      perror("jtsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("jtsh");
  }
  return 1;
}
/**
   @brief Bultin command: installs/updates github This builtin is alias.
   @param args List of args. None.
   @return Always returns 1, to continue executing.
 */
int jtsh_install_github(char **args)
{
  printf("Installing github\n");
  args[1]="sudo";
  args[2]="apt-get";
  args[3]="install";
  args[4]="git";
  args[5]=NULL;

  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp("sudo", args) == -1) {
      perror("jtsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("jtsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}
/**
   @brief Bultin command: create a hello world file for a specified language
   @param args List of args. arh[0]] is "helloworld" arg[1] is the Filename, arg[2] is the language.
   @return Always returns 1, to continue executing.
 */
int jtsh_hello_world(char **args)
{
  if (args[1] == NULL) {
      printf("expected a filename or \"help\" as the first argument\n");
      return 1;
  }else{
    if(strcmp(args[1], "help") == 0){
      printf("The suported language are:\n");
      printf("c\n");
      printf("c++\n");
      printf("c#\n");
      printf("java\n");
      printf("python\n");
      return 1;
    }
  }
  if (args[2] == NULL) {
      printf("expected a language as second argument\n");
      return 1;
  }
  printf("Creating file %s in %s:\n", args[1], args[2]);
  FILE *hello;
  char name[255];
  //Java
  if(strcmp(args[2], "java") == 0) {
    sprintf(name, "%s.%s", args[1], args[2]);
    hello = fopen(name, "w");
    if(hello){
      fputs("public class ", hello);
      fputs(args[1], hello);
      fputs("\n{\n", hello);
      fputs("  public static void main(String[] args) {\n",hello);
      fputs("    System.out.println(\"Hello, World\");\n",hello);
      fputs("  }\n\n",hello);
      fputs("}\n",hello);
      fclose(hello);
      return 1;
    }else{
       perror("jtsh");
        return 1;
    }
  }
  //C
  if(strcmp(args[2], "c") == 0){
    sprintf(name, "%s.%s", args[1], args[2]);
    hello = fopen(name, "w");
    if(hello){
      fputs("#include <stdio.h>\n\n", hello);
      fputs("main()\n", hello);
      fputs("{\n", hello);
      fputs("  printf(\"Hello, World\");\n",hello);
      fputs("}\n",hello);
      fclose(hello);
      return 1;
    }else{
       perror("jtsh");
        return 1;
    }
  }
  //C#
  if(strcmp(args[2], "c#") == 0){
    sprintf(name, "%s.cs", args[1]);
    hello = fopen(name, "w");
    if(hello){
      fputs("public class ", hello);
      fputs(args[1], hello);
      fputs("\n{\n", hello);
      fputs("  public static void Main() {\n",hello);
      fputs("    System.Console.WriteLine(\"Hello, World\");\n",hello);
      fputs("  }\n\n",hello);
      fputs("}\n",hello);
      fclose(hello);
      return 1;
    }else{
       perror("jtsh");
        return 1;
    }
  }
  //c++
  if(strcmp(args[2], "c++") == 0){
    sprintf(name, "%s.cc", args[1]);
    hello = fopen(name, "w");
    if(hello){
      fputs("#include <iostream>\n\n", hello);
      fputs("int main(int argc, char **argv) {\n", hello);
      fputs("  std::cout << \"Hello, World\" << std::endl;\n", hello);
      fputs("  return 0; \n",hello);
      fputs("}\n",hello);
      fclose(hello);
      return 1;
    }else{
       perror("jtsh");
        return 1;
    }
  } 
  //Python
  if(strcmp(args[2], "python") == 0){
    sprintf(name, "%s.py", args[1]);
    hello = fopen(name, "w");
    if(hello){
      fputs("print(\"Hello, World\")\n ", hello);
      fclose(hello);
      return 1;
    }else{
       perror("jtsh");
        return 1;
    }
  }
  else{
    printf("%s is not a supported language\n", args[2]);

  }
  return 1;
}
/**
   @brief Builtin command: print help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int jtsh_help(char **args)
{
  int i;
  printf("Welcome to the JTShell. You have access to all bash commands.\n");
  printf("Additionally you have access to all the JTShell built in functions\n");
  printf("The following are built in:\n");

  for (i = 0; i < jtsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}
// lifted from http://stackoverflow.com/questions/779875/what-is-the-function-to-replace-string-in-c
char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }

    strcpy(tmp, orig);
    return result;
}
/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int jtsh_exit(char **args)
{
  free(homepage);
  return 0;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int jtsh_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("jtsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("jtsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int jtsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < jtsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return jtsh_launch(args);
}

/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *jtsh_read_line(void)
{
  char *line = NULL;
  ssize_t bufsize = 0; 
  getline(&line, &bufsize, stdin);
  return line;
}

// lifted from http://stackoverflow.com/questions/1627624/search-for-string-in-text-file-c
char * read_file_line(FILE *fin) {
    char *buffer;
    char *tmp;
    int read_chars = 0;
    int bufsize = 512;
    char *line = malloc(bufsize);

    if ( !line ) {
        return NULL;
    }

    buffer = line;

    while ( fgets(buffer, bufsize - read_chars, fin) ) {
        read_chars = strlen(line);

        if ( line[read_chars - 1] == '\n' ) {
            line[read_chars - 1] = '\0';
            return line;
        }

        else {
            bufsize = 2 * bufsize;
            tmp = realloc(line, bufsize);
            if ( tmp ) {
                line = tmp;
                buffer = line + read_chars;
            }
            else {
                free(line);
                return NULL;
            }
        }
    }
    return NULL;
}

// lifted from http://stackoverflow.com/questions/1627624/search-for-string-in-text-file-c
int jtsh_search_file(char **args)
{
  if (args[1] == NULL) {
      printf("expected a file as argument\n");
  }
  if (args[2] == NULL) {
      printf("expected a search term as second argument\n");
      return 1;
  }
  printf("searching file %s for term %s:\n", args[1], args[2]);
  printf("-------------\n\n");

  int lineCount = 1;
  int wordCount = 0;
  int currentLineCount = 0;
  FILE *fin;
  char *line;

  fin = fopen(args[1], "r");

  if (fin) {
      // lifted from http://stackoverflow.com/questions/9052490/find-the-count-of-substring-in-string
      while ((line = read_file_line(fin))) {
        char *tmp = line;

        while((tmp = strstr(tmp, args[2])))
        {
          currentLineCount++;
          wordCount++;
          tmp++;
        }

        if(currentLineCount > 0){
          printf("   (%d) matches on line %d: '%s'\n", currentLineCount, lineCount, line);
        }
        currentLineCount = 0;
        lineCount++;
      }
  }else{
     printf("Could not find file %s\n", args[1]);
     printf("\n-------------\n");
     return 1;
  }

  fclose(fin);

  printf("\n-------------\n");
  printf("search complete, found (%d) matches\n", wordCount);
  return 1;
}

//Modification of jtsh_search_file
int jtsh_replace_file(char **args)
{
  if (args[1] == NULL) {
      printf("expected a file as first argument\n");
  }
  if (args[2] == NULL) {
      printf("expected a search term as second argument\n");
  }
  if (args[3] == NULL) {
      printf("expected a replace term as third argument\n");
      return 1;
  }
  printf("searching file %s for term %s:\n", args[1], args[2]);
  printf("-------------\n\n");

  char *tempFileName = "temp";
  int lineCount = 1;
  int wordCount = 0;
  int currentLineCount = 0;
  FILE *fin;
  FILE *fout = fopen(tempFileName, "ab+");
  char *line;

  fin = fopen(args[1], "r");

  if (fin) {

      // lifted from http://stackoverflow.com/questions/9052490/find-the-count-of-substring-in-string
      while ((line = read_file_line(fin))) {
        char *tmp = line;

        while((tmp = strstr(tmp, args[2])))
        {
          currentLineCount++;
          wordCount++;
          tmp++;
        }

        if(currentLineCount > 0){
          printf("   (%d) matches on line %d: '%s'\n", currentLineCount, lineCount, line);

          tmp = str_replace(line, args[2], args[3]);
          printf("Replacement line: %s\n", tmp);
          fputs(tmp, fout);
        } else {
          fputs(line, fout);
        }

        fputs("\n", fout); // adding new lines in the file
        currentLineCount = 0;
        lineCount++;
        free(line);
      }
  }else{
     printf("Could not find file %s\n", args[1]);
     printf("\n-------------\n");
     return 1;
  }

  fclose(fin);
  fclose(fout);

  remove(args[1]);
  rename(tempFileName, args[1]);

  printf("\n-------------\n");
  printf("search complete, replaced (%d) matches\n", wordCount);
  return 1;
}

/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **jtsh_split_line(char *line)
{
  int bufsize = JTSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "jtsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, JTSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += JTSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "jtsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, JTSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void jtsh_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("<JT> ");
    line = jtsh_read_line();
    args = jtsh_split_line(line);
    status = jtsh_execute(args);

    free(line);
    free(args);
  } while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.
  FILE *config;
  config = fopen("config", "r");
  if(config){
    printf("Config file found\n");
    char line[256];
    while(fgets(line, sizeof(line), config)!=NULL) {
        char **data;
        data = jtsh_split_line(line);
        if(strcmp(data[0], "homepage")==0){
          homepage = malloc(sizeof(data[1]));
          sprintf(homepage, "%s", data[1]);
        }
        free(data);
      }
    fclose(config);
  }else{
    printf("Config file not found\n");
    homepage = "google.ca";
  }
  // Run command loop.
  jtsh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}