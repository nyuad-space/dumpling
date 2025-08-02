// event flow logic

<flash_readwrite.cpp>
TODO: add logic for after running out of storage for regular logging
v- Question: can i merge getSensorFilename into getSensorFileInfo? Also, why is that function not used anywhere?

<main.cpp>
// TODO: LED indication for circular/ regular logging etc.
// TODO: consider edge cases
// TODO: separate project for reading from flash 
v- // TODO: for attachInterrupt, instead of CHANGE, stick to either falling/rising edge
// TODO: circular is only writing header and not data. check write to circular logic again

// TODO: how to make sure flash is clean state before logging data

<new>
// third status log file
// F411 LED for circular/regular 
// fix HDC302 reading zeros
