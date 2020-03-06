#ifndef _CHARSET_H_
#define _CHARSET_H_


bool ansi2unicode(const char *inbuf, char **outbuf);
bool unicode2ansi( const char *inbuf, char **outbuf);
bool utf82unicode( const char *inbuf, char **outbuf);
bool unicode2utf8( const char *inbuf, char **outbuf);

bool ansi2utf8( const char *inbuf, char **outbuf);
bool utf82ansi( const char *inbuf, char **outbuf);

#endif
