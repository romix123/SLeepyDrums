///////////////////////////////   SETTING VALUE   ///////////////////////////////////

//Determine the setting value.
//By changing the number in this array you can set sensitivity, threshold and so on.

byte SNARE[6] = {
    100, //sensitivity (1-100)
    10,  //threshold (1-100)
    10,  //scan time (1-)
    30,  //mask time (1-)
    38,  //note (0-127)
    1    //curve type (0-4)
};

byte TOM[6] = {
    100, //sensitivity (1-100)
    10,  //threshold (1-100)
    10,  //scan time (1-)
    30,  //mask time(1-)
    50,  //note (0-127)
    1    //curve type(0-4)
};

byte FLOOR[6] = {
    100, //sensitivity (1-100)
    10,  //threshold (1-100)
    10,  //scan time (1-)
    30,  //mask time (1-)
    41,  //note (0-127)
    1    //curve type (0-4)
};

byte KICK[6] = {
    100, //sensitivity (1-100)
    10,  //threshold (1-100)
    10,  //scan time (1-)
    30,  //mask time(1-)
    36,  //note (0-127)
    1    //curve type(0-4)
};

byte CRASH[6] = {
    100, //sensitivity (1-100)
    10,  //threshold (1-100)
    10,  //scan time (1-)
    30,  //mask time (1-)
    49,  //note (0-127)
    1    //curve type (0-4)
};

byte CYMBAL[6] = {
    100, //sensitivity (1-100)
    10,  //threshold (1-100)
    10,  //scan time (1-)
    30,  //mask time(1-)
    48,  //note (0-127)
    1    //curve type(0-4)
};
