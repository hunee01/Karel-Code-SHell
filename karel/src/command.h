#ifndef COMMAND_H
#define COMMAND_H

void command_init(void);
void help(void);
void echo(const char *args);
void pwd(const char *cwd);
void cd(const char *dir);
void rm(const char *delMode,const char *file);
void c_mkdir(const char *dir);
void touch(const char *file);
void rd(const char *file);
void whoami(void);

#endif
