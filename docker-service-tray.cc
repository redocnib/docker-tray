//g++ docker-service-tray.cc -o docker-tray `pkg-config --cflags --libs gtk+-3.0 appindicator3-0.1` -lsystemd


#include <iostream>
#include <string>
#include <cctype>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <systemd/sd-bus.h>
#include <libgen.h>
# define TRAY_APPINDICATOR 1
#include "tray.h"


using namespace std;

static void docker_switch_db(struct tray_menu * item);
static void quit_cb(struct tray_menu * item);

void * sdbusHookServiceAsync(void * vargp);

static struct tray tray = {
  .menu =
  (struct tray_menu[]) {
    {
      .text = "Please Wait...", .cb = docker_switch_db
    },
    {
      .text = "Quit",
      .cb = quit_cb
    }, {
      .text = NULL
    }
  },
};

bool QUITTING = false; //Shared state for thread, no mutex for now

string TRAY_ICON1  = "/home/beep/Desktop/Projects/c/test1/active.png";
string TRAY_ICON2  = "/home/beep/Desktop/Projects/c/test1/inactive.png";
string TRAY_ICON3  = "/home/beep/Desktop/Projects/c/test1/unknown.png";


/* Application Entry Point */
int main() {
  char exePath[PATH_MAX];
    ssize_t len = ::readlink("/proc/self/exe", exePath, sizeof(exePath));
    if (len == -1 || len == sizeof(exePath))
        len = 0;
    exePath[len] = '\0';
    string base_path =  dirname(exePath);
    TRAY_ICON1 = base_path + "/icons/active.png";
    TRAY_ICON2 = base_path + "/icons/inactive.png";
    TRAY_ICON3 = base_path + "/icons/unknown.png";

    tray.icon = const_cast<char*>(TRAY_ICON3.c_str());

  if (tray_init( & tray) < 0) {
    printf("failed to create tray\n");
    return 1;
  }

  pthread_t thread_id;
  pthread_create( & thread_id, NULL, sdbusHookServiceAsync, NULL);
  while (tray_loop(1) == 0) {
    //Tray Updates
  }
  QUITTING = true;
  (void) pthread_join(thread_id, NULL);
  return 0;
}



/*
  Threaded sd_bus event lister
*/
void * sdbusHookServiceAsync(void * vargp) {
  sd_bus* bus = NULL;
  sd_bus_error err = SD_BUS_ERROR_NULL;
  char* msg = NULL;
  sd_bus_message* m = NULL;
  void* userdata = NULL;

  sd_bus_default_system(&bus);

  sd_bus_match_signal(
    bus, /* bus */
    NULL, /* slot */
    NULL, /* sender */
    "/org/freedesktop/systemd1/unit/docker_2eservice", /* path */
    "org.freedesktop.DBus.Properties", /* interface */
    "PropertiesChanged", /* member */
    NULL, //message_callback ,                      /* callback */
    userdata
  );

  while (!QUITTING) {
    m = NULL;
    int r = sd_bus_process(bus, &m);
    if (r < 0) {
      printf("Failed to process requests: %d", r);
      continue;
    }
    if (r == 0) {
      r = sd_bus_wait(bus, 1000000);
      if (r < 0) {
        printf("Failed to wait: %d", r);
      }
      continue;
    }
    if (!m) continue;
    sd_bus_get_property_string(
      bus, /* bus */
      "org.freedesktop.systemd1", /* destination */
      "/org/freedesktop/systemd1/unit/docker_2eservice", /* path */
      "org.freedesktop.systemd1.Unit", /* interface */
      "ActiveState", /* member */ &err, &msg);

    //printf("State Update From Service: %s\n", msg);
    
    if (strcmp(msg, "active") == 0) {
      tray.icon = const_cast<char*>(TRAY_ICON1.c_str());
      tray.menu[0].text = "Stop Docker Service";
      tray.menu[0].disabled = 0;
    } else if (strcmp(msg, "inactive") == 0) {
      tray.icon = const_cast<char*>(TRAY_ICON2.c_str());
      tray.menu[0].text = "Start Docker Service";
      tray.menu[0].disabled = 0;
    } else {
      tray.icon = const_cast<char*>(TRAY_ICON3.c_str());
      tray.menu[0].text = "Working...";
      tray.menu[0].disabled = 1;
    }

    tray_update( & tray);
    sd_bus_message_unref(m);
    free(msg);
  }
  sd_bus_error_free( & err);
  sd_bus_unref(bus);
  return NULL;
}


/*
   Tray Callbacks
*/

static void docker_switch_db(struct tray_menu * item) {
  (void) item;
  if (strcmp(tray.icon, const_cast<char*>(TRAY_ICON1.c_str())) == 0) {
    system("systemctl stop docker");
  } else {
    system("systemctl start docker");
  }
  tray_update( & tray);
}

static void quit_cb(struct tray_menu * item) {
  (void) item;
  tray_exit();
}

