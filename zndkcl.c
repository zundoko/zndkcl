/**
 * @file     zndkcl.c
 * @brief    test app for a command line
 *
 * @note
 *
 * @date     2017-01-16
 * @author   zundoko
 */

#include <stdint.h>             /* uint32_t      */
#include <stdio.h>              /* printf()      */
#include <stdlib.h>             /* EXIT_SUCCESS  */
#include <string.h>             /* memset()      */
#include <unistd.h>             /* sleep()       */
#include <ctype.h>              /* isspace()     */

#include "zndkcl.h"             /* own header    */

/**
 * _trim_whitespaces()
 * @brief    trim leading/trailing white-spaces
 *
 * @param    [in]  *text       char ::= input command
 * @return         *begin      char ::= input command (white-spaces have been trimmed)
 */
char *
_trim_whitespaces(char *text)
{
    char   *begin;
    char   *end;

    if (text == NULL) {          /* no input data */
        return  text;
    }

    /* trim  leading spaces */
    begin = text;
    while (isspace((unsigned char)*begin)) {
        begin++;
    }

    /* trim trailing spaces */
    end =   begin + strlen(begin) - 1;
    while ((isspace((unsigned char)*end  )) &&
           (end > begin                   )    ) {
        end--;
    }
    *(end + 1) = '\0';          /* NULL termination */

    return  begin;
}

/**
 * _split()
 * @brief    split a string with a delimitter
 *
 * @param    [in]   buf        char ::= input command
 * @param    [in]  *delm       char ::= input command delimier
 * @param    [out] *args[]     char ::= splited by dilimiter
 * @return          idx             ::= # of extracted elements
 */
int
_split(char *buf, char *delim, char *args[])
{
    char   *tok  = NULL;
    int     idx  = 0;

    tok = strtok(buf, delim);
    while (tok != NULL) {
//        printf("args[%d]: %s\n", i, tok);
        args[idx] = tok;
        idx++;

        tok = strtok(NULL, delim);
    }

    return  idx;
}

/**
 * _parse_cmd_one_by_one()
 * @brief    parse a command one by one
 *
 * @param    [in]  *op         char ::= operand of the command
 * @param    [in]  *args[]     char ::= command elements (<op> <arg>[ <arg>])
 * @param    [in    nargs      int  ::= # of <arg>s in args
 * @retrun          stat       int  ::= process status
 */
int
_parse_cmd_one_by_one(char *op, char *args[], int nargs)
{
    int     stat = 0;

    if (strcmp(op, "m") == 0) { /* m <msgid> <msgsz>[ <data>[ <data>]] */
        if (nargs > 1) {
            uint32_t  msgid  = strtol(args[1], NULL, 16);
            uint32_t  msgsz  = atoi(args[2]);
            uint32_t  data;
            int       nword  = msgsz / sizeof(uint32_t);
            int       idx_wd;
            printf(" msgid   =    0x%05X\n", msgid);
            printf(" msgsz   =      %5d\n" , msgsz);
            if (nword > nargs - 2) { /* -2: msgid + msgsz */
                stat  = -1;
                printf(" args too short: nword=%d, but # of data=%d\n", nword, nargs - 2);
            } else {
                for (idx_wd = 0; idx_wd < nword; idx_wd++) {
                    data = strtol(args[3 + idx_wd], NULL, 16);
                    printf(" data[%d] = 0x%08X\n", idx_wd, data);
                }
            }
        } else {
            stat = -2;
            printf(" args too short: %d\n", nargs);
        }
    } else{
        printf(" %s():[L%4d]: unknown command detected\n", __func__, __LINE__);
    }

    return  stat;
}

/**
 * _load_cmd_file()
 * @brief    load commands list
 *
 * @param    [in]  *filepath   char ::= path to command file
 * @param    [in]  *delim      char ::= input command delimier
 * @return          stat       int  ::= process status
 */
int _parse_cmd(char *buf, char *delim, char *args[]); /* prototype declaration for recursion */
int
_load_cmd_file(char *filepath, char *delim)
{
    int      stat   = 0;
    FILE    *fp;
    char    *text   = NULL;
    size_t   len;
    ssize_t  nbytes;
    int      line    = 0;
    char    *args[N_CMDLINE_ARGS];

    fp = fopen(filepath, "r");
    if (fp == NULL) {
        perror("fopen");
        return -1;
    }
    while ((nbytes = getline(&text, &len, fp)) != -1) {
        text[strcspn(text, "\r\n")] = 0; /* remove CR, LF */
        printf("  L%4d(%4ld[B], len=%ld): %s, %2X\n", ++line, nbytes, len, text, text[0]);
        _parse_cmd(text, delim, args);
    }

    fclose(fp);
    if (text) {
        free(text);
    }

    return  stat;
}

