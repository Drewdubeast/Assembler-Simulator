#include <stdio.h>
#include <string.h>

int main() {

    char s[] = "a b c";

    split(s);
    /*
    char lines[100][100];
    int i = 0;

    FILE *file;
    file = fopen("run.a", "r");
    //char *s = "a b c";
    //takes the lines from the file and puts it into 'lines' array
    while(fgets(lines[i], 100,  file)) {
        split(lines[i]);
        i++;
    }*/
}

void split(char s[]) {
  char *token;
  int i = 0;

  //replace all tabs with spaces
  while(s[i] != '\0') {
    if(s[i]=='\t') {
        s[i] = ' ';
    }
    i++;
  }

  token = strtok (s," ");
  while (token != NULL)
  {
    printf("%s\n", token);
    token = strtok (NULL, " ");
  }
  return 0;
}
