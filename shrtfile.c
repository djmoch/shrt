/* See LICENSE file for copyright and license details */
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shrtfile.h"

static int line2kv(struct keyval *dst, char *line);
static char *trim(char *str);

struct shrtfile *
shrtfile_read(const char *path)
{
	struct shrtfile *rv = NULL;
	FILE *fp = NULL;
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	struct keyval *keyvals = NULL;

	if ((rv = malloc(sizeof(struct shrtfile))) == NULL) {
		warn("failed to malloc shrtfile");
		goto errshrtfile;
	}
	memset(rv, 0, sizeof(struct shrtfile));
	if ((rv->path = strndup(path, NAME_MAX)) == NULL) {
		warn("failed to strndup shrtfile->path");
		goto errshrtfile;
	}
	if((fp = fopen(path, "r")) == NULL) {
		warn("fopen failed in shrtfile_read");
		goto errshrtfile;
	}

	errno = 0;
	while ((nread = getline(&line, &len, fp)) != -1) {
		rv->len += 1;
		if ((keyvals = realloc(keyvals, sizeof(struct keyval) * rv->len)) == NULL) {
			warn("failed to realloc shrtfile->tbl");
			goto errshrtfile;
		}
		memset(&keyvals[rv->len - 1], 0, sizeof(struct keyval));
		if(!line2kv(&keyvals[rv->len - 1], line)) {
			warnx("shrtfile contains ill-formatted line: %s", line);
		}
	}
	if (errno) {
		warn("getline failed in shrtfile_read");
		goto errshrtfile;
	}
	free(line);

	rv->tbl = keyvals;
	fclose(fp);
	return rv;

errshrtfile:
	if (line) free(line);
	if (keyvals) free(keyvals);
	if (fp) fclose(fp);
	if (rv) free(rv);
	return NULL;
}

void
shrtfile_free(struct shrtfile *sf)
{
	if (sf->tbl) {
		free(sf->tbl);
	}
	free(sf);
}

char *
shrtfile_get(const struct shrtfile *sf, const char *key)
{
	for (size_t i=0; i<sf->len; i++) {
		if (strcmp(key, sf->tbl[i].key) == 0) {
			return sf->tbl[i].value;
		}
	}
	return NULL;
}

static int
line2kv(struct keyval *dst, char *line)
{
	char *key = NULL, *value = NULL;

	if ((key = strtok(line, "=")) == NULL) {
		warnx("strtok could not find equal sign");
		return 0;
	}
	if ((value = strtok(NULL, "\0")) == NULL) {
		warnx("strtok could not find null terminator");
		return 0;
	}
	dst->key = trim(strdup(key));
	dst->value = trim(strdup(value));
	return 1;
}

static char *
trim(char *str)
{
	while (isspace((unsigned char)*str)) {
		memmove(str, str + 1, strlen(str) + 1);
	}

	while (isspace((unsigned char)*(str + strlen(str) - 1))) {
		str[strlen(str) - 1] = '\0';
	}
	return str;
}
