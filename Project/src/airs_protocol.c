// Module to implement the ground controller application-layer protocol.

// The protocol is fully defined in the README file. This module
// includes functions to parse and perform commands sent by an
// airplane (the docommand function), and has functions to send
// responses to ensure proper and consistent formatting of these
// messages.

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "util.h"
#include "airplane.h"
#include "airs_protocol.h"
#include "planelist.h"

/************************************************************************
 * Call this response function if a command was accepted
 */
void send_ok(airplane *plane) {
    fprintf(plane->fp_send, "OK\n");
}

/************************************************************************
 * Call this response function if an error can be described by a simple
 * string.
 */
void send_err(airplane *plane, char *desc) {
    fprintf(plane->fp_send, "ERR %s\n", desc);
}

/************************************************************************
 * Call this response function if you want to embed a specific string
 * argument (sarg) into an error reply (which is now a format string).
 */
void send_err_sarg(airplane *plane, char *fmtstring, char *sarg) {
    fprintf(plane->fp_send, "ERR ");
    fprintf(plane->fp_send, fmtstring, sarg);
    fprintf(plane->fp_send, "\n");
}

/************************************************************************
 * Handle the "REG" command.
 */
static void cmd_reg(airplane *plane, char *rest) {
    if (rest == NULL) {
        send_err(plane, "Missing plane ID");
        return;
    }
    
    // Check that plane ID is not too long
    if (PLANE_UNREG == plane->state) {
        if (strlen(rest) > PLANE_MAXID) {
            send_err(plane, "Plane ID too long");
            return;
        }

        // Check that plane ID is alphanumeric
        for (size_t i = 0; i < strlen(rest); i++) {
            if (!isalnum(rest[i])) {
                send_err(plane, "Plane ID must be alphanumeric");
                return;
            }
        }

        // Check if flight ID is already taken
        if (is_flightid_taken(rest)) {
            send_err(plane, "Flight ID is already taken");
            return;
        }

        // Register the plane
        strncpy(plane->id, rest, PLANE_MAXID);
        plane->id[PLANE_MAXID] = '\0';
        plane->state = PLANE_ATTERMINAL;
        send_ok(plane);
    } else {
        send_err(plane, "Plane already registered");
    }
}

/************************************************************************
 * Handle the "REQTAXI" command.
 */
static void cmd_reqtaxi(airplane *plane, char *rest) {
    if (PLANE_UNREG == plane->state) {
        send_err(plane, "Plane must register first");
        return;
    }
    
    send_err(plane, "Command not implemented yet");
}

/************************************************************************
 * Handle the "BYE" command.
 */
static void cmd_bye(airplane *plane, char *rest) {
    plane->state = PLANE_DONE;
    send_ok(plane);
}


/************************************************************************
 * Handle the "REQPOS" command.
 */
static void cmd_reqpos(airplane *plane, char *rest) {
    if (PLANE_UNREG == plane->state) {
        send_err(plane, "Plane must register first");
        return;
    }
    
    send_err(plane, "Command not implemented yet");
}

/************************************************************************
 * Handle the "REQAHEAD" command.
 */
static void cmd_reqahead(airplane *plane, char *rest) {
    if (PLANE_UNREG == plane->state) {
        send_err(plane, "Plane must register first");
        return;
    }
    
    send_err(plane, "Command not implemented yet");
}

/************************************************************************
 * Handle the "INAIR" command.
 */
static void cmd_inair(airplane *plane, char *rest) {
    if (PLANE_UNREG == plane->state) {
        send_err(plane, "Plane must register first");
        return;
    }
    
    send_err(plane, "Command not implemented yet");
}

/************************************************************************
 * Parses and performs the actions in the line of text (command and
 * optionally arguments) passed in as "command".
 */
void docommand(airplane *plane, char *command) {
    char *saveptr;
    char *cmd = strtok_r(command, " \t\r\n", &saveptr);
    if (cmd == NULL) {  // Empty line (no command) -- just ignore line
        return;
    }

    // Get arguments (everything after command, trimmed)
    char *args = strtok_r(NULL, "\r\n", &saveptr);
    if (args != NULL) {
        args = trim(args);
    }

    // Dispatch to appropriate command handler
    if (strcmp(cmd, "REG") == 0) {
        cmd_reg(plane, args);
    } else if (strcmp(cmd, "REQTAXI") == 0) {
        cmd_reqtaxi(plane, args);
    } else if (strcmp(cmd, "BYE") == 0) {
        cmd_bye(plane, args);
    } else if (strcmp(cmd, "REQPOS") == 0) {
        cmd_reqpos(plane, args);
    } else if (strcmp(cmd, "REQAHEAD") == 0) {
        cmd_reqahead(plane, args);
    } else if (strcmp(cmd, "INAIR") == 0) {
        cmd_inair(plane, args);
    } else {
        send_err(plane, "Unknown command");
    }
}
