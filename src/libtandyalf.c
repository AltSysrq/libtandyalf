/*
  Copyright (c) 2012 Jason Lingle
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
  3. Neither the name of the author nor the names of its contributors
     may be used to endorse or promote products derived from this software
     without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGE.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define __USE_GNU
#include <dlfcn.h>

#define BUFFER_SIZE 256

static int (*c_execve)(const char*, char*const[], char*const[]) = 0;

static void __attribute__((constructor)) lookup_execve_syscall() {
  if (!c_execve)
    c_execve = dlsym(RTLD_NEXT, "execve");
}

static void remap_command(const char** path) {
  static const char prefix[] = "TANDYALF";
  char envname[256];
  char* replacement;
  unsigned i;
  strcpy(envname, prefix);
  strncat(envname, *path, BUFFER_SIZE - sizeof(prefix));
  /* Change slashes to underscores */
  for (i = 0; i < BUFFER_SIZE && envname[i]; ++i)
    if (envname[i] == '/')
      envname[i] = '_';

  /* See if there is a replacement */
  replacement = getenv(envname);
  if (replacement)
    *path = replacement;
}

extern int execve(const char* program, char*const args[], char*const env[]) {
  lookup_execve_syscall();
  remap_command(&program);
  return (*c_execve)(program, args, env);
}
