#include <stdio.h>
#include <string.h>

int main() {

    String hi = "hi";

    printf("%s",hi);

    char lines[100][100];
    int i = 0;

    FILE *file;
    file = fopen("run.a", "r");

    printf("1");
    char *s = "a b c";
    split(s);
    printf("2");
    //takes the lines from the file and puts it into 'lines' array
    while(fgets(lines[i], 100,  file)) {
        printf("%s", lines[i]);
        split(lines[i]);
        i++;
    }
}

void split(char s[]) {
  char *token;
  int i = 0;

  //replace all tabs with spaces
  while(s[i] != '\0') {
    if(s[i]=='\t') {    printf("test");
        s[i] = ' ';
    }
    i++;
  }

  token = strtok (s," ");
  while (token != NULL)
  {
    printf("test");
    token = strtok (NULL, " ");
  }
  return 0;
}
