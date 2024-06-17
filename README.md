# shrt

This is a URL shortener.
There are many like it, but this is mine.
Try putting all the vowels in "shrt" and enjoy yourself!

Also handles go-get redirects.

There's no UI to add a new shortlink at this time.
Just edit the DB by hand.

## Building

``` sh
make
```

## Installing

As root:

``` sh
make install
```

## Using

Shrt is a CGI script, so it expects certain environment variables to be
defined when envoked.
See RFC3875 for the CGI specification.
See the script files in the test subdirectory for simple use cases.

Shrt is customizable.
Just modify the values in config.h to suit your needs and rebuild.

Note that shrt expects its data file to be relative to the current
directory.

## Verifying Release Archives

Archives created prior to December 1, 2022 can be verified in one of
two ways:

1. Each .tar.gz archive is signed inline with signify(1) and can be
   verified (using the -z flag) against the public key located at
   https://www.danielmoch.com/static/djmoch-signify.pub

2. Using the .sig file corresponding to an archive, verification can
   be done with gpg(1) against the public key located at
   https://www.danielmoch.com/static/gpg.asc

Archives created on or after December 1, 2022 will only be signed with
`signify`, and not with `gpg`.
To support the use of `minisign`, which is compatable with `signify`,
detached signatures will be created.
These signature files are indicated with a `.minisig` extension.

## More Documentation

``` sh
man 1 shrt
```
