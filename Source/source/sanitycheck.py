'''
Helper script to generate sanity-check files for existing NMEA data files. Check files use the same filenames with ".check" appended.
Based on standard as described in:
  https://www.gpsinformation.org/dale/nmea.htm#GGA
  http://aprs.gids.nl/nmea/#gga
Run as
   python sanitycheck.py <filename>
'''

import sys

def extract_lat_lon(csv):
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
        
    return la, lo


def extract_time(csv):
    time = csv[1].split(".")[0]  # ignore milliseconds
    return ":".join([time[:-4], time[-4:-2], time[-2:]])


def extract_altitude(csv):
    return csv[9]


def extract_geoid_height(csv):
    return csv[11]
    

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
    
    entries = []

    for l in lines:
        csv = l.split(",")
        line_entry = {}
        if csv[0] == "$GPGGA":  # only parse GGA sentences
            line_entry['latitude'], line_entry['longitude'] = extract_lat_lon(csv)
            line_entry['time'] = extract_time(csv)
            line_entry['altitude'] = extract_altitude(csv)
            line_entry['geoid_height'] = extract_geoid_height(csv)
            
            entries.append(line_entry)

    try:
        check_fn = fn + ".check"
        with open(check_fn, "w") as f:
            # Print out CSVs for each line:  [0] latitude, [1] longitude, [2] time (:-separated string), [3] altitude, [4] height (of geoid)
            for idx, entry in enumerate(entries):
                f.write(",".join([str(v) for v in [entry['latitude'], entry['longitude'], entry['time'], entry['altitude'], entry['geoid_height']]]) + "\n")
            print("\tWrote {} lines to file {} - finished.\n".format(idx, check_fn))
    except IOError as e:
        print("\tCould not write to file {} - aborting\n".format())
            

if __name__ == "__main__":
    main()
    
