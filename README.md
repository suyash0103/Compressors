# Compressors
Compression algorithms
##LZW
### Compile
`g++ encoding.cpp -o encoding`
`g++ decoding.cpp -o decoding`
### Run
~~~~bash
# encode
./encoding ipfile encodedopfile codebook

# decode
./decoding encodedopfile opfile codebook

# Verify
diff ipfile opfile
~~~~