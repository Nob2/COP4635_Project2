Group Members:
    - Nicholas Brown-Duncan

Compilation instructions:
    - After unzipping the file, run the following command(s)
    - make main
    - If you wish to use client, also run
        - make client

Running program:
    - After running ./main the server will begin to listen for requests
    - To use browser:
        - Go to localhost:60001
            - Both ports 8080, 46000, 60001 were tested to work in a Linux subsystem for Windows environment
            - Alongside Microsoft Edge and Google Chrome browsers for both Linux and Windows OS.
        - Once you reach main page, click on the links as desired
            - The server will respond when it receives and fulfills a request
            - If the client sends an invalid file to the server, it will say "Failed to open file: [fileName] for client"
    - Using the client application, run it as follows
        - ./client [FileName]
        - Where FileName is the name of the file you wish to receive from the server
            - If the file does not exist, the server will respond "File does not exist"
            - Else it will send the contents of the file, which will be printed to the client's screen
    - This program has been tested on the SSH server (utilizing client program)