/**
 * _parse_cmd()
 * @brief    parses a input command (recursive function)
 *
 * @param    [in]   buf        char ::= input command  (<op> <arg>[<delim><arg>])
 * @param    [in]  *delim      char ::= input command delimier
 * @param    [out] *args[]     char ::= split w/ dilimiter ' '
 * @return          stat            ::= process status
 */
int
_parse_cmd(char *buf, char *delim, char *args[])
{
    int     stat = 0;
//    int     idx;
    int     nele;               /* # of elements */

    /* trim leading/trailing whitespaces: "  op a1 a2 a3 " -> "op a1 a2 a3" */
    buf = _trim_whitespaces(buf);
//    printf(" cmd : %s, delim=\"%s\"\n", buf, delim);

    /* split a buf w/ delim             : "op a1 a2 a3" -> "op", "a1", "a2", "a3" */
    nele = _split(buf, delim, args);
//    printf(" args:");
//    for (idx = 0; idx < nele; idx++) {
//        printf(" %s", args[idx]);
//    }
//    printf("\n");

    {
        char  *op  = args[0];
        if        (strcmp(op, "\0") == 0) { /* NULL ? (new line ?)   */
            /* DO NOTHING. */
        } else if (strcmp(op, "#" ) == 0) { /* # is a comment line.  */
            /* DO NOTHING. */
        } else if (strcmp(op, "f" ) == 0) { /* f <path_to_cmds_file> */
            if (nele > 1) {
                char *filepath = args[1];
                printf(" ----- loads command file start: %s -----\n", filepath);
                _load_cmd_file(filepath, delim);
                printf(" ----- loads command file end  : %s -----\n", filepath);
            } else {
                printf(" args too short: %s\n", buf);
            }
        } else {
            stat = _parse_cmd_one_by_one(op, args, nele - 1);
            if (stat < 0) {
                printf(" %s():[L%4d]: invalid command, %s (%d)\n", __func__, __LINE__, buf, stat);
            }
        }
    }

    return  stat;
}

/**
 * _get_line()
 * @brief    read a user input command (stdin)
 *
 * @param    [in]  *prompt     char ::= command prompt
 * @param    [out} *buf        char ::= command buffer
 * @param    [in]   len        int  ::= length of command buffer
 * @return          stat            ::= process status
 */
int
_get_line(char *prompt, char *buf, int len)
{
    int     stat = 0;

    if (prompt == NULL) {
        printf("> ");
    } else {
        printf("%s", prompt);
    }
    fflush(stdout);

    if (fgets(buf, len, stdin) == NULL) {
        printf("\nno input command found\n");
    }
    buf[ strlen(buf) - 1 ] = '\0'; /* NULL termination */

    if (strlen(buf) == 0) {
        stat = -1;
    }

    return  stat;
}

/**
 * main()
 * @brief    main routine for mq tx
 *
 * @param    [in]   argc        int ::= # of args
 * @param    [in]  *argv[]     char ::= entity of args
 * @return          stat            ::= process termination (EXIT_)
 */
int
main(int argc, char *argv[])
{
    int   stat                 = EXIT_SUCCESS;
    int   is_run               = TRUE;
    char  buf[LEN_CMDLINE    ];
    char *args[N_CMDLINE_ARGS];

    printf("=== zndkcmdline start ===\n");
    printf("type \'?\' for help\n");

    while (is_run == TRUE) {
        /* parse a input command (stdin) */
        stat     = _get_line("> ", buf, LEN_CMDLINE);
        if (stat <  0) {
            /* DO NOTHING */
        } else {
            if        (strcmp(buf, "q") == 0) {
                printf(" quit\n");
                is_run = FALSE;
                break;
            } else if (strcmp(buf, "?") == 0) {
                printf(" Usage: <op> <arg>[ <arg>]\n");
                printf("\n");
                printf("  op:\n");
                printf("   f FILE       loads FILE.\n");
                printf("   m <msgid> <msgsz>[ <data>[ <data>]]\n");
                printf("\n");
                continue;
            }
            stat     = _parse_cmd(buf, " ", args); /* " ": input command delimiter */
            if (stat <  0) {
                printf(" %s():[L%4d]: invalid command, %s\n", __func__, __LINE__, buf);
                break;
            }
        }
    }

    printf("=== zndkcmdline end   ===\n");

    return  stat;
}

/* end */
