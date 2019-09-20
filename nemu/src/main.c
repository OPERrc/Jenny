#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "nemu.h"
int init_monitor(int, char *[]);
void ui_mainloop(int);
uint32_t expr(char *, bool *);

uint32_t str2num(char *num) {
	int point = 0;
	uint32_t result = 0;
	while (num[point]) {
		result = result * 10 + num[point] - '0';
		point++;
	}	
	return result;
}

void value_test() {
	FILE *fp = fopen("~/ics2019/nemu/tools/gen-expr/input", "r");
	assert(fp != NULL);
  char *line_read = readline("");
	while (line_read != NULL) {
		char *num = strtok(line_read, " ");
    uint32_t key = str2num(num);

		line_read = NULL;
		char *EXPR = strtok(line_read, " ");
		bool flag = true;
		bool *success = &flag;
		uint32_t answer = expr(EXPR, success);
		printf("key = %d, answer = %d", key, answer);
		if (key == answer) 
			printf("  correct!\n");
		else
			printf("  wrong!\n");

		line_read = readline("");
	}
	fclose(fp);
}

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);
	
	value_test();

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
