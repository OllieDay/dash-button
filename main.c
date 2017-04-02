#include "dash.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Called when a button push is detected
static void cb(const struct button *button);

int main(void) {
	// Initialize first button
	struct button button1 = {
		.name = "Button 1",
		.addr = {0, 0, 0, 0, 0, 0}
	};

	// Initialize second button
	struct button button2 = {
		.name = "Button 2",
		.addr = {0, 0, 0, 0, 0, 0}
	};

	// Initialize more buttons...

	// Initialize button array and calculate length
	struct button buttons[] = {button1, button2};
	size_t len = sizeof buttons / sizeof *buttons;

	// Start detecting button pushes
	if (dash_init(buttons, len, cb) != DASH_SUCCESS) {
		perror("dash_init()");
		exit(EXIT_FAILURE);
	}

	puts("Detecting button pushes...");

	// Block until there is user input
	getchar();

	// Stop detecting button pushes
	dash_free();
}

static void cb(const struct button *button) {
	printf("%s pushed!\n", button->name);

	// Switch off lights, turn heating on, send email, etc.
}
