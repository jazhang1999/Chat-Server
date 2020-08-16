
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.net.*;
import java.io.*;
import java.nio.Buffer;

public class ChatClientThread extends Thread {

    private Socket socket   = null;
    private JavaGUI client   = null;
    private BufferedReader streamIn = null;

    public ChatClientThread(JavaGUI _client, Socket _socket) {
        this.client   = _client;
        this.socket   = _socket;
        open();
        start();
    }

    public void open() {
        try {
            streamIn  = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        } catch(IOException ioe) {
            System.out.println("Error getting input stream: " + ioe);
            client.close();
        }
    }

    public void close() {
        try {
            if (streamIn != null)
                streamIn.close();
        } catch(IOException ioe) {
            System.out.println("Error closing input stream: " + ioe);
        }
    }

    public void run() {
        System.out.println("Got into run");
        while (true) {
            String received = "";
            try {
                while (streamIn.ready()) {
                    int ascii = streamIn.read();
                    received += (char)(ascii);
                    if (ascii == 0)
                        client.handle(received);
                }
            } catch(IOException ioe) {
                System.out.println("Listening error: " + ioe.getMessage());
                close();
                client.close();
            }
        }
    }
}