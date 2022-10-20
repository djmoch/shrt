/* See LICENSE file for copyright and license details */
#include <err.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "shrtfile.h"

static const char *dbpath = "shrt.db";
static const char *srvname = "djmo.ch";
static const char *scmtype = "git";
static const char *suffix = ".git";
static const char *rdrname = "git://git.danielmoch.com";
static const char *barerdr = "https://www.danielmoch.com";
static char *empty_path = "";

struct query_param
{
	char *key;
	char *value;
};

struct cgi_req
{
	char *path_info;
	char *request_method;
	struct query_param *query_params;
};

enum
{
	EX_ARGS = 1,
	EX_SHRTFILE
};

static void
usage(const char *arg0, const int rv)
{
	FILE *fp;
	if (rv) {
		fp = stderr;
	} else {
		fp = stdout;
	}

	fprintf(fp, "usage: %s [-hv]\n", arg0);
	exit(rv);
}

static void
print_version(const char *arg0)
{
	printf("%s version %s\n", arg0, VERSION);
	exit(EXIT_SUCCESS);
}

void
free_request(struct cgi_req *req)
{
	if (req->query_params) {
		free(req->query_params);
	}
	free(req);
}

struct cgi_req *
parse_request()
{
	struct cgi_req *req;
	char *query_string;
	char *qkey;
	size_t pnum = 0;

	if ((req = malloc(sizeof(struct cgi_req))) == NULL) {
		err(errno, "malloc");
	}
	memset(req, 0, sizeof(struct cgi_req));

	if ((req->path_info = getenv("PATH_INFO")) == NULL) {
		req->path_info = empty_path;
	}
	if ((req->request_method = getenv("REQUEST_METHOD")) == NULL) {
		warnx("REQUEST_METHOD not found in environment");
		goto reqerror;
	}
	if ((query_string = getenv("QUERY_STRING")) == NULL) {
		warnx("QUERY_STRING not found in environment");
		goto reqerror;
	}

	while ((qkey = strtok(query_string, "=")) != NULL) {
		if ((req->query_params =
				realloc(req->query_params, sizeof(struct query_param) * (pnum+1)))
				== NULL) {
			err(errno, "realloc");
		}
		req->query_params[pnum].key = qkey;
		if ((req->query_params[pnum].value = strtok(NULL, ";")) == NULL) {
			errx(1, "");
		}
		pnum += 1;
	}

	return req;

reqerror:
	free_request(req);
	return NULL;
}

static void
handl(struct cgi_req *req, struct shrtfile *shrt)
{
	char *val;

	if (strcasecmp(req->request_method, "GET") != 0) {
		puts("Content-Type: text/plain; charset=utf-8");
		puts("Status: 405 Method Not Allowed\n");
		puts("Method not allowed");
		return;
	}

	if (req->path_info[0] == '/') {
		memmove(req->path_info, &req->path_info[1], strlen(req->path_info) + 1);
	}

	if (strcmp(req->path_info, "robots.txt") == 0) {
		puts("Content-Type: text/plain; charset=utf-8\n");
		puts("# Welcome to Shrt");
		puts("User-Agent: *");
		puts("Disallow:");
		return;
	}

	if (strcmp(req->path_info, "") == 0 && strcmp(barerdr, "") != 0) {
		puts("Status: 302 Found");
		printf("Location: %s\n\n", barerdr);
		return;
	}

	if (index(req->path_info, '/') == NULL) {
		if ((val = shrtfile_get(shrt, req->path_info)) != NULL) {
			puts("Status: 301 Moved Permanently");
			printf("Location: %s\n\n", val);
			puts("Redirecting");
			return;
		}
	}

	val = strtok(req->path_info, "/");
	puts("Content-Type: text/html; charset=utf-8\n");
	puts("<!DOCTYPE html>");
	puts("<html>");
	puts("<head>");
	puts("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>");
	printf("<meta name=\"go-import\" content=\"%s/%s %s %s/%s%s\">\n",
		srvname, val, scmtype, rdrname, val, suffix);
	printf("<meta http-equiv=\"refresh\" content=\"0; url=https://godoc.org/%s/%s\">\n",
		srvname, val);
	puts("</head>");
	puts("<body>");
	printf("Redirecting to docs at <a href=\"https://godoc.org/%s/%s\">godoc.org/%s/%s</a>...\n",
		srvname, val, srvname, val);
	puts("</body>");
	puts("</html>");
	return;
}

int
main(int argc, char **argv)
{
	int opt;
	struct cgi_req *req = NULL;

	if (argc > 2) {
		usage(argv[0], EX_ARGS);
	}

	while ((opt = getopt(argc, argv, "hv")) != -1) {
		switch (opt) {
		case 'h':
			usage(argv[0], EXIT_SUCCESS);
		case 'v':
			print_version(argv[0]);
		default:
			fprintf(stderr, "%s: unknown option -- %d\n", argv[0], opt);
			usage(argv[0], EX_ARGS);
		}
	}

#ifdef __OpenBSD__
	unveil(dbpath, "r");
	pledge("stdio rpath", "");
#endif

	struct shrtfile *shrt = shrtfile_read(dbpath);
	if (NULL == shrt) {
		err(EX_SHRTFILE, "db error");
	}

	if ((req = parse_request()) != NULL) {
		handl(req, shrt);
		free_request(req);
	}
	shrtfile_free(shrt);
}
