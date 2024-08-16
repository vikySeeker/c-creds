//include gaurd
#ifndef CREDS_MODES_H
#define CREDS_MODES_H

//Mode enum used to select mode
typedef enum _Mode {
	Help,
	Save,
	Get,
	Delete,
	Interactive,
	Invalid
} Mode;

void set_mode(Mode m);
Mode get_mode();

int process_mode(char*);
#endif
