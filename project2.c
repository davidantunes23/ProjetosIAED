/* iaed-23 - ist1107061 - project2 */

/*
 * File: project2.c
 * Author: David Antunes
 * Description: A public transportation line management system, allowing the
   creation of stops and routes, as well as their consultation and elimination.
*/

/* Libraries */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Header File */

#include "project2.h"

/* Functions */

/* commandp is a function that executes the command "p". */
void commandp(char *input, Stop **headStop, Stop **tailStop, Line **headLine) {
    int arg;
    Stop *stop = myalloc(sizeof(Stop));

    arg = inputp(input, stop);

    if (arg == 0) {
        printStops(headStop, headLine);
        free(stop);
    }
    if (arg == 1) {
        printCoords(stop, headStop);
        free(stop->name);
        free(stop);
    }
    else if (arg == 3) {
        if (checkStop(stop, headStop) == 0)
            createStop(stop, headStop, tailStop);
    }
}

/* commandc is a function that executes the command "c". */
void commandc(char *input, Line **headLine, Line **tailLine) {
    int arg;
    Line *line = myalloc(sizeof(Line));
    char inv[INV];

    arg = inputc(input, line, inv);

    if (arg == 0) {
        printLines(headLine);
        free(line);
    }
    if (arg == 1) {
        if (checkLine(line, headLine) == 0) {
            createLine(line, headLine, tailLine);
        }
        else {
        printRoute(line, headLine);
        free(line->name);
        free(line);
        }
    }
    else if (arg == 2 && checkInv(inv) == 1) {
        printRouteInv(line, headLine);
        free(line->name);
        free(line);
    }
}

/* commandl is a function that executes the command "l". */
void commandl(char *input, Stop **headStop, Line **headLine) {
    Line *line;
    Link *link = myalloc(sizeof(Link));
    char linename[MAXLEN], origin[MAXLEN], destination[MAXLEN];
    link->next = NULL;
    link->prev = NULL;

    inputl(input, link, linename, origin, destination);
    
    line = checkLink(linename, headLine);
    if (line == NULL) {
        printf("%s: no such line.\n", linename);
        free(link);
    }
    else if (checkStops(link, origin, destination, headStop) == 0) {
        createLink(line, link);
    }
    else {
        free(link);
    }
}

/* commandi is a function that executes the command "i". */
void commandi(Stop **headStop, Line **headLine) {
    Stop *tempstop = *headStop;
    char **arr;
    int i, linesNum;
    
    while (tempstop != NULL) {
        linesNum = lineCounter(tempstop, headLine);
        if (linesNum > 1) {
            arr = myalloc(linesNum * sizeof(char*));
            createLineArr(tempstop, headLine, arr);
            printf("%s %d: ", tempstop->name, linesNum);
            alphabeticSort(arr, 0, linesNum - 1);
            printArr(arr, linesNum - 1);
            for (i = 0; i < linesNum; i++) free(arr[i]);
            free(arr);
        }
        tempstop = tempstop->next;
    }
}

/* commanda is a function that executes the command "a". */
void commanda(Stop **headStop, Line **headLine) {
    deleteStops(headStop);
    deleteLines(headLine);
}

/* commandr is a function that executes the command "r". */
void commandr(char *input, Line **headLine, Line **tailLine) {
    Line *auxline = myalloc(sizeof(Line)), *line;
    inputr(input, auxline);
    line = getLine(auxline, headLine);
    if (line == NULL) {
        printf("%s: no such line.\n", auxline->name);
        return;
    }
    deleteLinks(line);
    deleteLine(line, headLine, tailLine);
    free(auxline->name);
    free(auxline);
}

/* commande is a function that executes the command "e". */
void commande(char *input, Stop **headStop, Stop **tailStop, Line **headLine) {
    char stopname[MAXLEN];
    Stop *stop;

    inpute(input, stopname);
    stop = getStop(stopname, headStop);
    if (stop == NULL) {
        printf("%s: no such stop.\n", stopname);
        return;
    }
    delStopFromLines(stop, headLine);
    deleteStop(stop, headStop, tailStop);
}

int main() {
    char input[MAXLEN];
    Stop *headStop = NULL;
    Stop *tailStop = NULL;
    Line *headLine = NULL;
    Line *tailLine = NULL;

    input[0] = 'a';

    while (input[0] != 'q')
    {
        readLine(input);
        if (input[0] == 'p') commandp(input, &headStop, &tailStop, &headLine);
        else if (input[0] == 'c') commandc(input, &headLine, &tailLine);
        else if (input[0] == 'l') commandl(input, &headStop, &headLine);
        else if (input[0] == 'i') commandi(&headStop, &headLine);
        else if (input[0] == 'a') commanda(&headStop, &headLine);
        else if (input[0] == 'r') commandr(input, &headLine, &tailLine);
        else if (input[0] == 'e') commande(input, &headStop, &tailStop, 
        &headLine);
    }
    commanda(&headStop, &headLine);
    return 0;
}
