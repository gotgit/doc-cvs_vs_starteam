#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include "mystring.h"

int access_dir (char *file);
int process_file (char *file);
char *strencode (char *buf);

int
main (int argc, char *argv[])
{
	int i;

	if (argc < 2)
	{
		//access_dir("./");
		printf ("%s <filename> ... \n", argv[0]);
		return -1;
	}
	for (i = 1; i < argc; i++)
	{
		process_file (argv[i]);
	}
	return 0;
}


char *
strencode (char *buf)
{
	return strreplace (buf, "\"", "\\\"");
}


int
access_dir (char *dirname)
{
	DIR *dp;
	struct dirent *ep;
	char fullname[1024];
	struct stat filestat;


	dp = opendir (dirname);
	if (dp != NULL)
	{
		while (ep = readdir (dp))
		{
			if (!strcmp (ep->d_name, "."))
				continue;
			else if (!strcmp (ep->d_name, ".."))
				continue;

			strcpy (fullname, dirname);
			if (fullname[strlen (fullname) - 1] != '/')
				strcat (fullname, "/");
			strcat (fullname, ep->d_name);

			if (stat (fullname, &filestat))
				continue;
			if (S_ISDIR (filestat.st_mode))
			{
				printf ("%s is dir!\n", fullname);
				access_dir (fullname);
			}
			else
				process_file (fullname);
		}
		(void) closedir (dp);
	}
	else
		puts ("Couldn't open the directory.");

	return 0;
}

#define	STARTEAM_LOG_SEPRATOR	"----------------------------"
#define	STARTEAM_LOG_TERMINATOR	"============================================================================="
#define	STARTEAM_LOG_TAG_REV	"Revision: "
#define	STARTEAM_LOG_TAG_AUTHOR	"Author: "
#define	STARTEAM_LOG_TAG_DATE	"Date: "
#define	SUFFIX_LOG		".jxlog"
#define	SUFFIX_SH		".jxsh"
#define	MAX_NAME		512
#define	MAX_COMMENT		4096


int
process_file (char *filename)
{
	FILE *fpr, *fpw;
	char shell_name[2049];
	char source_file[2049];
	char buf[2049];
	char *p;
	int index = -1;
	int bLogBegin = 0;
	typedef struct
	{
		int revision;
		char author[MAX_NAME];
		char date[MAX_NAME];
		char comment[MAX_COMMENT];
	}
	rcs_t;

	rcs_t rcs;
	rcs_t rcslogs[256];

	if (strstr (filename, SUFFIX_LOG) == NULL)
	{
		printf ("can not process %s\n", filename);
		return 0;
	}
	if (strstr (filename, SUFFIX_SH) != NULL)
	{
		printf ("can not process %s\n", filename);
		return 0;
	}

	fpr = fopen (filename, "r");
	if (!fpr)
	{
		printf ("can not open file %s!\n", filename);
		exit (1);
	}

	while (fgets (buf, 2048, fpr))
	{
		while (buf[strlen (buf) - 1] == '\n' ||
			   buf[strlen (buf) - 1] == '\r')
		{
			buf[strlen (buf) - 1] = '\0';
		}
		if (!strncmp (buf, STARTEAM_LOG_SEPRATOR,
					  strlen (STARTEAM_LOG_SEPRATOR)) ||
			!strncmp (buf, STARTEAM_LOG_TERMINATOR,
					  strlen (STARTEAM_LOG_TERMINATOR)))
		{
			bLogBegin = 1;
			if (index >= 0)
			{
				memcpy (&rcslogs[index], &rcs, sizeof (rcs_t));
			}
			rcs.revision = 0;
			strcpy (rcs.author, "");
			strcpy (rcs.date, "");
			strcpy (rcs.comment, "");
			index++;
			continue;
		}
		if (!strncmp (buf, STARTEAM_LOG_TAG_REV,
					  strlen (STARTEAM_LOG_TAG_REV)))
		{
			rcs.revision = atoi (buf + strlen (STARTEAM_LOG_TAG_REV));
			continue;
		}
		if (!strncmp (buf, STARTEAM_LOG_TAG_AUTHOR,
					  strlen (STARTEAM_LOG_TAG_AUTHOR)))
		{
			p = (char *) strstr (buf, STARTEAM_LOG_TAG_DATE);
			if (p == NULL)
			{
				continue;
			}
			*(p - 1) = '\0';
			while (buf[strlen (buf) - 1] == '\n' ||
				   buf[strlen (buf) - 1] == '\r')
			{
				buf[strlen (buf) - 1] = '\0';
			}
			strncpy (rcs.author,
					 buf + strlen (STARTEAM_LOG_TAG_AUTHOR), MAX_NAME - 1);
			strncpy (rcs.date,
					 p + strlen (STARTEAM_LOG_TAG_DATE), MAX_NAME - 1);
			continue;
		}
		if (bLogBegin)
		{
			trim (buf);
			while (buf[strlen (buf) - 1] == '\n' ||
				   buf[strlen (buf) - 1] == '\r')
			{
				buf[strlen (buf) - 1] = '\0';
			}
			if (*buf)
			{
				strncat (rcs.comment, buf, MAX_COMMENT - 1);
			}
		}
	}
	fclose (fpr);
	index--;
	printf ("index:%d\n", index);

	strcpy (shell_name, filename);
	strcat (shell_name, SUFFIX_SH);

	if (*filename == '.' && *(filename + 1) == '/')
	{
		strcpy (source_file, filename + 2);
	}
	else
	{
		strcpy (source_file, filename);
	}
	source_file[strlen (source_file) - strlen (SUFFIX_LOG)] = '\0';


	fpw = fopen (shell_name, "w");


	bLogBegin = 1;
	for (; index >= 0; index--)
	{
		/*  
		   printf("revision:%d, author:%s, date:%s\n"
		   "comment:%s\nxxxxxxxxxxxxxxxx\n",
		   rcslogs[index].revision
		   ,rcslogs[index].author
		   ,rcslogs[index].date
		   ,rcslogs[index].comment
		   );
		 */
		fprintf (fpw, "st -qq co -lf -r %d %s\n",
				 rcslogs[index].revision, source_file);

		if (bLogBegin == 1)
		{
			bLogBegin = 0;
			fprintf (fpw, "cvs add %s\n", source_file);
		}
		fprintf (fpw, "cvs ci -m \"%s (Author:%s, Date:%s)\" %s\n",
				 strencode (rcslogs[index].comment),
				 strencode (rcslogs[index].author),
				 strencode (rcslogs[index].date), source_file);
	}

	fclose (fpw);
	return 0;

}
