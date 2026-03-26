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
    // TODO: Handle the "REG" command
    if (rest == NULL) {
        send_err(plane, "Missing plane ID");
        return;
    }

    if (strlen(rest) > PLANE_MAXID) {
        send_err(plane, "Plane ID too long");
        return;
    }

    for (size_t i = 0; i < strlen(rest); i++) {
        if (!isalnum(rest[i])) {
            send_err(plane, "Plane ID must be alphanumeric");
            return;
        }
    }
    strncpy(plane->id, rest, PLANE_MAXID);
    plane->id[PLANE_MAXID] = '\0';
    send_ok(plane);
}

/************************************************************************
 * Handle the "REQTAXI" command.
 */
static void cmd_reqtaxi(airplane *plane, char *rest) {
    // TODO for part 1: Add state check (plane must be registered)
    if (PLANE_ATTERMINAL != plane->state) {
        send_err(plane, "Plane not at terminal");
        return;
    } else {
        send_ok(plane);
    }
}

/************************************************************************
 * Handle the "BYE" command.
 */
static void cmd_bye(airplane *plane, char *rest) {
    // TODO: Handle "BYE" command
    plane->state = PLANE_DONE;
}


/************************************************************************
 * Handle the "REQPOS" command.
 */
static void cmd_reqpos(airplane *plane, char *rest) {
    // TODO: Handle "REQPOS" command
    if (PLANE_ATTERMINAL == plane->state) {
        fprintf(plane->fp_send, "POS %s\n", "ATTERMINAL");
    } else if (PLANE_TAXIING == plane->state) {
        fprintf(plane->fp_send, "POS %s\n", "TAXIING");
    } else if (PLANE_INAIR == plane->state) {
        fprintf(plane->fp_send, "POS %s\n", "INAIR");
    } else {
        send_err(plane, "Unknown plane state");
    }
}

/************************************************************************
 * Handle the "REQAHEAD" command.
 */
static void cmd_reqahead(airplane *plane, char *rest) {
    // TODO: Handle "REQAHEAD" command
    for (size_t i = 0; i < strlen(rest); i++) {
        if (!isalnum(rest[i])) {
            send_err(plane, "Plane ID must be alphanumeric");
            return;
        }
    }
    fprintf(plane->fp_send, "AHEAD %s\n", rest);
}

/************************************************************************
 * Handle the "INAIR" command.
 */
static void cmd_inair(airplane *plane, char *rest) {
    // TODO: Handle "INAIR" command
    if (PLANE_INAIR != plane->state) {
        send_err(plane, "Plane not in air");
        return;
    } else {
        send_ok(plane);
    }
}

/************************************************************************
 * Handle the "TAKEOFF" command.
 */
static void cmd_takeoff(airplane *plane, char *rest) {
    //  TODO: Handle "TAKEOFF" command
    if (PLANE_CLEAR != plane->state) {
        send_err(plane, "Plane not cleared to fly");
        return;
    } else {
        plane->state = PLANE_INAIR;
        send_ok(plane);
    }
}

/************************************************************************
 * Handle the "NOTICE" command.
 */
static void cmd_notice(airplane *plane, char *rest) {
    //  TODO: Handle "NOTICE" command
    fprintf(plane->fp_send, "NOTICE %s\n", rest);
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

    // TODO: Only some commands are recognized below. Must include all
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
    } else  if (strcmp(cmd, "TAKEOFF") == 0) {
        cmd_takeoff(plane, args);
    } else if (strcmp(cmd, "NOTICE") == 0) {
        cmd_notice(plane, args);
    } else {
        send_err(plane, "Unknown command");
    }
}
