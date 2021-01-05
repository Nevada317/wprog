#include "task.h"
#include <stddef.h>
#include <stdlib.h>


static task_t* CurrentTask;

task_t* TASK_Create() {
	task_t* ptr;
	if (CurrentTask == NULL) {
		CurrentTask = malloc(sizeof(task_t));
		ptr = CurrentTask;
	} else {
		ptr = CurrentTask;
		while ((ptr->Next) != NULL) {
			ptr = (task_t*) ptr->Next;
		}
		ptr->Next = malloc(sizeof(task_t));
		ptr = (task_t*) ptr->Next;
	}
	ptr->Filename = NULL;
	ptr->Next = NULL;
	ptr->Action = Action_None;
	return ptr;
}

task_t* TASK_GetNext() {
	return CurrentTask;
}

void TASK_Destroy() {
	task_t* ptr;
	ptr = (task_t*) CurrentTask->Next;

	if (CurrentTask->Filename)
		free(CurrentTask->Filename);

	free(CurrentTask);
	CurrentTask = ptr;
}
