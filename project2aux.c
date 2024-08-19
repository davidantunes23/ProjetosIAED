/* iaed-23 - ist1107061 - project2 */

/*
 * File: project2aux.c
 * Author: David Antunes
 * Description: A program that contains the auxiliary functions of project2.c .
*/

/* Libraries */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Header File */

#include "project2.h"

/* Functions */

/* allocates memory. If there's no memory available prints a message and ends 
the program. */
void* myalloc(int size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        printf("No memory.\n");
        exit(2);
    }
    else return ptr;
}

/* reads a line from stdin. */
int readLine(char s[]) {
    int c, i = 0;
    while ((c = getchar()) != '\n' && c != EOF)
        s[i++] = c;
    s[i] = '\0';
    return i;
}

/* returns the number of quotation marks in the input. */
int numQuotes(char *input) {
    int num = 0, i = 0;
    while (input[i] != '\0') {
        if (input[i] == '"') num++;
        i++; 
    }
    return num;
}

/* returns the number of spaces before a quotation mark in the input.  */
int spaceCounter(char *input) {
    int num = 0, i = 0;
    while (input[i] != '\0') {
        if (input[i] == ' ' || input[i] == '\t') num++;
        else if (input[i] == '"') break;
        i++;
    }
    return num;
}

/* reads the arguments of command "p" and assigns them to their respective
variables. Returns the number of arguments read. */
int inputp(char *input, Stop *stop) {
    int arg = 0;
    char stopname[MAXLEN];
    
    if (numQuotes(input) == 0) 
        arg = sscanf(input, "%*c %s %lf %lf", stopname, &(stop->latitude), 
            &(stop->longitude));
    
    else arg = sscanf(input, "%*c \"%[^\"]\" %lf %lf", stopname, 
        &(stop->latitude), &(stop->longitude));

    if (arg == 1 || arg == 3)
    {
        stop->name = myalloc((strlen(stopname)+1) * sizeof(char));
        strcpy(stop->name, stopname);
        stop->next = NULL;
    }
    if (arg == -1) arg = 0;
    return arg;
}

/* checks if two stops have the same name. */
int stopcmp(Stop stop1, Stop stop2) {
    int res = strcmp(stop1.name, stop2.name);
    return res;
}

