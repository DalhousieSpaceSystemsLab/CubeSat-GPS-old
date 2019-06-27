'''
Helper script to generate sanity-check files for existing NMEA data files. Check files use the same filenames with ".check" appended.
Run as
   python sanitycheck.py <filename>
'''

import sys

def main():
    try:
        fn = sys.argv[1]
    except IndexError:  # no command-line arg given so use default
        fn = "nmea01.txt"
        
    print("\nTrying '{}' as NMEA data file to parse.. ".format(fn), end="")
    
    try:
        with open(fn) as f:
            data = f.read()
        print(" data parsed!\n")
    except FileNotFoundError as e:
        print(" could not open file {}\n".format(fn))
        return 0

    lines = data.split("\n")
    latitudes = []
    longitudes = []


    for l in lines:
        csv = l.split(",")
        if csv[0] == "$GPGGA":  # only parse GGA sentences
            try:
                la = int(csv[2][0:2]) + (float(csv[2][2:]) / 60.0)  # deg = degrees + minutes/60
                lo = int(csv[4][0:3]) + (float(csv[4][3:]) / 60.0)
            except ValueError as e:
                print(csv)
                print(l)
                raise e
                
            if csv[3] == "S":
                la *= -1.0
            if csv[5] == "W":
                lo *= -1.0
                
            latitudes.append(la)
            longitudes.append(lo)

    try:
        check_fn = fn + ".check"
        with open(check_fn, "w") as f:
            for idx, lat in enumerate(latitudes):
                f.write(str(latitudes[idx]) + ", " + str(longitudes[idx]) + "\n")
            print("\tWrote {} lines to file {} - finished.\n".format(idx, check_fn))
    except IOError as e:
        print("\tCould not write to file {} - aborting\n".format())
            

if __name__ == "__main__":
    main()
    
