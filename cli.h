
#ifndef CLI_H_INCLUDED
#define CLI_H_INCLUDED

/*
    The main function which executes when the program begins
*/
int main();

/*
    A function called periodically where the CLI can perform logic
    - Listen to STDIN and process commands
*/
void cli_periodic();

#endif