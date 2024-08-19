/* iaed-23 - ist1107061 - project2 */

/*
 * File: project2.h
 * Author: David Antunes
 * Description: Header file of project2.c.
*/

/* Constant Definitions */

#define MAXLEN 65535

#define NO 0
#define YES 1

#define INV 8

/* Struct Definitions */

typedef struct stop {
    char *name;
    double latitude;
    double longitude;
    struct stop *next;
} Stop;

typedef struct link {
    Stop *origin;
    Stop *destination;
    double cost;
    double duration;
    struct link *next;
    struct link *prev;
} Link;

typedef struct line {
    char *name;
    int stopnum;
    double cost;
    double duration;
    Link *headLink;
    Link *tailLink;
    struct line *next;
} Line;

/* Function Declaration */

/* general */
void* myalloc(int size);

/* p */
int inputp(char *input, Stop *stop);
void printStops(Stop **headStop, Line **headLine);
void printCoords(Stop *stop, Stop **headStop);
int checkStop(Stop *stop, Stop **headStop);
void createStop(Stop *stop, Stop **headStop, Stop **tailStop);

/*c*/
int inputc(char *input, Line *line, char *inv);
void printLines(Line **headLine);
int checkLine(Line *line, Line **headLine);
void createLine(Line *line, Line **headLine, Line **tailLine);
void printRoute(Line *linename, Line **headLine);
int checkInv(char str[]);
void printRouteInv(Line *linename, Line **headLine);

/* l */
void inputl(char *input, Link *link, char *line, char *orig, char *dest);
void* checkLink(char *linename, Line **headLine);
int checkStops(Link *plink, char *origin, char *destination, Stop **headStop);
void createLink(Line *line, Link *plink);

/* i */
int lineCounter(Stop *stop, Line **headLine);
void createLineArr(Stop *stop, Line **headLine, char **arr);
void alphabeticSort(char **arr, int l, int r);
void printArr(char **arr, int len);

/* a */
void deleteStops(Stop **headStop);
void deleteLines(Line **headLine);

/* r */
void inputr(char *input, Line *line);
void* getLine(Line *line, Line **headLine);
void deleteLinks(Line *line);
void deleteLine(Line *line, Line **headLine, Line **tailLine);

/* e */
void inpute(char *input, char *stopname);
void* getStop(char *stopname, Stop **headStop);
void delStopFromLines(Stop *stop, Line **headLine);
void deleteStop(Stop *stop, Stop **headStop, Stop **tailStop);

/* main */
int readLine(char s[]);