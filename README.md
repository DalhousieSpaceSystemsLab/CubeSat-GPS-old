# CubeSat GPS Interface

CubeSat GPS is an end-to-end interface for a NMEA 0183 GPS for the [DSS LORIS CubeSat Project](dalorbits.ca)

## Prerequisites

- C++11 Compiler
- [CMake](https://cmake.org)
- NMEA 0183 Compatible GPS 


## Usage

```sh
cd Source/build
cmake ../source 
make
./Loris-GPS
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## Special Thanks
[minmea](https://github.com/kosma/minmea) - a lightweight GPS NMEA 0183 Parser used in this repository

## License
[MIT](https://choosealicense.com/licenses/mit/)
