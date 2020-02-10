# Docker-Tray

Docker-Tray is a Linux (gtk) C++ application to monitor docker.service and allows to start and stop the service from system tray.

## Execution

Running the application.

```bash
g++ docker-service-tray.cc -o docker-tray `pkg-config --cflags --libs gtk+-3.0 appindicator3-0.1` -lsystemd
./docker-tray
```

## Dependencies
```
gtk+-3.0
appindicator3-0.1
lsystemd
tray.h from https://github.com/zserge/tray
```
The application utalizes sd_bus for hooking service states.


## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

To do:
* Implement make, build and packing
* Enable/Disable service from tray.
* Enable/disable application on startup
* Refactor the code, implement proper testing.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
