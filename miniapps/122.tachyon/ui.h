/*
 * ui.h - defines for user interface functions
 *
 *  $Id: ui.h 499 2006-10-06 19:58:28Z keeper $
 */

/* Different types of message, for levels of verbosity etc */
#define MSG_0      100
#define MSG_1      101
#define MSG_2      102
#define MSG_3      103
#define MSG_4      104
#define MSG_5      105
#define MSG_ERR    200
#define MSG_ABORT  300

void rt_ui_message(int, char *);
void rt_ui_progress(int);
int  rt_ui_checkaction(void);

