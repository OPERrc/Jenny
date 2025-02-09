#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
void isa_reg_display(void);
uint32_t paddr_read(paddr_t, int);
uint32_t expr(char *, bool *);
WP *new_wp();
void free_wp(WP *wp);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

// Insert part
static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static WP *wp_head = NULL;

bool check_wp();
// End of Insert part

static int cmd_help(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  // Insert part
  { "si", "With 1 argument [N]. Let the program run N steps then stop. If N is not given it is set to default value '1'.", cmd_si },
  { "info", "With 1 argument 'r' or 'w'. 'r' print the register information, 'w' print the watchpoint information.", cmd_info},
  { "x", "With 2 arguments [N] and [EXPR]. From the address of [EXPR], print continuous [N] lines of 4 bytes.", cmd_x},
	{ "p", "With 1 argument [EXPR]. Calculate the value of the [EXPR].", cmd_p},
	{ "w", "With 1 argument [EXPR]. Watch the value of the [EXPR]. If the value changes, suspend the program.",  cmd_w},
	{ "d", "With 1 argument [N]. Delete the NO [N] watch point.", cmd_d},
	// End of Insert part

  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

// Insert part
bool check_wp() {
	bool change = false;
	for (WP *p = wp_head; p != NULL; p = p->next) {
		bool flag = true;
		bool *success = &flag;
		uint32_t value = expr(p->EXPR, success);
		if (!*success)
			assert(0);
		if (value != p->value) {
			printf("Watchpoint %d: %s\n", p->NO, p->EXPR);
			printf("Old value = %d\n", p->value);
			printf("New value = %d\n", value);
			p->value = value;
			change = true;
		}
	}
	return change;
}


inline bool is_num(char ch) {
  if (ch >= '0' && ch <= '9')
	return true;
  else
	return false;
}

static int cmd_si(char *args) {
  char *numstr = strtok(args, " ");
  args = NULL;
  char *EMPTY = strtok(args, " ");
  if (numstr == NULL)
    cpu_exec(1);
  else if (EMPTY != NULL) {
    printf("Too many arguments! Need only 1 non-negative integer.\n");
	return 0;
  }
  else {
	uint64_t num = 0, point = 0;

	while (numstr[point]) {
	  if (is_num(numstr[point]))
	    num = num * 10 + numstr[point] - '0';
	  else {
		printf("Arguments input error! Need 1 non-negative integer.\n");
		return 0;
		}
	  point = point + 1;
	}

	cpu_exec(num);
  }
  return 0; 
}

static int cmd_info(char *args) {
  char *op = strtok(args, " ");
  if (op == NULL) {
    printf("Lack of arguments! Need 1 'r' or 'w' argument.\n");
	return 0;
  }
  else {
	args = NULL;
	char *EMPTY = strtok(args, " ");
	if (EMPTY != NULL) {
	  printf("Too many arguments! Need 1 'r' or 'w' argument.\n");
	  return 0;
	}
    if (strcmp(op, "r") == 0) {
	  isa_reg_display();
	  return 0;
	}	
	if (strcmp(op, "w") == 0) {
		if (wp_head == NULL) {
			printf("No watchpoints currently.\n");
			return 0;
		}
		printf("Num     Type          What\n");
		for (WP *p = wp_head; p != NULL; p = p->next)
			printf("%-8dwatchpoints   %s\n", p->NO, p->EXPR);
	  return 0;
	}
	printf("Arguments input error! Need 1 'r' or 'w' argument.\n");
    return 0;
  }
}

static int cmd_x(char *args) {
  // Get the first argument N
  char *N = strtok(args, " ");
  if (N == NULL) {
    printf("Lack of arguments! Need 2 arguments: 'N', 'EXPR'.\n");
	return 0;
  }
  
  // Get the second argument EXPR
  args = NULL;
  char *EXPR = strtok(args, " ");
  if (EXPR == NULL) {
    printf("Lack of arguments! Need 2 arguments: 'N', 'EXPR'.\n");
	return 0;
  }

  // Get the remaining string
  args = NULL;
  char *EMPTY = strtok(args, " ");
  if (EMPTY != NULL) {
    printf("Too many arguments! Need 2 arguments: 'N', 'EXPR'.\n");
	return 0;
  }

  // Get the number from N
  int point = 0, num = 0;
  while (N[point]) {
    if (is_num(N[point])) {
	  num = num * 10 + N[point] - '0';
	  point++;
	}
    else {
	  printf("Arguments input error! The first argument 'N' should be a non-negative integer!\n");
	  return 0;
	}	
  }

  // Get the address from EXPR, the result of EXPR should be a 16bit integer
  bool flag = true;
	bool *success = &flag;
	paddr_t result = expr(EXPR, success);
	if (!success) {
		printf("The second argument [EXPR] input error!\n");
		return 0;
	}

  for (int i=0; i<num; i++)
    printf("0x%-12x: 0x%x\n", result+i, paddr_read(result+i, 1));   
  
  return 0;
}

static int cmd_p(char *args) {
	bool flag = true;
	bool *success = &flag;
	uint32_t result = expr(args, success);
	if (!*success) {
		printf("EXPR input error!\n");
		return 0;
	}
	else {
	  printf("%d\n", result);
	  return 0;
	}
} 

static int cmd_w(char *args) {
	bool flag = true;
	bool *success = &flag;
	uint32_t value = expr(args, success);
	if (*success) {
		WP *p = new_wp();
		strcpy(p->EXPR, args);
		p->value = value;
		p->next = wp_head;
		wp_head = p;
		printf("Watchpoint %d (%s) created.\n", p->NO, p->EXPR);
	}
	else {
		printf("Argument [EXPR] input error!\n");
		return 0;
	}
	return 0;
}

static int cmd_d(char *args) {
	char *N = strtok(args, " ");
	if (N == NULL) {
		printf("Lack of arguments! Need 1 argument [N].\n");
		return 0;
	}
	
	args = NULL;
	char *EMPTY = strtok(args, " ");
	if (EMPTY != NULL) {
		printf("Too many arguments! Need only 1 argument [N].\n");
		return 0;
	}

	int point = 0;
	uint32_t num = 0;
	while (N[point])
		if (is_num(N[point])) {
			num = num * 10 + N[point] - '0';
			point++;
		}
		else {
			printf("Arguments input error! [N] should be an integer!\n");
			return 0;
		}
	
	if (wp_head->NO == num) {
	  WP *q = wp_head;
		wp_head = wp_head->next;
		printf("Watchpoint %d (%s) deleted.\n", q->NO, q->EXPR);
		free_wp(q);
		return 0;
	}
	else
	for (WP *p = wp_head; p->next != NULL; p = p->next)
		if (p->next->NO == num) {	
			WP *q = p->next;
			p->next = p->next->next;
			printf("Watchpoint %d (%s) deleted.\n", q->NO, q->EXPR);
			free_wp(q);
			return 0;
		}

	printf("Watchpoint %d is currently free.\n", num);
	return 0;
}
// End of Insert part

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
