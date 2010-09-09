/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       cmd.h
**  Content:    structures and functions for commands.
*/

/* Commands are in the form of REQUESTS (Ant to Server) and RESPONSES (Server to
** Ant), as well as NOTIFICATIONS (Server to other Ants) when applicable. */

#ifndef __CMD_H__
#define __CMD_H__

enum {
/* Command type constants: */
    CMD_START,
    CMD_TURN,
    
    CMD_MOVE_REQ,
    CMD_MOVE_RES,
    
    CMD_SMELL_REQ,
    CMD_SMELL_RES,
    
    CMD_PICK_REQ,
    CMD_PICK_RES,
    
    CMD_AID_REQ,
    CMD_AID_RES,
    
    CMD_YELL_REQ,
    CMD_YELL_RES,
    CMD_YELL_NOT,
    
    CMD_STOP
};

#define NUM_CMDS (CMD_STOP + 1)

enum {
/* Relative direction constants: */
    DIR_NORTH,
    DIR_NORTHEAST,
    DIR_EAST,
    DIR_SOUTHEAST,
    DIR_SOUTH,
    DIR_SOUTHWEST,
    DIR_WEST,
    DIR_NORTHWEST
};

#define NUM_DIRS (DIR_NORTHWEST + 1)

struct cmd_t {
/* Generic command structure, for casts. All structures have the same size: */
    int type;
};

/* TYPE DEFINITIONS: */

typedef struct cmd_t* cmd_t;
typedef struct cmd_start_t* cmd_start_t;

typedef struct cmd_move_req_t* cmd_move_req_t;
typedef struct cmd_move_res_t* cmd_move_req_t;

typedef struct cmd_smell_req_t* cmd_smell_req_t;
typedef struct cmd_smell_res_t* cmd_smell_req_t;

typedef struct cmd_pick_req_t* cmd_pick_req_t;
typedef struct cmd_pick_res_t* cmd_pick_req_t;

typedef struct cmd_aid_req_t* cmd_aid_req_t;
typedef struct cmd_aid_res_t* cmd_aid_req_t;

typedef struct cmd_yell_req_t* cmd_yell_req_t;
typedef struct cmd_yell_res_t* cmd_yell_req_t;

typedef struct cmd_stop_t* cmd_stop_t;
typedef struct cmd_no_t* cmd_no_t;

/* COMMAND-SPECIFIC STRUCTURES: */

struct cmd_start_t {
/* START command: Control tells an Ant the simulation has begun */
    int type;
};

struct cmd_turn_t {
/* TURN command: Control tells an Ant it can attempt its move */
    int type;
};

struct cmd_move_req_t {
/* MOVE REQUEST: Ant tells Control it wants to move in a certain direction. */
    int type;
    int dir;
};

struct cmd_move_res_t {
/* MOVE RESPONSE: Control tells Ant whether its move was accepted. */
    int type;
    int status;
};

struct cmd_smell_req_t {
/* SMELL REQUEST: Ant tells Control it wants to sniff around. */
    int type;
};

struct cmd_smell_res_t {
/* SMELL RESPONSE: Control tells an Ant what's in adjacent tiles. */
    int type;
    char objects[3][3];
};

/* PICK REQUEST: Ant tells Control it wants to PICK in a certain direction. */
struct cmd_pick_req_t {
    int type;
    int dir;
};

/* PICK RESPONSE: Control tells Ant whether it could pick up the food. */
struct cmd_pick_res_t {
    int type;
    int status;
};

/* AID REQUEST: Ant tells Control it wants to AID in a certain direction. */
struct cmd_aid_req_t {
    int type;
    int dir;
};

/* AID RESPONSE: Control tells Ant whether it could aid picking up the food. */
struct cmd_aid_res_t {
    int type;
    int status;
};

/* YELL REQUEST: Ant tells Control it wants to yell */
struct cmd_yell_req_t {
    int type;
};

/* YELL RESPONSE: Control tells Ant whether it could yell. This implementation
   does not support sore throats, so YELL always succeeds. */
struct cmd_yell_res_t {
    int type;
};

/* YELL NOTIFICATION: Control tells all the other Ants about the yell. */
struct cmd_yell_not_t {
    int type;
    int x, y;
};

struct cmd_stop_t {
/* STOP command: Control tells an Ant the simulation has ended */
    int type;
};

/* NO command: general purpose command.
** Control tells an Ant its request was NOT accepted or processed. */
struct cmd_no_t {
    int type;
    int reason;
};

#endif
