#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mystring.h"


/*!
 * \fn		strtolower
 * \brief	string to lower
 * \param	char *str
 * \return	char *
 * \author	Jiang Xin
 * \date	2002.07.08
 */
char *
strtolower (char *str)
{
	char *p;

	p = str;
	while (*p)
	{
		*p = tolower (*p);
		p++;
	}
	return str;
}


/*!
 * \fn		strtoupper
 * \brief	string to upper
 * \param	char *str
 * \return	char *
 * \author	Jiang Xin
 * \date	2002.07.08
 */
char *
strtoupper (char *str)
{
	char *p;

	p = str;
	while (*p)
	{
		*p = toupper (*p);
		p++;
	}
	return str;
}


/*!
 * \fn		ltrim
 * \brief	strip left space of a string
 * \param	char *str
 * \return	char *
 * \author	Jiang Xin
 * \date	2002.07.08
 */
char *
ltrim (char *str)
{
	char *p, *q;

	p = str;
	q = str;
	while (*q == ' ' || *q == '\t')
		q++;
	if (p != q)
	{
		for (; *q; p++, q++)
		{
			*p = *q;
		}
		*p = 0;
	}
	return str;
}


/*!
 * \fn		rtrimex
 * \brief	strip right space of a string
 * \param	char *str
 * \param	char *spacechars	space defination
 * \return	char *
 * \author	Yang Zhiwei
 * \date	2001
 */
char *
rtrimex (char *str, char *spacechars)
{
	char *s = str;
	char *p = str;				//the place may be the end of str of trimed

	if (!s)
		return str;
	if (!*s)
		return str;

	// if first char of s is space,it may be the end
	// if ( strchr(spacechars,*s) ) p=s; 

	for (; *s; s++)
	{
		// if ( s[0]!=' '&&strchr(spacechars,s[1]) ) 
		if (strchr (spacechars, *s) == NULL)
			p = s + 1;
	}
	*p = 0;
	return str;
}


/*!
 * \fn		rtrim
 * \brief	strip right space of a string. Space include space and tab
 * \param	char *str
 * \return	char *
 * \author	Yang Zhiwei
 * \date	2001
 */
char *
rtrim (char *str)
{
	return rtrimex (str, " \t");
}


/*!
 * \fn		trim
 * \brief	strip both left and right space of a string
 * \param	char *str
 * \return	char *
 * \author	Jiang Xin
 * \date	2002.07.08
 */
char *
trim (char *str)
{
	str = ltrim (str);
	str = rtrim (str);
	return str;
}


/*!
 * \fn		strreplace
 * \brief	substitute "from" to "to" in string "buf"
 * \param	char *buf, char *from, char *to
 * \return	char *
 * \author	Jiang Xin
 * \date	2003.04.09
 */
char *
strreplace (char *buf, char *from, char *to)
{
	int len = strlen (buf) + 1;
	char *ret = (char *) malloc (len);
	char *p = NULL, *q = NULL, *r;

	if (!ret)
	{
		printf ("malloc failed!\n");
		exit (1);
	}

	strcpy (ret, buf);
	p = ret;
	q = buf;

	while (r = strstr (q, from))
	{
		len += strlen (to) - strlen (from);
		realloc (ret, len);
		p = ret + (r - q);
		strcpy (p, to);
		p += strlen (to);
		r += strlen (from);
		strcpy (p, r);
		q = r;
	}

	return ret;
}


#ifdef	JXCOMMENT_TEST

int
test_replace (int argc, char *argv[])
{
	char *p;
	char *to, *from, *buf;


	buf = "my name are johnson";
	from = "are";
	to = "is";

	switch (argc)
	{
	case 2:
		to = argv[1];
		break;
	case 3:
		from = argv[1];
		to = argv[2];
		break;
	case 4:
		buf = argv[1];
		from = argv[2];
		to = argv[3];
		break;
	default:
		buf = "my name are johnson";
		from = "are";
		to = "...is...";
		break;
	}

	if (argc == 2)
	{
		to = argv[1];
	}
	p = strreplace (buf, from, to);
	printf ("%s\n", p);
}

int
test_trim (int ac, char *av[])
{
#define	MYSTRING_TEST_BUF	"    This is UPPER. this is lower. !!!  "
	register i;
	char buf[] = MYSTRING_TEST_BUF;

	strcpy (buf, MYSTRING_TEST_BUF);
	printf ("strtolower: \n\t%s.\n", strtolower (buf));

	strcpy (buf, MYSTRING_TEST_BUF);
	printf ("strtoupper: \n\t%s.\n", strtoupper (buf));

	strcpy (buf, MYSTRING_TEST_BUF);
	printf ("ltrim: \n\t>%s<\n", ltrim (buf));

	strcpy (buf, MYSTRING_TEST_BUF);
	printf ("rtrim: \n\t>%s<\n", rtrim (buf));

	strcpy (buf, MYSTRING_TEST_BUF);
	printf ("trim: \n\t>%s<\n", trim (buf));

	strcpy (buf, "	  	");
	printf ("ltrim: \n\t>%s<\n", ltrim (buf));

	strcpy (buf, "	  	");
	printf ("rtrim: \n\t>%s<\n", rtrim (buf));

	return 0;
}



static struct functab_t
{
	char *name;
	int (*func) ();
}
functab[] =
{
	{
	"trim", test_trim}
	,
	{
	"replace", test_replace}
};

#define FUNCTABCOUNT sizeof(functab)/sizeof(*functab)

int
main (int ac, char *av[])
{
	int i;

	if (ac > 1)
	{

		for (i = 0; i < FUNCTABCOUNT; i++)
		{
			if (!strcmp (av[1], functab[i].name))
			{
				return functab[i].func (ac - 1, av + 1);
			}
		}
	}

	puts ("Usage:");
	for (i = 0; i < FUNCTABCOUNT; i++)
	{
		printf ("\t%s %s\n", av[0], functab[i].name);
	}
	return 0;
}

#endif
