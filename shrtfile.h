/* See LICENSE file for copyright and license details */

struct keyval {
	char *key;
	char *value;
};

/*
 * The shrtfile struct contains the data read from a specially-formatted
 * file. The syntax of the file is human readable. Each line
 * represents a key-value pair. The key is everything to the left
 * of the first equals sign, and the value is everything to the
 * right. Whitespace is trimmed from the beginning and end of both
 * keys and values.
 */
struct shrtfile {
	size_t len;
	char *path;
	struct keyval *tbl;
};

/*
 * The shrtfile_read function reads an existing ShrtFile from the
 * filesystem and returns a pointer to a ShrtFile object. See the
 * ShrtFile documentation for the expected file format.
 */
struct shrtfile *shrtfile_read(const char *path);

void shrtfile_free(struct shrtfile *sf);

/*
 * The Get method gets the value of the specified key. If the key
 * does not exist, NULL is returned.
 */
char *shrtfile_get(const struct shrtfile *sf, const char *key);