/* checks if a stop already exists. */
int checkStop(Stop *stop, Stop **headStop) {
    Stop *temp = *headStop;
    while (temp != NULL) {
        if (stopcmp(*temp, *stop) == 0) {
            printf("%s: stop already exists.\n", stop->name);
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

/* creates a new stop. */
void createStop(Stop *stop, Stop **headStop, Stop **tailStop) {
    if (*headStop == NULL) {
        *headStop = stop;
        *tailStop = stop;
        return;
    }
    (*tailStop)->next = stop;
    *tailStop = stop;
}

/* prints the latitude and longitude of a given stop. */
void printCoords(Stop *stop, Stop **headStop) {
    Stop *temp = *headStop;
    while (temp != NULL) {
        if (stopcmp(*temp, *stop) == 0) {
            printf("%16.12f %16.12f\n", temp->latitude, temp->longitude);
            return;
        }
        else
            temp = temp->next;
    }
    printf("%s: no such stop.\n", stop->name);
}

/* returns the number of lines that go through a certain stop. */
int lineCounter(Stop *stop, Line **headLine) {
    Line *templine = *headLine;
    Link *templink;
    int linesNum = 0;
    while (templine != NULL) {
        templink = templine->headLink;
        while (templink != NULL) {
            if (templink->origin == stop || templink->destination == stop) {
                linesNum++;
                break;
            }
            templink = templink->next;
        }
        templine = templine->next;
    }
    return linesNum;
}

/* prints stop name, latitude, longitude and number of lines that go through
that stop, for every stop. */
void printStops(Stop **headStop, Line **headLine) {
    Stop *temp = *headStop;
    int linesNum;
    while (temp != NULL) {
        linesNum = lineCounter(temp, headLine);
        printf("%s: %16.12f %16.12f %d\n", temp->name, temp->latitude, 
            temp->longitude, linesNum);
        temp = temp->next;
    }
}

/* reads the arguments of command "c" and assigns them to their respective
variables. Returns the number of arguments read. */
int inputc(char *input, Line *line, char *inv) {
    int arg = 0;
    char linename[MAXLEN];
    line->cost = 0;
    line->duration = 0;
    line->stopnum = 0;
    line->next = NULL;
    line->headLink = NULL;
    line->tailLink = NULL;
    
    arg = sscanf(input, "%*c %s %s", linename, inv);

    if (arg == 1 || arg == 2)
    {
        line->name = myalloc((strlen(linename) + 1) * sizeof(char));
        strcpy(line->name, linename);
    }
    if (arg == -1) arg = 0;
    return arg;
}

/* checks if two lines have the same name. */
int linecmp(Line line1, Line line2) {
    int res = strcmp(line1.name, line2.name);
    return res;
}

/* checks if the given line has already been created. */
int checkLine(Line *line, Line **headLine) {
    Line *temp = *headLine;

    while (temp != NULL) {
        if (linecmp(*line, *temp) == 0) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

/* creates a new line. */
void createLine(Line *line, Line **headLine, Line **tailLine) {
    if (*headLine == NULL) {
        *headLine = line;
        *tailLine = line;
        return;
    }
    (*tailLine)->next = line;
    *tailLine = line;
}

/* prints line name, origin stop, destination stop, number of stops, total cost 
and total duration, for every line, by order of creation. Origin and destination 
are ommitted for lines without links. */
void printLines(Line **headLine) {
    Line *temp = *headLine;
    while (temp != NULL) {
        if (temp->stopnum != 0) {
            printf("%s %s %s %d %.2f %.2f\n", temp->name, 
                temp->headLink->origin->name, temp->tailLink->destination->name, 
                temp->stopnum, temp->cost, temp->duration);
        }
        else {
            printf("%s %d %.2f %.2f\n", temp->name, temp->stopnum, temp->cost,
                   temp->duration);
        }
        temp = temp->next;
    }
}

/* receives a line with only the line name, and returns the corresponding line 
from the list of lines. Returns NULL if a line with that name doesn't exist. */
void* getLine(Line *line, Line **headLine) {
    Line *temp = *headLine;
    while (temp != NULL) {
        if (linecmp(*temp, *line) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

/* prints the route of the given line. */
void printRoute(Line *linename, Line **headLine) {
    Line *line = getLine(linename, headLine);
    Link *temp = line->headLink;
    if (line->stopnum > 0) {    
        while (temp != NULL) {
            printf("%s, ", temp->origin->name);
            temp = temp->next;
        }
        printf("%s\n", line->tailLink->destination->name);
    }
}

/* prints the route of the given line in reverse order. */
void printRouteInv(Line *linename, Line **headLine) {
    Line *line = getLine(linename, headLine);
    Link *temp = line->tailLink;
    if (line->stopnum > 0) {
        while (temp != NULL) {
            printf("%s, ", temp->destination->name);
            temp = temp->prev;
        }
        printf("%s\n", line->headLink->origin->name);
    }
}

/* checks if a string is "inverso" or one of its abbreviations, up to 3 
letters */
int checkInv(char str[]) {

    if (strcmp(str, "inverso") == 0 || strcmp(str, "invers") == 0 
        || strcmp(str, "inver") == 0 || strcmp(str, "inve") == 0 
        || strcmp(str, "inv") == 0) return 1;
    
    else {
        printf("incorrect sort option.\n");
        return 0;
    }
}

/* reads the arguments of command "l" and assigns them to their respective
variables. */
void inputl(char *input, Link *link, char *line, char *orig, char *dest) {

    if (numQuotes(input) == 0) {
        sscanf(input, "%*c %s %s %s %lf %lf", line, orig, dest, &(link->cost), 
            &(link->duration));
    }
    else if (numQuotes(input) == 4) {
        sscanf(input, "%*c %s \"%[^\"]\" \"%[^\"]\" %lf %lf", line, orig, dest, 
            &(link->cost), &(link->duration));
    }
    else if (spaceCounter(input) == 2) {
        sscanf(input, "%*c %s \"%[^\"]\" %s %lf %lf", line, orig, dest, 
            &(link->cost), &(link->duration));
    }
    else sscanf(input, "%*c %s %s \"%[^\"]\" %lf %lf", line, orig, dest, 
            &(link->cost), &(link->duration));
}

/* checks if the origin stop and destination stop of a link exists. */
int checkStops(Link *plink, char *origin, char *destination, Stop **headStop) {
    Stop *temp = *headStop;
    int origCheck = NO, destCheck = NO;
    while (temp != NULL) {
        if (origCheck == YES && destCheck == YES) break;
        if (strcmp(origin, temp->name) == 0) {
            plink->origin = temp;
            origCheck = YES;
        }
        if (strcmp(destination, temp->name) == 0) {
            plink->destination = temp;
            destCheck = YES;
        }
        temp = temp->next;
    }
    if ((origCheck) == NO) {
        printf("%s: no such stop.\n", origin);
        return 1;
    }
    else if ((destCheck) == NO) {
        printf("%s: no such stop.\n", destination);
        return 1;
    }
    return 0;
}

/* adds the cost and duration of a link to the total cost and duration of the 
line, and adds 1 to the number of stops that the line goes through. */
void addToLine(Line *line, Link *link) {
    line->cost += link->cost;
    line->duration += link->duration;
    line->stopnum += 1;
}

/* creates a new link. */
void createLink(Line *line, Link *plink) {
    if (plink->cost < 0 || plink->duration < 0) {
        printf("negative cost or duration.\n");
        return;
    }
    if (line->headLink == NULL) {
        line->headLink = plink;
        line->tailLink = line->headLink;
        line->stopnum += 1;
        addToLine(line, plink);
    }
    else if (line->tailLink->destination == plink->origin) {
        (line->tailLink)->next = plink;
        plink->prev = line->tailLink;
        line->tailLink = plink;
        addToLine(line, plink);
    }
    else if (line->headLink->origin == plink->destination) {
        plink->next = line->headLink;
        line->headLink->prev = plink;
        line->headLink = plink;
        addToLine(line, plink);
    }
    else printf("link cannot be associated with bus line.\n");
}

/* checks if the line of a link exists. */
void* checkLink(char *linename, Line **headLine) {
    Line *temp = *headLine;

    while (temp != NULL)
    {
        if (strcmp(linename, temp->name) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

/* creates an array with the names of all the lines that go through the given 
stop. */
void createLineArr(Stop *stop, Line **headLine, char **arr) {
    int i = 0;
    Line *templine = *headLine;
    Link *templink;
        
    while (templine != NULL) {
        templink = templine->headLink;
        while (templink != NULL) {
            if (templink->origin == stop || templink->destination == stop) {
                arr[i] = myalloc(BUFSIZ*sizeof(char));
                strcpy(arr[i], templine->name);
                i++;
                break;
            }
            templink = templink->next;
        }
        templine = templine->next;
    }
}

/* alphabeticSort sorts an array of line names by alphabetic order */
void alphabeticSort(char **arr, int l, int r) {
    
    int i, j, done;
    char aux[BUFSIZ];
    
    for (i = l; i < r; i++) {
        done = 1;
        for (j = r; j > i; j--)
            if (strcmp(arr[j], arr[j-1]) < 0) {
                strcpy(aux, arr[j]); 
                strcpy(arr[j], arr[j-1]); 
                strcpy(arr[j-1], aux);
                done = 0;
            }
        if (done) break;
    }
}

/* prints an array of line names. */
void printArr(char **arr, int len) {
    int i;
    for (i = 0; i < len; i++) {
        printf("%s ", arr[i]);
    }
    printf("%s\n", arr[len]);
}

/* deletes all stops. */
void deleteStops(Stop **headStop) {
    Stop *temp, *prev = *headStop;
    if (*headStop != NULL) {
        temp = (*headStop)->next;
        while (temp != NULL) {
            free(prev->name);
            free(prev);
            prev = temp;
            temp = temp->next;
        }
        free(prev->name);
        free(prev);
        *headStop = NULL;
    }
}

/* deletes all links from the given line. */
void deleteLinks(Line *line) {
    Link *temp;
    if (line->headLink != NULL) {
        temp = line->headLink->next;
        while (temp != NULL) {
            free(temp->prev);
            temp = temp->next;
        }
        free(line->tailLink);
        line->headLink = NULL;
    }
}

/* deletes all lines.  */
void deleteLines(Line **headLine) {
    Line *temp, *prev = *headLine;
    if (*headLine != NULL) {
        temp = (*headLine)->next;
        while (temp != NULL) {
            deleteLinks(prev);
            free(prev->name);
            free(prev);
            prev = temp;
            temp = temp->next;
        }
        deleteLinks(prev);
        free(prev->name);
        free(prev);
        *headLine = NULL;
    }
}

/* deletes the given line. */
void deleteLine(Line *line, Line **headLine, Line **tailLine) {
    Line *temp = *headLine;
    if (line == *headLine) {
        *headLine = (*headLine)->next;
        free(temp->name);
        free(temp);
        return;
    }
    while (temp->next != line) {
        temp = temp->next;
    }
    if (line == *tailLine) *tailLine = temp;
    temp->next = line->next;
    free(line->name);
    free(line);
}

/* reads the arguments of command "r" and assigns them to their respective
variables. */
void inputr(char *input, Line *line) {
    char linename[MAXLEN];
    sscanf(input, "%*c %s", linename);
    line->name = myalloc((strlen(linename)+1)*sizeof(char));
    strcpy(line->name, linename);
}

/* reads the arguments of command "e" and assigns them to their respective
variables. */
void inpute(char *input, char *stopname) {
    if (numQuotes(input) == 0) {
        sscanf(input, "%*c %s", stopname);
    }
    else sscanf(input, "%*c \"%[^\"]\"", stopname);
}

/* receives a stop with only the stop name, and returns the corresponding stop 
from the list of stops. Returns NULL if a stop with that name doesn't exist. */
void* getStop(char *stopname, Stop **headStop) {
    Stop *temp = *headStop;
    while (temp != NULL) {
        if (strcmp(stopname, temp->name) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

/* subtracts the cost and duration of a link from the total cost and duration of 
the line, and reduces the number of stops of the line by one. */
void subFromLine(Line *line, Link *link) {
    line->cost -= link->cost;
    line->duration -= link->duration;
    line->stopnum -= 1;
}

/* updates the links of a line in order to delete the given stop. */
void updateLinks(Stop *stop, Line *line) {
    Link *link, *del;
    link = line->headLink;
    while (link != NULL) {
        /* if there is only one link. */
        if (line->headLink->next == NULL && (line->headLink->origin == stop
            ||line->headLink->destination == stop)) {
                free(line->headLink);
                line->headLink = NULL;
                line->tailLink = NULL;
                line->cost = 0;
                line->duration = 0;
                line->stopnum = 0;
                break;
            }
        /* if the stop deleted is the origin of the line. */
        else if (stop == line->headLink->origin) {
            del = line->headLink;
            line->headLink = del->next;
            line->headLink->prev = NULL;
            subFromLine(line, del);
            free(del);
            link = line->headLink;
        }
        /* if the stop deleted is the destination of the line. */
        else if (stop == line->tailLink->destination) {
            del = line->tailLink;
            line->tailLink = del->prev;
            line->tailLink->next = NULL;
            subFromLine(line, del);
            free(del);
        }
        else if (stop == link->destination) {
            del = link->next;
            link->destination = del->destination;
            link->next = del->next;
            /* if the deleted link is the last one. */
            if (del == line->tailLink) {
                line->tailLink = link;
                link->next = NULL;
            }
            else {
                link->next->prev = link;
            }
            link->cost += del->cost;
            link->duration += del->duration;
            line->stopnum -= 1;
            free(del);
        }
        else link = link->next;
    }
}

/* updates the links of all lines, removing the given stop. */
void delStopFromLines(Stop *stop, Line **headLine) {
    Line *line = *headLine;
    while (line != NULL) {
        updateLinks(stop, line);
        line = line->next;
    }
}

/* deletes the given stop. */
void deleteStop(Stop *stop, Stop **headStop, Stop **tailStop) {
    Stop *temp = *headStop;
    if (stop == *headStop) {
        *headStop = (*headStop)->next;
        free(stop->name);
        free(stop);
        return;
    }
    while (temp->next != stop) {
        temp = temp->next;
    }
    if (stop == *tailStop) *tailStop = temp;
    temp->next = stop->next;
    free(stop->name);
    free(stop);
}
