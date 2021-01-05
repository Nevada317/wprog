#include <stdbool.h>

typedef struct {
	char* Filename;
	bool Verify;
	bool EEP;
	struct task_t* Next;
} task_t;

task_t* TASK_Create();
task_t* TASK_GetNext();
void TASK_Destroy();
