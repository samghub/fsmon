#ifndef _FSMON_H_
#define _FSMON_H_

#define FSMON_VERSION "1.2"

#include <stdint.h>
#include "fsev.h"
#include "util.h"

#define eprintf(x,y...) fprintf(stderr,x,##y)

#define FM_DEV "/dev/fsevents"
#define FM_BUFSIZE 4096

struct filemonitor_backend_t;
struct filemonitor_event_t;
struct filemonitor_t;

struct filemonitor_event_t {
	int pid;
	int ppid;
	const char *proc;
	const char *file;
	const char *newfile; // renamed/moved
	int uid;
	int gid;
	int type;
	int mode;
	uint32_t inode;
	uint64_t tstamp;
	int dev_major;
	int dev_minor;
};

typedef bool (*FileMonitorCallback)(struct filemonitor_t *fm, struct filemonitor_event_t *ev);

struct filemonitor_backend_t {
	const char *name;
	bool (*begin)(struct filemonitor_t *fm);
	bool (*loop)(struct filemonitor_t *fm, FileMonitorCallback cb);
	bool (*end)(struct filemonitor_t *fm);
};

struct filemonitor_t {
	const char *root;
	const char *proc;
	const char *link;
	int pid;
	int child;
	int alarm;
	int fd;
	bool json;
	volatile sig_atomic_t running;
	bool fileonly;
	uint64_t count;
	void (*control_c)();
	struct filemonitor_backend_t backend;
};

typedef struct filemonitor_backend_t FileMonitorBackend;
typedef struct filemonitor_event_t FileMonitorEvent;
typedef struct filemonitor_t FileMonitor;

#if __APPLE__
extern FileMonitorBackend fmb_devfsev;
extern FileMonitorBackend fmb_fsevapi;
extern FileMonitorBackend fmb_kqueue;
#else
extern FileMonitorBackend fmb_inotify;
#endif

#endif
