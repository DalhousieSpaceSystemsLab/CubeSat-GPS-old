# CubeSat GPS Interface
### Overview:
CubeSat GPS is an end-to-end interface for a NMEA 0183 GPS for the [DSS LORIS CubeSat Project](dalorbits.ca)

### Objectives:
- Parse and decode NMEA 0183 GPS sentences
- Provide geolocation data to the Altitude Control System and ground station

### Language:
- C++ 11

## Dependencies:
- Linux based Operating System
- C++11 Compiler
- [CMake](https://cmake.org)
- NMEA 0183 Compatible GPS 

### Target Operating System:
- Angstrom v2017.12

## Usage
```sh
cd Source/build
cmake ../source 
make
./Loris-GPS
```

## Contributing
Pull requests are welcome. Our feature list is located on [Trello](https://trello.com/b/jXkRj0IH/loris-payload). For major changes, please open a trello task first to discuss what you would like to change

## Special Thanks
[minmea](https://github.com/kosma/minmea) - a lightweight GPS NMEA 0183 Parser used in this repository

## License
[MIT](https://choosealicense.com/licenses/mit/)
