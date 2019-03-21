# LocalMap
## Compile
`g++ encoding.cpp -o encoding` <br/>
`g++ decoding.cpp -o decoding`
## Run
~~~~bash
# generate tokens
./tonkenizer ipfile tokens

# encode
./encoding ipfile encodedopfile dictionary

# decode
./decoding encodedopfile opfile dictionary

# dictionary is list of words in language separated by \n

# Verify
diff ipfile opfile
~~~~
