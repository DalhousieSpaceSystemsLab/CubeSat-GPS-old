#include "Testing.hpp"





bool compare_floats(float a, float b) {
    float epsilon = 1.0e-4;
    return std::abs(a - b) <= epsilon;
}


float parse_token_float(string line, string separator, unsigned int index) {
    float token;
    string::size_type start = 0;
    string::size_type end;
    
    for(int i=0; i<=index; i++) {
        end = line.find(separator, start);
        token = stof(line.substr(start, end-start));
        start = end + 1;
    }
    
    return token;
}



// test that decode() works as expected
bool test_decode(bool verbose) {
    string paragraph, check_line;
    gps_data decoded_data;
    float lat, lon, alt, hgt;
    bool success = true;
    bool line_success = false;
    
    reset_datafile();
    reset_checkfile();
    
    while(poll(&paragraph)) {
        decoded_data = decode(paragraph);
        if(read_sanitycheck_from_file(&check_line)) {
            lat = decoded_data.latitude;
            lon = decoded_data.longitude;
            alt = decoded_data.altitude;
            hgt = decoded_data.height;  // geoid height
            
            if(verbose) {
                cout << INDENT_SPACES << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
                cout << INDENT_SPACES << INDENT_SPACES << "Decoded      (LAT,LONG): " << lat << ", " << lon << endl;
                cout << INDENT_SPACES << INDENT_SPACES << "Decoded      (ALT,HEIGHT): " << alt << ", " << hgt << endl;
                cout << INDENT_SPACES << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
                cout << INDENT_SPACES << INDENT_SPACES << "Sanity check (LAT,LONG,TIME,ALT,HEIGHT): " + check_line << endl;
                cout << INDENT_SPACES << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl << endl;
            }
            
            line_success = compare_floats(lat, parse_token_float(check_line, ",", 0)) && compare_floats(lon, parse_token_float(check_line, ",", 1)); // lat and long
            line_success = line_success && compare_floats(alt, parse_token_float(check_line, ",", 3)) && compare_floats(hgt, parse_token_float(check_line, ",", 4)); // alt and height
            if(verbose) {
                if(line_success)
                    cout << INDENT_SPACES << " ^ Passed ^ " << endl << endl;
                else
                    cout << INDENT_SPACES << " ^ FAILED ^ " << endl << endl;
            }
        }
        
        success = success && line_success;
    }
    return success;
}


// test that build_message() works as expected
bool test_build_message(bool verbose) {
    string paragraph, check_line;
    gps_data decoded_data;
    Message message;
    float lat, lon, alt, hgt;
    bool success = true;
    bool line_success = false;
    
    reset_datafile();
    reset_checkfile();
    
    while(poll(&paragraph)) {
        decoded_data = decode(paragraph);
        build_message(decoded_data, &message);
        
        //char msg[256] = "";
	    //message.flatten(msg);
	    //message = Message(msg);
        
        if(read_sanitycheck_from_file(&check_line)) {
            lat = message.GetMessageContents().GetFloat(data_types.latitude);
            lon = message.GetMessageContents().GetFloat(data_types.longitude);
            
            if(verbose) {
                cout << INDENT_SPACES << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
                cout << INDENT_SPACES << INDENT_SPACES << "Built Message (LAT,LONG): " << lat << ", " << lon << endl;
                cout << INDENT_SPACES << INDENT_SPACES << "Built Message (ALT,HEIGHT): " << alt << ", " << hgt << endl;
                cout << INDENT_SPACES << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
                cout << INDENT_SPACES << INDENT_SPACES << "Sanity check  (LAT,LONG,TIME,ALT,HEIGHT): " + check_line << endl;
                cout << INDENT_SPACES << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
            }
            
            line_success = compare_floats(lat, parse_token_float(check_line, ",", 0)) && compare_floats(lon, parse_token_float(check_line, ",", 1)); // lat and long
            line_success = line_success && compare_floats(alt, parse_token_float(check_line, ",", 3)) && compare_floats(hgt, parse_token_float(check_line, ",", 4)); // alt and height
            if(verbose) {
                if(line_success)
                    cout << INDENT_SPACES << "^ Passed ^ " << endl << endl;
                else
                    cout << INDENT_SPACES << "^ FAILED ^ " << endl << endl;
            }
        }
            
        success = success && line_success;
    }
    
    return success;
}




void print_test_summary(string label, bool success) {

    string SEPARATOR = "== - = - = - = - = - = - = - = - = - = - = - = - = - = - = - ==";
    
    cout << endl << SEPARATOR << endl;
    
    cout << "  " << label << ":  ";
    if(success)
        cout << "PASSED";
    else
        cout << "FAILED";
        
    cout << endl << SEPARATOR << endl;
}
