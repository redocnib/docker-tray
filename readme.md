# Docker-Tray

Docker-Tray is a Linux (gtk) C++ application to monitor docker.service and allows to start and stop the service from system tray.

## Screenshots
![bar 1](https://user-images.githubusercontent.com/14855762/74221387-06442e80-4cd8-11ea-979b-98598d8dba52.png)
![Screenshot from 2020-02-11 14-11-18](https://user-images.githubusercontent.com/14855762/74221610-8b2f4800-4cd8-11ea-902d-52aad27ae968.png)
![image](https://user-images.githubusercontent.com/14855762/74221812-f37e2980-4cd8-11ea-9442-d5c073afb271.png)


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

## Autostart - run on startup
For debian systems add the following text to ```nano ~/.config/autostart/docker-tray.desktop``` 
replace <path to executable> with orginal path.
```
[Desktop Entry]
Name=Docker Tray
Comment=Test
Exec=<path to executable>/docker-tray
Type=Application
Terminal=false
Hidden=false
```
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
